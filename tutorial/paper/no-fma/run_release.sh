#!/bin/bash

if grep -q avx2 /proc/cpuinfo; then
    INSTRUCTION_FLAG="-mavx2"
fi

g++ smm_conv.cpp -O3 -o smm_conv_release.exe -lpthread -std=c++11 $INSTRUCTION_FLAG -march=native
