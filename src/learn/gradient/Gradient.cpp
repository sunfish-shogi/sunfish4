/* Gradient.cpp
 *
 * Kubo Ryosuke
 */

#include "learn/gradient/Gradient.hpp"
#include "core/position/Position.hpp"

namespace sunfish {

void extractMaterial(MaterialGradient mg, const Position& pos, float d) {
  int pc[std::extent<MaterialGradient>::value] = {};

  SQUARE_EACH(square) {
    Piece piece = pos.getPieceOnBoard(square);
    if (piece.isEmpty()) {
      continue;
    }

    ASSERT(piece.type().raw() < std::extent<MaterialGradient>::value);
    if (piece.isBlack()) {
      pc[piece.type().raw()]++;
    } else {
      pc[piece.type().raw()]--;
    }
  }

  HAND_EACH(pieceType) {
    int count = pos.getBlackHandPieceCount(pieceType)
              - pos.getWhiteHandPieceCount(pieceType);
    pc[pieceType.raw()] += count;
  }

  for (size_t i = 0; i < std::extent<MaterialGradient>::value; i++) {
    mg[i] += pc[i] * d;
  }
}

} // namespace sunfish
