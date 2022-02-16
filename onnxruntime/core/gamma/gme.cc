#include "core/gamma/gme.h"
#include <stdlib.h>
#include <filesystem>
namespace fs = std::filesystem;

namespace gme {

// input: home/opc/workspace/vision_service_ocr_models/ocr_onnx/../mbin/det_doc/model.onnx
// output: det_doc
// input: home/opc/workspace/vision_service_ocr_models/ocr_onnx/../det_doc/mbin/model.onnx
// output: det_doc
std::string get_model_name(const std::string& s, const std::string& fallback) {
  char actualpath[4097];
  std::string model_location_(realpath(s.c_str(), actualpath));

  std::string model_name = fallback;
  unsigned long i = model_location_.rfind("/");
  if (i != std::string::npos) {
    std::string tmp = model_location_.substr(0, i);
    i = tmp.rfind("/");
    if (i != std::string::npos) {
      model_name = tmp.substr(i + 1);
      if (model_name == "mbin") {
        tmp = tmp.substr(0, i);
        i = tmp.rfind("/");
        if (i != std::string::npos) {
          model_name = tmp.substr(i + 1);
        }
      }
    }
  }
  return model_name;
}

void ensure_folder(const std::string& folder){
  if (!fs::is_directory(folder)){
    fs::create_directories(folder);
  }
}

bool exists(const std::string& f) {
  return fs::exists(f);
}

}  // namespace gme
