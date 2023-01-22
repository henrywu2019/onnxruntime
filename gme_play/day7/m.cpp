#include <bits/stdc++.h>
#include "/home/henry/wendy/git.repo/onnxruntime/onnxruntime/core/mlas/inc/mlas.h"
using namespace std;

/*
 * Based on day6 example, increase the input channel number to 16.
 * */

vector<float> get_input(int mx = 6*6) {
  auto l = vector<float>(mx);
  std::iota(l.begin(), l.end(), 1);
  return l;
}

vector<float> get_input(int n, int c, int w, int h) {
  auto l = vector<float>(n*c*w*h);
  float start=1.;
  for (int i=0;i<n;i++)
    for(int j=0;j<c;j++){
      start=1;
      for(int m=0;m<h;m++)
        for(int k=0;k<w;k++){
          int idx = k + m*w + h*w*j + h*w*c*i;
          l[idx] = start, start += 1.0;
        }
    }
  return l;
}

float O[16] = {};

void extract(const vector<float>& v, vector<float>& D, int j, int H, int W, pair<int, int> hw, int channel) {
  if ((int)D.size() < hw.first * hw.second) D.resize(H * hw.second);
  int i = 0;
  for (int x = 0; x < H; x++) {
    auto src = v.data() + j + x * W + channel*H*W;
    ::memcpy(D.data() + i, src, hw.second * sizeof(float));
    i += hw.second;
  }
}

/*
> g++ -O3 main.cpp -std=c++11 -mavx2
> ./a.out
48 | Compute Time: 1000 ns
 * */

void gme_conv(vector<float>& I, vector<float>& F, vector<float>& sliced_mat, float* Output, int Ci) {
  auto start = std::chrono::high_resolution_clock::now();
  for(int c=0; c<Ci; c++){
    for (int i = 0; i < 3; i++) {
      // auto t0 = std::chrono::high_resolution_clock::now();
      extract(I, sliced_mat, i, 6, 6, {4, 4}, c);  //////
      // t1 = std::chrono::high_resolution_clock::now();
      // std::cout << __LINE__ << " | Compute Time: " << std::chrono::duration_cast< std::chrono::nanoseconds >((t1 - t0)).count() << " ns" << std::endl; t0=t1;

      for (int k = 0; k < 3; k++) {
        auto shifted_start = sliced_mat.data() + 4 * k;
        for (int m = 0; m < 1; m++) {
          float scalar = F[k * 3 + i];
          // scalar * (shifted_start: shifted_start+16)
          for (int t = 0; t < 16; t++) {
            auto z = *(shifted_start + t);
            Output[t] += scalar * z;
          }
        }
      }
    }
  }

  auto t1 = std::chrono::high_resolution_clock::now();
  std::cout << __FUNCTION__ << " | Compute Time: " << std::chrono::duration_cast<std::chrono::nanoseconds>((t1 - start)).count() << " ns" << std::endl;
}

void print_output() {
  for (int t = 0; t < 16; t++) {
    printf("%f, ", O[t]);
    if (t % 4 == 3) printf("\n");
  }
  printf("\n");
}

int main() {
  auto I = get_input(1,16,6,6);
  auto F = get_input(9);



  ::memset(O, 0, 16 * sizeof(float));

  vector<float> sliced_mat(6 * 4, 0);
  gme_conv(I, F, sliced_mat, O, 16);
  print_output();
  return 0;
}
