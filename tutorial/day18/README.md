# 4 times faster

## tunable_x is 8

```angular2html
✔ /media/henry/wendy/git.repo/onnxruntime/gme_play/day18 [gme_research|●1✚ 4…55]
00:20 $ ./run_tunable_release.sh && ./main_tunable_release.exe 64 256 48 48 1 8
+ g++ tunable_conv.cpp main_tunable.cpp -o main_tunable_release.exe -O3 -I/home/henry/wendy/git.repo/onnxruntime/include/ -L/home/henry/wendy/git.repo/onnxruntime/build/Linux/Release -L/home/henry/wendy/git.repo/onnxruntime/build/Linux/Release/_deps/google_nsync-build/ -lonnxruntime_mlas -lonnxruntime_common -lnsync_cpp -std=c++20 -mavx2 -march=native
filter size: 147456
input total size: 576.00KB
reorder_input: 1207262 ns
reorder_filter: 311647 ns
called: 0
run | algo run Time: 78,107,129 ns
restore_output: 308684 ns
main | total algo Time: 79976908 ns
2914771.25,2955667.25,2996563.00,3037459.00,3078355.25,3119251.25,3160147.25,3201042.75,3241939.25,3282835.25,3323731.25,3364627.25,3405523.00,3446419.50,3487315.25,3528211.00,
3569107.25,3610002.75,3650899.50,3691795.25,3732691.25,3773587.50,3814482.50,3855379.50,3896275.25,3937171.25,3978067.50,4018963.00,4059859.50,4100755.50,4141651.25,4182547.50,
...157805328.00,
157875264.00,157945168.00,158015120.00,158085024.00,158154960.00,158224848.00,158294784.00,158364736.00,158434688.00,158504576.00,158574512.00,158644416.00,158714368.00,158784272.00,158854240.00,158924128.00,
✔ /media/henry/wendy/git.repo/onnxruntime/gme_play/day18 [gme_research|●1✚ 4…55] 
00:21 $ ./run_tunable_release.sh && ./main_tunable_release.exe 64 256 400 296 1 8
+ g++ tunable_conv.cpp main_tunable.cpp -o main_tunable_release.exe -O3 -I/home/henry/wendy/git.repo/onnxruntime/include/ -L/home/henry/wendy/git.repo/onnxruntime/build/Linux/Release -L/home/henry/wendy/git.repo/onnxruntime/build/Linux/Release/_deps/google_nsync-build/ -lonnxruntime_mlas -lonnxruntime_common -lnsync_cpp -std=c++20 -mavx2 -march=native
filter size: 147456
input total size: 29600.00KB
reorder_input: 61762307 ns
reorder_filter: 324386 ns
called: 0
run | algo run Time: 4207,999,936 ns
restore_output: 17305057 ns
main | total algo Time: 4287458090 ns
14199763.00,14240660.00,14281554.00,14322452.00,14363348.00,14404243.00,14445140.00,14486035.00,14526932.00,14567828.00,14608723.00,14649621.00,14690515.00,14731412.00,14772308.00,14813203.00,
14854101.00,14894995.00,14935892.00,14976788.00,15017683.00,15058581.00,15099476.00,15140372.00,15181268.00,15222163.00,15263061.00,15303956.00,15344852.00,15385748.00,15426643.00,15467541.00,
...8259781632.00,
8259851264.00,8259920896.00,8259991552.00,8260061696.00,8260130816.00,8260200448.00,8260270592.00,8260342272.00,8260411392.00,8260481024.00,8260551680.00,8260621312.00,8260690944.00,8260760576.00,8260829696.00,8260900864.00,
```

## tunable_x is 32

```angular2html
00:20 $ ./run_tunable_release.sh && ./main_tunable_release.exe 64 256 48 48 1 8
+ g++ tunable_conv.cpp main_tunable.cpp -o main_tunable_release.exe -O3 -I/home/henry/wendy/git.repo/onnxruntime/include/ -L/home/henry/wendy/git.repo/onnxruntime/build/Linux/Release -L/home/henry/wendy/git.repo/onnxruntime/build/Linux/Release/_deps/google_nsync-build/ -lonnxruntime_mlas -lonnxruntime_common -lnsync_cpp -std=c++20 -mavx2 -march=native
filter size: 147456
input total size: 576.00KB
reorder_input: 1148779 ns
reorder_filter: 304134 ns
called: 0
run | algo run Time: 21,055,813 ns
restore_output: 415017 ns
main | total algo Time: 22962734 ns
2914771.00,2955667.00,2996563.25,3037459.50,3078355.50,3119251.25,3160147.00,3201043.25,3241939.50,3282835.00,3323731.00,3364627.00,3405523.25,3446419.50,3487315.50,3528211.00,
3569107.00,3610003.25,3650899.00,3691795.00,3732691.25,3773587.25,3814483.00,3855379.50,3896275.50,3937171.25,3978067.00,4018963.00,4059859.00,4100755.50,4141651.25,4182547.00,
...157805360.00,
157875264.00,157945136.00,158015072.00,158085040.00,158155024.00,158224928.00,158294832.00,158364688.00,158434624.00,158504656.00,158574544.00,158644448.00,158714336.00,158784272.00,158854304.00,158924160.00,

✔ /media/henry/wendy/git.repo/onnxruntime/gme_play/day18 [gme_research|●1✚ 4…55] 
00:21 $ ./run_tunable_release.sh && ./main_tunable_release.exe 64 256 400 296 1 8
+ g++ tunable_conv.cpp main_tunable.cpp -o main_tunable_release.exe -O3 -I/home/henry/wendy/git.repo/onnxruntime/include/ -L/home/henry/wendy/git.repo/onnxruntime/build/Linux/Release -L/home/henry/wendy/git.repo/onnxruntime/build/Linux/Release/_deps/google_nsync-build/ -lonnxruntime_mlas -lonnxruntime_common -lnsync_cpp -std=c++20 -mavx2 -march=native
filter size: 147456
input total size: 29600.00KB
reorder_input: 62687916 ns
reorder_filter: 284439 ns
called: 0
run | algo run Time: 1144,534,032 ns
restore_output: 41782670 ns
main | total algo Time: 1249357984 ns
14199764.00,14240660.00,14281555.00,14322451.00,14363348.00,14404244.00,14445139.00,14486035.00,14526931.00,14567828.00,14608724.00,14649620.00,14690515.00,14731411.00,14772306.00,14813204.00,
14854100.00,14894995.00,14935891.00,14976787.00,15017684.00,15058578.00,15099475.00,15140370.00,15181268.00,15222164.00,15263059.00,15303955.00,15344851.00,15385747.00,15426644.00,15467539.00,
...8259782144.00,
8259851264.00,8259921408.00,8259991552.00,8260062208.00,8260131840.00,8260201472.00,8260271104.00,8260341248.00,8260410368.00,8260481024.00,8260551168.00,8260620288.00,8260690944.00,8260761088.00,8260830720.00,8260900864.00,
```


After Unrolling register loop:

```angular2html
✔ ~/wendy/git.repo/onnxruntime/gme_play/day18 [gme_research|✚ 3…55] 
10:42 $ sudo perf record ./main_tunable_release.exe 64 256 47 47 1 8
[sudo] password for henry: 
filter size: 147456
input total size: 552.25KB
reorder_input: 1092212 ns
reorder_filter: 337569 ns
run_unrolled | algo run Time: 5951552 ns
restore_output: 426139 ns
main | total algo Time: 7827795 ns
635737.50,644075.00,652412.69,660750.25,669087.81,677425.44,685763.06,694100.69,702438.25,710775.81,719113.50,727451.00,735788.69,744126.19,752463.88,760801.50,
769139.06,777476.62,785814.25,794151.81,802489.50,810827.00,819164.69,827502.25,835839.88,844177.56,852514.94,860852.69,869190.25,877527.88,885865.50,894202.94,
...9070483456.00,
9074683904.00,9078887424.00,9083087872.00,9087289344.00,9091492864.00,9095694336.00,9099894784.00,9104097280.00,9108296704.00,9112499200.00,9116701696.00,9120903168.00,9125104640.00,9129306112.00,9133507584.00,9137709056.00,
[ perf record: Woken up 1 times to write data ]
[ perf record: Captured and wrote 0.027 MB perf.data (32 samples) ]
✔ ~/wendy/git.repo/onnxruntime/gme_play/day18 [gme_research|✚ 3…56] 
10:45 $ ./main_release.exe 64 256 47 47 1 8
argc: 7
input_height:47,input_width:47,input_channel:256,filter_batch:64,kernel_width:3,kernel_height:3,LINE:5
output size: 518400B
filter size: 147456
input total size: 552.25KB
onnxruntime_conv_nchwc | Reorder Time: 5134956 ns
Conv2d Algo: MLAS_NCHWC_CONV_NCHWC_ALGORITHM
MLAS_NCHWC_CONV_NCHWC_ALGORITHM kernel was called: 2880 times.
onnxruntime_conv_nchwc | Algo Time: 16648994 ns
onnxruntime_conv_nchwc | restore MlasReorderOutputNchw: 80992 ns
onnxruntime_conv_nchwc | entire Algo Time: 21873744 ns
2869267.00,2910163.50,2951059.25,2991954.50,3032851.25,3073747.25,3114643.00,3155539.25,3196435.00,3237331.50,3278227.25,3319123.25,3360019.25,3400915.00,3441811.50,3482707.25,
3523603.25,3564499.25,3605395.00,3646291.50,3687187.25,3728083.50,3768979.50,3809875.00,3850771.50,3891667.25,3932563.50,3973459.25,4014354.50,4055251.50,4096147.25,4137043.25,
...151227632.00,
151297536.00,151367456.00,151437424.00,151507312.00,151577264.00,151647184.00,151717120.00,151787056.00,151856976.00,151926880.00,151996816.00,152066736.00,152136688.00,152206592.00,152276528.00,152346464.00,

==============================================================================
```