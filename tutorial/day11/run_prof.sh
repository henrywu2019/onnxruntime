set -x


#g++ -pg main.cpp ort_conv.cpp ort_conv2.cpp -o main_prof.exe -O3 -I/home/henry/wendy/git.repo/onnxruntime/include/ \
#-L/home/henry/wendy/git.repo/onnxruntime/cmake/cmake-build-debug/Release \
#-L/home/henry/wendy/git.repo/onnxruntime/cmake/cmake-build-debug/_deps/google_nsync-build/ \
# -lonnxruntime_mlas -lonnxruntime_common -lnsync_cpp -std=c++11 -mavx2 -march=native

g++ -pg main.cpp ort_conv.cpp ort_conv2.cpp -o main_prof.exe -O3 -I/home/opc/workspace/onnxruntime/include/ \
-L/home/opc/workspace/onnxruntime/cmake/cmake-build-debug/Release \
-L/home/opc/workspace/onnxruntime/cmake/cmake-build-debug/_deps/google_nsync-build/ \
 -lonnxruntime_mlas -lonnxruntime_common -lnsync_cpp -std=c++11 -mavx2 -march=native \
 -lbenchmark -fuse-linker-plugin -mtune=native

# DegreeOfParallelism

#/home/henry/wendy/git.repo/mlas/build

#g++ main.cpp -I/home/henry/wendy/git.repo/mlas/inc \
#-L/home/henry/wendy/git.repo/mlas/build -L/home/henry/wendy/git.repo/onnxruntime/cmake/cmake-build-debug \
#-lonnxruntime_common -lonnxruntime_mlas -lonnxruntime_mlas_static -lgomp -std=c++11 -mavx2 -v
