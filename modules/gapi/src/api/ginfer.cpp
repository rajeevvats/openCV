// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Copyright (C) 2018-2019 Intel Corporation


#include "precomp.hpp"

#include <functional> // hash
#include <numeric> // accumulate
#include <unordered_set>
#include <iterator>

#include <ade/util/algorithm.hpp>

#include <opencv2/gapi/infer.hpp>

cv::gapi::GNetPackage::GNetPackage(std::initializer_list<GNetParam> &&ii)
    : networks(std::move(ii)) {
}

std::vector<cv::gapi::GBackend> cv::gapi::GNetPackage::backends() const {
    std::unordered_set<cv::gapi::GBackend> unique_set;
    for (const auto &nn : networks) unique_set.insert(nn.backend);
    return std::vector<cv::gapi::GBackend>(unique_set.begin(), unique_set.end());
}

cv::GMat cv::GInferInputs::operator[](const std::string& name) {
    return in_blobs[name];
}

const std::unordered_map<std::string, cv::GMat>& cv::GInferInputs::getBlobs() const {
    return in_blobs;
}

cv::GInferOutputs::GInferOutputs(std::shared_ptr<cv::GCall> call)
    : m_call(std::move(call)), m_info(cv::util::any_cast<InOutInfo>(&m_call->params()))
{
};

cv::GMat cv::GInferOutputs::at(const std::string& name)
{
    auto it = out_blobs.find(name);
    if (it == out_blobs.end()) {
        // FIXME: Avoid modifying GKernel
        m_call->kernel().outShapes.push_back(cv::GShape::GMAT);
        int out_idx = static_cast<int>(out_blobs.size());
        it = out_blobs.emplace(name, m_call->yield(out_idx)).first;
        m_info->out_names.push_back(name);
    }
    return it->second;
};
