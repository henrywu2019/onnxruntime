set -x

g++ main.cpp ort_conv.cpp -o main_release.exe -O3 -I/home/henry/wendy/git.repo/onnxruntime/include/ \
-L/home/henry/wendy/git.repo/onnxruntime/build/Linux/Release \
-L/home/henry/wendy/git.repo/onnxruntime/build/Linux/Release/_deps/google_nsync-build/ \
 -lonnxruntime_mlas -lonnxruntime_common -lnsync_cpp -std=c++11 -mavx2

# DegreeOfParallelism

#/home/henry/wendy/git.repo/mlas/build

#g++ main.cpp -I/home/henry/wendy/git.repo/mlas/inc \
#-L/home/henry/wendy/git.repo/mlas/build -L/home/henry/wendy/git.repo/onnxruntime/cmake/cmake-build-debug \
#-lonnxruntime_common -lonnxruntime_mlas -lonnxruntime_mlas_static -lgomp -std=c++11 -mavx2 -v