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

enum {
  MaxKingSafety = 8,
  MinKingSafety = -7,
  KingSafetyLen = MaxKingSafety - MinKingSafety + 1,
};

template <Turn turn>
int safetyOfKing(const Position& pos);

template <class T>
struct FeatureVector {
  using Type = T;
  using KingHand = Type[Square::N][EvalHandIndex::End];
  using KingPieceR = Type[RelativeSquare::N][EvalPieceIndex::End];
  using KingPieceXR = Type[SQUARE_FILES][RelativeSquare::N][EvalPieceIndex::End];
  using KingPieceYR = Type[SQUARE_RANKS][RelativeSquare::N][EvalPieceIndex::End];
  using KingPiece = Type[Square::N][Square::N][EvalPieceIndex::End];
  using KingNeighborHand = Type[Square::N][Neighbor3x3::NN][EvalPieceTypeIndex::End][EvalHandIndex::End];
  using KingNeighborPieceR = Type[Neighbor3x3::NN][EvalPieceTypeIndex::End][RelativeSquare::N][EvalPieceIndex::End];
  using KingNeighborPieceXR = Type[SQUARE_FILES][Neighbor3x3::NN][EvalPieceTypeIndex::End][RelativeSquare::N][EvalPieceIndex::End];
  using KingNeighborPieceYR = Type[SQUARE_RANKS][Neighbor3x3::NN][EvalPieceTypeIndex::End][RelativeSquare::N][EvalPieceIndex::End];
  using KingNeighborPiece = Type[Square::N][Neighbor3x3::NN][EvalPieceTypeIndex::End][Square::N][EvalPieceIndex::End];
  using KingOpen = Type[Square::N][Square::N][8];
  using KingSafetyHand = Type[Square::N][KingSafetyLen][EvalHandIndex::End];
  using KingSafetyPieceR = Type[KingSafetyLen][RelativeSquare::N][EvalPieceIndex::End];
  using KingSafetyPieceXR = Type[SQUARE_FILES][KingSafetyLen][RelativeSquare::N][EvalPieceIndex::End];
  using KingSafetyPieceYR = Type[SQUARE_RANKS][KingSafetyLen][RelativeSquare::N][EvalPieceIndex::End];
  using KingSafetyPiece = Type[Square::N][KingSafetyLen][Square::N][EvalPieceIndex::End];

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

  KingSafetyHand kingSafetyHand;
  KingSafetyPieceR kingSafetyPieceR;
  KingSafetyPieceXR kingSafetyPieceXR;
  KingSafetyPieceYR kingSafetyPieceYR;
  KingSafetyPiece kingSafetyPiece;
};

template <class T>
struct OptimizedFeatureVector {
  using Type = T;
  using KingNeighborHand = Type[Square::N][Neighbor3x3::NN][EvalPieceTypeIndex::End][EvalHandIndex::End];
  using KingNeighborPiece = Type[Square::N][Neighbor3x3::NN][EvalPieceTypeIndex::End][Square::N][EvalPieceIndex::End];
  using KingOpen = Type[Square::N][Square::N][8];
  using KingSafetyHand = Type[Square::N][KingSafetyLen][EvalHandIndex::End];
  using KingSafetyPiece = Type[Square::N][KingSafetyLen][Square::N][EvalPieceIndex::End];

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

  KingSafetyHand kingSafetyHand;
  KingSafetyPiece kingSafetyPiece;
};

} // namespace sunfish

#endif // SUNFISH_SEARCH_EVAL_FEATUREVECTOR_HPP__
