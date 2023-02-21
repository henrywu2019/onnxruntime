#!/bin/bash

set -x

Python -V

./build.sh --config Release --skip_tests --skip_onnx_tests \
--parallel --build_shared_lib \
--cmake_extra_defines onnxruntime_DEBUG_NODE_INPUTS_OUTPUTS=0 \
--cmake_extra_defines onnxruntime_BUILD_BENCHMARKS=0 \
--cmake_extra_defines onnxruntime_BUILD_SHARED_LIB=ON \
 && pushd build/Linux/Release && yes |sudo cp -fP *.so* /usr/local/lib/





