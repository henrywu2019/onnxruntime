//
// Created by henry on 2/21/23.
//

#ifndef ONNXRUNTIME_GAMMA_COMMON_H
#define ONNXRUNTIME_GAMMA_COMMON_H

void print_matrix(float* m, int h, int w);
int ceil_int(int x, int y);
int floor_int(int x, int y);

void make_input(float* l, int n, int c, int w, int h, float channel_delta = 0.1, float cell_delta = 1, float batch_delta=0.2);

#endif  // ONNXRUNTIME_GAMMA_COMMON_H
