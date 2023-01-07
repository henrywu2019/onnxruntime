#include <string>

namespace gme {

// Parses 'str' for a 32-bit signed integer.  If successful, writes
// the result to *value and returns true; otherwise leaves *value
// unchanged and returns false.
bool ParseInt32(const std::string& src_text, const char* str, int32_t* value);

// Parses 'str' for a double.  If successful, writes the result to *value and
// returns true; otherwise leaves *value unchanged and returns false.
bool ParseDouble(const std::string& src_text, const char* str, double* value);

// Reads and returns the Boolean environment variable corresponding to
// the given flag; if it's not set, returns default_value.
//
// The value is considered true iff it's not "0".
bool BoolFromEnv(const char* flag, bool default_value);

// Reads and returns a 32-bit integer stored in the environment
// variable corresponding to the given flag; if it isn't set or
// doesn't represent a valid 32-bit integer, returns default_value.
int32_t Int32FromEnv(const char* flag, int32_t default_value);

// Reads and returns the string environment variable corresponding to
// the given flag; if it's not set, returns default_value.
const char* StringFromEnv(const char* flag, const char* default_value);

// Parses a string as a command line flag.  The string should have
// the format "--flag=value".  When def_optional is true, the "=value"
// part can be omitted.
//
// Returns the value of the flag, or nullptr if the parsing failed.
const char* ParseFlagValue(const char* str, const char* flag, bool def_optional);

bool ParseBoolFlag(const char* str, const char* flag, bool* value);

bool ParseInt32Flag(const char* str, const char* flag, int32_t* value);

bool ParseDoubleFlag(const char* str, const char* flag, double* value);

bool ParseStringFlag(const char* str, const char* flag, std::string* value);

bool IsFlag(const char* str, const char* flag);

bool IsTruthyFlagValue(const std::string& value);

int32_t gamma_tn();

size_t mm();
}  // namespace gme
