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

} // namespace

namespace sunfish {

int getEvalPieceIndex(Piece piece) {
  switch (piece.raw()) {
  case PieceNumber::BPawn     : return EvalPieceIndex::BPawn;
  case PieceNumber::WPawn     : return EvalPieceIndex::WPawn;
  case PieceNumber::BLance    : return EvalPieceIndex::BLance;
  case PieceNumber::WLance    : return EvalPieceIndex::WLance;
  case PieceNumber::BKnight   : return EvalPieceIndex::BKnight;
  case PieceNumber::WKnight   : return EvalPieceIndex::WKnight;
  case PieceNumber::BSilver   : return EvalPieceIndex::BSilver;
  case PieceNumber::WSilver   : return EvalPieceIndex::WSilver;
  case PieceNumber::BGold     : // fall through
  case PieceNumber::WTokin    : // fall through
  case PieceNumber::BProLance : // fall through
  case PieceNumber::WProKnight: // fall through
  case PieceNumber::BProSilver: return EvalPieceIndex::BGold;
  case PieceNumber::WGold     : // fall through
  case PieceNumber::BTokin    : // fall through
  case PieceNumber::WProLance : // fall through
  case PieceNumber::BProKnight: // fall through
  case PieceNumber::WProSilver: return EvalPieceIndex::WGold;
  case PieceNumber::BBishop   : return EvalPieceIndex::BBishop;
  case PieceNumber::WBishop   : return EvalPieceIndex::WBishop;
  case PieceNumber::BRook     : return EvalPieceIndex::BRook;
  case PieceNumber::WRook     : return EvalPieceIndex::WRook;
  case PieceNumber::BHorse    : return EvalPieceIndex::BHorse;
  case PieceNumber::WHorse    : return EvalPieceIndex::WHorse;
  case PieceNumber::BDragon   : return EvalPieceIndex::BDragon;
  case PieceNumber::WDragon   : return EvalPieceIndex::WDragon;
  }
  ASSERT(false);
  return 0; // unreachable
}

} // namespace sunfish
