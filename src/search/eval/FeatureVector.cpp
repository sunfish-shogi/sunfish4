/* FeatureVector.cpp
 *
 * Kubo Ryosuke
 */

#include "search/eval/FeatureVector.hpp"
#include "core/position/Position.hpp"
#include "core/move/MoveTables.hpp"
#include <cstdint>

namespace {

using namespace sunfish;

const int EvalPieceIndexTable[] = {
  /*  0 PieceNumber::BPawn      */ EvalPieceIndex::BPawn,
  /*  1 PieceNumber::BLance     */ EvalPieceIndex::BLance,
  /*  2 PieceNumber::BKnight    */ EvalPieceIndex::BKnight,
  /*  3 PieceNumber::BSilver    */ EvalPieceIndex::BSilver,
  /*  4 PieceNumber::BGold      */ EvalPieceIndex::BGold,
  /*  5 PieceNumber::BBishop    */ EvalPieceIndex::BBishop,
  /*  6 PieceNumber::BRook      */ EvalPieceIndex::BRook,
  /*  7                         */ 0,
  /*  8 PieceNumber::BTokin     */ EvalPieceIndex::BGold,
  /*  9 PieceNumber::BProLance  */ EvalPieceIndex::BGold,
  /* 10 PieceNumber::BProKnight */ EvalPieceIndex::BGold,
  /* 11 PieceNumber::BProSilver */ EvalPieceIndex::BGold,
  /* 12                         */ 0,
  /* 13 PieceNumber::BHorse     */ EvalPieceIndex::BHorse,
  /* 14 PieceNumber::BDragon    */ EvalPieceIndex::BDragon,
  /* 15                         */ 0,
  /* 16 PieceNumber::WPawn      */ EvalPieceIndex::WPawn,
  /* 17 PieceNumber::WLance     */ EvalPieceIndex::WLance,
  /* 18 PieceNumber::WKnight    */ EvalPieceIndex::WKnight,
  /* 19 PieceNumber::WSilver    */ EvalPieceIndex::WSilver,
  /* 20 PieceNumber::WGold      */ EvalPieceIndex::WGold,
  /* 21 PieceNumber::WBishop    */ EvalPieceIndex::WBishop,
  /* 22 PieceNumber::WRook      */ EvalPieceIndex::WRook,
  /* 23                         */ 0,
  /* 24 PieceNumber::WTokin     */ EvalPieceIndex::WGold,
  /* 25 PieceNumber::WProLance  */ EvalPieceIndex::WGold,
  /* 26 PieceNumber::WProKnight */ EvalPieceIndex::WGold,
  /* 27 PieceNumber::WProSilver */ EvalPieceIndex::WGold,
  /* 28                         */ 0,
  /* 29 PieceNumber::WHorse     */ EvalPieceIndex::WHorse,
  /* 30 PieceNumber::WDragon    */ EvalPieceIndex::WDragon,
  /* 31                         */ 0,
};

const int EvalPieceTypeIndexTable[] = {
  /*  0 PieceNumber::Pawn      */ EvalPieceTypeIndex::Pawn,
  /*  1 PieceNumber::Lance     */ EvalPieceTypeIndex::Lance,
  /*  2 PieceNumber::Knight    */ EvalPieceTypeIndex::Knight,
  /*  3 PieceNumber::Silver    */ EvalPieceTypeIndex::Silver,
  /*  4 PieceNumber::Gold      */ EvalPieceTypeIndex::Gold,
  /*  5 PieceNumber::Bishop    */ EvalPieceTypeIndex::Bishop,
  /*  6 PieceNumber::Rook      */ EvalPieceTypeIndex::Rook,
  /*  7                        */ 0,
  /*  8 PieceNumber::Tokin     */ EvalPieceTypeIndex::Gold,
  /*  9 PieceNumber::ProLance  */ EvalPieceTypeIndex::Gold,
  /* 10 PieceNumber::ProKnight */ EvalPieceTypeIndex::Gold,
  /* 11 PieceNumber::ProSilver */ EvalPieceTypeIndex::Gold,
  /* 12                        */ 0,
  /* 13 PieceNumber::Horse     */ EvalPieceTypeIndex::Horse,
  /* 14 PieceNumber::Dragon    */ EvalPieceTypeIndex::Dragon,
  /* 15                        */ 0,
};

} // namespace

namespace sunfish {

int getEvalPieceIndex(Piece piece) {
  return EvalPieceIndexTable[piece.raw()];
}

int getEvalPieceTypeIndex(PieceType pieceType) {
  return EvalPieceTypeIndexTable[pieceType.raw()];
}

#define N3x3 Neighbor3x3

namespace {
int HSymNeighbor3x3[Neighbor3x3::NN] = {
  N3x3::N20, N3x3::N21, N3x3::N22,
  N3x3::N10,            N3x3::N12,
  N3x3::N00, N3x3::N01, N3x3::N02,
};
} // namespace

int getHSymNeighbor3x3(int src) {
  return HSymNeighbor3x3[src]; 
}

int SquareDiffToNeighbor3x3[41] = {
  N3x3::N00, N3x3::N01, N3x3::N02, 0, 0, 0, 0, 0, 0,
  N3x3::N10,         0, N3x3::N12, 0, 0, 0, 0, 0, 0,
  N3x3::N20, N3x3::N21, N3x3::N22,
};

int getNeighbor3x3(Square king, Square square) {
  return SquareDiffToNeighbor3x3[square.raw()-king.raw()+10];
}

int getNeighbor3x3R(Square king, Square square) {
  return SquareDiffToNeighbor3x3[king.raw()-square.raw()+10];
}

} // namespace sunfish
