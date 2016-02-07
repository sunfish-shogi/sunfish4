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

int getEvalPieceIndex(Piece piece);

template <class T>
struct FeatureVector {
  using Type = T;
  using KingHandType = Type[Square::N][EvalHandIndex::End];
  using KingPieceTypeR = Type[RelativeSquare::N][EvalPieceIndex::End];
  using KingPieceTypeXR = Type[SQUARE_FILES][RelativeSquare::N][EvalPieceIndex::End];
  using KingPieceTypeYR = Type[SQUARE_RANKS][RelativeSquare::N][EvalPieceIndex::End];
  using KingPieceType = Type[Square::N][Square::N][EvalPieceIndex::End];
  using KingNeighborHandType = Type[Square::N][8][EvalPieceIndex::End][EvalHandIndex::End];
  using KingNeighborPieceTypeR = Type[8][EvalPieceIndex::End][RelativeSquare::N][EvalPieceIndex::End];
  using KingNeighborPieceTypeXR = Type[SQUARE_FILES][8][EvalPieceIndex::End][RelativeSquare::N][EvalPieceIndex::End];
  using KingNeighborPieceTypeYR = Type[SQUARE_RANKS][8][EvalPieceIndex::End][RelativeSquare::N][EvalPieceIndex::End];
  using KingNeighborPieceType = Type[Square::N][8][EvalPieceIndex::End][Square::N][EvalPieceIndex::End];
  using KingOpenType = Type[Square::N][Square::N][8];

  KingHandType kingHand;

  KingPieceTypeR kingPieceR;
  KingPieceTypeXR kingPieceXR;
  KingPieceTypeYR kingPieceYR;
  KingPieceType kingPiece;

  KingNeighborHandType kingNeighborHand;

  KingNeighborPieceTypeR kingNeighborPieceR;
  KingNeighborPieceTypeXR kingNeighborPieceXR;
  KingNeighborPieceTypeYR kingNeighborPieceYR;
  KingNeighborPieceType kingNeighborPiece;

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
  using KingHandType = Type[Square::N][EvalHandIndex::End];
  using KingPieceType = Type[Square::N][Square::N][EvalPieceIndex::End];
  using KingNeighborHandType = Type[Square::N][8][EvalPieceIndex::End][EvalHandIndex::End];
  using KingNeighborPieceType = Type[Square::N][8][EvalPieceIndex::End][Square::N][EvalPieceIndex::End];
  using KingOpenType = Type[Square::N][Square::N][8];

  KingHandType kingHand;

  KingPieceType kingPiece;

  KingNeighborHandType kingNeighborHand;

  KingNeighborPieceType kingNeighborPiece;

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
