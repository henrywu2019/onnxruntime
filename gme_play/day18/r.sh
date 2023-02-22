#!/bin/bash

set -x

g++ test_sse4.cpp -o test_sse4.exe -I/home/henry/wendy/git.repo/onnxruntime/gme_play/gamma/include \
-L/home/henry/wendy/git.repo/onnxruntime/build/Linux/Release \
 -L/home/henry/wendy/git.repo/onnxruntime/build/Linux/Release/_deps/google_nsync-build \
  -lgamma_shared -lonnxruntime_mlas -lonnxruntime_common -lnsync_cpp -std=c++20 -O3 -mavx2 -march=native
