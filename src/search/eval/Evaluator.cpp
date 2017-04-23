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
#include <memory>
#include <cstring>

namespace {

const char* const EvalBin = "eval.bin";

CONSTEXPR_CONST Score EnteringKing = 1000;

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
    if (!load(*this)) {
      initializeZero();
    }
    break;

  case InitType::Zero:
    initializeZero();
    break;
  }
}

void Evaluator::initializeZero() {
  memset(reinterpret_cast<void*>(&ofv_), 0, sizeof(ofv_));
  onChanged(DataSourceType::Zero);
}

void Evaluator::onChanged(DataSourceType dataSourceType) {
  cache_.clear();
  dataSourceType_ = dataSourceType;
}

Score Evaluator::calculateMaterialScore(const Position& position) const {
  Score score = Score::zero();

  auto& blackHand = position.getBlackHand();
  score += material::pawn()   * blackHand.get(PieceType::pawn  ());
  score += material::lance()  * blackHand.get(PieceType::lance ());
  score += material::knight() * blackHand.get(PieceType::knight());
  score += material::silver() * blackHand.get(PieceType::silver());
  score += material::gold()   * blackHand.get(PieceType::gold  ());
  score += material::bishop() * blackHand.get(PieceType::bishop());
  score += material::rook()   * blackHand.get(PieceType::rook  ());

  auto& whiteHand = position.getWhiteHand();
  score -= material::pawn()   * whiteHand.get(PieceType::pawn  ());
  score -= material::lance()  * whiteHand.get(PieceType::lance ());
  score -= material::knight() * whiteHand.get(PieceType::knight());
  score -= material::silver() * whiteHand.get(PieceType::silver());
  score -= material::gold()   * whiteHand.get(PieceType::gold  ());
  score -= material::bishop() * whiteHand.get(PieceType::bishop());
  score -= material::rook()   * whiteHand.get(PieceType::rook  ());

  Bitboard occ = position.getBOccupiedBitboard() | position.getWOccupiedBitboard();
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

  if (position.getBlackKingSquare().getRank() <= 3) {
    score += EnteringKing;
  }

  if (position.getWhiteKingSquare().getRank() >= 7) {
    score -= EnteringKing;
  }

  return score;
}

Score Evaluator::calculateMaterialScoreDiff(Score score,
                                            const Position& position,
                                            Move move,
                                            Piece captured) const {
  Piece piece = position.getPieceOnBoard(move.to());
  if (move.isPromotion()) {
    if (position.getTurn() == Turn::White) {
      score += material::promotionScore(piece);
    } else {
      score -= material::promotionScore(piece);
    }
  } else if (piece == Piece::blackKing()) {
    if (move.from().getRank() >= 4 && move.to().getRank() <= 3) {
      score += EnteringKing;
    } else if (move.from().getRank() <= 3 && move.to().getRank() >= 4) {
      score -= EnteringKing;
    }
  } else if (piece == Piece::whiteKing()) {
    if (move.from().getRank() <= 6 && move.to().getRank() >= 7) {
      score -= EnteringKing;
    } else if (move.from().getRank() >= 7 && move.to().getRank() <= 6) {
      score += EnteringKing;
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

Score Evaluator::calculatePositionalScore(const Position& position) {
  int32_t score = operate<FeatureOperationType::Evaluate>
                         (ofv_, position, 0);
  return static_cast<Score::RawType>(score / positionalScoreScale());
}

Score Evaluator::calculateTotalScore(Score materialScore,
                                     const Position& position) {
  Score score;
  if (cache_.check(position.getHash(), score)) {
    return score;
  }

  auto positionalScore = calculatePositionalScore(position);
  score = materialScore + positionalScore;

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

bool load(const char* path, Evaluator::FVType& fv) {
  std::ifstream file(path, std::ios::in | std::ios::binary);
  if (!file) {
    LOG(warning) << "failed to open: " << path;
    return false;
  }

  char ver[32] = { 0 };
  uint8_t len;
  file.read(reinterpret_cast<char*>(&len), sizeof(len));
  if (len >= sizeof(ver)) {
    LOG(warning) << "invalid feature vector version";
    return false;
  }
  file.read(reinterpret_cast<char*>(ver), len);
  if (strcmp(SUNFISH_FV_VERSION, ver) != 0) {
    LOG(warning) << "invalid feature vector version: " << ver;
    return false;
  }

  memset(reinterpret_cast<void*>(&fv), 0, sizeof(Evaluator::FVType));

  file.read(reinterpret_cast<char*>(&fv), sizeof(Evaluator::FVType));

  file.close();

  return true;
}

bool load(Evaluator::FVType& fv) {
  return load(EvalBin, fv);
}

bool load(const char* path, Evaluator& eval) {
  auto fv = std::unique_ptr<Evaluator::FVType>(new Evaluator::FVType);
  load(path, *fv.get());
  optimize(*fv, eval.ofv());
  eval.onChanged(Evaluator::DataSourceType::EvalBin);
  return true;
}

bool load(Evaluator& eval) {
  return load(EvalBin, eval);
}

bool save(const char* path, const Evaluator::FVType& fv) {
  std::ofstream file(path, std::ios::out | std::ios::binary);
  if (!file) {
    LOG(warning) << "failed to open: " << path;
    return false;
  }

  const char* ver = SUNFISH_FV_VERSION;
  uint8_t len = strlen(ver);
  file.write(reinterpret_cast<const char*>(&len), sizeof(len));
  file.write(reinterpret_cast<const char*>(ver), len);

  file.write(reinterpret_cast<const char*>(&fv), sizeof(Evaluator::FVType));

  file.close();

  return true;
}

bool save(const Evaluator::FVType& fv) {
  return save(EvalBin, fv);
}

} // namespace sunfish
