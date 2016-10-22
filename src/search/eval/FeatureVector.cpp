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

int getEvalPieceTypeIndex(PieceType pieceType) {
  switch (pieceType.raw()) {
  case PieceNumber::Pawn     : return EvalPieceTypeIndex::BPawn;
  case PieceNumber::Lance    : return EvalPieceTypeIndex::BLance;
  case PieceNumber::Knight   : return EvalPieceTypeIndex::BKnight;
  case PieceNumber::Silver   : return EvalPieceTypeIndex::BSilver;
  case PieceNumber::Gold     : // fall through
  case PieceNumber::Tokin    : // fall through
  case PieceNumber::ProLance : // fall through
  case PieceNumber::ProKnight: // fall through
  case PieceNumber::ProSilver: return EvalPieceTypeIndex::BGold;
  case PieceNumber::Bishop   : return EvalPieceTypeIndex::BBishop;
  case PieceNumber::Horse    : return EvalPieceTypeIndex::BHorse;
  case PieceNumber::Rook     : return EvalPieceTypeIndex::BRook;
  case PieceNumber::Dragon   : return EvalPieceTypeIndex::BDragon;
  }
  ASSERT(false);
  return 0; // unreachable
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
