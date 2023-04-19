# Zero-Overhead High Performance Direct Convolution Without Runtime Packing and Reordering On CPUs

## Abstract

Convolution is one of the most computationally intensive operations that must be performed for machine-learning model inference. One traditional approach to compute convolutions is known as the Im2Col + BLAS method, which incurs extra memory overhead. Another approach is to reorder the input and then do direction convolution with another reordering as a postprocessing. This approach requires runtime input and output data reordering, so increases peak memory usage and introduces extra complexities into the neural network. This paper demonstrates a new direct convolution method without runtime packing and reordering with optimal register allocation strategy which dramatically reduces memory and latency for inference on consumer grade CPUs.

## Introduction

Convolutional Neural Networks (CNNs) are a class of deep neural networks that are widely used in computer vision applications such as image and video recognition, object detection, and segmentation. At a high level, the numerical algorithm used in CNN computation involves performing a series of matrix multiplications or convolutions between the input data and learnable weights (aka filters or kernels) of the network. The output of each layer is then passed through an activation function and fed into the next layer. In all the layers, convolution layer is the most computationally intensive operations, so optimizing convolution is critical to improve inference performance. A significant restriction of CNN models on mobile and low-power devices is the considerable computational burden associated with consecutive convolutional layers wher high-end expensive GPU is unavailble. Even for most AI services providers, CPU serving is still a pragmatic solution due to the cost control pressure.


There are many ways to do convolution, but GEMM and direct convolution are the most widely studies and adopted in the industry and academia. GEMM has a major drawback of big memory overhead. The process involves packing overlapping image blocks, whose sizes match those of the kernel, into the columns of a sizeable temporary matrix. This extra packing operation and memory overhead has been a big problem in many use cases, so that direct convolution is proposed as a more efficient algorithm. The exisitng direct convolution algorithm doesn't need data packing but it need runtime input and output data reordering. Therefore, it is not real zero-memory overhead because the temporary memory used in the reordering process is the same as input data size, so it is not negligible. The reordering also adds extra complexity to the neural networks because the output reordering, as the next layer's input, need to be considered. Furthermore, the existing direct convolution involves several hyperparameters such as input block size, kernel thunk size, output block size, making the implementation even more complicated and error-prone.


## Background

OCR model has many receipts input where height is much longer than width.


## Zero-Overhead Direct Convolution

This paper presents a simplified method for performing direct convolution on consumer-grade CPUs without requiring packing or reordering. The input data layout is NCHW, which conforms to the default in PyTorch and ONNX protocol. We describe the algorithm without CPU vector extension support, followed by the optimal register allocation algorithm that employs CPU Fused-Multiply-Add (FMA) instructions. Additionally, we present experimental results that demonstrate on-par or superior performance with less memory consumption.

### ZPZR Direct Convolution without FMA

```c++
for (int co = 0; co < Co; co++) {
  for (int channel = 0; channel < Ci; channel++) {
    reset(p, F, O);
    for (int k = 0; k < 3; k++) {
      for (int j = 0; j < len; j++)
          O[j] += *(p[0] + j) * F[k];
      for (int j = 0; j < len; j++) {
          auto z1 = *(p[1] + j);
          O[j] += z1 * F[k + 3];
          O[len + j] += z1 * F[k];
      }
      for (int i = 2; i < ih - 2; i++) {
          for (int j = 0; j < len; j++) {
          auto z1 = *(p[i] + j);
          auto idx = i * len + j;
          O[idx - 2 * len] += z1 * F[k + 6];
          O[idx - len] += z1 * F[k + 3];
          O[idx] += z1 * F[k];
          }
      }
      for (int j = 0; j < len; j++) {
          auto z1 = *(p[ih - 2] + j);
          auto idx = (ih - 2) * len + j;
          O[idx - 2 * len] += z1 * F[k + 6];
          O[idx - len] += z1 * F[k + 3];
      }
      for (int j = 0; j < len; j++) {
          auto z1 = *(p[ih - 1] + j);
          auto idx = (ih - 1) * len + j;
          O[idx - 2 * len] += z1 * F[k + 6];
      }
      advance(); // window sliding down
    }
  }
}
```

Real zero-memory overhead.

### ZPZR Direct Convolution with FMA

In AVX, FMA instructions can be used for the 16 256-bit YMM register files including Y0 - Y15. For a 3 by 3 kernel, the best register allocation strategy is to use 1 YMM register Y0 for input, 1 register Y1 to load kernel data, 4 x 3 = 12 registers to store the FMA results. For a 2x2 kernel, the best strategy is Y0 for input data, Y1 for kernel, 7x2=14 registers to store the FMA results by kernel's batch. If the total register number is `R`, then we can process `(R-1)//K_h` batch kernel every time.


The algorithm will be:

```
REP(){
    REP(){
        load y0 from input_data
        REP(kernel batch i: 0-4)
            y1 <- kernel[i]
            y4 <- fma(y0, y1)
    }
}
```
