/*
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
*/

#include "fastcv_hal_core.hpp"
#include "fastcv_hal_utils.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/core/base.hpp>


class ParallelTableLookup : public cv::ParallelLoopBody
{
public:

    ParallelTableLookup(const uchar* src_data_, int width_, size_t src_step_, const uchar* lut_data_, uchar* dst_data_, size_t dst_step_) :
        cv::ParallelLoopBody(), src_data(src_data_), width(width_), src_step(src_step_), lut_data(lut_data_), dst_data(dst_data_), dst_step(dst_step_)
    {
    }

    virtual void operator()(const cv::Range& range) const CV_OVERRIDE
    {
        fcvStatus status = FASTCV_SUCCESS;
        for (int y = range.start; y < range.end; y++) {
            status = fcvTableLookupu8((uint8_t*)src_data + y * src_step, width, 1, src_step, (uint8_t*)lut_data, (uint8_t*)dst_data + y * dst_step, dst_step);
            if(status != FASTCV_SUCCESS)
                CV_LOG_ERROR(NULL,"FastCV error:"<<getFastCVErrorString(status));
        }
    }

private:
    const uchar* src_data;
    int          width;
    size_t       src_step;
    const uchar* lut_data;
    uchar*       dst_data;
    size_t       dst_step;
};

int fastcv_hal_lut(
    const uchar*    src_data,
    size_t          src_step,
    size_t          src_type,
    const uchar*    lut_data,
    size_t          lut_channel_size,
    size_t          lut_channels,
    uchar*          dst_data,
    size_t          dst_step,
    int             width,
    int             height)
{
    if((width*height)<=(320*240))
        CV_HAL_RETURN_NOT_IMPLEMENTED("Switching to default OpenCV solution!");

    INITIALIZATION_CHECK;

    fcvStatus           status;
    if (src_type == CV_8UC1 && lut_channels == 1 && lut_channel_size == 1)
    {
        cv::parallel_for_(cv::Range(0, height),
            ParallelTableLookup(src_data, width, src_step, lut_data, dst_data, dst_step));
        status = FASTCV_SUCCESS;
        CV_HAL_RETURN(status, hal_lut);
    }
    else
    {
        CV_HAL_RETURN_NOT_IMPLEMENTED("Multi-channel input is not supported");
    }
}

int fastcv_hal_normHammingDiff8u(
    const uchar*    a,
    const uchar*    b,
    int             n,
    int             cellSize,
    int*            result)
{
    fcvStatus           status;

    if (cellSize != 1)
        CV_HAL_RETURN_NOT_IMPLEMENTED(cv::format("NORM_HAMMING2 cellSize:%d is not supported", cellSize));

    INITIALIZATION_CHECK;

    uint32_t dist = 0;

    dist = fcvHammingDistanceu8((uint8_t*)a, (uint8_t*)b, n);

    *result = dist;
    status = FASTCV_SUCCESS;
    CV_HAL_RETURN(status, hal_normHammingDiff8u);
}

int fastcv_hal_mul8u16u(
    const uchar*     src1_data,
    size_t           src1_step,
    const uchar*     src2_data,
    size_t           src2_step,
    ushort*          dst_data,
    size_t           dst_step,
    int              width,
    int              height,
    double           scale)
{
    if(scale != 1.0)
        CV_HAL_RETURN_NOT_IMPLEMENTED("Scale factor not supported");

    INITIALIZATION_CHECK;

    fcvStatus status = FASTCV_SUCCESS;

    if (src1_step < (size_t)width && src2_step < (size_t)width)
    {
        src1_step = width*sizeof(uchar);
        src2_step = width*sizeof(uchar);
        dst_step  = width*sizeof(ushort);
    }

    status = fcvElementMultiplyu8u16_v2(src1_data, width, height, src1_step,
                            src2_data, src2_step, dst_data, dst_step);

    CV_HAL_RETURN(status,hal_multiply);
}

int fastcv_hal_sub8u32f(
    const uchar*     src1_data,
    size_t           src1_step,
    const uchar*     src2_data,
    size_t           src2_step,
    float*           dst_data,
    size_t           dst_step,
    int              width,
    int              height)
{
    INITIALIZATION_CHECK;

    fcvStatus status = FASTCV_SUCCESS;

    if (src1_step < (size_t)width && src2_step < (size_t)width)
    {
        src1_step = width*sizeof(uchar);
        src2_step = width*sizeof(uchar);
        dst_step  = width*sizeof(float);
    }

    status = fcvImageDiffu8f32_v2(src1_data, src2_data, width, height, src1_step,
                                  src2_step, dst_data, dst_step);

    CV_HAL_RETURN(status,hal_subtract);

}

int fastcv_hal_transpose2d(
    const uchar*     src_data,
    size_t           src_step,
    uchar*           dst_data,
    size_t           dst_step,
    int              src_width,
    int              src_height,
    int              element_size)
{
    INITIALIZATION_CHECK;

    if (src_data == dst_data)
        CV_HAL_RETURN_NOT_IMPLEMENTED("In-place not supported");

    fcvStatus status = FASTCV_SUCCESS;

    switch (element_size)
    {
        case 1:
            status = fcvTransposeu8_v2(src_data, src_width, src_height, src_step,
                                       dst_data, dst_step);
            break;
        case 2:
            status = fcvTransposeu16_v2((const uint16_t*)src_data, src_width, src_height,
                                       src_step, (uint16_t*)dst_data, dst_step);
            break;
        case 4:
            status = fcvTransposef32_v2((const float32_t*)src_data, src_width, src_height,
                                       src_step, (float32_t*)dst_data, dst_step);
            break;
        default:
            CV_HAL_RETURN_NOT_IMPLEMENTED("srcType not supported");
    }

    CV_HAL_RETURN(status,hal_transpose);
}

int fastcv_hal_meanStdDev(
    const uchar*      src_data,
    size_t            src_step,
    int               width,
    int               height,
    int               src_type,
    double*           mean_val,
    double*           stddev_val,
    uchar*            mask,
    size_t            mask_step)
{
    INITIALIZATION_CHECK;

    CV_UNUSED(mask_step);

    if(src_type != CV_8UC1)
    {
        CV_HAL_RETURN_NOT_IMPLEMENTED("src type not supported");
    }
    else if(mask != nullptr)
    {
        CV_HAL_RETURN_NOT_IMPLEMENTED("mask not supported");
    }
    else if(mean_val == nullptr && stddev_val == nullptr)
    {
        CV_HAL_RETURN_NOT_IMPLEMENTED("null ptr for mean and stddev");
    }

    float32_t mean, variance;

    fcvStatus status = fcvImageIntensityStats_v2(src_data, src_step, 0, 0, width, height,
                                   &mean, &variance, FASTCV_BIASED_VARIANCE_ESTIMATOR);

    if(mean_val != nullptr)
        *mean_val = mean;
    if(stddev_val != nullptr)
        *stddev_val = std::sqrt(variance);

    CV_HAL_RETURN(status,hal_meanStdDev);
}

int fastcv_hal_flip(
    int             src_type,
    const uchar*    src_data,
    size_t          src_step,
    int             src_width,
    int             src_height,
    uchar*          dst_data,
    size_t          dst_step,
    int             flip_mode)
{
    INITIALIZATION_CHECK;

    if(src_type!=CV_8UC1 && src_type!=CV_16UC1 && src_type!=CV_8UC3)
        CV_HAL_RETURN_NOT_IMPLEMENTED("Data type is not supported, Switching to default OpenCV solution!");

    if((src_width*src_height)<=(640*480))
        CV_HAL_RETURN_NOT_IMPLEMENTED("Switching to default OpenCV solution!");

    fcvStatus       status = FASTCV_SUCCESS;;
    fcvFlipDir      dir;

    switch (flip_mode)
    {
        //Flip around X-Axis: Vertical Flip or FLIP_ROWS
        case 0:
            CV_HAL_RETURN_NOT_IMPLEMENTED("Switching to default OpenCV solution due to low perf!");
            dir = FASTCV_FLIP_VERT;
            break;

        //Flip around Y-Axis: Horizontal Flip or FLIP_COLS
        case 1:
            dir = FASTCV_FLIP_HORIZ;
            break;

        //Flip around both X and Y-Axis or FLIP_BOTH
        case -1:
            dir = FASTCV_FLIP_BOTH;
            break;
        default:
            CV_HAL_RETURN_NOT_IMPLEMENTED("Invalid flip_mode, Switching to default OpenCV solution!");
    }

    if(src_type==CV_8UC1)
        fcvFlipu8(src_data, src_width, src_height, src_step, dst_data, dst_step, dir);
    else if(src_type==CV_16UC1)
        fcvFlipu16((uint16_t*)src_data, src_width, src_height, src_step, (uint16_t*)dst_data, dst_step, dir);
    else if(src_type==CV_8UC3)
        status = fcvFlipRGB888u8((uint8_t*)src_data, src_width, src_height, src_step, (uint8_t*)dst_data, dst_step, dir);
    else
        CV_HAL_RETURN_NOT_IMPLEMENTED(cv::format("Data type:%d is not supported, Switching to default OpenCV solution!", src_type));

    CV_HAL_RETURN(status, hal_flip);
}

int fastcv_hal_rotate(
    int             src_type,
    const uchar*    src_data,
    size_t          src_step,
    int             src_width,
    int             src_height,
    uchar*          dst_data,
    size_t          dst_step,
    int             angle)
{
    if((src_width*src_height)<(120*80))
        CV_HAL_RETURN_NOT_IMPLEMENTED("Switching to default OpenCV solution for lower resolution!");

    fcvStatus           status;
    fcvRotateDegree     degree;

    if (src_type != CV_8UC1 && src_type != CV_8UC2)
        CV_HAL_RETURN_NOT_IMPLEMENTED(cv::format("src_type:%d is not supported", src_type));

    INITIALIZATION_CHECK;

    switch (angle)
    {
        case 90:
            degree = FASTCV_ROTATE_90;
            break;
        case 180:
            degree = FASTCV_ROTATE_180;
            break;
        case 270:
            degree = FASTCV_ROTATE_270;
            break;
        default:
            CV_HAL_RETURN_NOT_IMPLEMENTED(cv::format("Rotation angle:%d is not supported", angle));
    }

    switch(src_type)
    {
        case CV_8UC1:
            status = fcvRotateImageu8(src_data, src_width, src_height, src_step, dst_data, dst_step, degree);
            break;
        case CV_8UC2:
            status = fcvRotateImageInterleavedu8((uint8_t*)src_data, src_width, src_height, src_step, (uint8_t*)dst_data,
                                                    dst_step, degree);
            break;
        default:
            CV_HAL_RETURN_NOT_IMPLEMENTED(cv::format("src_type:%d is not supported", src_type));
    }
    CV_HAL_RETURN(status, hal_rotate);
}

int fastcv_hal_addWeighted8u(
    const uchar*    src1_data,
    size_t          src1_step,
    const uchar*    src2_data,
    size_t          src2_step,
    uchar*          dst_data,
    size_t          dst_step,
    int             width,
    int             height,
    const double    scalars[3])
{
    if( (scalars[0] < -128.0f) || (scalars[0] >= 128.0f) ||
        (scalars[1] < -128.0f) || (scalars[1] >= 128.0f) ||
        (scalars[2] < -(1<<23))|| (scalars[2] >= 1<<23))
        CV_HAL_RETURN_NOT_IMPLEMENTED(
            cv::format("Alpha:%f,Beta:%f,Gamma:%f is not supported because it's too large or too small\n",
            scalars[0],scalars[1],scalars[2]));

    INITIALIZATION_CHECK;

    fcvStatus status = FASTCV_SUCCESS;

    if (height == 1)
    {
        src1_step = width*sizeof(uchar);
        src2_step = width*sizeof(uchar);
        dst_step  = width*sizeof(uchar);

        cv::parallel_for_(cv::Range(0, width), [&](const cv::Range &range){
            int rangeWidth = range.end - range.start;
            const uint8_t *src1 = src1_data + range.start;
            const uint8_t *src2 = src2_data + range.start;
            uint8_t *dst = dst_data + range.start;
            fcvAddWeightedu8_v2(src1, rangeWidth, height, src1_step, src2, src2_step,
                scalars[0], scalars[1], scalars[2], dst, dst_step);
            });
    }
    else
    {
        cv::parallel_for_(cv::Range(0, height), [&](const cv::Range &range){
            int rangeHeight = range.end - range.start;
            const uint8_t *src1 = src1_data + range.start * src1_step;
            const uint8_t *src2 = src2_data + range.start * src2_step;
            uint8_t *dst = dst_data + range.start * dst_step;
            fcvAddWeightedu8_v2(src1, width, rangeHeight, src1_step, src2, src2_step,
                scalars[0], scalars[1], scalars[2], dst, dst_step);
            });
    }

    CV_HAL_RETURN(status, hal_addWeighted8u_v2);
}

int fastcv_hal_gemm32f(
    const float*    src1,
    size_t          src1_step,
    const float*    src2,
    size_t          src2_step,
    float           alpha,
    const float*    src3,
    size_t          src3_step,
    float           beta,
    float*          dst,
    size_t          dst_step,
    int             m,
    int             n,
    int             k,
    int             flags)
{
// dst = alpha*src1*src2 + beta*src3;

    cv::Mat dst_temp1, dst_temp2;
    float *dstp = NULL;
    bool inplace = false;
    size_t dst_stride;
    fcvStatus status = FASTCV_SUCCESS;

    if(flags != 0)
    {
        CV_HAL_RETURN_NOT_IMPLEMENTED("gemm flags not supported");
    }
    else if(((src1_step & 3) != 0) ||   // check for stride multiple of 4
            ((src2_step & 3) != 0) || 
            ((src3_step & 3) != 0) || 
            ((dst_step & 3) != 0)
           )  
    {
        CV_HAL_RETURN_NOT_IMPLEMENTED("stride multiple of 4 supported");
    }

    INITIALIZATION_CHECK;

    if(src1 == dst || src2 == dst || src3 == dst)
    {
        dst_temp1 = cv::Mat(m, k, CV_32FC1);
        dstp = dst_temp1.ptr<float>();
        inplace = true;
        dst_stride = dst_temp1.step[0];
    }
    else
    {
        dstp = dst;
        dst_stride = dst_step;
    }
    float *dstp1 = dstp;
    if(alpha != 0.0)
        status = fcvMatrixMultiplyf32_v2(src1, n, m, src1_step, src2, k, 
                                        src2_step, dstp, dst_stride);

    if(alpha != 1.0 && alpha != 0.0 && status == FASTCV_SUCCESS)
    {
        status = fcvMultiplyScalarf32(dstp, k, m, dst_stride, alpha, dstp1, dst_stride);
    }

    if(src3 != NULL && beta != 0.0 && status == FASTCV_SUCCESS)
    {
        cv::Mat dst3 = cv::Mat(m, k, CV_32FC1);
        if(beta != 1.0)
        {
            status = fcvMultiplyScalarf32(src3, k, m, src3_step, beta, (float32_t*)dst3.data, dst3.step);
            if(status == FASTCV_SUCCESS)
                fcvAddf32_v2(dstp, k, m, dst_stride, (float32_t*)dst3.data, dst3.step, dstp1, dst_stride);
        }
        else
            fcvAddf32_v2(dstp, k, m, dst_stride, src3, src3_step, dstp1, dst_stride);
    }

    if(inplace == true)
    {
        cv::Mat dst_mat = cv::Mat(m, k , CV_32FC1, (void*)dst, dst_step);
        dst_temp1(cv::Rect(0, 0, k, m)).copyTo(dst_mat(cv::Rect(0, 0, k, m)));
    }

    CV_HAL_RETURN(status,hal_gemm32f);
}

int fastcv_hal_mul8u(
    const uchar     *src1_data,
    size_t          src1_step,
    const uchar     *src2_data,
    size_t          src2_step,
    uchar           *dst_data,
    size_t          dst_step,
    int             width,
    int             height,
    double          scale)
{
    int8_t sF;

    if(scale >= 1.0)
    {
        if(scale == 1.0)        { sF =  0; }
        else if(scale == 2.0)   { sF = -1; }
        else if(scale == 4.0)   { sF = -2; }
        else if(scale == 8.0)   { sF = -3; }
        else if(scale == 16.0)  { sF = -4; }
        else if(scale == 32.0)  { sF = -5; }
        else if(scale == 64.0)  { sF = -6; }
        else if(scale == 128.0) { sF = -7; }
        else if(scale == 256.0) { sF = -8; }
        else CV_HAL_RETURN_NOT_IMPLEMENTED("scale factor not supported");
    }
    else if(scale > 0 && scale < 1.0)
    {
        if(scale == 1/2.0)        { sF = 1; }
        else if(scale == 1/4.0)   { sF = 2; }
        else if(scale == 1/8.0)   { sF = 3; }
        else if(scale == 1/16.0)  { sF = 4; }
        else if(scale == 1/32.0)  { sF = 5; }
        else if(scale == 1/64.0)  { sF = 6; }
        else if(scale == 1/128.0) { sF = 7; }
        else if(scale == 1/256.0) { sF = 8; }
        else CV_HAL_RETURN_NOT_IMPLEMENTED("scale factor not supported");
    }
    else
        CV_HAL_RETURN_NOT_IMPLEMENTED("scale factor not supported");

    INITIALIZATION_CHECK;

    int nStripes = cv::getNumThreads();

    if(height == 1)
    {
        cv::parallel_for_(cv::Range(0, width), [&](const cv::Range &range){
                      int rangeWidth = range.end - range.start;
                      const uchar* yS1 =  src1_data + static_cast<size_t>(range.start);
                      const uchar* yS2 =  src2_data + static_cast<size_t>(range.start);
                      uchar* yD = dst_data + static_cast<size_t>(range.start);
                      fcvElementMultiplyu8(yS1, rangeWidth, 1, 0, yS2, 0, sF, 
                                            FASTCV_CONVERT_POLICY_SATURATE, yD, 0);
                      }, nStripes);
    }
    else
    {
        cv::parallel_for_(cv::Range(0, height), [&](const cv::Range &range){
                      int rangeHeight = range.end - range.start;
                      const uchar* yS1 =  src1_data + static_cast<size_t>(range.start)*src1_step;
                      const uchar* yS2 =  src2_data + static_cast<size_t>(range.start)*src2_step;
                      uchar* yD = dst_data + static_cast<size_t>(range.start)*dst_step;
                      fcvElementMultiplyu8(yS1, width, rangeHeight, src1_step, yS2, src2_step,
                                            sF, FASTCV_CONVERT_POLICY_SATURATE, yD, dst_step);
                      }, nStripes);
    }

    return CV_HAL_ERROR_OK;
}

int fastcv_hal_mul16s(
    const short     *src1_data,
    size_t          src1_step,
    const short     *src2_data,
    size_t          src2_step,
    short           *dst_data,
    size_t          dst_step,
    int             width,
    int             height,
    double          scale)
{
    int8_t sF;

    if(scale >= 1.0)
    {
        if(scale == 1.0)        { sF =  0; }
        else if(scale == 2.0)   { sF = -1; }
        else if(scale == 4.0)   { sF = -2; }
        else if(scale == 8.0)   { sF = -3; }
        else if(scale == 16.0)  { sF = -4; }
        else if(scale == 32.0)  { sF = -5; }
        else if(scale == 64.0)  { sF = -6; }
        else if(scale == 128.0) { sF = -7; }
        else if(scale == 256.0) { sF = -8; }
        else CV_HAL_RETURN_NOT_IMPLEMENTED("scale factor not supported");
    }
    else if(scale > 0 && scale < 1.0)
    {
        if(scale == 1/2.0)        { sF = 1; }
        else if(scale == 1/4.0)   { sF = 2; }
        else if(scale == 1/8.0)   { sF = 3; }
        else if(scale == 1/16.0)  { sF = 4; }
        else if(scale == 1/32.0)  { sF = 5; }
        else if(scale == 1/64.0)  { sF = 6; }
        else if(scale == 1/128.0) { sF = 7; }
        else if(scale == 1/256.0) { sF = 8; }
        else CV_HAL_RETURN_NOT_IMPLEMENTED("scale factor not supported");
    }
    else
        CV_HAL_RETURN_NOT_IMPLEMENTED("scale factor not supported");

    INITIALIZATION_CHECK;

    int nStripes = cv::getNumThreads();

    if(height == 1)
    {
        cv::parallel_for_(cv::Range(0, width), [&](const cv::Range &range){
                      int rangeWidth = range.end - range.start;
                      const short* yS1 =  src1_data + static_cast<size_t>(range.start);
                      const short* yS2 =  src2_data + static_cast<size_t>(range.start);
                      short* yD = dst_data + static_cast<size_t>(range.start);
                      fcvElementMultiplys16(yS1, rangeWidth, 1, 0, yS2, 0, sF, 
                                             FASTCV_CONVERT_POLICY_SATURATE, yD, 0);
                      }, nStripes);
    }
    else
    {
        cv::parallel_for_(cv::Range(0, height), [&](const cv::Range &range){
                      int rangeHeight = range.end - range.start;
                      const short* yS1 =  src1_data + static_cast<size_t>(range.start) * (src1_step/sizeof(short));
                      const short* yS2 =  src2_data + static_cast<size_t>(range.start) * (src2_step/sizeof(short));
                      short* yD = dst_data + static_cast<size_t>(range.start) * (dst_step/sizeof(short));
                      fcvElementMultiplys16(yS1, width, rangeHeight, src1_step, yS2, src2_step, 
                                                sF, FASTCV_CONVERT_POLICY_SATURATE, yD, dst_step);
                      }, nStripes);
    }

    return CV_HAL_ERROR_OK;
}

int fastcv_hal_mul32f(
    const float    *src1_data,
    size_t          src1_step,
    const float    *src2_data,
    size_t          src2_step,
    float          *dst_data,
    size_t          dst_step,
    int             width,
    int             height,
    double          scale)
{
    if(scale != 1.0)
        CV_HAL_RETURN_NOT_IMPLEMENTED("scale factor not supported");

    INITIALIZATION_CHECK;

    int nStripes = cv::getNumThreads();

    if(height == 1)
    {
        cv::parallel_for_(cv::Range(0, width), [&](const cv::Range &range){
                      int rangeWidth = range.end - range.start;
                      const float* yS1 =  src1_data + static_cast<size_t>(range.start);
                      const float* yS2 =  src2_data + static_cast<size_t>(range.start);
                      float* yD = dst_data + static_cast<size_t>(range.start);
                      fcvElementMultiplyf32(yS1, rangeWidth, 1, 0, yS2, 0, yD, 0);
                      }, nStripes);
    }
    else
    {
        cv::parallel_for_(cv::Range(0, height), [&](const cv::Range &range){
                      int rangeHeight = range.end - range.start;
                      const float* yS1 =  src1_data + static_cast<size_t>(range.start) * (src1_step/sizeof(float));
                      const float* yS2 =  src2_data + static_cast<size_t>(range.start) * (src2_step/sizeof(float));
                      float* yD = dst_data + static_cast<size_t>(range.start) * (dst_step/sizeof(float));
                      fcvElementMultiplyf32(yS1, width, rangeHeight, src1_step, 
                                                  yS2, src2_step, yD, dst_step);
                      }, nStripes);
    }

    return CV_HAL_ERROR_OK;
}

int fastcv_hal_split8u(
    const uchar    *src_data,
    uchar          **dst_data,
    int             len,
    int             cn)
{
    if(cn != 2)
        CV_HAL_RETURN_NOT_IMPLEMENTED("no of channels not supported");

    INITIALIZATION_CHECK;

    int nStripes = cv::getNumThreads();
                  
    cv::parallel_for_(cv::Range(0, len), [&](const cv::Range &range){
                      const uchar* yS =  src_data + static_cast<size_t>(range.start)*2;
                      uchar* y1D = dst_data[0] + static_cast<size_t>(range.start);
                      uchar* y2D = dst_data[1] + static_cast<size_t>(range.start);
                      int width_ = range.end - range.start;
                      fcvDeinterleaveu8(yS, width_, 1, 0, y1D, 0, y2D, 0);
                      }, nStripes);

    return CV_HAL_ERROR_OK;
}

int fastcv_hal_merge8u(
    const uchar     **src_data,
    uchar           *dst_data,
    int              len,
    int              cn)
{    
    if(cn != 2 && cn != 3 && cn != 4)
        CV_HAL_RETURN_NOT_IMPLEMENTED("no of channels not supported");

    INITIALIZATION_CHECK;

    int nStripes = cv::getNumThreads();

    if(cn == 2)
    {
        cv::parallel_for_(cv::Range(0, len), [&](const cv::Range &range){
                          const uchar* yS1 =  src_data[0] + static_cast<size_t>(range.start);
                          const uchar* yS2 =  src_data[1] + static_cast<size_t>(range.start);
                          uchar* yD = dst_data + static_cast<size_t>(range.start)*2;
                          int width_ = range.end - range.start;
                          fcvChannelCombine2Planesu8(yS1, width_, 1, 0, yS2, 0, yD, 0);
                          }, nStripes);
    }    
    else if(cn == 3)
    {
        cv::parallel_for_(cv::Range(0, len), [&](const cv::Range &range){
                          const uchar* yS1 =  src_data[0] + static_cast<size_t>(range.start);
                          const uchar* yS2 =  src_data[1] + static_cast<size_t>(range.start);
                          const uchar* yS3 =  src_data[2] + static_cast<size_t>(range.start);
                          uchar* yD = dst_data + static_cast<size_t>(range.start)*3;
                          int width_ = range.end - range.start;
                          fcvChannelCombine3Planesu8(yS1, width_, 1, 0, yS2, 0, yS3, 0, yD, 0);
                          }, nStripes);
    }
    else //if(cn == 4)
    {
        cv::parallel_for_(cv::Range(0, len), [&](const cv::Range &range){
                          const uchar* yS1 =  src_data[0] + static_cast<size_t>(range.start);
                          const uchar* yS2 =  src_data[1] + static_cast<size_t>(range.start);
                          const uchar* yS3 =  src_data[2] + static_cast<size_t>(range.start);
                          const uchar* yS4 =  src_data[3] + static_cast<size_t>(range.start);
                          uchar* yD = dst_data + static_cast<size_t>(range.start)*4;
                          int width_ = range.end - range.start;
                          fcvChannelCombine4Planesu8(yS1, width_, 1, 0, yS2, 0, yS3, 0, yS4, 0, yD, 0);
                          }, nStripes);
    }

    return CV_HAL_ERROR_OK;
}