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
  static bool parseMove(const char* data, const Position& position, Move& move);

private:

  SfenParser();

};

} // namespace sunfish

#endif // SUNFISH_CORE_RECORD_SFENPARSER_HPP__
