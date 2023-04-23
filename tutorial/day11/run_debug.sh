set -x


g++ -g main_v2.cpp ort_conv.cpp ort_conv2.cpp -o main_debug.exe -O0 -I/home/opc/workspace/onnxruntime/include/ \
-L/home/opc/workspace/onnxruntime/build/Linux/Debug/ \
-L/home/opc/workspace/onnxruntime/build/Linux/Debug/_deps/google_nsync-build \
    -lonnxruntime_mlas -lonnxruntime_common -lnsync_cpp -std=c++20 -mavx2 -march=native \
    -fuse-linker-plugin -mtune=native -lpthread

# DegreeOfParallelism

#/home/henry/wendy/git.repo/mlas/build

#g++ main.cpp -I/home/henry/wendy/git.repo/mlas/inc \
#-L/home/henry/wendy/git.repo/mlas/build -L/home/henry/wendy/git.repo/onnxruntime/cmake/cmake-build-debug \
#-lonnxruntime_common -lonnxruntime_mlas -lonnxruntime_mlas_static -lgomp -std=c++11 -mavx2 -v
