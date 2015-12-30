/* Evaluator.cpp
 *
 * Kubo Ryosuke
 */

#include "search/eval/Evaluator.hpp"
#include "search/eval/FeatureTemplates.hpp"
#include "search/eval/Material.hpp"
#include "logger/Logger.hpp"
#include <fstream>
#include <mutex>
#include <cstring>

namespace sunfish {

std::shared_ptr<Evaluator> Evaluator::sharedEvaluator() {
  static std::weak_ptr<Evaluator> wptr;
  static std::mutex mutex;

  std::lock_guard<std::mutex> lock(mutex);
  std::shared_ptr<Evaluator> sptr = wptr.lock();
  if (!sptr) {
    sptr = std::make_shared<Evaluator>();
    wptr = sptr;
  }

  return sptr;
}

Evaluator::Evaluator() {
  initializeZero();
}

void Evaluator::initializeZero() {
  memset((void*)&fv_, 0, sizeof(fv_));
  onChanged();
}

bool Evaluator::read(const char* path) {
  std::ifstream file(path, std::ios::in | std::ios::binary);
  if (!file) {
    return false;
  }

  file.read(reinterpret_cast<char*>(&fv_), sizeof(fv_));
  file.close();

  onChanged();

  return true;
}

bool Evaluator::write(const char* path) const {
  std::ofstream file(path, std::ios::out | std::ios::binary);
  if (!file) {
    return false;
  }

  file.write(reinterpret_cast<const char*>(&fv_), sizeof(fv_));
  file.close();

  return true;
}

void Evaluator::onChanged() {
  cumulate(fv_);
}

ClassifiedScores Evaluator::evaluate(const Position& position) {
  return {
    calculateMaterialScore(position),
    calculatePositionalScore(position),
  };
}

Score Evaluator::calculateMaterialScore(const Position& position) {
  Score score = Score::zero();

  auto& blackHand = position.getBlackHand();
  score += material::Pawn   * blackHand.get(PieceType::pawn  ()); \
  score += material::Lance  * blackHand.get(PieceType::lance ()); \
  score += material::Knight * blackHand.get(PieceType::knight()); \
  score += material::Silver * blackHand.get(PieceType::silver()); \
  score += material::Gold   * blackHand.get(PieceType::gold  ()); \
  score += material::Bishop * blackHand.get(PieceType::bishop()); \
  score += material::Rook   * blackHand.get(PieceType::rook  ()); \

  auto& whiteHand = position.getWhiteHand();
  score -= material::Pawn   * whiteHand.get(PieceType::pawn  ()); \
  score -= material::Lance  * whiteHand.get(PieceType::lance ()); \
  score -= material::Knight * whiteHand.get(PieceType::knight()); \
  score -= material::Silver * whiteHand.get(PieceType::silver()); \
  score -= material::Gold   * whiteHand.get(PieceType::gold  ()); \
  score -= material::Bishop * whiteHand.get(PieceType::bishop()); \
  score -= material::Rook   * whiteHand.get(PieceType::rook  ()); \

  Bitboard occ = nosseOr(position.getBOccupiedBitboard(),
                         position.getWOccupiedBitboard());
  occ.unset(position.getBlackKingSquare());
  occ.unset(position.getWhiteKingSquare());

  BB_EACH(square, occ) {
    auto piece = position.getPieceOnBoard(square);

    if (piece.isBlack()) {
      score += material::score(piece);
    } else {
      score -= material::score(piece);
    }
  }

  return score;
}

int32_t Evaluator::calculatePositionalScore(const Position& position) {
  return operate<FeatureType, int32_t,
                 FeatureOperationType::Evaluate>
                (fv_, position, 0);
}

template <Turn turn>
ClassifiedScores Evaluator::evaluateDiff(ClassifiedScores scores,
                                         const Position& position,
                                         Move move,
                                         Piece captured) {
  Piece piece = position.getPieceOnBoard(move.to());

  if (piece.type() == PieceType::king()) {
    if (!captured.isEmpty()) {
      if (turn == Turn::Black) {
        scores.materialScore += material::exchangeScore(captured);
      } else {
        scores.materialScore -= material::exchangeScore(captured);
      }
    }
    scores.positionalScore = calculatePositionalScore(position);
    return scores;
  }

  auto bking = position.getBlackKingSquare().raw();
  auto wking = position.getWhiteKingSquare().raw();
  auto rbking = position.getWhiteKingSquare().dsym().raw();
  auto rwking = position.getBlackKingSquare().dsym().raw();

  if (move.isDrop()) {
    PieceType pieceType = piece.type();
    if (turn == Turn::Black) {
      auto n = position.getBlackHandPieceCount(pieceType) + 1;
      scores.positionalScore -= fv_.kkp[bking][wking][kkpHandIndex(pieceType, n)];
      scores.positionalScore += fv_.kkp[bking][wking][kkpBoardIndex(pieceType, move.to())];
    } else {
      auto n = position.getWhiteHandPieceCount(pieceType) + 1;
      scores.positionalScore += fv_.kkp[rbking][rwking][kkpHandIndex(pieceType, n)];
      scores.positionalScore -= fv_.kkp[rbking][rwking][kkpBoardIndex(pieceType, move.to().dsym())];
    }

    goto skip_capture;
 
  } else if (move.isPromotion()) {
    Piece pieceBefore = piece.unpromote();

    if (turn == Turn::Black) {
      scores.materialScore += material::promotionScore(piece);
      scores.positionalScore -= fv_.kkp[bking][wking][kkpBoardIndex(pieceBefore.type(), move.from())];
      scores.positionalScore += fv_.kkp[bking][wking][kkpBoardIndex(piece.type(), move.to())];
    } else {
      scores.materialScore -= material::promotionScore(piece);
      scores.positionalScore += fv_.kkp[rbking][rwking][kkpBoardIndex(pieceBefore.type(), move.from().dsym())];
      scores.positionalScore -= fv_.kkp[rbking][rwking][kkpBoardIndex(piece.type(), move.to().dsym())];
    }

  } else {
    if (turn == Turn::Black) {
      scores.materialScore -= fv_.kkp[bking][wking][kkpBoardIndex(piece.type(), move.from())];
      scores.positionalScore += fv_.kkp[bking][wking][kkpBoardIndex(piece.type(), move.to())];
    } else {
      scores.materialScore += fv_.kkp[rbking][rwking][kkpBoardIndex(piece.type(), move.from().dsym())];
      scores.positionalScore -= fv_.kkp[rbking][rwking][kkpBoardIndex(piece.type(), move.to().dsym())];
    }
  }

  if (!captured.isEmpty()) {
    if (turn == Turn::Black) {
      scores.materialScore += material::exchangeScore(captured);
      scores.positionalScore += fv_.kkp[rbking][rwking][kkpBoardIndex(captured.type(), move.from().dsym())];
      auto n = position.getBlackHandPieceCount(captured.type());
      scores.positionalScore += fv_.kkp[bking][wking][kkpHandIndex(captured.type(), n)];
    } else {
      scores.materialScore -= material::exchangeScore(captured);
      scores.positionalScore -= fv_.kkp[bking][wking][kkpBoardIndex(captured.type(), move.from())];
      auto n = position.getWhiteHandPieceCount(captured.type());
      scores.positionalScore -= fv_.kkp[rbking][rwking][kkpHandIndex(captured.type(), n)];
    }
  }

skip_capture:

  return scores;
}
template
ClassifiedScores
Evaluator::evaluateDiff<Turn::Black>(ClassifiedScores,
                                     const Position&, Move,
                                     Piece);
template
ClassifiedScores
Evaluator::evaluateDiff<Turn::White>(ClassifiedScores,
                                     const Position&, Move,
                                     Piece);

} // namespace sunfish
