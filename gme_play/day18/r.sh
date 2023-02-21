#!/bin/bash

set -x

c++ test_sse4.cpp -o test_sse4.exe \
-L/home/henry/wendy/git.repo/onnxruntime/build/Linux/Release -lgamma -std=c++20 -mavx2 -march=native
