// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#include "test_precomp.hpp"
#include <opencv2/dnn/shape_utils.hpp>
#include "npy_blob.hpp"
namespace opencv_test { namespace {

template<typename TString>
static std::string _tf(TString filename)
{
    String rootFolder = "dnn/";
    return findDataFile(rootFolder + filename, false);
}


class Test_Model : public DNNTestLayer
{
public:
    void testDetectModel(const std::string& weights, const std::string& cfg,
                         const std::string& imgPath, const std::vector<int>& refClassIds,
                         const std::vector<float>& refConfidences,
                         const std::vector<Rect2d>& refBoxes,
                         double scoreDiff, double iouDiff,
                         double confThreshold = 0.24, double nmsThreshold = 0.0,
                         const Size& size = {-1, -1}, Scalar mean = Scalar(),
                         float scale = 1.0, bool swapRB = false, bool crop = false)
    {
        checkBackend();

        Mat frame = imread(imgPath);
        DetectionModel model(weights, cfg);

        model.setInputSize(size).setInputMean(mean).setInputScale(scale)
             .setInputSwapRB(swapRB).setInputCrop(crop);

        std::vector<int> classIds;
        std::vector<float> confidences;
        std::vector<Rect> boxes;

        model.detect(frame, classIds, confidences, boxes, confThreshold, nmsThreshold);

        std::vector<Rect2d> boxesDouble(boxes.size());
        for (int i = 0; i < boxes.size(); i++) {
            boxesDouble[i] = boxes[i];
        }
        normAssertDetections(refClassIds, refConfidences, refBoxes, classIds,
                         confidences, boxesDouble, "",
                         confThreshold, scoreDiff, iouDiff);
    }

    void testClassifyModel(const std::string& weights, const std::string& cfg,
                    const std::string& imgPath, std::pair<int, float> ref, float norm,
                    const Size& size = {-1, -1}, Scalar mean = Scalar(),
                    float scale = 1.0, bool swapRB = false, bool crop = false)
    {
        checkBackend();

        Mat frame = imread(imgPath);
        ClassificationModel model(weights, cfg);
        model.setInputSize(size).setInputMean(mean).setInputScale(scale)
             .setInputSwapRB(swapRB).setInputCrop(crop);

        std::pair<int, float> prediction = model.classify(frame);
        EXPECT_EQ(prediction.first, ref.first);
        ASSERT_NEAR(prediction.second, ref.second, norm);
    }
};

TEST_P(Test_Model, Classify)
{
    std::pair<int, float> ref(652, 0.641789);

    std::string img_path = _tf("grace_hopper_227.png");
    std::string config_file = _tf("bvlc_alexnet.prototxt");
    std::string weights_file = _tf("bvlc_alexnet.caffemodel");

    Size size{227, 227};
    float norm = 1e-4;

    testClassifyModel(weights_file, config_file, img_path, ref, norm, size);
}


TEST_P(Test_Model, DetectRegion)
{
    std::vector<int> refClassIds = {6, 1, 11};
    std::vector<float> refConfidences = {0.750469f, 0.780879f, 0.901615f};
    std::vector<Rect2d> refBoxes = {Rect2d(240.032, 52.994, 135.408, 72.134),
                                    Rect2d(112.320, 109.865, 192.067, 200.220),
                                    Rect2d(57.658, 140.608, 117.603, 249.683)};

    std::string img_path = _tf("dog416.png");
    std::string weights_file = _tf("yolo-voc.weights");
    std::string config_file = _tf("yolo-voc.cfg");

    float scale = 1.0 / 255.0;
    Size size{416, 416};
    bool swapRB = true;

    double confThreshold = 0.24;
    double scoreDiff = (target == DNN_TARGET_OPENCL_FP16 || target == DNN_TARGET_MYRIAD) ? 1e-2 : 8e-5;
    double iouDiff = 0.014;
    double nmsThreshold = (target == DNN_TARGET_MYRIAD) ? 0.397 : 0.4;

    testDetectModel(weights_file, config_file, img_path, refClassIds, refConfidences,
                    refBoxes, scoreDiff, iouDiff, confThreshold, nmsThreshold, size,
                    Scalar(), scale, swapRB);
}

TEST_P(Test_Model, DetectionOutput)
{
    std::vector<int> refClassIds = {7, 12};
    std::vector<float> refConfidences = {0.991359f, 0.94786f};
    std::vector<Rect2d> refBoxes = {Rect2d(491.822, 81.1668, 211.751, 98.0672),
                                    Rect2d(132.093, 223.903, 206.984, 343.257)};

    std::string img_path = _tf("dog416.png");
    std::string weights_file = _tf("resnet50_rfcn_final.caffemodel");
    std::string config_file = _tf("rfcn_pascal_voc_resnet50.prototxt");

    Scalar mean = Scalar(102.9801, 115.9465, 122.7717);
    Size size{800, 600};

    double scoreDiff = (backend == DNN_BACKEND_OPENCV && target == DNN_TARGET_OPENCL_FP16) ?
                        4e-3 : default_l1;
    double iouDiff = 0.016;
    float confThreshold = 0.8;
    float nmsThreshold = 0;

    testDetectModel(weights_file, config_file, img_path, refClassIds, refConfidences, refBoxes,
                    scoreDiff, iouDiff, confThreshold, nmsThreshold, size, mean);
}


TEST_P(Test_Model, DetectionMobilenetSSD)
{
    Mat ref = blobFromNPY(_tf("mobilenet_ssd_caffe_out.npy"));
    int dims[] = {ref.size[2], ref.size[3]};
    ref = ref.reshape(0, 2, dims);

    std::string img_path = _tf("street.png");
    Mat frame = imread(img_path);
    int frameWidth  = frame.cols;
    int frameHeight = frame.rows;

    std::vector<int> refClassIds;
    std::vector<float> refConfidences;
    std::vector<Rect2d> refBoxes;
    for (int i = 0; i < ref.rows; i++)
    {
        refClassIds.emplace_back(ref.at<float>(i, 1));
        refConfidences.emplace_back(ref.at<float>(i, 2));
        double left   = ref.at<float>(i, 3) * (frameWidth - 1);
        double top    = ref.at<float>(i, 4) * (frameHeight - 1);
        double right  = ref.at<float>(i, 5) * (frameWidth - 1);
        double bottom = ref.at<float>(i, 6) * (frameHeight - 1);
        double width  = right  - left + 1;
        double height = bottom - top + 1;
        refBoxes.emplace_back(left, top, width, height);
    }

    std::string weights_file = _tf("MobileNetSSD_deploy.caffemodel");
    std::string config_file = _tf("MobileNetSSD_deploy.prototxt");

    Scalar mean = Scalar(127.5, 127.5, 127.5);
    float scale = 1.0f / 127.5;
    Size size{300, 300};

    double scoreDiff = (target == DNN_TARGET_OPENCL_FP16 || target == DNN_TARGET_MYRIAD) ? 1.5e-2 : 1e-5;
    double iouDiff = 0.041;

    float confThreshold = FLT_MIN;
    float nmsThreshold = 0;

    testDetectModel(weights_file, config_file, img_path, refClassIds, refConfidences, refBoxes,
                    scoreDiff, iouDiff, confThreshold, nmsThreshold, size, mean, scale);
}

INSTANTIATE_TEST_CASE_P(/**/, Test_Model, dnnBackendsAndTargets());

}} // namespace
