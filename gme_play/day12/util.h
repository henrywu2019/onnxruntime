//
// Created by henry on 1/30/23.
//

#ifndef ONNXRUNTIME_UTIL_H
#define ONNXRUNTIME_UTIL_H
#include <bits/stdc++.h>

using namespace std;

void get_input(vector<float>& l, int n, int c, int w, int h, float channel_delta=0, float cell_delta=1) {
  if ((int)l.size() < n * c * w * h) l.resize(n * c * w * h);
  float start = 1.;
  for (int i = 0; i < n; i++){
    for (int j = 0; j < c; j++) {
#ifdef RANDDATA
      start = start/(abs(int(start))+1) + ((double) rand() / (RAND_MAX)) + 0.1; if(rand()&1) start*=-1;
#else
      start = 1+i*0.1+j*channel_delta;
#endif
      for (int m = 0; m < h; m++)
        for (int k = 0; k < w; k++) {
          int idx = k + m * w + h * w * j + h * w * c * i;
#ifdef RANDDATA
          if(rand()&1) start*=-1;
          l[idx] = start, start += 0.01;
#else
          l[idx] = start, start += cell_delta;
#endif
        }
    }
  }
}


#endif  // ONNXRUNTIME_UTIL_H
