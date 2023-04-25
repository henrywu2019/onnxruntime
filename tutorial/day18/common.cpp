//
// Created by henry on 2/21/23.
//
#include "gamma_common.h"
#include "conv2d.h"
#include "sein.hpp"
#include <immintrin.h>



void print_matrix(float* m, int h, int w) {
  if (h * w > 100) return;
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w - 1; j++)
      printf("%.1f,", m[i * w + j]);
    printf("%.1f\n", m[i * w + w - 1]);
  }
}

int ceil_int(int x, int y) {
  return int(ceil((float)x / y));
}

int floor_int(int x, int y) {
  return int(floor((float)x / y));
}

bool array_equal(float* a,float* b,int z,float tolerance){
  REP(i,0,z){
    if(a[i]!=b[i] and abs((a[i]-b[i])/a[i])>tolerance){
      return false;
    }
  }
  return true;
}