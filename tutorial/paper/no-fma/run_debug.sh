#!/bin/bash

if grep -q avx2 /proc/cpuinfo; then
    INSTRUCTION_FLAG="-mavx2"
fi

g++ smm_conv.cpp common.cpp -O0 -g -o smm_conv_debug.exe -lpthread -std=c++11 $INSTRUCTION_FLAG -march=native

g++ zpzr_conv.cpp common.cpp -O0 -g -o zpzr_conv_debug.exe -lpthread -std=c++11 $INSTRUCTION_FLAG -march=native

g++ zpzr_conv_co_inner.cpp common.cpp -O0 -g -o zpzr_conv_co_inner_debug.exe -std=c++11 $INSTRUCTION_FLAG -march=native
