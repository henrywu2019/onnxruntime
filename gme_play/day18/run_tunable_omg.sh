#!/bin/bash

set -x

g++ tunable_conv.cpp main_tunable.cpp -o main_tunable_omg.exe -O3 -I/home/opc/mlops/onnxruntime/include/ \
-L/home/opc/mlops/onnxruntime/build_main/Release \
-L/home/opc/mlops/onnxruntime/build_main/Release/_deps/google_nsync-build/ \
-lonnxruntime_mlas -lonnxruntime_common -lnsync_cpp -lpthread -std=c++20 -mavx2 -march=native
