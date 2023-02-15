#include <bits/stdc++.h>
#include "immintrin.h"
#include "fmaintrin.h"
// #define RANDDATA

using namespace std;
using namespace chrono;

void print_matrix(float* m, int h, int w) {
  if (h*w>100) return;
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w - 1; j++)
      printf("%.1f,", m[i * w + j]);
    printf("%.1f\n", m[i * w + w - 1]);
  }
}

void get_input(float* l, int n, int c, int w, int h, float channel_delta = 0, float cell_delta = 1) {
  float start = 1.;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < c; j++) {
#ifdef RANDDATA
      start = start / (abs(int(start)) + 1) + ((double)rand() / (RAND_MAX)) + 0.1;
      if (rand() & 1) start *= -1;
#else
      start = 1 + i * 0.1 + j * channel_delta;
#endif
      for (int m = 0; m < h; m++)
        for (int k = 0; k < w; k++) {
          int idx = k + m * w + h * w * j + h * w * c * i;
#ifdef RANDDATA
          if (rand() & 1) start *= -1;
          l[idx] = start, start += 0.01;
#else
          l[idx] = start, start += cell_delta;
#endif
        }
    }
  }
}

const int VL = 8;  // vectorization length
struct conv_attr {
  int N, C, H, W;  // input
  int K, R, L;     // kernel
  int OH, OW;      // output
  conv_attr(int N_, int C_, int H_, int W_, int K_, int R_, int L_) : N(N_), C(C_), H(H_), W(W_), K(K_), R(R_), L(L_) {
    OH = H - R + 1;
    OW = W - L + 1;
  }
};

struct conv_wfh {  // can refactor using inheritance
  conv_attr ca;
  // core attr
  int core_h, core_w, core_c;
  int core_w2;  // breadth after padding = core_batch*VL
  int core_size;
  int core_batch;        // = ceil(core_w/VL)
  int core_padding = 0;  // = core_w2 - core_w

  int ori_channel_stride;

  int filter_size;
  int filter_channel_stride;
  int filter_batch_stride;

  int out_channel_stride;

  // original memory store
  float* input;
  float* kernel;
  float* output;
  // wfh Algo memory store
  float* core;
  float* o;
  float* p;
  float* f;

  conv_wfh(conv_attr ca_, float* input_, float* kernel_, float* output_) : ca(ca_), input(input_), kernel(kernel_), output(output_) {
    core_h = ca.H - 4;
    core_w = ca.W - 4;
    core_c = ca.C;
    core_batch = core_w % VL == 0 ? core_w / VL : int(float(core_w) / VL + 1.0);
    core_w2 = core_batch * VL;
    core_size = core_h * core_w2 * core_c;
    core_padding = core_w2 - core_w;
    ori_channel_stride = ca.H * ca.W;

    filter_channel_stride = ca.R * ca.L;
    filter_batch_stride = ca.C * filter_channel_stride;
    filter_size = ca.K * filter_batch_stride;

    out_channel_stride = ca.OH * ca.OW;  // NKHW
  }

  void reorder_input();
  void reorder_filter() {
    int idx;
    f = (float*)_mm_malloc(sizeof(float) * filter_size, 32);
    int i = 0;
    for (int k_ = 0; k_ < ca.K; k_++) {
      for (int r_ = ca.R - 1; r_ >= 0; r_--) {
        for (int l_ = ca.L - 1; l_ >= 0; l_--) {
          for (int c_ = 0; c_ < ca.C; c_++) {
            idx = k_ * filter_batch_stride + c_ * filter_channel_stride + r_ * ca.L + l_;
            float x = kernel[idx];
            f[i++] = x;
          }
        }
      }
    }
  }
  void run();
  void print() {
    print_matrix(output,ca.OH, ca.OW);
  }
};



// channel is removed; NHBcw(8)
void conv_wfh::reorder_input() {
  auto start = high_resolution_clock::now();
  // core
  core = (float*)_mm_malloc(sizeof(float) * core_size, 32);
  float* tmp = core;
  int safe_batch = core_padding > 0 ? core_batch - 1 : core_batch;  //

  float* input_base = input + ((ca.W + 1) << 1);
  float* input_src;

  for (int i = 0; i < core_h; i++) {        // height
    for (int j = 0; j < safe_batch; j++) {  // width
      auto tmp_base = input_base + i * ca.W + (j << 3);
      for (int c_ = 0; c_ < core_c; c_++) {  // channel
        input_src = tmp_base + c_ * ori_channel_stride;
        ::memcpy(tmp, input_src, VL * sizeof(float));
        tmp += VL;
      }
    }
    if (core_padding > 0) {
      auto tmp_base = input_base + i * ca.W + (safe_batch << 3);
      for (int c_ = 0; c_ < core_c; c_++) {  // channel
        input_src = tmp_base + c_ * ori_channel_stride;
        ::memcpy(tmp, input_src, (VL - core_padding) * sizeof(float));
        tmp += VL;
      }
    }
  }
  // o
  // p
  long long t = duration_cast<nanoseconds>((high_resolution_clock::now() - start)).count();
  cout << __FUNCTION__ << " | reorder Time: " << t << " ns" << endl;
}

/*
 * Reading and writing the same memory address repeatedly can result in cache thrashing, where the cache constantly has to replace its entries because they are being overwritten.
 * This can lead to decreased cache performance, as the cache has to constantly retrieve data from main memory, instead of serving it from the cache. To maintain good cache
 * locality, it's best to have a pattern of access that spreads out the memory accesses across multiple cache lines, so that the cache can store as much of the data as possible.
 * */
void conv_wfh::run() {
  auto start = high_resolution_clock::now();
  const int remaining_bytes = (VL - core_padding) * sizeof(float);
  __m256 x = {}, y = {}, r = {};
  float **output_base = new float *[filter_channel_stride * ca.K](), **output_src = new float *[filter_channel_stride * ca.K]();
  int safe_batch = core_padding > 0 ? core_batch - 1 : core_batch;  //

  for (int k_ = 0; k_ < ca.K; k_++) {
    for (int r_ = 0; r_ < ca.R; r_++)
      for (int l_ = 0; l_ < ca.L; l_++)
        output_base[l_ + r_ * ca.L + filter_channel_stride * k_] = output + ca.OW * r_ + l_ + out_channel_stride * k_;
  }


  long long called=0;
  for (int ch_ = 0; ch_ < core_h; ch_++) {        // h
    for (int cb_ = 0; cb_ < safe_batch; cb_++) {  // w
      for (int k_ = 0; k_ < ca.K; k_++) {  // k
        // unroll!!!!
        for (int r_ = 0; r_ < ca.R; r_++) {
          for (int l_ = 0; l_ < ca.L; l_++) {
            int ri = l_ + r_ * ca.L + k_ * filter_channel_stride;
            output_src[ri] = output_base[ri] + ch_ * ca.OW + (cb_ << 3);
            r = _mm256_loadu_ps(output_src[ri]);
            for (int c_ = 0; c_ < ca.C; c_++) {
              x = _mm256_load_ps(core + ((ch_ * core_batch * core_c + cb_ * core_c) << 3) + c_ * 8);// input is unrelated to r_ and l_, but related to channel
              y = _mm256_set1_ps(f[c_ + ri * ca.C]);  // kernel
              r = _mm256_fmadd_ps(x, y, r);
              called++;
              /*printf("%lld,xid=%d,yid=%d,ri=%d\n",
                     called,
                     ((ch_ * core_batch * core_c + cb_ * core_c) << 3) + c_ * 8,
                     ri * ca.C,
                     ri);*/
            }
            //printf("%lld,ri=%d\n",called, ri);
            _mm256_storeu_ps(output_src[ri], r);
            /*for (int c_ = 0; c_ < ca.C; c_++) {
              for(int v=0;v<8;v++){
                output_src[ri][v] += core[((ch_ * core_batch * core_c + cb_ * core_c) << 3) + c_ * 8+v]*f[c_ + ri * ca.C];// input is unrelated to r_ and l_, but related to channel
              }
            }*/
          }
        }
      }
    }
    //if (core_padding > 0) { /* last line to handle padding */
    if (1) { /* last line to handle padding */
      for (int k_ = 0; k_ < ca.K; k_++) {
        for (int r_ = 0; r_ < ca.R; r_++) {
          for (int l_ = 0; l_ < ca.L; l_++) {
            int ri = l_ + r_ * ca.L + filter_channel_stride * k_;  //  register index
            output_src[ri] = output_base[ri] + ch_ * ca.OW + (safe_batch << 3);
            r = _mm256_loadu_ps(output_src[ri]);
            for (int c_ = 0; c_ < ca.C; c_++) {
              x = _mm256_load_ps(core + ((ch_ * core_batch * core_c + safe_batch * core_c) << 3) + c_ * 8);  // input is unrelated to r_ and l_
              int y_idx = c_ + ri * ca.C;
              y = _mm256_set1_ps(f[y_idx]);  // kernel
              r = _mm256_fmadd_ps(x, y, r);
            }
            float t[8] = {};  // inline void extract_float_less_VL(){}
            _mm256_storeu_ps(t, r);
            ::memcpy(output_src[ri], t, remaining_bytes);
          }
        }
      }
    }
  }
  long long t = duration_cast<nanoseconds>((high_resolution_clock::now() - start)).count(); cout << __FUNCTION__ << " | algo run Time: " << t << " ns" << endl;
}

int main(int argc, char** argv) {
  srand(0xdeadbeef);
  int input_height = 30, input_width = 30, input_channel = 2, filter_batch = 1, kernel_width = 3, kernel_height = 3;
  // input_channel = 256, input_height = 400, input_width = 296;

  if (argc >= 2) {
    if (strcmp(argv[1], "-h") == 0) {
      printf("xxx.exe filter_batch=1 input_channel=256 input_height=400 input_width=296 run_flag=2 factor=8\n");
      return 0;
    }
    filter_batch = stoi(argv[1]);
  }
  if (argc >= 3) {
    input_channel = stoi(argv[2]);
  }
  if (argc >= 4) {
    input_height = stoi(argv[3]);
  }
  if (argc >= 5) {
    input_width = stoi(argv[4]);
  }
  int run_flag = 2;
  if (argc >= 6) {
    run_flag = stoi(argv[5]);
  }
  int factor = 8;
  if (argc >= 7) {
    factor = stoi(argv[6]);
  }

  const int output_height = input_height - kernel_height + 1, output_width = input_width - kernel_width + 1;
  conv_attr ca(1, input_channel, input_height, input_width, filter_batch, kernel_height, kernel_width);
  float* I = (float*)_mm_malloc(sizeof(float) * input_channel * input_width * input_height, 32);
  float* F = (float*)_mm_malloc(sizeof(float) * filter_batch * input_channel * kernel_width * kernel_height, 32);

  get_input(I, 1, input_channel, input_width, input_height, 0.1, 1);
  get_input(F, filter_batch, input_channel, kernel_width, kernel_height, 0.1);
  printf("filter size: %d\n", filter_batch * input_channel * kernel_width * kernel_height);
  printf("input total size: %.2fKB\n", 1 * input_channel * input_width * input_height / (1024.));
  float* O = (float*)_mm_malloc(sizeof(float) * output_height * output_width * filter_batch, 32);

  conv_wfh cw(ca, I, F, O);
  auto start = high_resolution_clock::now();
  cw.reorder_input();
  cw.reorder_filter();
  cw.run();
  long long t = duration_cast<nanoseconds>((high_resolution_clock::now() - start)).count();
  cout << __FUNCTION__ << " | total algo Time: " << t << " ns" << endl;
  cw.print();
  _mm_free(I), _mm_free(F), _mm_free(O);
  return 0;
}
