#!/bin/bash

set -x

#for i in 4 8 16 24 32 48 64 80 96 128 256 384 512; do ./zpzr_fma_main_release.exe $i 128 24 34 2 3 16; sleep 1; done
for i in 4 8 16 24 32 48 64 80 96 128 256 384 512; do ./conv_zpzr_main_release.exe $i 64 24 34 2 3 32; sleep 60; w; done

for i in 4 8 16 24 32 48 64 80 96 128 256 384 512; do ./conv_ort_main_release.exe $i 64 24 34 1 3; sleep 60; w; done
