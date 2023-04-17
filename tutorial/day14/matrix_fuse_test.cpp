#include <bits/stdc++.h>
#include "immintrin.h"
#include "fmaintrin.h"


/*
 * int h=340, int w=340
 * 13:43 $ ./fuse_test.exe
matrix_fuse | matrix fuse time: 263451 ns
matrix_fuse2 | matrix fuse time: 55771 ns
 * */

using namespace std;
using namespace chrono;
void print_matrix(float* m, int h, int w) {
  if (h*w>10000) return;
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w - 1; j++)
      printf("%.1f,", m[i * w + j]);
    printf("%.1f\n", m[i * w + w - 1]);
  }
}

//assuming h2 and h2 are 8*int
/*
31.61 │ 48:   vmovaps    (%r14,%rax,4),%ymm1
31.41 │       vaddps     (%r12,%rax,4),%ymm1,%ymm0
36.98 │       vmovaps    %ymm0,(%rbx,%rax,4)
      │       add        $0x8,%rax
      │       cmp        %eax,%r8d
      │     ↑ jg         48
matrix_fuse | matrix fuse time: 21,046,143 ns
20:46 $ sudo perf stat -d ./fuse_test.exe
matrix_fuse | matrix fuse time: 21697943 ns

 Performance counter stats for './fuse_test.exe':

             68.49 msec task-clock                #    0.993 CPUs utilized
                 1      context-switches          #   14.600 /sec
                 0      cpu-migrations            #    0.000 /sec
            33,973      page-faults               #  496.001 K/sec
       252,123,188      cycles                    #    3.681 GHz                      (52.12%)
       183,833,982      instructions              #    0.73  insn per cycle           (64.94%)
        32,213,842      branches                  #  470.317 M/sec                    (64.96%)
            64,320      branch-misses             #    0.20% of all branches          (64.96%)
        44,314,123      L1-dcache-loads           #  646.979 M/sec                    (64.99%)
         5,222,496      L1-dcache-load-misses     #   11.79% of all L1-dcache accesses  (59.54%)
         1,353,270      LLC-loads                 #   19.758 M/sec                    (46.72%)
           485,600      LLC-load-misses           #   35.88% of all LL-cache accesses  (46.70%)

       0.068997818 seconds time elapsed

       0.016251000 seconds user
       0.052816000 seconds sys
 * */
void matrix_fuse(float* dest, int h, int w, float *delta, int h2, int w2, float* tmp){ // 1.57us
  auto start = high_resolution_clock::now();
  __m256 x={}, y={}, z={};
  //int called = 0;
  for(int i=0;i<h2*w2;i+=8){
    x = _mm256_loadu_ps(dest+i);
    y = _mm256_loadu_ps(delta+i);
    z = _mm256_add_ps(x,y);
    ///called++;
    _mm256_storeu_ps(tmp+i, z);
  }
  /*for(int i=0;i<h2;i++){
    ::memcpy(dest+i*w, tmp+i*w2, w2*sizeof(float));
  }*/
  long long t = duration_cast<nanoseconds>((high_resolution_clock::now() - start)).count(); cout << __FUNCTION__ << " | matrix fuse time: " << t << " ns" << endl;
  //printf("called:%d\n", called);
}

/*
46.13 │ 40:   vmovups    0x4(%rbx,%rax,4),%ymm1
50.17 │       vaddps     (%r14,%rax,4),%ymm1,%ymm0
 3.70 │       vmovups    %ymm0,0x4(%rbx,%rax,4)
      │       add        $0x8,%rax
      │       cmp        %eax,%r8d
      │     ↑ jg         40
matrix_fuse2 | matrix fuse time: 6,593,020 ns
20:45 $ sudo perf stat -d ./fuse_test.exe
matrix_fuse2 | matrix fuse time: 6141563 ns

 Performance counter stats for './fuse_test.exe':

             49.64 msec task-clock                #    0.986 CPUs utilized
                 6      context-switches          #  120.881 /sec
                 0      cpu-migrations            #    0.000 /sec
            22,698      page-faults               #  457.293 K/sec
       188,707,454      cycles                    #    3.802 GHz                      (39.21%)
       108,173,527      instructions              #    0.57  insn per cycle           (55.32%)
        19,500,722      branches                  #  392.878 M/sec                    (63.38%)
            44,563      branch-misses             #    0.23% of all branches          (71.33%)
        30,915,178      L1-dcache-loads           #  622.842 M/sec                    (75.93%)
         4,029,090      L1-dcache-load-misses     #   13.03% of all L1-dcache accesses  (68.75%)
           271,749      LLC-loads                 #    5.475 M/sec                    (44.67%)
            84,622      LLC-load-misses           #   31.14% of all LL-cache accesses  (36.73%)

       0.050348689 seconds time elapsed

       0.019282000 seconds user
       0.030851000 seconds sys
 * */
void matrix_fuse_direct(float* dest, int h, int w, float *delta, int h2, int w2){ // 1.57us
  auto start = high_resolution_clock::now();
  __m256 x={}, y={}, z={};
  for(int i=0;i<h2*w2;i+=8){
    x = _mm256_loadu_ps(dest+i+1);
    y = _mm256_load_ps(delta+i);
    z = _mm256_add_ps(x,y);
    _mm256_storeu_ps(dest+i+1, z);
  }
  long long t = duration_cast<nanoseconds>((high_resolution_clock::now() - start)).count(); cout << __FUNCTION__ << " | matrix fuse time: " << t << " ns" << endl;
}

void matrix_fuse_by_memcpy(float* dest, int h, int w, float *delta, int h2, int w2){ // 1.57us
  auto start = high_resolution_clock::now();
  __m256 x={}, y={}, z={};
  for(int i=0;i<h2;i+=8){
    ::memcpy(dest+i*w,delta+i*w2,sizeof(float)*w2);
  }
  long long t = duration_cast<nanoseconds>((high_resolution_clock::now() - start)).count(); cout << __FUNCTION__ << " | memcpy time: " << t << " ns" << endl;
}

void test_matrix_fuse(int h=400, int w=298){
  int h2=h-2, w2=w-2;
  float* dest=(float*)_mm_malloc(sizeof(float) * (h*w), 32);
  float* delta=(float*)_mm_malloc(sizeof(float) * (h2*w2), 32);
  float* tmp=(float*)_mm_malloc(sizeof(float) * (h*w), 32);
  for (int i = 0; i < h; i++)
    for (int j = 0; j < w; j++)
      dest[i*w+j] = 1; //i*w+j;

  for (int i = 0; i < h2; i++)
    for (int j = 0; j < w2; j++)
      delta[i*w2+j] = 2; //i*w2+j-1;
  //matrix_fuse(dest,h,w,delta,h2,w2,tmp);
  matrix_fuse_direct(dest,h,w,delta,h2,w2);
  matrix_fuse_by_memcpy(dest,h,w,delta,h2,w2);
  print_matrix(dest, h, w);
  _mm_free(dest), _mm_free(delta), _mm_free(tmp);
}

int main(int argc, char** argv) {
  test_matrix_fuse();
  return 0;
}