//
// Created by henry on 1/30/23.
//

#include "wfh.h"
#include <memory.h>

// channel is removed
void conv_wfh::reorder_input() {
  core = new float[core_size]();
  float *tmp=core;
  int safe_batch = core_padding>0?core_batch-1:core_batch;//

  float* input_base = input+((ca.W+1)<<1);
  float* input_src;

  for(int i=0;i<core_h;i+=VL){ // height
    for(int j=0;j<safe_batch;j++){ // width
      auto tmp_base= input_base+i*ca.W+(j<<3);
      for(int c_=0;c_<core_c;c_++){ // channel
        input_src = tmp_base+c_*ori_channel_stride;
        ::memcpy(tmp, input_src, VL*sizeof(float));
        tmp+=VL;
      }
    }
    if (core_padding>0){
      auto tmp_base= input_base+i*ca.W+(safe_batch<<3);
      for(int c_=0;c_<core_c;c_++){ // channel
        input_src = tmp_base+c_*ori_channel_stride;
        ::memcpy(tmp, input_src, (VL-core_padding)*sizeof(float));
        tmp+=VL;
      }
    }
  }
}