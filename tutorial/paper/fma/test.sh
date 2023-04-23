#!/bin/bash

for i in 4 8 16 24 32 48 64 80 96 128 256 384 512; do ./ort_conv_main_release.exe $i 256 800 34 1 3; sleep 1; done
for i in 4 8 16 24 32 48 64 80 96 128 256 384 512; do ./zpzr_fma_main_release.exe $i 256 800 34 2 3 16; sleep 1; done
