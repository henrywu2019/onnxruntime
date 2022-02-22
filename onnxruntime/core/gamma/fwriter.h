// https://codereview.stackexchange.com/questions/173929/modern-c-singleton-template
#pragma once

#include "core/gamma/env.h"
#include <chrono>
#include <vector>
#include <string>
#include <fstream>

namespace gme {

static const std::string bm_log_folder(StringFromEnv("BM_LOG_FOLDER", "/tmp"));
static const std::string bm_log_filename(StringFromEnv("BM_LOG_FILENAME", "bm.log"));
static const std::string file_path = bm_log_folder + "/" + bm_log_filename;

class fwriter {
 private:
  fwriter() {
    last = std::chrono::steady_clock::now();
    opened = false;
  }  // Disallow instantiation outside of the class.

  ~fwriter() {
    force_write();
    if (opened){
      l.close();
      opened = false;
    }
  }
  std::chrono::time_point<std::chrono::steady_clock> last;
  std::string perf_data;
  std::ofstream l;
  bool opened;

 public:
  fwriter(const fwriter&) = delete;
  fwriter& operator=(const fwriter&) = delete;
  fwriter(fwriter&&) = delete;
  fwriter& operator=(fwriter&&) = delete;

  void dump();

  void put(const std::string& s);

  bool force_write();

  static auto& instance() {
    static fwriter i;
    return i;
  }
};

}  // namespace gme
