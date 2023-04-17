//
// Created by henry on 1/21/23.
//

#ifndef ONNXRUNTIME_ORT_H
#define ONNXRUNTIME_ORT_CONV_H

#include <bits/stdc++.h>
#include "/home/opc/workspace/onnxruntime/onnxruntime/core/mlas/inc/mlas.h"

using namespace std;

// MlasNchwcConv(long const*, long const*, long const*, long const*, long const*, long const*, unsigned long, float const*, float const*, float const*, float*, MLAS_ACTIVATION const*, bool, onnxruntime::concurrency::ThreadPool*)

void ReorderInputNchw(const int64_t* input_shape, const float* S, float* D);

void onnxruntime_conv_nchw(size_t BatchCount,
                           size_t GroupCount,
                           size_t InputChannels,
                           size_t InputHeight,
                           size_t InputWidth,
                           size_t FilterCount,
                           size_t KernelHeight,
                           size_t KernelWidth,
                           size_t PaddingLeftHeight,
                           size_t PaddingLeftWidth,
                           size_t PaddingRightHeight,
                           size_t PaddingRightWidth,
                           size_t DilationHeight,
                           size_t DilationWidth,
                           size_t StrideHeight,
                           size_t StrideWidth,
                           size_t OutputHeight,
                           size_t OutputWidth,
                           const float* Input,
                           const float* Filter,
                           const float* Bias,
                           float* Output);

void onnxruntime_conv_nchwc(
    size_t BatchCount,
    size_t GroupCount,
    size_t InputChannels,
    size_t InputHeight,
    size_t InputWidth,
    size_t FilterCount,
    size_t KernelHeight,
    size_t KernelWidth,
    size_t PaddingLeftHeight,
    size_t PaddingLeftWidth,
    size_t PaddingRightHeight,
    size_t PaddingRightWidth,
    size_t DilationHeight,
    size_t DilationWidth,
    size_t StrideHeight,
    size_t StrideWidth,
    size_t OutputHeight,
    size_t OutputWidth,
    const float* Input,
    const float* Filter,
    const float* Bias,
    float* Output);

#endif  // ONNXRUNTIME_ORT_H
