#include "tunable_conv.h"
#include <sein.hpp>
#include "immintrin.h"
#define SIMPLE_INDEX
#define ALGO_INPUT_AS_OUTER_LOOP
#define ALGO_KERNL_AS_OUTER_LOOP

int ceil_int(int x, int y){
  return int(ceil((float)x/y));
}
int floor_int(int x, int y){
  return int(floor((float)x/y));
}

void print_matrix(float* m, int h, int w) {
  if (h*w>100) return;
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w - 1; j++)
      printf("%.1f,", m[i * w + j]);
    printf("%.1f\n", m[i * w + w - 1]);
  }
}

void print_output(float* Output, int h, int w, int output_channel, bool all) {
  if (all or h * w < 10 * 10) {
    for (int i = 0; i < h; i++) {
      for (int j = 0; j < w - 1; j++)
        printf("%.1f,", Output[i * w + j]);
      printf("%.1f\n", Output[i * w + w - 1]);
    }
    return;
  }
  for (int t = 0; t < min(32, output_channel * w * h); t++) {
    printf("%.2f,", Output[t]);
    if (t % 16 == 15) printf("\n");
  }
  printf("...");
  for (int t = output_channel * w * h - 17; t < output_channel * w * h; t++) {
    printf("%.2f,", Output[t]);
    if (t % 16 == 15) printf("\n");
  }
  // printf("\n");
}



#define input_index_ori(N,C,H,w) (N*input_batch_stride + C*input_channel_stride + H*ca.W + w)
//#define input_index_new(N,C_,H,w,c) (N*input_batch_stride + C_*input_block_stride + H*ca.W*tunable_x + w*tunable_x + c)
#define filter_index_ori(K,C,R,l) (K*filter_batch_stride + C*filter_channel_stride + R*ca.L + l)
#define filter_index_new(K_,C_,R,l,c,k) (K_*filter_chunk_stride + C_*filter_block_stride + R*ca.L*tunable_x*VEC_LEN + l*tunable_x*VEC_LEN + c*VEC_LEN + k)
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

void tunable_conv::restore_output(){
  auto start = high_resolution_clock::now();
  output_nchw = (float*)_mm_malloc(sizeof(float) * output_size, 32);
  int ori_idx=0, new_idx=0;
  // N is always 1
  REP(i,0,ca.K){ // C
    int C_= floor_int(i, tunable_x);
    REP(k,0,ca.OH){ // H
      REP(l,0,ca.OW){ // W
        int c = i - tunable_x*C_;
        ori_idx = output_index_nchwc(C_,k,l, c);
        output_nchw[new_idx++] = output[ori_idx];
      }
    }
  }
  assert(new_idx == output_size);
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
              f[new_idx++] = kernel[ori_idx];
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

#ifdef ALGO_INPUT_AS_OUTER_LOOP

#endif

#ifdef ALGO_KERNL_AS_OUTER_LOOP
  // not consider reg_n=4 in batch dim
  auto pr = new __m256[reg_n]();
  REP(i,0,hunk_number_in_batch_dim){ // N(K_)
    REP(k,0,ca.OH){
      REP(j,0,slice_number_in_channel_dim){
        REP(l,0,ca.OW){
          REP(t,0,reg_n) pr[t]=_mm256_xor_ps(pr[t],pr[t]);
          __m256 x{}, y{};
          REP(n,0,ca.R){
            REP(o,0,ca.L){
              REP(m,0,tunable_x){ // channel dim
                int i_offset = input_index_new(0,j,k+n,l+o,m);
                x = _mm256_set1_ps(core[i_offset]);
                REP(p,0,reg_n){
                  int f_offset = filter_index_new((i*reg_n+p),j,n,o,m,0);
                  y = _mm256_load_ps(f+f_offset);
                  pr[p] = _mm256_fmadd_ps(x, y, pr[p]);
                }
              }
            }
          }
          REP(q,0,reg_n){
            int o_offset = output_index_nchwc(i,k,l,q*VEC_LEN);
            auto tmp = _mm256_loadu_ps(output+o_offset);
            pr[q] = _mm256_add_ps(pr[q], tmp);
            _mm256_storeu_ps(output+o_offset, pr[q]);
          }

        }
      }
    }
  }
  delete [] pr;
#endif
  printf("called: %lld\n",called);
  long long t = duration_cast<nanoseconds>((high_resolution_clock::now() - start)).count(); cout << __FUNCTION__ << " | algo run Time: " << t << " ns" << endl;
}
