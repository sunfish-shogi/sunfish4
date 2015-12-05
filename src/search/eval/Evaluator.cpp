/* Evaluator.cpp
 *
 * Kubo Ryosuke
 */

#include "search/eval/Evaluator.hpp"
#include "search/eval/Material.hpp"
#include "core/position/Position.hpp"

namespace sunfish {

Evaluator::Evaluator() {
}

Value Evaluator::evaluateMaterial(const Position& position) const {
  Value value = Value::zero();

  SQUARE_EACH(square) {
    Piece piece = position.getPieceOnBoard(square);

    if (piece.type() == PieceType::king()) {
      continue;
    }

    if (piece.isBlack()) {
      value += material::piece(piece);
    } else {
      value -= material::piece(piece);
    }
  }

  auto& blackHand = position.getBlackHand();
  value += material::Pawn   * blackHand.get(PieceType::pawn());
  value += material::Lance  * blackHand.get(PieceType::lance());
  value += material::Knight * blackHand.get(PieceType::knight());
  value += material::Silver * blackHand.get(PieceType::silver());
  value += material::Gold   * blackHand.get(PieceType::gold());
  value += material::Bishop * blackHand.get(PieceType::bishop());
  value += material::Rook   * blackHand.get(PieceType::rook());

  auto& whiteHand = position.getWhiteHand();
  value -= material::Pawn   * whiteHand.get(PieceType::pawn());
  value -= material::Lance  * whiteHand.get(PieceType::lance());
  value -= material::Knight * whiteHand.get(PieceType::knight());
  value -= material::Silver * whiteHand.get(PieceType::silver());
  value -= material::Gold   * whiteHand.get(PieceType::gold());
  value -= material::Bishop * whiteHand.get(PieceType::bishop());
  value -= material::Rook   * whiteHand.get(PieceType::rook());

  return value;
}

} // namespace sunfish
