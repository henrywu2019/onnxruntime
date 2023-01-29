//
// Created by henry on 1/21/23.
//
#include "ort_conv.h"

const int BlockSize = 8;  // for AVX2, YMM register is 8*32 = 256bits.
vector<float> BufferNchwcFilter, BufferNchwcBias, BufferNchwcInput, BufferNchwcOutput;

void ReorderInputNchw(const int64_t* input_shape, const float* S, float* D) {
  const int64_t nchwc_block_size = static_cast<int64_t>(MlasNchwcGetBlockSize());
  int64_t batch_count = input_shape[0];
  int64_t channel_count = input_shape[1];
  int64_t nchwc_channel_count = (channel_count + nchwc_block_size - 1) & ~(nchwc_block_size - 1);
  int64_t spatial_count = input_shape[2] * input_shape[3];
  for (int64_t n = 0; n < batch_count; n++) {
    MlasReorderInputNchw(S, D, static_cast<size_t>(channel_count), static_cast<size_t>(spatial_count));
    S += spatial_count * channel_count;
    D += spatial_count * nchwc_channel_count;
  }
}



void onnxruntime_conv_nchwc(
    size_t BatchCount,
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
    float* Output) {
  auto start = std::chrono::high_resolution_clock::now();
  int64_t InputShape[] = {int64_t(BatchCount), int64_t(GroupCount) * int64_t(InputChannels), int64_t(InputHeight), int64_t(InputWidth)};
  int64_t FilterShape[] = {int64_t(GroupCount) * int64_t(FilterCount), int64_t(InputChannels), int64_t(KernelHeight), int64_t(KernelWidth)};
  int64_t OutputShape[] = {int64_t(BatchCount), int64_t(GroupCount) * int64_t(FilterCount), int64_t(OutputHeight), int64_t(OutputWidth)};

  int64_t KernelShape[] = {int64_t(KernelHeight), int64_t(KernelWidth)};
  int64_t DilationShape[] = {int64_t(DilationHeight), int64_t(DilationWidth)};
  int64_t Padding[] = {int64_t(PaddingLeftHeight), int64_t(PaddingLeftWidth), int64_t(PaddingRightHeight), int64_t(PaddingRightWidth)};
  int64_t StrideShape[] = {int64_t(StrideHeight), int64_t(StrideWidth)};

  //
  // Select the type of convolution that will be performed.
  //

  bool DoReorderInput;
  bool ReorderFilterOIHWBo;

  if (GroupCount > 1 && InputChannels == 1 && FilterCount == 1) {
    // Depthwise convolution.
    DoReorderInput = true;
    ReorderFilterOIHWBo = true;
  } else if (InputChannels >= BlockSize) {
    // NCHWc or pointwise convolution;
    DoReorderInput = true;
    ReorderFilterOIHWBo = false;
  } else {
    // NCHW convolution.
    DoReorderInput = false;
    ReorderFilterOIHWBo = true;
  }

  size_t NchwcInputChannels = (GroupCount * InputChannels + BlockSize - 1) & ~(BlockSize - 1);
  size_t NchwcOutputChannels = (GroupCount * FilterCount + BlockSize - 1) & ~(BlockSize - 1);

  //
  // Reorder the filter buffer as needed.
  //

  float* ReorderedFilter;

  if (ReorderFilterOIHWBo) {
    size_t NchwcFilterElements = NchwcOutputChannels * InputChannels * KernelHeight * KernelWidth;
    BufferNchwcFilter.resize(NchwcFilterElements);
    ReorderedFilter = BufferNchwcFilter.data();
    MlasReorderFilterOIHWBo(FilterShape, Filter, ReorderedFilter);
  } else {
    size_t NchwcFilterElements = NchwcOutputChannels * NchwcInputChannels * KernelHeight * KernelWidth;
    BufferNchwcFilter.resize(NchwcFilterElements);
    ReorderedFilter = BufferNchwcFilter.data();
    MlasReorderFilterOIHWBiBo(FilterShape, Filter, ReorderedFilter);
  }

  //
  // Align the bias buffer to the filter count if needed.
  //

  if (Bias != nullptr && GroupCount * FilterCount < NchwcOutputChannels) {
    BufferNchwcBias.resize(NchwcOutputChannels);
    float* AlignedBias = BufferNchwcBias.data();

    size_t i;
    for (i = 0; i < GroupCount * FilterCount; i++) {
      AlignedBias[i] = Bias[i];
    }
    for (; i < NchwcOutputChannels; i++) {
      AlignedBias[i] = 0.0f;
    }

    Bias = AlignedBias;
  }

  //
  // Reorder the input buffer if needed.
  //

  if (DoReorderInput) {
    size_t NchwcInputElements = BatchCount * NchwcInputChannels * InputHeight * InputWidth;
    BufferNchwcInput.resize(NchwcInputElements);
    float* NchwcInput = BufferNchwcInput.data();
    ReorderInputNchw(InputShape, Input, NchwcInput);
    Input = NchwcInput;
    InputShape[1] = NchwcInputChannels;
  }

  int64_t NchwcOutputShape[] = {int64_t(BatchCount), int64_t(NchwcOutputChannels), int64_t(OutputHeight), int64_t(OutputWidth)};

  size_t NchwcOutputElements = BatchCount * NchwcOutputChannels * OutputHeight * OutputWidth;
  BufferNchwcOutput.resize(NchwcOutputElements);
  float* NchwcOutput = BufferNchwcOutput.data();

  MLAS_ACTIVATION Activation;
  Activation.ActivationKind = MlasIdentityActivation;
  //auto t0 = std::chrono::high_resolution_clock::now();
  //std::cout << __FUNCTION__ << " | Reorder Time: " << std::chrono::duration_cast<std::chrono::nanoseconds>((t0 - start)).count() << " ns" << std::endl;

  MlasNchwcConv(InputShape,
                KernelShape,
                DilationShape,
                Padding,
                StrideShape,
                NchwcOutputShape,
                GroupCount,
                Input,
                ReorderedFilter,
                Bias,
                NchwcOutput,
                &Activation,
                true,
                nullptr);

  //
  // Reorder the output buffer.
  //

  MlasReorderOutputNchw(OutputShape, NchwcOutput, Output);
  auto t1 = std::chrono::high_resolution_clock::now();
  std::cout << __FUNCTION__ << " | Compute Time: " << std::chrono::duration_cast<std::chrono::nanoseconds>((t1 - start)).count() << " ns" << std::endl;

}