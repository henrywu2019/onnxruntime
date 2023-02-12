#include <bits/stdc++.h>
#include "immintrin.h"
// #include "fmaintrin.h"
#define ORTCONV
// #define RANDDATA
#ifdef ORTCONV
#include "ort_conv.h"
#endif

using namespace std;

long l1_cache_size = sysconf(_SC_LEVEL1_DCACHE_SIZE);

/*
 * Based on day6 example, increase the input channel number to 16.
 * */

void get_input(vector<float>& l, int n, int c, int w, int h, float channel_delta = 0.1, float cell_delta = 1, float batch_delta=0.2) {
  if ((int)l.size() < n * c * w * h) l.resize(n * c * w * h);
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

long long extract_time = 0;
int LINE = 1;

void extract(const vector<float>& v, float* D, int offset, int H, int W, pair<int, int> hw, int channel) {
  auto t0 = std::chrono::high_resolution_clock::now();
  // if ((int)D.size() < hw.first * hw.second) D.resize(H * hw.second);
  int i = 0;
  for (int x = 0; x < H; x++) {
    auto src = v.data() + offset + x * W + channel * H * W;
    ::memcpy(D + i, src, hw.second * sizeof(float));
    i += hw.second;
  }
  auto t1 = std::chrono::high_resolution_clock::now();
  extract_time += std::chrono::duration_cast<std::chrono::nanoseconds>((t1 - t0)).count();
}

// extract LINE lines data to D
pair<int, float*> extract100(float* src, float* D, int H, int W, const pair<int, int>& hw, int channel, int line) {
  auto t0 = std::chrono::high_resolution_clock::now();
  int i = 0;
  for (int x = 0; x < line; x++) {
    ::memcpy(D + i, src, hw.second * sizeof(float));
    i += hw.second;
    src += W;
  }
  auto t1 = std::chrono::high_resolution_clock::now();
  extract_time += std::chrono::duration_cast<std::chrono::nanoseconds>((t1 - t0)).count();
  return {i, src};
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
                   int Kh,  // kernel height
                   int Kw,  // kernel width
                   int Ci,
                   int Co, int input_h, int input_w, int output_h, int output_w) {  // O(Ci*3*3*Co*(Oh*Ow))
  auto start = std::chrono::high_resolution_clock::now();
  for (int channel = 0; channel < Ci; channel++) {
    int mini_batch = int(output_h / LINE);
    int remaining_row = output_h - mini_batch * LINE;

    pair<int, float*> pr;
    for (int i = 0; i < Kw; i++) {
      int pp[3] = {};
      pr = {0, I.data() + i + channel * input_h * input_w};

      for (int h_ = 0; h_ < mini_batch; h_++) {
        pr = extract100(pr.second, sliced_mat, input_h, input_w, {output_h, output_w}, channel, LINE);

        auto shifted_start = sliced_mat;  // sliding down OK
        int64_t area = pr.first;
        if (i == 0 and channel == 0 and h_ == 0)
          printf("area:%ld,output_h:%d,output_w:%d,size:%ld Byte\n", area, output_h, output_w, (area * sizeof(float)));
        for (int64_t m = 0; m < Co; m++) {
          int ix = i + channel * Kw * Kh + m * Kw * Kh * Ci;
          //::printf("%d\n", idx);
          float scalar = F[ix];
          // scalar * (shifted_start: shifted_start+16)
          int64_t idx = 0;
          for (int t = 0; t < area; t++) {
            auto z = *(shifted_start + t);  // OK
            Output[pp[0] + t] += scalar * z;
          }
        }
        pp[0] += area;
      }

      // remaining
      pr = extract100(pr.second, sliced_mat, input_h, input_w, {output_h, output_w}, channel, remaining_row);
      int64_t area = pr.first;
      for (int k = 0; k < Kh; k++) {                     // w1, w4, w7
        auto shifted_start = sliced_mat + output_w * k;  // sliding down
        for (int64_t m = 0; m < Co; m++) {
          int ix = k * Kw + i + channel * Kw * Kh + m * Kw * Kh * Ci;
          float scalar = F[ix];
          // scalar * (shifted_start: shifted_start+16)
          for (int t = 0; t < area; t++) {
            auto z = *(shifted_start + t);
            Output[pp[0] + t] += scalar * z;
          }
        }
      }
    }
  }

  auto t1 = std::chrono::high_resolution_clock::now();
  long long t = std::chrono::duration_cast<std::chrono::nanoseconds>((t1 - start)).count();
  std::cout << __FUNCTION__ << " | Compute Time: " << t << " ns" << std::endl;
  return t;
}

long long extract_time_o = 0;

void extract_o(const vector<float>& v, float* D, int offset, int H, int W, pair<int, int> hw, int channel) {
  auto t0 = std::chrono::high_resolution_clock::now();
  // if ((int)D.size() < hw.first * hw.second) D.resize(H * hw.second);
  int i = 0;
  for (int x = 0; x < H; x++) {
    auto src = v.data() + offset + x * W + channel * H * W;
    ::memcpy(D + i, src, hw.second * sizeof(float));
    i += hw.second;
  }
  auto t1 = std::chrono::high_resolution_clock::now();
  extract_time_o += std::chrono::duration_cast<std::chrono::nanoseconds>((t1 - t0)).count();
}

long long gme_conv_ori(vector<float>& I,
                       vector<float>& F,
                       float* sliced_mat,
                       float* Output,
                       int Kh,
                       int Kw,
                       int Ci,
                       int Co, int input_h, int input_w, int output_h, int output_w) {  // O(Ci*3*3*Co*(Oh*Ow))
  auto start = std::chrono::high_resolution_clock::now();
  int64_t idx = 0;
  for (int channel = 0; channel < Ci; channel++) {
    for (int i = 0; i < Kw; i++) {
      // auto t0 = std::chrono::high_resolution_clock::now();
      extract_o(I, sliced_mat, i, input_h, input_w, {output_h, output_w}, channel);  //////
      // t1 = std::chrono::high_resolution_clock::now();
      // std::cout << __LINE__ << " | Compute Time: " << std::chrono::duration_cast< std::chrono::nanoseconds >((t1 - t0)).count() << " us" << std::endl; t0=t1;

      int64_t area = output_h * output_w;
      if (i == 0 and channel == 0)
        printf("area:%ld,output_h:%d,output_w:%d,size:%ld Byte\n", area, output_h, output_w, (area * sizeof(float)));

      for (int k = 0; k < Kh; k++) {                     // w1, w4, w7
        auto shifted_start = sliced_mat + output_w * k;  // sliding down
        for (int64_t m = 0; m < Co; m++) {
          idx = k * Kw + i + channel * Kw * Kh + m * Kw * Kh * Ci;
          //::printf("%d\n", idx);
          float scalar = F[idx];
          for (int t = 0; t < area; t++) {
            auto z = *(shifted_start + t);
            idx = t + m * area;
            Output[idx] += scalar * z;
          }
        }
      }
    }
  }
  printf("idx: %ld\n", idx);

  auto t1 = std::chrono::high_resolution_clock::now();
  long long t = std::chrono::duration_cast<std::chrono::nanoseconds>((t1 - start)).count();
  std::cout << __FUNCTION__ << " | Compute Time: " << t << " ns" << std::endl;
  return t;
}
extern "C" int _compute(float **pa, float*O, float* F, int ih, int len);
class block {
  float** pa;
  float* F;
  float* O;
  int len;
  int ih;
  int iw;
  float** fs; //filter by filter_batch
  float** os; //output by filter_batch

 public:
  block(int ih_, int ow_, int iw_) : len(ow_), ih(ih_), iw(iw_) {
    pa = new float*[ih];
  }
  ~block() { delete[] pa; }
  /*
   * 23:55 $ sudo perf stat -d ./main_release.exe 1 1 400 296 8 8
argc: 7
L1 data cache size: 32768 bytes
L1 data cache line: 64 bytes
L2 data cache size: 262144 bytes
L3 data cache size: 8388608 bytes
input_height:400,input_width:296,input_channel:1,filter_batch:1,kernel_width:3,kernel_height:3,LINE:13
output size: 468048B
filter size: 9
input total size: 115.62KB
gme_conv_no_extraction | Compute Time: 83837 ns
18744.00,18789.00,18834.00,18879.00,18924.00,18969.00,19014.00,19059.00,19104.00,19149.00,19194.00,19239.00,19284.00,19329.00,19374.00,19419.00,
19464.00,19509.00,19554.00,19599.00,19644.00,19689.00,19734.00,19779.00,19824.00,19869.00,19914.00,19959.00,20004.00,20049.00,20094.00,20139.00,
...5319249.00,5319294.00,5319339.00,5319384.00,5319429.00,5319474.00,5319519.00,5319564.00,5319609.00,5319654.00,5319699.00,5319744.00,5319789.00,
5319834.00,5319879.00,5319924.00,5319969.00,
==============================================================================

 Performance counter stats for './main_release.exe 1 1 400 296 8 8':

              1.80 msec task-clock                #    0.824 CPUs utilized
                 0      context-switches          #    0.000 /sec
                 0      cpu-migrations            #    0.000 /sec
               369      page-faults               #  204.937 K/sec
         6,999,525      cycles                    #    3.887 GHz
         6,249,387      instructions              #    0.89  insn per cycle
         1,041,547      branches                  #  578.460 M/sec
            24,983      branch-misses             #    2.40% of all branches
         1,499,735      L1-dcache-loads           #  832.931 M/sec
           132,483      L1-dcache-load-misses     #    8.83% of all L1-dcache accesses
     <not counted>      LLC-loads                                                     (0.00%)
     <not counted>      LLC-load-misses                                               (0.00%)

       0.002184749 seconds time elapsed

       0.002258000 seconds user
       0.000000000 seconds sys
=====================================================================================================
    ./main_release.exe 1 1 66 6402 8 8
  is faster than:
    ./main_release.exe 1 1 6402 66 8 8
  by 10%

It is even slower than that when Co is outside!
*/
  //
  void run(int output_h, int output_w, int Co, int Ci) {
#if 0
    _compute(pa, O, F, ih, len);
#else
    for (int k = 0; k < 3; k++) {
      //float f0=F[k], f3=F[k+3], f6=F[k+6];
      for (int j = 0; j < len; j++){
        for(int co=0;co<Co;co++)
          *(O+j+output_h * output_w*co) += pa[0][j] * *(F+k+co*9*Ci);
      }
      // i == 1
      for (int j = 0; j < len; j++) {
        for(int co=0;co<Co;co++){
          *(O+j+output_h * output_w*co) += pa[1][j] * *(F+k+3+co*9*Ci);
          *(O+j+output_h * output_w*co+len) += pa[1][j] * *(F+k+co*9*Ci);
        }
      }
      // compute_3
      for (int i = 2; i < ih - 2; i++) {
        for (int j = 0; j < len; j++) {
          for(int co=0;co<Co;co++){
            auto z1 = pa[i][j];
            auto idx = (i-2)*len+j;
            *(O+idx+output_h*output_w*co) += pa[1][j] * *(F+k+6+co*9*Ci);
            *(O+idx+len+output_h*output_w*co) += pa[1][j] * *(F+k+3+co*9*Ci);
            *(O+idx+2*len+output_h*output_w*co) += pa[1][j] * *(F+k+co*9*Ci);
          }
        }
      }
      // i == ih-2
      for (int j = 0; j < len; j++) {
        for(int co=0;co<Co;co++){
          auto z1 = pa[ih - 2][j];
          auto idx = (ih - 4) * len + j;
          *(O+idx+output_h*output_w*co) += pa[1][j] * *(F+k+6+co*9*Ci);
          *(O+idx+len+output_h*output_w*co) += pa[1][j] * *(F+k+3+co*9*Ci);
        }
      }
      // i == ih-1
      for (int j = 0; j < len; j++) {
        for(int co=0;co<Co;co++){
          auto z1 = *(pa[ih - 1] + j);
          auto idx = (ih - 3) * len + j;
          *(O+idx+output_h*output_w*co) += pa[1][j] * *(F+k+6+co*9*Ci);
        }
      }
      advance();
    }
#endif
  }

  void advance() {
    for (auto i = 0; i < ih; i++) pa[i]++;
  }

  void reset(float* next_head, float* next_filter, float* next_O) {
    F = next_filter;
    for (auto i = 0; i < ih; i++)
      pa[i] = next_head + i * iw;
    O = next_O;
  }
};

long long gme_conv_no_extraction(vector<float>& I,
                                 vector<float>& F,
                                 float* Output,
                                 int Kh,
                                 int Kw,
                                 int Ci,
                                 int Co, int input_h, int input_w, int output_h, int output_w) {  // O(Ci*3*3*Co*(Oh*Ow))
  auto start = std::chrono::high_resolution_clock::now();
  block blk(input_h, output_w, input_w);
  blk.reset(I.data(), F.data(), Output);
  for (int channel = 0; channel < Ci; channel++) {
      //blk.reset(I.data() + channel * input_h * input_w,
      //          F.data() + channel * Kh * Kw + co * Kw * Kh * Ci,
      //          Output + co * output_h * output_w);
      blk.run(output_h, output_w, Co, Ci);
  }

  auto t1 = std::chrono::high_resolution_clock::now();
  long long t = std::chrono::duration_cast<std::chrono::nanoseconds>((t1 - start)).count();
  std::cout << __FUNCTION__ << " | Compute Time: " << t << " ns" << std::endl;
  return t;
}

void print_output(float* Output, int h, int w, int output_channel, bool all = false) {
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

long long run(int run_flag, int input_height, int input_width, int input_channel, int filter_batch, int kernel_width = 3, int kernel_height = 3, int factor = 8) {
  LINE = min(int(l1_cache_size / (input_width - kernel_width + 1)), input_height) / factor;  // TODO
  printf("input_height:%d,input_width:%d,input_channel:%d,filter_batch:%d,kernel_width:%d,kernel_height:%d,LINE:%d\n",
         input_height, input_width, input_channel, filter_batch, kernel_width, kernel_height, LINE);
  const int output_height = input_height - kernel_height + 1, output_width = input_width - kernel_width + 1;
  float* O = new float[output_height * output_width * filter_batch]();
  printf("output size: %ldB\n", output_height * output_width * filter_batch * sizeof(float));

  vector<float> I, F;
  get_input(I, 1, input_channel, input_width, input_height, 0.1, 1);
  get_input(F, filter_batch, input_channel, kernel_width, kernel_height, 0.1, 1, 0.2);
  printf("filter size: %d\n", filter_batch * input_channel * kernel_width * kernel_height);
  printf("input total size: %.2fKB\n", 1 * input_channel * input_width * input_height / (1024.));

  // if(run_flag & 0b100){
  if (0) {
    float* sliced_mat_o = (float*)_mm_malloc(sizeof(float) * output_height * output_width, 32);
    auto t = gme_conv_ori(I, F, sliced_mat_o, O, kernel_height, kernel_width, input_channel, filter_batch,
                          input_height, input_width, output_height, output_width);
    print_output(O, output_height, output_width, filter_batch);
    _mm_free(sliced_mat_o);
    ::memset(O, 0, output_height * output_width * sizeof(float) * filter_batch);

    printf("\nextract time: %lld ns\n", extract_time_o);
    printf("==============================================================================\n");
  }

  if (run_flag & 0b1000) {
    auto t = gme_conv_no_extraction(I, F, O, kernel_height, kernel_width, input_channel, filter_batch,
                                    input_height, input_width, output_height, output_width);
    print_output(O, output_height, output_width, filter_batch);
    ::memset(O, 0, output_height * output_width * sizeof(float) * filter_batch);
    printf("\n==============================================================================\n");
  }

  // if (run_flag & 0b10) {
  if (0) {
    // vector<float> sliced_mat(input_height * output_width, 0);
    // float* sliced_mat = (float*) _mm_malloc(sizeof(float) * input_height * output_width, 32);
    float* sliced_mat = (float*)_mm_malloc(sizeof(float) * LINE * output_width, 32);
    auto t = gme_conv(I, F, sliced_mat, O, kernel_height, kernel_width, input_channel, filter_batch,
                      input_height, input_width, output_height, output_width);
    print_output(O, output_height, output_width, filter_batch);
    _mm_free(sliced_mat);
    ::memset(O, 0, output_height * output_width * sizeof(float) * filter_batch);

    printf("\nextract time: %lld ns\n", extract_time);
    printf("==============================================================================\n");
  }

  if (run_flag & 0b1) {
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
    print_output(O, output_height, output_width, filter_batch, 0);
    ::memset(O, 0, output_height * output_width * sizeof(float) * filter_batch);
    printf("\n==============================================================================\n");
#endif
  }

  if (run_flag & 0b10000) {
#ifdef ORTCONV
    onnxruntime_conv_nchw(1, 1, input_channel, input_height, input_width,
                          filter_batch, kernel_height, kernel_width,
                          0, 0, 0, 0,
                          1, 1,
                          1, 1,
                          output_height, output_width, I.data(), F.data(), nullptr, O);
    print_output(O, output_height, output_width, filter_batch, 0);
    ::memset(O, 0, output_height * output_width * sizeof(float) * filter_batch);
    printf("\n==============================================================================\n");
#endif
  }
  // direct conv
  if (run_flag & 0b100000) {

  }
  delete[] O;
  return 0;
}

/*
Conv node: Conv_nchwc_gme_149
Input 0 Name: reorder_gme_147
 Shape: {1,256,400,296}
Input 1 Name: reorder_gme_148
 Shape: {64,256,3,3}
*/
int main(int argc, char** argv) {
  srand(0xdeadbeef);
  printf("argc: %d\n", argc);
  // 64x256x3x3
  int input_width = 100, input_height = 3000, input_channel = 2, filter_batch = 1, kernel_width = 3, kernel_height = 3;
  if (argc >= 2) {
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
  auto t = run(run_flag, input_height, input_width, input_channel, filter_batch, kernel_width, kernel_height, factor);
  return 0;
}
