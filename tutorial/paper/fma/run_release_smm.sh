#!/bin/bash

if grep -q avx2 /proc/cpuinfo; then
    INSTRUCTION_FLAG="-mavx2"
else
    echo "No AVX2 suport! Exit!!!"
    exit
fi

g++ smm_conv.cpp common.cpp -O3 -o conv_smm_main_release.exe -std=c++11 $INSTRUCTION_FLAG -march=native
