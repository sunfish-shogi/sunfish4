/* FeatureVector.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_SEARCH_EVAL_FEATUREVECTOR_HPP__
#define SUNFISH_SEARCH_EVAL_FEATUREVECTOR_HPP__

#include "core/base/Square.hpp"
#include "core/base/Piece.hpp"
#include "core/position/Hand.hpp"

#define SUNFISH_FV_VERSION "2018.05.29.0-light"

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
  Pawn = 0,
  Lance,
  Knight,
  Silver,
  Gold,
  Bishop,
  Horse,
  Rook,
  Dragon,
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
  using KingPieceR = Type[RelativeSquare::N][EvalPieceIndex::End];
  using KingPieceXR = Type[SQUARE_FILES][RelativeSquare::N][EvalPieceIndex::End];
  using KingPieceYR = Type[SQUARE_RANKS][RelativeSquare::N][EvalPieceIndex::End];
  using KingPiece = Type[Square::N][Square::N][EvalPieceIndex::End];
  //using KingPieceRNeighborX = Type[RelativeSquare::N][EvalPieceIndex::End][EvalPieceIndex::End];
  //using KingPieceXRNeighborX = Type[SQUARE_FILES][RelativeSquare::N][EvalPieceIndex::End][EvalPieceIndex::End];
  //using KingPieceYRNeighborX = Type[SQUARE_RANKS][RelativeSquare::N][EvalPieceIndex::End][EvalPieceIndex::End];
  //using KingPieceNeighborX = Type[Square::N][Square::N][EvalPieceIndex::End][EvalPieceIndex::End];
  //using KingPieceRNeighborY = Type[RelativeSquare::N][EvalPieceIndex::End][EvalPieceIndex::End];
  //using KingPieceXRNeighborY = Type[SQUARE_FILES][RelativeSquare::N][EvalPieceIndex::End][EvalPieceIndex::End];
  //using KingPieceYRNeighborY = Type[SQUARE_RANKS][RelativeSquare::N][EvalPieceIndex::End][EvalPieceIndex::End];
  //using KingPieceNeighborY = Type[Square::N][Square::N][EvalPieceIndex::End][EvalPieceIndex::End];
  //using KingPieceRNeighborXY = Type[RelativeSquare::N][EvalPieceIndex::End][EvalPieceIndex::End];
  //using KingPieceXRNeighborXY = Type[SQUARE_FILES][RelativeSquare::N][EvalPieceIndex::End][EvalPieceIndex::End];
  //using KingPieceYRNeighborXY = Type[SQUARE_RANKS][RelativeSquare::N][EvalPieceIndex::End][EvalPieceIndex::End];
  //using KingPieceNeighborXY = Type[Square::N][Square::N][EvalPieceIndex::End][EvalPieceIndex::End];
  //using KingPieceRNeighborXY2 = Type[RelativeSquare::N][EvalPieceIndex::End][EvalPieceIndex::End];
  //using KingPieceXRNeighborXY2 = Type[SQUARE_FILES][RelativeSquare::N][EvalPieceIndex::End][EvalPieceIndex::End];
  //using KingPieceYRNeighborXY2 = Type[SQUARE_RANKS][RelativeSquare::N][EvalPieceIndex::End][EvalPieceIndex::End];
  //using KingPieceNeighborXY2 = Type[Square::N][Square::N][EvalPieceIndex::End][EvalPieceIndex::End];
  using KingNeighborHand = Type[Square::N][Neighbor3x3::NN][EvalPieceTypeIndex::End][EvalHandIndex::End];
  //using KingNeighborPieceR = Type[Neighbor3x3::NN][EvalPieceTypeIndex::End][RelativeSquare::N][EvalPieceIndex::End];
  //using KingNeighborPieceXR = Type[SQUARE_FILES][Neighbor3x3::NN][EvalPieceTypeIndex::End][RelativeSquare::N][EvalPieceIndex::End];
  //using KingNeighborPieceYR = Type[SQUARE_RANKS][Neighbor3x3::NN][EvalPieceTypeIndex::End][RelativeSquare::N][EvalPieceIndex::End];
  //using KingNeighborPiece = Type[Square::N][Neighbor3x3::NN][EvalPieceTypeIndex::End][Square::N][EvalPieceIndex::End];
  using KingKingHand = Type[Square::N][Square::N][EvalHandTypeIndex::End];
  //using KingKingPiece = Type[Square::N][Square::N][Square::N][EvalPieceTypeIndex::End];
  using KingOpenR = Type[RelativeSquare::N][8];
  using KingOpenXR = Type[SQUARE_FILES][RelativeSquare::N][8];
  using KingOpenYR = Type[SQUARE_RANKS][RelativeSquare::N][8];
  using KingOpen = Type[Square::N][Square::N][8];
  using KingEffect9X = Type[SQUARE_FILES][10];
  using KingEffect9Y = Type[SQUARE_RANKS][10];
  using KingEffect9 = Type[Square::N][10];
  using KingEffect25X = Type[SQUARE_FILES][26];
  using KingEffect25Y = Type[SQUARE_RANKS][26];
  using KingEffect25 = Type[Square::N][26];
  using KingEffect9DiffX = Type[SQUARE_FILES][19];
  using KingEffect9DiffY = Type[SQUARE_RANKS][19];
  using KingEffect9Diff = Type[Square::N][19];
  using KingEffect25DiffX = Type[SQUARE_FILES][51];
  using KingEffect25DiffY = Type[SQUARE_RANKS][51];
  using KingEffect25Diff = Type[Square::N][51];

  KingHand kingHand;

  KingPieceR kingPieceR;
  KingPieceXR kingPieceXR;
  KingPieceYR kingPieceYR;
  KingPiece kingPiece;

  //KingPieceRNeighborX kingPieceRNeighborX;
  //KingPieceXRNeighborX kingPieceXRNeighborX;
  //KingPieceYRNeighborX kingPieceYRNeighborX;
  //KingPieceNeighborX kingPieceNeighborX;

  //KingPieceRNeighborY kingPieceRNeighborY;
  //KingPieceXRNeighborY kingPieceXRNeighborY;
  //KingPieceYRNeighborY kingPieceYRNeighborY;
  //KingPieceNeighborY kingPieceNeighborY;

  //KingPieceRNeighborXY kingPieceRNeighborXY;
  //KingPieceXRNeighborXY kingPieceXRNeighborXY;
  //KingPieceYRNeighborXY kingPieceYRNeighborXY;
  //KingPieceNeighborXY kingPieceNeighborXY;

  //KingPieceRNeighborXY2 kingPieceRNeighborXY2;
  //KingPieceXRNeighborXY2 kingPieceXRNeighborXY2;
  //KingPieceYRNeighborXY2 kingPieceYRNeighborXY2;
  //KingPieceNeighborXY2 kingPieceNeighborXY2;

  KingNeighborHand kingNeighborHand;

  //KingNeighborPieceR kingNeighborPieceR;
  //KingNeighborPieceXR kingNeighborPieceXR;
  //KingNeighborPieceYR kingNeighborPieceYR;
  //KingNeighborPiece kingNeighborPiece;

  KingKingHand kingKingHand;
  //KingKingPiece kingKingPiece;

  KingOpenR kingBRookUpR;
  KingOpenXR kingBRookUpXR;
  KingOpenYR kingBRookUpYR;
  KingOpen kingBRookUp;

  KingOpenR kingWRookUpR;
  KingOpenXR kingWRookUpXR;
  KingOpenYR kingWRookUpYR;
  KingOpen kingWRookUp;

  KingOpenR kingBRookDownR;
  KingOpenXR kingBRookDownXR;
  KingOpenYR kingBRookDownYR;
  KingOpen kingBRookDown;

  KingOpenR kingWRookDownR;
  KingOpenXR kingWRookDownXR;
  KingOpenYR kingWRookDownYR;
  KingOpen kingWRookDown;

  KingOpenR kingBRookLeftR;
  KingOpenXR kingBRookLeftXR;
  KingOpenYR kingBRookLeftYR;
  KingOpen kingBRookLeft;

  KingOpenR kingWRookLeftR;
  KingOpenXR kingWRookLeftXR;
  KingOpenYR kingWRookLeftYR;
  KingOpen kingWRookLeft;

  KingOpenR kingBRookRightR;
  KingOpenXR kingBRookRightXR;
  KingOpenYR kingBRookRightYR;
  KingOpen kingBRookRight;

  KingOpenR kingWRookRightR;
  KingOpenXR kingWRookRightXR;
  KingOpenYR kingWRookRightYR;
  KingOpen kingWRookRight;

  KingOpenR kingBBishopLeftUp45R;
  KingOpenXR kingBBishopLeftUp45XR;
  KingOpenYR kingBBishopLeftUp45YR;
  KingOpen kingBBishopLeftUp45;

  KingOpenR kingWBishopLeftUp45R;
  KingOpenXR kingWBishopLeftUp45XR;
  KingOpenYR kingWBishopLeftUp45YR;
  KingOpen kingWBishopLeftUp45;

  KingOpenR kingBBishopRightDown45R;
  KingOpenXR kingBBishopRightDown45XR;
  KingOpenYR kingBBishopRightDown45YR;
  KingOpen kingBBishopRightDown45;

  KingOpenR kingWBishopRightDown45R;
  KingOpenXR kingWBishopRightDown45XR;
  KingOpenYR kingWBishopRightDown45YR;
  KingOpen kingWBishopRightDown45;

  KingOpenR kingBBishopRightUp45R;
  KingOpenXR kingBBishopRightUp45XR;
  KingOpenYR kingBBishopRightUp45YR;
  KingOpen kingBBishopRightUp45;

  KingOpenR kingWBishopRightUp45R;
  KingOpenXR kingWBishopRightUp45XR;
  KingOpenYR kingWBishopRightUp45YR;
  KingOpen kingWBishopRightUp45;

  KingOpenR kingBBishopLeftDown45R;
  KingOpenXR kingBBishopLeftDown45XR;
  KingOpenYR kingBBishopLeftDown45YR;
  KingOpen kingBBishopLeftDown45;

  KingOpenR kingWBishopLeftDown45R;
  KingOpenXR kingWBishopLeftDown45XR;
  KingOpenYR kingWBishopLeftDown45YR;
  KingOpen kingWBishopLeftDown45;

  KingOpenR kingBLanceR;
  KingOpenXR kingBLanceXR;
  KingOpenYR kingBLanceYR;
  KingOpen kingBLance;

  KingOpenR kingWLanceR;
  KingOpenXR kingWLanceXR;
  KingOpenYR kingWLanceYR;
  KingOpen kingWLance;

  KingEffect9X kingAllyEffect9X;
  KingEffect9Y kingAllyEffect9Y;
  KingEffect9 kingAllyEffect9;

  KingEffect9X kingEnemyEffect9X;
  KingEffect9Y kingEnemyEffect9Y;
  KingEffect9 kingEnemyEffect9;

  KingEffect25X kingAllyEffect25X;
  KingEffect25Y kingAllyEffect25Y;
  KingEffect25 kingAllyEffect25;

  KingEffect25X kingEnemyEffect25X;
  KingEffect25Y kingEnemyEffect25Y;
  KingEffect25 kingEnemyEffect25;

  KingEffect9DiffX kingEffect9DiffX;
  KingEffect9DiffY kingEffect9DiffY;
  KingEffect9Diff kingEffect9Diff;

  KingEffect25DiffX kingEffect25DiffX;
  KingEffect25DiffY kingEffect25DiffY;
  KingEffect25Diff kingEffect25Diff;
};

template <class T>
struct OptimizedFeatureVector {
  using Type = T;
  using KingHand = Type[Square::N][EvalHandIndex::End];
  using KingPiece = Type[Square::N][Square::N][EvalPieceIndex::End];
  //using KingPieceNeighborX = Type[Square::N][Square::N][EvalPieceIndex::End][EvalPieceIndex::End];
  //using KingPieceNeighborY = Type[Square::N][Square::N][EvalPieceIndex::End][EvalPieceIndex::End];
  //using KingPieceNeighborXY = Type[Square::N][Square::N][EvalPieceIndex::End][EvalPieceIndex::End];
  //using KingPieceNeighborXY2 = Type[Square::N][Square::N][EvalPieceIndex::End][EvalPieceIndex::End];
  using KingNeighborHand = Type[Square::N][Neighbor3x3::NN][EvalPieceTypeIndex::End][EvalHandIndex::End];
  //using KingNeighborPiece = Type[Square::N][Neighbor3x3::NN][EvalPieceTypeIndex::End][Square::N][EvalPieceIndex::End];
  using KingKingHand = Type[Square::N][Square::N][EvalHandTypeIndex::End];
  //using KingKingPiece = Type[Square::N][Square::N][Square::N][EvalPieceTypeIndex::End];
  using KingOpen = Type[Square::N][Square::N][8];
  using KingEffect9 = Type[Square::N][10];
  using KingEffect25 = Type[Square::N][26];
  using KingEffect9Diff = Type[Square::N][19];
  using KingEffect25Diff = Type[Square::N][51];

  KingHand kingHand;

  KingPiece kingPiece;

  //KingPieceNeighborX kingPieceNeighborX;

  //KingPieceNeighborY kingPieceNeighborY;

  //KingPieceNeighborXY kingPieceNeighborXY;

  //KingPieceNeighborXY2 kingPieceNeighborXY2;

  KingNeighborHand kingNeighborHand;

  //KingNeighborPiece kingNeighborPiece;

  KingKingHand kingKingHand;
  //KingKingPiece kingKingPiece;

  KingOpen kingBRookUp;
  KingOpen kingWRookUp;
  KingOpen kingBRookDown;
  KingOpen kingWRookDown;
  KingOpen kingBRookLeft;
  KingOpen kingWRookLeft;
  KingOpen kingBRookRight;
  KingOpen kingWRookRight;
  KingOpen kingBBishopLeftUp45;
  KingOpen kingWBishopLeftUp45;
  KingOpen kingBBishopRightDown45;
  KingOpen kingWBishopRightDown45;
  KingOpen kingBBishopRightUp45;
  KingOpen kingWBishopRightUp45;
  KingOpen kingBBishopLeftDown45;
  KingOpen kingWBishopLeftDown45;
  KingOpen kingBLance;
  KingOpen kingWLance;

  KingEffect9 kingAllyEffect9;
  KingEffect9 kingEnemyEffect9;
  KingEffect25 kingAllyEffect25;
  KingEffect25 kingEnemyEffect25;
  KingEffect9Diff kingEffect9Diff;
  KingEffect25Diff kingEffect25Diff;
};

} // namespace sunfish

#endif // SUNFISH_SEARCH_EVAL_FEATUREVECTOR_HPP__
