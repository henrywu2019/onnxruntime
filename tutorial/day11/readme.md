1 filter batch(2-3faster):

```angular2html
(ocr-py38) ✔ ~/wendy/git.repo/onnxruntime/gme_play/day11 [gme_research|✚ 5…36] 
11:31 $ ./run_release.sh  && ./main_release.exe 1 256 400 296 15 8
++ g++ main.cpp ort_conv.cpp ort_conv2.cpp -o main_release.exe -O3 -I/home/henry/wendy/git.repo/onnxruntime/include/ -L/home/henry/wendy/git.repo/onnxruntime/build/Linux/Release -L/home/henry/wendy/git.repo/onnxruntime/build/Linux/Release/_deps/google_nsync-build/ -lonnxruntime_mlas -lonnxruntime_common -lnsync_cpp -std=c++11 -mavx2 -march=native
argc: 7
L1 data cache size: 32768 bytes
L1 data cache line: 64 bytes
L2 data cache size: 262144 bytes
L3 data cache size: 8388608 bytes
input_height:400,input_width:296,input_channel:256,filter_batch:1,kernel_width:3,kernel_height:3,LINE:13
output size: 468048B
filter size: 2304
input total size: 29600.00KB
gme_conv_no_extraction | Compute Time: 28503153 ns
156.96,124.38,17.03,65.25,17.21,-74.60,-4.43,-29.28,-39.19,-77.36,39.73,53.73,-46.55,-54.42,67.88,215.49,
-84.65,120.09,81.95,-25.37,147.42,21.54,-54.43,36.77,35.21,44.65,273.88,-116.74,104.07,-74.26,49.55,-75.00,
...2720.11,-1351.46,353.59,1236.76,-356.50,1348.25,1004.40,2313.01,635.56,1064.17,2123.59,2607.42,1461.01,
-662.00,204.29,362.28,715.78,
==============================================================================
Conv2d Algo: MLAS_NCHWC_CONV_NCHWC_ALGORITHM
MLAS_NCHWC_CONV_NCHWC_ALGORITHM kernel was called: 12736 times.
onnxruntime_conv_nchwc | Compute Time: 158781505 ns
156.96,124.38,17.03,65.25,17.21,-74.60,-4.43,-29.28,-39.19,-77.36,39.73,53.73,-46.55,-54.42,67.88,215.49,
-84.65,120.09,81.95,-25.37,147.42,21.54,-54.43,36.77,35.21,44.65,273.88,-116.74,104.07,-74.26,49.55,-75.00,
...2720.11,-1351.46,353.59,1236.76,-356.50,1348.25,1004.40,2313.01,635.56,1064.17,2123.59,2607.42,1461.01,
-662.00,204.29,362.28,715.78,
==============================================================================
onnxruntime_conv_nchw | MlasConvPrepare Time: 35456 ns
MlasConvAlgorithmExpandThenGemmSegmented is called.
GemmFloatKernel is called: 0 times.
onnxruntime_conv_nchw | Compute Time: 67836117 ns
156.96,124.38,17.03,65.25,17.21,-74.60,-4.43,-29.28,-39.19,-77.36,39.73,53.73,-46.55,-54.42,67.88,215.49,
-84.65,120.09,81.95,-25.37,147.42,21.54,-54.43,36.77,35.21,44.65,273.88,-116.74,104.07,-74.26,49.55,-75.00,
...2720.11,-1351.46,353.59,1236.76,-356.50,1348.25,1004.40,2313.01,635.56,1064.17,2123.59,2607.42,1461.01,
-662.00,204.29,362.28,715.78,
==============================================================================

```

64 filter batch:
```angular2html
(ocr-py38) ✔ ~/wendy/git.repo/onnxruntime/gme_play/day11 [gme_research|✚ 5…36] 
11:32 $ ./run_release.sh  && ./main_release.exe 64 256 400 296 15 8
++ g++ main.cpp ort_conv.cpp ort_conv2.cpp -o main_release.exe -O3 -I/home/henry/wendy/git.repo/onnxruntime/include/ -L/home/henry/wendy/git.repo/onnxruntime/build/Linux/Release -L/home/henry/wendy/git.repo/onnxruntime/build/Linux/Release/_deps/google_nsync-build/ -lonnxruntime_mlas -lonnxruntime_common -lnsync_cpp -std=c++11 -mavx2 -march=native
argc: 7
L1 data cache size: 32768 bytes
L1 data cache line: 64 bytes
L2 data cache size: 262144 bytes
L3 data cache size: 8388608 bytes
input_height:400,input_width:296,input_channel:256,filter_batch:64,kernel_width:3,kernel_height:3,LINE:13
output size: 29955072B
filter size: 147456
input total size: 29600.00KB
gme_conv_no_extraction | Compute Time: 1840393674 ns
156.96,124.38,17.03,65.25,17.21,-74.60,-4.43,-29.28,-39.19,-77.36,39.73,53.73,-46.55,-54.42,67.88,215.49,
-84.65,120.09,81.95,-25.37,147.42,21.54,-54.43,36.77,35.21,44.65,273.88,-116.74,104.07,-74.26,49.55,-75.00,
...-2029.57,
-379.44,-2822.85,2906.91,1379.32,-2813.61,-3597.17,1024.90,-821.29,-1296.09,-752.32,3325.90,2426.19,424.70,887.35,1518.40,-651.12,

==============================================================================
Conv2d Algo: MLAS_NCHWC_CONV_NCHWC_ALGORITHM
MLAS_NCHWC_CONV_NCHWC_ALGORITHM kernel was called: 25472 times.
onnxruntime_conv_nchwc | Compute Time: 420099156 ns
156.96,124.38,17.03,65.25,17.21,-74.60,-4.43,-29.28,-39.19,-77.36,39.73,53.73,-46.55,-54.42,67.88,215.49,
-84.65,120.09,81.95,-25.37,147.42,21.54,-54.43,36.77,35.21,44.65,273.88,-116.74,104.07,-74.26,49.55,-75.00,
...-2029.57,
-379.44,-2822.85,2906.91,1379.32,-2813.61,-3597.17,1024.90,-821.29,-1296.09,-752.32,3325.90,2426.19,424.70,887.35,1518.40,-651.12,

==============================================================================
onnxruntime_conv_nchw | MlasConvPrepare Time: 81969 ns
MlasConvAlgorithmExpandThenGemmSegmented is called.
GemmFloatKernel is called: 181170 times.
onnxruntime_conv_nchw | Compute Time: 407762235 ns
156.96,124.38,17.03,65.25,17.21,-74.60,-4.43,-29.28,-39.19,-77.36,39.73,53.73,-46.55,-54.42,67.88,215.49,
-84.65,120.09,81.95,-25.37,147.42,21.54,-54.43,36.77,35.21,44.65,273.88,-116.74,104.07,-74.26,49.55,-75.00,
...-2029.57,
-379.44,-2822.85,2906.91,1379.32,-2813.61,-3597.17,1024.90,-821.29,-1296.09,-752.32,3325.90,2426.19,424.70,887.35,1518.40,-651.12,

==============================================================================
```

cache:

64 filter batch:

```angular2html
13:36 $ sudo perf stat -d ./main_release.exe 64 256 400 296 8 8
argc: 7
L1 data cache size: 32768 bytes
L1 data cache line: 64 bytes
L2 data cache size: 262144 bytes
L3 data cache size: 8388608 bytes
input_height:400,input_width:296,input_channel:256,filter_batch:64,kernel_width:3,kernel_height:3,LINE:13
output size: 29955072B
filter size: 147456
input total size: 29600.00KB
gme_conv_no_extraction | Compute Time: 1814230232 ns
156.96,124.38,17.03,65.25,17.21,-74.60,-4.43,-29.28,-39.19,-77.36,39.73,53.73,-46.55,-54.42,67.88,215.49,
-84.65,120.09,81.95,-25.37,147.42,21.54,-54.43,36.77,35.21,44.65,273.88,-116.74,104.07,-74.26,49.55,-75.00,
...-2029.57,
-379.44,-2822.85,2906.91,1379.32,-2813.61,-3597.17,1024.90,-821.29,-1296.09,-752.32,3325.90,2426.19,424.70,887.35,1518.40,-651.12,

==============================================================================

 Performance counter stats for './main_release.exe 64 256 400 296 8 8':

          2,334.06 msec task-clock                #    1.000 CPUs utilized          
                 4      context-switches          #    1.714 /sec                   
                 0      cpu-migrations            #    0.000 /sec                   
            37,197      page-faults               #   15.937 K/sec                  
     9,061,115,700      cycles                    #    3.882 GHz                      (49.96%)
    14,348,360,475      instructions              #    1.58  insn per cycle           (62.47%)
     1,458,473,590      branches                  #  624.867 M/sec                    (62.47%)
        18,062,886      branch-misses             #    1.24% of all branches          (62.46%)
     4,647,641,304      L1-dcache-loads           #    1.991 G/sec                    (62.51%)
       738,154,566      L1-dcache-load-misses     #   15.88% of all L1-dcache accesses  (62.55%)
       183,627,227      LLC-loads                 #   78.673 M/sec                    (50.05%)
        22,719,529      LLC-load-misses           #   12.37% of all LL-cache accesses  (50.00%)

       2.334582511 seconds time elapsed

       2.270601000 seconds user
       0.063960000 seconds sys

```

1 filter batch:
```angular2html
13:36 $ sudo perf stat -d ./main_release.exe 1 256 400 296 8 8
argc: 7
L1 data cache size: 32768 bytes
L1 data cache line: 64 bytes
L2 data cache size: 262144 bytes
L3 data cache size: 8388608 bytes
input_height:400,input_width:296,input_channel:256,filter_batch:1,kernel_width:3,kernel_height:3,LINE:13
output size: 468048B
filter size: 2304
input total size: 29600.00KB
gme_conv_no_extraction | Compute Time: 28072638 ns
156.96,124.38,17.03,65.25,17.21,-74.60,-4.43,-29.28,-39.19,-77.36,39.73,53.73,-46.55,-54.42,67.88,215.49,
-84.65,120.09,81.95,-25.37,147.42,21.54,-54.43,36.77,35.21,44.65,273.88,-116.74,104.07,-74.26,49.55,-75.00,
...2720.11,-1351.46,353.59,1236.76,-356.50,1348.25,1004.40,2313.01,635.56,1064.17,2123.59,2607.42,1461.01,
-662.00,204.29,362.28,715.78,
==============================================================================

 Performance counter stats for './main_release.exe 1 256 400 296 8 8':

            550.29 msec task-clock                #    0.999 CPUs utilized          
                 1      context-switches          #    1.817 /sec                   
                 0      cpu-migrations            #    0.000 /sec                   
            29,857      page-faults               #   54.257 K/sec                  
     2,108,693,576      cycles                    #    3.832 GHz                      (50.03%)
     2,498,439,819      instructions              #    1.18  insn per cycle           (62.94%)
       586,283,129      branches                  #    1.065 G/sec                    (62.93%)
        18,515,473      branch-misses             #    3.16% of all branches          (62.94%)
       687,822,570      L1-dcache-loads           #    1.250 G/sec                    (62.93%)
        18,040,037      L1-dcache-load-misses     #    2.62% of all L1-dcache accesses  (62.32%)
         3,385,259      LLC-loads                 #    6.152 M/sec                    (49.42%)
           408,969      LLC-load-misses           #   12.08% of all LL-cache accesses  (49.42%)

       0.550728024 seconds time elapsed

       0.510566000 seconds user
       0.040202000 seconds sys


```