#include "tunable_conv.h"
#include <sein.hpp>
#include "immintrin.h"
#define SIMPLE_INDEX

int ceil_int(int x, int y){
  return int(ceil((float)x/y));
}


void print_matrix(float* m, int h, int w) {
  if (h*w>100) return;
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w - 1; j++)
      printf("%.1f,", m[i * w + j]);
    printf("%.1f\n", m[i * w + w - 1]);
  }
}


#define input_index_ori(N,C,H,W) (N*input_batch_stride + C*input_channel_stride + H*ca.W + W)
#define input_index_new(N,C_,H,W,c) (N*input_batch_stride + C_*input_block_stride + H*ca.W*tunable_x + W*tunable_x + c)
#define filter_index_ori(K,C,R,L) (K*filter_batch_stride + C*filter_channel_stride + R*ca.L + L)
#define filter_index_new(K_,C_,R,L,c,k) (k_*filter_chunk_stride + C_*filter_block_stride + R*ca.L*tunable_x*VEC_LEN + L*tunable_x*VEC_LEN + c*VEC_LEN + k)
// channel is removed; NHBcw(8)
void tunable_conv::reorder_input() {
  auto start = high_resolution_clock::now();
  core = (float*)_mm_malloc(sizeof(float) * input_size, 32);
  int ori_idx=0, new_idx=0;
  REP(i,0,ca.N){
    REP(j,0,slice_number_in_channel_dim){
      REP(k,0,ca.H){
        REP(l,0,ca.W){
          REP(m,0,tunable_x){
            ori_idx = i*input_batch_stride + (m+j*tunable_x)*input_channel_stride + k*ca.W + l;
#ifdef SIMPLE_INDEX
            core[new_idx++] = input[ori_idx];
#else
            new_idx = i*input_batch_stride + j*input_block_stride + k*ca.W*tunable_x + l*tunable_x + m;
            core[new_idx] = input[ori_idx];
#endif
          }
        }
      }
    }
  }
#ifdef SIMPLE_INDEX
  assert(new_idx == input_size);
#else
  assert(new_idx+1 == input_size);
#endif
  auto t = duration_cast<nanoseconds>((high_resolution_clock::now() - start)).count();cout << __FUNCTION__ << ": " << t << " ns" << endl;
}

void tunable_conv::reorder_filter() {
  auto start = high_resolution_clock::now();
  f = (float*)_mm_malloc(sizeof(float) * filter_size, 32);
  int ori_idx=0, new_idx=0;
  REP(i,0,slice_number_in_batch_dim){ //K'
    REP(j,0,slice_number_in_channel_dim){ // C'
      REP(k,0,ca.R){ // R
        REP(l,0,ca.L){ // L
          REP(m,0,tunable_x){ // c_
            REP(n,0,VEC_LEN){ // k_
              ori_idx = (i*VEC_LEN+n)*filter_batch_stride + (m+j*tunable_x)*filter_channel_stride + k*ca.L + l;
#ifdef SIMPLE_INDEX
              f[new_idx++] = input[ori_idx];
#else
              new_idx = i*filter_chunk_stride + j*filter_block_stride + k*ca.L*tunable_x*VEC_LEN + l*tunable_x*VEC_LEN + m*VEC_LEN+n;
              f[new_idx] = kernel[ori_idx];
#endif
            }
          }
        }
      }
    }
  }
#ifdef SIMPLE_INDEX
  assert(new_idx == filter_size);
#else
  assert(new_idx+1 == filter_size);
#endif
  auto t = duration_cast<nanoseconds>((high_resolution_clock::now() - start)).count();cout << __FUNCTION__ << ": " << t << " ns" << endl;
}

void tunable_conv::run() {
  int reg_idx=0;
  long long called=0;
  auto start = high_resolution_clock::now();

  REP(i,0,slice_number_in_batch_dim){ //K'
    REP(j,0,slice_number_in_channel_dim){ // C'
      REP(k,0,ca.R){ // R
        REP(l,0,ca.L){ // L
          REP(m,0,tunable_x){ // c_
            //REP(n,0,VEC_LEN){}
            //auto input_x = _mm256_set1_ps(core[input_index_new(0,j,)]);
          }
        }
      }
    }
  }





  /*const int remaining_bytes = (VEC_LEN - core_padding) * sizeof(float);
  __m256 x = {}, y = {}, r = {};
  float **output_base = new float *[filter_channel_stride * ca.K](), **output_src = new float *[filter_channel_stride * ca.K]();
  int safe_batch = core_padding > 0 ? core_batch - 1 : core_batch;  //

  for (int k_ = 0; k_ < ca.K; k_++) {
    for (int r_ = 0; r_ < ca.R; r_++)
      for (int l_ = 0; l_ < ca.L; l_++)
        output_base[l_ + r_ * ca.L + filter_channel_stride * k_] = output + ca.OW * r_ + l_ + out_channel_stride * k_;
  }
  alignas(32)  float tmp_out[8*2]={};
  for (int ch_ = 0; ch_ < core_h; ch_++) {        // h
    for (int cb_ = 0; cb_ < safe_batch; cb_++) {  // w
      for (int k_ = 0; k_ < ca.K; k_++) {  // k
        for (int r_ = 0; r_ < ca.R; r_++) {
          for (int l_ = 0; l_ < ca.L; l_++) {
            int ri = l_ + r_ * ca.L + k_ * filter_channel_stride;
            output_src[ri] = output_base[ri] + ch_ * ca.OW + (cb_ << 3);
            //__asm__("vfmadd231ps $ymm0, %2, %0");
            r = _mm256_loadu_ps(output_src[ri]);
            //r = _mm256_set1_ps(0);
            for (int c_ = 0; c_ < ca.C; c_++) {
              int x_of = ((ch_ * core_batch * core_c + cb_ * core_c) << 3) + c_ * 8;
              int y_of = c_ + ri * ca.C;
              x = _mm256_load_ps(core + x_of);// input is unrelated to r_ and l_, but related to channel
              y = _mm256_set1_ps(f[y_of]);  // kernel
              r = _mm256_fmadd_ps(x, y, r);
              //printf("%lld,xid=%d,yid=%d,ri=%d\n", called, x_of, y_of, ri);
            }
            //printf("%lld,ri=%d\n",called, ri);
            //_mm256_storeu_ps(output_src[ri], r);
            called++;
            // vmovups YMMWORD PTR [r8],ymm0
            //_mm256_store_ps(tmp_out+(reg_idx++<<3), r), reg_idx=reg_idx%8;
            reg_idx=reg_idx%2;
            _mm256_store_ps(tmp_out+(reg_idx&1)*8, r);
            //_mm256_store_ps(tmp_out, r);
            ++reg_idx;
          }
        }
      }
    }
    //if (core_padding > 0) {
    if (1) {
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
            //_mm256_storeu_ps(t, r);
            ::memcpy(output_src[ri], t, remaining_bytes);
          }
        }
      }
    }
  }*/
  printf("called: %lld\n",called);
  long long t = duration_cast<nanoseconds>((high_resolution_clock::now() - start)).count(); cout << __FUNCTION__ << " | algo run Time: " << t << " ns" << endl;
}
