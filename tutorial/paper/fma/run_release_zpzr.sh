#!/bin/bash

if grep -q avx2 /proc/cpuinfo; then
    INSTRUCTION_FLAG="-mavx2"
else
    echo "No AVX2 suport! Exit!!!"
    exit
fi

g++ zpzr_fma_main.cpp common.cpp zpzr_conv.cpp -O3 -o conv_zpzr_main_release.exe -lpthread -std=c++11 $INSTRUCTION_FLAG -march=native
