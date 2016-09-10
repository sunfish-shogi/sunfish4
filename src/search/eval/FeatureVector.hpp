/* FeatureVector.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_SEARCH_EVAL_FEATUREVECTOR_HPP__
#define SUNFISH_SEARCH_EVAL_FEATUREVECTOR_HPP__

#include "core/base/Square.hpp"
#include "core/base/Piece.hpp"
#include "core/position/Hand.hpp"

#define SUNFISH_FV_VERSION "2016.09.10.3"

namespace sunfish {

class Position;

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

namespace EvalHandTypeIndex_ {
enum Type {
  Pawn   = 0,
  Lance  = Pawn   + 18,
  Knight = Lance  + 4,
  Silver = Knight + 4,
  Gold   = Silver + 4,
  Bishop = Gold   + 4,
  Rook   = Bishop + 2,
  End    = Rook + 2,
};
}; // namespace EvalHandTypeIndex_
using EvalHandTypeIndex = EvalHandTypeIndex_::Type;

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
} // namespace EvalPieceIndex_
using EvalPieceIndex = EvalPieceIndex_::Type;

int getEvalPieceIndex(Piece piece);

namespace EvalPieceTypeIndex_ {
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
} // namespace EvalPieceTypeIndex_
using EvalPieceTypeIndex = EvalPieceTypeIndex_::Type;

int getEvalPieceTypeIndex(PieceType pieceType);

namespace Neighbor3x3_ {
enum Type {
  N00 = 0,
       N01, N02,
  N10,      N12,
  N20, N21, N22,
  NN,
};
} // namespace Neighbor3x3
using Neighbor3x3 = Neighbor3x3_::Type;

int getHSymNeighbor3x3(int src);
int getNeighbor3x3(Square king, Square square);
int getNeighbor3x3R(Square king, Square square);

template <class T>
struct FeatureVector {
  using Type = T;
  using KingHand = Type[Square::N][EvalHandIndex::End];
  using Piece = Type[Square::N][EvalPieceIndex::End];
  using KingPieceR = Type[RelativeSquare::N][EvalPieceIndex::End];
  using KingPieceXR = Type[SQUARE_FILES][RelativeSquare::N][EvalPieceIndex::End];
  using KingPieceYR = Type[SQUARE_RANKS][RelativeSquare::N][EvalPieceIndex::End];
  using KingPiece = Type[Square::N][Square::N][EvalPieceIndex::End];
  using KingNeighborHand = Type[Square::N][Neighbor3x3::NN][EvalPieceTypeIndex::End][EvalHandIndex::End];
  using KingNeighborPieceR = Type[Neighbor3x3::NN][EvalPieceTypeIndex::End][RelativeSquare::N][EvalPieceIndex::End];
  using KingNeighborPieceXR = Type[SQUARE_FILES][Neighbor3x3::NN][EvalPieceTypeIndex::End][RelativeSquare::N][EvalPieceIndex::End];
  using KingNeighborPieceYR = Type[SQUARE_RANKS][Neighbor3x3::NN][EvalPieceTypeIndex::End][RelativeSquare::N][EvalPieceIndex::End];
  using KingNeighborPiece = Type[Square::N][Neighbor3x3::NN][EvalPieceTypeIndex::End][Square::N][EvalPieceIndex::End];
  using KingKingHand = Type[Square::N][Square::N][EvalHandTypeIndex::End];
  using KingKingPiece = Type[Square::N][Square::N][Square::N][EvalPieceTypeIndex::End];
  using Open = Type[Square::N][8];
  using KingOpenR = Type[RelativeSquare::N][8];
  using KingOpenXR = Type[SQUARE_FILES][RelativeSquare::N][8];
  using KingOpenYR = Type[SQUARE_RANKS][RelativeSquare::N][8];
  using KingOpen = Type[Square::N][Square::N][8];

  KingHand kingHand;

  Piece piece;
  KingPieceR kingPieceR;
  KingPieceXR kingPieceXR;
  KingPieceYR kingPieceYR;
  KingPiece kingPiece;

  KingNeighborHand kingNeighborHand;

  KingNeighborPieceR kingNeighborPieceR;
  KingNeighborPieceXR kingNeighborPieceXR;
  KingNeighborPieceYR kingNeighborPieceYR;
  KingNeighborPiece kingNeighborPiece;

  KingKingHand kingKingHand;
  KingKingPiece kingKingPiece;

  Open rookVer;
  KingOpenR kingRookVerR;
  KingOpenXR kingRookVerXR;
  KingOpenYR kingRookVerYR;
  KingOpen kingRookVer;

  Open rookHor;
  KingOpenR kingRookHorR ;
  KingOpenXR kingRookHorXR;
  KingOpenYR kingRookHorYR;
  KingOpen kingRookHor;

  Open bishopDiagL45;
  KingOpenR kingBishopDiagL45R ;
  KingOpenXR kingBishopDiagL45XR;
  KingOpenYR kingBishopDiagL45YR;
  KingOpen kingBishopDiagL45;

  Open bishopDiagR45;
  KingOpenR kingBishopDiagR45R ;
  KingOpenXR kingBishopDiagR45XR;
  KingOpenYR kingBishopDiagR45YR;
  KingOpen kingBishopDiagR45;

  Open lance;
  KingOpenR kingLanceR ;
  KingOpenXR kingLanceXR;
  KingOpenYR kingLanceYR;
  KingOpen kingLance;
};

template <class T>
struct OptimizedFeatureVector {
  using Type = T;
  using KingHand = Type[Square::N][EvalHandIndex::End];
  using KingPiece = Type[Square::N][Square::N][EvalPieceIndex::End];
  using KingNeighborHand = Type[Square::N][Neighbor3x3::NN][EvalPieceTypeIndex::End][EvalHandIndex::End];
  using KingNeighborPiece = Type[Square::N][Neighbor3x3::NN][EvalPieceTypeIndex::End][Square::N][EvalPieceIndex::End];
  using KingKingHand = Type[Square::N][Square::N][EvalHandTypeIndex::End];
  using KingKingPiece = Type[Square::N][Square::N][Square::N][EvalPieceTypeIndex::End];
  using KingOpen = Type[Square::N][Square::N][8];

  KingHand kingHand;

  KingPiece kingPiece;

  KingNeighborHand kingNeighborHand;

  KingNeighborPiece kingNeighborPiece;

  KingKingHand kingKingHand;
  KingKingPiece kingKingPiece;

  KingOpen kingRookVer;
  KingOpen kingRookHor;
  KingOpen kingBishopDiagL45;
  KingOpen kingBishopDiagR45;
  KingOpen kingLance;
};

} // namespace sunfish

#endif // SUNFISH_SEARCH_EVAL_FEATUREVECTOR_HPP__
