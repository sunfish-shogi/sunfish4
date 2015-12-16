/* Zobrist.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_CORE_POSITION_ZOBRIST_HPP__
#define SUNFISH_CORE_POSITION_ZOBRIST_HPP__

#include "../base/Square.hpp"
#include "../base/Piece.hpp"
#include <cstdint>
#include <cassert>

namespace sunfish {

class Zobrist {
public:

  using Type = uint64_t;

  static Type board(const Square& sq, const Piece& piece) {
    return Board[sq.raw()][piece.raw()];
  }

  static Type blackHand(const PieceType pieceType) {
    return BlackHand[pieceType.raw()];
  }

  static Type whiteHand(const PieceType pieceType) {
    return WhiteHand[pieceType.raw()];
  }

  static Type black() {
    return Black;
  }

private:

  Zobrist();

  static const Type Board[Square::N][PieceNumber::Num];
  static const Type BlackHand[7];
  static const Type WhiteHand[7];
  static const Type Black;

};

} // namespace sunfish

#endif // SUNFISH_CORE_POSITION_ZOBRIST_HPP__
