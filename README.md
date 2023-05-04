- Cut off for AMD_ARM precision comparison



```
./build.sh --config Debug --enable_pybind --build_wheel --parallel --build_shared_lib --cmake_extra_defines onnxruntime_DEBUG_NODE_INPUTS_OUTPUTS=1 onnxruntime_DEBUG_NODE_INPUTS_OUTPUTS_ENABLE_DUMP_TO_SQLDB=1 --skip_tests --skip_onnx_tests
```



```
orttraining/tools/scripts/sqldb_to_tensors.py
```
