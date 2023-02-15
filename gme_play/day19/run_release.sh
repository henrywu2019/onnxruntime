set -x


g++ fast_conv.cpp -o fast_conv.exe -O3 -std=c++20 -mavx2 -march=native

# DegreeOfParallelism

#/home/henry/wendy/git.repo/mlas/build

#g++ main.cpp -I/home/henry/wendy/git.repo/mlas/inc \
#-L/home/henry/wendy/git.repo/mlas/build -L/home/henry/wendy/git.repo/onnxruntime/cmake/cmake-build-debug \
#-lonnxruntime_common -lonnxruntime_mlas -lonnxruntime_mlas_static -lgomp -std=c++11 -mavx2 -v
