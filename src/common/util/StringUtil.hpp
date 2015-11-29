/* StringUtil.h
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_COMMON_UTIL_STRINGUTIL_HPP__
#define SUNFISH_COMMON_UTIL_STRINGUTIL_HPP__

#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <utility>
#include <cstdint>

namespace sunfish {

class StringUtil {
private:

  StringUtil() {}

public:

  static std::string stringify(unsigned u32) {
    std::ostringstream oss;
    oss << std::setw(8) << std::setfill('0') << std::hex << u32;
    return oss.str();
  }

  static std::string stringify(uint64_t u64) {
    return stringify((unsigned)(u64>>32)) + stringify((unsigned)u64);
  }

  static std::string chomp(const std::string& line) {
    for (int index = (int)line.length()-1; index >= 0; index--) {
      if (line.at(index) != '\n') {
        return line.substr(0, index + 1);
      }
    }
    return line;
  }

  template <class T>
  static std::vector<std::string> split(const char* line, T&& isDelim) {
    std::vector<std::string> list;

    const char* startPos = nullptr;
    for (const char* p = line; ; p++) {
      if (isDelim(p[0]) || p[0] == '\0') {
        if (startPos != nullptr) {
          size_t length = static_cast<size_t>(reinterpret_cast<intptr_t>(p) - reinterpret_cast<intptr_t>(startPos));
          list.emplace_back(startPos, length);
          startPos = nullptr;
        }
        if (p[0] == '\0') {
          break;
        }
      } else {
        if (startPos == nullptr) {
          startPos = p;
        }
      }
    }

    return list;
  }

  template <class T>
  static std::vector<std::string> split(const std::string& line, T&& isDelim) {
    return split(line.c_str(), std::forward<T>(isDelim));
  }

};

} // namespace sunfish

#endif // SUNFISH_COMMON_UTIL_STRINGUTIL_HPP__
