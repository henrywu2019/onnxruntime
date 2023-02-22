#ifndef ONNXRUNTIME_CONV2D_H
#define ONNXRUNTIME_CONV2D_H

#define AVX2_F32_NUM 8

struct conv_attr {
  int N, C, H, W;  // input
  int K, R, L;     // kernel
  int OH, OW;      // output
  int input_channel_stride = 0;
  int input_batch_stride = 0;
  int input_size=0;

  int input_block_stride = 0;

  int filter_size=0;
  int filter_channel_stride;
  int filter_batch_stride;

  int output_size=0;

  conv_attr(int N_, int C_, int H_, int W_, int K_=64, int R_=3, int L_=3) : N(N_), C(C_), H(H_), W(W_), K(K_), R(R_), L(L_) {
    OH = H - R + 1;
    OW = W - L + 1;

    input_channel_stride = H * W;
    input_batch_stride = input_channel_stride*C;
    input_size = input_batch_stride*N;

    filter_channel_stride = R * L;
    filter_batch_stride = C * filter_channel_stride;
    filter_size = K * filter_batch_stride;

    output_size = OH * OW * K;
  }
};

float* make_conv2d_input(float* l, int n, int c, int w, int h,
                       float channel_delta = 0.1, float cell_delta = 1, float batch_delta = 0.2, bool random_ = false, float start = 1.0);

float* make_conv2d_input(conv_attr& ca,
                       float channel_delta = 0.1, float cell_delta = 1, float batch_delta = 0.2, bool random_ = false, float start = 1.0);

void reorder_NCHW_NCHWc8_base(float* s, float* d, const conv_attr& ca);
void reorder_NCHW_NCHWc8_avx2(float* s, float* d, conv_attr& ca);
void restore_NCHWc8_NCHW_avx2(float* s, float* d, conv_attr& ca);
void restore_NCHWc8_NCHW_avx512(float* s, float* d, conv_attr& ca);

void reorder_NCHW_NCHWc16_base(float* s, float* d, const conv_attr& ca);
void reorder_NCHW_NCHWc16_avx2(float* s, float* d, conv_attr& ca);

void reorder_NCHW_NCHWc32_base(float* s, float* d, const conv_attr& ca);
void reorder_NCHW_NCHWc32_avx2(float* s, float* d, conv_attr& ca);

inline int input_index_nchw(int N, int C, int H, int W, const conv_attr& ca, int c_split){
  return N*ca.input_batch_stride + C*ca.input_channel_stride + H*ca.W + W;
}

inline int input_index_new(int N, int C_, int H, int w, int c, const conv_attr& ca, int c_split){
  return N*ca.input_batch_stride + C_*ca.input_block_stride + H*ca.W*c_split + w*c_split + c;
}

#endif  // ONNXRUNTIME_CONV2D_H
