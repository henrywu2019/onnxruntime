#include <sein.hpp>
#include "immintrin.h"
#include "tunable_conv.h"
// #define RANDDATA
#define SIMPLE_INDEX

using namespace std;
using namespace chrono;


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
  int input_height = 401, input_width = 299, input_channel = 256, filter_batch = 64, kernel_width = 3, kernel_height = 3;
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

  tunable_conv cw(ca, I, F, O);
  auto start = high_resolution_clock::now();
  cw.reorder_input();
  cw.reorder_filter();
  cw.run_32_32();
  cw.restore_output();
  long long t = duration_cast<nanoseconds>((high_resolution_clock::now() - start)).count();
  cout << __FUNCTION__ << " | total algo Time: " << t << " ns" << endl;
  cw.print();
  _mm_free(I), _mm_free(F), _mm_free(O);
  return 0;
}
