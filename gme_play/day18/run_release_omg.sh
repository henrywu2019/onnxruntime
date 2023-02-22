set -x

nasm -felf64 block_compute.asm -o compute.o

ROOT=/home/henry/wendy/git.repo/onnxruntime
ROOT=/home/opc/mlops/onnxruntime

g++ compute.o main.cpp ort_conv.cpp ort_conv2.cpp -o main_release.exe -O3 -I${ROOT}/gme_play/gamma/include \
-L$ROOT/build/Linux/Release \
-L$ROOT/build/Linux/Release/_deps/google_nsync-build \
 -lgamma -lonnxruntime_mlas -lonnxruntime_common -lnsync_cpp -lpthread -std=c++20 -mavx2 -march=native

# DegreeOfParallelism

#/home/henry/wendy/git.repo/mlas/build

#g++ main.cpp -I/home/henry/wendy/git.repo/mlas/inc \
#-L/home/henry/wendy/git.repo/mlas/build -L/home/henry/wendy/git.repo/onnxruntime/cmake/cmake-build-debug \
#-lonnxruntime_common -lonnxruntime_mlas -lonnxruntime_mlas_static -lgomp -std=c++11 -mavx2 -v
