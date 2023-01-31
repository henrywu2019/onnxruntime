#ifndef ONNXRUNTIME_WFH_H
#define ONNXRUNTIME_WFH_H

const int VL=8;//vectorization length
struct conv_attr{
  int N,C,H,W;   // input
  int K,R,L;   // kernel
  int OH,OW;   // output
  conv_attr(int N_,int C_,int H_,int W_,int K_,int R_,int L_):N(N_),C(C_),H(H_),W(W_),K(K_),R(R_),L(L_){
    OH = H-R+1;
    OW=W-L+1;
  }
};

struct conv_wfh{ // can refactor using inheritance
  conv_attr ca;
  // core attr
  int core_h, core_w, core_c;
  int core_w2; // breadth after padding = core_batch*VL
  int core_size;
  int core_batch; // = ceil(core_w/VL)
  int core_padding=0; // = core_w2 - core_w

  int ori_channel_stride;
  // original memory store
  float *input;
  float *kernel;
  float *output;
  // wfh Algo memory store
  float* core;
  float* o;
  float* p;


  conv_wfh(conv_attr ca_, float* input_, float* kernel_, float* output_):ca(ca_),input(input_), kernel(kernel_), output(output_){
    core_h = ca.H-4;
    core_w = ca.W-4;
    core_c = ca.C;
    core_batch = core_w%VL==0 ? core_w/VL: int(float(core_w)/VL+1.0);
    core_w2 = core_batch*VL;
    core_size = core_h*core_w2*core_c;
    core_padding = core_w2 - core_w;
    ori_channel_stride = ca.H*ca.W;
  }

  void reorder_input();

};

#endif  // ONNXRUNTIME_WFH_H
