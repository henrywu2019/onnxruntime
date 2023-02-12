//
// Created by henry on 2/12/23.
//

#ifndef ONNXRUNTIME_TUNABLE_CONV_H
#define ONNXRUNTIME_TUNABLE_CONV_H

const int VEC_LEN = 8;  // vectorization length

int ceil_int(int x, int y);
void print_matrix(float* m, int h, int w);

struct conv_attr {
  int N, C, H, W;  // input
  int K, R, L;     // kernel
  int OH, OW;      // output
  conv_attr(int N_, int C_, int H_, int W_, int K_, int R_, int L_) : N(N_), C(C_), H(H_), W(W_), K(K_), R(R_), L(L_) {
    OH = H - R + 1;
    OW = W - L + 1;
  }
};

struct tunable_conv {  // can refactor using inheritance
  conv_attr ca;
  // core attr
  int tunable_x = 4;

  int input_size;
  int input_batch_stride;
  int input_block_stride;
  int input_channel_stride;

  int filter_size;
  int filter_channel_stride;
  int filter_batch_stride;
  int filter_block_stride_channel_dim=-1;
  int filter_block_stride_batch_dim=-1;
  int filter_block_stride;
  int filter_chunk_stride;

  int slice_number_in_channel_dim;
  int slice_number_in_batch_dim;


  int out_channel_stride;

  // original memory store
  float* input;
  float* kernel;
  float* output;

  float *core, *f; // reordered input and kernel

  tunable_conv(conv_attr ca_, float* input_, float* kernel_, float* output_)
      :ca(ca_), input(input_), kernel(kernel_), output(output_) {
    input_channel_stride = ca.H * ca.W;
    input_block_stride = input_channel_stride * tunable_x;
    input_batch_stride = input_channel_stride*ca.C;
    input_size = input_channel_stride*ca.C*ca.N; // NCHW

    slice_number_in_channel_dim = ceil_int(ca.C, tunable_x);
    slice_number_in_batch_dim = ceil_int(ca.K, VEC_LEN);

    filter_channel_stride = ca.R * ca.L;
    filter_batch_stride = ca.C * filter_channel_stride;
    filter_size = ca.K * filter_batch_stride;
    //filter_block_stride_batch_dim = ca.L*ca.R*VEC_LEN;
    //filter_block_stride_channel_dim = filter_channel_stride*tunable_x;
    filter_block_stride = ca.L*ca.R*tunable_x*VEC_LEN;
    filter_chunk_stride = filter_block_stride*slice_number_in_channel_dim;

    out_channel_stride = ca.OH * ca.OW;  // NKHW
  }

  void reorder_input();
  void reorder_filter();
  void run();
  void print() {
    print_matrix(output,ca.OH, ca.OW);
  }
};

#endif  // ONNXRUNTIME_TUNABLE_CONV_H
