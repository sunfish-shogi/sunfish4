/* StringUtil.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_COMMON_STRING_STRINGSTRING_HPP__
#define SUNFISH_COMMON_STRING_STRINGSTRING_HPP__

#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <utility>
#include <cstdint>
#include <cstdlib>
#include <cctype>

namespace sunfish {

class StringUtil {
private:

  StringUtil() {}

public:

  static std::string stringify(uint16_t u16) {
    std::ostringstream oss;
    oss << std::setw(4) << std::setfill('0') << std::hex << u16;
    return oss.str();
  }

  static std::string stringify(uint32_t u32) {
    std::ostringstream oss;
    oss << std::setw(8) << std::setfill('0') << std::hex << u32;
    return oss.str();
  }

  static std::string stringify(uint64_t u64) {
    return stringify((unsigned)(u64>>32)) + stringify((unsigned)u64);
  }

  static std::string trim(const std::string& line) {
    std::string::size_type spos;
    std::string::size_type epos;
    for (spos = 0; spos < line.length(); spos++) {
      if (!isspace(line[spos])) { break; }
    }
    for (epos = line.length() - 1; epos > spos; epos--) {
      if (!isspace(line[epos])) { break; }
    }
    return line.substr(spos, epos - spos + 1);
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

  static std::vector<std::string> split(const char* line, char delim) {
    return split(line, [delim](char c) { return c == delim; });
  }

  static std::vector<std::string> split(const std::string line, char delim) {
    return split(line, [delim](char c) { return c == delim; });
  }

  static std::pair<std::string, std::string> splitOnce(const std::string& line, char delim) {
    auto sep = line.find_first_of(delim);
    if (sep == std::string::npos) {
      return { line, "" };
    }
    return { line.substr(0, sep), line.substr(sep + 1) };
  }

  static int toInt(const std::string str, int defaultValue) {
    if (str.empty()) {
      return defaultValue;
    }

    char* endptr;
    int i = strtol(str.c_str(), &endptr, 10);
    if (endptr == str.c_str() || *endptr != '\0') {
      return defaultValue;
    }

    return i;
  }

  static float toFloat(const std::string str, float defaultValue) {
    if (str.empty()) {
      return defaultValue;
    }

    char* endptr;
    float f = strtof(str.c_str(), &endptr);
    if (endptr == str.c_str() || *endptr != '\0') {
      return defaultValue;
    }

    return f;
  }

  static bool isNumber(const std::string str) {
    char* endptr;
    strtof(str.c_str(), &endptr);
    return endptr != str.c_str() && *endptr == '\0';
  }

  static std::string ordinal(unsigned n) {
    std::ostringstream oss;
    oss << n;
    if (n == 1) {
      oss << "st";
    } else if (n == 2) {
      oss << "nd";
    } else if (n == 3) {
      oss << "rd";
    } else {
      oss << "th";
    }
    return oss.str();
  }

};

} // namespace sunfish

#endif // SUNFISH_COMMON_STRING_STRINGSTRING_HPP__
