// Created by henry on 11/14/22
#include "fast_conv.h"
#include "oracle_common.h"
#define SIMPLE_INDEX

#define MACRO1(i,z) {y01 = _mm256_set1_ps(kernel[kernel_index(i,c_,0,0)]);y##z = _mm256_fmadd_ps(y00, y01, y##z);}
#define MACRO2(i,z) {y01 = _mm256_set1_ps(kernel[kernel_index(i,c_,1,0)]);y##z = _mm256_fmadd_ps(y00, y01, y##z);}
#define MACRO3(i,z) {y01 = _mm256_set1_ps(kernel[kernel_index(i,c_,2,0)]);y##z = _mm256_fmadd_ps(y00, y01, y##z);}

void print_m256(const __m256& v){
  float* f = (float*)&v;
  printf("%f %f %f %f %f %f %f %f\n", f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7]);
}

void fast_conv::run() {
  __m256 y00{}, y01{}, y02{}, y03{};
  __m256 y04{}, y05{}, y06{}, y07{}, y08{}, y09{}, y10{}, y11{}, y12{}, y13{}, y14{}, y15{}; // 8*12
  int k=0;
  REP2(kbase,0,ca.K,4){
    REP2(w_,0,ca.OW,8){

      {
        const int h_=0;
        REP(c_,0,ca.C-1){
          int i_offset=input_index(c_,h_,w_);
          y00 = _mm256_loadu_ps(input + i_offset++);
          k=kbase+0,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,0)]); y04 = _mm256_fmadd_ps(y00, y01, y04); // k=kbase
          k=kbase+1,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,0)]); y07 = _mm256_fmadd_ps(y00, y01, y07); // k=kbase+1
          k=kbase+2,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,0)]); y10 = _mm256_fmadd_ps(y00, y01, y10); // k=kbase+2
          k=kbase+3,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,0)]); y13 = _mm256_fmadd_ps(y00, y01, y13); // k=kbase+3

          y00 = _mm256_loadu_ps(input + i_offset++);
          k=kbase+0,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,1)]); y04 = _mm256_fmadd_ps(y00, y01, y04); // k=kbase
          k=kbase+1,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,1)]); y07 = _mm256_fmadd_ps(y00, y01, y07); // k=kbase+1
          k=kbase+2,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,1)]); y10 = _mm256_fmadd_ps(y00, y01, y10); // k=kbase+2
          k=kbase+3,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,1)]); y13 = _mm256_fmadd_ps(y00, y01, y13); // k=kbase+2

          y00 = _mm256_loadu_ps(input + i_offset++);
          k=kbase+0,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,2)]); y04 = _mm256_fmadd_ps(y00, y01, y04); //k=kbase
          k=kbase+1,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,2)]); y07 = _mm256_fmadd_ps(y00, y01, y07); //k=kbase+1
          k=kbase+2,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,2)]); y10 = _mm256_fmadd_ps(y00, y01, y10); //k=kbase+2
          k=kbase+3,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,2)]); y13 = _mm256_fmadd_ps(y00, y01, y13); //k=kbase+3
        }
        // when c_=ca.C-1
        {
          int c_=ca.C-1;
          int i_offset=input_index(c_,h_,w_);
          y00 = _mm256_loadu_ps(input + i_offset++);
          k=kbase+0,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,0)]); y04 = _mm256_fmadd_ps(y00, y01, y04); // k=kbase
          k=kbase+1,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,0)]); y07 = _mm256_fmadd_ps(y00, y01, y07); // k=kbase+1
          k=kbase+2,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,0)]); y10 = _mm256_fmadd_ps(y00, y01, y10); // k=kbase+2
          k=kbase+3,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,0)]); y13 = _mm256_fmadd_ps(y00, y01, y13); // k=kbase+3

          y00 = _mm256_loadu_ps(input + i_offset++);
          k=kbase+0,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,1)]); y04 = _mm256_fmadd_ps(y00, y01, y04); // k=kbase
          k=kbase+1,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,1)]); y07 = _mm256_fmadd_ps(y00, y01, y07); // k=kbase+1
          k=kbase+2,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,1)]); y10 = _mm256_fmadd_ps(y00, y01, y10); // k=kbase+2
          k=kbase+3,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,1)]); y13 = _mm256_fmadd_ps(y00, y01, y13); // k=kbase+2

          y00 = _mm256_loadu_ps(input + i_offset++);
          k=kbase+0,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,2)]); y04 = _mm256_fmadd_ps(y00, y01, y04); //k=kbase
          k=kbase+1,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,2)]); y07 = _mm256_fmadd_ps(y00, y01, y07); //k=kbase+1
          k=kbase+2,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,2)]); y10 = _mm256_fmadd_ps(y00, y01, y10); //k=kbase+2
          k=kbase+3,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,2)]); y13 = _mm256_fmadd_ps(y00, y01, y13); //k=kbase+3
        }
      }

      {
        const int h_=1;
        REP(c_,0,ca.C-1){
          int i_offset=input_index(c_,h_,w_);
          y00 = _mm256_loadu_ps(input + i_offset++);
          k=kbase+0,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,0)]); y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,0)]); y05 = _mm256_fmadd_ps(y00, y01, y05); y04 = _mm256_fmadd_ps(y00, y02, y04); // k=kbase
          k=kbase+1,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,0)]); y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,0)]); y08 = _mm256_fmadd_ps(y00, y01, y08); y07 = _mm256_fmadd_ps(y00, y02, y07); // k=kbase+1
          k=kbase+2,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,0)]); y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,0)]); y11 = _mm256_fmadd_ps(y00, y01, y11); y10 = _mm256_fmadd_ps(y00, y02, y10); // k=kbase+2
          k=kbase+3,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,0)]); y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,0)]); y14 = _mm256_fmadd_ps(y00, y01, y14); y13 = _mm256_fmadd_ps(y00, y02, y13); // k=kbase+3

          y00 = _mm256_loadu_ps(input + i_offset++);
          k=kbase+0,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,1)]); y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,1)]); y05 = _mm256_fmadd_ps(y00, y01, y05); y04 = _mm256_fmadd_ps(y00, y02, y04); // k=kbase
          k=kbase+1,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,1)]); y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,1)]); y08 = _mm256_fmadd_ps(y00, y01, y08); y07 = _mm256_fmadd_ps(y00, y02, y07); // k=kbase+1
          k=kbase+2,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,1)]); y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,1)]); y11 = _mm256_fmadd_ps(y00, y01, y11); y10 = _mm256_fmadd_ps(y00, y02, y10); // k=kbase+2
          k=kbase+3,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,1)]); y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,1)]); y14 = _mm256_fmadd_ps(y00, y01, y14); y13 = _mm256_fmadd_ps(y00, y02, y13); // k=kbase+3

          y00 = _mm256_loadu_ps(input + i_offset++);
          k=kbase+0,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,2)]); y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,2)]); y05 = _mm256_fmadd_ps(y00, y01, y05); y04 = _mm256_fmadd_ps(y00, y02, y04);// k=kbase
          k=kbase+1,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,2)]); y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,2)]); y08 = _mm256_fmadd_ps(y00, y01, y08); y07 = _mm256_fmadd_ps(y00, y02, y07);// k=kbase+1
          k=kbase+2,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,2)]); y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,2)]); y11 = _mm256_fmadd_ps(y00, y01, y11); y10 = _mm256_fmadd_ps(y00, y02, y10);// k=kbase+2
          k=kbase+3,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,2)]); y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,2)]); y14 = _mm256_fmadd_ps(y00, y01, y14); y13 = _mm256_fmadd_ps(y00, y02, y13);// k=kbase+3
        }

        // when c_=ca.C-1
        {
          int c_=ca.C-1;
          int i_offset=input_index(c_,h_,w_);
          y00 = _mm256_loadu_ps(input + i_offset++);
          k=kbase+0,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,0)]); y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,0)]); y05 = _mm256_fmadd_ps(y00, y01, y05); y04 = _mm256_fmadd_ps(y00, y02, y04); // k=kbase
          k=kbase+1,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,0)]); y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,0)]); y08 = _mm256_fmadd_ps(y00, y01, y08); y07 = _mm256_fmadd_ps(y00, y02, y07); // k=kbase+1
          k=kbase+2,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,0)]); y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,0)]); y11 = _mm256_fmadd_ps(y00, y01, y11); y10 = _mm256_fmadd_ps(y00, y02, y10); // k=kbase+2
          k=kbase+3,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,0)]); y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,0)]); y14 = _mm256_fmadd_ps(y00, y01, y14); y13 = _mm256_fmadd_ps(y00, y02, y13); // k=kbase+3

          y00 = _mm256_loadu_ps(input + i_offset++);
          k=kbase+0,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,1)]); y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,1)]); y05 = _mm256_fmadd_ps(y00, y01, y05); y04 = _mm256_fmadd_ps(y00, y02, y04); // k=kbase
          k=kbase+1,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,1)]); y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,1)]); y08 = _mm256_fmadd_ps(y00, y01, y08); y07 = _mm256_fmadd_ps(y00, y02, y07); // k=kbase+1
          k=kbase+2,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,1)]); y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,1)]); y11 = _mm256_fmadd_ps(y00, y01, y11); y10 = _mm256_fmadd_ps(y00, y02, y10); // k=kbase+2
          k=kbase+3,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,1)]); y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,1)]); y14 = _mm256_fmadd_ps(y00, y01, y14); y13 = _mm256_fmadd_ps(y00, y02, y13); // k=kbase+3

          y00 = _mm256_loadu_ps(input + i_offset++);
          k=kbase+0,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,2)]); y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,2)]); y05 = _mm256_fmadd_ps(y00, y01, y05); y04 = _mm256_fmadd_ps(y00, y02, y04);// k=kbase
          k=kbase+1,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,2)]); y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,2)]); y08 = _mm256_fmadd_ps(y00, y01, y08); y07 = _mm256_fmadd_ps(y00, y02, y07);// k=kbase+1
          k=kbase+2,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,2)]); y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,2)]); y11 = _mm256_fmadd_ps(y00, y01, y11); y10 = _mm256_fmadd_ps(y00, y02, y10);// k=kbase+2
          k=kbase+3,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,2)]); y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,2)]); y14 = _mm256_fmadd_ps(y00, y01, y14); y13 = _mm256_fmadd_ps(y00, y02, y13);// k=kbase+3
        }
      }

      REP(h_,2,ca.H-2){
        REP(c_,0,ca.C-1){
          int i_offset=input_index(c_,h_,w_);
          y00 = _mm256_loadu_ps(input + i_offset++);
          k=kbase+0,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,0)]); y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,0)]); y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,0)]); y06 = _mm256_fmadd_ps(y00, y01, y06); y05 = _mm256_fmadd_ps(y00, y02, y05); y04 = _mm256_fmadd_ps(y00, y03, y04); // k=kbase
          k=kbase+1,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,0)]); y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,0)]); y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,0)]); y09 = _mm256_fmadd_ps(y00, y01, y09); y08 = _mm256_fmadd_ps(y00, y02, y08); y07 = _mm256_fmadd_ps(y00, y03, y07); // k=kbase+1
          k=kbase+2,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,0)]); y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,0)]); y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,0)]); y12 = _mm256_fmadd_ps(y00, y01, y12); y11 = _mm256_fmadd_ps(y00, y02, y11); y10 = _mm256_fmadd_ps(y00, y03, y10); // k=kbase+2
          k=kbase+3,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,0)]); y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,0)]); y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,0)]); y15 = _mm256_fmadd_ps(y00, y01, y15); y14 = _mm256_fmadd_ps(y00, y02, y14); y13 = _mm256_fmadd_ps(y00, y03, y13); // k=kbase+3

          y00 = _mm256_loadu_ps(input + i_offset++);
          k=kbase+0,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,1)]); y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,1)]); y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,1)]); y06 = _mm256_fmadd_ps(y00, y01, y06); y05 = _mm256_fmadd_ps(y00, y02, y05); y04 = _mm256_fmadd_ps(y00, y03, y04);
          k=kbase+1,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,1)]); y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,1)]); y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,1)]); y09 = _mm256_fmadd_ps(y00, y01, y09); y08 = _mm256_fmadd_ps(y00, y02, y08); y07 = _mm256_fmadd_ps(y00, y03, y07);
          k=kbase+2,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,1)]); y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,1)]); y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,1)]); y12 = _mm256_fmadd_ps(y00, y01, y12); y11 = _mm256_fmadd_ps(y00, y02, y11); y10 = _mm256_fmadd_ps(y00, y03, y10);
          k=kbase+3,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,1)]); y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,1)]); y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,1)]); y15 = _mm256_fmadd_ps(y00, y01, y15); y14 = _mm256_fmadd_ps(y00, y02, y14); y13 = _mm256_fmadd_ps(y00, y03, y13);

          y00 = _mm256_loadu_ps(input + i_offset++);
          k=kbase+0,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,2)]); y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,2)]); y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,2)]); y06 = _mm256_fmadd_ps(y00, y01, y06); y05 = _mm256_fmadd_ps(y00, y02, y05); y04 = _mm256_fmadd_ps(y00, y03, y04);
          k=kbase+1,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,2)]); y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,2)]); y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,2)]); y09 = _mm256_fmadd_ps(y00, y01, y09); y08 = _mm256_fmadd_ps(y00, y02, y05); y07 = _mm256_fmadd_ps(y00, y03, y07);
          k=kbase+2,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,2)]); y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,2)]); y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,2)]); y12 = _mm256_fmadd_ps(y00, y01, y12); y11 = _mm256_fmadd_ps(y00, y02, y05); y10 = _mm256_fmadd_ps(y00, y03, y10);
          k=kbase+3,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,2)]); y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,2)]); y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,2)]); y15 = _mm256_fmadd_ps(y00, y01, y15); y14 = _mm256_fmadd_ps(y00, y02, y05); y13 = _mm256_fmadd_ps(y00, y03, y13);
        }

        // when c_=ca.C-1
        {
          int c_=ca.C-1;
          int i_offset=input_index(c_,h_,w_);

          y00 = _mm256_loadu_ps(input + i_offset++);
          k=kbase+0,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,0)]); y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,0)]); y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,0)]); y06 = _mm256_fmadd_ps(y00, y01, y06); y05 = _mm256_fmadd_ps(y00, y02, y05); y04 = _mm256_fmadd_ps(y00, y03, y04); // k=kbase
          k=kbase+1,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,0)]); y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,0)]); y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,0)]); y09 = _mm256_fmadd_ps(y00, y01, y09); y08 = _mm256_fmadd_ps(y00, y02, y08); y07 = _mm256_fmadd_ps(y00, y03, y07); // k=kbase+1
          k=kbase+2,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,0)]); y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,0)]); y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,0)]); y12 = _mm256_fmadd_ps(y00, y01, y12); y11 = _mm256_fmadd_ps(y00, y02, y11); y10 = _mm256_fmadd_ps(y00, y03, y10); // k=kbase+2
          k=kbase+3,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,0)]); y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,0)]); y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,0)]); y15 = _mm256_fmadd_ps(y00, y01, y15); y14 = _mm256_fmadd_ps(y00, y02, y14); y13 = _mm256_fmadd_ps(y00, y03, y13); // k=kbase+3

          y00 = _mm256_loadu_ps(input + i_offset++);
          k=kbase+0,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,1)]); y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,1)]); y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,1)]); y06 = _mm256_fmadd_ps(y00, y01, y06); y05 = _mm256_fmadd_ps(y00, y02, y05); y04 = _mm256_fmadd_ps(y00, y03, y04);
          k=kbase+1,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,1)]); y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,1)]); y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,1)]); y09 = _mm256_fmadd_ps(y00, y01, y09); y08 = _mm256_fmadd_ps(y00, y02, y08); y07 = _mm256_fmadd_ps(y00, y03, y07);
          k=kbase+2,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,1)]); y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,1)]); y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,1)]); y12 = _mm256_fmadd_ps(y00, y01, y12); y11 = _mm256_fmadd_ps(y00, y02, y11); y10 = _mm256_fmadd_ps(y00, y03, y10);
          k=kbase+3,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,1)]); y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,1)]); y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,1)]); y15 = _mm256_fmadd_ps(y00, y01, y15); y14 = _mm256_fmadd_ps(y00, y02, y14); y13 = _mm256_fmadd_ps(y00, y03, y13);

          y00 = _mm256_loadu_ps(input + i_offset++);
          k=kbase+0,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,2)]); y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,2)]); y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,2)]); y06 = _mm256_fmadd_ps(y00, y01, y06); y05 = _mm256_fmadd_ps(y00, y02, y05); y04 = _mm256_fmadd_ps(y00, y03, y04);
          k=kbase+1,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,2)]); y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,2)]); y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,2)]); y09 = _mm256_fmadd_ps(y00, y01, y09); y08 = _mm256_fmadd_ps(y00, y02, y05); y07 = _mm256_fmadd_ps(y00, y03, y07);
          k=kbase+2,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,2)]); y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,2)]); y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,2)]); y12 = _mm256_fmadd_ps(y00, y01, y12); y11 = _mm256_fmadd_ps(y00, y02, y05); y10 = _mm256_fmadd_ps(y00, y03, y10);
          k=kbase+3,y01 = _mm256_set1_ps(kernel[kernel_index(k,c_,0,2)]); y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,2)]); y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,2)]); y15 = _mm256_fmadd_ps(y00, y01, y15); y14 = _mm256_fmadd_ps(y00, y02, y05); y13 = _mm256_fmadd_ps(y00, y03, y13);

          // write y04, y07, y10, y11 to output
          //print_m256(y04);
          k=kbase+0,_mm256_storeu_ps(output+output_index(k,h_-2,w_), y04); _mm256_storeu_ps((float*)&y04,y05); _mm256_storeu_ps((float*)&y05,y06); y06 = _mm256_setzero_ps(); // k=kbase
          k=kbase+1,_mm256_storeu_ps(output+output_index(k,h_-2,w_), y07); _mm256_storeu_ps((float*)&y07,y08); _mm256_storeu_ps((float*)&y08,y09); y09 = _mm256_setzero_ps(); // k=kbase+1
          k=kbase+2,_mm256_storeu_ps(output+output_index(k,h_-2,w_), y10); _mm256_storeu_ps((float*)&y10,y11); _mm256_storeu_ps((float*)&y11,y12); y12 = _mm256_setzero_ps(); // k=kbase+2
          k=kbase+3,_mm256_storeu_ps(output+output_index(k,h_-2,w_), y13); _mm256_storeu_ps((float*)&y13,y14); _mm256_storeu_ps((float*)&y14,y15); y15 = _mm256_setzero_ps(); // k=kbase+3
        }
      }

      {
        const int h_=ca.H-2;
        REP(c_,0,ca.C-1){
          int i_offset=input_index(c_,h_,w_);
          y00 = _mm256_loadu_ps(input + i_offset++);
          k=kbase+0,y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,0)]); y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,0)]); y05 = _mm256_fmadd_ps(y00, y02, y05); y04 = _mm256_fmadd_ps(y00, y03, y04);
          k=kbase+1,y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,0)]); y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,0)]); y08 = _mm256_fmadd_ps(y00, y02, y08); y07 = _mm256_fmadd_ps(y00, y03, y07);
          k=kbase+2,y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,0)]); y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,0)]); y11 = _mm256_fmadd_ps(y00, y02, y11); y10 = _mm256_fmadd_ps(y00, y03, y10);
          k=kbase+3,y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,0)]); y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,0)]); y14 = _mm256_fmadd_ps(y00, y02, y14); y13 = _mm256_fmadd_ps(y00, y03, y13);

          y00 = _mm256_loadu_ps(input + i_offset++);
          k=kbase+0,y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,1)]); y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,1)]); y05 = _mm256_fmadd_ps(y00, y02, y05); y04 = _mm256_fmadd_ps(y00, y03, y04);
          k=kbase+1,y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,1)]); y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,1)]); y08 = _mm256_fmadd_ps(y00, y02, y08); y07 = _mm256_fmadd_ps(y00, y03, y07);
          k=kbase+2,y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,1)]); y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,1)]); y11 = _mm256_fmadd_ps(y00, y02, y11); y10 = _mm256_fmadd_ps(y00, y03, y10);
          k=kbase+3,y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,1)]); y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,1)]); y14 = _mm256_fmadd_ps(y00, y02, y14); y13 = _mm256_fmadd_ps(y00, y03, y13);

          y00 = _mm256_loadu_ps(input + i_offset++);
          k=kbase+0,y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,2)]); y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,2)]); y05 = _mm256_fmadd_ps(y00, y02, y05); y04 = _mm256_fmadd_ps(y00, y03, y04);
          k=kbase+1,y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,2)]); y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,2)]); y08 = _mm256_fmadd_ps(y00, y02, y08); y07 = _mm256_fmadd_ps(y00, y03, y07);
          k=kbase+2,y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,2)]); y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,2)]); y11 = _mm256_fmadd_ps(y00, y02, y11); y10 = _mm256_fmadd_ps(y00, y03, y10);
          k=kbase+3,y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,2)]); y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,2)]); y14 = _mm256_fmadd_ps(y00, y02, y14); y13 = _mm256_fmadd_ps(y00, y03, y13);
        }

        // when c_=ca.C-1
        {
          int c_=ca.C-1;
          int i_offset=input_index(c_,h_,w_);
          y00 = _mm256_loadu_ps(input + i_offset++);
          k=kbase+0,y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,0)]); y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,0)]); y05 = _mm256_fmadd_ps(y00, y02, y05); y04 = _mm256_fmadd_ps(y00, y03, y04);
          k=kbase+1,y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,0)]); y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,0)]); y08 = _mm256_fmadd_ps(y00, y02, y08); y07 = _mm256_fmadd_ps(y00, y03, y07);
          k=kbase+2,y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,0)]); y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,0)]); y11 = _mm256_fmadd_ps(y00, y02, y11); y10 = _mm256_fmadd_ps(y00, y03, y10);
          k=kbase+3,y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,0)]); y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,0)]); y14 = _mm256_fmadd_ps(y00, y02, y14); y13 = _mm256_fmadd_ps(y00, y03, y13);

          y00 = _mm256_loadu_ps(input + i_offset++);
          k=kbase+0,y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,1)]); y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,1)]); y05 = _mm256_fmadd_ps(y00, y02, y05); y04 = _mm256_fmadd_ps(y00, y03, y04);
          k=kbase+1,y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,1)]); y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,1)]); y08 = _mm256_fmadd_ps(y00, y02, y08); y07 = _mm256_fmadd_ps(y00, y03, y07);
          k=kbase+2,y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,1)]); y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,1)]); y11 = _mm256_fmadd_ps(y00, y02, y11); y10 = _mm256_fmadd_ps(y00, y03, y10);
          k=kbase+3,y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,1)]); y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,1)]); y14 = _mm256_fmadd_ps(y00, y02, y14); y13 = _mm256_fmadd_ps(y00, y03, y13);

          y00 = _mm256_loadu_ps(input + i_offset++);
          k=kbase+0,y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,2)]); y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,2)]); y05 = _mm256_fmadd_ps(y00, y02, y05); y04 = _mm256_fmadd_ps(y00, y03, y04);
          k=kbase+1,y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,2)]); y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,2)]); y08 = _mm256_fmadd_ps(y00, y02, y08); y07 = _mm256_fmadd_ps(y00, y03, y07);
          k=kbase+2,y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,2)]); y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,2)]); y11 = _mm256_fmadd_ps(y00, y02, y11); y10 = _mm256_fmadd_ps(y00, y03, y10);
          k=kbase+3,y02 = _mm256_set1_ps(kernel[kernel_index(k,c_,1,2)]); y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,2)]); y14 = _mm256_fmadd_ps(y00, y02, y14); y13 = _mm256_fmadd_ps(y00, y03, y13);

          // write y04, y07, y10, y11 to output
          //print_m256(y04);
          k=kbase+0,_mm256_storeu_ps(output+output_index(k,h_-2,w_), y04); _mm256_storeu_ps((float*)&y04,y05); y05 = _mm256_setzero_ps();
          k=kbase+1,_mm256_storeu_ps(output+output_index(k,h_-2,w_), y07); _mm256_storeu_ps((float*)&y07,y08); y08 = _mm256_setzero_ps();
          k=kbase+2,_mm256_storeu_ps(output+output_index(k,h_-2,w_), y10); _mm256_storeu_ps((float*)&y10,y11); y11 = _mm256_setzero_ps();
          k=kbase+3,_mm256_storeu_ps(output+output_index(k,h_-2,w_), y13); _mm256_storeu_ps((float*)&y13,y14); y14 = _mm256_setzero_ps();
        }
      }

      {
        const int h_=ca.H-1;
        REP(c_,0,ca.C-1){
          int i_offset=input_index(c_,h_,w_);
          y00 = _mm256_loadu_ps(input + i_offset++);
          k=kbase+0,y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,0)]); y04 = _mm256_fmadd_ps(y00, y03, y04);
          k=kbase+1,y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,0)]); y07 = _mm256_fmadd_ps(y00, y03, y07);
          k=kbase+2,y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,0)]); y10 = _mm256_fmadd_ps(y00, y03, y10);
          k=kbase+3,y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,0)]); y13 = _mm256_fmadd_ps(y00, y03, y13);

          y00 = _mm256_loadu_ps(input + i_offset++);
          k=kbase+0,y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,1)]); y04 = _mm256_fmadd_ps(y00, y03, y04);
          k=kbase+1,y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,1)]); y07 = _mm256_fmadd_ps(y00, y03, y07);
          k=kbase+2,y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,1)]); y10 = _mm256_fmadd_ps(y00, y03, y10);
          k=kbase+3,y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,1)]); y13 = _mm256_fmadd_ps(y00, y03, y13);
          //MACRO2(0, 4); MACRO2(1, 5); MACRO2(2, 6); MACRO2(3, 7);

          y00 = _mm256_loadu_ps(input + i_offset++);
          k=kbase+0,y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,2)]); y04 = _mm256_fmadd_ps(y00, y03, y04);
          k=kbase+1,y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,2)]); y07 = _mm256_fmadd_ps(y00, y03, y07);
          k=kbase+2,y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,2)]); y10 = _mm256_fmadd_ps(y00, y03, y10);
          k=kbase+3,y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,2)]); y13 = _mm256_fmadd_ps(y00, y03, y13);
        }

        // when c_=ca.C-1
        {
          int c_=ca.C-1;
          int i_offset=input_index(c_,h_,w_);
          y00 = _mm256_loadu_ps(input + i_offset++);
          k=kbase+0,y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,0)]); y04 = _mm256_fmadd_ps(y00, y03, y04);
          k=kbase+1,y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,0)]); y07 = _mm256_fmadd_ps(y00, y03, y07);
          k=kbase+2,y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,0)]); y10 = _mm256_fmadd_ps(y00, y03, y10);
          k=kbase+3,y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,0)]); y13 = _mm256_fmadd_ps(y00, y03, y13);

          y00 = _mm256_loadu_ps(input + i_offset++);
          k=kbase+0,y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,1)]); y04 = _mm256_fmadd_ps(y00, y03, y04);
          k=kbase+1,y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,1)]); y07 = _mm256_fmadd_ps(y00, y03, y07);
          k=kbase+2,y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,1)]); y10 = _mm256_fmadd_ps(y00, y03, y10);
          k=kbase+3,y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,1)]); y13 = _mm256_fmadd_ps(y00, y03, y13);
          //MACRO2(0, 4); MACRO2(1, 5); MACRO2(2, 6); MACRO2(3, 7);

          y00 = _mm256_loadu_ps(input + i_offset++);
          k=kbase+0,y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,2)]); y04 = _mm256_fmadd_ps(y00, y03, y04);
          k=kbase+1,y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,2)]); y07 = _mm256_fmadd_ps(y00, y03, y07);
          k=kbase+2,y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,2)]); y10 = _mm256_fmadd_ps(y00, y03, y10);
          k=kbase+3,y03 = _mm256_set1_ps(kernel[kernel_index(k,c_,2,2)]); y13 = _mm256_fmadd_ps(y00, y03, y13);

          //print_m256(y04);
          k=kbase+0,_mm256_storeu_ps(output+output_index(k,h_-2,w_), y04);
          k=kbase+1,_mm256_storeu_ps(output+output_index(k,h_-2,w_), y07);
          k=kbase+2,_mm256_storeu_ps(output+output_index(k,h_-2,w_), y10);
          k=kbase+3,_mm256_storeu_ps(output+output_index(k,h_-2,w_), y13);
          y04 = _mm256_setzero_ps();
          y07 = _mm256_setzero_ps();
          y10 = _mm256_setzero_ps();
          y13 = _mm256_setzero_ps();
        }
      }

    }
    //printf("write to output\n");
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
  printf("\n");
}
