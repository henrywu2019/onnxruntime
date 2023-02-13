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