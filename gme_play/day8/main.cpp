#include <bits/stdc++.h>
#include "immintrin.h"
#include "fmaintrin.h"
#define ORTCONV
#ifdef ORTCONV
#include "ort_conv.h"
#endif

using namespace std;

/*
 * Based on day6 example, increase the input channel number to 16.
 * */

void get_input(vector<float>& l, int n, int c, int w, int h) {
  if ((int)l.size() < n * c * w * h) l.resize(n * c * w * h);
  float start = 1.;
  for (int i = 0; i < n; i++)
    for (int j = 0; j < c; j++) {
      //start = start/(abs(int(start))+1) + ((double) rand() / (RAND_MAX)) + 0.1; if(rand()&1) start*=-1;
      start = 1;
      for (int m = 0; m < h; m++)
        for (int k = 0; k < w; k++) {
          int idx = k + m * w + h * w * j + h * w * c * i;
          //if(rand()&1) start*=-1;
          //l[idx] = start, start += 0.1; //1.0;//
          l[idx] = start, start += 1;
        }
    }
}

long long extract_time=0;

void extract(const vector<float>& v, float* D, int offset, int H, int W, pair<int, int> hw, int channel) {
  auto t0 = std::chrono::high_resolution_clock::now();
  //if ((int)D.size() < hw.first * hw.second) D.resize(H * hw.second);
  int i = 0;
  for (int x = 0; x < H; x++) {
    auto src = v.data() + offset + x * W + channel * H * W;
    ::memcpy(D + i, src, hw.second * sizeof(float));
    i += hw.second;
  }
  auto t1 = std::chrono::high_resolution_clock::now();
  extract_time += std::chrono::duration_cast<std::chrono::microseconds>((t1 - t0)).count();
}


/*
> g++ -O3 main.cpp -std=c++11 -mavx2
> ./a.out
48 | Compute Time: 1000 ns
 * */
//__attribute__((__always_inline__))
long long gme_conv(vector<float>& I,
                          vector<float>& F,
                          float* sliced_mat,
                          float* Output,
                          int Kh,
                          int Kw,
                          int Ci,
                          int Co, int input_h, int input_w, int output_h, int output_w) { // O(Ci*3*3*Co*(Oh*Ow))
  auto start = std::chrono::high_resolution_clock::now();
  for (int channel = 0; channel < Ci; channel++) {
    for (int i = 0; i < Kw; i++) {
      // auto t0 = std::chrono::high_resolution_clock::now();
      extract(I, sliced_mat, i, input_h, input_w, {output_h, output_w}, channel);  //////
      // t1 = std::chrono::high_resolution_clock::now();
      // std::cout << __LINE__ << " | Compute Time: " << std::chrono::duration_cast< std::chrono::microseconds >((t1 - t0)).count() << " us" << std::endl; t0=t1;

      int64_t area = output_h*output_w;
      if (i==0 and channel==0)
        printf("area:%ld,output_h:%d,output_w:%d,size:%ld Byte\n", area, output_h, output_w, (area*sizeof(float)));

      for (int k = 0; k < Kh; k++) { // w1, w4, w7
        auto shifted_start = sliced_mat + output_w * k; // sliding down
        for (int64_t m = 0; m < Co; m++) {
          int64_t idx = k * Kw + i + channel * Kw * Kh + m*Kw*Kh*Ci;
          //::printf("%d\n", idx);
          float scalar = F[idx];
#if 0
          int t=0;
          float* tmp=Output+m*area;
          while(area>=t+8){
            __m256 x = _mm256_loadu_ps(shifted_start+t);
            __m256 res = _mm256_loadu_ps(tmp);
            __m256 sc = _mm256_set1_ps(scalar);
            auto y = _mm256_fmadd_ps(x, sc, res);
            _mm256_storeu_ps(tmp, y);
            t+=8, tmp+=8;
          }
          for (; t < area; t++) {
            auto z = *(shifted_start + t);
            idx = t+m*area;
            Output[idx] += scalar * z;
          }
#else
          // scalar * (shifted_start: shifted_start+16)
          for (int t = 0; t < area; t++) {
            auto z = *(shifted_start + t);
            idx = t+m*area;
            Output[idx] += scalar * z;
          }
#endif
        }
      }
    }
  }

  auto t1 = std::chrono::high_resolution_clock::now();
  long long t=std::chrono::duration_cast<std::chrono::microseconds>((t1 - start)).count();
  std::cout << __FUNCTION__ << " | Compute Time: " << t << " us" << std::endl;
  return t;
}

void print_output(float* Output, int h, int w, int output_channel) {
  for (int t = 0; t < min(32, output_channel*w*h); t++) {
    printf("%.2f, ", Output[t]);
    if (t % 32 == 31) printf("\n");
  }
  //printf("\n");
}


long long run(int input_height, int input_width, int input_channel, int filter_batch, int kernel_width=3, int kernel_height=3){
  const int output_height =input_height-kernel_height+1, output_width=input_width-kernel_width+1;
  float* O = new float[output_height*output_width*filter_batch]();

  vector<float> I, F;
  get_input(I, 1, input_channel, input_width, input_height);
  get_input(F, filter_batch, input_channel, kernel_width, kernel_height);
  printf("filter size: %d\n",filter_batch* input_channel* kernel_width* kernel_height);
  printf("input total size: %.2fKB\n",1* input_channel* input_width* input_height/(1024.));

#ifdef ORTCONV
  onnxruntime_conv_nchwc(1, 1, input_channel, input_height, input_width,
                         filter_batch, kernel_height, kernel_width,
                         0, 0, 0, 0,
                         1, 1,
                         1, 1,
                         output_height, output_width,
                         I.data(),
                         F.data(),
                         nullptr,
                         O);
  print_output(O, output_height, output_width, filter_batch);
  ::memset(O, 0, output_height * output_width * sizeof(float)*filter_batch);

  onnxruntime_conv_nchw(1,1,input_channel, input_height, input_width,
                        filter_batch, kernel_height, kernel_width,
                        0, 0, 0, 0,
                        1, 1,
                        1, 1,
                        output_height, output_width,I.data(), F.data(), nullptr, O);
  print_output(O, output_height, output_width, filter_batch);
  ::memset(O, 0, output_height * output_width * sizeof(float)*filter_batch);
#endif
  //vector<float> sliced_mat(input_height * output_width, 0);
  float* sliced_mat = (float*) _mm_malloc(sizeof(float) * input_height * output_width, 32);
  auto t = gme_conv(I, F, sliced_mat, O, kernel_height, kernel_width, input_channel, filter_batch,
                    input_height, input_width,output_height, output_width);
  print_output(O, output_height, output_width, filter_batch);
  _mm_free(sliced_mat);

  delete[] O;
  printf("extract time: %lld us\n", extract_time);
  extract_time = 0;
  return t;
}

int main(int argc, char** argv) {
  printf("argc: %d\n", argc);
  long l1_cache_size = sysconf(_SC_LEVEL1_DCACHE_SIZE);
  std::cout << "L1 data cache size: " << l1_cache_size << " bytes" << std::endl;
  long l1_cache_line = sysconf(_SC_LEVEL1_DCACHE_LINESIZE);
  std::cout << "L1 data cache line: " << l1_cache_line << " bytes" << std::endl;
  long l2_cache_size = sysconf(_SC_LEVEL2_CACHE_SIZE);
  std::cout << "L2 data cache size: " << l2_cache_size << " bytes" << std::endl;
  long l3_cache_size = sysconf(_SC_LEVEL3_CACHE_SIZE);
  std::cout << "L3 data cache size: " << l3_cache_size << " bytes" << std::endl;

  int input_width=6, input_height=1200, input_channel=8, filter_batch = 1, kernel_width=3, kernel_height=3;
#if 0
  vector<pair<int, long long>> res;
  for (auto i=input_width;i<1200;i++){
    auto t = run(input_height, i, input_channel, filter_batch);
    //printf("%lld, %lld\n", i*input_height, t);
    res.emplace_back(i*input_height, t);
    i+=400;
  }
  for(auto pr: res){
    cout << int(pr.first/1024) << ", " ;
  }
  cout << endl;
  for(auto pr: res){
    cout << pr.second << ",";
  }
#else
  input_width=640, input_height=640, input_channel=256, filter_batch = 2, kernel_width=3, kernel_height=3;
  if (argc==2){
    filter_batch = stoi(argv[1]);
  }
  auto t = run(input_height, input_width, input_channel, filter_batch, kernel_width, kernel_height);
#endif
  return 0;
}
