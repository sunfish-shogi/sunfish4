/* Directory.cpp
 *
 * Kubo Ryosuke
 */

#include "common/file_system/Directory.hpp"
#include "common/string/Wildcard.hpp"
#include "logger/Logger.hpp"

#include <sstream>
#include <string>
#include <cstring>

#if WIN32
# include <windows.h>
#else
# include <dirent.h>
#endif

namespace {

#if WIN32
const char Slash = '\\';
#else
const char Slash = '/';
#endif

} // namespace

namespace sunfish {

Directory::Directory(const char* directory) {
  auto dlen = strlen(directory);
  if (directory[dlen-1] == Slash) {
    dlen--;
  }
  std::string path(directory, dlen);
  path_.swap(path);
}

Directory::Files Directory::files(const char* wildcard) {
#if WIN32
  // generate file pattern
  std::stringstream pattern;
  pattern << path_ << Slash << wildcard;

  WIN32_FIND_DATA fd;
  HANDLE hFind = FindFirstFile(pattern.str().c_str(), &fd);
  if (hFind == INVALID_HANDLE_VALUE) {
    LOG(warning) << "There are no files in `" << path_ << '\'';
    return 0;
  }
#else
  DIR* pdir;
  struct dirent* pent;

  if ((pdir = opendir(path_.c_str())) == nullptr) {
    LOG(error) << "Could not open the directory: `" << path_;
    return {};
  }

  Wildcard mathcer(wildcard);
#endif

  Files files;

  for (;;) {
#if WIN32
    std::string fname = fd.cFileName;
#else
    if (NULL == (pent = readdir(pdir))) {
      closedir( pdir );
      break;
    }

    std::string fname = pent->d_name;
#endif

    // exclude directories
    if (fname[fname.length()-1] == '.') {
      continue;
    }

#if !WIN32
    if (!mathcer.match(fname)) {
      continue;
    }
#endif

    std::ostringstream path;
    path << path_ << Slash << fname;

    files.push_back(path.str());

#if WIN32
    if (!FindNextFile(hFind, &fd)) {
      FindClose( hFind );
      break;
    }
#endif
  }

  return files;
}

} // namespace sunfish
