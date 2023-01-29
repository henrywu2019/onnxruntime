#!/bin/bash

set -x

python -V

./build.sh --build_dir ~/wendy/git.repo/onnxruntime/cmake/cmake-build-debug --config Debug --skip_tests --skip_onnx_tests \
--parallel --build_shared_lib \
--cmake_extra_defines onnxruntime_DEBUG_NODE_INPUTS_OUTPUTS=1 \
--cmake_extra_defines onnxruntime_BUILD_BENCHMARKS=1 \
--cmake_extra_defines onnxruntime_BUILD_SHARED_LIB=ON \
&&\
pushd cmake/cmake-build-debug/Debug/ && \
yes |sudo cp -fP *.so* /usr/local/lib/ && \
popd





