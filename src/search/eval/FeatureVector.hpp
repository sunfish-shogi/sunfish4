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
  using KingHand = Type[Square::N][EvalHandIndex::End];
  using KingPieceR = Type[RelativeSquare::N][EvalPieceIndex::End];
  using KingPieceXR = Type[SQUARE_FILES][RelativeSquare::N][EvalPieceIndex::End];
  using KingPieceYR = Type[SQUARE_RANKS][RelativeSquare::N][EvalPieceIndex::End];
  using KingPiece = Type[Square::N][Square::N][EvalPieceIndex::End];
  using KingNeighborHand = Type[Square::N][8][EvalPieceIndex::End][EvalHandIndex::End];
  using KingNeighborPieceR = Type[8][EvalPieceIndex::End][RelativeSquare::N][EvalPieceIndex::End];
  using KingNeighborPieceXR = Type[SQUARE_FILES][8][EvalPieceIndex::End][RelativeSquare::N][EvalPieceIndex::End];
  using KingNeighborPieceYR = Type[SQUARE_RANKS][8][EvalPieceIndex::End][RelativeSquare::N][EvalPieceIndex::End];
  using KingNeighborPiece = Type[Square::N][8][EvalPieceIndex::End][Square::N][EvalPieceIndex::End];
  using KingOpen = Type[Square::N][Square::N][8];

  KingHand kingHand;

  KingPieceR kingPieceR;
  KingPieceXR kingPieceXR;
  KingPieceYR kingPieceYR;
  KingPiece kingPiece;

  KingNeighborHand kingNeighborHand;

  KingNeighborPieceR kingNeighborPieceR;
  KingNeighborPieceXR kingNeighborPieceXR;
  KingNeighborPieceYR kingNeighborPieceYR;
  KingNeighborPiece kingNeighborPiece;

  KingOpen kingBRookVer;
  KingOpen kingWRookVer;
  KingOpen kingBRookHor;
  KingOpen kingWRookHor;
  KingOpen kingBBishopDiagL45;
  KingOpen kingWBishopDiagL45;
  KingOpen kingBBishopDiagR45;
  KingOpen kingWBishopDiagR45;
  KingOpen kingBLance;
  KingOpen kingWLance;
};

template <class T>
struct OptimizedFeatureVector {
  using Type = T;
  using KingHand = Type[Square::N][EvalHandIndex::End];
  using KingPiece = Type[Square::N][Square::N][EvalPieceIndex::End];
  using KingNeighborHand = Type[Square::N][8][EvalPieceIndex::End][EvalHandIndex::End];
  using KingNeighborPiece = Type[Square::N][8][EvalPieceIndex::End][Square::N][EvalPieceIndex::End];
  using KingOpen = Type[Square::N][Square::N][8];

  KingHand kingHand;

  KingPiece kingPiece;

  KingNeighborHand kingNeighborHand;

  KingNeighborPiece kingNeighborPiece;

  KingOpen kingBRookVer;
  KingOpen kingWRookVer;
  KingOpen kingBRookHor;
  KingOpen kingWRookHor;
  KingOpen kingBBishopDiagL45;
  KingOpen kingWBishopDiagL45;
  KingOpen kingBBishopDiagR45;
  KingOpen kingWBishopDiagR45;
  KingOpen kingBLance;
  KingOpen kingWLance;
};

} // namespace sunfish

#endif // SUNFISH_SEARCH_EVAL_FEATUREVECTOR_HPP__
