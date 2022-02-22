#include "core/gamma/fwriter.h"
#include "core/gamma/env.h"
#include "core/gamma/gme.h"

namespace gme {
static const std::string bm_log_folder(StringFromEnv("BM_LOG_FOLDER", "/tmp"));
static const std::string bm_log_filename(StringFromEnv("BM_LOG_FILENAME", "bm.log"));

void fwriter::dump() {
  auto end = std::chrono::steady_clock::now();
  std::chrono::duration<double> diff = end - last;
  if (diff.count() > 10.0) {
    std::string file_path = bm_log_folder + "/" + bm_log_filename;
    if (file_path.size() > 5) {
      if (!opened) {
        l.open(file_path, std::ios::app);
        opened = true;
      }
      l << perf_data;
      perf_data.clear();
      last = end;
    }
  }
}

// clear cache if there are 10MB+ data in memory
void fwriter::force_write() {
  if (opened and !perf_data.empty()) {
    l << perf_data;
    perf_data.clear();
  }
}

void fwriter::put(const std::string& s) {
  perf_data += s;

  // if data is 50MB+, force to write it to file
  if (perf_data.size() > 1024 * 1024 * 50) {
    force_write();
  }
}

}  // namespace gme