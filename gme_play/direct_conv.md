# High Performance Direct Convolution Without Runtime Packing and Reordering On CPUs


## Abstract


Convolution is one of the most computationally intensive operations that must be performed for machine-learning model inference. One traditional approach to compute convolutions is known as the Im2Col + BLAS method, which incurs extra memory overhead. Another approach is to reorder the input and then do direction convolution with another reordering as a postprocessing. This approach requires runtime input and output data reordering, so increases peak memory usage and introduces extra complexities into the neural network. This paper demonstrates a new direct convolution method without runtime packing and reordering with optimal register allocation strategy which dramatically reduces memory and latency for inference on consumer grade CPUs.
