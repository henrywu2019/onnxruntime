
```angular2html
12:10 $ ./run_test.sh && ./wfh_test.exe
++ g++ wfh_test.cpp -o wfh_test.exe -O3 -std=c++11 -mavx2 -march=native
filter size: 2304
input total size: 29600.00KB
reorder_input | reorder Time: 58801736 ns
run | algo run Time: 57445044 ns
main | total algo Time: 116313323 ns

```