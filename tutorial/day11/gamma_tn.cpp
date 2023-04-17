#include <iostream>
#include <cstring>
#include <fstream>
#include <thread>
#include <limits>

int32_t gamma_tn() {
  static int32_t r = -1;
  if (r > 0) return r;

  auto x = [](const char* s) {
    int32_t t = 0;
    char c[256] = {};
    sprintf(c, "/sys/fs/cgroup/cpu/cpu.cfs_%s_us", s);
    std::fstream x(c, std::ios_base::in);
    x >> t;
    if (t == 0) return int32_t(std::thread::hardware_concurrency());
    return t;
  };
  auto n = x("quota");
  return r = (n >> 31) ? 0x30 : n / x("period");
}



