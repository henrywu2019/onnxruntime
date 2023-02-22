#!/bin/bash

set -x

ROOT=/home/henry/wendy/git.repo/onnxruntime
ROOT=/home/opc/mlops/onnxruntime

g++ tunable_conv.cpp main_tunable.cpp -o main_tunable_omg.exe -I${ROOT}/gme_play/gamma/include \
-L$ROOT/build/Linux/Release \
-L$ROOT/build/Linux/Release/_deps/google_nsync-build \
-lgamma -lonnxruntime_mlas -lonnxruntime_common -lnsync_cpp -lpthread -std=c++20 -O3 -mavx2 -march=native
