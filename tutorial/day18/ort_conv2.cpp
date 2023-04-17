#include "ort_conv.h"

const int BlockSize = 8;  // for AVX2, YMM register is 8*32 = 256bits.
vector<float> BufferWorking;



void onnxruntime_conv_nchw(size_t BatchCount,
                           size_t GroupCount,
                           size_t InputChannels,
                           size_t InputHeight,
                           size_t InputWidth,
                           size_t FilterCount,
                           size_t KernelHeight,
                           size_t KernelWidth,
                           size_t PaddingLeftHeight,
                           size_t PaddingLeftWidth,
                           size_t PaddingRightHeight,
                           size_t PaddingRightWidth,
                           size_t DilationHeight,
                           size_t DilationWidth,
                           size_t StrideHeight,
                           size_t StrideWidth,
                           size_t OutputHeight,
                           size_t OutputWidth,
                           const float* Input,
                           const float* Filter,
                           const float* Bias,
                           float* Output)
{
  auto start = std::chrono::high_resolution_clock::now();
  int64_t InputShape[] = {int64_t(InputHeight), int64_t(InputWidth)};
  int64_t KernelShape[] = {int64_t(KernelHeight), int64_t(KernelWidth)};
  int64_t DilationShape[] = {int64_t(DilationHeight), int64_t(DilationWidth)};
  int64_t Padding[] = {int64_t(PaddingLeftHeight), int64_t(PaddingLeftWidth), int64_t(PaddingRightHeight), int64_t(PaddingRightWidth)};
  int64_t StrideShape[] = {int64_t(StrideHeight), int64_t(StrideWidth)};
  int64_t OutputShape[] = {int64_t(OutputHeight), int64_t(OutputWidth)};

  MLAS_ACTIVATION Activation;
  Activation.ActivationKind = MlasIdentityActivation;

  MLAS_CONV_PARAMETERS Parameters;
  size_t WorkingBufferSize;
  auto threadpool_ = nullptr;

  MlasConvPrepare(&Parameters,
                  2,
                  BatchCount,
                  GroupCount,
                  InputChannels,
                  InputShape,
                  KernelShape,
                  DilationShape,
                  Padding,
                  StrideShape,
                  OutputShape,
                  FilterCount,
                  &Activation,
                  &WorkingBufferSize,
                  0.0f,
                  threadpool_);

  BufferWorking.resize(WorkingBufferSize);
  auto t0 = std::chrono::high_resolution_clock::now();
  std::cout << __FUNCTION__ << " | MlasConvPrepare Time: " << std::chrono::duration_cast<std::chrono::microseconds>((t0 - start)).count() << " us" << std::endl;


  MlasConv(&Parameters,
           Input,
           Filter,
           Bias,
           BufferWorking.data(), // BufferWorking.GetBuffer(WorkingBufferSize),
           Output,
           threadpool_);
  auto t1 = std::chrono::high_resolution_clock::now();
  std::cout << __FUNCTION__ << " | Compute Time: " << std::chrono::duration_cast<std::chrono::microseconds>((t1 - start)).count() << " us" << std::endl;

}