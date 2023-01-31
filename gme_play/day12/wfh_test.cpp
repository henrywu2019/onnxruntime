#include <bits/stdc++.h>
#include "immintrin.h"
#include "fmaintrin.h"
#define RANDDATA

using namespace std;
using namespace chrono;

void get_input(vector<float>& l, int n, int c, int w, int h, float channel_delta=0, float cell_delta=1) {
  if ((int)l.size() < n * c * w * h) l.resize(n * c * w * h);
  float start = 1.;
  for (int i = 0; i < n; i++){
    for (int j = 0; j < c; j++) {
#ifdef RANDDATA
      start = start/(abs(int(start))+1) + ((double) rand() / (RAND_MAX)) + 0.1; if(rand()&1) start*=-1;
#else
      start = 1+i*0.1+j*channel_delta;
#endif
      for (int m = 0; m < h; m++)
        for (int k = 0; k < w; k++) {
          int idx = k + m * w + h * w * j + h * w * c * i;
#ifdef RANDDATA
          if(rand()&1) start*=-1;
          l[idx] = start, start += 0.01;
#else
          l[idx] = start, start += cell_delta;
#endif
        }
    }
  }
}

const int VL=8;//vectorization length
struct conv_attr{
  int N,C,H,W;   // input
  int K,R,L;   // kernel
  int OH,OW;   // output
  conv_attr(int N_,int C_,int H_,int W_,int K_,int R_,int L_):N(N_),C(C_),H(H_),W(W_),K(K_),R(R_),L(L_){
    OH = H-R+1;
    OW=W-L+1;
  }
};

struct conv_wfh{ // can refactor using inheritance
  conv_attr ca;
  // core attr
  int core_h, core_w, core_c;
  int core_w2; // breadth after padding = core_batch*VL
  int core_size;
  int core_batch; // = ceil(core_w/VL)
  int core_padding=0; // = core_w2 - core_w

  int ori_channel_stride;

  int filter_size;
  int filter_channel_stride;
  int filter_batch_stride;

  // original memory store
  float *input;
  float *kernel;
  float *output;
  // wfh Algo memory store
  float* core;
  float* o;
  float* p;
  float* f;


  conv_wfh(conv_attr ca_, float* input_, float* kernel_, float* output_):ca(ca_),input(input_), kernel(kernel_), output(output_){
    core_h = ca.H-4;
    core_w = ca.W-4;
    core_c = ca.C;
    core_batch = core_w%VL==0 ? core_w/VL: int(float(core_w)/VL+1.0);
    core_w2 = core_batch*VL;
    core_size = core_h*core_w2*core_c;
    core_padding = core_w2 - core_w;
    ori_channel_stride = ca.H*ca.W;

    filter_channel_stride = ca.R*ca.L;
    filter_batch_stride = ca.C*filter_channel_stride;
    filter_size = ca.K*filter_batch_stride;
  }

  void reorder_input();
  void reorder_filter(){
    int idx;
    f = new float[filter_size]();
    int i=0;
    for(int k_=0;k_<ca.K;k_++){
      for(int r_=0;r_<ca.R;r_++){
        for(int l_=0;l_<ca.L;l_++){
          for(int c_=0;c_<ca.C;c_++){
            idx=k_*filter_batch_stride+c_*filter_channel_stride+r_*ca.L+l_;
            f[i++] = kernel[idx];
          }
        }
      }
    }
  }
  void run();
  void print(){
    for(int i=0;i<ca.OH;i++){
      for(int j=0;j<ca.OW-1;j++)
        printf("%.1f,",output[i*ca.OW+j]);
      printf("%.1f\n",output[i*ca.OW+ca.OW-1]);
    }
  }
};

// channel is removed
void conv_wfh::reorder_input() {
  auto start = high_resolution_clock::now();
  // core
  core = new float[core_size]();
  float *tmp=core;
  int safe_batch = core_padding>0?core_batch-1:core_batch;//

  float* input_base = input+((ca.W+1)<<1);
  float* input_src;

  for(int i=0;i<core_h;i++){ // height
    for(int j=0;j<safe_batch;j++){ // width
      auto tmp_base= input_base+i*ca.W+(j<<3);
      for(int c_=0;c_<core_c;c_++){ // channel
        input_src = tmp_base+c_*ori_channel_stride;
        ::memcpy(tmp, input_src, VL*sizeof(float));
        tmp+=VL;
      }
    }
    if (core_padding>0){
      auto tmp_base= input_base+i*ca.W+(safe_batch<<3);
      for(int c_=0;c_<core_c;c_++){ // channel
        input_src = tmp_base+c_*ori_channel_stride;
        ::memcpy(tmp, input_src, (VL-core_padding)*sizeof(float));
        tmp+=VL;
      }
    }
  }
  // o
  // p
  long long t=duration_cast<nanoseconds>((high_resolution_clock::now() - start)).count();cout << __FUNCTION__ << " | reorder Time: " << t << " ns" << endl;
}


void conv_wfh::run() {
  auto start = high_resolution_clock::now();
  const int remaining_bytes=(VL-core_padding)*sizeof(float);
  __m256 x, y, res;
  float* output_src= nullptr;
  int safe_batch = core_padding>0?core_batch-1:core_batch;//


  for(int r_=0;r_<ca.R;r_++){
    for(int l_=0;l_<ca.L;l_++){
      int input_idx=0;
      int offset_y= 2-r_, offset_x=2-l_;
      float* output_base = output + ca.OW*offset_y + offset_x;
      for(int ch_=0;ch_<core_h;ch_++){ // h
        for(int cb_=0;cb_<safe_batch;cb_++){ // w
          output_src =output_base + ch_*ca.OW + cb_*VL;
          res = _mm256_loadu_ps(output_src);
          for(int c_=0;c_<ca.C;c_++){
            x = _mm256_loadu_ps(core+input_idx++ * VL); // input
            y = _mm256_set1_ps(*(f + c_ + l_*ca.C+ r_*ca.L*ca.C));     //kernel
            res = _mm256_fmadd_ps(x, y, res);
          }
          _mm256_storeu_ps(output_src, res);
        }
        if (core_padding>0){
          output_src = output_base + ch_*ca.OW + safe_batch*VL;
          res = _mm256_loadu_ps(output_src);
          for(int c_=0;c_<ca.C;c_++){
            x = _mm256_loadu_ps(core+input_idx++ * VL); // input
            y = _mm256_set1_ps(*(f + c_ + l_*ca.C+ r_*ca.L*ca.C));     //kernel
            res = _mm256_fmadd_ps(x, y, res);
          }
          //inline void extract_float_less_VL(){}
          float t[8]={};
          _mm256_storeu_ps(t, res);
          ::memcpy(output_src,t,remaining_bytes);
        }
      }
    }
  }
  long long t=duration_cast<nanoseconds>((high_resolution_clock::now() - start)).count();cout << __FUNCTION__ << " | algo run Time: " << t << " ns" << endl;
}

int main(){
  srand(0xdeadbeef);
  int input_height = 30, input_width = 30, input_channel = 2, filter_batch = 1, kernel_width = 3, kernel_height = 3;
  input_height = 400, input_width = 296, input_channel = 256;
  const int output_height = input_height - kernel_height + 1, output_width = input_width - kernel_width + 1;
  conv_attr ca(1, input_channel, input_height, input_width,filter_batch, kernel_height, kernel_width);
  vector<float> I, F;
  get_input(I, 1, input_channel, input_width, input_height, 0.1, 1);
  get_input(F, filter_batch, input_channel, kernel_width, kernel_height, 0.1);
  printf("filter size: %d\n", filter_batch * input_channel * kernel_width * kernel_height);
  printf("input total size: %.2fKB\n", 1 * input_channel * input_width * input_height / (1024.));
  float* O = new float[output_height * output_width * filter_batch]();

  conv_wfh cw(ca,I.data(),F.data(), O);
  auto start = high_resolution_clock::now();
  cw.reorder_input();
  cw.reorder_filter();
  cw.run();
  long long t=duration_cast<nanoseconds>((high_resolution_clock::now() - start)).count();cout << __FUNCTION__ << " | total algo Time: " << t << " ns" << endl;
  cw.print();
  return 0;
}