/* Resource.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_COMMON_RESOURCE_RESOURCE_HPP__
#define SUNFISH_COMMON_RESOURCE_RESOURCE_HPP__

#include <string>
#include <map>

namespace sunfish {

class Resource {
public:

  using INI = std::map<std::string, std::map<std::string, std::string>>;

  Resource() = delete;

  static std::string string(const char* path, const char* defaultValue);

  static INI ini(const char* path);

};

inline std::string getValue(const Resource::INI& ini,
                            const char* sectionName,
                            const char* key) {
  auto section = ini.find(sectionName);
  if (section == ini.end()) {
    return "";
  }

  auto kv = section->second.find(key);
  if (kv == section->second.end()) {
    return "";
  }

  return kv->second;
}

} // namespace sunfish

#endif // SUNFISH_COMMON_RESOURCE_RESOURCE_HPP__
