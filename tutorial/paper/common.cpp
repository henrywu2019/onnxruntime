#include "common.h"


int print_peak_mem() {
  struct rusage usage;
  getrusage(RUSAGE_SELF, &usage);
  cout << "Peak memory usage: " << usage.ru_maxrss << " KB" << endl;
  return usage.ru_maxrss;
}

void get_input(float* l, int n, int c, int w, int h, float channel_delta, float cell_delta, float batch_delta) {
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

void print_output(float* Output, int h, int w, int output_channel, bool all) {
  if (all or h * w < 50 * 50) {
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
  printf("...\n");
  for (int t = output_channel * w * h - 17; t < output_channel * w * h; t++) {
    printf("%.2f,", Output[t]);
    if (t % 16 == 10) printf("\n");
  }
  printf("\b \n");
}


void append_file(const string& s){
  ofstream outfile("result.csv", ios_base::app);
  if (!outfile.is_open()) {
    cout << "Failed to open file\n";
    return;
  }
  outfile << s;
  outfile.close();
}
