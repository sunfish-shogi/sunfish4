/* FileUtil.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_COMMON_FILESYSTEM_FileUtil_HPP__
#define SUNFISH_COMMON_FILESYSTEM_FileUtil_HPP__

#include "common/Def.hpp"
#include <string>

namespace sunfish {

class FileUtil {
public:

  FileUtil() = delete;
  FileUtil(const FileUtil&) = delete;
  FileUtil(FileUtil&&) = delete;

  static bool isDirectory(const char* path);

  static bool isDirectory(const std::string& path) {
    return isDirectory(path.c_str());
  }

  static bool isFile(const char* path);

  static bool isFile(const std::string& path) {
    return isFile(path.c_str());
  }

};

} // namespace sunfish

#endif // SUNFISH_COMMON_FILESYSTEM_FileUtil_HPP__
