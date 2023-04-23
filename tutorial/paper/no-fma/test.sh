#!/bin/bash

for i in 4 8 16 24 32 48 64 80 96 128 256 384 512; do ./smm_conv_release.exe $i 256 800 34 7 3; sleep 1; done
for i in 4 8 16 24 32 48 64 80 96 128 256 384 512; do ./zpzr_conv_release.exe $i 256 800 34 7 3; sleep 1; done

