#!/bin/bash

set -x

ROOT=/home/henry/wendy/git.repo/onnxruntime
ROOT=~/workspace/onnxruntime

g++ test_sse4.cpp -o test_sse4.exe -I${ROOT}/gme_play/gamma/include \
-L$ROOT/build/Linux/Release \
 -L$ROOT/build/Linux/Release/_deps/google_nsync-build \
  -lgamma -lonnxruntime_mlas -lonnxruntime_common -lnsync_cpp -std=c++20 -O3 -mavx2 -march=native
