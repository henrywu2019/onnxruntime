cmake_minimum_required(VERSION 3.10)

project(gme VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options(-mavx2 -march=native)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")


#target_include_directories(gme PRIVATE /home/henry/wendy/git.repo/onnxruntime/include/)
add_executable(gme_day9 main.cpp ort_conv.cpp ort_conv2.cpp ort_conv.h)

target_link_libraries(gme_day9
        -L/home/henry/wendy/git.repo/onnxruntime/build/Linux/Release
        -L/home/henry/wendy/git.repo/onnxruntime/build/Linux/Release/_deps/google_nsync-build/
        -lonnxruntime_mlas -lonnxruntime_common -lnsync_cpp)