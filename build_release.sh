#!/bin/bash

set -x

pip install numpy==1.23.5

./build.sh --config Release --parallel --skip_tests --skip_onnx_tests --build_shared_lib --enable_pybind --build_wheel && \
find build -name '*whl'

