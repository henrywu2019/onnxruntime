//
// Created by henry on 2/21/23.
//

#ifndef ONNXRUNTIME_GAMMA_COMMON_H
#define ONNXRUNTIME_GAMMA_COMMON_H

void print_matrix(float* m, int h, int w);
int ceil_int(int x, int y);
int floor_int(int x, int y);
bool array_equal(float*,float*,int,float tolerance=1e-4);


#endif  // ONNXRUNTIME_GAMMA_COMMON_H
