/* Directory.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_COMMON_FILESYSTEM_DIRECTORY_HPP__
#define SUNFISH_COMMON_FILESYSTEM_DIRECTORY_HPP__

#include "common/Def.hpp"

#include <list>
#include <string>

namespace sunfish {

class Directory{
public:
  using Files = std::list<std::string>;

public:
  Directory(const char* directory);
  Directory(const std::string& directory);
  Directory(const Directory&) = default;
  Directory(Directory&& src) NOEXCEPT = default;
  ~Directory() = default;

  Files files(const char* wildcard);

private:
  void init(const char* directory);

  std::string path_;

};

} // namespace sunfish

#endif // SUNFISH_COMMON_FILESYSTEM_DIRECTORY_HPP__
