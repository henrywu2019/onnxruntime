#!/bin/bash

for i in 4 8 16 24 32 48 64 80 96 128 256 384 512; do ./fast_conv_release.exe $i 256 34 800 2 8 16 0 3; done
for i in 4 8 16 24 32 48 64 80 96 128 256 384 512; do ./main_release.exe $i 256 34 800 1 8; done
