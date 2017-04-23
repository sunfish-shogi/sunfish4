/* FeatureVector.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_SEARCH_EVAL_FEATUREVECTOR_HPP__
#define SUNFISH_SEARCH_EVAL_FEATUREVECTOR_HPP__

#include "core/base/Square.hpp"
#include "core/base/Piece.hpp"
#include "core/position/Hand.hpp"

#define SUNFISH_FV_VERSION "2017.05.23.0"

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
  using KingEffect9 = Type[Square::N][10];
  using KingEffect25 = Type[Square::N][26];

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

  KingKingHand kingKingHand;
  KingKingPiece kingKingPiece;

  Open bRookVer;
  KingOpenR kingBRookVerR;
  KingOpenXR kingBRookVerXR;
  KingOpenYR kingBRookVerYR;
  KingOpen kingBRookVer;

  Open wRookVer;
  KingOpenR kingWRookVerR;
  KingOpenXR kingWRookVerXR;
  KingOpenYR kingWRookVerYR;
  KingOpen kingWRookVer;

  Open bRookHor;
  KingOpenR kingBRookHorR;
  KingOpenXR kingBRookHorXR;
  KingOpenYR kingBRookHorYR;
  KingOpen kingBRookHor;

  Open wRookHor;
  KingOpenR kingWRookHorR;
  KingOpenXR kingWRookHorXR;
  KingOpenYR kingWRookHorYR;
  KingOpen kingWRookHor;

  Open bBishopDiagL45;
  KingOpenR kingBBishopDiagL45R;
  KingOpenXR kingBBishopDiagL45XR;
  KingOpenYR kingBBishopDiagL45YR;
  KingOpen kingBBishopDiagL45;

  Open wBishopDiagL45;
  KingOpenR kingWBishopDiagL45R;
  KingOpenXR kingWBishopDiagL45XR;
  KingOpenYR kingWBishopDiagL45YR;
  KingOpen kingWBishopDiagL45;

  Open bBishopDiagR45;
  KingOpenR kingBBishopDiagR45R;
  KingOpenXR kingBBishopDiagR45XR;
  KingOpenYR kingBBishopDiagR45YR;
  KingOpen kingBBishopDiagR45;

  Open wBishopDiagR45;
  KingOpenR kingWBishopDiagR45R;
  KingOpenXR kingWBishopDiagR45XR;
  KingOpenYR kingWBishopDiagR45YR;
  KingOpen kingWBishopDiagR45;

  Open bLance;
  KingOpenR kingBLanceR;
  KingOpenXR kingBLanceXR;
  KingOpenYR kingBLanceYR;
  KingOpen kingBLance;

  Open wLance;
  KingOpenR kingWLanceR;
  KingOpenXR kingWLanceXR;
  KingOpenYR kingWLanceYR;
  KingOpen kingWLance;

  KingEffect9 kingAllyEffect9;
  KingEffect9 kingEnemyEffect9;
  KingEffect25 kingAllyEffect25;
  KingEffect25 kingEnemyEffect25;
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
  using KingEffect9 = Type[Square::N][10];
  using KingEffect25 = Type[Square::N][26];

  KingHand kingHand;

  KingPiece kingPiece;

  KingNeighborHand kingNeighborHand;

  KingNeighborPiece kingNeighborPiece;

  KingKingHand kingKingHand;
  KingKingPiece kingKingPiece;

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

  KingEffect9 kingAllyEffect9;
  KingEffect9 kingEnemyEffect9;
  KingEffect25 kingAllyEffect25;
  KingEffect25 kingEnemyEffect25;
};

} // namespace sunfish

#endif // SUNFISH_SEARCH_EVAL_FEATUREVECTOR_HPP__
