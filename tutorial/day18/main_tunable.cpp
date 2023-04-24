#include <sein.hpp>
#include "immintrin.h"
#include "tunable_conv.h"
#include "ort_conv.h"


int main(int argc, char** argv) {
  srand(0xdeadbeef);
  int input_height = 24, input_width = 34, input_channel = 64, filter_batch = 64, kernel_width = 3, kernel_height = 3;

  if (argc >= 2) {
    if (strcmp(argv[1], "-h") == 0) {
      printf("xxx.exe filter_batch=1 input_channel=256 input_height=400 input_width=296 run_flag=2 tunable_x=32 tunable_y=8\n");
      return 0;
    }
    filter_batch = stoi(argv[1]);
  }
  int tunable_x = 8, tunable_y = 8, run_flag = 9;
  (argc >= 3) and (input_channel = stoi(argv[2]));
  (argc >= 4) and (input_height = stoi(argv[3]));
  (argc >= 5) and (input_width = stoi(argv[4]));
  (argc >= 6) and (run_flag = stoi(argv[5]));
  (argc >= 7) and (tunable_x = stoi(argv[6]));
  (argc >= 8) and (tunable_y = stoi(argv[7]));

  const int output_height = input_height - kernel_height + 1, output_width = input_width - kernel_width + 1;
  conv_attr ca(1, input_channel, input_height, input_width, filter_batch, kernel_height, kernel_width);
  float* I = (float*)_mm_malloc(sizeof(float) * input_channel * input_width * input_height, 32);
  float* F = (float*)_mm_malloc(sizeof(float) * filter_batch * input_channel * kernel_width * kernel_height, 32);

  make_conv2d_input(I, 1, input_channel, input_width, input_height, 0.1, 1);
  make_conv2d_input(F, filter_batch, input_channel, kernel_width, kernel_height, 0.1, 1, 0.2);
  printf("filter size: %d\n", filter_batch * input_channel * kernel_width * kernel_height);
  printf("input total size: %.2fKB\n", 1 * input_channel * input_width * input_height / (1024.));
  float* O = (float*)_mm_malloc(sizeof(float) * output_height * output_width * filter_batch, 32);

  if (run_flag==9){
    auto_profiler ap("gme_conv");
    tunable_conv cw(ca, I, F, O, tunable_x, tunable_y);
    cw.reorder_input();
    cw.reorder_filter();
    cw.run_tunable();
    cw.restore_output();
    cw.print();
  }else if(run_flag&1){
    auto_profiler ap("ort_conv");
    onnxruntime_conv_nchwc(1, 1, input_channel, input_height, input_width,
                           filter_batch, kernel_height, kernel_width,
                           0, 0, 0, 0,
                           1, 1,
                           1, 1,
                           output_height, output_width,
                           I,
                           F,
                           nullptr,
                           O);
    print_output(O, output_height, output_width, filter_batch, 0);
  }

  _mm_free(I), _mm_free(F), _mm_free(O);
  return 0;
}
