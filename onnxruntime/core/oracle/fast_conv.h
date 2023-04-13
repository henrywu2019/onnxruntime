// Created by Fuheng Wu on 11/14/22.
#ifndef TUNABLE_ALGO_FAST_CONV_H
#define TUNABLE_ALGO_FAST_CONV_H
#include "immintrin.h"
#include "threadpool.h"
#include "oracle_common.h"

const int VEC_LEN = 8;

int ceil_int(int x, int y);
void print_matrix(float* m, int h, int w);
void print_output(float* Output, int h, int w, int output_channel, bool all = false);

struct conv_attr {
  int N, C, H, W;
  int K, R, L;
  int OH, OW;
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

struct fast_conv {
  conv_attr ca;

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


  float* input;
  float* kernel;
  float* output;

  float** out_buff = nullptr;
  int out_buff_num=0;


  float* fr = nullptr;
  float* input_nhbcw8 = nullptr;
  float* input_nhcw = nullptr;
  float* input_nchc8w = nullptr;
  bs::thread_pool pool;


  int slice_number_in_batch_dim;

  int CHANNEL_SPLIT=32;
  int thread_num=-1;
  int get_currency(){
    return thread_num;
    return thread::hardware_concurrency();
  }

  fast_conv(conv_attr ca_, float* input_, float* kernel_, float* output_, int channel_split=32, int thread_num_=false)
      :ca(ca_), input(input_), kernel(kernel_), output(output_), CHANNEL_SPLIT(channel_split), thread_num(thread_num_){
    assert(ca.C%8 == 0 or ca.C<=4);
    if(thread_num>0){
      CHANNEL_SPLIT = ceil_int(ca.C, get_currency());
    }
    input_channel_stride = ca.H * ca.W;
    input_batch_stride = input_channel_stride*ca.C;
    input_size = input_channel_stride*ca.C*ca.N;

    output_channel_stride = ca.OH * ca.OW;
    output_batch_stride = output_channel_stride*ca.K;
    output_size = output_channel_stride*ca.K;

    filter_channel_stride = ca.R * ca.L;
    filter_batch_stride = ca.C * filter_channel_stride;
    filter_chunk_stride = filter_batch_stride*4;
    filter_size = ca.K * filter_batch_stride;
    slice_number_in_batch_dim = ceil_int(ca.K, 4);

    if (output == nullptr)
      output = (float*)_mm_malloc(sizeof(float) * output_size, 32);
    if (thread_num>0){
      out_buff_num = get_currency();
      printf("concurrency number: %d\n", get_currency());
      out_buff = new float*[out_buff_num]();
      REP(x,0,out_buff_num){
        out_buff[x] = new float[output_size]();
      }
    }else{
      if (ca.C>CHANNEL_SPLIT){
        out_buff_num = ceil_int(ca.C,CHANNEL_SPLIT);
        out_buff = new float*[out_buff_num]();
        REP(x,0,out_buff_num){
          out_buff[x] = new float[output_size]();
        }
      }
    }
    reorder_kernel();
  }

  void reorder_kernel(int k_split=4);
  void reorder_input_NHbcw8();
  void reorder_input_NhcW();
  void reorder_input_NcHc8W();
  void run_nchw(float* output_, int cbase, int cstop);
  void run_nhcw(float* output_, int cbase, int cstop);
  void run_nchc8w(float* output_, int cbase, int cstop);
  void run();
  void run_full();
  void run_full_nchc8w();

  void print() {
    //print_matrix(output,ca.OH, ca.OW);
    print_output(output,ca.OH, ca.OW,ca.K);
  }
  inline int input_index(int c_, int h_, int w_){
    return c_*input_channel_stride + h_*ca.W + w_;
  }
  inline int input_index_nchw(int c_, int h_, int w_){
    return c_*input_channel_stride + h_*ca.W + w_;
  }

  inline int input_index_nhcw(int h_, int c_, int w_){
    int r = h_*ca.W*ca.C + c_*ca.W + w_;
    return r;
  }

  inline int input_index_nchc8w(int c, int h_, int c_, int w_){
    return c*ca.H*ca.W*8 + h_*ca.W*8 + c_*ca.W + w_;
  }

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


#endif
