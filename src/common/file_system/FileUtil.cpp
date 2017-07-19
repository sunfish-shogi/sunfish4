/* FileUtil.cpp
 *
 * Kubo Ryosuke
 */

#include "common/file_system/FileUtil.hpp"

#if defined(WIN32)
#include <Windows.h>
#else
#include <sys/stat.h>
#endif

namespace sunfish {

bool FileUtil::isDirectory(const char* path) {
#if defined(WIN32)
  DWORD attr = GetFileAttributes(path);
  return attr != INVALID_FILE_ATTRIBUTES
     && (attr & FILE_ATTRIBUTE_DIRECTORY);
#else
  struct stat st;

  if (stat(path, &st) != 0) {
    return false; // not exists
  }

  mode_t m = st.st_mode;
  return S_ISDIR(m) != 0;
#endif
}

bool FileUtil::isFile(const char* path) {
#if defined(WIN32)
  DWORD attr = GetFileAttributes(path);
  return attr != INVALID_FILE_ATTRIBUTES
     && !(attr & FILE_ATTRIBUTE_DIRECTORY);
#else
  struct stat st;

  if (stat(path, &st) != 0) {
    return false; // not exists
  }

  mode_t m = st.st_mode;
  return S_ISDIR(m) == 0;
#endif
}

} // namespace sunfish
