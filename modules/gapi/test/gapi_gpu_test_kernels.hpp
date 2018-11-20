// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Copyright (C) 2018 Intel Corporation


#ifndef GAPI_GPU_TEST_KERNELS_HPP
#define GAPI_GPU_TEST_KERNELS_HPP

#include "opencv2/gapi/gpu/ggpukernel.hpp"

namespace cv
{
    void reference_symm7x7_CPU(const cv::Mat& in, cv::Mat& kernel_coeff, int shift, cv::Mat &out);

#ifdef HAVE_OPENCL
    namespace gapi_test_kernels
    {

        G_TYPED_KERNEL(TSymm7x7_test, <GMat(GMat, Mat, int)>, "org.opencv.imgproc.symm7x7_test") {
            static GMatDesc outMeta(GMatDesc in, Mat, int) {
                return in.withType(CV_8U, 1);
            }
        };

        extern cv::gapi::GKernelPackage gpuTestPackage;

    } // namespace gapi_test_kernels
#endif
} // namespace cv

#endif // GAPI_GPU_TEST_KERNELS_HPP
