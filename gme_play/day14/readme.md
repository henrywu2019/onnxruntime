
0.29ns to store to a register

```angular2html
18:16 $ ./run_test.sh  && ./wfh_test.exe 64 256 400 296
++ g++ wfh_test.cpp -o wfh_test.exe -O3 -std=c++11 -mavx2 -march=native
++ g++ matrix_fuse_test.cpp -o fuse_test.exe -O3 -std=c++11 -mavx2 -march=native
filter size: 147456
input total size: 29600.00KB
reorder_input | reorder Time: 57352229 ns
called:8211456
run | algo run Time: 21019160 ns
main | total algo Time: 78769370 ns
✔ ~/wendy/git.repo/onnxruntime/gme_play/day14 [gme_research|✚ 1…47] 
18:17 $ ./run_test.sh  && ./wfh_test.exe 64 256 400 296
++ g++ wfh_test.cpp -o wfh_test.exe -O3 -std=c++11 -mavx2 -march=native
++ g++ matrix_fuse_test.cpp -o fuse_test.exe -O3 -std=c++11 -mavx2 -march=native
filter size: 147456
input total size: 29600.00KB
reorder_input | reorder Time: 65553774 ns
called:8211456
run | algo run Time: 23425487 ns
main | total algo Time: 89351840 ns
✔ ~/wendy/git.repo/onnxruntime/gme_play/day14 [gme_research|✚ 1…47] 
18:17 $ python
Python 3.9.12 (main, Jun  1 2022, 11:38:51) 
[GCC 7.5.0] :: Anaconda, Inc. on linux
Type "help", "copyright", "credits" or "license" for more information.
>>> 23425487 - 21019160
2406327
>>> (23425487 - 21019160)/8211456
0.2930451067386831
>>>
```

258ns to store to a memory address in RAM.