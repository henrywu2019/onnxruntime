#include <sein.hpp>
#include "conv2d.h"
#include "gamma_common.h"
#include <immintrin.h>

int GAMMA_GATHER_INDEX_BASE[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};


float* make_conv2d_input(float* l, int n, int c, int w, int h, float channel_delta, float cell_delta, float batch_delta, bool random_, float start) {
  if(l== nullptr) l=(float*)_mm_malloc(sizeof(float) * n*c*h*w, 32);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < c; j++) {
      if (random_) {
        start = start / (abs(int(start)) + 1) + ((double)rand() / (RAND_MAX)) + 0.1;
        if (rand() & 1) start *= -1;
      } else {
        start = 1 + i * batch_delta + j * channel_delta;
      }
      for (int m = 0; m < h; m++){
        for (int k = 0; k < w; k++) {
          int idx = k + m * w + h * w * j + h * w * c * i;
          if (random_) {
            if (rand() & 1) start *= -1;
            l[idx] = start, start += 0.01;
          } else {
            l[idx] = start, start += cell_delta;
          }
        }
      }
    }
  }
  return l;
}

float* make_conv2d_input(conv_attr& ca, float channel_delta, float cell_delta, float batch_delta, bool random_, float start){
  float* l=(float*)_mm_malloc(sizeof(float) * ca.input_size, 32);
  make_conv2d_input(l,ca.N,ca.C,ca.H,ca.W, channel_delta, cell_delta, batch_delta, random_, start);
  return l;
}

void reorder_NCHW_NCHWc8_base(float* s, float* d, const conv_attr& ca){
  auto_profiler ap(__FUNCTION__ );
  int slice_number_in_channel_dim = ceil_int(ca.C, 8); // for input and kernel
  int ori_idx = 0, new_idx = 0;
  REP(i, 0, ca.N) {
    REP(j, 0, slice_number_in_channel_dim) {
      REP(k, 0, ca.H) {
        REP(l, 0, ca.W) {
          REP(m, 0, 8) {
            ori_idx = i * ca.input_batch_stride + (m + j * 8) * ca.input_channel_stride + k * ca.W + l;
            d[new_idx++] = s[ori_idx];
          }
        }
      }
    }
  }
}

void reorder_NCHW_NCHWc8_avx2(float* s, float* d, conv_attr& ca){
  auto_profiler ap(__FUNCTION__ );
  int slice_number_in_channel_dim = ceil_int(ca.C, 8); // for input and kernel
  int ori_idx = 0, new_idx = 0, GatherStride= ca.input_channel_stride;
  ca.input_block_stride = 8*ca.input_channel_stride;
  auto idx = _mm256_loadu_si256((__m256i*)GAMMA_GATHER_INDEX_BASE);
  auto indices = _mm256_mullo_epi32(idx, _mm256_set1_epi32(GatherStride));
  REP(i, 0, ca.N) {
    REP(j, 0, slice_number_in_channel_dim) {
      int i_offset = i*ca.input_batch_stride+j*ca.input_block_stride;
      REP(k, 0, ca.H) {
        REP(l, 0, ca.W) {
          auto src_vec = _mm256_i32gather_ps(s+i_offset++, indices, sizeof(float));
          _mm256_storeu_ps(d, src_vec);
          d+=8;
        }
      }
    }
  }
}

#define AVX2_F32_NUM 8

void reorder_NCHW_NCHWc16_base(float* s, float* d, const conv_attr& ca){
  auto_profiler ap(__FUNCTION__ );
  int slice_number_in_channel_dim = ceil_int(ca.C, 16); // for input and kernel
  int ori_idx = 0, new_idx = 0;
  REP(i, 0, ca.N) {
    REP(j, 0, slice_number_in_channel_dim) {
      REP(k, 0, ca.H) {
        REP(l, 0, ca.W) {
          REP(m, 0, 16) {
            ori_idx = i * ca.input_batch_stride + (m + j * 16) * ca.input_channel_stride + k * ca.W + l;
            d[new_idx++] = s[ori_idx];
          }
        }
      }
    }
  }
}

void reorder_NCHW_NCHWc16_avx2(float* s, float* d, conv_attr& ca){
  auto_profiler ap(__FUNCTION__ );
  int slice_number_in_channel_dim = ceil_int(ca.C, 16); // for input and kernel
  int ori_idx = 0, new_idx = 0, GatherStride= ca.input_channel_stride;
  ca.input_block_stride = 16*ca.input_channel_stride;
  auto idx = _mm256_loadu_si256((__m256i*)GAMMA_GATHER_INDEX_BASE);
  auto indices = _mm256_mullo_epi32(idx, _mm256_set1_epi32(GatherStride));
  REP(i, 0, ca.N) {
    REP(j, 0, slice_number_in_channel_dim) {
      int base=i*ca.input_batch_stride+j*ca.input_block_stride;
      int i_offset = base;
      REP(k, 0, ca.H) {
        REP(l, 0, ca.W) {
          auto src_vec = _mm256_i32gather_ps(s+i_offset++, indices, sizeof(float));
          _mm256_storeu_ps(d+input_index_new(i,j,k,l,0,ca,16), src_vec);
        }
      }
      i_offset = base+ca.input_block_stride/2;
      REP(k, 0, ca.H) {
        REP(l, 0, ca.W) {
          auto src_vec = _mm256_i32gather_ps(s+i_offset++, indices, sizeof(float));
          _mm256_storeu_ps(d+input_index_new(i,j,k,l,0,ca,16)+8, src_vec);
        }
      }
    }
  }
}

void reorder_NCHW_NCHWc32_base(float* s, float* d, const conv_attr& ca){
  auto_profiler ap(__FUNCTION__ );
  int slice_number_in_channel_dim = ceil_int(ca.C, 32); // for input and kernel
  int ori_idx = 0, new_idx = 0;
  REP(i, 0, ca.N) {
    REP(j, 0, slice_number_in_channel_dim) {
      REP(k, 0, ca.H) {
        REP(l, 0, ca.W) {
          REP(m, 0, 32) {
            ori_idx = i * ca.input_batch_stride + (m + j * 32) * ca.input_channel_stride + k * ca.W + l;
            d[new_idx++] = s[ori_idx];
          }
        }
      }
    }
  }
}

void reorder_NCHW_NCHWc32_avx2(float* s, float* d, conv_attr& ca){
  auto_profiler ap(__FUNCTION__ );
  int slice_number_in_channel_dim = ceil_int(ca.C, 32); // for input and kernel
  int ori_idx = 0, new_idx = 0, GatherStride= ca.input_channel_stride;
  ca.input_block_stride = 32*ca.input_channel_stride;
  auto idx = _mm256_loadu_si256((__m256i*)GAMMA_GATHER_INDEX_BASE);
  auto indices = _mm256_mullo_epi32(idx, _mm256_set1_epi32(GatherStride));
  REP(i, 0, ca.N) {
    REP(j, 0, slice_number_in_channel_dim) {
      int base=i*ca.input_batch_stride+j*ca.input_block_stride;
      int i_offset = base;
      REP(k, 0, ca.H) {
        REP(l, 0, ca.W) {
          auto src_vec = _mm256_i32gather_ps(s+i_offset++, indices, sizeof(float));
          _mm256_storeu_ps(d+input_index_new(i,j,k,l,0,ca,32), src_vec);
        }
      }
      i_offset = base+ca.input_block_stride/4;
      REP(k, 0, ca.H) {
        REP(l, 0, ca.W) {
          auto src_vec = _mm256_i32gather_ps(s+i_offset++, indices, sizeof(float));
          _mm256_storeu_ps(d+input_index_new(i,j,k,l,0,ca,32)+8, src_vec);
        }
      }
      i_offset = base+ca.input_block_stride/2;
      REP(k, 0, ca.H) {
        REP(l, 0, ca.W) {
          auto src_vec = _mm256_i32gather_ps(s+i_offset++, indices, sizeof(float));
          _mm256_storeu_ps(d+input_index_new(i,j,k,l,0,ca,32)+16, src_vec);
        }
      }
      i_offset = base+3*ca.input_block_stride/4;
      REP(k, 0, ca.H) {
        REP(l, 0, ca.W) {
          auto src_vec = _mm256_i32gather_ps(s+i_offset++, indices, sizeof(float));
          _mm256_storeu_ps(d+input_index_new(i,j,k,l,0,ca,32)+24, src_vec);
        }
      }
    }
  }
}

