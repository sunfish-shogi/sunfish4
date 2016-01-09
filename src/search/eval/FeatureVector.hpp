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

namespace KingHand_ {
enum Type {
  BPawn   = 0,
  WPawn   = BPawn   + 18,
  BLance  = WPawn   + 18,
  WLance  = BLance  + 4,
  BKnight = WLance  + 4,
  WKnight = BKnight + 4,
  BSilver = WKnight + 4,
  WSilver = BSilver + 4,
  BGold   = WSilver + 4,
  WGold   = BGold   + 4,
  BBishop = WGold   + 4,
  WBishop = BBishop + 2,
  BRook   = WBishop + 2,
  WRook   = BRook   + 2,
  End     = WRook + 2,
};
}; // namespace KingHand_
using KingHand = KingHand_::Type;

namespace KingPiece_ {
enum Type {
  BPawn   = 0,
  WPawn   = BPawn   + 81 - 9,
  BLance  = WPawn   + 81 - 9,
  WLance  = BLance  + 81 - 9,
  BKnight = WLance  + 81 - 9,
  WKnight = BKnight + 81 - 18,
  BSilver = WKnight + 81 - 18,
  WSilver = BSilver + 81,
  BGold   = WSilver + 81,
  WGold   = BGold   + 81,
  BBishop = WGold   + 81,
  WBishop = BBishop + 81,
  BHorse  = WBishop + 81,
  WHorse  = BHorse  + 81,
  BRook   = WHorse  + 81,
  WRook   = BRook   + 81,
  BDragon = WRook   + 81,
  WDragon = BDragon + 81,
  End     = WDragon + 81,
};
} // namespace KingPiece_
using KingPiece = KingPiece_::Type;

namespace KingGold_ {
enum Type {
  LeftUpSilver = 0,
  UpSilver,
  RightUpSilver,
  LeftSilver,
  RightSilver,
  LeftDownSilver,
  DownSilver,
  RightDownSilver,
  LeftUpGold,
  UpGold,
  RightUpGold,
  LeftGold,
  RightGold,
  LeftDownGold,
  DownGold,
  RightDownGold,
  End,
};
} // namespace KingGold_
using KingGold = KingGold_::Type;

int kingPieceIndex(Piece pieceType, Square square);
int kingHandIndex(Piece pieceType, Hand::Type n);
int kingGoldIndex(Direction dir);
int kingSilverIndex(Direction dir);

int symmetricalKingPieceIndex(int index);
int symmetricalKingGoldIndex(int index);

template <class T>
struct FeatureVector {
  using Type = T;
  using KingHandType = Type[81][KingHand::End];
  using KingPieceType = Type[81][KingPiece::End];
  using KingGoldPieceType = Type[81][KingGold::End][KingPiece::End];
  using KingOpenType = Type[81][81][8];
  using KingNumGoldType = Type[81][9];

  KingHandType kingHand;
  KingPieceType kingPiece;
  KingGoldPieceType kingGoldPiece;
  KingOpenType kingBRookVer;
  KingOpenType kingWRookVer;
  KingOpenType kingBRookHor;
  KingOpenType kingWRookHor;
  KingOpenType kingBBishopDiagL45;
  KingOpenType kingWBishopDiagL45;
  KingOpenType kingBBishopDiagR45;
  KingOpenType kingWBishopDiagR45;
  KingOpenType kingBLance;
  KingOpenType kingWLance;
  KingNumGoldType kingNumGold;
};

template <class T>
struct CumulatedVector {
  using Type = T;
  using KingHandType = Type[81][KingHand::End];

  KingHandType kingHand;
};

} // namespace sunfish

#endif // SUNFISH_SEARCH_EVAL_FEATUREVECTOR_HPP__
