/* PositionUtil.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_CORE_UTIL_POSITIONUTIL_HPP__
#define SUNFISH_CORE_UTIL_POSITIONUTIL_HPP__

#include "core/position/Position.hpp"
#include "core/record/CsaReader.hpp"
#include <sstream>
#include <string>

namespace sunfish {

class PositionUtil {
public:

  static Position createPositionFromCsaString(const char* src) {
    return createPositionFromCsaString(std::string(src));
  }

  static Position createPositionFromCsaString(const std::string& src) {
    std::istringstream iss(src);
    Position pos;
    CsaReader::readPosition(iss, pos);
    return pos;
  }

private:

  PositionUtil();

};

} // sunfish

#endif // SUNFISH_CORE_UTIL_POSITIONUTIL_HPP__
