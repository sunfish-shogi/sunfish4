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

namespace EvalHandIndex_ {
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
}; // namespace EvalHandIndex_
using EvalHandIndex = EvalHandIndex_::Type;

namespace EvalPieceIndex_ {
enum Type {
  BPawn = 0,
  WPawn,
  BLance,
  WLance,
  BKnight,
  WKnight,
  BSilver,
  WSilver,
  BGold,
  WGold,
  BBishop,
  WBishop,
  BHorse,
  WHorse,
  BRook,
  WRook,
  BDragon,
  WDragon,
  End,
};
} // namespace KingPiece_
using EvalPieceIndex = EvalPieceIndex_::Type;

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

int getEvalPieceIndex(Piece piece);
int getEvalGoldIndex(Direction dir);
int getEvalSilverIndex(Direction dir);

int symmetricalKingGoldIndex(int index);

template <class T>
struct FeatureVector {
  using Type = T;
  using KingHandType = Type[Square::N][EvalHandIndex::End];
  using KingNumGoldHandType = Type[Square::N][9][EvalHandIndex::End];
  using KingPieceTypeR = Type[RelativeSquare::N][EvalPieceIndex::End];
  using KingPieceType = Type[Square::N][Square::N][EvalPieceIndex::End];
  using KingNumGoldPieceType = Type[Square::N][9][Square::N][EvalPieceIndex::End];
  using KingGoldPieceTypeR = Type[KingGold::End][RelativeSquare::N][EvalPieceIndex::End];
  using KingGoldPieceType = Type[Square::N][KingGold::End][Square::N][EvalPieceIndex::End];
  using KingOpenType = Type[Square::N][Square::N][8];

  KingHandType kingHand;
  KingNumGoldHandType kingNumGoldHand;

  KingPieceTypeR kingPieceR;
  KingPieceType kingPiece;
  KingNumGoldPieceType kingNumGoldPiece;

  KingGoldPieceTypeR kingGoldPieceR;
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
};

template <class T>
struct OptimizedFeatureVector {
  using Type = T;
  using KingNumGoldHandType = Type[Square::N][9][EvalHandIndex::End];
  using KingNumGoldPieceType = Type[Square::N][9][Square::N][EvalPieceIndex::End];
  using KingGoldPieceType = Type[Square::N][KingGold::End][Square::N][EvalPieceIndex::End];
  using KingOpenType = Type[Square::N][Square::N][8];

  KingNumGoldHandType kingNumGoldHand;

  KingNumGoldPieceType kingNumGoldPiece;

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
};

} // namespace sunfish

#endif // SUNFISH_SEARCH_EVAL_FEATUREVECTOR_HPP__
