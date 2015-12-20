/* Evaluator.cpp
 *
 * Kubo Ryosuke
 */

#include "search/eval/Evaluator.hpp"
#include "search/eval/Material.hpp"

namespace sunfish {

Evaluator::Evaluator() {
}

Score Evaluator::evaluateMaterial(const Position& position) const {
  Score score = Score::zero();

  Bitboard occ = position.getBOccupiedBitboard() | position.getWOccupiedBitboard();

  BB_EACH(square, occ) {
    Piece piece = position.getPieceOnBoard(square);

    if (piece.type() == PieceType::king()) {
      continue;
    }

    if (piece.isBlack()) {
      score += material::score(piece);
    } else {
      score -= material::score(piece);
    }
  }

  auto& blackHand = position.getBlackHand();
  score += material::Pawn   * blackHand.get(PieceType::pawn());
  score += material::Lance  * blackHand.get(PieceType::lance());
  score += material::Knight * blackHand.get(PieceType::knight());
  score += material::Silver * blackHand.get(PieceType::silver());
  score += material::Gold   * blackHand.get(PieceType::gold());
  score += material::Bishop * blackHand.get(PieceType::bishop());
  score += material::Rook   * blackHand.get(PieceType::rook());

  auto& whiteHand = position.getWhiteHand();
  score -= material::Pawn   * whiteHand.get(PieceType::pawn());
  score -= material::Lance  * whiteHand.get(PieceType::lance());
  score -= material::Knight * whiteHand.get(PieceType::knight());
  score -= material::Silver * whiteHand.get(PieceType::silver());
  score -= material::Gold   * whiteHand.get(PieceType::gold());
  score -= material::Bishop * whiteHand.get(PieceType::bishop());
  score -= material::Rook   * whiteHand.get(PieceType::rook());

  return score;
}

template <Turn turn>
Score Evaluator::evaluateDiff(const Position& position, Move move, Piece captured) const {
  Score score = Score::zero();

  if (move.isPromotion()) {
    Piece piece = position.getPieceOnBoard(move.to());
    score += material::promotionScore(piece);
  }

  if (!captured.isEmpty()) {
    score += material::exchangeScore(captured);
  }

  return turn == Turn::Black ? score : -score;
}
template Score Evaluator::evaluateDiff<Turn::Black>(const Position&, Move, Piece) const;
template Score Evaluator::evaluateDiff<Turn::White>(const Position&, Move, Piece) const;

} // namespace sunfish
