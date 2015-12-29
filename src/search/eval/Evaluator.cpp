/* Evaluator.cpp
 *
 * Kubo Ryosuke
 */

#include "search/eval/Evaluator.hpp"
#include "search/eval/Material.hpp"
#include "logger/Logger.hpp"
#include <fstream>
#include <mutex>
#include <cstring>

namespace {

using namespace sunfish;

const int8_t SqIndexBPawn[] = {
  -1,  0,  1,  2,  3,  4,  5,  6,  7,
  -1,  8,  9, 10, 11, 12, 13, 14, 15,
  -1, 16, 17, 18, 19, 20, 21, 22, 23,
  -1, 24, 25, 26, 27, 28, 29, 30, 31,
  -1, 32, 33, 34, 35, 36, 37, 38, 39,
  -1, 40, 41, 42, 43, 44, 45, 46, 47,
  -1, 48, 49, 50, 51, 52, 53, 54, 55,
  -1, 56, 57, 58, 59, 60, 61, 62, 63,
  -1, 64, 65, 66, 67, 68, 69, 70, 71,
};

const int8_t SqIndexBKnight[] = {
  -1, -1,  0,  1,  2,  3,  4,  5,  6,
  -1, -1,  7,  8,  9, 10, 11, 12, 13,
  -1, -1, 14, 15, 16, 17, 18, 19, 20,
  -1, -1, 21, 22, 23, 24, 25, 26, 27,
  -1, -1, 28, 29, 30, 31, 32, 33, 34,
  -1, -1, 35, 36, 37, 38, 39, 40, 41,
  -1, -1, 42, 43, 44, 45, 46, 47, 48,
  -1, -1, 49, 50, 51, 52, 53, 54, 55,
  -1, -1, 56, 57, 58, 59, 60, 61, 62,
};

struct TableInfo {
  const int8_t* table;
  int begin;
  int end;
};

const TableInfo KkpTableInfo[] = {
  { SqIndexBPawn  , KKP::BPawn  , KKP::BLance  },
  { SqIndexBPawn  , KKP::BLance , KKP::BKnight },
  { SqIndexBKnight, KKP::BKnight, KKP::BSilver },
  { nullptr       , KKP::BSilver, KKP::BGold   },
  { nullptr       , KKP::BGold  , KKP::BBishop },
  { nullptr       , KKP::BBishop, KKP::BHorse  },
  { nullptr       , KKP::BHorse , KKP::BRook   },
  { nullptr       , KKP::BRook  , KKP::BDragon },
  { nullptr       , KKP::BDragon, KKP::End     },
};

int squareToIndex(const int8_t* table, Square square) {
  return table == nullptr ? square.raw() : table[square.raw()];
}

Square indexToSquare(const int8_t* table, int index) {
  if (table == nullptr) {
    return Square(index);
  }

  SQUARE_EACH(square) {
    if (static_cast<int>(table[square.raw()]) == index) {
      return square;
    }
  }

  ASSERT(false);
  return Square::invalid();
}

inline int kkpBoardIndex(PieceType pieceType, Square square) {
  switch (pieceType.raw()) {
  case PieceNumber::Pawn     : return KKP::BPawn   + SqIndexBPawn[square.raw()];
  case PieceNumber::Lance    : return KKP::BLance  + SqIndexBPawn[square.raw()];
  case PieceNumber::Knight   : return KKP::BKnight + SqIndexBKnight[square.raw()];
  case PieceNumber::Silver   : return KKP::BSilver + square.raw();
  case PieceNumber::Gold     : // fall through
  case PieceNumber::Tokin    : // fall through
  case PieceNumber::ProLance : // fall through
  case PieceNumber::ProKnight: // fall through
  case PieceNumber::ProSilver: return KKP::BGold   + square.raw();
  case PieceNumber::Bishop   : return KKP::BBishop + square.raw();
  case PieceNumber::Rook     : return KKP::BRook   + square.raw();
  case PieceNumber::Horse    : return KKP::BHorse  + square.raw();
  case PieceNumber::Dragon   : return KKP::BDragon + square.raw();
  }
  ASSERT(false);
  return 0; // unreachable
}

inline int kkpHandIndex(PieceType pieceType, Hand::Type n) {
  switch (pieceType.raw()) {
  case PieceNumber::Pawn     : return KKP::HPawn   + n - 1;
  case PieceNumber::Lance    : return KKP::HLance  + n - 1;
  case PieceNumber::Knight   : return KKP::HKnight + n - 1;
  case PieceNumber::Silver   : return KKP::HSilver + n - 1;
  case PieceNumber::Gold     : return KKP::HGold   + n - 1;
  case PieceNumber::Bishop   : return KKP::HBishop + n - 1;
  case PieceNumber::Rook     : return KKP::HRook   + n - 1;
  }
  ASSERT(false);
  return 0; // unreachable
}

} // namespace

namespace sunfish {

int symmetricalKkpIndex(int index) {
  if (index < KKP::BPawn) {
    return index;
  }

  auto tableSize = sizeof(KkpTableInfo) / sizeof(KkpTableInfo[0]);
  for (unsigned i = 0; i < tableSize; i++) {
    const auto& ti = KkpTableInfo[i];
    if (ti.begin <= index && index < ti.end) {
      auto square = indexToSquare(ti.table, index - ti.begin);
      return ti.begin + squareToIndex(ti.table, square.hsym());
    }
  }
  ASSERT(false);
  return 0;
}

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
  memset((void*)kkp_, 0, sizeof(kkp_));
  onChanged();
}

bool Evaluator::read(const char* path) {
  std::ifstream file(path, std::ios::in | std::ios::binary);
  if (!file) {
    return false;
  }

  file.read(reinterpret_cast<char*>(kkp_), sizeof(kkp_));
  file.close();

  onChanged();

  return true;
}

bool Evaluator::write(const char* path) const {
  std::ofstream file(path, std::ios::out | std::ios::binary);
  if (!file) {
    return false;
  }

  file.write(reinterpret_cast<const char*>(kkp_), sizeof(kkp_));
  file.close();

  return true;
}

void Evaluator::onChanged() {
  // generate kkpc
#define GEN_KKPC_HAND(T, n) do { \
  Type cum = 0; \
  for (int i = 0; i < n; i++) { \
    cum += kkp_[bking.raw()][wking.raw()][KKP::H ## T + i]; \
  } \
} while (false)
  SQUARE_EACH(bking) {
    SQUARE_EACH(wking) {
      GEN_KKPC_HAND(Pawn  , 18);
      GEN_KKPC_HAND(Lance ,  4);
      GEN_KKPC_HAND(Knight,  4);
      GEN_KKPC_HAND(Silver,  4);
      GEN_KKPC_HAND(Gold  ,  4);
      GEN_KKPC_HAND(Bishop,  4);
      GEN_KKPC_HAND(Rook  ,  4);
    }
  }
#undef GEN_KKPC_HAND
}

ClassifiedScores Evaluator::evaluate(const Position& position) const {
  ClassifiedScores scores = { Score::zero(), Score::zero() };
  auto bking = position.getBlackKingSquare().raw();
  auto wking = position.getWhiteKingSquare().raw();
  auto rbking = position.getWhiteKingSquare().dsym().raw();
  auto rwking = position.getBlackKingSquare().dsym().raw();

#define CALC_BLACK_HAND(t, T) do { \
  auto n = blackHand.get(PieceType::t()); \
  if (n != 0) { \
    scores.materialScore += material::T * n; \
    scores.positionalScore += kkpc_[bking][wking][KKP::H ## T + n - 1]; \
  } \
} while (false)
  auto& blackHand = position.getBlackHand();
  CALC_BLACK_HAND(pawn  , Pawn  );
  CALC_BLACK_HAND(lance , Lance );
  CALC_BLACK_HAND(knight, Knight);
  CALC_BLACK_HAND(silver, Silver);
  CALC_BLACK_HAND(gold  , Gold  );
  CALC_BLACK_HAND(bishop, Bishop);
  CALC_BLACK_HAND(rook  , Rook  );
#undef CALC_BLACK_HAND

#define CALC_WHITE_HAND(t, T) do { \
  auto n = whiteHand.get(PieceType::t()); \
  if (n != 0) { \
    scores.materialScore -= material::T * n; \
    scores.positionalScore -= kkpc_[rbking][rwking][KKP::H ## T + n - 1]; \
  } \
} while (false)
  auto& whiteHand = position.getWhiteHand();
  CALC_WHITE_HAND(pawn  , Pawn  );
  CALC_WHITE_HAND(lance , Lance );
  CALC_WHITE_HAND(knight, Knight);
  CALC_WHITE_HAND(silver, Silver);
  CALC_WHITE_HAND(gold  , Gold  );
  CALC_WHITE_HAND(bishop, Bishop);
  CALC_WHITE_HAND(rook  , Rook  );
#undef CALC_WHITE_HAND

  Bitboard occ = nosseOr(position.getBOccupiedBitboard(),
                         position.getWOccupiedBitboard());
  occ.unset(position.getBlackKingSquare());
  occ.unset(position.getWhiteKingSquare());

  BB_EACH(square, occ) {
    auto piece = position.getPieceOnBoard(square);
    auto pieceType = piece.type();

    if (piece.isBlack()) {
      scores.materialScore += material::score(piece);
      scores.positionalScore += kkp_[bking][wking][kkpBoardIndex(pieceType, square)];
    } else {
      scores.materialScore -= material::score(piece);
      scores.positionalScore -= kkp_[rbking][rwking][kkpBoardIndex(pieceType, square.dsym())];
    }
  }

  return scores;
}

template <Turn turn>
ClassifiedScores Evaluator::evaluateDiff(ClassifiedScores scores,
                                         const Position& position,
                                         Move move,
                                         Piece captured) const {
  auto bking = position.getBlackKingSquare().raw();
  auto wking = position.getWhiteKingSquare().raw();
  auto rbking = position.getWhiteKingSquare().dsym().raw();
  auto rwking = position.getBlackKingSquare().dsym().raw();

  if (move.isDrop()) {
    PieceType pieceType = move.droppingPieceType();
    if (turn == Turn::Black) {
      auto n = position.getBlackHandPieceCount(pieceType) + 1;
      scores.positionalScore -= kkp_[bking][wking][kkpHandIndex(pieceType, n)];
      scores.positionalScore += kkp_[bking][wking][kkpBoardIndex(pieceType, move.to())];
    } else {
      auto n = position.getWhiteHandPieceCount(pieceType) + 1;
      scores.positionalScore += kkp_[rbking][rwking][kkpHandIndex(pieceType, n)];
      scores.positionalScore -= kkp_[rbking][rwking][kkpBoardIndex(pieceType, move.to().dsym())];
    }

    goto skip_capture;
 
  } else if (move.isPromotion()) {
    Piece piece = position.getPieceOnBoard(move.to());
    Piece pieceBefore = piece.unpromote();

    if (turn == Turn::Black) {
      scores.materialScore += material::promotionScore(piece);
      scores.positionalScore -= kkp_[bking][wking][kkpBoardIndex(pieceBefore.type(), move.from())];
      scores.positionalScore += kkp_[bking][wking][kkpBoardIndex(piece.type(), move.to())];
    } else {
      scores.materialScore -= material::promotionScore(piece);
      scores.positionalScore += kkp_[rbking][rwking][kkpBoardIndex(pieceBefore.type(), move.from().dsym())];
      scores.positionalScore -= kkp_[rbking][rwking][kkpBoardIndex(piece.type(), move.to().dsym())];
    }

  } else {
    Piece piece = position.getPieceOnBoard(move.to());

    if (piece.type() == PieceType::king()) {
      return evaluate(position);
    }

    if (turn == Turn::Black) {
      scores.materialScore -= kkp_[bking][wking][kkpBoardIndex(piece.type(), move.from())];
      scores.positionalScore += kkp_[bking][wking][kkpBoardIndex(piece.type(), move.to())];
    } else {
      scores.materialScore += kkp_[rbking][rwking][kkpBoardIndex(piece.type(), move.from().dsym())];
      scores.positionalScore -= kkp_[rbking][rwking][kkpBoardIndex(piece.type(), move.to().dsym())];
    }
  }

  if (!captured.isEmpty()) {
    if (turn == Turn::Black) {
      scores.materialScore += material::exchangeScore(captured);
      scores.positionalScore += kkp_[rbking][rwking][kkpBoardIndex(captured.type(), move.from().dsym())];
      auto n = position.getBlackHandPieceCount(captured.type());
      scores.positionalScore += kkp_[bking][wking][kkpHandIndex(captured.type(), n)];
    } else {
      scores.materialScore -= material::exchangeScore(captured);
      scores.positionalScore -= kkp_[bking][wking][kkpBoardIndex(captured.type(), move.from())];
      auto n = position.getWhiteHandPieceCount(captured.type());
      scores.positionalScore -= kkp_[rbking][rwking][kkpHandIndex(captured.type(), n)];
    }
  }

skip_capture:

  return scores;
}
template
ClassifiedScores
Evaluator::evaluateDiff<Turn::Black>(ClassifiedScores,
                                     const Position&, Move,
                                     Piece) const;
template
ClassifiedScores
Evaluator::evaluateDiff<Turn::White>(ClassifiedScores,
                                     const Position&, Move,
                                     Piece) const;

} // namespace sunfish
