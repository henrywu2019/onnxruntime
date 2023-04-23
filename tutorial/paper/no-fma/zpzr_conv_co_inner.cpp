#include "common.h"


struct conv_attr {
  int N, C, H, W;  // input
  int K, R, L;     // kernel
  int OH, OW;      // output
  conv_attr(int N_, int C_, int H_, int W_, int K_, int R_, int L_) : N(N_), C(C_), H(H_), W(W_), K(K_), R(R_), L(L_) {
    OH = H - R + 1;
    OW = W - L + 1;
  }
};

struct block {
  float* F;
  float* O;

  conv_attr ca;
 public:
  int ksize=3;
  float* p=nullptr;
  block(const conv_attr& ca_):ca(ca_) {
    F=O=nullptr;
  }
  ~block() {}

  inline int oid(int co_, int oh_, int ow_){
    return ow_ + oh_*ca.OW + co_*ca.OW*ca.OH;
  }

  void zpzr_algo_3x3(int ch) {
    for (int k = 0; k < 3; k++) { // R
      for (int j = 0; j < ca.OW; j++){ // i == 0
        for(int z=0;z< ca.K;z++){
          O[oid(z,0,j)] += *(p + j) * F[k];
        }
      }
      for (int j = 0; j < ca.OW; j++) { // i == 1
        auto z1 = *(p + ca.W + j);
        for(int z=0;z< ca.K;z++){
          O[oid(z,0,j)] += z1 * F[k + 3];
          O[oid(z,1,j)] += z1 * F[k];
        }
      }
      for (int i = 2; i < ca.OH; i++) { // i:[2 ~ ca.H-3]
        for (int j = 0; j < ca.OW; j++) {
          auto z1 = *(p+i*ca.W + j);
          for(int z=0;z< ca.K;z++){
            O[oid(z,i-2,j)] += z1 * F[k + 6];
            O[oid(z,i-1,j)] += z1 * F[k + 3];
            O[oid(z,i,j)] += z1 * F[k];
          }
        }
      }
      for (int j = 0; j < ca.OW; j++) {
        auto z1 = *(p + (ca.H - 2)*ca.W + j);
        for(int z=0;z< ca.K;z++){
          O[oid(z,ca.OH-2,j)] += z1 * F[k + 6];
          O[oid(z,ca.OH-1,j)] += z1 * F[k + 3];
        }
      }
      for (int j = 0; j < ca.OW; j++) {
        auto z1 = *(p + (ca.H - 1)*ca.W + j);
        for(int z=0;z< ca.K;z++){
          O[oid(z,ca.OH-1,j)] += z1 * F[k + 6];
        }
      }
      p++;
    }
  }

  void zpzr_algo_2x2(int ch) {
    for (int k = 0; k < 2; k++) {
      for (int j = 0; j < ca.OW; j++)
        O[j] += *(p + j) * F[k];
      for (int i = 1; i < ca.H - 1; i++) {
        for (int j = 0; j < ca.OW; j++) {
          auto z1 = *(p +i*ca.W + j);
          auto idx = i * ca.OW + j;
          O[idx - ca.OW] += z1 * F[k + 2];
          O[idx] += z1 * F[k];
        }
      }
      for (int j = 0; j < ca.OW; j++) {
        auto z1 = *(p + ca.W*(ca.H-1) + j);
        auto idx = (ca.H - 1) * ca.OW + j;
        O[idx - 1 * ca.OW] += z1 * F[k + 2];
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
                                 const conv_attr& ca) {  // O(Ci*3*3*ca.K*(Oh*Ow))
  auto start = std::chrono::high_resolution_clock::now();
  block blk(ca);
  for (int channel = 0; channel < ca.C; channel++) {
    //for (int co = 0; co < ca.K; co++) {
      blk.reset(I + channel * ca.H * ca.W,
                F + channel * ca.R * ca.L,
                Output);
      if (blk.ca.R==3)
        blk.zpzr_algo_3x3(channel);
      else{
        blk.zpzr_algo_2x2(channel);
      }
    //}
  }
  auto t1 = std::chrono::high_resolution_clock::now();
  long long t = std::chrono::duration_cast<std::chrono::microseconds>((t1 - start)).count();
  std::cout << __FUNCTION__ << " | Compute Time: " << t << " us" << std::endl;
  return t;
}




long long run(int run_flag, int input_height, int input_width, int input_channel, int filter_batch, int kernel_width = 3, int kernel_height = 3) {
  printf("input_height:%d,input_width:%d,input_channel:%d,filter_batch:%d,kernel_width:%d,kernel_height:%d,run_flag:%d\n",
      input_height, input_width, input_channel, filter_batch, kernel_width, kernel_height, run_flag);
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

  conv_attr ca(1, input_channel, input_height, input_width, filter_batch, kernel_height, kernel_width);
  auto start = high_resolution_clock::now();

  zpzr_algo_run(I, F, O, ca);
  print_output(O, output_height, output_width, filter_batch);
  long long t = duration_cast<microseconds>((high_resolution_clock::now() - start)).count();

  delete [] O;
  delete [] I;
  delete [] F;
  return t;
}



int main(int argc, char** argv) {
  int input_width = 34, input_height = 800, input_channel = 256, filter_batch = 64, kernel_width = 3, kernel_height = 3, run_flag = 9;
  if (argc >= 2 && strcmp(argv[1], "-h") == 0) {
    std::cout << argv[0] << " filter_batch=32 input_channel=256 input_height=800 input_width=34 run_flag=9 kernel_width=3" << std::endl;
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
  assert(run_flag==9);
  auto t = run(run_flag, input_height, input_width, input_channel, filter_batch, kernel_width, kernel_height);
  auto peak = print_peak_mem();

  stringstream result;
  result<< input_channel << "," << input_height << "," << input_width
    << "," << filter_batch << "," << "0" << "," << t << "," << kernel_width
    << "," << run_flag << "," << peak << endl;
  append_file(result.str());
  return 0;
}
