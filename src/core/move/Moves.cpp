/* Moves.cpp
 *
 * Kubo Ryosuke
 */

#include "core/move/Moves.hpp"
#include <algorithm>

namespace sunfish {

void sortMovesForDebug(Moves& moves, const Position& position) {
  std::sort(moves.begin(), moves.end(), [&position](Move lhs, Move rhs) {
    if (lhs.isDrop() != rhs.isDrop()) {
      return rhs.isDrop();
    }

    if (lhs.isDrop()) {
      if (lhs.droppingPieceType() != rhs.droppingPieceType()) {
        return lhs.droppingPieceType().raw() < rhs.droppingPieceType().raw();
      }

    } else {
      Piece lp = position.getPieceOnBoard(lhs.from());
      Piece rp = position.getPieceOnBoard(rhs.from());
      if (lp != rp) {
        return lp.raw() < rp.raw();
      }

      if (lhs.from().getFile() != rhs.from().getFile()) {
        return lhs.from().getFile() < rhs.from().getFile();
      }
    }

    if (lhs.from().getRank() != rhs.from().getRank()) {
      return lhs.from().getRank() < rhs.from().getRank();
    }

    if (lhs.to().getFile() != rhs.to().getFile()) {
      return lhs.to().getFile() < rhs.to().getFile();
    }

    if (lhs.to().getRank() != rhs.to().getRank()) {
      return lhs.to().getRank() < rhs.to().getRank();
    }

    if (lhs.isPromotion() != rhs.isPromotion()) {
      return lhs.isPromotion();
    }

    return false;
  });
}

} // namespace sunfish
