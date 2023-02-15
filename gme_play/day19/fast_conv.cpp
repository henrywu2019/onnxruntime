//
// Created by henry on 2/14/23.
//
#include "fast_conv.h"
#include <sein.hpp>
#define SIMPLE_INDEX

#define MACRO1(i,z) {y1 = _mm256_set1_ps(kernel[filter_index(i,c_,0,0)]);y##z = _mm256_fmadd_ps(y0, y1, y##z);}
#define MACRO2(i,z) {y1 = _mm256_set1_ps(kernel[filter_index(i,c_,1,0)]);y##z = _mm256_fmadd_ps(y0, y1, y##z);}
#define MACRO3(i,z) {y1 = _mm256_set1_ps(kernel[filter_index(i,c_,2,0)]);y##z = _mm256_fmadd_ps(y0, y1, y##z);}

#define MACRO4(i,z) {y2 = _mm256_set1_ps(kernel[filter_index(i,c_,0,1)]);y##z = _mm256_fmadd_ps(y0, y2, y##z);}
#define MACRO5(i,z) {y2 = _mm256_set1_ps(kernel[filter_index(i,c_,1,1)]);y##z = _mm256_fmadd_ps(y0, y2, y##z);}
#define MACRO6(i,z) {y2 = _mm256_set1_ps(kernel[filter_index(i,c_,2,1)]);y##z = _mm256_fmadd_ps(y0, y2, y##z);}

#define MACRO7(i,z) {y3 = _mm256_set1_ps(kernel[filter_index(i,c_,0,2)]);y##z = _mm256_fmadd_ps(y0, y3, y##z);}
#define MACRO8(i,z) {y3 = _mm256_set1_ps(kernel[filter_index(i,c_,1,2)]);y##z = _mm256_fmadd_ps(y0, y3, y##z);}
#define MACRO9(i,z) {y3 = _mm256_set1_ps(kernel[filter_index(i,c_,2,2)]);y##z = _mm256_fmadd_ps(y0, y3, y##z);}

void print_m256(const __m256& v){
  float* f = (float*)&v;
  printf("%f %f %f %f %f %f %f %f\n", f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7]);

}

void fast_conv::run() {
  __m256 y0{}, y1{}, y2{}, y3{};
  __m256 y4{}, y5{}, y6{}, y7{}, y8{}, y9{}, y10{}, y11{}, y12{}, y13{}, y14{}, y15{}; // 8*12
  for(int w_=0;w_ < ca.OW; w_+=8){
    REP(h_,0,ca.H){
      REP(c_,0,ca.C){
        int i_offset=input_index(c_,h_,w_);
        if(h_==0){
          y0 = _mm256_loadu_ps(input + i_offset++);
          y1 = _mm256_set1_ps(kernel[filter_index(0,c_,0,0)]);
          y4 = _mm256_fmadd_ps(y0, y1, y4);

          y0 = _mm256_loadu_ps(input + i_offset++);
          y1 = _mm256_set1_ps(kernel[filter_index(0,c_,0,1)]);
          y4 = _mm256_fmadd_ps(y0, y1, y4);

          y0 = _mm256_loadu_ps(input + i_offset++);
          y1 = _mm256_set1_ps(kernel[filter_index(0,c_,0,2)]);
          y4 = _mm256_fmadd_ps(y0, y1, y4);
        }
        else if(h_==1)
        {
          y0 = _mm256_loadu_ps(input + i_offset++);
          y1 = _mm256_set1_ps(kernel[filter_index(0,c_,0,0)]);
          y2 = _mm256_set1_ps(kernel[filter_index(0,c_,1,0)]);
          y5 = _mm256_fmadd_ps(y0, y1, y5);
          y4 = _mm256_fmadd_ps(y0, y2, y4);

          y0 = _mm256_loadu_ps(input + i_offset++);
          y1 = _mm256_set1_ps(kernel[filter_index(0,c_,0,1)]);
          y2 = _mm256_set1_ps(kernel[filter_index(0,c_,1,1)]);
          y5 = _mm256_fmadd_ps(y0, y1, y5);
          y4 = _mm256_fmadd_ps(y0, y2, y4);

          y0 = _mm256_loadu_ps(input + i_offset++);
          y1 = _mm256_set1_ps(kernel[filter_index(0,c_,0,2)]);
          y2 = _mm256_set1_ps(kernel[filter_index(0,c_,1,2)]);
          y5 = _mm256_fmadd_ps(y0, y1, y5);
          y4 = _mm256_fmadd_ps(y0, y2, y4);
        }
        else if(h_<ca.H-2)
        {
          {
            y0 = _mm256_loadu_ps(input + i_offset++);
            y1 = _mm256_set1_ps(kernel[filter_index(0,c_,0,0)]);
            y2 = _mm256_set1_ps(kernel[filter_index(0,c_,1,0)]);
            y3 = _mm256_set1_ps(kernel[filter_index(0,c_,2,0)]);
            y6 = _mm256_fmadd_ps(y0, y1, y6);
            y5 = _mm256_fmadd_ps(y0, y2, y5);
            y4 = _mm256_fmadd_ps(y0, y3, y4);
          }
          {
            y0 = _mm256_loadu_ps(input + i_offset++);
            y1 = _mm256_set1_ps(kernel[filter_index(0,c_,0,1)]);
            y2 = _mm256_set1_ps(kernel[filter_index(0,c_,1,1)]);
            y3 = _mm256_set1_ps(kernel[filter_index(0,c_,2,1)]);
            y6 = _mm256_fmadd_ps(y0, y1, y6);
            y5 = _mm256_fmadd_ps(y0, y2, y5);
            y4 = _mm256_fmadd_ps(y0, y3, y4);
          }
          {
            y0 = _mm256_loadu_ps(input + i_offset++);
            y1 = _mm256_set1_ps(kernel[filter_index(0,c_,0,2)]);
            y2 = _mm256_set1_ps(kernel[filter_index(0,c_,1,2)]);
            y3 = _mm256_set1_ps(kernel[filter_index(0,c_,2,2)]);
            y6 = _mm256_fmadd_ps(y0, y1, y6);
            y5 = _mm256_fmadd_ps(y0, y2, y5);
            y4 = _mm256_fmadd_ps(y0, y3, y4);
          }
          // write y4 to output
          //print_m256(y4);
          if(c_+1==ca.C){
            _mm256_storeu_ps(output+output_index(0,h_-2,w_), y4);
            _mm256_storeu_ps((float*)&y4,y5);
            _mm256_storeu_ps((float*)&y5,y6);
            y6 = _mm256_setzero_ps();
          }
        }
        else if(h_==ca.H-2)
        {
          y0 = _mm256_loadu_ps(input + i_offset++);
          y2 = _mm256_set1_ps(kernel[filter_index(0,c_,1,0)]);
          y3 = _mm256_set1_ps(kernel[filter_index(0,c_,2,0)]);
          y5 = _mm256_fmadd_ps(y0, y2, y5);
          y4 = _mm256_fmadd_ps(y0, y3, y4);

          y0 = _mm256_loadu_ps(input + i_offset++);
          y2 = _mm256_set1_ps(kernel[filter_index(0,c_,1,1)]);
          y3 = _mm256_set1_ps(kernel[filter_index(0,c_,2,1)]);
          y5 = _mm256_fmadd_ps(y0, y2, y5);
          y4 = _mm256_fmadd_ps(y0, y3, y4);

          y0 = _mm256_loadu_ps(input + i_offset++);
          y2 = _mm256_set1_ps(kernel[filter_index(0,c_,1,2)]);
          y3 = _mm256_set1_ps(kernel[filter_index(0,c_,2,2)]);
          y5 = _mm256_fmadd_ps(y0, y2, y5);
          y4 = _mm256_fmadd_ps(y0, y3, y4);
          if(c_+1==ca.C){
            _mm256_storeu_ps(output+output_index(0,h_-2,w_), y4);
            //print_m256(y4);
            _mm256_storeu_ps((float*)&y4,y5);
            y5 = _mm256_setzero_ps();
          }
        }else{ //(h_==ca.OH-1)
          y0 = _mm256_loadu_ps(input + i_offset++);
          y3 = _mm256_set1_ps(kernel[filter_index(0,c_,2,0)]);
          y4 = _mm256_fmadd_ps(y0, y3, y4);

          y0 = _mm256_loadu_ps(input + i_offset++);
          y3 = _mm256_set1_ps(kernel[filter_index(0,c_,2,1)]);
          y4 = _mm256_fmadd_ps(y0, y3, y4);

          y0 = _mm256_loadu_ps(input + i_offset++);
          y3 = _mm256_set1_ps(kernel[filter_index(0,c_,2,2)]);
          y4 = _mm256_fmadd_ps(y0, y3, y4);
          if(c_+1==ca.C){
            _mm256_storeu_ps(output+output_index(0,h_-2,w_), y4);
            //print_m256(y4);
            y4 = _mm256_setzero_ps();
          }
        }
      }
      ///////////////////////////////////////////
    }
    printf("write to output\n");
  }
}

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
  int input_height = 10, input_width = 10, input_channel = 256, filter_batch = 1, kernel_width = 3, kernel_height = 3;
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

  fast_conv cw(ca, I, F, O);
  auto start = high_resolution_clock::now();
  cw.run();
  long long t = duration_cast<nanoseconds>((high_resolution_clock::now() - start)).count();
  cout << __FUNCTION__ << " | total algo Time: " << t << " ns" << endl;
  cw.print();
  _mm_free(I), _mm_free(F), _mm_free(O);
  return 0;
}

void print_output(float* Output, int h, int w, int output_channel, bool all) {
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
