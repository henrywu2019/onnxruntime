#include "/home/henry/wendy/git.repo/onnxruntime/onnxruntime/core/gamma/npy.h"
#include <vector>
#include <string>
#include <iostream>

using namespace std;

// 5683200, shape:1,3,1600,1184,

int main() {
  std::vector<unsigned long> shape {};
  bool fortran_order=false;
  std::vector<float> data;
  
  const std::string path {"/home/henry/wendy/git.repo/onnxruntime/gme_play/x.npy"};
  npy::LoadArrayFromNumpy(path, shape, fortran_order, data);

  cout << data.size() << ", shape:";
  for(auto i: shape) cout << i << ",";
  cout<<endl;
}