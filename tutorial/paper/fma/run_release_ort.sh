#!/bin/bash

INSTRUCTION_FLAG="-march=armv8-a -mcpu=cortex-a57"

ORT_HOME=/home/fuheng/onnxruntime
ORT_HOME=/home/opc/workspace/onnxruntime

g++ ort_conv_main.cpp ort_conv.cpp common.cpp -O3 -o conv_ort_main_release.exe -lpthread -std=c++11 $INSTRUCTION_FLAG -march=native -I${ORT_HOME}/include/ \
-L${ORT_HOME}/build/Linux/Release/ \
-L${ORT_HOME}/build/Linux/Release/_deps/google_nsync-build \
    -lonnxruntime_mlas -lonnxruntime_common -lnsync_cpp -std=c++11 -march=native \
    -fuse-linker-plugin -mtune=native -lpthread

#g++ zpzr_fma_main.cpp common.cpp -O3 -o zpzr_fma_main_release.exe -lpthread -std=c++11 $INSTRUCTION_FLAG -march=native
