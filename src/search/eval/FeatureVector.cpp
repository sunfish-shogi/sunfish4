/* FeatureVector.cpp
 *
 * Kubo Ryosuke
 */

#include "search/eval/FeatureVector.hpp"
#include "core/position/Position.hpp"
#include "core/move/MoveTables.hpp"
#include <cstdint>

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
  case PieceNumber::BTokin    : // fall through
  case PieceNumber::BProLance : // fall through
  case PieceNumber::BProKnight: // fall through
  case PieceNumber::BProSilver: return EvalPieceIndex::BGold;
  case PieceNumber::WGold     : // fall through
  case PieceNumber::WTokin    : // fall through
  case PieceNumber::WProLance : // fall through
  case PieceNumber::WProKnight: // fall through
  case PieceNumber::WProSilver: return EvalPieceIndex::WGold;
  case PieceNumber::BBishop   : return EvalPieceIndex::BBishop;
  case PieceNumber::WBishop   : return EvalPieceIndex::WBishop;
  case PieceNumber::BHorse    : return EvalPieceIndex::BHorse;
  case PieceNumber::WHorse    : return EvalPieceIndex::WHorse;
  case PieceNumber::BRook     : return EvalPieceIndex::BRook;
  case PieceNumber::WRook     : return EvalPieceIndex::WRook;
  case PieceNumber::BDragon   : return EvalPieceIndex::BDragon;
  case PieceNumber::WDragon   : return EvalPieceIndex::WDragon;
  }
  ASSERT(false);
  return 0; // unreachable
}

namespace {

CONSTEXPR_CONST int8_t square5x5Index[41] = {
  /*  0 -  8 */  0,  1,  2,  3,  4, -1, -1, -1, -1,
  /*  9 - 17 */  5,  6,  7,  8,  9, -1, -1, -1, -1,
  /* 18 - 26 */ 10, 11, -1, 12, 13, -1, -1, -1, -1,
  /* 27 - 35 */  5,  6,  7,  8,  9, -1, -1, -1, -1,
  /* 36 - 40 */  0,  1,  2,  3,  4,
};

CONSTEXPR_CONST int8_t safetyBPawn[14] = {
    0,  0,  0,  0,  0,
    1,  2,  1,  1,  0,
    1,  2,      1,  0
};

CONSTEXPR_CONST int8_t safetyBLance[14] = {
    1,  0,  0,  0,  0,
    1,  1,  1,  1,  1,
    1,  1,      1,  1,
};

CONSTEXPR_CONST int8_t safetyBKnight[14] = {
    0,  0,  0,  0,  0,
    0,  0,  1,  1,  1,
    0,  1,      2,  1,
};

CONSTEXPR_CONST int8_t safetyBSilver[14] = {
    1,  2,  2,  1,  1,
    1,  2,  3,  2,  1,
    1,  3,      3,  1,
};

CONSTEXPR_CONST int8_t safetyBGold[14] = {
    1,  2,  3,  2,  1,
    1,  2,  4,  3,  1,
    1,  2,      3,  1,
};

CONSTEXPR_CONST int8_t safetyBBishop[14] = {
    1,  1,  1,  1,  1,
    1,  2,  1,  2,  1,
    1,  1,      1,  1,
};

CONSTEXPR_CONST int8_t safetyBRook[14] = {
    1,  1,  1,  2,  1,
    1,  1,  1,  2,  1,
    1,  1,      1,  1,
};

CONSTEXPR_CONST int8_t safetyBHorse[14] = {
    1,  3,  3,  2,  1,
    2,  4,  4,  3,  2,
    1,  3,      2,  1,
};

CONSTEXPR_CONST int8_t safetyBDragon[14] = {
    1,  3,  3,  3,  1,
    3,  4,  4,  4,  3,
    1,  3,      2,  1,
};

CONSTEXPR_CONST int8_t safetyWPawn[14] = {
   -1, -2, -1, -1,  0,
   -2, -3, -1, -1,  0,
   -3, -4,     -1,  0,
};

CONSTEXPR_CONST int8_t safetyWLance[14] = {
    0, -1, -1, -1,  0,
   -1, -3, -1, -1,  0,
   -2, -3,     -1,  0,
};

CONSTEXPR_CONST int8_t safetyWKnight[14] = {
   -2, -2, -1,  0,  0,
   -4, -2, -1,  0,  0,
   -3, -3,      0,  0,
};

CONSTEXPR_CONST int8_t safetyWSilver[14] = {
   -3, -3, -3, -2, -2,
   -4, -5, -4, -4, -3,
   -4, -5,     -3, -3,
};

CONSTEXPR_CONST int8_t safetyWGold[14] = {
   -3, -4, -4, -1,  0,
   -4, -5, -5, -4, -2,
   -4, -5,     -4, -2,
};

CONSTEXPR_CONST int8_t safetyWBishop[14] = {
   -1, -3, -2, -3, -3,
   -3, -4, -4, -4, -3,
   -1, -3,     -3, -1,
};

CONSTEXPR_CONST int8_t safetyWRook[14] = {
   -1, -3, -3, -3, -1,
   -3, -4, -5, -4, -3,
   -3, -5,     -5, -3,
};

CONSTEXPR_CONST int8_t safetyWHorse[14] = {
   -3, -4, -3, -4, -3,
   -4, -5, -5, -5, -4,
   -3, -5,     -5, -3,
};

CONSTEXPR_CONST int8_t safetyWDragon[14] = {
   -3, -4, -4, -4, -3,
   -4, -5, -5, -5, -4,
   -4, -5,     -5, -4,
};

} // namespace

template <Turn turn>
int safetyOfKing(const Position& pos) {
  int safety = 0;

  auto king = turn == Turn::Black ? pos.getBlackKingSquare() : pos.getWhiteKingSquare();
  auto bb = MoveTables::neighbor5x5(king);

  BB_EACH(square, bb) {
    int rel = (int)square.raw() - (int)king.raw();
    if (turn == Turn::White) {
      rel = -rel;
    }
    ASSERT(rel >= -20);
    ASSERT(rel <= 20);
    int index = square5x5Index[rel + 20];
    ASSERT(index >= 0);
    ASSERT(index <= 13);

    Piece piece = pos.getPieceOnBoard(square);

    if (turn == Turn::Black) {
      switch (piece.raw()) {
        case PieceNumber::BPawn     : safety += safetyBPawn  [index]; break;
        case PieceNumber::BLance    : safety += safetyBLance [index]; break;
        case PieceNumber::BKnight   : safety += safetyBKnight[index]; break;
        case PieceNumber::BSilver   : safety += safetyBSilver[index]; break;
        case PieceNumber::BGold     : // fall-through
        case PieceNumber::BTokin    : // fall-through
        case PieceNumber::BProLance : // fall-through
        case PieceNumber::BProKnight: // fall-through
        case PieceNumber::BProSilver: safety += safetyBGold  [index]; break;
        case PieceNumber::BBishop   : safety += safetyBBishop[index]; break;
        case PieceNumber::BRook     : safety += safetyBRook  [index]; break;
        case PieceNumber::BHorse    : safety += safetyBHorse [index]; break;
        case PieceNumber::BDragon   : safety += safetyBDragon[index]; break;
        case PieceNumber::WPawn     : safety += safetyWPawn  [index]; break;
        case PieceNumber::WLance    : safety += safetyWLance [index]; break;
        case PieceNumber::WKnight   : safety += safetyWKnight[index]; break;
        case PieceNumber::WSilver   : safety += safetyWSilver[index]; break;
        case PieceNumber::WGold     : // fall-through
        case PieceNumber::WTokin    : // fall-through
        case PieceNumber::WProLance : // fall-through
        case PieceNumber::WProKnight: // fall-through
        case PieceNumber::WProSilver: safety += safetyWGold  [index]; break;
        case PieceNumber::WBishop   : safety += safetyWBishop[index]; break;
        case PieceNumber::WRook     : safety += safetyWRook  [index]; break;
        case PieceNumber::WHorse    : safety += safetyWHorse [index]; break;
        case PieceNumber::WDragon   : safety += safetyWDragon[index]; break;
      }
    } else {
      switch (piece.raw()) {
        case PieceNumber::BPawn     : safety += safetyWPawn  [index]; break;
        case PieceNumber::BLance    : safety += safetyWLance [index]; break;
        case PieceNumber::BKnight   : safety += safetyWKnight[index]; break;
        case PieceNumber::BSilver   : safety += safetyWSilver[index]; break;
        case PieceNumber::BGold     : // fall-through
        case PieceNumber::BTokin    : // fall-through
        case PieceNumber::BProLance : // fall-through
        case PieceNumber::BProKnight: // fall-through
        case PieceNumber::BProSilver: safety += safetyWGold  [index]; break;
        case PieceNumber::BBishop   : safety += safetyWBishop[index]; break;
        case PieceNumber::BRook     : safety += safetyWRook  [index]; break;
        case PieceNumber::BHorse    : safety += safetyWHorse [index]; break;
        case PieceNumber::BDragon   : safety += safetyWDragon[index]; break;
        case PieceNumber::WPawn     : safety += safetyBPawn  [index]; break;
        case PieceNumber::WLance    : safety += safetyBLance [index]; break;
        case PieceNumber::WKnight   : safety += safetyBKnight[index]; break;
        case PieceNumber::WSilver   : safety += safetyBSilver[index]; break;
        case PieceNumber::WGold     : // fall-through
        case PieceNumber::WTokin    : // fall-through
        case PieceNumber::WProLance : // fall-through
        case PieceNumber::WProKnight: // fall-through
        case PieceNumber::WProSilver: safety += safetyBGold  [index]; break;
        case PieceNumber::WBishop   : safety += safetyBBishop[index]; break;
        case PieceNumber::WRook     : safety += safetyBRook  [index]; break;
        case PieceNumber::WHorse    : safety += safetyBHorse [index]; break;
        case PieceNumber::WDragon   : safety += safetyBDragon[index]; break;
      }
    }
  }

  safety = safety / 2;
  safety = std::min(safety, (int)MaxKingSafety);
  safety = std::max(safety, (int)MinKingSafety);

  return safety;
}
template int safetyOfKing<Turn::Black>(const Position& pos);
template int safetyOfKing<Turn::White>(const Position& pos);

} // namespace sunfish
