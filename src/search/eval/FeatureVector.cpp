/* FeatureVector.cpp
 *
 * Kubo Ryosuke
 */

#include "search/eval/FeatureVector.hpp"
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
