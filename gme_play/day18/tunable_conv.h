//
// Created by henry on 2/12/23.
//

#ifndef ONNXRUNTIME_TUNABLE_CONV_H
#define ONNXRUNTIME_TUNABLE_CONV_H

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

struct tunable_conv {  // can refactor using inheritance
  conv_attr ca;
  // core attr
  const int tunable_x = VEC_LEN*4; // 32, must be multiple of VEC_LEN
  int tunable_y;

  int input_size;
  int input_batch_stride;
  int input_block_stride;
  int input_channel_stride;

  int output_size;
  int output_batch_stride;
  int output_block_stride;
  int output_channel_stride;

  int filter_size;
  int filter_channel_stride;
  int filter_batch_stride;
  int filter_block_stride_channel_dim=-1;
  int filter_block_stride_batch_dim=-1;
  int filter_block_stride;
  int filter_chunk_stride;
  int filter_hunk_stride;

  int slice_number_in_channel_dim;
  int slice_number_in_batch_dim;
  int hunk_number_in_batch_dim;

  int out_channel_stride;
  int reg_n=4;
  // original memory store
  float* input;
  float* kernel;
  float* output;
  float* output_nchw;

  float *core, *f; // reordered input and kernel

  tunable_conv(conv_attr ca_, float* input_, float* kernel_, float* output_)
      :ca(ca_), input(input_), kernel(kernel_), output(output_) {
    reg_n = tunable_x/VEC_LEN; //?????
    //reg_n = 4;
    tunable_y = tunable_x; //reg_n*VEC_LEN;

    input_channel_stride = ca.H * ca.W;
    input_block_stride = input_channel_stride * tunable_x;
    input_batch_stride = input_channel_stride*ca.C;
    input_size = input_channel_stride*ca.C*ca.N; // NCHW

    output_channel_stride = ca.OH * ca.OW;
    output_block_stride = output_channel_stride * tunable_y;
    output_batch_stride = output_channel_stride*ca.K;
    output_size = output_channel_stride*ca.K; // there is no such thing as output_batch

    slice_number_in_channel_dim = ceil_int(ca.C, tunable_x);
    slice_number_in_batch_dim = ceil_int(ca.K, VEC_LEN);
    hunk_number_in_batch_dim = ceil_int(ceil_int(ca.K, VEC_LEN),reg_n); //?????

    filter_channel_stride = ca.R * ca.L;
    filter_batch_stride = ca.C * filter_channel_stride;
    filter_size = ca.K * filter_batch_stride;
    //filter_block_stride_batch_dim = ca.L*ca.R*VEC_LEN;
    //filter_block_stride_channel_dim = filter_channel_stride*tunable_x;
    filter_block_stride = ca.L*ca.R*tunable_x*VEC_LEN;
    filter_chunk_stride = filter_block_stride*slice_number_in_channel_dim;
    filter_hunk_stride = filter_chunk_stride*reg_n;

    out_channel_stride = ca.OH * ca.OW;  // NKHW
  }

  void reorder_input();
  void restore_output();
  void reorder_filter();
  void run();
  void run_asm();
  //void set_x(int x){tunable_x=x;}
  void print() {
    //print_matrix(output,ca.OH, ca.OW);
    print_output(output_nchw,ca.OH, ca.OW,ca.K);
  }
  int input_index_new(int N, int C_, int H, int w, int c){
    return N*input_batch_stride + C_*input_block_stride + H*ca.W*tunable_x + w*tunable_x + c;
  }
  int output_index_nchwc(int C_, int H, int w, int c){ // n always == 1
    return C_*output_block_stride + H*ca.OW*tunable_y + w*tunable_y + c;
  }
};
/* Why tunable_y equals to reg_n * VEC_LEN?
 * Because reg_n * VEC_LEN is the length of calculated results in channel dimension in the output, it is more convenient to make them equal.
 * So 32 is a better number!
 * Actually tunable_y could be multiple of reg_n * VEC_LEN, or vice versa(like in onnxruntime).
 * */

#endif  // ONNXRUNTIME_TUNABLE_CONV_H
