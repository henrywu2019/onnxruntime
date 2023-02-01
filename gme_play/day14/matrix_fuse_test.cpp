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
void matrix_fuse(float* dest, int h, int w, float *delta, int h2, int w2, float* tmp){ // 1.57us
  auto start = high_resolution_clock::now();
  __m256 x={}, y={}, z={};
  for(int i=0;i<h2*w2;i+=8){
    x = _mm256_load_ps(dest+i);
    y = _mm256_load_ps(delta+i);
    z = _mm256_add_ps(x,y);
    _mm256_store_ps(tmp+i, z);
  }
  /*for(int i=0;i<h2;i++){
    ::memcpy(dest+i*w, tmp+i*w2, w2*sizeof(float));
  }*/
  long long t = duration_cast<nanoseconds>((high_resolution_clock::now() - start)).count(); cout << __FUNCTION__ << " | matrix fuse time: " << t << " ns" << endl;
}

void matrix_fuse2(float* dest, int h, int w, float *delta, int h2, int w2){ // 1.57us
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

void test_matrix_fuse(int h=3400, int w=3400){
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
  matrix_fuse(dest,h,w,delta,h2,w2,tmp);
  matrix_fuse2(dest,h,w,delta,h2,w2);
  print_matrix(dest, h, w);
  _mm_free(dest), _mm_free(delta), _mm_free(tmp);
}

int main(int argc, char** argv) {
  test_matrix_fuse();
  return 0;
}