#include <bits/stdc++.h>
#define ORTCONV
#ifdef ORTCONV
#include "ort_conv.h"
#endif

using namespace std;

/*
 * Based on day6 example, increase the input channel number to 16.
 * */

void get_input(vector<float>& l, int n, int c, int w, int h) {
  if (l.size() < n * c * w * h)
    l.resize(n * c * w * h);
  float start = 1.;
  for (int i = 0; i < n; i++)
    for (int j = 0; j < c; j++) {
      start /= 2 + ((double) rand() / (RAND_MAX)) + 0.1;
      //start = 1;
      for (int m = 0; m < h; m++)
        for (int k = 0; k < w; k++) {
          int idx = k + m * w + h * w * j + h * w * c * i;
          l[idx] = start, start += 0.1; //1.0;
          //l[idx] = start, start += 1;
        }
    }
}

void extract(const vector<float>& v, vector<float>& D, int j, int H, int W, pair<int, int> hw, int channel) {
  if ((int)D.size() < hw.first * hw.second) D.resize(H * hw.second);
  int i = 0;
  for (int x = 0; x < H; x++) {
    auto src = v.data() + j + x * W + channel * H * W;
    ::memcpy(D.data() + i, src, hw.second * sizeof(float));
    i += hw.second;
  }
}


/*
> g++ -O3 main.cpp -std=c++11 -mavx2
> ./a.out
48 | Compute Time: 1000 ns
 * */
void gme_conv(vector<float>& I,
              vector<float>& F,
              vector<float>& sliced_mat,
              float* Output,
              int Kh,
              int Kw,
              int Ci,
              int Co, int input_h, int input_w, int output_h, int output_w) {
  auto start = std::chrono::high_resolution_clock::now();
  for (int c = 0; c < Ci; c++) {
    for (int i = 0; i < Kw; i++) {
      // auto t0 = std::chrono::high_resolution_clock::now();
      extract(I, sliced_mat, i, input_h, input_w, {output_h, output_w}, c);  //////
      // t1 = std::chrono::high_resolution_clock::now();
      // std::cout << __LINE__ << " | Compute Time: " << std::chrono::duration_cast< std::chrono::nanoseconds >((t1 - t0)).count() << " ns" << std::endl; t0=t1;

      static const int64_t area = output_h*output_w;
      for (int k = 0; k < Kh; k++) {
        auto shifted_start = sliced_mat.data() + output_w * k;
        for (int64_t m = 0; m < Co; m++) {
          int64_t idx = k * Kw + i + c * Kw * Kh + m*Kw*Kh*Ci;
          //::printf("%d\n", idx);
          float scalar = F[idx];
          // scalar * (shifted_start: shifted_start+16)
          for (int t = 0; t < area; t++) {
            auto z = *(shifted_start + t);
            idx = t+m*area;
            Output[idx] += scalar * z;
          }
        }
      }
    }
  }

  auto t1 = std::chrono::high_resolution_clock::now();
  std::cout << __FUNCTION__ << " | Compute Time: "
            << std::chrono::duration_cast<std::chrono::milliseconds>((t1 - start)).count()
            << " ms" << std::endl;
}

void print_output(float* Output, int h, int w, int output_channel) {
  for (int t = 0; t < min(32, output_channel*w*h); t++) {
    printf("%.2f, ", Output[t]);
    if (t % 8 == 7) printf("\n");
  }
  printf("\n");
}

const int Filter_Batch = 1;
const int input_channel=16;
const int input_width=800, input_height=600, kernel_width=3, kernel_height=3;
const int output_height =input_height-kernel_height+1, output_width=input_width-kernel_width+1;
float O[output_height*output_width*Filter_Batch] = {};
int main() {
  vector<float> I, F;
  get_input(I, 1, input_channel, input_width, input_height);
  get_input(F, Filter_Batch, input_channel, kernel_width, kernel_height);
  printf("filter size: %d\n",Filter_Batch* input_channel* kernel_width* kernel_height);
  printf("input size: %.2fMB\n",1* input_channel* input_width* input_height/(1024*1024.));

#ifdef ORTCONV
  onnxruntime_convolution(1, 1, input_channel, input_height, input_width,
                          Filter_Batch, kernel_height, kernel_width,
                          0, 0, 0, 0,
                          1, 1,
                          1, 1,
                          output_height, output_width,
                          I.data(),
                          F.data(),
                          nullptr,
                          O);

  print_output(O, output_height, output_width, Filter_Batch);

  ::memset(O, 0, output_height * output_width * sizeof(float)*Filter_Batch);
#endif
  vector<float> sliced_mat(input_height * output_width, 0);
  gme_conv(I, F, sliced_mat, O, kernel_height, kernel_width, input_channel, Filter_Batch,
           input_height, input_width,output_height, output_width);
  print_output(O, output_height, output_width, Filter_Batch);
  return 0;
}
