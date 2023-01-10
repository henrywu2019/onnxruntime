#include <algorithm>  // std::generate
#include <assert.h>
#include <iostream>
#include <sstream>
#include <vector>
#include "/home/henry/wendy/git.repo/onnxruntime/include/onnxruntime/core/session/onnxruntime_cxx_api.h"

// pretty prints a shape dimension vector
std::string print_shape(const std::vector<int64_t>& v) {
  std::stringstream ss("");
  for (size_t i = 0; i < v.size() - 1; i++)
    ss << v[i] << "x";
  ss << v[v.size() - 1];
  return ss.str();
}

int calculate_product(const std::vector<int64_t>& v) {
  int total = 1;
  for (auto& i : v) total *= i;
  return total;
}

using namespace std;

int main(int argc, char** argv) {
  // onnxruntime setup
  Ort::Env env(ORT_LOGGING_LEVEL_WARNING, "example-model-explorer");
  Ort::SessionOptions session_options;
  Ort::Session session = Ort::Session(env,"lr_init.onnx",session_options);  // access experimental components via the Experimental namespace

  try {
    Ort::RunOptions run_options;
    const char* input_names[] = {"X"};
    const char* output_names[] = {"Y"};

    std::array<int64_t, 4> input_shape_{1, 1, 2, 1};
    Ort::Value::CreateTensor<float>(memory_info, input_image_.data(), input_image_.size(), input_shape_.data(), input_shape_.size());
    auto output_tensors = session.Run(run_options, input_names);
    cout << "done" << endl;

    // double-check the dimensions of the output tensors
    // NOTE: the number of output tensors is equal to the number of output nodes specifed in the Run() call
    assert(output_tensors.size() == session.GetOutputNames().size() &&
           output_tensors[0].IsTensor());
    cout << "output_tensor_shape: " << print_shape(output_tensors[0].GetTensorTypeAndShapeInfo().GetShape()) << endl;

  } catch (const Ort::Exception& exception) {
    cout << "ERROR running model inference: " << exception.what() << endl;
    exit(-1);
  }
}