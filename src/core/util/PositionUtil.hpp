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

  static bool isEvenInitialPosition(const Position& pos) {
    return isSamePosition(pos, Position(Position::Handicap::Even));
  }

  static bool isSamePosition(const Position& pos1, const Position& pos2) {
    SQUARE_EACH(square) {
      if (pos1.getPieceOnBoard(square) != pos2.getPieceOnBoard(square)) {
        return false;
      }
    }

    HAND_EACH(piece) {
      if (pos1.getBlackHandPieceCount(piece) != pos2.getBlackHandPieceCount(piece)) {
        return false;
      }
      if (pos1.getWhiteHandPieceCount(piece) != pos2.getWhiteHandPieceCount(piece)) {
        return false;
      }
    }

    return true;
  }

private:

  PositionUtil();

};

} // sunfish

#endif // SUNFISH_CORE_UTIL_POSITIONUTIL_HPP__
