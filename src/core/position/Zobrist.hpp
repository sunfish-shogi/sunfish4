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
private:

  Zobrist();

  static const uint64_t Board[Square::N][PieceNumber::Num];
  static const uint64_t HandBPawn[18];
  static const uint64_t HandBLance[4];
  static const uint64_t HandBKnight[4];
  static const uint64_t HandBSilver[4];
  static const uint64_t HandBGold[4];
  static const uint64_t HandBBishop[2];
  static const uint64_t HandBRook[2];
  static const uint64_t HandWPawn[18];
  static const uint64_t HandWLance[4];
  static const uint64_t HandWKnight[4];
  static const uint64_t HandWSilver[4];
  static const uint64_t HandWGold[4];
  static const uint64_t HandWBishop[2];
  static const uint64_t HandWRook[2];
  static const uint64_t Black;

public:

  static uint64_t board(const Square& sq, const Piece& piece) {
    return Board[sq.raw()][piece.raw()];
  }

#define FUNC_HAND__(piece) \
  static uint64_t handB ## piece(int num) { \
    return HandB ## piece[num]; \
  } \
  static uint64_t handW ## piece(int num) { \
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

  static uint64_t handBlack(const PieceType pieceType, int num) {
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

  static uint64_t handWhite(const PieceType piece, int num) {
    switch (piece.raw()) {
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

  static uint64_t black() {
    return Black;
  }

};

} // namespace sunfish

#endif // SUNFISH_CORE_POSITION_ZOBRIST_HPP__
