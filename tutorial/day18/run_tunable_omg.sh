#!/bin/bash

set -x

ROOT=/home/henry/wendy/git.repo/onnxruntime
ROOT=/home/opc/workspace/onnxruntime

g++ tunable_conv.cpp main_tunable.cpp ort_conv.cpp common.cpp conv2d.cpp -o main_tunable_omg20230423.exe -O3 \
-L$ROOT/build/Linux/Release \
-L$ROOT/build/Linux/Release/_deps/google_nsync-build \
-lonnxruntime_mlas -lonnxruntime_common -lnsync_cpp -lpthread -std=c++20 -mavx2 -march=native
