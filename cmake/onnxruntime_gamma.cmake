# Copyright (c) Microsoft Corporation. All rights reserved.
# Licensed under the MIT License.

file(GLOB onnxruntime_gamma_srcs CONFIGURE_DEPENDS
    "${ONNXRUNTIME_INCLUDE_DIR}/core/gamma/*.h"
    "${ONNXRUNTIME_ROOT}/core/gamma/*.h"
    "${ONNXRUNTIME_ROOT}/core/gamma/*.cc"
    )


source_group(TREE ${REPO_ROOT} FILES ${onnxruntime_gamma_srcs})

onnxruntime_add_static_library(onnxruntime_gamma ${onnxruntime_gamma_srcs})
install(DIRECTORY ${PROJECT_SOURCE_DIR}/../include/onnxruntime/core/gamma  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/onnxruntime/core)
onnxruntime_add_include_to_target(onnxruntime_gamma onnxruntime_common onnxruntime_framework onnx onnx_proto ${PROTOBUF_LIB} flatbuffers)
if(onnxruntime_ENABLE_INSTRUMENT)
  target_compile_definitions(onnxruntime_gamma PUBLIC ONNXRUNTIME_ENABLE_INSTRUMENT)
endif()

target_include_directories(onnxruntime_gamma PRIVATE ${ONNXRUNTIME_ROOT} ${eigen_INCLUDE_DIRS})
target_link_libraries(onnxruntime_gamma PRIVATE nlohmann_json::nlohmann_json)
if (onnxruntime_USE_EXTENSIONS)
  target_link_libraries(onnxruntime_gamma PRIVATE onnxruntime_extensions)
endif()
add_dependencies(onnxruntime_gamma ${onnxruntime_EXTERNAL_DEPENDENCIES})
set_target_properties(onnxruntime_gamma PROPERTIES FOLDER "ONNXRuntime")
if (onnxruntime_USE_CUDA)
  target_include_directories(onnxruntime_gamma PRIVATE ${onnxruntime_CUDNN_HOME}/include ${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES})
endif()
if (onnxruntime_USE_ROCM)
  target_compile_options(onnxruntime_gamma PRIVATE -Wno-sign-compare -D__HIP_PLATFORM_HCC__=1)
  target_include_directories(onnxruntime_gamma PRIVATE ${onnxruntime_ROCM_HOME}/hipfft/include ${onnxruntime_ROCM_HOME}/include ${onnxruntime_ROCM_HOME}/hipcub/include ${onnxruntime_ROCM_HOME}/hiprand/include ${onnxruntime_ROCM_HOME}/rocrand/include)
# ROCM provider sources are generated, need to add include directory for generated headers
  target_include_directories(onnxruntime_gamma PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/amdgpu/onnxruntime ${CMAKE_CURRENT_BINARY_DIR}/amdgpu/orttraining)
endif()
if (onnxruntime_ENABLE_TRAINING OR onnxruntime_ENABLE_TRAINING_OPS)
  target_include_directories(onnxruntime_gamma PRIVATE ${ORTTRAINING_ROOT})
endif()

if (onnxruntime_ENABLE_TRAINING_TORCH_INTEROP)
  onnxruntime_add_include_to_target(onnxruntime_gamma Python::Module) 
endif()
