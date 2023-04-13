#include "core/oracle/gme.h"
#include <iostream>
#include <cstring>
#include <fstream>
#include <limits>

extern "C" int64_t _gamma_tn();

namespace gme {

// Parses 'str' for a 32-bit signed integer.  If successful, writes
// the result to *value and returns true; otherwise leaves *value
// unchanged and returns false.
bool ParseInt32(const std::string& src_text, const char* str, int32_t* value) {
  // Parses the environment variable as a decimal integer.
  char* end = nullptr;
  const long long_value = strtol(str, &end, 10);  // NOLINT

  // Has strtol() consumed all characters in the string?
  if (*end != '\0') {
    // No - an invalid character was encountered.
    std::cerr << src_text << " is expected to be a 32-bit integer, "
              << "but actually has value \"" << str << "\".\n";
    return false;
  }

  // Is the parsed value in the range of an Int32?
  const int32_t result = static_cast<int32_t>(long_value);
  if (long_value == std::numeric_limits<long>::max() ||
      long_value == std::numeric_limits<long>::min() ||
      // The parsed value overflows as a long.  (strtol() returns
      // LONG_MAX or LONG_MIN when the input overflows.)
      result != long_value
      // The parsed value overflows as an Int32.
  ) {
    std::cerr << src_text << " is expected to be a 32-bit integer, "
              << "but actually has value \"" << str << "\", "
              << "which overflows.\n";
    return false;
  }

  *value = result;
  return true;
}

// Parses 'str' for a double.  If successful, writes the result to *value and
// returns true; otherwise leaves *value unchanged and returns false.
bool ParseDouble(const std::string& src_text, const char* str, double* value) {
  // Parses the environment variable as a decimal integer.
  char* end = nullptr;
  const double double_value = strtod(str, &end);  // NOLINT

  // Has strtol() consumed all characters in the string?
  if (*end != '\0') {
    // No - an invalid character was encountered.
    std::cerr << src_text << " is expected to be a double, "
              << "but actually has value \"" << str << "\".\n";
    return false;
  }

  *value = double_value;
  return true;
}

// Returns the name of the environment variable corresponding to the
// given flag.  For example, FlagToEnvVar("foo") will return
// "BENCHMARK_FOO" in the open-source version.
static std::string FlagToEnvVar(const char* flag) {
  const std::string flag_str(flag);

  std::string env_var;
  for (size_t i = 0; i != flag_str.length(); ++i)
    env_var += static_cast<char>(::toupper(flag_str.c_str()[i]));

  return "GME_" + env_var;
}

// Reads and returns the Boolean environment variable corresponding to
// the given flag; if it's not set, returns default_value.
//
// The value is considered true iff it's not "0".
bool BoolFromEnv(const char* flag, bool default_value) {
  const std::string env_var = FlagToEnvVar(flag);
  const char* const string_value = getenv(env_var.c_str());
  return string_value == nullptr ? default_value
                                 : strcmp(string_value, "0") != 0;
}

// Reads and returns a 32-bit integer stored in the environment
// variable corresponding to the given flag; if it isn't set or
// doesn't represent a valid 32-bit integer, returns default_value.
int32_t Int32FromEnv(const char* flag, int32_t default_value) {
  const std::string env_var = FlagToEnvVar(flag);
  const char* const string_value = getenv(env_var.c_str());
  if (string_value == nullptr) {
    // The environment variable is not set.
    return default_value;
  }

  int32_t result = default_value;
  if (!ParseInt32(std::string("Environment variable ") + env_var, string_value,
                  &result)) {
    std::cout << "The default value " << default_value << " is used.\n";
    return default_value;
  }

  return result;
}

// Reads and returns the string environment variable corresponding to
// the given flag; if it's not set, returns default_value.
const char* StringFromEnv(const char* flag, const char* default_value) {
  const std::string env_var = FlagToEnvVar(flag);
  const char* const value = getenv(env_var.c_str());
  return value == nullptr ? default_value : value;
}

// Parses a string as a command line flag.  The string should have
// the format "--flag=value".  When def_optional is true, the "=value"
// part can be omitted.
//
// Returns the value of the flag, or nullptr if the parsing failed.
const char* ParseFlagValue(const char* str, const char* flag,
                           bool def_optional) {
  // str and flag must not be nullptr.
  if (str == nullptr || flag == nullptr) return nullptr;

  // The flag must start with "--".
  const std::string flag_str = std::string("--") + std::string(flag);
  const size_t flag_len = flag_str.length();
  if (strncmp(str, flag_str.c_str(), flag_len) != 0) return nullptr;

  // Skips the flag name.
  const char* flag_end = str + flag_len;

  // When def_optional is true, it's OK to not have a "=value" part.
  if (def_optional && (flag_end[0] == '\0')) return flag_end;

  // If def_optional is true and there are more characters after the
  // flag name, or if def_optional is false, there must be a '=' after
  // the flag name.
  if (flag_end[0] != '=') return nullptr;

  // Returns the string after "=".
  return flag_end + 1;
}

bool ParseBoolFlag(const char* str, const char* flag, bool* value) {
  // Gets the value of the flag as a string.
  const char* const value_str = ParseFlagValue(str, flag, true);

  // Aborts if the parsing failed.
  if (value_str == nullptr) return false;

  // Converts the string value to a bool.
  *value = IsTruthyFlagValue(value_str);
  return true;
}

bool ParseInt32Flag(const char* str, const char* flag, int32_t* value) {
  // Gets the value of the flag as a string.
  const char* const value_str = ParseFlagValue(str, flag, false);

  // Aborts if the parsing failed.
  if (value_str == nullptr) return false;

  // Sets *value to the value of the flag.
  return ParseInt32(std::string("The value of flag --") + flag, value_str,
                    value);
}

bool ParseDoubleFlag(const char* str, const char* flag, double* value) {
  // Gets the value of the flag as a string.
  const char* const value_str = ParseFlagValue(str, flag, false);

  // Aborts if the parsing failed.
  if (value_str == nullptr) return false;

  // Sets *value to the value of the flag.
  return ParseDouble(std::string("The value of flag --") + flag, value_str,
                     value);
}

bool ParseStringFlag(const char* str, const char* flag, std::string* value) {
  // Gets the value of the flag as a string.
  const char* const value_str = ParseFlagValue(str, flag, false);

  // Aborts if the parsing failed.
  if (value_str == nullptr) return false;

  *value = value_str;
  return true;
}

bool IsFlag(const char* str, const char* flag) {
  return (ParseFlagValue(str, flag, true) != nullptr);
}

bool IsTruthyFlagValue(const std::string& value) {
  if (value.empty()) return true;
  char ch = value[0];
  return isalnum(ch) &&
         !(ch == '0' || ch == 'f' || ch == 'F' || ch == 'n' || ch == 'N');
}

int32_t gamma_tn(){
  static int32_t r = -1;
  if (r > 0) return r;
  return r=int32_t(_gamma_tn());
}

size_t get_mem_total() {
  std::string token;
  std::ifstream file("/proc/meminfo");
  while(file >> token) {
    if(token == "MemTotal:") {
      size_t mem=0UL;
      auto res = (file >> mem) ? mem * 1024UL : 0UL;
      return res;
    }
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
  return 0;
}

size_t memory_hb() {
  static size_t r = 1UL;
  if (r != 1UL) return r;
  // MAX MEM in MB
  if ((r = gme::Int32FromEnv("MAX_MEM_MB", 0)) > 0){
    return (r *= (2UL<<20));
  }
  const static char* fp = "/sys/fs/cgroup/memory/memory.limit_in_bytes";
  auto x = [](const char* s) {
    size_t t = 0UL;
    std::fstream x(s, std::ios_base::in);
    x >> t;
    if (t == 0L) return get_mem_total();
    return t;
  };
  if (gme::BoolFromEnv("DEBUG", false))
    printf("%lu\n",x(fp));
  return r = x(fp);
}
}  // namespace gme
