#include "common.h"
#include "ort_conv.h"


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
  long long t = duration_cast<microseconds>((high_resolution_clock::now() - start)).count();

  delete [] O;
  delete [] I;
  delete [] F;
  return t;
}



int main(int argc, char** argv) {
  int input_width = 34, input_height = 800, input_channel = 256, filter_batch = 64, kernel_width = 3, kernel_height = 3, run_flag = 7;
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
  assert(run_flag==1);
  auto t = run(run_flag, input_height, input_width, input_channel, filter_batch, kernel_width, kernel_height);
  auto peak = print_peak_mem();

  stringstream result;
  result<< input_channel << "," << input_height << "," << input_width
    << "," << filter_batch << "," << "0" << "," << t << "," << kernel_width
    << "," << run_flag << "," << peak << endl;
  append_file(result.str());
  return 0;
}
