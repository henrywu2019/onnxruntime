#!/bin/bash

./build.sh --config Release --build_shared_lib --parallel --build_wheel --skip_tests --skip_onnx_tests --skip_winml_tests --use_cuda --cuda_version 11.6 --use_tensorrt --cuda_home /usr/local/cuda --cudnn_home /usr/lib/x86_64-linux-gnu --tensorrt_home /usr/lib/x86_64-linux-gnu


