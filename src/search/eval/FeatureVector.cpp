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

const int8_t SqIndexWPawn[] = {
   0,  1,  2,  3,  4,  5,  6,  7, -1,
   8,  9, 10, 11, 12, 13, 14, 15, -1,
  16, 17, 18, 19, 20, 21, 22, 23, -1,
  24, 25, 26, 27, 28, 29, 30, 31, -1,
  32, 33, 34, 35, 36, 37, 38, 39, -1,
  40, 41, 42, 43, 44, 45, 46, 47, -1,
  48, 49, 50, 51, 52, 53, 54, 55, -1,
  56, 57, 58, 59, 60, 61, 62, 63, -1,
  64, 65, 66, 67, 68, 69, 70, 71, -1,
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

const int8_t SqIndexWKnight[] = {
   0,  1,  2,  3,  4,  5,  6, -1, -1,
   7,  8,  9, 10, 11, 12, 13, -1, -1,
  14, 15, 16, 17, 18, 19, 20, -1, -1,
  21, 22, 23, 24, 25, 26, 27, -1, -1,
  28, 29, 30, 31, 32, 33, 34, -1, -1,
  35, 36, 37, 38, 39, 40, 41, -1, -1,
  42, 43, 44, 45, 46, 47, 48, -1, -1,
  49, 50, 51, 52, 53, 54, 55, -1, -1,
  56, 57, 58, 59, 60, 61, 62, -1, -1,
};

struct TableInfo {
  const int8_t* table;
  int begin;
  int end;
};

const TableInfo KpTableInfo[] = {
  { SqIndexBPawn  , KingPiece::BPawn  , KingPiece::WPawn   },
  { SqIndexWPawn  , KingPiece::WPawn  , KingPiece::BLance  },
  { SqIndexBPawn  , KingPiece::BLance , KingPiece::WLance  },
  { SqIndexWPawn  , KingPiece::WLance , KingPiece::BKnight },
  { SqIndexBKnight, KingPiece::BKnight, KingPiece::WKnight },
  { SqIndexWKnight, KingPiece::WKnight, KingPiece::BSilver },
  { nullptr       , KingPiece::BSilver, KingPiece::WSilver },
  { nullptr       , KingPiece::WSilver, KingPiece::BGold   },
  { nullptr       , KingPiece::BGold  , KingPiece::WGold   },
  { nullptr       , KingPiece::WGold  , KingPiece::BBishop },
  { nullptr       , KingPiece::BBishop, KingPiece::WBishop },
  { nullptr       , KingPiece::WBishop, KingPiece::BHorse  },
  { nullptr       , KingPiece::BHorse , KingPiece::WHorse  },
  { nullptr       , KingPiece::WHorse , KingPiece::BRook   },
  { nullptr       , KingPiece::BRook  , KingPiece::WRook   },
  { nullptr       , KingPiece::WRook  , KingPiece::BDragon },
  { nullptr       , KingPiece::BDragon, KingPiece::WDragon },
  { nullptr       , KingPiece::WDragon, KingPiece::End     },
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

int kingPieceIndex(Piece piece, Square square) {
  switch (piece.raw()) {
  case PieceNumber::BPawn     : return KingPiece::BPawn   + SqIndexBPawn[square.raw()];
  case PieceNumber::WPawn     : return KingPiece::WPawn   + SqIndexWPawn[square.raw()];
  case PieceNumber::BLance    : return KingPiece::BLance  + SqIndexBPawn[square.raw()];
  case PieceNumber::WLance    : return KingPiece::WLance  + SqIndexWPawn[square.raw()];
  case PieceNumber::BKnight   : return KingPiece::BKnight + SqIndexBKnight[square.raw()];
  case PieceNumber::WKnight   : return KingPiece::WKnight + SqIndexWKnight[square.raw()];
  case PieceNumber::BSilver   : return KingPiece::BSilver + square.raw();
  case PieceNumber::WSilver   : return KingPiece::WSilver + square.raw();
  case PieceNumber::BGold     : // fall through
  case PieceNumber::WTokin    : // fall through
  case PieceNumber::BProLance : // fall through
  case PieceNumber::WProKnight: // fall through
  case PieceNumber::BProSilver: return KingPiece::BGold   + square.raw();
  case PieceNumber::WGold     : // fall through
  case PieceNumber::BTokin    : // fall through
  case PieceNumber::WProLance : // fall through
  case PieceNumber::BProKnight: // fall through
  case PieceNumber::WProSilver: return KingPiece::WGold   + square.raw();
  case PieceNumber::BBishop   : return KingPiece::BBishop + square.raw();
  case PieceNumber::WBishop   : return KingPiece::WBishop + square.raw();
  case PieceNumber::BRook     : return KingPiece::BRook   + square.raw();
  case PieceNumber::WRook     : return KingPiece::WRook   + square.raw();
  case PieceNumber::BHorse    : return KingPiece::BHorse  + square.raw();
  case PieceNumber::WHorse    : return KingPiece::WHorse  + square.raw();
  case PieceNumber::BDragon   : return KingPiece::BDragon + square.raw();
  case PieceNumber::WDragon   : return KingPiece::WDragon + square.raw();
  }
  ASSERT(false);
  return 0; // unreachable
}

int kingHandIndex(Piece piece, Hand::Type n) {
  switch (piece.raw()) {
  case PieceNumber::BPawn     : return KingHand::BPawn   + n - 1;
  case PieceNumber::WPawn     : return KingHand::WPawn   + n - 1;
  case PieceNumber::BLance    : return KingHand::BLance  + n - 1;
  case PieceNumber::WLance    : return KingHand::WLance  + n - 1;
  case PieceNumber::BKnight   : return KingHand::BKnight + n - 1;
  case PieceNumber::WKnight   : return KingHand::WKnight + n - 1;
  case PieceNumber::BSilver   : return KingHand::BSilver + n - 1;
  case PieceNumber::WSilver   : return KingHand::WSilver + n - 1;
  case PieceNumber::BGold     : return KingHand::BGold   + n - 1;
  case PieceNumber::WGold     : return KingHand::WGold   + n - 1;
  case PieceNumber::BBishop   : return KingHand::BBishop + n - 1;
  case PieceNumber::WBishop   : return KingHand::WBishop + n - 1;
  case PieceNumber::BRook     : return KingHand::BRook   + n - 1;
  case PieceNumber::WRook     : return KingHand::WRook   + n - 1;
  }
  ASSERT(false);
  return 0; // unreachable
}

int kingGoldIndex(Direction dir) {
  switch (dir) {
  case Direction::LeftUp   : return KingGold::LeftUpGold;
  case Direction::Up       : return KingGold::UpGold;
  case Direction::RightUp  : return KingGold::RightUpGold;
  case Direction::Left     : return KingGold::LeftGold;
  case Direction::Right    : return KingGold::RightGold;
  case Direction::LeftDown : return KingGold::LeftDownGold;
  case Direction::Down     : return KingGold::DownGold;
  case Direction::RightDown: return KingGold::RightDownGold;
  default:
    ASSERT(false);
    return 0;
  }
}

int kingSilverIndex(Direction dir) {
  switch (dir) {
  case Direction::LeftUp   : return KingGold::LeftUpSilver;
  case Direction::Up       : return KingGold::UpSilver;
  case Direction::RightUp  : return KingGold::RightUpSilver;
  case Direction::Left     : return KingGold::LeftSilver;
  case Direction::Right    : return KingGold::RightSilver;
  case Direction::LeftDown : return KingGold::LeftDownSilver;
  case Direction::Down     : return KingGold::DownSilver;
  case Direction::RightDown: return KingGold::RightDownSilver;
  default:
    ASSERT(false);
    return 0;
  }
}

int symmetricalKingPieceIndex(int index) {
  auto tableSize = sizeof(KpTableInfo) / sizeof(KpTableInfo[0]);
  for (unsigned i = 0; i < tableSize; i++) {
    const auto& ti = KpTableInfo[i];
    if (ti.begin <= index && index < ti.end) {
      auto square = indexToSquare(ti.table, index - ti.begin);
      return ti.begin + squareToIndex(ti.table, square.hsym());
    }
  }
  ASSERT(false);
  return 0;
}

int symmetricalKingGoldIndex(int index) {
  switch (index) {
  case KingGold::LeftUpGold     : return KingGold::RightUpGold;
  case KingGold::UpGold         : return KingGold::UpGold;
  case KingGold::RightUpGold    : return KingGold::LeftUpGold;
  case KingGold::LeftGold       : return KingGold::RightGold;
  case KingGold::RightGold      : return KingGold::LeftGold;
  case KingGold::LeftDownGold   : return KingGold::RightDownGold;
  case KingGold::DownGold       : return KingGold::DownGold;
  case KingGold::RightDownGold  : return KingGold::LeftDownGold;
  case KingGold::LeftUpSilver   : return KingGold::RightUpSilver;
  case KingGold::UpSilver       : return KingGold::UpSilver;
  case KingGold::RightUpSilver  : return KingGold::LeftUpSilver;
  case KingGold::LeftSilver     : return KingGold::RightSilver;
  case KingGold::RightSilver    : return KingGold::LeftSilver;
  case KingGold::LeftDownSilver : return KingGold::RightDownSilver;
  case KingGold::DownSilver     : return KingGold::DownSilver;
  case KingGold::RightDownSilver: return KingGold::LeftDownSilver;
  }
  ASSERT(false);
  return 0;
}

} // namespace sunfish
