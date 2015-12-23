/* Resource.cpp
 *
 * Kubo Ryosuke
 */

#include "common/resource/Resource.hpp"
#include <fstream>

namespace sunfish {

std::string Resource::string(const char* path, const char* defaultValue) {
  std::ifstream fin(path);

  if (!fin) {
    return defaultValue;
  }

  std::string string(defaultValue);
  std::getline(fin, string);

  fin.close();

  return string;
}

} // namespace sunfish
