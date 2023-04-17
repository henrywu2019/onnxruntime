#include <sein.hpp>
#include "immintrin.h"
#include "fmaintrin.h"

void test_matrix_fuse(float* dest, float* delta, int h=398, int w=296){
  auto_profiler ap(__FUNCTION__ );
  for (int i = 0; i < h; i++){
    for (int j = 0; j < w; j++){
      dest[i*w+j] += delta[i*w+j];
    }
  }
}

int main(int argc, char** argv) {
  int h=398, w=296;
  float* dest=(float*)_mm_malloc(sizeof(float) * (h*w), 32);
  float* delta=(float*)_mm_malloc(sizeof(float) * (h*w), 32);
  test_matrix_fuse(dest, delta);
  ::printf("%.2f\n",dest[0]);
  _mm_free(dest), _mm_free(delta);
  return 0;
}