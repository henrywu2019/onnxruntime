# Survey Of Convolution Algorithm 

## Algorithms

- NCHWc - NCHWc
- NCHW - NCHWc
- GEMM-based NCHW
- SMM


## Testing Result

```
filter size: 2304
input total size: 1024.00KB
onnxruntime_conv_nchwc | Reorder Time: 1944 us

MLAS_NCHWC_CONV_NCHWC_ALGORITHM kernel was called: 1984 times.
onnxruntime_conv_nchwc | Compute Time: 4707 us


onnxruntime_conv_nchw | Compute Time: 2327 us


area:3844,output_h:62,output_w:62,size:15376 Byte
gme_conv | Compute Time: 1022 us
extract time: 46 us
```