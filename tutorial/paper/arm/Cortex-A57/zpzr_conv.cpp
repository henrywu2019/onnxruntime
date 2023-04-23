#include "common.h"


class block {
  float* F;
  float* O;
  int len;
  int ih;
  int iw;

 public:
  int ksize=3;
  float* p=nullptr;
  block(int ih_, int ow_, int iw_, int ksize_) : len(ow_), ih(ih_), iw(iw_), ksize(ksize_) {
    F=O=nullptr;
  }
  ~block() {
  }

  void run3x3() {
    for (int k = 0; k < 3; k++) {
      for (int j = 0; j < len; j++) // i == 0
        O[j] += *(p + j) * F[k];
      for (int j = 0; j < len; j++) { // i == 1
        auto z1 = *(p + iw + j);
        O[j] += z1 * F[k + 3];
        O[len + j] += z1 * F[k];
      }
      for (int i = 2; i < ih - 2; i++) { // i:[2 ~ ih-3]
        for (int j = 0; j < len; j++) {
          auto z1 = *(p+i*iw + j);
          auto idx = i * len + j;
          O[idx - 2 * len] += z1 * F[k + 6];
          O[idx - len] += z1 * F[k + 3];
          O[idx] += z1 * F[k];
        }
      }
      for (int j = 0; j < len; j++) { // i == ih-2
        auto z1 = *(p + (ih - 2)*iw + j);
        auto idx = (ih - 2) * len + j;
        O[idx - 2 * len] += z1 * F[k + 6];
        O[idx - len] += z1 * F[k + 3];
      }
      for (int j = 0; j < len; j++) { // i == ih-1
        auto z1 = *(p + (ih - 1)*iw + j);
        auto idx = (ih - 1) * len + j;
        O[idx - 2 * len] += z1 * F[k + 6];
      }
      p++;
    }
  }

  void zpzr_algo_2x2() {
    for (int k = 0; k < 2; k++) {
      for (int j = 0; j < len; j++)
        O[j] += *(p + j) * F[k];
      for (int i = 1; i < ih - 1; i++) {
        for (int j = 0; j < len; j++) {
          auto z1 = *(p +i*iw + j);
          auto idx = i * len + j;
          O[idx - len] += z1 * F[k + 2];
          O[idx] += z1 * F[k];
        }
      }
      for (int j = 0; j < len; j++) {
        auto z1 = *(p + iw*(ih-1) + j);
        auto idx = (ih - 1) * len + j;
        O[idx - 1 * len] += z1 * F[k + 2];
      }
      p++;
    }
  }

  void reset(float* next_head, float* next_filter, float* next_O) {
    F = next_filter;
    p = next_head;
    O = next_O;
  }
};

long long zpzr_algo_run(float* I,
                                 float* F,
                                 float* Output,
                                 int Kh,
                                 int Kw,
                                 int Ci,
                                 int Co, int input_h, int input_w, int output_h, int output_w) {  // O(Ci*3*3*Co*(Oh*Ow))
  auto start = std::chrono::high_resolution_clock::now();
  block blk(input_h, output_w, input_w, Kw);
  for (int channel = 0; channel < Ci; channel++) {
    for (int co = 0; co < Co; co++) {
      blk.reset(I + channel * input_h * input_w,
                F + channel * Kh * Kw + co * Kw * Kh * Ci,
                Output + co * output_h * output_w);
      if (blk.ksize==3)
        blk.run3x3();
      else{
        blk.zpzr_algo_2x2();
      }
    }
  }
  auto t1 = std::chrono::high_resolution_clock::now();
  long long t = std::chrono::duration_cast<std::chrono::microseconds>((t1 - start)).count();
  std::cout << __FUNCTION__ << " | Compute Time: " << t << " us" << std::endl;
  return t;
}




long long run(int run_flag, int input_height, int input_width, int input_channel, int filter_batch, int kernel_width = 3, int kernel_height = 3) {
  printf("input_height:%d,input_width:%d,input_channel:%d,filter_batch:%d,kernel_width:%d,kernel_height:%d\n",
      input_height, input_width, input_channel, filter_batch, kernel_width, kernel_height);
  const int output_height = input_height - kernel_height + 1, output_width = input_width - kernel_width + 1;
  float *O, *I, *F;

  O = new float[output_height * output_width * filter_batch]{};
  I = new float[input_channel * input_width * input_height]{};
  F = new float[filter_batch * input_channel * kernel_width * kernel_height]{};

  get_input(I, 1, input_channel, input_width, input_height, 0.1, 1);
  get_input(F, filter_batch, input_channel, kernel_width, kernel_height, 0.1, 1, 0.2);
  printf("output size: %ldB\n", output_height * output_width * filter_batch * sizeof(float));
  printf("filter size: %d\n", filter_batch * input_channel * kernel_width * kernel_height);
  printf("input total size: %.2fKB\n", 1 * input_channel * input_width * input_height / (1024.));

  auto start = high_resolution_clock::now();

  zpzr_algo_run(I, F, O, kernel_height, kernel_width, input_channel, filter_batch,
                                    input_height, input_width, output_height, output_width);
  print_output(O, output_height, output_width, filter_batch);
  long long t = duration_cast<microseconds>((high_resolution_clock::now() - start)).count();

  delete [] O;
  delete [] I;
  delete [] F;
  return t;
}



int main(int argc, char** argv) {
  int input_width = 34, input_height = 800, input_channel = 256, filter_batch = 64, kernel_width = 3, kernel_height = 3, run_flag = 8;
  if (argc >= 2 && strcmp(argv[1], "-h") == 0) {
    std::cout << argv[0] << " filter_batch=32 input_channel=256 input_height=800 input_width=34 run_flag=2 kernel_width=3" << std::endl;
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
  assert(run_flag==8);
  auto t = run(run_flag, input_height, input_width, input_channel, filter_batch, kernel_width, kernel_height);
  auto peak = print_peak_mem();

  stringstream result;
  result<< input_channel << "," << input_height << "," << input_width
    << "," << filter_batch << "," << "0" << "," << t << "," << kernel_width
    << "," << run_flag << "," << peak << endl;
  append_file(result.str());
  return 0;
}
