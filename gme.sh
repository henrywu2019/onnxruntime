#!/bin/bash

set -x

#./build.sh --config Release --enable_pybind --build_wheel --parallel --skip_tests --skip_onnx_tests --use_dnnl
./build.sh --config Release --enable_pybind --build_wheel --parallel --skip_tests --skip_onnx_tests # --use_dnnl --use_tvm --use_cloud

#--cmake_extra_defines onnxruntime_USE_JEMALLOC=ON  --cmake_extra_defines onnxruntime_USE_AVX2=ON



