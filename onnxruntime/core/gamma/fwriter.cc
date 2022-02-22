#include "core/gamma/fwriter.h"

#include "core/gamma/gme.h"

namespace gme {


void fwriter::dump() {
  auto end = std::chrono::steady_clock::now();
  std::chrono::duration<double> diff = end - last;
  if (diff.count() > 10.0) {
    if (force_write())
      last = end;
  }
}

// clear cache if there are 10MB+ data in memory
bool fwriter::force_write() {
  if (file_path.size() > 5 and !perf_data.empty()) {
    if (!opened) {
      l.open(file_path, std::ios::app);
      l << "time,model,node,cpu,rss_delta,rss,node_input\n";
      opened = true;
    }
    l << perf_data;
    perf_data.clear();
    return true;
  }
  return false;
}

void fwriter::put(const std::string& s) {
  perf_data += s;
  // if data is 50MB+, force to write it to file
  if (perf_data.size() > 1024 * 1024 * 50) {
    force_write();
  }
}

}  // namespace gme