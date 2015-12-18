/* FileUtil.cpp
 *
 * Kubo Ryosuke
 */

#include "common/file_system/FileUtil.hpp"

#include<sys/stat.h>

namespace sunfish {

bool FileUtil::isDirectory(const char* path) {
  struct stat st;

  if (stat(path, &st) != 0) {
    return false; // not exists
  }

  mode_t m = st.st_mode;
  return S_ISDIR(m) != 0;
}

bool FileUtil::isFile(const char* path) {
  struct stat st;

  if (stat(path, &st) != 0) {
    return false; // not exists
  }

  mode_t m = st.st_mode;
  return S_ISDIR(m) == 0;
}

} // namespace sunfish
