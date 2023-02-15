//
// Created by henry on 2/14/23.
//

#ifndef TUNABLE_ALGO_FAST_CONV_H
#define TUNABLE_ALGO_FAST_CONV_H
#include "immintrin.h"
#include <sein.hpp>

const int VEC_LEN = 8;  // vectorization length

int ceil_int(int x, int y);
void print_matrix(float* m, int h, int w);
void print_output(float* Output, int h, int w, int output_channel, bool all = false);

struct conv_attr {
  int N, C, H, W;  // input
  int K, R, L;     // kernel
  int OH, OW;      // output
  conv_attr(int N_, int C_, int H_, int W_, int K_, int R_, int L_) : N(N_), C(C_), H(H_), W(W_), K(K_), R(R_), L(L_) {
    OH = H - R + 1;
    OW = W - L + 1;
  }
};

int ceil_int(int x, int y) {
  return int(ceil((float)x / y));
}
int floor_int(int x, int y) {
  return int(floor((float)x / y));
}

struct fast_conv {  // can refactor using inheritance
  conv_attr ca;
  // core attr

  int input_size;
  int input_batch_stride;
  int input_channel_stride;

  int output_size;
  int output_batch_stride;
  int output_channel_stride;

  int filter_size;
  int filter_channel_stride;
  int filter_batch_stride;
  int filter_chunk_stride;

  // original memory store
  float* input;
  float* kernel;
  float* output;

  float** out_buff = nullptr;
  int out_buff_num=0;

  float* fr; // reordered kernel
  int slice_number_in_batch_dim;
  int slice_number_in_channel_dim;

  fast_conv(conv_attr ca_, float* input_, float* kernel_, float* output_)
      :ca(ca_), input(input_), kernel(kernel_), output(output_){
    input_channel_stride = ca.H * ca.W;
    input_batch_stride = input_channel_stride*ca.C;
    input_size = input_channel_stride*ca.C*ca.N; // NCHW

    output_channel_stride = ca.OH * ca.OW;
    output_batch_stride = output_channel_stride*ca.K;
    output_size = output_channel_stride*ca.K; // there is no such thing as output_batch

    filter_channel_stride = ca.R * ca.L;
    filter_batch_stride = ca.C * filter_channel_stride;
    filter_chunk_stride = filter_batch_stride*4;
    filter_size = ca.K * filter_batch_stride;
    slice_number_in_batch_dim = ceil_int(ca.K, 4);
    slice_number_in_channel_dim = ceil_int(ca.K, 32);

    if (output == nullptr)
      output = (float*)_mm_malloc(sizeof(float) * output_size, 32);
    if (ca.C>32){
      out_buff_num = ceil_int(ca.C,32)-1;
      out_buff = new float*[out_buff_num](); // TODO
      REP(x,0,out_buff_num){
        out_buff[x] = new float[output_size](); // TODO
      }
    }
    reorder_kernel();
  }

  void reorder_kernel(int k_split=4);
  void run(float* output_, int cbase=0, int cstop=32);
  void run_full();

  void print() {
    //print_matrix(output,ca.OH, ca.OW);
    print_output(output,ca.OH, ca.OW,ca.K);
  }
  inline int input_index(int c_, int h_, int w_){
    return c_*input_channel_stride + h_*ca.W + w_;
  }
#define DEBUG
  inline int kernel_index(int k_, int c_, int r_, int l_) {
#ifdef DEBUG
    static int count=0, prev=0;
    auto r = k_ * filter_batch_stride + c_ * filter_channel_stride + r_ * ca.L + l_;
    printf("%d,",r-prev), prev=r;
    if(count++%64==0){printf("\n");}
    return r;
#else
    return k_ * filter_batch_stride + c_ * filter_channel_stride + r_ * ca.L + l_;
#endif
  }
  inline int kid(int k_, int c_, int r, int l, int k) {
    return k_ * ca.C * ca.R * ca.L * 4 + c_ * ca.R * ca.L * 4 + r * ca.L * 4 + l * 4 + k;
  }
  inline int output_index(int k_, int oh_, int ow_){
    return k_*output_channel_stride + oh_*ca.OW + ow_;
  }
};

#endif  // TUNABLE_ALGO_FAST_CONV_H
