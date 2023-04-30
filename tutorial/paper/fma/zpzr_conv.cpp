#include "zpzr_conv.h"
#define SIMPLE_INDEX

int ceil_int(int x, int y) {
  return int(ceil((float)x / y));
}
int floor_int(int x, int y) {
  return int(floor((float)x / y));
}


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
  if (ca.R==2) return;
  auto start = high_resolution_clock::now();
  fr = (float*)_mm_malloc(sizeof(float) * filter_size, 32); // TODO: maybe too small
  int ori_idx = 0, new_idx = 0;
  REP(i, 0, slice_number_in_batch_dim) {      // K'
    REP(j, 0, ca.C) {                         // C
      REP(k, 0, ca.R) {                       // R
        REP(l, 0, ca.L) {                     // L
          REP(n, 0, k_split) {                // k_ - TODO
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

void fast_conv::reorder_input_NHbcw8(){
  auto start = high_resolution_clock::now();
  input_nhbcw8 = (float*)_mm_malloc(sizeof(float) * input_size, 32);
  float* tmp = input_nhbcw8;
  int safe_batch = ca.W/8;

  float* input_base = input + ((ca.W + 1) << 1);
  float* input_src;

  for (int i = 0; i < ca.H; i++) {          // height
    for (int j = 0; j < safe_batch; j++) {  // width
      auto tmp_base = input_base + i * ca.W + j*8;
      for (int c_ = 0; c_ < ca.C; c_++) {   // channel
        input_src = tmp_base + c_ * input_channel_stride;
        ::memcpy(tmp, input_src, 8 * sizeof(float));
        tmp += 8;
      }
    }
  }
  long long t = duration_cast<nanoseconds>((high_resolution_clock::now() - start)).count();
  cout << __FUNCTION__ << " | reorder Time: " << t << " ns" << endl;
}

void fast_conv::reorder_input_NhcW(){
  auto start = high_resolution_clock::now();
  input_nhcw = (float*)_mm_malloc(sizeof(float) * input_size, 32);
  float* tmp = input_nhcw;
  REP(i,0,ca.H) {
    REP(j,0,ca.C) {
        ::memcpy(tmp, input+ input_index_nchw(j, i, 0), ca.W * sizeof(float));
        tmp += ca.W;
      }
  }
  long long t = duration_cast<nanoseconds>((high_resolution_clock::now() - start)).count();
  cout << __FUNCTION__ << " | reorder Time: " << t << " ns" << endl;
}

void fast_conv::reorder_input_NcHc8W(){
  auto start = high_resolution_clock::now();
  input_nchc8w = (float*)_mm_malloc(sizeof(float) * input_size, 32);
  float* tmp = input_nchc8w;
  REP(i,0,ca.C/8) {
      REP(j,0,ca.H) {
        REP(k,0,8) {
          ::memcpy(tmp, input+ input_index_nchw(k + i * 8, j, 0), ca.W * sizeof(float));
          tmp += ca.W;
        }
      }
  }
  long long t = duration_cast<nanoseconds>((high_resolution_clock::now() - start)).count();
  cout << __FUNCTION__ << " | reorder Time: " << t << " ns" << endl;
}

void fast_conv::run_full(){
  cout << __FUNCTION__ << " | thread_num: " << thread_num << endl;
  if(thread_num>0 and ca.C>32){
      std::mutex iomutex;
      auto loop = [&](const int a, const int b){
        auto start = high_resolution_clock::now();
        //std::lock_guard<std::mutex> iolock(iomutex);
        std::cout << "Thread #" << a/CHANNEL_SPLIT << ": on CPU " << sched_getcpu() << " (" << a << "," << b << ")" << CHANNEL_SPLIT << "\n";
        auto output_ = out_buff[floor_int(a,CHANNEL_SPLIT)];
        run_nchw(output, a, min(a+CHANNEL_SPLIT, ca.C));
      };
      //pool.parallelize_loop(0, ca.C, loop, out_buff_num).wait();
      auto start = high_resolution_clock::now();
      REP(i,0,out_buff_num){
        matrix_fuse(output, out_buff[i], output_size);
      }
      auto t = duration_cast<microseconds>((high_resolution_clock::now() - start)).count();
      cout << "matrix_fuse: " << t << " us" << endl;
      return;
  }else{
    if (ca.R==2 and ca.L==2){
      REP2(cbase,0,ca.C,CHANNEL_SPLIT){
        run_nchw2x2(output, cbase, min(cbase+CHANNEL_SPLIT, ca.C));
      }
    }else{
      REP2(cbase,0,ca.C,CHANNEL_SPLIT){
        run_nchw(output, cbase, min(cbase+CHANNEL_SPLIT, ca.C));
      }
    }
  }
}

void fast_conv::run_full_nchc8w(){
  run_nchc8w(output, 0, min(ca.C, CHANNEL_SPLIT));
  REP2(cbase,CHANNEL_SPLIT,ca.C,CHANNEL_SPLIT){
    run_nchc8w(out_buff[cbase/CHANNEL_SPLIT-1], cbase, min(cbase+CHANNEL_SPLIT, ca.C));
  }
  auto start = high_resolution_clock::now();
  REP(i,0,out_buff_num){
    matrix_fuse(output, out_buff[i], output_size);
  }
  auto t = duration_cast<microseconds>((high_resolution_clock::now() - start)).count();
  cout << "matrix_fuse: " << t << " us" << endl;
}

union floatpun { int i; float f; } fp;
void fast_conv::run_nchw(float* output_, int cbase, int cstop) {
  __m256 y00{}, y01{}, y02{}, y03{};
  __m256 y04{}, y05{}, y06{}, y07{}, y08{}, y09{}, y10{}, y11{}, y12{}, y13{}, y14{}, y15{}; // 8*12
  float* d= nullptr;
  REP(k,0,slice_number_in_batch_dim){
    REP2(w_,0,ca.OW,8){
      // TC: O(9*CHANNEL_SPLIT*H=288H)
      {
        const int h_=0;
        REP(c_,cbase,cstop){
          int i_offset= input_index_nchw(c_, h_, w_);
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
        REP(c_,cbase,cstop){
          int i_offset= input_index_nchw(c_, h_, w_);
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
        REP(c_,cbase,cstop){
          int i_offset= input_index_nchw(c_, h_, w_);
          y00 = _mm256_loadu_ps(input + i_offset++); y01 = _mm256_loadu_ps(input + i_offset++); y02 = _mm256_loadu_ps(input + i_offset++);

          y03 = _mm256_set1_ps(fr[kid(k,c_,0,0,0)]); y06 = _mm256_fmadd_ps(y00, y03, y06); y03 = _mm256_set1_ps(fr[kid(k,c_,1,0,0)]); y05 = _mm256_fmadd_ps(y00, y03, y05); y03 = _mm256_set1_ps(fr[kid(k,c_,2,0,0)]); y04 = _mm256_fmadd_ps(y00, y03, y04);
          y03 = _mm256_set1_ps(fr[kid(k,c_,0,0,1)]); y09 = _mm256_fmadd_ps(y00, y03, y09); y03 = _mm256_set1_ps(fr[kid(k,c_,1,0,1)]); y08 = _mm256_fmadd_ps(y00, y03, y08); y03 = _mm256_set1_ps(fr[kid(k,c_,2,0,1)]); y07 = _mm256_fmadd_ps(y00, y03, y07);
          y03 = _mm256_set1_ps(fr[kid(k,c_,0,0,2)]); y12 = _mm256_fmadd_ps(y00, y03, y12); y03 = _mm256_set1_ps(fr[kid(k,c_,1,0,2)]); y11 = _mm256_fmadd_ps(y00, y03, y11); y03 = _mm256_set1_ps(fr[kid(k,c_,2,0,2)]); y10 = _mm256_fmadd_ps(y00, y03, y10);
          y03 = _mm256_set1_ps(fr[kid(k,c_,0,0,3)]); y15 = _mm256_fmadd_ps(y00, y03, y15); y03 = _mm256_set1_ps(fr[kid(k,c_,1,0,3)]); y14 = _mm256_fmadd_ps(y00, y03, y14); y03 = _mm256_set1_ps(fr[kid(k,c_,2,0,3)]); y13 = _mm256_fmadd_ps(y00, y03, y13);

          y03 = _mm256_set1_ps(fr[kid(k,c_,0,1,0)]); y06 = _mm256_fmadd_ps(y01, y03, y06); y03 = _mm256_set1_ps(fr[kid(k,c_,1,1,0)]); y05 = _mm256_fmadd_ps(y01, y03, y05); y03 = _mm256_set1_ps(fr[kid(k,c_,2,1,0)]); y04 = _mm256_fmadd_ps(y01, y03, y04);
          y03 = _mm256_set1_ps(fr[kid(k,c_,0,1,1)]); y09 = _mm256_fmadd_ps(y01, y03, y09); y03 = _mm256_set1_ps(fr[kid(k,c_,1,1,1)]); y08 = _mm256_fmadd_ps(y01, y03, y08); y03 = _mm256_set1_ps(fr[kid(k,c_,2,1,1)]); y07 = _mm256_fmadd_ps(y01, y03, y07);
          y03 = _mm256_set1_ps(fr[kid(k,c_,0,1,2)]); y12 = _mm256_fmadd_ps(y01, y03, y12); y03 = _mm256_set1_ps(fr[kid(k,c_,1,1,2)]); y11 = _mm256_fmadd_ps(y01, y03, y11); y03 = _mm256_set1_ps(fr[kid(k,c_,2,1,2)]); y10 = _mm256_fmadd_ps(y01, y03, y10);
          y03 = _mm256_set1_ps(fr[kid(k,c_,0,1,3)]); y15 = _mm256_fmadd_ps(y01, y03, y15); y03 = _mm256_set1_ps(fr[kid(k,c_,1,1,3)]); y14 = _mm256_fmadd_ps(y01, y03, y14); y03 = _mm256_set1_ps(fr[kid(k,c_,2,1,3)]); y13 = _mm256_fmadd_ps(y01, y03, y13);

          y03 = _mm256_set1_ps(fr[kid(k,c_,0,2,0)]); y06 = _mm256_fmadd_ps(y02, y03, y06); y03 = _mm256_set1_ps(fr[kid(k,c_,1,2,0)]); y05 = _mm256_fmadd_ps(y02, y03, y05); y03 = _mm256_set1_ps(fr[kid(k,c_,2,2,0)]); y04 = _mm256_fmadd_ps(y02, y03, y04);
          y03 = _mm256_set1_ps(fr[kid(k,c_,0,2,1)]); y09 = _mm256_fmadd_ps(y02, y03, y09); y03 = _mm256_set1_ps(fr[kid(k,c_,1,2,1)]); y08 = _mm256_fmadd_ps(y02, y03, y08); y03 = _mm256_set1_ps(fr[kid(k,c_,2,2,1)]); y07 = _mm256_fmadd_ps(y02, y03, y07);
          y03 = _mm256_set1_ps(fr[kid(k,c_,0,2,2)]); y12 = _mm256_fmadd_ps(y02, y03, y12); y03 = _mm256_set1_ps(fr[kid(k,c_,1,2,2)]); y11 = _mm256_fmadd_ps(y02, y03, y11); y03 = _mm256_set1_ps(fr[kid(k,c_,2,2,2)]); y10 = _mm256_fmadd_ps(y02, y03, y10);
          y03 = _mm256_set1_ps(fr[kid(k,c_,0,2,3)]); y15 = _mm256_fmadd_ps(y02, y03, y15); y03 = _mm256_set1_ps(fr[kid(k,c_,1,2,3)]); y14 = _mm256_fmadd_ps(y02, y03, y14); y03 = _mm256_set1_ps(fr[kid(k,c_,2,2,3)]); y13 = _mm256_fmadd_ps(y02, y03, y13);
        }

        // when c_=cstop-1
        {
          // write y04, y07, y10, y13 to output_, print_m256(y04);
          d=output_+oid(k*4+0,h_-2,w_); _mm256_storeu_ps(d, _mm256_add_ps(y04, _mm256_loadu_ps(d))); _mm256_storeu_ps((float*)&y04,y05); _mm256_storeu_ps((float*)&y05,y06); y06 = _mm256_setzero_ps(); // k=kbase
          d=output_+oid(k*4+1,h_-2,w_); _mm256_storeu_ps(d, _mm256_add_ps(y07, _mm256_loadu_ps(d))); _mm256_storeu_ps((float*)&y07,y08); _mm256_storeu_ps((float*)&y08,y09); y09 = _mm256_setzero_ps(); // k=kbase+1
          d=output_+oid(k*4+2,h_-2,w_); _mm256_storeu_ps(d, _mm256_add_ps(y10, _mm256_loadu_ps(d))); _mm256_storeu_ps((float*)&y10,y11); _mm256_storeu_ps((float*)&y11,y12); y12 = _mm256_setzero_ps(); // k=kbase+2
          d=output_+oid(k*4+3,h_-2,w_); _mm256_storeu_ps(d, _mm256_add_ps(y13, _mm256_loadu_ps(d))); _mm256_storeu_ps((float*)&y13,y14); _mm256_storeu_ps((float*)&y14,y15); y15 = _mm256_setzero_ps(); // k=kbase+3
        }
      }

      {
        const int h_=ca.H-2;
        REP(c_,cbase,cstop){
          int i_offset= input_index_nchw(c_, h_, w_);
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
          // write y04, y07, y10, y13 to output_
          //print_m256(y04);
          d=output_+oid(k*4+0,h_-2,w_); _mm256_storeu_ps(d, _mm256_add_ps(y04, _mm256_loadu_ps(d))); _mm256_storeu_ps((float*)&y04,y05); y05 = _mm256_setzero_ps();
          d=output_+oid(k*4+1,h_-2,w_); _mm256_storeu_ps(d, _mm256_add_ps(y07, _mm256_loadu_ps(d))); _mm256_storeu_ps((float*)&y07,y08); y08 = _mm256_setzero_ps();
          d=output_+oid(k*4+2,h_-2,w_); _mm256_storeu_ps(d, _mm256_add_ps(y10, _mm256_loadu_ps(d))); _mm256_storeu_ps((float*)&y10,y11); y11 = _mm256_setzero_ps();
          d=output_+oid(k*4+3,h_-2,w_); _mm256_storeu_ps(d, _mm256_add_ps(y13, _mm256_loadu_ps(d))); _mm256_storeu_ps((float*)&y13,y14); y14 = _mm256_setzero_ps();
        }
      }

      {
        const int h_=ca.H-1;
        REP(c_,cbase,cstop){
          int i_offset= input_index_nchw(c_, h_, w_);
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
          //print_m256(y04);
          d=output_+oid(k*4+0,h_-2,w_); _mm256_storeu_ps(d, _mm256_add_ps(y04, _mm256_loadu_ps(d)));
          d=output_+oid(k*4+1,h_-2,w_); _mm256_storeu_ps(d, _mm256_add_ps(y07, _mm256_loadu_ps(d)));
          d=output_+oid(k*4+2,h_-2,w_); _mm256_storeu_ps(d, _mm256_add_ps(y10, _mm256_loadu_ps(d)));
          d=output_+oid(k*4+3,h_-2,w_); _mm256_storeu_ps(d, _mm256_add_ps(y13, _mm256_loadu_ps(d)));
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


void fast_conv::run_nchw_v2(float* output_, int cbase, int cstop) {
  __m256 y00{}, y01{}, y02{}, y03{};
  __m256 y04{}, y05{}, y06{}, y07{}, y08{}, y09{}, y10{}, y11{}, y12{}, y13{}, y14{}, y15{}; // 8*12

}


void fast_conv::run_nchw2x2(float* output_, int cbase, int cstop) {
  __m256 y00{}, y01{};
  __m256 y02{}, y03{}, y04{}, y05{}, y06{}, y07{}, y08{}, y09{}, y10{}, y11{}, y12{}, y13{}, y14{}, y15{};
  float* d= nullptr;
  REP2(k,0,ca.K,7)
  {
    REP2(w_,0,ca.OW,8){
      // TC: O(9*CHANNEL_SPLIT*H=288H)
      {
        const int h_=0;
        REP(c_,cbase,cstop){
          int i_offset= input_index_nchw(c_, h_, w_);
          y00 = _mm256_loadu_ps(input + i_offset++);
          y01 = _mm256_set1_ps(kernel[kin(k+0,c_,0,0)]); y02 = _mm256_fmadd_ps(y00, y01, y02);
          y01 = _mm256_set1_ps(kernel[kin(k+1,c_,0,0)]); y04 = _mm256_fmadd_ps(y00, y01, y04);
          y01 = _mm256_set1_ps(kernel[kin(k+2,c_,0,0)]); y06 = _mm256_fmadd_ps(y00, y01, y06);
          y01 = _mm256_set1_ps(kernel[kin(k+3,c_,0,0)]); y08 = _mm256_fmadd_ps(y00, y01, y08);
          y01 = _mm256_set1_ps(kernel[kin(k+4,c_,0,0)]); y10 = _mm256_fmadd_ps(y00, y01, y10);
          y01 = _mm256_set1_ps(kernel[kin(k+5,c_,0,0)]); y12 = _mm256_fmadd_ps(y00, y01, y12);
          y01 = _mm256_set1_ps(kernel[kin(k+6,c_,0,0)]); y14 = _mm256_fmadd_ps(y00, y01, y14);

          y00 = _mm256_loadu_ps(input + i_offset++);
          y01 = _mm256_set1_ps(kernel[kin(k+0,c_,0,1)]); y02 = _mm256_fmadd_ps(y00, y01, y02);
          y01 = _mm256_set1_ps(kernel[kin(k+1,c_,0,1)]); y04 = _mm256_fmadd_ps(y00, y01, y04);
          y01 = _mm256_set1_ps(kernel[kin(k+2,c_,0,1)]); y06 = _mm256_fmadd_ps(y00, y01, y06);
          y01 = _mm256_set1_ps(kernel[kin(k+3,c_,0,1)]); y08 = _mm256_fmadd_ps(y00, y01, y08);
          y01 = _mm256_set1_ps(kernel[kin(k+4,c_,0,1)]); y10 = _mm256_fmadd_ps(y00, y01, y10);
          y01 = _mm256_set1_ps(kernel[kin(k+5,c_,0,1)]); y12 = _mm256_fmadd_ps(y00, y01, y12);
          y01 = _mm256_set1_ps(kernel[kin(k+6,c_,0,1)]); y14 = _mm256_fmadd_ps(y00, y01, y14);


        }
      }

      REP(h_,1,ca.H-1){
        REP(c_,cbase,cstop){
          int i_offset= input_index_nchw(c_, h_, w_);
          y00 = _mm256_loadu_ps(input + i_offset++);

          y01 = _mm256_set1_ps(kernel[kin(k+0,c_,0,0)]); y03 = _mm256_fmadd_ps(y00, y01, y03); y01 = _mm256_set1_ps(kernel[kin(k+0,c_,1,0)]); y02 = _mm256_fmadd_ps(y00, y01, y02);
          y01 = _mm256_set1_ps(kernel[kin(k+1,c_,0,0)]); y05 = _mm256_fmadd_ps(y00, y01, y05); y01 = _mm256_set1_ps(kernel[kin(k+1,c_,1,0)]); y04 = _mm256_fmadd_ps(y00, y01, y04);
          y01 = _mm256_set1_ps(kernel[kin(k+2,c_,0,0)]); y07 = _mm256_fmadd_ps(y00, y01, y07); y01 = _mm256_set1_ps(kernel[kin(k+2,c_,1,0)]); y06 = _mm256_fmadd_ps(y00, y01, y06);
          y01 = _mm256_set1_ps(kernel[kin(k+3,c_,0,0)]); y09 = _mm256_fmadd_ps(y00, y01, y09); y01 = _mm256_set1_ps(kernel[kin(k+3,c_,1,0)]); y08 = _mm256_fmadd_ps(y00, y01, y08);
          y01 = _mm256_set1_ps(kernel[kin(k+4,c_,0,0)]); y11 = _mm256_fmadd_ps(y00, y01, y09); y01 = _mm256_set1_ps(kernel[kin(k+4,c_,1,0)]); y10 = _mm256_fmadd_ps(y00, y01, y10);
          y01 = _mm256_set1_ps(kernel[kin(k+5,c_,0,0)]); y13 = _mm256_fmadd_ps(y00, y01, y09); y01 = _mm256_set1_ps(kernel[kin(k+5,c_,1,0)]); y12 = _mm256_fmadd_ps(y00, y01, y12);
          y01 = _mm256_set1_ps(kernel[kin(k+6,c_,0,0)]); y15 = _mm256_fmadd_ps(y00, y01, y09); y01 = _mm256_set1_ps(kernel[kin(k+6,c_,1,0)]); y14 = _mm256_fmadd_ps(y00, y01, y14);

          y00 = _mm256_loadu_ps(input + i_offset++);
          y01 = _mm256_set1_ps(kernel[kin(k+0,c_,0,1)]); y03 = _mm256_fmadd_ps(y00, y01, y03); y01 = _mm256_set1_ps(kernel[kin(k+0,c_,1,1)]); y02 = _mm256_fmadd_ps(y00, y01, y02);
          y01 = _mm256_set1_ps(kernel[kin(k+1,c_,0,1)]); y05 = _mm256_fmadd_ps(y00, y01, y05); y01 = _mm256_set1_ps(kernel[kin(k+1,c_,1,1)]); y04 = _mm256_fmadd_ps(y00, y01, y04);
          y01 = _mm256_set1_ps(kernel[kin(k+2,c_,0,1)]); y07 = _mm256_fmadd_ps(y00, y01, y07); y01 = _mm256_set1_ps(kernel[kin(k+2,c_,1,1)]); y06 = _mm256_fmadd_ps(y00, y01, y06);
          y01 = _mm256_set1_ps(kernel[kin(k+3,c_,0,1)]); y09 = _mm256_fmadd_ps(y00, y01, y09); y01 = _mm256_set1_ps(kernel[kin(k+3,c_,1,1)]); y08 = _mm256_fmadd_ps(y00, y01, y08);
          y01 = _mm256_set1_ps(kernel[kin(k+4,c_,0,1)]); y11 = _mm256_fmadd_ps(y00, y01, y11); y01 = _mm256_set1_ps(kernel[kin(k+4,c_,1,1)]); y10 = _mm256_fmadd_ps(y00, y01, y10);
          y01 = _mm256_set1_ps(kernel[kin(k+5,c_,0,1)]); y13 = _mm256_fmadd_ps(y00, y01, y13); y01 = _mm256_set1_ps(kernel[kin(k+5,c_,1,1)]); y12 = _mm256_fmadd_ps(y00, y01, y12);
          y01 = _mm256_set1_ps(kernel[kin(k+6,c_,0,1)]); y15 = _mm256_fmadd_ps(y00, y01, y15); y01 = _mm256_set1_ps(kernel[kin(k+6,c_,1,1)]); y14 = _mm256_fmadd_ps(y00, y01, y14);
        }

        // when c_=cstop-1
        {
          // write y02, y04, y06, y08...14 to output_, print_m256(y04);
          int tmp=oid(k+0,h_-1,w_);
          d=output_+tmp; _mm256_storeu_ps(d, _mm256_add_ps(y02, _mm256_loadu_ps(d))); _mm256_storeu_ps((float*)&y02,y03); y03 = _mm256_setzero_ps();
          d=output_+oid(k+1,h_-1,w_); _mm256_storeu_ps(d, _mm256_add_ps(y04, _mm256_loadu_ps(d))); _mm256_storeu_ps((float*)&y04,y05); y05 = _mm256_setzero_ps();
          d=output_+oid(k+2,h_-1,w_); _mm256_storeu_ps(d, _mm256_add_ps(y06, _mm256_loadu_ps(d))); _mm256_storeu_ps((float*)&y06,y07); y07 = _mm256_setzero_ps();
          d=output_+oid(k+3,h_-1,w_); _mm256_storeu_ps(d, _mm256_add_ps(y08, _mm256_loadu_ps(d))); _mm256_storeu_ps((float*)&y08,y09); y09 = _mm256_setzero_ps();
          d=output_+oid(k+4,h_-1,w_); _mm256_storeu_ps(d, _mm256_add_ps(y10, _mm256_loadu_ps(d))); _mm256_storeu_ps((float*)&y10,y11); y11 = _mm256_setzero_ps();
          d=output_+oid(k+5,h_-1,w_); _mm256_storeu_ps(d, _mm256_add_ps(y12, _mm256_loadu_ps(d))); _mm256_storeu_ps((float*)&y12,y13); y13 = _mm256_setzero_ps();
          d=output_+oid(k+6,h_-1,w_); _mm256_storeu_ps(d, _mm256_add_ps(y14, _mm256_loadu_ps(d))); _mm256_storeu_ps((float*)&y14,y15); y15 = _mm256_setzero_ps();
        }
      }

      {
        const int h_=ca.H-1;
        REP(c_,cbase,cstop){
          int i_offset= input_index_nchw(c_, h_, w_);
          y00 = _mm256_loadu_ps(input + i_offset++);
          y01 = _mm256_set1_ps(kernel[kin(k+0,c_,1,0)]); y02 = _mm256_fmadd_ps(y00, y01, y02);
          y01 = _mm256_set1_ps(kernel[kin(k+1,c_,1,0)]); y04 = _mm256_fmadd_ps(y00, y01, y04);
          y01 = _mm256_set1_ps(kernel[kin(k+2,c_,1,0)]); y06 = _mm256_fmadd_ps(y00, y01, y06);
          y01 = _mm256_set1_ps(kernel[kin(k+3,c_,1,0)]); y08 = _mm256_fmadd_ps(y00, y01, y08);
          y01 = _mm256_set1_ps(kernel[kin(k+4,c_,1,0)]); y10 = _mm256_fmadd_ps(y00, y01, y10);
          y01 = _mm256_set1_ps(kernel[kin(k+5,c_,1,0)]); y12 = _mm256_fmadd_ps(y00, y01, y12);
          y01 = _mm256_set1_ps(kernel[kin(k+6,c_,1,0)]); y14 = _mm256_fmadd_ps(y00, y01, y14);

          y00 = _mm256_loadu_ps(input + i_offset++);
          y01 = _mm256_set1_ps(kernel[kin(k+0,c_,1,1)]); y02 = _mm256_fmadd_ps(y00, y01, y02);
          y01 = _mm256_set1_ps(kernel[kin(k+1,c_,1,1)]); y04 = _mm256_fmadd_ps(y00, y01, y04);
          y01 = _mm256_set1_ps(kernel[kin(k+2,c_,1,1)]); y06 = _mm256_fmadd_ps(y00, y01, y06);
          y01 = _mm256_set1_ps(kernel[kin(k+3,c_,1,1)]); y08 = _mm256_fmadd_ps(y00, y01, y08);
          y01 = _mm256_set1_ps(kernel[kin(k+4,c_,1,1)]); y10 = _mm256_fmadd_ps(y00, y01, y10);
          y01 = _mm256_set1_ps(kernel[kin(k+5,c_,1,1)]); y12 = _mm256_fmadd_ps(y00, y01, y12);
          y01 = _mm256_set1_ps(kernel[kin(k+6,c_,1,1)]); y14 = _mm256_fmadd_ps(y00, y01, y14);

        }

        // when c_=cstop-1
        {
          //print_m256(y04);
          d=output_+oid(k+0,h_-1,w_); _mm256_storeu_ps(d, _mm256_add_ps(y02, _mm256_loadu_ps(d)));
          d=output_+oid(k+1,h_-1,w_); _mm256_storeu_ps(d, _mm256_add_ps(y04, _mm256_loadu_ps(d)));
          d=output_+oid(k+2,h_-1,w_); _mm256_storeu_ps(d, _mm256_add_ps(y06, _mm256_loadu_ps(d)));
          d=output_+oid(k+3,h_-1,w_); _mm256_storeu_ps(d, _mm256_add_ps(y08, _mm256_loadu_ps(d)));
          d=output_+oid(k+4,h_-1,w_); _mm256_storeu_ps(d, _mm256_add_ps(y10, _mm256_loadu_ps(d)));
          d=output_+oid(k+5,h_-1,w_); _mm256_storeu_ps(d, _mm256_add_ps(y12, _mm256_loadu_ps(d)));
          d=output_+oid(k+6,h_-1,w_); _mm256_storeu_ps(d, _mm256_add_ps(y14, _mm256_loadu_ps(d)));
          y02 = _mm256_setzero_ps();
          y04 = _mm256_setzero_ps();
          y06 = _mm256_setzero_ps();
          y08 = _mm256_setzero_ps();
          y10 = _mm256_setzero_ps();
          y12 = _mm256_setzero_ps();
          y14 = _mm256_setzero_ps();
        }
      }
    }
    //printf("write to output_\n");
  }
}


void fast_conv::run_nhcw(float* output_, int cbase, int cstop) {
      __m256 y00{}, y01{}, y02{}, y03{};
      __m256 y04{}, y05{}, y06{}, y07{}, y08{}, y09{}, y10{}, y11{}, y12{}, y13{}, y14{}, y15{}; // 8*12
      REP(k,0,slice_number_in_batch_dim){
        REP2(w_,0,ca.OW,8){
          // TC: O(9*CHANNEL_SPLIT*H=288H)
          {
        const int h_=0;
        REP(c_,cbase,cstop){
          int i_offset= input_index_nhcw(h_, c_, w_);
          y00 = _mm256_loadu_ps(input_nhcw + i_offset++);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,0,0)]); y04 = _mm256_fmadd_ps(y00, y01, y04);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,0,1)]); y07 = _mm256_fmadd_ps(y00, y01, y07);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,0,2)]); y10 = _mm256_fmadd_ps(y00, y01, y10);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,0,3)]); y13 = _mm256_fmadd_ps(y00, y01, y13);

          y00 = _mm256_loadu_ps(input_nhcw + i_offset++);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,1,0)]); y04 = _mm256_fmadd_ps(y00, y01, y04);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,1,1)]); y07 = _mm256_fmadd_ps(y00, y01, y07);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,1,2)]); y10 = _mm256_fmadd_ps(y00, y01, y10);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,1,3)]); y13 = _mm256_fmadd_ps(y00, y01, y13);

          y00 = _mm256_loadu_ps(input_nhcw + i_offset++);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,2,0)]); y04 = _mm256_fmadd_ps(y00, y01, y04);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,2,1)]); y07 = _mm256_fmadd_ps(y00, y01, y07);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,2,2)]); y10 = _mm256_fmadd_ps(y00, y01, y10);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,2,3)]); y13 = _mm256_fmadd_ps(y00, y01, y13);
        }
          }

          {
        const int h_=1;
        REP(c_,cbase,cstop){
          int i_offset= input_index_nhcw(h_, c_, w_);
          y00 = _mm256_loadu_ps(input_nhcw + i_offset++);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,0,0)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,0,0)]); y05 = _mm256_fmadd_ps(y00, y01, y05);
          y04 = _mm256_fmadd_ps(y00, y02, y04); // k=kbase
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,0,1)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,0,1)]); y08 = _mm256_fmadd_ps(y00, y01, y08); y07 = _mm256_fmadd_ps(y00, y02, y07); // k=kbase+1
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,0,2)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,0,2)]); y11 = _mm256_fmadd_ps(y00, y01, y11); y10 = _mm256_fmadd_ps(y00, y02, y10); // k=kbase+2
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,0,3)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,0,3)]); y14 = _mm256_fmadd_ps(y00, y01, y14); y13 = _mm256_fmadd_ps(y00, y02, y13); // k=kbase+3

          y00 = _mm256_loadu_ps(input_nhcw + i_offset++);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,1,0)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,1,0)]); y05 = _mm256_fmadd_ps(y00, y01, y05); y04 = _mm256_fmadd_ps(y00, y02, y04); // k=kbase
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,1,1)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,1,1)]); y08 = _mm256_fmadd_ps(y00, y01, y08); y07 = _mm256_fmadd_ps(y00, y02, y07); // k=kbase+1
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,1,2)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,1,2)]); y11 = _mm256_fmadd_ps(y00, y01, y11); y10 = _mm256_fmadd_ps(y00, y02, y10); // k=kbase+2
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,1,3)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,1,3)]); y14 = _mm256_fmadd_ps(y00, y01, y14); y13 = _mm256_fmadd_ps(y00, y02, y13); // k=kbase+3

          y00 = _mm256_loadu_ps(input_nhcw + i_offset++);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,2,0)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,2,0)]); y05 = _mm256_fmadd_ps(y00, y01, y05); y04 = _mm256_fmadd_ps(y00, y02, y04);// k=kbase
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,2,1)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,2,1)]); y08 = _mm256_fmadd_ps(y00, y01, y08); y07 = _mm256_fmadd_ps(y00, y02, y07);// k=kbase+1
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,2,2)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,2,2)]); y11 = _mm256_fmadd_ps(y00, y01, y11); y10 = _mm256_fmadd_ps(y00, y02, y10);// k=kbase+2
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,2,3)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,2,3)]); y14 = _mm256_fmadd_ps(y00, y01, y14); y13 = _mm256_fmadd_ps(y00, y02, y13);// k=kbase+3
        }
          }

          REP(h_,2,ca.H-2){
              REP(c_,cbase,cstop){
                  int i_offset= input_index_nhcw(h_, c_, w_);
          y00 = _mm256_loadu_ps(input_nhcw + i_offset++); y01 = _mm256_loadu_ps(input_nhcw + i_offset++); y02 = _mm256_loadu_ps(input_nhcw + i_offset++);

          y03 = _mm256_set1_ps(fr[kid(k,c_,0,0,0)]); y06 = _mm256_fmadd_ps(y00, y03, y06); y03 = _mm256_set1_ps(fr[kid(k,c_,1,0,0)]); y05 = _mm256_fmadd_ps(y00, y03, y05); y03 = _mm256_set1_ps(fr[kid(k,c_,2,0,0)]); y04 = _mm256_fmadd_ps(y00, y03, y04);
          y03 = _mm256_set1_ps(fr[kid(k,c_,0,0,1)]); y09 = _mm256_fmadd_ps(y00, y03, y09); y03 = _mm256_set1_ps(fr[kid(k,c_,1,0,1)]); y08 = _mm256_fmadd_ps(y00, y03, y08); y03 = _mm256_set1_ps(fr[kid(k,c_,2,0,1)]); y07 = _mm256_fmadd_ps(y00, y03, y07);
          y03 = _mm256_set1_ps(fr[kid(k,c_,0,0,2)]); y12 = _mm256_fmadd_ps(y00, y03, y12); y03 = _mm256_set1_ps(fr[kid(k,c_,1,0,2)]); y11 = _mm256_fmadd_ps(y00, y03, y11); y03 = _mm256_set1_ps(fr[kid(k,c_,2,0,2)]); y10 = _mm256_fmadd_ps(y00, y03, y10);
          y03 = _mm256_set1_ps(fr[kid(k,c_,0,0,3)]); y15 = _mm256_fmadd_ps(y00, y03, y15); y03 = _mm256_set1_ps(fr[kid(k,c_,1,0,3)]); y14 = _mm256_fmadd_ps(y00, y03, y14); y03 = _mm256_set1_ps(fr[kid(k,c_,2,0,3)]); y13 = _mm256_fmadd_ps(y00, y03, y13);

          y03 = _mm256_set1_ps(fr[kid(k,c_,0,1,0)]); y06 = _mm256_fmadd_ps(y01, y03, y06); y03 = _mm256_set1_ps(fr[kid(k,c_,1,1,0)]); y05 = _mm256_fmadd_ps(y01, y03, y05); y03 = _mm256_set1_ps(fr[kid(k,c_,2,1,0)]); y04 = _mm256_fmadd_ps(y01, y03, y04);
          y03 = _mm256_set1_ps(fr[kid(k,c_,0,1,1)]); y09 = _mm256_fmadd_ps(y01, y03, y09); y03 = _mm256_set1_ps(fr[kid(k,c_,1,1,1)]); y08 = _mm256_fmadd_ps(y01, y03, y08); y03 = _mm256_set1_ps(fr[kid(k,c_,2,1,1)]); y07 = _mm256_fmadd_ps(y01, y03, y07);
          y03 = _mm256_set1_ps(fr[kid(k,c_,0,1,2)]); y12 = _mm256_fmadd_ps(y01, y03, y12); y03 = _mm256_set1_ps(fr[kid(k,c_,1,1,2)]); y11 = _mm256_fmadd_ps(y01, y03, y11); y03 = _mm256_set1_ps(fr[kid(k,c_,2,1,2)]); y10 = _mm256_fmadd_ps(y01, y03, y10);
          y03 = _mm256_set1_ps(fr[kid(k,c_,0,1,3)]); y15 = _mm256_fmadd_ps(y01, y03, y15); y03 = _mm256_set1_ps(fr[kid(k,c_,1,1,3)]); y14 = _mm256_fmadd_ps(y01, y03, y14); y03 = _mm256_set1_ps(fr[kid(k,c_,2,1,3)]); y13 = _mm256_fmadd_ps(y01, y03, y13);

          y03 = _mm256_set1_ps(fr[kid(k,c_,0,2,0)]); y06 = _mm256_fmadd_ps(y02, y03, y06); y03 = _mm256_set1_ps(fr[kid(k,c_,1,2,0)]); y05 = _mm256_fmadd_ps(y02, y03, y05); y03 = _mm256_set1_ps(fr[kid(k,c_,2,2,0)]); y04 = _mm256_fmadd_ps(y02, y03, y04);
          y03 = _mm256_set1_ps(fr[kid(k,c_,0,2,1)]); y09 = _mm256_fmadd_ps(y02, y03, y09); y03 = _mm256_set1_ps(fr[kid(k,c_,1,2,1)]); y08 = _mm256_fmadd_ps(y02, y03, y08); y03 = _mm256_set1_ps(fr[kid(k,c_,2,2,1)]); y07 = _mm256_fmadd_ps(y02, y03, y07);
          y03 = _mm256_set1_ps(fr[kid(k,c_,0,2,2)]); y12 = _mm256_fmadd_ps(y02, y03, y12); y03 = _mm256_set1_ps(fr[kid(k,c_,1,2,2)]); y11 = _mm256_fmadd_ps(y02, y03, y11); y03 = _mm256_set1_ps(fr[kid(k,c_,2,2,2)]); y10 = _mm256_fmadd_ps(y02, y03, y10);
          y03 = _mm256_set1_ps(fr[kid(k,c_,0,2,3)]); y15 = _mm256_fmadd_ps(y02, y03, y15); y03 = _mm256_set1_ps(fr[kid(k,c_,1,2,3)]); y14 = _mm256_fmadd_ps(y02, y03, y14); y03 = _mm256_set1_ps(fr[kid(k,c_,2,2,3)]); y13 = _mm256_fmadd_ps(y02, y03, y13);
        }

        // when c_=cstop-1
        {
          // write y04, y07, y10, y13 to output_, print_m256(y04);
          _mm256_storeu_ps(output_+oid(k*4+0,h_-2,w_), y04); _mm256_storeu_ps((float*)&y04,y05); _mm256_storeu_ps((float*)&y05,y06); y06 = _mm256_setzero_ps(); // k=kbase
          _mm256_storeu_ps(output_+oid(k*4+1,h_-2,w_), y07); _mm256_storeu_ps((float*)&y07,y08); _mm256_storeu_ps((float*)&y08,y09); y09 = _mm256_setzero_ps(); // k=kbase+1
          _mm256_storeu_ps(output_+oid(k*4+2,h_-2,w_), y10); _mm256_storeu_ps((float*)&y10,y11); _mm256_storeu_ps((float*)&y11,y12); y12 = _mm256_setzero_ps(); // k=kbase+2
          _mm256_storeu_ps(output_+oid(k*4+3,h_-2,w_), y13); _mm256_storeu_ps((float*)&y13,y14); _mm256_storeu_ps((float*)&y14,y15); y15 = _mm256_setzero_ps(); // k=kbase+3
        }
      }

      {
        const int h_=ca.H-2;
        REP(c_,cbase,cstop){
          int i_offset= input_index_nhcw(h_, c_, w_);
          y00 = _mm256_loadu_ps(input_nhcw + i_offset++);
          y02 = _mm256_set1_ps(fr[kid(k,c_,1,0,0)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,0,0)]); y05 = _mm256_fmadd_ps(y00, y02, y05); y04 = _mm256_fmadd_ps(y00, y03, y04);
          y02 = _mm256_set1_ps(fr[kid(k,c_,1,0,1)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,0,1)]); y08 = _mm256_fmadd_ps(y00, y02, y08); y07 = _mm256_fmadd_ps(y00, y03, y07);
          y02 = _mm256_set1_ps(fr[kid(k,c_,1,0,2)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,0,2)]); y11 = _mm256_fmadd_ps(y00, y02, y11); y10 = _mm256_fmadd_ps(y00, y03, y10);
          y02 = _mm256_set1_ps(fr[kid(k,c_,1,0,3)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,0,3)]); y14 = _mm256_fmadd_ps(y00, y02, y14); y13 = _mm256_fmadd_ps(y00, y03, y13);

          y00 = _mm256_loadu_ps(input_nhcw + i_offset++);
          y02 = _mm256_set1_ps(fr[kid(k,c_,1,1,0)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,1,0)]); y05 = _mm256_fmadd_ps(y00, y02, y05); y04 = _mm256_fmadd_ps(y00, y03, y04);
          y02 = _mm256_set1_ps(fr[kid(k,c_,1,1,1)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,1,1)]); y08 = _mm256_fmadd_ps(y00, y02, y08); y07 = _mm256_fmadd_ps(y00, y03, y07);
          y02 = _mm256_set1_ps(fr[kid(k,c_,1,1,2)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,1,2)]); y11 = _mm256_fmadd_ps(y00, y02, y11); y10 = _mm256_fmadd_ps(y00, y03, y10);
          y02 = _mm256_set1_ps(fr[kid(k,c_,1,1,3)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,1,3)]); y14 = _mm256_fmadd_ps(y00, y02, y14); y13 = _mm256_fmadd_ps(y00, y03, y13);

          y00 = _mm256_loadu_ps(input_nhcw + i_offset++);
          y02 = _mm256_set1_ps(fr[kid(k,c_,1,2,0)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,2,0)]); y05 = _mm256_fmadd_ps(y00, y02, y05); y04 = _mm256_fmadd_ps(y00, y03, y04);
          y02 = _mm256_set1_ps(fr[kid(k,c_,1,2,1)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,2,1)]); y08 = _mm256_fmadd_ps(y00, y02, y08); y07 = _mm256_fmadd_ps(y00, y03, y07);
          y02 = _mm256_set1_ps(fr[kid(k,c_,1,2,2)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,2,2)]); y11 = _mm256_fmadd_ps(y00, y02, y11); y10 = _mm256_fmadd_ps(y00, y03, y10);
          y02 = _mm256_set1_ps(fr[kid(k,c_,1,2,3)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,2,3)]); y14 = _mm256_fmadd_ps(y00, y02, y14); y13 = _mm256_fmadd_ps(y00, y03, y13);
        }

        // when c_=cstop-1
        {
          // write y04, y07, y10, y13 to output_
          //print_m256(y04);
          _mm256_storeu_ps(output_+oid(k*4+0,h_-2,w_), y04); _mm256_storeu_ps((float*)&y04,y05); y05 = _mm256_setzero_ps();
          _mm256_storeu_ps(output_+oid(k*4+1,h_-2,w_), y07); _mm256_storeu_ps((float*)&y07,y08); y08 = _mm256_setzero_ps();
          _mm256_storeu_ps(output_+oid(k*4+2,h_-2,w_), y10); _mm256_storeu_ps((float*)&y10,y11); y11 = _mm256_setzero_ps();
          _mm256_storeu_ps(output_+oid(k*4+3,h_-2,w_), y13); _mm256_storeu_ps((float*)&y13,y14); y14 = _mm256_setzero_ps();
        }
      }

      {
        const int h_=ca.H-1;
        REP(c_,cbase,cstop){
          int i_offset= input_index_nhcw(h_, c_, w_);
          y00 = _mm256_loadu_ps(input_nhcw + i_offset++);
          y03 = _mm256_set1_ps(fr[kid(k,c_,2,0,0)]); y04 = _mm256_fmadd_ps(y00, y03, y04);
          y03 = _mm256_set1_ps(fr[kid(k,c_,2,0,1)]); y07 = _mm256_fmadd_ps(y00, y03, y07);
          y03 = _mm256_set1_ps(fr[kid(k,c_,2,0,2)]); y10 = _mm256_fmadd_ps(y00, y03, y10);
          y03 = _mm256_set1_ps(fr[kid(k,c_,2,0,3)]); y13 = _mm256_fmadd_ps(y00, y03, y13);

          y00 = _mm256_loadu_ps(input_nhcw + i_offset++);
          y03 = _mm256_set1_ps(fr[kid(k,c_,2,1,0)]); y04 = _mm256_fmadd_ps(y00, y03, y04);
          y03 = _mm256_set1_ps(fr[kid(k,c_,2,1,1)]); y07 = _mm256_fmadd_ps(y00, y03, y07);
          y03 = _mm256_set1_ps(fr[kid(k,c_,2,1,2)]); y10 = _mm256_fmadd_ps(y00, y03, y10);
          y03 = _mm256_set1_ps(fr[kid(k,c_,2,1,3)]); y13 = _mm256_fmadd_ps(y00, y03, y13);

          y00 = _mm256_loadu_ps(input_nhcw + i_offset++);
          y03 = _mm256_set1_ps(fr[kid(k,c_,2,2,0)]); y04 = _mm256_fmadd_ps(y00, y03, y04);
          y03 = _mm256_set1_ps(fr[kid(k,c_,2,2,1)]); y07 = _mm256_fmadd_ps(y00, y03, y07);
          y03 = _mm256_set1_ps(fr[kid(k,c_,2,2,2)]); y10 = _mm256_fmadd_ps(y00, y03, y10);
          y03 = _mm256_set1_ps(fr[kid(k,c_,2,2,3)]); y13 = _mm256_fmadd_ps(y00, y03, y13);
        }

        // when c_=cstop-1
        {
          //print_m256(y04);
          _mm256_storeu_ps(output_+oid(k*4+0,h_-2,w_), y04);
          _mm256_storeu_ps(output_+oid(k*4+1,h_-2,w_), y07);
          _mm256_storeu_ps(output_+oid(k*4+2,h_-2,w_), y10);
          _mm256_storeu_ps(output_+oid(k*4+3,h_-2,w_), y13);
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

void fast_conv::run_nchc8w(float* output_, int cbase, int cstop){
      __m256 y00{}, y01{}, y02{}, y03{};
      __m256 y04{}, y05{}, y06{}, y07{}, y08{}, y09{}, y10{}, y11{}, y12{}, y13{}, y14{}, y15{}; // 8*12
      REP(k,0,slice_number_in_batch_dim){
        REP2(w_,0,ca.OW,8){
          // TC: O(9*CHANNEL_SPLIT*H=288H)
          {
        const int h_=0;
        REP(c_,cbase,cstop){
          int i_offset= input_index_nchc8w(c_/8, h_, c_%8, w_);
          y00 = _mm256_loadu_ps(input_nchc8w + i_offset++);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,0,0)]); y04 = _mm256_fmadd_ps(y00, y01, y04);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,0,1)]); y07 = _mm256_fmadd_ps(y00, y01, y07);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,0,2)]); y10 = _mm256_fmadd_ps(y00, y01, y10);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,0,3)]); y13 = _mm256_fmadd_ps(y00, y01, y13);

          y00 = _mm256_loadu_ps(input_nchc8w + i_offset++);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,1,0)]); y04 = _mm256_fmadd_ps(y00, y01, y04);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,1,1)]); y07 = _mm256_fmadd_ps(y00, y01, y07);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,1,2)]); y10 = _mm256_fmadd_ps(y00, y01, y10);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,1,3)]); y13 = _mm256_fmadd_ps(y00, y01, y13);

          y00 = _mm256_loadu_ps(input_nchc8w + i_offset++);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,2,0)]); y04 = _mm256_fmadd_ps(y00, y01, y04);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,2,1)]); y07 = _mm256_fmadd_ps(y00, y01, y07);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,2,2)]); y10 = _mm256_fmadd_ps(y00, y01, y10);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,2,3)]); y13 = _mm256_fmadd_ps(y00, y01, y13);
        }
          }

          {
        const int h_=1;
        REP(c_,cbase,cstop){
          int i_offset= input_index_nchc8w(c_/8, h_, c_%8, w_);
          y00 = _mm256_loadu_ps(input_nchc8w + i_offset++);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,0,0)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,0,0)]); y05 = _mm256_fmadd_ps(y00, y01, y05);
          y04 = _mm256_fmadd_ps(y00, y02, y04); // k=kbase
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,0,1)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,0,1)]); y08 = _mm256_fmadd_ps(y00, y01, y08); y07 = _mm256_fmadd_ps(y00, y02, y07); // k=kbase+1
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,0,2)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,0,2)]); y11 = _mm256_fmadd_ps(y00, y01, y11); y10 = _mm256_fmadd_ps(y00, y02, y10); // k=kbase+2
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,0,3)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,0,3)]); y14 = _mm256_fmadd_ps(y00, y01, y14); y13 = _mm256_fmadd_ps(y00, y02, y13); // k=kbase+3

          y00 = _mm256_loadu_ps(input_nchc8w + i_offset++);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,1,0)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,1,0)]); y05 = _mm256_fmadd_ps(y00, y01, y05); y04 = _mm256_fmadd_ps(y00, y02, y04); // k=kbase
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,1,1)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,1,1)]); y08 = _mm256_fmadd_ps(y00, y01, y08); y07 = _mm256_fmadd_ps(y00, y02, y07); // k=kbase+1
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,1,2)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,1,2)]); y11 = _mm256_fmadd_ps(y00, y01, y11); y10 = _mm256_fmadd_ps(y00, y02, y10); // k=kbase+2
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,1,3)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,1,3)]); y14 = _mm256_fmadd_ps(y00, y01, y14); y13 = _mm256_fmadd_ps(y00, y02, y13); // k=kbase+3

          y00 = _mm256_loadu_ps(input_nchc8w + i_offset++);
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,2,0)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,2,0)]); y05 = _mm256_fmadd_ps(y00, y01, y05); y04 = _mm256_fmadd_ps(y00, y02, y04);// k=kbase
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,2,1)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,2,1)]); y08 = _mm256_fmadd_ps(y00, y01, y08); y07 = _mm256_fmadd_ps(y00, y02, y07);// k=kbase+1
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,2,2)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,2,2)]); y11 = _mm256_fmadd_ps(y00, y01, y11); y10 = _mm256_fmadd_ps(y00, y02, y10);// k=kbase+2
          y01 = _mm256_set1_ps(fr[kid(k,c_,0,2,3)]); y02 = _mm256_set1_ps(fr[kid(k,c_,1,2,3)]); y14 = _mm256_fmadd_ps(y00, y01, y14); y13 = _mm256_fmadd_ps(y00, y02, y13);// k=kbase+3
        }
          }

          REP(h_,2,ca.H-2){
              REP(c_,cbase,cstop){
                  int i_offset= input_index_nchc8w(c_/8, h_, c_%8, w_);
          y00 = _mm256_loadu_ps(input_nchc8w + i_offset++); y01 = _mm256_loadu_ps(input_nchc8w + i_offset++); y02 = _mm256_loadu_ps(input_nchc8w + i_offset++);

          y03 = _mm256_set1_ps(fr[kid(k,c_,0,0,0)]); y06 = _mm256_fmadd_ps(y00, y03, y06); y03 = _mm256_set1_ps(fr[kid(k,c_,1,0,0)]); y05 = _mm256_fmadd_ps(y00, y03, y05); y03 = _mm256_set1_ps(fr[kid(k,c_,2,0,0)]); y04 = _mm256_fmadd_ps(y00, y03, y04);
          y03 = _mm256_set1_ps(fr[kid(k,c_,0,0,1)]); y09 = _mm256_fmadd_ps(y00, y03, y09); y03 = _mm256_set1_ps(fr[kid(k,c_,1,0,1)]); y08 = _mm256_fmadd_ps(y00, y03, y08); y03 = _mm256_set1_ps(fr[kid(k,c_,2,0,1)]); y07 = _mm256_fmadd_ps(y00, y03, y07);
          y03 = _mm256_set1_ps(fr[kid(k,c_,0,0,2)]); y12 = _mm256_fmadd_ps(y00, y03, y12); y03 = _mm256_set1_ps(fr[kid(k,c_,1,0,2)]); y11 = _mm256_fmadd_ps(y00, y03, y11); y03 = _mm256_set1_ps(fr[kid(k,c_,2,0,2)]); y10 = _mm256_fmadd_ps(y00, y03, y10);
          y03 = _mm256_set1_ps(fr[kid(k,c_,0,0,3)]); y15 = _mm256_fmadd_ps(y00, y03, y15); y03 = _mm256_set1_ps(fr[kid(k,c_,1,0,3)]); y14 = _mm256_fmadd_ps(y00, y03, y14); y03 = _mm256_set1_ps(fr[kid(k,c_,2,0,3)]); y13 = _mm256_fmadd_ps(y00, y03, y13);

          y03 = _mm256_set1_ps(fr[kid(k,c_,0,1,0)]); y06 = _mm256_fmadd_ps(y01, y03, y06); y03 = _mm256_set1_ps(fr[kid(k,c_,1,1,0)]); y05 = _mm256_fmadd_ps(y01, y03, y05); y03 = _mm256_set1_ps(fr[kid(k,c_,2,1,0)]); y04 = _mm256_fmadd_ps(y01, y03, y04);
          y03 = _mm256_set1_ps(fr[kid(k,c_,0,1,1)]); y09 = _mm256_fmadd_ps(y01, y03, y09); y03 = _mm256_set1_ps(fr[kid(k,c_,1,1,1)]); y08 = _mm256_fmadd_ps(y01, y03, y08); y03 = _mm256_set1_ps(fr[kid(k,c_,2,1,1)]); y07 = _mm256_fmadd_ps(y01, y03, y07);
          y03 = _mm256_set1_ps(fr[kid(k,c_,0,1,2)]); y12 = _mm256_fmadd_ps(y01, y03, y12); y03 = _mm256_set1_ps(fr[kid(k,c_,1,1,2)]); y11 = _mm256_fmadd_ps(y01, y03, y11); y03 = _mm256_set1_ps(fr[kid(k,c_,2,1,2)]); y10 = _mm256_fmadd_ps(y01, y03, y10);
          y03 = _mm256_set1_ps(fr[kid(k,c_,0,1,3)]); y15 = _mm256_fmadd_ps(y01, y03, y15); y03 = _mm256_set1_ps(fr[kid(k,c_,1,1,3)]); y14 = _mm256_fmadd_ps(y01, y03, y14); y03 = _mm256_set1_ps(fr[kid(k,c_,2,1,3)]); y13 = _mm256_fmadd_ps(y01, y03, y13);

          y03 = _mm256_set1_ps(fr[kid(k,c_,0,2,0)]); y06 = _mm256_fmadd_ps(y02, y03, y06); y03 = _mm256_set1_ps(fr[kid(k,c_,1,2,0)]); y05 = _mm256_fmadd_ps(y02, y03, y05); y03 = _mm256_set1_ps(fr[kid(k,c_,2,2,0)]); y04 = _mm256_fmadd_ps(y02, y03, y04);
          y03 = _mm256_set1_ps(fr[kid(k,c_,0,2,1)]); y09 = _mm256_fmadd_ps(y02, y03, y09); y03 = _mm256_set1_ps(fr[kid(k,c_,1,2,1)]); y08 = _mm256_fmadd_ps(y02, y03, y08); y03 = _mm256_set1_ps(fr[kid(k,c_,2,2,1)]); y07 = _mm256_fmadd_ps(y02, y03, y07);
          y03 = _mm256_set1_ps(fr[kid(k,c_,0,2,2)]); y12 = _mm256_fmadd_ps(y02, y03, y12); y03 = _mm256_set1_ps(fr[kid(k,c_,1,2,2)]); y11 = _mm256_fmadd_ps(y02, y03, y11); y03 = _mm256_set1_ps(fr[kid(k,c_,2,2,2)]); y10 = _mm256_fmadd_ps(y02, y03, y10);
          y03 = _mm256_set1_ps(fr[kid(k,c_,0,2,3)]); y15 = _mm256_fmadd_ps(y02, y03, y15); y03 = _mm256_set1_ps(fr[kid(k,c_,1,2,3)]); y14 = _mm256_fmadd_ps(y02, y03, y14); y03 = _mm256_set1_ps(fr[kid(k,c_,2,2,3)]); y13 = _mm256_fmadd_ps(y02, y03, y13);
        }

        // when c_=cstop-1
        {
          // write y04, y07, y10, y13 to output_, print_m256(y04);
          _mm256_storeu_ps(output_+oid(k*4+0,h_-2,w_), y04); _mm256_storeu_ps((float*)&y04,y05); _mm256_storeu_ps((float*)&y05,y06); y06 = _mm256_setzero_ps(); // k=kbase
          _mm256_storeu_ps(output_+oid(k*4+1,h_-2,w_), y07); _mm256_storeu_ps((float*)&y07,y08); _mm256_storeu_ps((float*)&y08,y09); y09 = _mm256_setzero_ps(); // k=kbase+1
          _mm256_storeu_ps(output_+oid(k*4+2,h_-2,w_), y10); _mm256_storeu_ps((float*)&y10,y11); _mm256_storeu_ps((float*)&y11,y12); y12 = _mm256_setzero_ps(); // k=kbase+2
          _mm256_storeu_ps(output_+oid(k*4+3,h_-2,w_), y13); _mm256_storeu_ps((float*)&y13,y14); _mm256_storeu_ps((float*)&y14,y15); y15 = _mm256_setzero_ps(); // k=kbase+3
        }
      }

      {
        const int h_=ca.H-2;
        REP(c_,cbase,cstop){
          int i_offset= input_index_nchc8w(c_/8, h_, c_%8, w_);
          y00 = _mm256_loadu_ps(input_nchc8w + i_offset++);
          y02 = _mm256_set1_ps(fr[kid(k,c_,1,0,0)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,0,0)]); y05 = _mm256_fmadd_ps(y00, y02, y05); y04 = _mm256_fmadd_ps(y00, y03, y04);
          y02 = _mm256_set1_ps(fr[kid(k,c_,1,0,1)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,0,1)]); y08 = _mm256_fmadd_ps(y00, y02, y08); y07 = _mm256_fmadd_ps(y00, y03, y07);
          y02 = _mm256_set1_ps(fr[kid(k,c_,1,0,2)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,0,2)]); y11 = _mm256_fmadd_ps(y00, y02, y11); y10 = _mm256_fmadd_ps(y00, y03, y10);
          y02 = _mm256_set1_ps(fr[kid(k,c_,1,0,3)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,0,3)]); y14 = _mm256_fmadd_ps(y00, y02, y14); y13 = _mm256_fmadd_ps(y00, y03, y13);

          y00 = _mm256_loadu_ps(input_nchc8w + i_offset++);
          y02 = _mm256_set1_ps(fr[kid(k,c_,1,1,0)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,1,0)]); y05 = _mm256_fmadd_ps(y00, y02, y05); y04 = _mm256_fmadd_ps(y00, y03, y04);
          y02 = _mm256_set1_ps(fr[kid(k,c_,1,1,1)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,1,1)]); y08 = _mm256_fmadd_ps(y00, y02, y08); y07 = _mm256_fmadd_ps(y00, y03, y07);
          y02 = _mm256_set1_ps(fr[kid(k,c_,1,1,2)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,1,2)]); y11 = _mm256_fmadd_ps(y00, y02, y11); y10 = _mm256_fmadd_ps(y00, y03, y10);
          y02 = _mm256_set1_ps(fr[kid(k,c_,1,1,3)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,1,3)]); y14 = _mm256_fmadd_ps(y00, y02, y14); y13 = _mm256_fmadd_ps(y00, y03, y13);

          y00 = _mm256_loadu_ps(input_nchc8w + i_offset++);
          y02 = _mm256_set1_ps(fr[kid(k,c_,1,2,0)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,2,0)]); y05 = _mm256_fmadd_ps(y00, y02, y05); y04 = _mm256_fmadd_ps(y00, y03, y04);
          y02 = _mm256_set1_ps(fr[kid(k,c_,1,2,1)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,2,1)]); y08 = _mm256_fmadd_ps(y00, y02, y08); y07 = _mm256_fmadd_ps(y00, y03, y07);
          y02 = _mm256_set1_ps(fr[kid(k,c_,1,2,2)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,2,2)]); y11 = _mm256_fmadd_ps(y00, y02, y11); y10 = _mm256_fmadd_ps(y00, y03, y10);
          y02 = _mm256_set1_ps(fr[kid(k,c_,1,2,3)]); y03 = _mm256_set1_ps(fr[kid(k,c_,2,2,3)]); y14 = _mm256_fmadd_ps(y00, y02, y14); y13 = _mm256_fmadd_ps(y00, y03, y13);
        }

        // when c_=cstop-1
        {
          // write y04, y07, y10, y13 to output_
          //print_m256(y04);
          _mm256_storeu_ps(output_+oid(k*4+0,h_-2,w_), y04); _mm256_storeu_ps((float*)&y04,y05); y05 = _mm256_setzero_ps();
          _mm256_storeu_ps(output_+oid(k*4+1,h_-2,w_), y07); _mm256_storeu_ps((float*)&y07,y08); y08 = _mm256_setzero_ps();
          _mm256_storeu_ps(output_+oid(k*4+2,h_-2,w_), y10); _mm256_storeu_ps((float*)&y10,y11); y11 = _mm256_setzero_ps();
          _mm256_storeu_ps(output_+oid(k*4+3,h_-2,w_), y13); _mm256_storeu_ps((float*)&y13,y14); y14 = _mm256_setzero_ps();
        }
      }

      {
        const int h_=ca.H-1;
        REP(c_,cbase,cstop){
          int i_offset= input_index_nchc8w(c_/8, h_, c_%8, w_);
          y00 = _mm256_loadu_ps(input_nchc8w + i_offset++);
          y03 = _mm256_set1_ps(fr[kid(k,c_,2,0,0)]); y04 = _mm256_fmadd_ps(y00, y03, y04);
          y03 = _mm256_set1_ps(fr[kid(k,c_,2,0,1)]); y07 = _mm256_fmadd_ps(y00, y03, y07);
          y03 = _mm256_set1_ps(fr[kid(k,c_,2,0,2)]); y10 = _mm256_fmadd_ps(y00, y03, y10);
          y03 = _mm256_set1_ps(fr[kid(k,c_,2,0,3)]); y13 = _mm256_fmadd_ps(y00, y03, y13);

          y00 = _mm256_loadu_ps(input_nchc8w + i_offset++);
          y03 = _mm256_set1_ps(fr[kid(k,c_,2,1,0)]); y04 = _mm256_fmadd_ps(y00, y03, y04);
          y03 = _mm256_set1_ps(fr[kid(k,c_,2,1,1)]); y07 = _mm256_fmadd_ps(y00, y03, y07);
          y03 = _mm256_set1_ps(fr[kid(k,c_,2,1,2)]); y10 = _mm256_fmadd_ps(y00, y03, y10);
          y03 = _mm256_set1_ps(fr[kid(k,c_,2,1,3)]); y13 = _mm256_fmadd_ps(y00, y03, y13);

          y00 = _mm256_loadu_ps(input_nchc8w + i_offset++);
          y03 = _mm256_set1_ps(fr[kid(k,c_,2,2,0)]); y04 = _mm256_fmadd_ps(y00, y03, y04);
          y03 = _mm256_set1_ps(fr[kid(k,c_,2,2,1)]); y07 = _mm256_fmadd_ps(y00, y03, y07);
          y03 = _mm256_set1_ps(fr[kid(k,c_,2,2,2)]); y10 = _mm256_fmadd_ps(y00, y03, y10);
          y03 = _mm256_set1_ps(fr[kid(k,c_,2,2,3)]); y13 = _mm256_fmadd_ps(y00, y03, y13);
        }

        // when c_=cstop-1
        {
          //print_m256(y04);
          _mm256_storeu_ps(output_+oid(k*4+0,h_-2,w_), y04);
          _mm256_storeu_ps(output_+oid(k*4+1,h_-2,w_), y07);
          _mm256_storeu_ps(output_+oid(k*4+2,h_-2,w_), y10);
          _mm256_storeu_ps(output_+oid(k*4+3,h_-2,w_), y13);
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


void fast_conv::run_naive(){
  REP(k,0,ca.K){
    REP(h,0,ca.OH){
      REP(w,0,ca.OW){
        int i=oid(k,h,w);
        REP(c,0,ca.C){
          REP(r,0,ca.R){
            REP(l,0,ca.L){
              output[i] += input[input_index(c,h+r,w+l)] * kernel[kin(k,c,r,l)];
            }
          }
        }
      }
    }
  }
}
