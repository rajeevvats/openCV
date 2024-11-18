// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Tencent is pleased to support the open source community by making WeChat QRCode available.
// Copyright (C) 2020 THL A29 Limited, a Tencent company. All rights reserved.
//
// Modified from ZXing. Copyright ZXing authors.
// Licensed under the Apache License, Version 2.0 (the "License").

//
//  dmtxsymbol.hpp
//  test_dm
//
//  Created by wechatcv on 2022/5/7.
//

#ifndef __ZXING_DATAMATRIX_LIBDMTX_DMTXSYMBOL_HPP__
#define __ZXING_DATAMATRIX_LIBDMTX_DMTXSYMBOL_HPP__

#include <stdio.h>

namespace dmtx {

int dmtxGetSymbolAttribute(int attribute, int sizeIdx);
int dmtxGetBlockDataSize(int sizeIdx, int blockIdx);
int getSizeIdxFromSymbolDimension(int rows, int cols);
int dmtxGetSymbolAttribute(int attribute, int sizeIdx);
}  // namespace dmtx
#endif // __ZXING_DATAMATRIX_LIBDMTX_DMTXSYMBOL_HPP__
