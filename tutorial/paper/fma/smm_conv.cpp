#include "common.h"

void extract_submatrix(const float* v, float* D, int offset, int H, int W, pair<int, int> hw, int channel) {
  for (int x = 0; x < hw.first; x++) {
    auto src = v + offset + x * W + channel * H * W;
    ::memcpy(D + hw.second*x, src, hw.second * sizeof(float));
  }
}

long long smm_conv_algo(float* I,
    float* F,
    float* sliced_mat,
    float* Output,
    int Kh,
    int Kw,
    int Ci,
    int Co, int input_h, int input_w, int output_h, int output_w) {
      auto start = chrono::high_resolution_clock::now();
      int64_t idx = 0;
      for (int channel = 0; channel < Ci; channel++) {
        for (int i = 0; i < Kw; i++) {
          extract_submatrix(I, sliced_mat, i, input_h, input_w, {output_h, output_w}, channel);
          int64_t area = output_h * output_w;
          if (i == 0 and channel == 0)
            printf("area:%ld,output_h:%d,output_w:%d,size:%ld Byte\n", area, output_h, output_w, (area * sizeof(float)));
          for (int k = 0; k < Kh; k++) {
            auto shifted_start = sliced_mat + output_w * k;
            for (int64_t m = 0; m < Co; m++) {
              idx = k * Kw + i + channel * Kw * Kh + m * Kw * Kh * Ci;
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
      auto t1 = chrono::high_resolution_clock::now();
      long long t = chrono::duration_cast<chrono::microseconds>((t1 - start)).count();
      cout << __FUNCTION__ << " | Compute Time: " << t << " us" << endl;
      return t;
    }




long long run(int run_flag, int input_height, int input_width, int input_channel, int filter_batch, int kernel_width = 3, int kernel_height = 3) {
  printf("input_height:%d,input_width:%d,input_channel:%d,filter_batch:%d,kernel_width:%d,kernel_height:%d\n",
      input_height, input_width, input_channel, filter_batch, kernel_width, kernel_height);
  const int output_height = input_height - kernel_height + 1, output_width = input_width - kernel_width + 1;
  float* sliced_mat=nullptr, *O, *I, *F;

  O = new float[output_height * output_width * filter_batch]{};
  I = new float[input_channel * input_width * input_height]{};
  F = new float[filter_batch * input_channel * kernel_width * kernel_height]{};
  sliced_mat = new float[output_height * output_width]{};

  get_input(I, 1, input_channel, input_width, input_height, 0.1, 1);
  get_input(F, filter_batch, input_channel, kernel_width, kernel_height, 0.1, 1, 0.2);
  printf("output size: %ldB\n", output_height * output_width * filter_batch * sizeof(float));
  printf("filter size: %d\n", filter_batch * input_channel * kernel_width * kernel_height);
  printf("input total size: %.2fKB\n", 1 * input_channel * input_width * input_height / (1024.));

  auto start = high_resolution_clock::now();
  smm_conv_algo(I, F, sliced_mat, O, kernel_height, kernel_width, input_channel, filter_batch, input_height, input_width, output_height, output_width);
  print_output(O, output_height, output_width, filter_batch);
  long long t = duration_cast<microseconds>((high_resolution_clock::now() - start)).count();

  delete [] O;
  delete [] I;
  delete [] F;
  delete [] sliced_mat;
  return t;
}



int main(int argc, char** argv) {
  int input_width = 34, input_height = 24, input_channel = 256, filter_batch = 64, kernel_width = 3, kernel_height = 3, run_flag = 7;
  if (argc >= 2 && strcmp(argv[1], "-h") == 0) {
    std::cout << argv[0] << " filter_batch=32 input_channel=256 input_height=800 input_width=34 run_flag=7 kernel_width=3" << std::endl;
    return 0;
  }
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
  if (argc >= 6) {
    run_flag = stoi(argv[5]);
  }
  if (argc >= 7) {
    kernel_height = kernel_width = stoi(argv[6]);
  }
  assert(run_flag==7);
  auto t = run(run_flag, input_height, input_width, input_channel, filter_batch, kernel_width, kernel_height);
  auto peak = print_peak_mem();

  stringstream result;
  result<< input_channel << "," << input_height << "," << input_width
    << "," << filter_batch << "," << "0" << "," << t << "," << kernel_width
    << "," << run_flag << "," << peak << endl;
  append_file(result.str());
  return 0;
}
