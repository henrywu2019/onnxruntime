#!/bin/bash

if grep -q avx2 /proc/cpuinfo; then
    INSTRUCTION_FLAG="-mavx2"
else
    echo "No AVX2 suport! Exit!!!"
    exit
fi

g++ ort_fma.cpp common.cpp -O0 -g -o smm_conv_debug.exe -lpthread -std=c++11 $INSTRUCTION_FLAG -march=native

g++ zpzr_fma.cpp common.cpp -O0 -g -o zpzr_conv_debug.exe -lpthread -std=c++11 $INSTRUCTION_FLAG -march=native
