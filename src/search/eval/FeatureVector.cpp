/* FeatureVector.cpp
 *
 * Kubo Ryosuke
 */

#include "search/eval/FeatureVector.hpp"
#include <cstdint>

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

} // namespace

namespace sunfish {

int kkpBoardIndex(PieceType pieceType, Square square) {
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

int kkpHandIndex(PieceType pieceType, Hand::Type n) {
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

} // namespace sunfish
