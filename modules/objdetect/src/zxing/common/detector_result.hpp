// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Tencent is pleased to support the open source community by making WeChat QRCode available.
// Copyright (C) 2020 THL A29 Limited, a Tencent company. All rights reserved.
//
// Modified from ZXing. Copyright ZXing authors.
// Licensed under the Apache License, Version 2.0 (the "License").

#ifndef __ZXING_COMMON_DETECTOR_RESULT_HPP__
#define __ZXING_COMMON_DETECTOR_RESULT_HPP__

/*
 *  DetectorResult.hpp
 *  zxing
 *
 *  Copyright 2010 ZXing authors All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http:// www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "counted.hpp"
#include "array.hpp"
#include "bit_matrix.hpp"
#include "byte_matrix.hpp"
#include "../result_point.hpp"

namespace zxing {

class DetectorResult : public Counted {
private:
    Ref<BitMatrix> bits_;
    Ref<ByteMatrix> gray_;
    ArrayRef< Ref<ResultPoint> > points_;
    
#ifdef CALC_CODE_AREA_SCORE 
    int maskPatternNum_;
#endif
    
public:
    DetectorResult(Ref<BitMatrix> bits, ArrayRef< Ref<ResultPoint> > points, int dimension = 0, float modulesize = 0);
    DetectorResult(Ref<ByteMatrix> gray, ArrayRef< Ref<ResultPoint> > points, int dimension = 0, float modulesize = 0);
    Ref<BitMatrix> getBits();
    Ref<ByteMatrix> getGray();
    void SetGray(Ref<ByteMatrix> gray);
    ArrayRef< Ref<ResultPoint> > getPoints();
    int dimension_;
    float modulesize_;
    
#ifdef CALC_CODE_AREA_SCORE 
    int getMaskPatternNum() { return maskPatternNum_;}
    void setMaskPatternNum(int num) { maskPatternNum_ = num;}
#endif
    
};
}  // namespace zxing

#endif  // __DETECTOR_RESULT_H__
