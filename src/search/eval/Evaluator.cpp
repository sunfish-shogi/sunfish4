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

namespace {

const char* const EvalBin = "eval.bin";

} // namespace

namespace sunfish {

std::shared_ptr<Evaluator> Evaluator::sharedEvaluator() {
  static std::weak_ptr<Evaluator> wptr;
  static std::mutex mutex;

  std::lock_guard<std::mutex> lock(mutex);
  std::shared_ptr<Evaluator> sptr = wptr.lock();
  if (!sptr) {
    sptr = std::make_shared<Evaluator>(InitType::EvalBin);
    wptr = sptr;
  }

  return sptr;
}

Evaluator::Evaluator(InitType type) {
  switch (type) {
  case InitType::EvalBin:
    if (!readEvalBin()) {
      initializeZero();
    }
    break;

  case InitType::Zero:
    initializeZero();
    break;
  }
}

void Evaluator::initializeZero() {
  memset(reinterpret_cast<void*>(&fv_), 0, sizeof(fv_));
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

bool Evaluator::readEvalBin() {
  return read(EvalBin);
}

bool Evaluator::writeEvalBin() const {
  return write(EvalBin);
}

void Evaluator::onChanged() {
  cumulate(fv_, cv_);
  cache_.clear();
}

Score Evaluator::calculateMaterialScore(const Position& position) const {
  Score score = Score::zero();

  auto& blackHand = position.getBlackHand();
  score += material::Pawn   * blackHand.get(PieceType::pawn  ());
  score += material::Lance  * blackHand.get(PieceType::lance ());
  score += material::Knight * blackHand.get(PieceType::knight());
  score += material::Silver * blackHand.get(PieceType::silver());
  score += material::Gold   * blackHand.get(PieceType::gold  ());
  score += material::Bishop * blackHand.get(PieceType::bishop());
  score += material::Rook   * blackHand.get(PieceType::rook  ());

  auto& whiteHand = position.getWhiteHand();
  score -= material::Pawn   * whiteHand.get(PieceType::pawn  ());
  score -= material::Lance  * whiteHand.get(PieceType::lance ());
  score -= material::Knight * whiteHand.get(PieceType::knight());
  score -= material::Silver * whiteHand.get(PieceType::silver());
  score -= material::Gold   * whiteHand.get(PieceType::gold  ());
  score -= material::Bishop * whiteHand.get(PieceType::bishop());
  score -= material::Rook   * whiteHand.get(PieceType::rook  ());

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

Score Evaluator::calculateMaterialScoreDiff(Score score,
                                            const Position& position,
                                            Move move,
                                            Piece captured) const {
  if (move.isPromotion()) {
    Piece piece = position.getPieceOnBoard(move.to());
    if (position.getTurn() == Turn::White) {
      score += material::promotionScore(piece);
    } else {
      score -= material::promotionScore(piece);
    }
  }

  if (!captured.isEmpty()) {
    if (position.getTurn() == Turn::White) {
      score += material::exchangeScore(captured);
    } else {
      score -= material::exchangeScore(captured);
    }
  }

  return score;
}

int32_t Evaluator::calculatePositionalScore(const Position& position) {
  return operate<FeatureOperationType::Evaluate>
                (fv_, cv_, position, 0);
}

Score Evaluator::calculateTotalScore(Score materialScore,
                                     const Position& position) {
  Score score;
  if (cache_.check(position.getHash(), score)) {
    return score;
  }

  auto positionalScore = calculatePositionalScore(position);
  score = materialScore + static_cast<Score::RawType>(positionalScore / positionalScoreScale());

  cache_.entry(position.getHash(), score);

  return score;
}

Score Evaluator::estimateScore(Score score,
                               const Position& position,
                               Move move) {
  if (move.isPromotion()) {
    Piece piece = position.getPieceOnBoard(move.from());
    if (position.getTurn() == Turn::Black) {
      score += material::promotionScore(piece);
    } else {
      score -= material::promotionScore(piece);
    }
  }

  Piece captured = position.getPieceOnBoard(move.to());
  if (!captured.isEmpty()) {
    if (position.getTurn() == Turn::Black) {
      score += material::exchangeScore(captured);
    } else {
      score -= material::exchangeScore(captured);
    }
  }

  return score;
}


} // namespace sunfish
