//
// Created by henry on 2/21/23.
//
#include "gamma_common.h"
#include "sein.hpp"

int GAMMA_GATHER_INDEX_BASE[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

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

void make_conv2d_input(float* l, int n, int c, int w, int h, float channel_delta, float cell_delta, float batch_delta, bool random_, float start) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < c; j++) {
      if (random_) {
        start = start / (abs(int(start)) + 1) + ((double)rand() / (RAND_MAX)) + 0.1;
        if (rand() & 1) start *= -1;
      } else {
        start = 1 + i * batch_delta + j * channel_delta;
      }
      for (int m = 0; m < h; m++){
        for (int k = 0; k < w; k++) {
          int idx = k + m * w + h * w * j + h * w * c * i;
          if (random_) {
            if (rand() & 1) start *= -1;
            l[idx] = start, start += 0.01;
          } else {
            l[idx] = start, start += cell_delta;
          }
        }
      }
    }
  }
}
