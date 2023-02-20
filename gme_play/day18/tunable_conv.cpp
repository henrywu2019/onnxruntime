#include "tunable_conv.h"
#include <sein.hpp>
#include "immintrin.h"
#define SIMPLE_INDEX
#define ALGO_INPUT_AS_OUTER_LOOP
#define ALGO_KERNL_AS_OUTER_LOOP

int ceil_int(int x, int y) {
  return int(ceil((float)x / y));
}
int floor_int(int x, int y) {
  return int(floor((float)x / y));
}

void print_matrix(float* m, int h, int w) {
  if (h * w > 100) return;
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

#define input_index_ori(N, C, H, w) (N * input_batch_stride + C * input_channel_stride + H * ca.W + w)
// #define input_index_new(N,C_,H,w,c) (N*input_batch_stride + C_*input_block_stride + H*ca.W*tunable_x + w*tunable_x + c)
#define filter_index_ori(K, C, R, l) (K * filter_batch_stride + C * filter_channel_stride + R * ca.L + l)
//#define filter_index_new(K_, C_, R, l, c, k) (K_ * filter_chunk_stride + C_ * filter_block_stride + R * ca.L * tunable_x * VEC_LEN + l * tunable_x * VEC_LEN + c * VEC_LEN + k)
// channel is removed; NHBcw(8)
void tunable_conv::reorder_input() {
  auto start = high_resolution_clock::now();
  int ori_idx = 0, new_idx = 0;
  REP(i, 0, ca.N) {
    REP(j, 0, slice_number_in_channel_dim) {
      REP(k, 0, ca.H) {
        REP(l, 0, ca.W) {
          REP(m, 0, tunable_x) {
            ori_idx = i * input_batch_stride + (m + j * tunable_x) * input_channel_stride + k * ca.W + l;
#ifdef SIMPLE_INDEX
            core[new_idx++] = input[ori_idx];
#else
            new_idx = i * input_batch_stride + j * input_block_stride + k * ca.W * tunable_x + l * tunable_x + m;
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
  assert(new_idx + 1 == input_size);
#endif
  auto t = duration_cast<nanoseconds>((high_resolution_clock::now() - start)).count();
  cout << __FUNCTION__ << ": " << t << " ns" << endl;
}

void tunable_conv::restore_output() {
  auto start = high_resolution_clock::now();
  output_nchw = (float*)_mm_malloc(sizeof(float) * output_size, 32);
  int ori_idx = 0, new_idx = 0;
  // N is always 1
  REP(i, 0, ca.K) {  // C
    int C_ = floor_int(i, tunable_y);
    REP(k, 0, ca.OH) {    // H
      REP(l, 0, ca.OW) {  // W
        int c = i - tunable_y * C_;
        ori_idx = output_index_nchwc(C_, k, l, c);
        output_nchw[new_idx++] = output[ori_idx];
      }
    }
  }
  assert(new_idx == output_size);
  auto t = duration_cast<nanoseconds>((high_resolution_clock::now() - start)).count();
  cout << __FUNCTION__ << ": " << t << " ns" << endl;
}

void tunable_conv::reorder_filter() {
  auto start = high_resolution_clock::now();
  f = (float*)_mm_malloc(sizeof(float) * filter_size, 32);
  int ori_idx = 0, new_idx = 0;
  REP(i, 0, slice_number_in_batch_dim) {      // K'
    REP(j, 0, slice_number_in_channel_dim) {  // C'
      REP(k, 0, ca.R) {                       // R
        REP(l, 0, ca.L) {                     // L
          REP(m, 0, tunable_x) {              // c_
            REP(n, 0, VEC_LEN) {              // k_
              ori_idx = (i * VEC_LEN + n) * filter_batch_stride + (m + j * tunable_x) * filter_channel_stride + k * ca.L + l;
#ifdef SIMPLE_INDEX
              f[new_idx++] = kernel[ori_idx];
#else
              new_idx = i * filter_chunk_stride + j * filter_block_stride + k * ca.L * tunable_x * VEC_LEN + l * tunable_x * VEC_LEN + m * VEC_LEN + n;
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
  assert(new_idx + 1 == filter_size);
#endif
  auto t = duration_cast<nanoseconds>((high_resolution_clock::now() - start)).count();
  cout << __FUNCTION__ << ": " << t << " ns" << endl;
}

/*void _gme_conv(float* I, float* F, float* output, int oh_idx, int ow, int kh, int kw, int tunable_x, int c_idx, int k_idx, int iw, int input_block_stride, int output_block_stride) {
  int reg_n = 4;
  int j = c_idx, k = oh_idx, i = k_idx;
  REP(l, 0, ow) {
    asm("vxorps %xmm0,%xmm0,%xmm0");
    // REP(t,0,reg_n) pr[t]=_mm256_xor_ps(pr[t],pr[t]);
    __m256 y00{}, y01{}, y02{}, y03{}, y04{}, y05{}, y06{}, y07{}, y08{}, y09{}, y10{}, y11{}, y12{}, y13{}, y14{}, y15{};
    REP(n, 0, kh) {
      REP(o, 0, kw) {
        REP(m, 0, tunable_x) {  // channel dim
          int i_offset = input_index_new(0, j, k + n, l + o, m);
          y13 = _mm256_set1_ps(I[i_offset]);
          y14 = _mm256_set1_ps(I[i_offset + tunable_x]);
          y15 = _mm256_set1_ps(I[i_offset + tunable_x * 2]);

          REP(p, 0, reg_n) {
            int f_offset = filter_index_new((i * reg_n + p), j, n, o, m, 0);
            y12 = _mm256_load_ps(f + f_offset);
            pr[p] = _mm256_fmadd_ps(y01, y12, pr[p]);
          }
        }
      }
    }
    REP(q, 0, reg_n) {
      // assume tunable_x == tunable_y
      int o_offset = output_index_nchwc(i, k, l, q * VEC_LEN);
      auto tmp = _mm256_loadu_ps(output + o_offset);
      pr[q] = _mm256_add_ps(pr[q], tmp);
      _mm256_storeu_ps(output + o_offset, pr[q]);
    }
  }
}*/

void tunable_conv::run_tunable() {
  if (tunable_y == 8) {
    run_32_8();
  } else if (tunable_y == 32) {
    run_32_32();
  }
}

void tunable_conv::run() {
  int reg_idx = 0;
  long long called = 0;
  auto start = high_resolution_clock::now();
  asm("vxorps %xmm0,%xmm0,%xmm0");
  asm("vxorps %xmm1,%xmm1,%xmm1");
  asm("vxorps %xmm2,%xmm2,%xmm2");
  asm("vxorps %xmm3,%xmm3,%xmm3");

#ifdef ALGO_INPUT_AS_OUTER_LOOP

#endif

#ifdef ALGO_KERNL_AS_OUTER_LOOP
  // not consider reg_n=4 in batch dim
  auto pr = new __m256[reg_n]();
  REP(i, 0, hunk_number_in_batch_dim) {  // N(K_)
    REP(k, 0, ca.OH) {
      REP(j, 0, slice_number_in_channel_dim) {  // C_
        REP(l, 0, ca.OW) {
          REP(t, 0, reg_n)
          pr[t] = _mm256_xor_ps(pr[t], pr[t]);
          __m256 x{}, y{};
          REP(n, 0, ca.R) {
            REP(o, 0, ca.L) {
              REP(m, 0, tunable_x) {  // channel dim
                int i_offset = input_index_new(0, j, k + n, l + o, m);
                x = _mm256_set1_ps(core[i_offset]);
                REP(p, 0, reg_n) {
                  int f_offset = filter_index_new((i * reg_n + p), j, n, o, m, 0);
                  y = _mm256_load_ps(f + f_offset);
                  pr[p] = _mm256_fmadd_ps(x, y, pr[p]);
                }
              }
            }
          }
          REP(q, 0, reg_n) {
            // assume tunable_x == tunable_y
            int o_offset = output_index_nchwc(i, k, l, q * VEC_LEN);
            auto tmp = _mm256_loadu_ps(output + o_offset);
            pr[q] = _mm256_add_ps(pr[q], tmp);
            _mm256_storeu_ps(output + o_offset, pr[q]);
          }
        }
      }
    }
  }
  delete[] pr;
#endif
  printf("called: %lld\n", called);
  long long t = duration_cast<nanoseconds>((high_resolution_clock::now() - start)).count();
  cout << __FUNCTION__ << " | algo run Time: " << t << " ns" << endl;
}

// x=32
void tunable_conv::run_32_32() {
  auto start = high_resolution_clock::now();

  // not consider reg_n=4 in batch dim
  __m256 y12{}, y13{}, y14{}, y15{};
  REP(i, 0, hunk_number_in_batch_dim) {  // N(K_)
    REP(k, 0, ca.OH) {
      REP(j, 0, slice_number_in_channel_dim) {
        for (int l = 0; l < ca.OW; l += 3) {
          __m256 y00{}, y01{}, y02{}, y03{}, y04{}, y05{}, y06{}, y07{}, y08{}, y09{}, y10{}, y11{};
          REP(n, 0, ca.R) {
            REP(o, 0, ca.L) {
              REP(m, 0, tunable_x) {  // channel dim
                auto i_offset = input_index_new(0, j, k + n, l + o, m);
                y13 = _mm256_set1_ps(core[i_offset]);
                y14 = _mm256_set1_ps(core[i_offset + tunable_x]);
                y15 = _mm256_set1_ps(core[i_offset + tunable_x * 2]);
                auto f_offset = filter_index_new((i * reg_n), j, n, o, m, 0);
                y12 = _mm256_load_ps(f + f_offset);
                y00 = _mm256_fmadd_ps(y13, y12, y00);
                y04 = _mm256_fmadd_ps(y14, y12, y04);
                y08 = _mm256_fmadd_ps(y15, y12, y08);
                f_offset += filter_chunk_stride;
                y12 = _mm256_load_ps(f + f_offset);
                y01 = _mm256_fmadd_ps(y13, y12, y01);
                y05 = _mm256_fmadd_ps(y14, y12, y05);
                y09 = _mm256_fmadd_ps(y15, y12, y09);
                f_offset += filter_chunk_stride;
                y12 = _mm256_load_ps(f + f_offset);
                y02 = _mm256_fmadd_ps(y13, y12, y02);
                y06 = _mm256_fmadd_ps(y14, y12, y06);
                y10 = _mm256_fmadd_ps(y15, y12, y10);
                f_offset += filter_chunk_stride;
                y12 = _mm256_load_ps(f + f_offset);
                y03 = _mm256_fmadd_ps(y13, y12, y03);
                y07 = _mm256_fmadd_ps(y14, y12, y07);
                y11 = _mm256_fmadd_ps(y15, y12, y11);
              }
            }
          }
          int o_offset = output_index_nchwc(i, k, l, 0);
          //if (j > 0) {
            auto oo = o_offset;
            auto tmp = _mm256_load_ps(output + oo);
            y00 = _mm256_add_ps(y00, tmp);
            oo += VEC_LEN, tmp = _mm256_load_ps(output + oo), y01 = _mm256_add_ps(y01, tmp);
            oo += VEC_LEN, tmp = _mm256_load_ps(output + oo), y02 = _mm256_add_ps(y02, tmp);
            oo += VEC_LEN, tmp = _mm256_load_ps(output + oo), y03 = _mm256_add_ps(y03, tmp);
            oo += VEC_LEN, tmp = _mm256_load_ps(output + oo), y04 = _mm256_add_ps(y04, tmp);
            oo += VEC_LEN, tmp = _mm256_load_ps(output + oo), y05 = _mm256_add_ps(y05, tmp);
            oo += VEC_LEN, tmp = _mm256_load_ps(output + oo), y06 = _mm256_add_ps(y06, tmp);
            oo += VEC_LEN, tmp = _mm256_load_ps(output + oo), y07 = _mm256_add_ps(y07, tmp);
            oo += VEC_LEN, tmp = _mm256_load_ps(output + oo), y08 = _mm256_add_ps(y08, tmp);
            oo += VEC_LEN, tmp = _mm256_load_ps(output + oo), y09 = _mm256_add_ps(y09, tmp);
            oo += VEC_LEN, tmp = _mm256_load_ps(output + oo), y10 = _mm256_add_ps(y10, tmp);
            oo += VEC_LEN, tmp = _mm256_load_ps(output + oo), y11 = _mm256_add_ps(y11, tmp);
          //}
          _mm256_store_ps(output + o_offset, y00);
          o_offset += VEC_LEN, _mm256_store_ps(output + o_offset, y01);
          o_offset += VEC_LEN, _mm256_store_ps(output + o_offset, y02);
          o_offset += VEC_LEN, _mm256_store_ps(output + o_offset, y03);
          o_offset += VEC_LEN, _mm256_store_ps(output + o_offset, y04);
          o_offset += VEC_LEN, _mm256_store_ps(output + o_offset, y05);
          o_offset += VEC_LEN, _mm256_store_ps(output + o_offset, y06);
          o_offset += VEC_LEN, _mm256_store_ps(output + o_offset, y07);
          o_offset += VEC_LEN, _mm256_store_ps(output + o_offset, y08);
          o_offset += VEC_LEN, _mm256_store_ps(output + o_offset, y09);
          o_offset += VEC_LEN, _mm256_store_ps(output + o_offset, y10);
          o_offset += VEC_LEN, _mm256_store_ps(output + o_offset, y11);
        }
      }
    }
  }
  long long t = duration_cast<nanoseconds>((high_resolution_clock::now() - start)).count();
  cout << __FUNCTION__ << " | algo run Time: " << t << " ns" << endl;
}


// x=32
void tunable_conv::run_32_8() {
  auto start = high_resolution_clock::now();
  
  // not consider reg_n=4 in batch dim
  __m256 y12{}, y13{}, y14{}, y15{};
  REP(i, 0, hunk_number_in_batch_dim) {  // N(K_)
    int ori_channel_ = i*reg_n*VEC_LEN;
    int output_C_ = floor_int(ori_channel_, tunable_y);
    REP(k, 0, ca.OH) {
      REP(j, 0, slice_number_in_channel_dim) {
        for (int l = 0; l < ca.OW; l += 3) {
          __m256 y00{}, y01{}, y02{}, y03{}, y04{}, y05{}, y06{}, y07{}, y08{}, y09{}, y10{}, y11{};
          REP(n, 0, ca.R) {
            REP(o, 0, ca.L) {
              REP(m, 0, tunable_x) {  // channel dim
                auto i_offset = input_index_new(0, j, k + n, l + o, m);
                y13 = _mm256_set1_ps(core[i_offset]);
                y14 = _mm256_set1_ps(core[i_offset + tunable_x]);
                y15 = _mm256_set1_ps(core[i_offset + tunable_x * 2]);
                auto f_offset = filter_index_new((i * reg_n), j, n, o, m, 0);
                y12 = _mm256_load_ps(f + f_offset);
                y00 = _mm256_fmadd_ps(y13, y12, y00);
                y04 = _mm256_fmadd_ps(y14, y12, y04);
                y08 = _mm256_fmadd_ps(y15, y12, y08);
                f_offset += filter_chunk_stride;
                y12 = _mm256_load_ps(f + f_offset);
                y01 = _mm256_fmadd_ps(y13, y12, y01);
                y05 = _mm256_fmadd_ps(y14, y12, y05);
                y09 = _mm256_fmadd_ps(y15, y12, y09);
                f_offset += filter_chunk_stride;
                y12 = _mm256_load_ps(f + f_offset);
                y02 = _mm256_fmadd_ps(y13, y12, y02);
                y06 = _mm256_fmadd_ps(y14, y12, y06);
                y10 = _mm256_fmadd_ps(y15, y12, y10);
                f_offset += filter_chunk_stride;
                y12 = _mm256_load_ps(f + f_offset);
                y03 = _mm256_fmadd_ps(y13, y12, y03);
                y07 = _mm256_fmadd_ps(y14, y12, y07);
                y11 = _mm256_fmadd_ps(y15, y12, y11);
              }
            }
          }
          int o_offset = output_index_nchwc(output_C_, k, l, 0);
          //if (j > 0) {
          int oo;
          __m256 tmp{};
          oo = o_offset, tmp = _mm256_load_ps(output + oo), y00 = _mm256_add_ps(y00, tmp);
          oo += VEC_LEN, tmp = _mm256_load_ps(output + oo), y04 = _mm256_add_ps(y04, tmp);
          oo += VEC_LEN, tmp = _mm256_load_ps(output + oo), y08 = _mm256_add_ps(y08, tmp);
          
          oo = o_offset + output_block_stride, tmp = _mm256_load_ps(output + oo), y01 = _mm256_add_ps(y01, tmp);
          oo += VEC_LEN, tmp = _mm256_load_ps(output + oo), y05 = _mm256_add_ps(y05, tmp);
          oo += VEC_LEN, tmp = _mm256_load_ps(output + oo), y09 = _mm256_add_ps(y09, tmp);
          
          oo = o_offset + output_block_stride*2, tmp = _mm256_load_ps(output + oo), y02 = _mm256_add_ps(y02, tmp);
          oo += VEC_LEN, tmp = _mm256_load_ps(output + oo), y06 = _mm256_add_ps(y06, tmp);
          oo += VEC_LEN, tmp = _mm256_load_ps(output + oo), y10 = _mm256_add_ps(y10, tmp);
          
          oo = o_offset + output_block_stride*3, tmp = _mm256_load_ps(output + oo), y03 = _mm256_add_ps(y03, tmp);
          oo += VEC_LEN, tmp = _mm256_load_ps(output + oo), y07 = _mm256_add_ps(y07, tmp);
          oo += VEC_LEN, tmp = _mm256_load_ps(output + oo), y11 = _mm256_add_ps(y11, tmp);
          //}
          
          oo = o_offset, _mm256_store_ps(output + oo, y00);
          oo += VEC_LEN, _mm256_store_ps(output + oo, y04);
          oo += VEC_LEN, _mm256_store_ps(output + oo, y08);
          
          oo = o_offset + output_block_stride, _mm256_store_ps(output + oo, y01);
          oo += VEC_LEN, _mm256_store_ps(output + oo, y05);
          oo += VEC_LEN, _mm256_store_ps(output + oo, y09);
          
          oo = o_offset + output_block_stride*2, _mm256_store_ps(output + oo, y02);
          oo += VEC_LEN, _mm256_store_ps(output + oo, y06);
          oo += VEC_LEN, _mm256_store_ps(output + oo, y10);
          
          oo = o_offset + output_block_stride*3, _mm256_store_ps(output + oo, y03);
          oo += VEC_LEN, _mm256_store_ps(output + oo, y07);
          oo += VEC_LEN, _mm256_store_ps(output + oo, y11);
        }
      }
    }
  }
  long long t = duration_cast<nanoseconds>((high_resolution_clock::now() - start)).count();
  cout << __FUNCTION__ << " | algo run Time: " << t << " ns" << endl;
}

// y12
void tunable_conv::run_64_64() {
  //reset(64, 64);
  auto start = high_resolution_clock::now();

  __m256 y12{}, y13{};// y14{}, y15{};
  REP(i, 0, hunk_number_in_batch_dim) {  // N(K_)
    REP(k, 0, ca.OH) {
      REP(j, 0, slice_number_in_channel_dim) {
        for (int l = 0; l < ca.OW; l++) {
          __m256 y00{}, y01{}, y02{}, y03{}, y04{}, y05{}, y06{}, y07{};//y08{}, y09{}, y10{}, y11{};
          REP(n, 0, ca.R) {
            REP(o, 0, ca.L) {
              REP(m, 0, tunable_x) {  // channel dim
                int i_offset = input_index_new(0, j, k + n, l + o, m);
                y13 = _mm256_set1_ps(core[i_offset]);
                int f_offset = filter_index_new((i * reg_n), j, n, o, m, 0);
                y12 = _mm256_load_ps(f + f_offset), y00 = _mm256_fmadd_ps(y13, y12, y00);
                f_offset += filter_chunk_stride, y12 = _mm256_load_ps(f + f_offset);
                y01 = _mm256_fmadd_ps(y13, y12, y01);
                f_offset += filter_chunk_stride, y12 = _mm256_load_ps(f + f_offset);
                y02 = _mm256_fmadd_ps(y13, y12, y02);
                f_offset += filter_chunk_stride, y12 = _mm256_load_ps(f + f_offset);
                y03 = _mm256_fmadd_ps(y13, y12, y03);
                f_offset += filter_chunk_stride, y12 = _mm256_load_ps(f + f_offset);
                y04 = _mm256_fmadd_ps(y13, y12, y04);
                f_offset += filter_chunk_stride, y12 = _mm256_load_ps(f + f_offset);
                y05 = _mm256_fmadd_ps(y13, y12, y05);
                f_offset += filter_chunk_stride, y12 = _mm256_load_ps(f + f_offset);
                y06 = _mm256_fmadd_ps(y13, y12, y06);
                f_offset += filter_chunk_stride, y12 = _mm256_load_ps(f + f_offset);
                y07 = _mm256_fmadd_ps(y13, y12, y07);
              }
            }
          }
          int o_offset = output_index_nchwc(i, k, l, 0);
          int oo;
          if (j > 0) {
            oo = o_offset;
            auto tmp = _mm256_load_ps(output + oo);
            y00 = _mm256_add_ps(y00, tmp);
            oo += VEC_LEN, tmp = _mm256_load_ps(output + oo), y01 = _mm256_add_ps(y01, tmp);
            oo += VEC_LEN, tmp = _mm256_load_ps(output + oo), y02 = _mm256_add_ps(y02, tmp);
            oo += VEC_LEN, tmp = _mm256_load_ps(output + oo), y03 = _mm256_add_ps(y03, tmp);
            oo += VEC_LEN, tmp = _mm256_load_ps(output + oo), y04 = _mm256_add_ps(y04, tmp);
            oo += VEC_LEN, tmp = _mm256_load_ps(output + oo), y05 = _mm256_add_ps(y05, tmp);
            oo += VEC_LEN, tmp = _mm256_load_ps(output + oo), y06 = _mm256_add_ps(y06, tmp);
            oo += VEC_LEN, tmp = _mm256_load_ps(output + oo), y07 = _mm256_add_ps(y07, tmp);
          }
          _mm256_store_ps(output + o_offset, y00);
          o_offset += VEC_LEN, _mm256_store_ps(output + o_offset, y01);
          o_offset += VEC_LEN, _mm256_store_ps(output + o_offset, y02);
          o_offset += VEC_LEN, _mm256_store_ps(output + o_offset, y03);
          o_offset += VEC_LEN, _mm256_store_ps(output + o_offset, y04);
          o_offset += VEC_LEN, _mm256_store_ps(output + o_offset, y05);
          o_offset += VEC_LEN, _mm256_store_ps(output + o_offset, y06);
          o_offset += VEC_LEN, _mm256_store_ps(output + o_offset, y07);
        }
      }
    }
  }
  long long t = duration_cast<nanoseconds>((high_resolution_clock::now() - start)).count();
  cout << __FUNCTION__ << " | algo run Time: " << t << " ns" << endl;
}

//  the stack is used to store tmp __m256 value
void tunable_conv::run_64_64_v2() {
  auto start = high_resolution_clock::now();

  __m256 y16{}, y17{}, y18{};
  REP(i, 0, hunk_number_in_batch_dim) {  // N(K_)
    REP(k, 0, ca.OH) {
      REP(j, 0, slice_number_in_channel_dim) {
        for (int l = 0; l < ca.OW; l+=2) {
          __m256 y00{}, y01{}, y02{}, y03{}, y04{}, y05{}, y06{}, y07{}, y08{}, y09{}, y10{}, y11{}, y12{}, y13{}, y14{}, y15{};
          REP(n, 0, ca.R) {
            REP(o, 0, ca.L) {
              REP(m, 0, tunable_x) {  // channel dim
                int i_offset = input_index_new(0, j, k + n, l + o, m);
                y17 = _mm256_set1_ps(core[i_offset]);
                y18 = _mm256_set1_ps(core[i_offset + tunable_x]);
                int f_offset = filter_index_new((i * reg_n), j, n, o, m, 0);
                y16 = _mm256_load_ps(f + f_offset);
                y00 = _mm256_fmadd_ps(y17, y16, y00);
                y01 = _mm256_fmadd_ps(y18, y16, y01);
                f_offset += filter_chunk_stride, y16 = _mm256_load_ps(f + f_offset);
                y02 = _mm256_fmadd_ps(y17, y16, y02);
                y03 = _mm256_fmadd_ps(y18, y16, y03);
                f_offset += filter_chunk_stride, y16 = _mm256_load_ps(f + f_offset);
                y04 = _mm256_fmadd_ps(y17, y16, y04);
                y05 = _mm256_fmadd_ps(y18, y16, y05);
                f_offset += filter_chunk_stride, y16 = _mm256_load_ps(f + f_offset);
                y06 = _mm256_fmadd_ps(y17, y16, y06);
                y07 = _mm256_fmadd_ps(y18, y16, y07);
                f_offset += filter_chunk_stride, y16 = _mm256_load_ps(f + f_offset);
                y08 = _mm256_fmadd_ps(y17, y16, y08);
                y09 = _mm256_fmadd_ps(y18, y16, y09);
                f_offset += filter_chunk_stride, y16 = _mm256_load_ps(f + f_offset);
                y10 = _mm256_fmadd_ps(y17, y16, y10);
                y11 = _mm256_fmadd_ps(y18, y16, y11);
                f_offset += filter_chunk_stride, y16 = _mm256_load_ps(f + f_offset);
                y12 = _mm256_fmadd_ps(y17, y16, y12);
                y13 = _mm256_fmadd_ps(y18, y16, y13);
                f_offset += filter_chunk_stride, y16 = _mm256_load_ps(f + f_offset);
                y14 = _mm256_fmadd_ps(y17, y16, y14);
                y15 = _mm256_fmadd_ps(y18, y16, y15);
              }
            }
          }
          int o_offset = output_index_nchwc(i, k, l, 0);
          if (j > 0) {
            auto oo = o_offset;
            auto tmp = _mm256_load_ps(output + oo);
            y00 = _mm256_add_ps(y00, tmp);
            oo += VEC_LEN, tmp = _mm256_load_ps(output + oo), y02 = _mm256_add_ps(y02, tmp);
            oo += VEC_LEN, tmp = _mm256_load_ps(output + oo), y04 = _mm256_add_ps(y04, tmp);
            oo += VEC_LEN, tmp = _mm256_load_ps(output + oo), y06 = _mm256_add_ps(y06, tmp);
            oo += VEC_LEN, tmp = _mm256_load_ps(output + oo), y08 = _mm256_add_ps(y08, tmp);
            oo += VEC_LEN, tmp = _mm256_load_ps(output + oo), y10 = _mm256_add_ps(y10, tmp);
            oo += VEC_LEN, tmp = _mm256_load_ps(output + oo), y12 = _mm256_add_ps(y12, tmp);
            oo += VEC_LEN, tmp = _mm256_load_ps(output + oo), y14 = _mm256_add_ps(y14, tmp);
            oo += VEC_LEN, tmp = _mm256_load_ps(output + oo), y01 = _mm256_add_ps(y01, tmp);
            oo += VEC_LEN, tmp = _mm256_load_ps(output + oo), y03 = _mm256_add_ps(y03, tmp);
            oo += VEC_LEN, tmp = _mm256_load_ps(output + oo), y05 = _mm256_add_ps(y05, tmp);
            oo += VEC_LEN, tmp = _mm256_load_ps(output + oo), y07 = _mm256_add_ps(y07, tmp);
            oo += VEC_LEN, tmp = _mm256_load_ps(output + oo), y09 = _mm256_add_ps(y09, tmp);
            oo += VEC_LEN, tmp = _mm256_load_ps(output + oo), y11 = _mm256_add_ps(y11, tmp);
            oo += VEC_LEN, tmp = _mm256_load_ps(output + oo), y13 = _mm256_add_ps(y13, tmp);
            oo += VEC_LEN, tmp = _mm256_load_ps(output + oo), y15 = _mm256_add_ps(y15, tmp);
          }
          _mm256_store_ps(output + o_offset, y00);
          o_offset += VEC_LEN, _mm256_store_ps(output + o_offset, y02);
          o_offset += VEC_LEN, _mm256_store_ps(output + o_offset, y04);
          o_offset += VEC_LEN, _mm256_store_ps(output + o_offset, y06);
          o_offset += VEC_LEN, _mm256_store_ps(output + o_offset, y08);
          o_offset += VEC_LEN, _mm256_store_ps(output + o_offset, y10);
          o_offset += VEC_LEN, _mm256_store_ps(output + o_offset, y12);
          o_offset += VEC_LEN, _mm256_store_ps(output + o_offset, y14);
          o_offset += VEC_LEN, _mm256_store_ps(output + o_offset, y01);
          o_offset += VEC_LEN, _mm256_store_ps(output + o_offset, y03);
          o_offset += VEC_LEN, _mm256_store_ps(output + o_offset, y05);
          o_offset += VEC_LEN, _mm256_store_ps(output + o_offset, y07);
          o_offset += VEC_LEN, _mm256_store_ps(output + o_offset, y09);
          o_offset += VEC_LEN, _mm256_store_ps(output + o_offset, y11);
          o_offset += VEC_LEN, _mm256_store_ps(output + o_offset, y13);
          o_offset += VEC_LEN, _mm256_store_ps(output + o_offset, y15);
        }
      }
    }
  }
  long long t = duration_cast<nanoseconds>((high_resolution_clock::now() - start)).count();
  cout << __FUNCTION__ << " | algo run Time: " << t << " ns" << endl;
}
