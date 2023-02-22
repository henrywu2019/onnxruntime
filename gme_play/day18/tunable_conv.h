#ifndef ONNXRUNTIME_TUNABLE_CONV_H
#define ONNXRUNTIME_TUNABLE_CONV_H

#include "immintrin.h"
#include "conv2d.h"

const int VEC_LEN = 8;  // vectorization length

int ceil_int(int x, int y);
void print_matrix(float* m, int h, int w);
void print_output(float* Output, int h, int w, int output_channel, bool all = false);

struct tunable_conv {  // can refactor using inheritance
  conv_attr ca;
  // core attr
  int tunable_x = VEC_LEN*4; // 32, must be multiple of VEC_LEN
  int tunable_y;

  int output_batch_stride;
  int output_block_stride;
  int output_channel_stride;

  int filter_block_stride_channel_dim=-1;
  int filter_block_stride_batch_dim=-1;
  int filter_block_stride;
  int filter_chunk_stride;
  int filter_hunk_stride;

  int slice_number_in_channel_dim;
  int slice_number_in_batch_dim;
  int hunk_number_in_batch_dim;

  int slice_number_in_channel_dim_y;

  int out_channel_stride;
  int reg_n=4;
  // original memory store
  float* input;
  float* kernel;
  float* output;
  float* output_nchw;

  float *core=nullptr, *f; // reordered input and kernel

  tunable_conv(conv_attr ca_, float* input_, float* kernel_, float* output_, int tunable_x_=64, int tunable_y_=64)
      :ca(ca_), input(input_), kernel(kernel_), output(output_) {
    reset(tunable_x_, tunable_y_);
  }

  void reset(int tunable_x_, int tunable_y_){
    tunable_x = tunable_x_, tunable_y = tunable_y_;
    // if reg_n is 4 for AVX2
    reg_n = 4;

    ca.input_block_stride = ca.input_channel_stride * tunable_x;

    output_channel_stride = ca.OH * ca.OW;
    output_block_stride = output_channel_stride * tunable_y;
    output_batch_stride = output_channel_stride*ca.K;

    slice_number_in_channel_dim = ceil_int(ca.C, tunable_x); // for input and kernel
    slice_number_in_channel_dim_y = ceil_int(ca.C, tunable_y); // for output
    slice_number_in_batch_dim = ceil_int(ca.K, VEC_LEN);
    hunk_number_in_batch_dim = ceil_int(slice_number_in_batch_dim,reg_n); //?????

    filter_block_stride = ca.filter_channel_stride * tunable_x * VEC_LEN;
    filter_chunk_stride = filter_block_stride*slice_number_in_channel_dim;
    filter_hunk_stride = filter_chunk_stride*reg_n;

    out_channel_stride = ca.OH * ca.OW;  // NKHW
    if (core==nullptr)
      core = (float*)_mm_malloc(sizeof(float) * ca.input_size, 32);
  }

  void reorder_input();
  void reorder_input_8();
  void restore_output();
  void reorder_filter();
  void run_tunable();
  void run();
  void run_ort();
  void run_8_8();
  void run_32_32();
  void run_32_8();
  void run_64_64();
  void run_64_64_v2();
  //void set_x(int x){tunable_x=x;}
  void print() {
    //print_matrix(output,ca.OH, ca.OW);
    print_output(output_nchw,ca.OH, ca.OW,ca.K);
  }
  /*inline int input_index_new(int N, int C_, int H, int w, int c){
    return N*ca.input_batch_stride + C_*ca.input_block_stride + H*ca.W*tunable_x + w*tunable_x + c;
  }*/
  inline int output_index_nchwc(int C_, int H, int w, int c){ // n always == 1
    return C_*output_block_stride + H*ca.OW*tunable_y + w*tunable_y + c;
  }
  inline int filter_index_new(int K_, int C_, int R, int l, int c, int k) {
    return K_ * filter_chunk_stride + C_ * filter_block_stride + R * ca.L * tunable_x * VEC_LEN + l * tunable_x * VEC_LEN + c * VEC_LEN + k;
  }

};
/* Why tunable_y equals to reg_n * VEC_LEN?
 * Because reg_n * VEC_LEN is the length of calculated results in channel dimension in the output, it is more convenient to make them equal.
 * So 32 is a better number!
 * Actually tunable_y could be multiple of reg_n * VEC_LEN, or vice versa(like in onnxruntime).
 * */

#endif  // ONNXRUNTIME_TUNABLE_CONV_H
