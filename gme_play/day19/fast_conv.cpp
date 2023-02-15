//
// Created by henry on 2/14/23.
//
#include "fast_conv.h"
#include <sein.hpp>
#define SIMPLE_INDEX

void matrix_fuse(float* dest, float* src, int sz){
  REP(i,0,sz){
    dest[i] += src[i];
  }
}

#define MACRO1(i,z) {y01 = _mm256_set1_ps(fr[kid(i,c_,0,0)]);y##z = _mm256_fmadd_ps(y00, y01, y##z);}
#define MACRO2(i,z) {y01 = _mm256_set1_ps(fr[kid(i,c_,1,0)]);y##z = _mm256_fmadd_ps(y00, y01, y##z);}
#define MACRO3(i,z) {y01 = _mm256_set1_ps(fr[kid(i,c_,2,0)]);y##z = _mm256_fmadd_ps(y00, y01, y##z);}

void print_m256(const __m256& v){
  float* f = (float*)&v;
  printf("%f %f %f %f %f %f %f %f\n", f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7]);

}

void fast_conv::reorder_kernel(int k_split){
  auto start = high_resolution_clock::now();
  fr = (float*)_mm_malloc(sizeof(float) * filter_size, 32); // TODO: maybe too small
  int ori_idx = 0, new_idx = 0;
  REP(i, 0, slice_number_in_batch_dim) {      // K'
    REP(j, 0, ca.C) {                         // C
      REP(k, 0, ca.R) {                       // R
        REP(l, 0, ca.L) {                     // L
          REP(n, 0, k_split) {                // k_
            ori_idx = (i * k_split + n) * filter_batch_stride + j * filter_channel_stride + k * ca.L + l;
            fr[new_idx++] = kernel[ori_idx];
          }
        }
      }
    }
  }
  assert(new_idx == filter_size);
  auto t = duration_cast<microseconds>((high_resolution_clock::now() - start)).count();
  cout << __FUNCTION__ << ": " << t << " us" << endl;
}

void fast_conv::run_full(){
  run(output, 0, min(ca.C, 32));
  REP2(cbase,32,ca.C,32){
    run(out_buff[cbase/32-1], cbase, min(cbase+32, ca.C));
  }
  auto start = high_resolution_clock::now();
  REP(i,0,out_buff_num){
    matrix_fuse(output, out_buff[i], output_size);
  }
  auto t = duration_cast<microseconds>((high_resolution_clock::now() - start)).count();
  cout << "matrix_fuse: " << t << " us" << endl;
}

void fast_conv::run(float* output_, int cbase, int cstop) {
  __m256 y00{}, y01{}, y02{}, y03{};
  __m256 y04{}, y05{}, y06{}, y07{}, y08{}, y09{}, y10{}, y11{}, y12{}, y13{}, y14{}, y15{}; // 8*12
  REP(k,0,slice_number_in_batch_dim){
    REP2(w_,0,ca.OW,8){
      // TC: O(9*32*H=288H)
      {
        const int h_=0;
        REP(c_,cbase,cstop-1){
          int i_offset=input_index(c_,h_,w_);
          y00 = _mm256_loadu_ps(input + i_offset++);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,0,0)]); y04 = _mm256_fmadd_ps(y00, y01, y04);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,0,1)]); y07 = _mm256_fmadd_ps(y00, y01, y07);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,0,2)]); y10 = _mm256_fmadd_ps(y00, y01, y10);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,0,3)]); y13 = _mm256_fmadd_ps(y00, y01, y13);

          y00 = _mm256_loadu_ps(input + i_offset++);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,1,0)]); y04 = _mm256_fmadd_ps(y00, y01, y04);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,1,1)]); y07 = _mm256_fmadd_ps(y00, y01, y07);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,1,2)]); y10 = _mm256_fmadd_ps(y00, y01, y10);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,1,3)]); y13 = _mm256_fmadd_ps(y00, y01, y13);

          y00 = _mm256_loadu_ps(input + i_offset++);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,2,0)]); y04 = _mm256_fmadd_ps(y00, y01, y04);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,2,1)]); y07 = _mm256_fmadd_ps(y00, y01, y07);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,2,2)]); y10 = _mm256_fmadd_ps(y00, y01, y10);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,2,3)]); y13 = _mm256_fmadd_ps(y00, y01, y13);
        }
        // when c_=cstop-1
        {
          int c_=cstop-1;
          int i_offset=input_index(c_,h_,w_);
          y00 = _mm256_loadu_ps(input + i_offset++);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,0,0)]); y04 = _mm256_fmadd_ps(y00, y01, y04);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,0,1)]); y07 = _mm256_fmadd_ps(y00, y01, y07);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,0,2)]); y10 = _mm256_fmadd_ps(y00, y01, y10);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,0,3)]); y13 = _mm256_fmadd_ps(y00, y01, y13);

          y00 = _mm256_loadu_ps(input + i_offset++);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,1,0)]); y04 = _mm256_fmadd_ps(y00, y01, y04);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,1,1)]); y07 = _mm256_fmadd_ps(y00, y01, y07);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,1,2)]); y10 = _mm256_fmadd_ps(y00, y01, y10);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,1,3)]); y13 = _mm256_fmadd_ps(y00, y01, y13);

          y00 = _mm256_loadu_ps(input + i_offset++);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,2,0)]); y04 = _mm256_fmadd_ps(y00, y01, y04);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,2,1)]); y07 = _mm256_fmadd_ps(y00, y01, y07);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,2,2)]); y10 = _mm256_fmadd_ps(y00, y01, y10);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,2,3)]); y13 = _mm256_fmadd_ps(y00, y01, y13);
        }
      }

      {
        const int h_=1;
        REP(c_,cbase,cstop-1){
          int i_offset=input_index(c_,h_,w_);
          y00 = _mm256_loadu_ps(input + i_offset++);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,0,0)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,0,0)]); y05 = _mm256_fmadd_ps(y00, y01, y05); y04 = _mm256_fmadd_ps(y00, y02, y04); // k=kbase
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,0,1)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,0,1)]); y08 = _mm256_fmadd_ps(y00, y01, y08); y07 = _mm256_fmadd_ps(y00, y02, y07); // k=kbase+1
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,0,2)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,0,2)]); y11 = _mm256_fmadd_ps(y00, y01, y11); y10 = _mm256_fmadd_ps(y00, y02, y10); // k=kbase+2
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,0,3)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,0,3)]); y14 = _mm256_fmadd_ps(y00, y01, y14); y13 = _mm256_fmadd_ps(y00, y02, y13); // k=kbase+3

          y00 = _mm256_loadu_ps(input + i_offset++);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,1,0)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,1,0)]); y05 = _mm256_fmadd_ps(y00, y01, y05); y04 = _mm256_fmadd_ps(y00, y02, y04); // k=kbase
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,1,1)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,1,1)]); y08 = _mm256_fmadd_ps(y00, y01, y08); y07 = _mm256_fmadd_ps(y00, y02, y07); // k=kbase+1
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,1,2)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,1,2)]); y11 = _mm256_fmadd_ps(y00, y01, y11); y10 = _mm256_fmadd_ps(y00, y02, y10); // k=kbase+2
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,1,3)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,1,3)]); y14 = _mm256_fmadd_ps(y00, y01, y14); y13 = _mm256_fmadd_ps(y00, y02, y13); // k=kbase+3

          y00 = _mm256_loadu_ps(input + i_offset++);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,2,0)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,2,0)]); y05 = _mm256_fmadd_ps(y00, y01, y05); y04 = _mm256_fmadd_ps(y00, y02, y04);// k=kbase
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,2,1)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,2,1)]); y08 = _mm256_fmadd_ps(y00, y01, y08); y07 = _mm256_fmadd_ps(y00, y02, y07);// k=kbase+1
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,2,2)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,2,2)]); y11 = _mm256_fmadd_ps(y00, y01, y11); y10 = _mm256_fmadd_ps(y00, y02, y10);// k=kbase+2
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,2,3)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,2,3)]); y14 = _mm256_fmadd_ps(y00, y01, y14); y13 = _mm256_fmadd_ps(y00, y02, y13);// k=kbase+3
        }

        // when c_=cstop-1
        {
          int c_=cstop-1;
          int i_offset=input_index(c_,h_,w_);
          y00 = _mm256_loadu_ps(input + i_offset++);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,0,0)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,0,0)]); y05 = _mm256_fmadd_ps(y00, y01, y05); y04 = _mm256_fmadd_ps(y00, y02, y04); // k=kbase
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,0,1)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,0,1)]); y08 = _mm256_fmadd_ps(y00, y01, y08); y07 = _mm256_fmadd_ps(y00, y02, y07); // k=kbase+1
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,0,2)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,0,2)]); y11 = _mm256_fmadd_ps(y00, y01, y11); y10 = _mm256_fmadd_ps(y00, y02, y10); // k=kbase+2
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,0,3)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,0,3)]); y14 = _mm256_fmadd_ps(y00, y01, y14); y13 = _mm256_fmadd_ps(y00, y02, y13); // k=kbase+3

          y00 = _mm256_loadu_ps(input + i_offset++);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,1,0)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,1,0)]); y05 = _mm256_fmadd_ps(y00, y01, y05); y04 = _mm256_fmadd_ps(y00, y02, y04); // k=kbase
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,1,1)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,1,1)]); y08 = _mm256_fmadd_ps(y00, y01, y08); y07 = _mm256_fmadd_ps(y00, y02, y07); // k=kbase+1
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,1,2)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,1,2)]); y11 = _mm256_fmadd_ps(y00, y01, y11); y10 = _mm256_fmadd_ps(y00, y02, y10); // k=kbase+2
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,1,3)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,1,3)]); y14 = _mm256_fmadd_ps(y00, y01, y14); y13 = _mm256_fmadd_ps(y00, y02, y13); // k=kbase+3

          y00 = _mm256_loadu_ps(input + i_offset++);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,2,0)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,2,0)]); y05 = _mm256_fmadd_ps(y00, y01, y05); y04 = _mm256_fmadd_ps(y00, y02, y04);// k=kbase
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,2,1)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,2,1)]); y08 = _mm256_fmadd_ps(y00, y01, y08); y07 = _mm256_fmadd_ps(y00, y02, y07);// k=kbase+1
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,2,2)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,2,2)]); y11 = _mm256_fmadd_ps(y00, y01, y11); y10 = _mm256_fmadd_ps(y00, y02, y10);// k=kbase+2
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,2,3)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,2,3)]); y14 = _mm256_fmadd_ps(y00, y01, y14); y13 = _mm256_fmadd_ps(y00, y02, y13);// k=kbase+3
        }
      }

      REP(h_,2,ca.H-2){
        REP(c_,cbase,cstop-1){
          int i_offset=input_index(c_,h_,w_);
          y00 = _mm256_loadu_ps(input + i_offset++);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,0,0)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,0,0)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,0,0)]); y06 = _mm256_fmadd_ps(y00, y01, y06); y05 = _mm256_fmadd_ps(y00, y02, y05); y04 = _mm256_fmadd_ps(y00, y03, y04); // k=kbase
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,0,1)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,0,1)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,0,1)]); y09 = _mm256_fmadd_ps(y00, y01, y09); y08 = _mm256_fmadd_ps(y00, y02, y08); y07 = _mm256_fmadd_ps(y00, y03, y07); // k=kbase+1
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,0,2)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,0,2)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,0,2)]); y12 = _mm256_fmadd_ps(y00, y01, y12); y11 = _mm256_fmadd_ps(y00, y02, y11); y10 = _mm256_fmadd_ps(y00, y03, y10); // k=kbase+2
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,0,3)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,0,3)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,0,3)]); y15 = _mm256_fmadd_ps(y00, y01, y15); y14 = _mm256_fmadd_ps(y00, y02, y14); y13 = _mm256_fmadd_ps(y00, y03, y13); // k=kbase+3

          y00 = _mm256_loadu_ps(input + i_offset++);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,1,0)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,1,0)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,1,0)]); y06 = _mm256_fmadd_ps(y00, y01, y06); y05 = _mm256_fmadd_ps(y00, y02, y05); y04 = _mm256_fmadd_ps(y00, y03, y04);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,1,1)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,1,1)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,1,1)]); y09 = _mm256_fmadd_ps(y00, y01, y09); y08 = _mm256_fmadd_ps(y00, y02, y08); y07 = _mm256_fmadd_ps(y00, y03, y07);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,1,2)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,1,2)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,1,2)]); y12 = _mm256_fmadd_ps(y00, y01, y12); y11 = _mm256_fmadd_ps(y00, y02, y11); y10 = _mm256_fmadd_ps(y00, y03, y10);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,1,3)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,1,3)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,1,3)]); y15 = _mm256_fmadd_ps(y00, y01, y15); y14 = _mm256_fmadd_ps(y00, y02, y14); y13 = _mm256_fmadd_ps(y00, y03, y13);

          y00 = _mm256_loadu_ps(input + i_offset++);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,2,0)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,2,0)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,2,0)]); y06 = _mm256_fmadd_ps(y00, y01, y06); y05 = _mm256_fmadd_ps(y00, y02, y05); y04 = _mm256_fmadd_ps(y00, y03, y04);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,2,1)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,2,1)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,2,1)]); y09 = _mm256_fmadd_ps(y00, y01, y09); y08 = _mm256_fmadd_ps(y00, y02, y05); y07 = _mm256_fmadd_ps(y00, y03, y07);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,2,2)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,2,2)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,2,2)]); y12 = _mm256_fmadd_ps(y00, y01, y12); y11 = _mm256_fmadd_ps(y00, y02, y05); y10 = _mm256_fmadd_ps(y00, y03, y10);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,2,3)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,2,3)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,2,3)]); y15 = _mm256_fmadd_ps(y00, y01, y15); y14 = _mm256_fmadd_ps(y00, y02, y05); y13 = _mm256_fmadd_ps(y00, y03, y13);
        }

        // when c_=cstop-1
        {
          int c_=cstop-1;
          int i_offset=input_index(c_,h_,w_);

          y00 = _mm256_loadu_ps(input + i_offset++);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,0,0)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,0,0)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,0,0)]); y06 = _mm256_fmadd_ps(y00, y01, y06); y05 = _mm256_fmadd_ps(y00, y02, y05); y04 = _mm256_fmadd_ps(y00, y03, y04); // k=kbase
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,0,1)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,0,1)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,0,1)]); y09 = _mm256_fmadd_ps(y00, y01, y09); y08 = _mm256_fmadd_ps(y00, y02, y08); y07 = _mm256_fmadd_ps(y00, y03, y07); // k=kbase+1
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,0,2)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,0,2)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,0,2)]); y12 = _mm256_fmadd_ps(y00, y01, y12); y11 = _mm256_fmadd_ps(y00, y02, y11); y10 = _mm256_fmadd_ps(y00, y03, y10); // k=kbase+2
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,0,3)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,0,3)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,0,3)]); y15 = _mm256_fmadd_ps(y00, y01, y15); y14 = _mm256_fmadd_ps(y00, y02, y14); y13 = _mm256_fmadd_ps(y00, y03, y13); // k=kbase+3

          y00 = _mm256_loadu_ps(input + i_offset++);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,1,0)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,1,0)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,1,0)]); y06 = _mm256_fmadd_ps(y00, y01, y06); y05 = _mm256_fmadd_ps(y00, y02, y05); y04 = _mm256_fmadd_ps(y00, y03, y04);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,1,1)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,1,1)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,1,1)]); y09 = _mm256_fmadd_ps(y00, y01, y09); y08 = _mm256_fmadd_ps(y00, y02, y08); y07 = _mm256_fmadd_ps(y00, y03, y07);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,1,2)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,1,2)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,1,2)]); y12 = _mm256_fmadd_ps(y00, y01, y12); y11 = _mm256_fmadd_ps(y00, y02, y11); y10 = _mm256_fmadd_ps(y00, y03, y10);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,1,3)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,1,3)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,1,3)]); y15 = _mm256_fmadd_ps(y00, y01, y15); y14 = _mm256_fmadd_ps(y00, y02, y14); y13 = _mm256_fmadd_ps(y00, y03, y13);

          y00 = _mm256_loadu_ps(input + i_offset++);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,2,0)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,2,0)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,2,0)]); y06 = _mm256_fmadd_ps(y00, y01, y06); y05 = _mm256_fmadd_ps(y00, y02, y05); y04 = _mm256_fmadd_ps(y00, y03, y04);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,2,1)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,2,1)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,2,1)]); y09 = _mm256_fmadd_ps(y00, y01, y09); y08 = _mm256_fmadd_ps(y00, y02, y05); y07 = _mm256_fmadd_ps(y00, y03, y07);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,2,2)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,2,2)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,2,2)]); y12 = _mm256_fmadd_ps(y00, y01, y12); y11 = _mm256_fmadd_ps(y00, y02, y05); y10 = _mm256_fmadd_ps(y00, y03, y10);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,2,3)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,2,3)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,2,3)]); y15 = _mm256_fmadd_ps(y00, y01, y15); y14 = _mm256_fmadd_ps(y00, y02, y05); y13 = _mm256_fmadd_ps(y00, y03, y13);

          // write y04, y07, y10, y13 to output_
          //print_m256(y04);
          _mm256_storeu_ps(output_+output_index(k*4+0,h_-2,w_), y04); _mm256_storeu_ps((float*)&y04,y05); _mm256_storeu_ps((float*)&y05,y06); y06 = _mm256_setzero_ps(); // k=kbase
          _mm256_storeu_ps(output_+output_index(k*4+1,h_-2,w_), y07); _mm256_storeu_ps((float*)&y07,y08); _mm256_storeu_ps((float*)&y08,y09); y09 = _mm256_setzero_ps(); // k=kbase+1
          _mm256_storeu_ps(output_+output_index(k*4+2,h_-2,w_), y10); _mm256_storeu_ps((float*)&y10,y11); _mm256_storeu_ps((float*)&y11,y12); y12 = _mm256_setzero_ps(); // k=kbase+2
          _mm256_storeu_ps(output_+output_index(k*4+3,h_-2,w_), y13); _mm256_storeu_ps((float*)&y13,y14); _mm256_storeu_ps((float*)&y14,y15); y15 = _mm256_setzero_ps(); // k=kbase+3
        }
      }

      {
        const int h_=ca.H-2;
        REP(c_,cbase,cstop-1){
          int i_offset=input_index(c_,h_,w_);
          y00 = _mm256_loadu_ps(input + i_offset++);
          y02 = _mm256_set1_ps(fr[kid(k,c_,1,0,0)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,0,0)]); y05 = _mm256_fmadd_ps(y00, y02, y05); y04 = _mm256_fmadd_ps(y00, y03, y04);
          y02 = _mm256_set1_ps(fr[kid(k,c_,1,0,1)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,0,1)]); y08 = _mm256_fmadd_ps(y00, y02, y08); y07 = _mm256_fmadd_ps(y00, y03, y07);
          y02 = _mm256_set1_ps(fr[kid(k,c_,1,0,2)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,0,2)]); y11 = _mm256_fmadd_ps(y00, y02, y11); y10 = _mm256_fmadd_ps(y00, y03, y10);
          y02 = _mm256_set1_ps(fr[kid(k,c_,1,0,3)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,0,3)]); y14 = _mm256_fmadd_ps(y00, y02, y14); y13 = _mm256_fmadd_ps(y00, y03, y13);

          y00 = _mm256_loadu_ps(input + i_offset++);
          y02 = _mm256_set1_ps(fr[kid(k,c_,1,1,0)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,1,0)]); y05 = _mm256_fmadd_ps(y00, y02, y05); y04 = _mm256_fmadd_ps(y00, y03, y04);
          y02 = _mm256_set1_ps(fr[kid(k,c_,1,1,1)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,1,1)]); y08 = _mm256_fmadd_ps(y00, y02, y08); y07 = _mm256_fmadd_ps(y00, y03, y07);
          y02 = _mm256_set1_ps(fr[kid(k,c_,1,1,2)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,1,2)]); y11 = _mm256_fmadd_ps(y00, y02, y11); y10 = _mm256_fmadd_ps(y00, y03, y10);
          y02 = _mm256_set1_ps(fr[kid(k,c_,1,1,3)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,1,3)]); y14 = _mm256_fmadd_ps(y00, y02, y14); y13 = _mm256_fmadd_ps(y00, y03, y13);

          y00 = _mm256_loadu_ps(input + i_offset++);
          y02 = _mm256_set1_ps(fr[kid(k,c_,1,2,0)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,2,0)]); y05 = _mm256_fmadd_ps(y00, y02, y05); y04 = _mm256_fmadd_ps(y00, y03, y04);
          y02 = _mm256_set1_ps(fr[kid(k,c_,1,2,1)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,2,1)]); y08 = _mm256_fmadd_ps(y00, y02, y08); y07 = _mm256_fmadd_ps(y00, y03, y07);
          y02 = _mm256_set1_ps(fr[kid(k,c_,1,2,2)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,2,2)]); y11 = _mm256_fmadd_ps(y00, y02, y11); y10 = _mm256_fmadd_ps(y00, y03, y10);
          y02 = _mm256_set1_ps(fr[kid(k,c_,1,2,3)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,2,3)]); y14 = _mm256_fmadd_ps(y00, y02, y14); y13 = _mm256_fmadd_ps(y00, y03, y13);
        }

        // when c_=cstop-1
        {
          int c_=cstop-1;
          int i_offset=input_index(c_,h_,w_);
          y00 = _mm256_loadu_ps(input + i_offset++);
          y02 = _mm256_set1_ps(fr[kid(k,c_,1,0,0)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,0,0)]); y05 = _mm256_fmadd_ps(y00, y02, y05); y04 = _mm256_fmadd_ps(y00, y03, y04);
          y02 = _mm256_set1_ps(fr[kid(k,c_,1,0,1)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,0,1)]); y08 = _mm256_fmadd_ps(y00, y02, y08); y07 = _mm256_fmadd_ps(y00, y03, y07);
          y02 = _mm256_set1_ps(fr[kid(k,c_,1,0,2)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,0,2)]); y11 = _mm256_fmadd_ps(y00, y02, y11); y10 = _mm256_fmadd_ps(y00, y03, y10);
          y02 = _mm256_set1_ps(fr[kid(k,c_,1,0,3)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,0,3)]); y14 = _mm256_fmadd_ps(y00, y02, y14); y13 = _mm256_fmadd_ps(y00, y03, y13);

          y00 = _mm256_loadu_ps(input + i_offset++);
          y02 = _mm256_set1_ps(fr[kid(k,c_,1,1,0)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,1,0)]); y05 = _mm256_fmadd_ps(y00, y02, y05); y04 = _mm256_fmadd_ps(y00, y03, y04);
          y02 = _mm256_set1_ps(fr[kid(k,c_,1,1,1)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,1,1)]); y08 = _mm256_fmadd_ps(y00, y02, y08); y07 = _mm256_fmadd_ps(y00, y03, y07);
          y02 = _mm256_set1_ps(fr[kid(k,c_,1,1,2)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,1,2)]); y11 = _mm256_fmadd_ps(y00, y02, y11); y10 = _mm256_fmadd_ps(y00, y03, y10);
          y02 = _mm256_set1_ps(fr[kid(k,c_,1,1,3)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,1,3)]); y14 = _mm256_fmadd_ps(y00, y02, y14); y13 = _mm256_fmadd_ps(y00, y03, y13);

          y00 = _mm256_loadu_ps(input + i_offset++);
          y02 = _mm256_set1_ps(fr[kid(k,c_,1,2,0)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,2,0)]); y05 = _mm256_fmadd_ps(y00, y02, y05); y04 = _mm256_fmadd_ps(y00, y03, y04);
          y02 = _mm256_set1_ps(fr[kid(k,c_,1,2,1)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,2,1)]); y08 = _mm256_fmadd_ps(y00, y02, y08); y07 = _mm256_fmadd_ps(y00, y03, y07);
          y02 = _mm256_set1_ps(fr[kid(k,c_,1,2,2)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,2,2)]); y11 = _mm256_fmadd_ps(y00, y02, y11); y10 = _mm256_fmadd_ps(y00, y03, y10);
          y02 = _mm256_set1_ps(fr[kid(k,c_,1,2,3)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,2,3)]); y14 = _mm256_fmadd_ps(y00, y02, y14); y13 = _mm256_fmadd_ps(y00, y03, y13);

          // write y04, y07, y10, y13 to output_
          //print_m256(y04);
          _mm256_storeu_ps(output_+output_index(k*4+0,h_-2,w_), y04); _mm256_storeu_ps((float*)&y04,y05); y05 = _mm256_setzero_ps();
          _mm256_storeu_ps(output_+output_index(k*4+1,h_-2,w_), y07); _mm256_storeu_ps((float*)&y07,y08); y08 = _mm256_setzero_ps();
          _mm256_storeu_ps(output_+output_index(k*4+2,h_-2,w_), y10); _mm256_storeu_ps((float*)&y10,y11); y11 = _mm256_setzero_ps();
          _mm256_storeu_ps(output_+output_index(k*4+3,h_-2,w_), y13); _mm256_storeu_ps((float*)&y13,y14); y14 = _mm256_setzero_ps();
        }
      }

      {
        const int h_=ca.H-1;
        REP(c_,cbase,cstop-1){
          int i_offset=input_index(c_,h_,w_);
          y00 = _mm256_loadu_ps(input + i_offset++);
          y03 = _mm256_set1_ps(fr[kid(k,c_,2,0,0)]); y04 = _mm256_fmadd_ps(y00, y03, y04);
          y03 = _mm256_set1_ps(fr[kid(k,c_,2,0,1)]); y07 = _mm256_fmadd_ps(y00, y03, y07);
          y03 = _mm256_set1_ps(fr[kid(k,c_,2,0,2)]); y10 = _mm256_fmadd_ps(y00, y03, y10);
          y03 = _mm256_set1_ps(fr[kid(k,c_,2,0,3)]); y13 = _mm256_fmadd_ps(y00, y03, y13);

          y00 = _mm256_loadu_ps(input + i_offset++);
          y03 = _mm256_set1_ps(fr[kid(k,c_,2,1,0)]); y04 = _mm256_fmadd_ps(y00, y03, y04);
          y03 = _mm256_set1_ps(fr[kid(k,c_,2,1,1)]); y07 = _mm256_fmadd_ps(y00, y03, y07);
          y03 = _mm256_set1_ps(fr[kid(k,c_,2,1,2)]); y10 = _mm256_fmadd_ps(y00, y03, y10);
          y03 = _mm256_set1_ps(fr[kid(k,c_,2,1,3)]); y13 = _mm256_fmadd_ps(y00, y03, y13);

          y00 = _mm256_loadu_ps(input + i_offset++);
          y03 = _mm256_set1_ps(fr[kid(k,c_,2,2,0)]); y04 = _mm256_fmadd_ps(y00, y03, y04);
          y03 = _mm256_set1_ps(fr[kid(k,c_,2,2,1)]); y07 = _mm256_fmadd_ps(y00, y03, y07);
          y03 = _mm256_set1_ps(fr[kid(k,c_,2,2,2)]); y10 = _mm256_fmadd_ps(y00, y03, y10);
          y03 = _mm256_set1_ps(fr[kid(k,c_,2,2,3)]); y13 = _mm256_fmadd_ps(y00, y03, y13);
        }

        // when c_=cstop-1
        {
          int c_=cstop-1;
          int i_offset=input_index(c_,h_,w_);
          y00 = _mm256_loadu_ps(input + i_offset++);
          y03 = _mm256_set1_ps(fr[kid(k,c_,2,0,0)]); y04 = _mm256_fmadd_ps(y00, y03, y04);
          y03 = _mm256_set1_ps(fr[kid(k,c_,2,0,1)]); y07 = _mm256_fmadd_ps(y00, y03, y07);
          y03 = _mm256_set1_ps(fr[kid(k,c_,2,0,2)]); y10 = _mm256_fmadd_ps(y00, y03, y10);
          y03 = _mm256_set1_ps(fr[kid(k,c_,2,0,3)]); y13 = _mm256_fmadd_ps(y00, y03, y13);

          y00 = _mm256_loadu_ps(input + i_offset++);
          y03 = _mm256_set1_ps(fr[kid(k,c_,2,1,0)]); y04 = _mm256_fmadd_ps(y00, y03, y04);
          y03 = _mm256_set1_ps(fr[kid(k,c_,2,1,1)]); y07 = _mm256_fmadd_ps(y00, y03, y07);
          y03 = _mm256_set1_ps(fr[kid(k,c_,2,1,2)]); y10 = _mm256_fmadd_ps(y00, y03, y10);
          y03 = _mm256_set1_ps(fr[kid(k,c_,2,1,3)]); y13 = _mm256_fmadd_ps(y00, y03, y13);

          y00 = _mm256_loadu_ps(input + i_offset++);
          y03 = _mm256_set1_ps(fr[kid(k,c_,2,2,0)]); y04 = _mm256_fmadd_ps(y00, y03, y04);
          y03 = _mm256_set1_ps(fr[kid(k,c_,2,2,1)]); y07 = _mm256_fmadd_ps(y00, y03, y07);
          y03 = _mm256_set1_ps(fr[kid(k,c_,2,2,2)]); y10 = _mm256_fmadd_ps(y00, y03, y10);
          y03 = _mm256_set1_ps(fr[kid(k,c_,2,2,3)]); y13 = _mm256_fmadd_ps(y00, y03, y13);

          //print_m256(y04);
          _mm256_storeu_ps(output_+output_index(k*4+0,h_-2,w_), y04);
          _mm256_storeu_ps(output_+output_index(k*4+1,h_-2,w_), y07);
          _mm256_storeu_ps(output_+output_index(k*4+2,h_-2,w_), y10);
          _mm256_storeu_ps(output_+output_index(k*4+3,h_-2,w_), y13);
          y04 = _mm256_setzero_ps();
          y07 = _mm256_setzero_ps();
          y10 = _mm256_setzero_ps();
          y13 = _mm256_setzero_ps();
        }
      }

    }
    //printf("write to output_\n");
  }
}


void get_input(float* l, int n, int c, int w, int h, float channel_delta = 0.1, float cell_delta = 1, float batch_delta=0.2) {
  //if ((int)l.size() < n * c * w * h) l.resize(n * c * w * h);
  float start = 1.;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < c; j++) {
#ifdef RANDDATA
      start = start / (abs(int(start)) + 1) + ((double)rand() / (RAND_MAX)) + 0.1;
      if (rand() & 1) start *= -1;
#else
      start = 1 + i * batch_delta + j * channel_delta;
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

int main(int argc, char** argv) {
  srand(0xdeadbeef);
  int input_height = 10, input_width = 10, input_channel = 256, filter_batch = 4, kernel_width = 3, kernel_height = 3;
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
  get_input(F, filter_batch, input_channel, kernel_width, kernel_height, 0.1, 1, 0.2);
  printf("filter size: %d\n", filter_batch * input_channel * kernel_width * kernel_height);
  printf("input total size: %.2fKB\n", 1 * input_channel * input_width * input_height / (1024.));
  float* O = (float*)_mm_malloc(sizeof(float) * output_height * output_width * filter_batch, 32);

  fast_conv cw(ca, I, F, O);
  auto start = high_resolution_clock::now();
  cw.run_full();
  long long t = duration_cast<microseconds>((high_resolution_clock::now() - start)).count();
  cout << "\n\n==============" << endl;
  cout << __FUNCTION__ << ":" << input_channel << "," << t << "us" << endl;
  cw.print();
  _mm_free(I), _mm_free(F), _mm_free(O);
  return 0;
}

void print_output(float* Output, int h, int w, int output_channel, bool all) {
  if (all or h * w < 10 * 10) {
    REP(c,0,output_channel){
      for (int i = 0; i < h; i++) {
        for (int j = 0; j < w - 1; j++)
          printf("%.1f,", Output[c*(h*w)+i * w + j]);
        printf("%.1f\n", Output[c*(h*w)+i * w + w - 1]);
      }
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
