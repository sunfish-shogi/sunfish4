/* FeatureVector.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_SEARCH_EVAL_FEATUREVECTOR_HPP__
#define SUNFISH_SEARCH_EVAL_FEATUREVECTOR_HPP__

#include "core/base/Square.hpp"
#include "core/base/Piece.hpp"
#include "core/position/Hand.hpp"

namespace sunfish {

namespace KKP_ {
enum Type {
  HPawn   = 0,
  HLance  = HPawn   + 18,
  HKnight = HLance  + 4,
  HSilver = HKnight + 4,
  HGold   = HSilver + 4,
  HBishop = HGold   + 4,
  HRook   = HBishop + 2,
  BPawn   = HRook   + 2,
  BLance  = BPawn   + 81 - 9,
  BKnight = BLance  + 81 - 9,
  BSilver = BKnight + 81 - 18,
  BGold   = BSilver + 81,
  BBishop = BGold   + 81,
  BHorse  = BBishop + 81,
  BRook   = BHorse  + 81,
  BDragon = BRook   + 81,

  HandEnd = HRook   + 2,
  End     = BDragon + 81,
  Size    = 81 * 81 * End,
};
} // namespace KKP_
using KKP = KKP_::Type;

int kkpBoardIndex(PieceType pieceType, Square square);

int kkpHandIndex(PieceType pieceType, Hand::Type n);

int symmetricalKkpIndex(int index);

template <class T>
struct FeatureVector {
  using Type = T;
  using Kkp = Type[81][81][KKP::End];
  using KkpC = Type[81][81][KKP::HandEnd];

  Kkp kkp;
  KkpC kkpc;
};

} // namespace sunfish

#endif // SUNFISH_SEARCH_EVAL_FEATUREVECTOR_HPP__
