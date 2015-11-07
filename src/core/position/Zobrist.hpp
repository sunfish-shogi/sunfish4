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

#define FUNC_HAND__(piece) \
  static Type handB ## piece(int num) { \
    return HandB ## piece[num]; \
  } \
  static Type handW ## piece(int num) { \
    return HandW ## piece[num]; \
  }

  FUNC_HAND__(Pawn)
  FUNC_HAND__(Lance)
  FUNC_HAND__(Knight)
  FUNC_HAND__(Silver)
  FUNC_HAND__(Gold)
  FUNC_HAND__(Bishop)
  FUNC_HAND__(Rook)

#undef FUNC_HAND__

  static Type blackHand(const PieceType pieceType, int num) {
    switch (pieceType.raw()) {
    case PieceNumber::Pawn: return HandBPawn[num];
    case PieceNumber::Lance: return HandBLance[num];
    case PieceNumber::Knight: return HandBKnight[num];
    case PieceNumber::Silver: return HandBSilver[num];
    case PieceNumber::Gold: return HandBGold[num];
    case PieceNumber::Bishop: return HandBBishop[num];
    case PieceNumber::Rook: return HandBRook[num];
    default: assert(false);
    }
    return 0; // unreachable
  }

  static Type whiteHand(const PieceType pieceType, int num) {
    switch (pieceType.raw()) {
    case PieceNumber::Pawn: return HandWPawn[num];
    case PieceNumber::Lance: return HandWLance[num];
    case PieceNumber::Knight: return HandWKnight[num];
    case PieceNumber::Silver: return HandWSilver[num];
    case PieceNumber::Gold: return HandWGold[num];
    case PieceNumber::Bishop: return HandWBishop[num];
    case PieceNumber::Rook: return HandWRook[num];
    default: assert(false);
    }
    return 0; // unreachable
  }

  static Type black() {
    return Black;
  }

private:

  Zobrist();

  static const Type Board[Square::N][PieceNumber::Num];
  static const Type HandBPawn[18];
  static const Type HandBLance[4];
  static const Type HandBKnight[4];
  static const Type HandBSilver[4];
  static const Type HandBGold[4];
  static const Type HandBBishop[2];
  static const Type HandBRook[2];
  static const Type HandWPawn[18];
  static const Type HandWLance[4];
  static const Type HandWKnight[4];
  static const Type HandWSilver[4];
  static const Type HandWGold[4];
  static const Type HandWBishop[2];
  static const Type HandWRook[2];
  static const Type Black;

};

} // namespace sunfish

#endif // SUNFISH_CORE_POSITION_ZOBRIST_HPP__
