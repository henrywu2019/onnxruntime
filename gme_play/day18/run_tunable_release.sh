#!/bin/bash

set -x

g++ tunable_conv.cpp main_tunable.cpp -o main_tunable_release.exe -O3 -I/home/henry/wendy/git.repo/onnxruntime/include/ \
-L/home/henry/wendy/git.repo/onnxruntime/build/Linux/Release \
-L/home/henry/wendy/git.repo/onnxruntime/build/Linux/Release/_deps/google_nsync-build/ \
 -lonnxruntime_mlas -lonnxruntime_common -lnsync_cpp -std=c++20 -mavx2 -march=native


