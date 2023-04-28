#!/bin/bash

set -x

ORT_HOME=/home/opc/workspace/onnxruntime

g++ tunable_conv.cpp main_tunable.cpp conv2d.cpp common.cpp ort_conv.cpp -o main_tunable_release.exe -O3  -I${ORT_HOME}/include/ \
-L${ORT_HOME}/build/Linux/Release/ \
-L${ORT_HOME}/build/Linux/Release/_deps/google_nsync-build \
 -lonnxruntime_mlas -lonnxruntime_common -lnsync_cpp -std=c++20 -mavx2 -march=native -lpthread
