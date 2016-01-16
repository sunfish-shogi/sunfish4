/* Resource.cpp
 *
 * Kubo Ryosuke
 */

#include "common/resource/Resource.hpp"
#include "common/string/Wildcard.hpp"
#include "common/string/StringUtil.hpp"
#include "logger/Logger.hpp"
#include <fstream>
#include <regex>

namespace sunfish {

std::string Resource::string(const char* path, const char* defaultValue) {
  std::ifstream fin(path);

  if (!fin) {
    LOG(error) << "could not open the resource file: " << path;
    return defaultValue;
  }

  std::string string(defaultValue);
  std::getline(fin, string);

  fin.close();

  return string;
}

Resource::INI Resource::ini(const char* path) {
  std::ifstream fin(path);

  if (!fin) {
    LOG(error) << "could not open the INI file: " << path;
    return Resource::INI{};
  }

  static Wildcard Comment(";*");
  static Wildcard Section("[*]");
  INI ini;
  std::string sectionName;

  for (;;) {
    std::string line;
    std::getline(fin, line);

    if (fin.eof()) {
      break;
    }

    if (fin.fail()) {
      LOG(error) << "an error occured while reading INI file: " << path;
      return Resource::INI{};
    }

    line = StringUtil::trim(line);

    if (line.empty() || Comment.match(line)) {
      continue;
    }

    if (Section.match(line)) {
      sectionName = line.substr(1, line.length() - 2);
      continue;
    }

    auto sepPos = line.find('=');
    if (sepPos == std::string::npos || sepPos == 0) {
      LOG(error) << "invalid INI file format: " << path;
      LOG(error) << line;
      return Resource::INI{};
    }

    auto key = StringUtil::trim(line.substr(0, sepPos));
    auto value = StringUtil::trim(line.substr(sepPos + 1));

    ini[sectionName][key] = value;
  }

  fin.close();

  return ini;
}

} // namespace sunfish
