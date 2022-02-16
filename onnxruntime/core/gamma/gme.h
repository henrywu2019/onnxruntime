#pragma once
#include <string>


namespace gme {
std::string get_model_name(const std::string&, const std::string&);
void ensure_folder(const std::string&);
bool exists(const std::string& f);
}  // namespace gme
