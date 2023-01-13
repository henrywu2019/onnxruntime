#!/bin/bash

set -x

./build.sh --build_dir ~/wendy/git.repo/onnxruntime/cmake/cmake-build-debug --config Debug --skip_tests --skip_onnx_tests --parallel --build_shared_lib &&\
pushd cmake/cmake-build-debug/Debug/ && \
yes |sudo cp -fP *.so* /usr/local/lib/ && \
popd






