/* Resource.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_COMMON_RESOURCE_RESOURCE_HPP__
#define SUNFISH_COMMON_RESOURCE_RESOURCE_HPP__

#include <string>

namespace sunfish {

class Resource {
public:

  Resource() = delete;

  static std::string string(const char* path, const char* defaultValue);

};

} // namespace sunfish

#endif // SUNFISH_COMMON_RESOURCE_RESOURCE_HPP__
