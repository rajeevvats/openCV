// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#include "../precomp.hpp"
#include "cpu_kernels/fast_gemm.hpp"

#include <opencv2/dnn/shape_utils.hpp>

namespace cv { namespace dnn {

static void packWeight(size_t num_heads, size_t head_size, size_t input_hidden_size,
                       const float *weight_data, size_t hidden_size, std::vector<float> &packed_weight, const FastGemmOpt &opt) {
    // num_heads * pack(head_size, input_hidden_size)
    size_t pack_size = fastGemmPackBSize(head_size, input_hidden_size, opt);
    size_t packed_weight_size = num_heads * pack_size;
    packed_weight.resize(packed_weight_size, 0.f);
    auto *packed_weight_data = packed_weight.data();
    for (size_t i = 0; i < num_heads; i++) {
        fastGemmPackB(false, head_size, input_hidden_size, weight_data, hidden_size, packed_weight_data, opt);
        packed_weight_data += pack_size;
        weight_data += head_size;
    }
}

// Operator spec: https://github.com/microsoft/onnxruntime/blob/v1.16.1/docs/ContribOperators.md#com.microsoft.Attention
class AttentionLayerImpl CV_FINAL : public AttentionLayer {
 public:
    AttentionLayerImpl(const LayerParams &params) {
        setParamsFrom(params);

        CV_CheckTrue(params.has("num_heads"), "DNN/Attention: num_heads is required but missing");
        num_heads = params.get<int>("num_heads"); // required, no default value

        CV_CheckTrue(params.has("qkv_hidden_sizes"), "DNN/Attention: qkv_hidden_sizes is required but missing");
        auto param_qkv_hidden_sizes = params.get("qkv_hidden_sizes");
        CV_CheckEQ(param_qkv_hidden_sizes.size(), 3, "DNN/Attention: qkv_hidden_sizes must and only have three elements");
        qkv_hidden_sizes.resize(3);
        qkv_hidden_sizes[0] = static_cast<size_t>(param_qkv_hidden_sizes.get<int>(0));
        qkv_hidden_sizes[1] = static_cast<size_t>(param_qkv_hidden_sizes.get<int>(1));
        qkv_hidden_sizes[2] = static_cast<size_t>(param_qkv_hidden_sizes.get<int>(2));

        hidden_size = qkv_hidden_sizes[0] + qkv_hidden_sizes[1] + qkv_hidden_sizes[2];

        qkv_head_sizes.resize(3);
        std::transform(qkv_hidden_sizes.begin(), qkv_hidden_sizes.end(), std::back_inserter(qkv_head_sizes),
                       [this] (const size_t w) { return static_cast<size_t>(w / num_heads); });

        scale = params.get<float>("scale", sqrt(1.f / qkv_head_sizes[0]));

        is_prepacked = false;
    }

    virtual bool supportBackend(int backendId) CV_OVERRIDE {
        return backendId == DNN_BACKEND_OPENCV;
    }

    virtual bool getMemoryShapes(const std::vector<MatShape> &inputs,
                                 const int requiredOutputs,
                                 std::vector<MatShape> &outputs,
                                 std::vector<MatShape> &internals) const CV_OVERRIDE {
        CV_CheckEQ(inputs.size(), static_cast<size_t>(3), "DNN/Attention: three inputs are required");
        const auto &input_shape = inputs[0];
        const auto &weight_shape = inputs[1];
        const auto &bias_shape = inputs[2];
        size_t dim_bias = static_cast<size_t>(std::accumulate(bias_shape.begin(), bias_shape.end(), 1, std::multiplies<int>()));

        CV_CheckEQ(input_shape.size(), static_cast<size_t>(3), "DNN/Attention: invalid input dimension");
        CV_CheckEQ(weight_shape.size(), static_cast<size_t>(2), "DNN/Attention: invalid weight dimension");

        CV_CheckEQ(input_shape[2], weight_shape[0], "DNN/Attention: invalid input shape");
        CV_CheckEQ(static_cast<size_t>(weight_shape[1]), hidden_size, "DNN/Attention: invalid weight shape");
        CV_CheckEQ(dim_bias, hidden_size, "DNN/Attention: invalid bias shape");

        outputs.assign(1, inputs[0]);
        return false;
    }

    virtual void finalize(InputArrayOfArrays inputs_arr, OutputArrayOfArrays outputs_arr) CV_OVERRIDE {
        opt.init();

        std::vector<Mat> inputs;
        inputs_arr.getMatVector(inputs);
        const auto input_shape = shape(inputs[0]);
        batch_size = static_cast<size_t>(input_shape[0]);
        seq_len = static_cast<size_t>(input_shape[1]);
        input_hidden_size = static_cast<size_t>(input_shape[2]);
    }

    void forward(InputArrayOfArrays inputs_arr, OutputArrayOfArrays outputs_arr, OutputArrayOfArrays internals_arr) CV_OVERRIDE {
        CV_TRACE_FUNCTION();
        CV_TRACE_ARG_VALUE(name, "name", name.c_str());

        if (inputs_arr.depth() == CV_16S)
        {
            forward_fallback(inputs_arr, outputs_arr, internals_arr);
            return;
        }

        std::vector<Mat> inputs, outputs;
        inputs_arr.getMatVector(inputs);
        outputs_arr.getMatVector(outputs);
        const auto &input = inputs[0];
        const auto &bias = inputs[2];
        auto &output = outputs[0];

        if (!is_prepacked) {
            // prepack
            const auto &weight = inputs[1];
            const auto *weight_data = weight.ptr<const float>();
            packWeight(num_heads, qkv_head_sizes[0], input_hidden_size, weight_data, hidden_size, packed_weight_q, opt);
            packWeight(num_heads, qkv_head_sizes[1], input_hidden_size, weight_data + qkv_hidden_sizes[0], hidden_size, packed_weight_k, opt);
            packWeight(num_heads, qkv_head_sizes[2], input_hidden_size, weight_data + qkv_hidden_sizes[0] + qkv_hidden_sizes[1], hidden_size, packed_weight_v, opt);

            is_prepacked = true;
        }

        float *packed_weights[3] = {packed_weight_q.data(), packed_weight_k.data(), packed_weight_v.data()};
        size_t packed_weights_size[3] = {packed_weight_q.size(), packed_weight_k.size(), packed_weight_v.size()};

        const auto *input_data = input.ptr<const float>();
        const auto *bias_data = bias.ptr<const float>();

        auto *output_data = output.ptr<float>();
        float *qkv[3] = {output_data,
                         output_data + batch_size * seq_len * qkv_hidden_sizes[0],
                         output_data + batch_size * seq_len * (qkv_hidden_sizes[0] + qkv_hidden_sizes[1])};

        auto fn = [&](const Range &r) {
            for (int i = r.start; i < r.end; i++) {
                const int batch_index = static_cast<int>((i / 3) / num_heads);
                const int head_index = static_cast<int>((i / 3) % num_heads);
                const int qkv_index = static_cast<int>(i % 3);

                auto *dst = qkv[qkv_index];
                size_t head_size = qkv_head_sizes[qkv_index];

                int input_offset = batch_index * seq_len * input_hidden_size;
                int bias_offset = qkv_index * qkv_hidden_sizes[0] + head_index * head_size;
                int dst_offset = (batch_index * num_heads + head_index) * (seq_len * head_size);

                // broadcast bias ([NH] -> [BN, SH]) and make copy to dst
                const auto *bias_data_src = bias_data + bias_offset;
                auto *dst_data = dst + dst_offset;
                for (size_t seq_len_idx = 0; seq_len_idx < seq_len; seq_len_idx++) {
                    std::memcpy(dst_data, bias_data_src, head_size * sizeof(float));
                    dst_data += head_size; // incorrect?
                }

                auto *packed_weight = packed_weights[qkv_index] + packed_weights_size[qkv_index] * head_index;
                // single-thread gemm kernel
                fastGemm(false, seq_len, head_size, input_hidden_size,
                         1.f, input_data + input_offset, input_hidden_size,
                         packed_weight, 1.f, dst + dst_offset, head_size, opt);
            }
        };

        size_t loops = 3 * batch_size * num_heads;
        double nstripes = loops * seq_len * qkv_head_sizes[0] * input_hidden_size * (1 / 1024.0);
        parallel_for_(Range(0, loops), fn, nstripes);

        // input.copyTo(output);
    }

 private:
    size_t num_heads;
    std::vector<size_t> qkv_hidden_sizes; // order: {qk_hidden_size, qk_hidden_size, v_hidden_size}
    float scale;

    std::vector<size_t> qkv_head_sizes; // order: {qk_head_size, qk_head_size, v_head_size}

    size_t batch_size;
    size_t seq_len;
    size_t input_hidden_size;
    size_t hidden_size;

    bool is_prepacked;
    std::vector<float> packed_weight_q;
    std::vector<float> packed_weight_k;
    std::vector<float> packed_weight_v;

    FastGemmOpt opt;
};

Ptr<AttentionLayer> AttentionLayer::create(const LayerParams &params) {
    return makePtr<AttentionLayerImpl>(params);
}

}} // cv::dnn
