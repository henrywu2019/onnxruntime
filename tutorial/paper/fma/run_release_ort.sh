#!/bin/bash

if grep -q avx2 /proc/cpuinfo; then
    INSTRUCTION_FLAG="-mavx2"
else
    echo "No AVX2 suport! Exit!!!"
    exit
fi

ORT_HOME=/home/opc/workspace/onnxruntime

g++ ort_conv_main.cpp  ort_conv.cpp common.cpp -O3 -o ort_conv_main_release.exe -lpthread -std=c++11 $INSTRUCTION_FLAG -march=native -I${ORT_HOME}/include/ \
-L${ORT_HOME}/build/Linux/Release/ \
-L${ORT_HOME}/build/Linux/Release/_deps/google_nsync-build \
    -lonnxruntime_mlas -lonnxruntime_common -lnsync_cpp -std=c++11 -mavx2 -march=native \
    -fuse-linker-plugin -mtune=native -lpthread

#g++ zpzr_fma_main.cpp common.cpp -O3 -o zpzr_fma_main_release.exe -lpthread -std=c++11 $INSTRUCTION_FLAG -march=native
