/* Mate.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_SEARCH_MATE_MATE_HPP__
#define SUNFISH_SEARCH_MATE_MATE_HPP__

#include "core/position/Position.hpp"

namespace sunfish {

class Mate {
public:

  Mate() = delete;

  static bool mate1Ply(const Position& position) {
    if (position.getTurn() == Turn::Black) {
      return mate1Ply<Turn::Black>(position);
    } else {
      return mate1Ply<Turn::White>(position);
    }
  }

private:

  static bool isMate(Position position,
                     Move move) {
    Piece captured;
    if (!position.doMove(move, captured)) {
      return false;
    }

    return position.isMate();
  }

  template <Turn turn>
  static bool mate1Ply(const Position& position);

};

} // namespace sunfish

#endif // SUNFISH_SEARCH_MATE_MATE_HPP__
