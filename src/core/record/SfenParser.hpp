/* SfenParser.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_CORE_RECORD_SFENPARSER_HPP__
#define SUNFISH_CORE_RECORD_SFENPARSER_HPP__

#include "core/move/Move.hpp"
#include "core/position/Position.hpp"

namespace sunfish {

class SfenParser {
public:

  static bool parsePosition(const char* data, Position& position);

  static bool parsePosition(const std::string& data, Position& position) {
    return parsePosition(data.c_str(), position);
  }

  static bool parsePosition(const char* arg1,
                            const char* arg2,
                            const char* arg3,
                            const char* arg4,
                            Position& position);

  static bool parsePosition(const std::string& arg1,
                            const std::string& arg2,
                            const std::string& arg3,
                            const std::string& arg4,
                            Position& position) {
    return parsePosition(arg1.c_str(),
                         arg2.c_str(),
                         arg3.c_str(),
                         arg4.c_str(),
                         position);
  }

  static bool parseMove(const char* data, Move& move);

  static bool parseMove(const std::string& data, Move& move) {
    return parseMove(data.c_str(), move);
  }

private:

  SfenParser();

};

} // namespace sunfish

#endif // SUNFISH_CORE_RECORD_SFENPARSER_HPP__
