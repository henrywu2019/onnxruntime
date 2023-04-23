#include "common.h"
#include "zpzr_conv.h"


int main(int argc, char** argv) {
  int input_height = 10, input_width = 10, input_channel = 256, filter_batch = 4, kernel_width = 2, kernel_height = 2, run_flag = 16, channel_split=16;
  if (argc >= 2 && strcmp(argv[1], "-h") == 0) {
    std::cout << argv[0] << " filter_batch=32 input_channel=256 input_height=800 input_width=34 run_flag=2 kernel_width=3 channel_split=16" << std::endl;
    return 0;
  }
  if (argc >= 2) {
    filter_batch = std::stoi(argv[1]);
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
    kernel_width = kernel_height = stoi(argv[6]);
  }
  if (argc >= 8) {
    channel_split = stoi(argv[7]);
  }

  const int output_height = input_height - kernel_height + 1, output_width = input_width - kernel_width + 1;
  conv_attr ca(1, input_channel, input_height, input_width, filter_batch, kernel_height, kernel_width);

  float* I = (float*)_mm_malloc(sizeof(float) * input_channel * input_width * input_height, 32);
  get_input(I, 1, input_channel, input_width, input_height, 0.1, 1);

  float* F = (float*)_mm_malloc(sizeof(float) * filter_batch * input_channel * kernel_width * kernel_height, 32);


  get_input(F, filter_batch, input_channel, kernel_width, kernel_height, 0.1, 1, 0.2);
  printf("filter size: %.2fKB\n", filter_batch * input_channel * kernel_width * kernel_height*sizeof(float)/(1024.));
  printf("input total size: %.2fKB\n", sizeof(float) * input_channel * input_width * input_height / (1024.));
  printf("output total size: %.2fKB\n", sizeof(float) * filter_batch * output_width * output_height / (1024.));

  float* O = (float*)_mm_malloc(sizeof(float) * output_height * output_width * filter_batch, 32);

  print_peak_mem();

  fast_conv cw(ca, I, F, O, channel_split, 0);
  auto start = high_resolution_clock::now();

  if (run_flag==2) {
    printf("Fuheng's Algo\n");
    cw.run_full();
  } else if(run_flag==0) {
    printf("naive algo\n");
    cw.run_naive();
  }
  long long t = duration_cast<microseconds>((high_resolution_clock::now() - start)).count();
  cout << "\n\n==============" << endl;
  cout << __FUNCTION__ << ": input_channel-" << input_channel << ",input_height-" << input_height << ",input_width-" << input_width
    << ",filter_batch-" << filter_batch << ",channel_split-" << cw.CHANNEL_SPLIT << ",time-" << t << "us"
    << ",k=" << kernel_width
    << ",flag-" << run_flag << endl;

  cw.print();
  _mm_free(I), _mm_free(F), _mm_free(O);
  auto peak = print_peak_mem();
  stringstream result;
  result<< input_channel << "," << input_height << "," << input_width
    << "," << filter_batch << "," << cw.CHANNEL_SPLIT << "," << t << "," << kernel_width
    << "," << run_flag << "," << peak << endl;
  append_file(result.str());
  return 0;
}
