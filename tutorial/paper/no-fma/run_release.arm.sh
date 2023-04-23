#!/bin/bash

if grep -q avx2 /proc/cpuinfo; then
    INSTRUCTION_FLAG="-mavx2"
fi

g++ smm_conv.cpp common.cpp -O3 -o smm_conv_release.exe -std=c++11 $INSTRUCTION_FLAG -march=armv8-a -mfpu=neon-fp-armv8

g++ zpzr_conv.cpp common.cpp -O3 -o zpzr_conv_release.exe -std=c++11 $INSTRUCTION_FLAG -march=armv8-a -mfpu=neon-fp-armv8
