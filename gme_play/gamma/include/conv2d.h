//
// Created by henry on 2/21/23.
//

#ifndef ONNXRUNTIME_CONV2D_H
#define ONNXRUNTIME_CONV2D_H

struct conv_attr {
  int N, C, H, W;  // input
  int K, R, L;     // kernel
  int OH, OW;      // output
  int input_channel_stride = 0;
  int input_batch_stride = 0;
  int input_block_stride = 0;
  conv_attr(int N_, int C_, int H_, int W_, int K_, int R_, int L_) : N(N_), C(C_), H(H_), W(W_), K(K_), R(R_), L(L_) {
    OH = H - R + 1;
    OW = W - L + 1;

    input_channel_stride = H * W;
    input_batch_stride = input_channel_stride*C;
  }
};

#endif  // ONNXRUNTIME_CONV2D_H
