/* FeatureTemplates.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_SEARCH_EVAL_FEATURETEMPLATES_HPP__
#define SUNFISH_SEARCH_EVAL_FEATURETEMPLATES_HPP__

#include "core/base/Square.hpp"
#include "core/move/MoveTables.hpp"
#include "core/position/Position.hpp"
#include "search/eval/FeatureVector.hpp"
#include <vector>
#include <algorithm>
#include <utility>
#include <cstring>

#define FV_PART_COPY(out, in, part) memcpy( \
    reinterpret_cast<typename FV::Type*>(out.part), \
    reinterpret_cast<const typename FV::Type*>(in.part), \
    sizeof(fv.part))

namespace {

using namespace sunfish;

template <class KingOpenR,
          class KingOpenXR,
          class KingOpenYR,
          class KingOpen,
          class OKingOpen>
inline
void optimizeOpen(KingOpenR kingOpenR,
                  KingOpenXR kingOpenXR,
                  KingOpenYR kingOpenYR,
                  KingOpen kingOpen,
                  OKingOpen oKingOpen) {
  // kingOpenR, kingOpenXR, kingOpenYR, kingOpen
  //   => kingOpen
  SQUARE_EACH(king) {
    SQUARE_EACH(square) {
      for (int i = 0; i < 8; i++) {
        oKingOpen[king.raw()][square.raw()][i]
          = kingOpenR[RelativeSquare(king, square).raw()][i]
          + kingOpenXR[king.getFile()-1][RelativeSquare(king, square).raw()][i]
          + kingOpenYR[king.getRank()-1][RelativeSquare(king, square).raw()][i]
          + kingOpen[king.raw()][square.raw()][i];
      }
    }
  }
}

template <class PieceRNeighbor,
          class PieceXRNeighbor,
          class PieceYRNeighbor,
          class PieceNeighbor,
          class OPieceNeighbor>
inline
void optimizePieceNeighbor(PieceRNeighbor  pieceRNeighbor,
                           PieceXRNeighbor pieceXRNeighbor,
                           PieceYRNeighbor pieceYRNeighbor,
                           PieceNeighbor   pieceNeighbor,
                           OPieceNeighbor  oPieceNeighbor) {
  // pieceRNeighbor, pieceXRNeighbor, pieceYRNeighbor, pieceNeighbor
  //   => pieceNeighbor
  SQUARE_EACH(king) {
    SQUARE_EACH(square) {
      for (int i = 0; i < EvalPieceIndex::End; i++) {
        for (int j = 0; j < EvalPieceIndex::End; j++) {
          oPieceNeighbor[king.raw()][square.raw()][i][j]
            = pieceNeighbor[king.raw()][square.raw()][i][j]
            + pieceRNeighbor[RelativeSquare(king, square).raw()][i][j]
            + pieceXRNeighbor[king.getFile()-1][RelativeSquare(king, square).raw()][i][j]
            + pieceYRNeighbor[king.getRank()-1][RelativeSquare(king, square).raw()][i][j];
        }
      }
    }
  }
}

template <class KingEffect,
          class KingEffectX,
          class KingEffectY,
          class OKingEffect>
inline
void optimizeEffect(KingEffect kingEffect,
                    KingEffectX kingEffectX,
                    KingEffectY kingEffectY,
                    OKingEffect oKingEffect,
                    int lastIdx) {
  SQUARE_EACH(king) {
    for (int i = 0; i <= lastIdx; i++) {
      oKingEffect[king.raw()][i]
        = kingEffectX[king.getFile()-1][i]
        + kingEffectY[king.getRank()-1][i]
        + kingEffect[king.raw()][i];
    }
  }
}

template <class FV, class OFV>
inline
void optimize(FV& fv, OFV& ofv) {
  FV_PART_COPY(ofv, fv, kingHand);

  // kingPieceR, kingPieceXR, kingPieceYR, kingPiece,
  //   => kingPiece
  SQUARE_EACH(king) {
    SQUARE_EACH(square) {
      for (int i = 0; i < EvalPieceIndex::End; i++) {
        ofv.kingPiece[king.raw()][square.raw()][i]
          = fv.kingPiece[king.raw()][square.raw()][i]
          + fv.kingPieceR[RelativeSquare(king, square).raw()][i]
          + fv.kingPieceXR[king.getFile()-1][RelativeSquare(king, square).raw()][i]
          + fv.kingPieceYR[king.getRank()-1][RelativeSquare(king, square).raw()][i];
      }
    }
  }

  //optimizePieceNeighbor(fv.kingPieceRNeighborX,
  //                      fv.kingPieceXRNeighborX,
  //                      fv.kingPieceYRNeighborX,
  //                      fv.kingPieceNeighborX,
  //                      ofv.kingPieceNeighborX);

  //optimizePieceNeighbor(fv.kingPieceRNeighborY,
  //                      fv.kingPieceXRNeighborY,
  //                      fv.kingPieceYRNeighborY,
  //                      fv.kingPieceNeighborY,
  //                      ofv.kingPieceNeighborY);

  //optimizePieceNeighbor(fv.kingPieceRNeighborXY,
  //                      fv.kingPieceXRNeighborXY,
  //                      fv.kingPieceYRNeighborXY,
  //                      fv.kingPieceNeighborXY,
  //                      ofv.kingPieceNeighborXY);

  //optimizePieceNeighbor(fv.kingPieceRNeighborXY2,
  //                      fv.kingPieceXRNeighborXY2,
  //                      fv.kingPieceYRNeighborXY2,
  //                      fv.kingPieceNeighborXY2,
  //                      ofv.kingPieceNeighborXY2);

  FV_PART_COPY(ofv, fv, kingNeighborHand);

  // kingNeighborPieceR, kingNeighborPieceXR, kingNeighborPieceYR, kingNeighborPiece
  //   => kingNeighborPiece
  //SQUARE_EACH(king) {
  //  for (int n = 0; n < Neighbor3x3::NN; n++) {
  //    for (int i1 = 0; i1 < EvalPieceTypeIndex::End; i1++) {
  //      SQUARE_EACH(square) {
  //        for (int i2 = 0; i2 < EvalPieceIndex::End; i2++) {
  //          ofv.kingNeighborPiece[king.raw()][n][i1][square.raw()][i2]
  //            = fv.kingNeighborPiece[king.raw()][n][i1][square.raw()][i2]
  //            + fv.kingNeighborPieceR[n][i1][RelativeSquare(king, square).raw()][i2]
  //            + fv.kingNeighborPieceXR[king.getFile()-1][n][i1][RelativeSquare(king, square).raw()][i2]
  //            + fv.kingNeighborPieceYR[king.getRank()-1][n][i1][RelativeSquare(king, square).raw()][i2];
  //        }
  //      }
  //    }
  //  }
  //}

  FV_PART_COPY(ofv, fv, kingKingHand);
  //FV_PART_COPY(ofv, fv, kingKingPiece);

  optimizeOpen(fv.kingBRookUpR,
               fv.kingBRookUpXR,
               fv.kingBRookUpYR,
               fv.kingBRookUp,
               ofv.kingBRookUp);
  optimizeOpen(fv.kingWRookUpR,
               fv.kingWRookUpXR,
               fv.kingWRookUpYR,
               fv.kingWRookUp,
               ofv.kingWRookUp);
  optimizeOpen(fv.kingBRookDownR,
               fv.kingBRookDownXR,
               fv.kingBRookDownYR,
               fv.kingBRookDown,
               ofv.kingBRookDown);
  optimizeOpen(fv.kingWRookDownR,
               fv.kingWRookDownXR,
               fv.kingWRookDownYR,
               fv.kingWRookDown,
               ofv.kingWRookDown);
  optimizeOpen(fv.kingBRookLeftR,
               fv.kingBRookLeftXR,
               fv.kingBRookLeftYR,
               fv.kingBRookLeft,
               ofv.kingBRookLeft);
  optimizeOpen(fv.kingWRookLeftR,
               fv.kingWRookLeftXR,
               fv.kingWRookLeftYR,
               fv.kingWRookLeft,
               ofv.kingWRookLeft);
  optimizeOpen(fv.kingBRookRightR,
               fv.kingBRookRightXR,
               fv.kingBRookRightYR,
               fv.kingBRookRight,
               ofv.kingBRookRight);
  optimizeOpen(fv.kingWRookRightR,
               fv.kingWRookRightXR,
               fv.kingWRookRightYR,
               fv.kingWRookRight,
               ofv.kingWRookRight);
  optimizeOpen(fv.kingBBishopLeftUp45R,
               fv.kingBBishopLeftUp45XR,
               fv.kingBBishopLeftUp45YR,
               fv.kingBBishopLeftUp45,
               ofv.kingBBishopLeftUp45);
  optimizeOpen(fv.kingWBishopLeftUp45R,
               fv.kingWBishopLeftUp45XR,
               fv.kingWBishopLeftUp45YR,
               fv.kingWBishopLeftUp45,
               ofv.kingWBishopLeftUp45);
  optimizeOpen(fv.kingBBishopRightDown45R,
               fv.kingBBishopRightDown45XR,
               fv.kingBBishopRightDown45YR,
               fv.kingBBishopRightDown45,
               ofv.kingBBishopRightDown45);
  optimizeOpen(fv.kingWBishopRightDown45R,
               fv.kingWBishopRightDown45XR,
               fv.kingWBishopRightDown45YR,
               fv.kingWBishopRightDown45,
               ofv.kingWBishopRightDown45);
  optimizeOpen(fv.kingBBishopRightUp45R,
               fv.kingBBishopRightUp45XR,
               fv.kingBBishopRightUp45YR,
               fv.kingBBishopRightUp45,
               ofv.kingBBishopRightUp45);
  optimizeOpen(fv.kingWBishopRightUp45R,
               fv.kingWBishopRightUp45XR,
               fv.kingWBishopRightUp45YR,
               fv.kingWBishopRightUp45,
               ofv.kingWBishopRightUp45);
  optimizeOpen(fv.kingBBishopLeftDown45R,
               fv.kingBBishopLeftDown45XR,
               fv.kingBBishopLeftDown45YR,
               fv.kingBBishopLeftDown45,
               ofv.kingBBishopLeftDown45);
  optimizeOpen(fv.kingWBishopLeftDown45R,
               fv.kingWBishopLeftDown45XR,
               fv.kingWBishopLeftDown45YR,
               fv.kingWBishopLeftDown45,
               ofv.kingWBishopLeftDown45);
  optimizeOpen(fv.kingBLanceR,
               fv.kingBLanceXR,
               fv.kingBLanceYR,
               fv.kingBLance,
               ofv.kingBLance);
  optimizeOpen(fv.kingWLanceR,
               fv.kingWLanceXR,
               fv.kingWLanceYR,
               fv.kingWLance,
               ofv.kingWLance);

  optimizeEffect(fv.kingAllyEffect9,
                 fv.kingAllyEffect9X,
                 fv.kingAllyEffect9Y,
                 ofv.kingAllyEffect9, 9);
  optimizeEffect(fv.kingEnemyEffect9,
                 fv.kingEnemyEffect9X,
                 fv.kingEnemyEffect9Y,
                 ofv.kingEnemyEffect9, 9);
  optimizeEffect(fv.kingAllyEffect25,
                 fv.kingAllyEffect25X,
                 fv.kingAllyEffect25Y,
                 ofv.kingAllyEffect25, 25);
  optimizeEffect(fv.kingEnemyEffect25,
                 fv.kingEnemyEffect25X,
                 fv.kingEnemyEffect25Y,
                 ofv.kingEnemyEffect25, 25);
  optimizeEffect(fv.kingEffect9Diff,
                 fv.kingEffect9DiffX,
                 fv.kingEffect9DiffY,
                 ofv.kingEffect9Diff, 18);
  optimizeEffect(fv.kingEffect25Diff,
                 fv.kingEffect25DiffX,
                 fv.kingEffect25DiffY,
                 ofv.kingEffect25Diff, 50);
}

template <class KingOpenR,
          class KingOpenXR,
          class KingOpenYR,
          class KingOpen,
          class OKingOpen>
inline
void expandOpen(KingOpenR& kingOpenR,
                KingOpenXR& kingOpenXR,
                KingOpenYR& kingOpenYR,
                KingOpen& kingOpen,
                OKingOpen& oKingOpen) {
  // kingOpen
  //   => kingOpenR, kingOpenXR, kingOpenYR, kingOpen
  SQUARE_EACH(king) {
    SQUARE_EACH(square) {
      for (int i = 0; i < 8; i++) {
        auto val = oKingOpen[king.raw()][square.raw()][i];
        kingOpenR[RelativeSquare(king, square).raw()][i] += val;
        kingOpenXR[king.getFile()-1][RelativeSquare(king, square).raw()][i] += val;
        kingOpenYR[king.getRank()-1][RelativeSquare(king, square).raw()][i] += val;
        kingOpen[king.raw()][square.raw()][i] = val;
      }
    }
  }
}

template <class PieceRNeighbor,
          class PieceXRNeighbor,
          class PieceYRNeighbor,
          class PieceNeighbor,
          class OPieceNeighbor>
inline
void expandPieceNeighbor(OPieceNeighbor&  oPieceNeighbor,
                         PieceRNeighbor&  pieceRNeighbor,
                         PieceXRNeighbor& pieceXRNeighbor,
                         PieceYRNeighbor& pieceYRNeighbor,
                         PieceNeighbor&   pieceNeighbor) {
  // kingPieceNeighbor
  //   => kingPieceRNeighbor, kingPieceXRNeighbor, kingPieceYRNeighbor, kingPieceNeighbor,
  SQUARE_EACH(king) {
    SQUARE_EACH(square) {
      for (int i = 0; i < EvalPieceIndex::End; i++) {
        for (int j = 0; j < EvalPieceIndex::End; j++) {
          auto val = oPieceNeighbor[king.raw()][square.raw()][i][j];
          pieceRNeighbor[RelativeSquare(king, square).raw()][i][j] += val;
          pieceXRNeighbor[king.getFile()-1][RelativeSquare(king, square).raw()][i][j] += val;
          pieceYRNeighbor[king.getRank()-1][RelativeSquare(king, square).raw()][i][j] += val;
          pieceNeighbor[king.raw()][square.raw()][i][j] = val;
        }
      }
    }
  }
}

template <class KingEffect,
          class KingEffectX,
          class KingEffectY,
          class OKingEffect>
inline
void expandEffect(KingEffect& kingEffect,
                  KingEffectX& kingEffectX,
                  KingEffectY& kingEffectY,
                  OKingEffect& oKingEffect,
                  int lastIdx) {
  SQUARE_EACH(king) {
    for (int i = 0; i <= lastIdx; i++) {
      auto val = oKingEffect[king.raw()][i];
      kingEffectX[king.getFile()-1][i] += val;
      kingEffectY[king.getRank()-1][i] += val;
      kingEffect[king.raw()][i] = val;
    }
  }
}

template <class FV, class OFV>
inline
void expand(FV& fv, OFV& ofv) {
  memset(reinterpret_cast<void*>(&fv), 0, sizeof(FV));

  FV_PART_COPY(fv, ofv, kingHand);

  // kingPiece
  //   => kingPieceR, kingPieceXR, kingPieceYR, kingPiece,
  SQUARE_EACH(king) {
    SQUARE_EACH(square) {
      for (int i = 0; i < EvalPieceIndex::End; i++) {
        auto val = ofv.kingPiece[king.raw()][square.raw()][i];
        fv.kingPieceR[RelativeSquare(king, square).raw()][i] += val;
        fv.kingPieceXR[king.getFile()-1][RelativeSquare(king, square).raw()][i] += val;
        fv.kingPieceYR[king.getRank()-1][RelativeSquare(king, square).raw()][i] += val;
        fv.kingPiece[king.raw()][square.raw()][i] = val;
      }
    }
  }

  //expandPieceNeighbor(ofv.kingPieceNeighborX,
  //                    fv.kingPieceRNeighborX,
  //                    fv.kingPieceXRNeighborX,
  //                    fv.kingPieceYRNeighborX,
  //                    fv.kingPieceNeighborX);

  //expandPieceNeighbor(ofv.kingPieceNeighborY,
  //                    fv.kingPieceRNeighborY,
  //                    fv.kingPieceXRNeighborY,
  //                    fv.kingPieceYRNeighborY,
  //                    fv.kingPieceNeighborY);

  //expandPieceNeighbor(ofv.kingPieceNeighborXY,
  //                    fv.kingPieceRNeighborXY,
  //                    fv.kingPieceXRNeighborXY,
  //                    fv.kingPieceYRNeighborXY,
  //                    fv.kingPieceNeighborXY);

  //expandPieceNeighbor(ofv.kingPieceNeighborXY2,
  //                    fv.kingPieceRNeighborXY2,
  //                    fv.kingPieceXRNeighborXY2,
  //                    fv.kingPieceYRNeighborXY2,
  //                    fv.kingPieceNeighborXY2);

  FV_PART_COPY(fv, ofv, kingNeighborHand);

  // kingNeighborPiece
  //   => kingNeighborPieceR, kingNeighborPieceXR, kingNeighborPieceYR, kingNeighborPiece
  //SQUARE_EACH(king) {
  //  for (int n = 0; n < Neighbor3x3::NN; n++) {
  //    for (int i1 = 0; i1 < EvalPieceTypeIndex::End; i1++) {
  //      SQUARE_EACH(square) {
  //        for (int i2 = 0; i2 < EvalPieceIndex::End; i2++) {
  //          auto val = ofv.kingNeighborPiece[king.raw()][n][i1][square.raw()][i2];
  //          fv.kingNeighborPieceR[n][i1][RelativeSquare(king, square).raw()][i2] += val;
  //          fv.kingNeighborPieceXR[king.getFile()-1][n][i1][RelativeSquare(king, square).raw()][i2] += val;
  //          fv.kingNeighborPieceYR[king.getRank()-1][n][i1][RelativeSquare(king, square).raw()][i2] += val;
  //          fv.kingNeighborPiece[king.raw()][n][i1][square.raw()][i2] = val;
  //        }
  //      }
  //    }
  //  }
  //}

  FV_PART_COPY(fv, ofv, kingKingHand);
  //FV_PART_COPY(fv, ofv, kingKingPiece);

  expandOpen(fv.kingBRookUpR,
             fv.kingBRookUpXR,
             fv.kingBRookUpYR,
             fv.kingBRookUp,
             ofv.kingBRookUp);
  expandOpen(fv.kingWRookUpR,
             fv.kingWRookUpXR,
             fv.kingWRookUpYR,
             fv.kingWRookUp,
             ofv.kingWRookUp);
  expandOpen(fv.kingBRookDownR,
             fv.kingBRookDownXR,
             fv.kingBRookDownYR,
             fv.kingBRookDown,
             ofv.kingBRookDown);
  expandOpen(fv.kingWRookDownR,
             fv.kingWRookDownXR,
             fv.kingWRookDownYR,
             fv.kingWRookDown,
             ofv.kingWRookDown);
  expandOpen(fv.kingBRookLeftR,
             fv.kingBRookLeftXR,
             fv.kingBRookLeftYR,
             fv.kingBRookLeft,
             ofv.kingBRookLeft);
  expandOpen(fv.kingWRookLeftR,
             fv.kingWRookLeftXR,
             fv.kingWRookLeftYR,
             fv.kingWRookLeft,
             ofv.kingWRookLeft);
  expandOpen(fv.kingBRookRightR,
             fv.kingBRookRightXR,
             fv.kingBRookRightYR,
             fv.kingBRookRight,
             ofv.kingBRookRight);
  expandOpen(fv.kingWRookRightR,
             fv.kingWRookRightXR,
             fv.kingWRookRightYR,
             fv.kingWRookRight,
             ofv.kingWRookRight);
  expandOpen(fv.kingBBishopLeftUp45R,
             fv.kingBBishopLeftUp45XR,
             fv.kingBBishopLeftUp45YR,
             fv.kingBBishopLeftUp45,
             ofv.kingBBishopLeftUp45);
  expandOpen(fv.kingWBishopLeftUp45R,
             fv.kingWBishopLeftUp45XR,
             fv.kingWBishopLeftUp45YR,
             fv.kingWBishopLeftUp45,
             ofv.kingWBishopLeftUp45);
  expandOpen(fv.kingBBishopRightDown45R,
             fv.kingBBishopRightDown45XR,
             fv.kingBBishopRightDown45YR,
             fv.kingBBishopRightDown45,
             ofv.kingBBishopRightDown45);
  expandOpen(fv.kingWBishopRightDown45R,
             fv.kingWBishopRightDown45XR,
             fv.kingWBishopRightDown45YR,
             fv.kingWBishopRightDown45,
             ofv.kingWBishopRightDown45);
  expandOpen(fv.kingBBishopRightUp45R,
             fv.kingBBishopRightUp45XR,
             fv.kingBBishopRightUp45YR,
             fv.kingBBishopRightUp45,
             ofv.kingBBishopRightUp45);
  expandOpen(fv.kingWBishopRightUp45R,
             fv.kingWBishopRightUp45XR,
             fv.kingWBishopRightUp45YR,
             fv.kingWBishopRightUp45,
             ofv.kingWBishopRightUp45);
  expandOpen(fv.kingBBishopLeftDown45R,
             fv.kingBBishopLeftDown45XR,
             fv.kingBBishopLeftDown45YR,
             fv.kingBBishopLeftDown45,
             ofv.kingBBishopLeftDown45);
  expandOpen(fv.kingWBishopLeftDown45R,
             fv.kingWBishopLeftDown45XR,
             fv.kingWBishopLeftDown45YR,
             fv.kingWBishopLeftDown45,
             ofv.kingWBishopLeftDown45);
  expandOpen(fv.kingBLanceR,
             fv.kingBLanceXR,
             fv.kingBLanceYR,
             fv.kingBLance,
             ofv.kingBLance);
  expandOpen(fv.kingWLanceR,
             fv.kingWLanceXR,
             fv.kingWLanceYR,
             fv.kingWLance,
             ofv.kingWLance);

  expandEffect(fv.kingAllyEffect9,
               fv.kingAllyEffect9X,
               fv.kingAllyEffect9Y,
               ofv.kingAllyEffect9, 9);
  expandEffect(fv.kingEnemyEffect9,
               fv.kingEnemyEffect9X,
               fv.kingEnemyEffect9Y,
               ofv.kingEnemyEffect9, 9);
  expandEffect(fv.kingAllyEffect25,
               fv.kingAllyEffect25X,
               fv.kingAllyEffect25Y,
               ofv.kingAllyEffect25, 25);
  expandEffect(fv.kingEnemyEffect25,
               fv.kingEnemyEffect25X,
               fv.kingEnemyEffect25Y,
               ofv.kingEnemyEffect25, 25);
  expandEffect(fv.kingEffect9Diff,
               fv.kingEffect9DiffX,
               fv.kingEffect9DiffY,
               ofv.kingEffect9Diff, 18);
  expandEffect(fv.kingEffect25Diff,
               fv.kingEffect25DiffX,
               fv.kingEffect25DiffY,
               ofv.kingEffect25Diff, 50);
}

template <class FV>
inline
void add(FV& dst, const FV& src) {
  for (size_t i = 0; i < sizeof(FV) / sizeof(typename FV::Type); i++) {
    reinterpret_cast<typename FV::Type*>(&dst)[i]
        += reinterpret_cast<const typename FV::Type*>(&src)[i];
  }
}

template <class FV, class T>
inline
void each(FV& fv, T&& func) {
  for (size_t i = 0; i < sizeof(FV) / sizeof(typename FV::Type); i++) {
    func(reinterpret_cast<typename FV::Type*>(&fv)[i]);
  }
}

template <class FV1, class FV2, class T>
inline
void each(FV1& fv1, FV2& fv2, T&& func) {
  for (size_t i = 0; i < sizeof(FV1) / sizeof(typename FV1::Type); i++) {
    func(reinterpret_cast<typename FV1::Type*>(&fv1)[i],
         reinterpret_cast<typename FV2::Type*>(&fv2)[i]);
  }
}

template <class FV1, class FV2, class FV3, class T>
inline
void each(FV1& fv1, FV2& fv2, FV3& fv3, T&& func) {
  for (size_t i = 0; i < sizeof(FV1) / sizeof(typename FV1::Type); i++) {
    func(reinterpret_cast<typename FV1::Type*>(&fv1)[i],
         reinterpret_cast<typename FV2::Type*>(&fv2)[i],
         reinterpret_cast<typename FV3::Type*>(&fv3)[i]);
  }
}

template <class FV1, class FV2, class FV3, class FV4, class T>
inline
void each(FV1& fv1, FV2& fv2, FV3& fv3, FV4& fv4, T&& func) {
  for (size_t i = 0; i < sizeof(FV1) / sizeof(typename FV1::Type); i++) {
    func(reinterpret_cast<typename FV1::Type*>(&fv1)[i],
         reinterpret_cast<typename FV2::Type*>(&fv2)[i],
         reinterpret_cast<typename FV3::Type*>(&fv3)[i],
         reinterpret_cast<typename FV4::Type*>(&fv4)[i]);
  }
}

template <class FV1, class FV2, class FV3, class FV4, class FV5, class T>
inline
void each(FV1& fv1, FV2& fv2, FV3& fv3, FV4& fv4, FV5& fv5, T&& func) {
  for (size_t i = 0; i < sizeof(FV1) / sizeof(typename FV1::Type); i++) {
    func(reinterpret_cast<typename FV1::Type*>(&fv1)[i],
         reinterpret_cast<typename FV2::Type*>(&fv2)[i],
         reinterpret_cast<typename FV3::Type*>(&fv3)[i],
         reinterpret_cast<typename FV4::Type*>(&fv4)[i],
         reinterpret_cast<typename FV5::Type*>(&fv5)[i]);
  }
}

template <class KingOpenR,
          class KingOpenXR,
          class KingOpenYR,
          class KingOpen,
          class T>
inline
void symmetrizeOpen(KingOpenR kingOpenR,
                    KingOpenXR kingOpenXR,
                    KingOpenYR kingOpenYR,
                    KingOpen kingOpen,
                    T&& func) {
  SQUARE_EACH(king) {
    auto rking = king.hsym();
    if (rking.raw() < king.raw()) {
      continue;
    }

    SQUARE_EACH(square) {
      auto rsquare = square.hsym();
      if (rking != king || rsquare.raw() > square.raw()) {
        for (int i = 0; i < 8; i++) {
          func(kingOpen[king.raw()][square.raw()][i],
               kingOpen[rking.raw()][rsquare.raw()][i]);
        }
      }
    }
  }

  for (int rs = 0; rs < RelativeSquare::N; rs++) {
    int rrs = RelativeSquare(rs).hsym().raw();
    if (rrs < rs) {
      continue;
    }

    if (rrs > rs) {
      for (int i = 0; i < 8; i++) {
        func(kingOpenR[rs][i],
             kingOpenR[rrs][i]);
      }
    }

    for (int f = 0; f < 9; f++) {
      int rf = 8 - f;
      if (rrs != rs || rf > f) {
        for (int i = 0; i < 8; i++) {
          func(kingOpenXR[f][rs][i],
               kingOpenXR[rf][rrs][i]);
        }
      }
    }

    if (rrs > rs) {
      for (int r = 0; r < 9; r++) {
        for (int i = 0; i < 8; i++) {
          func(kingOpenYR[r][rs][i],
               kingOpenYR[r][rrs][i]);
        }
      }
    }
  }
}

template <class KingOpenR,
          class KingOpenXR,
          class KingOpenYR,
          class KingOpen,
          class T>
inline
void symmetrizeOpenLR(KingOpenR kingOpenR,
                      KingOpenXR kingOpenXR,
                      KingOpenYR kingOpenYR,
                      KingOpen kingOpen,
                      KingOpenR kingOpenR2,
                      KingOpenXR kingOpenXR2,
                      KingOpenYR kingOpenYR2,
                      KingOpen kingOpen2,
                      T&& func) {
  SQUARE_EACH(king) {
    auto rking = king.hsym();
    SQUARE_EACH(square) {
      auto rsquare = square.hsym();
      for (int i = 0; i < 8; i++) {
        func(kingOpen[king.raw()][square.raw()][i],
             kingOpen2[rking.raw()][rsquare.raw()][i]);
      }
    }
  }

  for (int rs = 0; rs < RelativeSquare::N; rs++) {
    int rrs = RelativeSquare(rs).hsym().raw();
    for (int i = 0; i < 8; i++) {
      func(kingOpenR[rs][i],
           kingOpenR2[rrs][i]);
    }

    for (int f = 0; f < 9; f++) {
      int rf = 8 - f;
      for (int i = 0; i < 8; i++) {
        func(kingOpenXR[f][rs][i],
             kingOpenXR2[rf][rrs][i]);
      }
    }

    for (int r = 0; r < 9; r++) {
      for (int i = 0; i < 8; i++) {
        func(kingOpenYR[r][rs][i],
             kingOpenYR2[r][rrs][i]);
      }
    }
  }
}

template <class FV, class T>
inline
void symmetrize(FV& fv, T&& func) {
  // hand
  SQUARE_EACH(king) {
    auto rking = king.hsym();
    if (rking.raw() > king.raw()) {
      for (int h = 0; h < EvalHandIndex::End; h++) {
        func(fv.kingHand[king.raw()][h],
             fv.kingHand[rking.raw()][h]);
      }
    }
  }

  // piece
  SQUARE_EACH(king) {
    auto rking = king.hsym();

    SQUARE_EACH(square) {
      auto rsquare = square.hsym();
      if (rking.raw() > king.raw() || (rking == king && rsquare.raw() > square.raw())) {
        for (int i = 0; i < EvalPieceIndex::End; i++) {
          func(fv.kingPiece[king.raw()][square.raw()][i],
               fv.kingPiece[rking.raw()][rsquare.raw()][i]);
        }
      }

      //for (int i = 0; i < EvalPieceIndex::End; i++) {
      //  for (int j = 0; j < EvalPieceIndex::End; j++) {
      //    if (rking.raw() > king.raw() || (rking == king && rsquare.raw() > square.raw())) {
      //      if (rsquare.safetyLeft().isValid()) {
      //        func(fv.kingPieceNeighborX[king.raw()][square.raw()][i][j],
      //             fv.kingPieceNeighborX[rking.raw()][rsquare.left().raw()][j][i]);
      //      }
      //      func(fv.kingPieceNeighborY[king.raw()][square.raw()][i][j],
      //           fv.kingPieceNeighborY[rking.raw()][rsquare.raw()][i][j]);
      //    }
      //    func(fv.kingPieceNeighborXY[king.raw()][square.raw()][i][j],
      //         fv.kingPieceNeighborXY2[rking.raw()][rsquare.raw()][i][j]);
      //  }
      //}
    }
  }

  for (int rs = 0; rs < RelativeSquare::N; rs++) {
    int rrs = RelativeSquare(rs).hsym().raw();

    if (rrs > rs) {
      for (int i = 0; i < EvalPieceIndex::End; i++) {
        func(fv.kingPieceR[rs][i],
             fv.kingPieceR[rrs][i]);
      }
    }

    //for (int i = 0; i < EvalPieceIndex::End; i++) {
    //  for (int j = 0; j < EvalPieceIndex::End; j++) {
    //    if (rrs > rs) {
    //      if (RelativeSquare(rrs).getFile() > 0) {
    //        int rrsx = RelativeSquare(rrs).left().raw();
    //        func(fv.kingPieceRNeighborX[rs][i][j],
    //             fv.kingPieceRNeighborX[rrsx][j][i]);
    //      }
    //      func(fv.kingPieceRNeighborY[rs][i][j],
    //           fv.kingPieceRNeighborY[rrs][i][j]);
    //    }
    //    func(fv.kingPieceRNeighborXY[rs][i][j],
    //         fv.kingPieceRNeighborXY2[rrs][i][j]);
    //  }
    //}

    for (int f = 0; f < 9; f++) {
      int rf = 8 - f;
      if (rrs > rs || (rrs == rs && rf > f)) {
        for (int i = 0; i < EvalPieceIndex::End; i++) {
          func(fv.kingPieceXR[f][rs][i],
               fv.kingPieceXR[rf][rrs][i]);
        }
      }

      //for (int i = 0; i < EvalPieceIndex::End; i++) {
      //  for (int j = 0; j < EvalPieceIndex::End; j++) {
      //    if (rrs > rs) {
      //      if (RelativeSquare(rrs).getFile() > 0) {
      //        int rrsx = RelativeSquare(rrs).left().raw();
      //        func(fv.kingPieceXRNeighborX[f][rs][i][j],
      //             fv.kingPieceXRNeighborX[rf][rrsx][j][i]);
      //      }
      //    }
      //    if (rrs > rs || (rrs == rs && rf > f)) {
      //      func(fv.kingPieceXRNeighborY[f][rs][i][j],
      //           fv.kingPieceXRNeighborY[rf][rrs][i][j]);
      //    }
      //    func(fv.kingPieceXRNeighborXY[f][rs][i][j],
      //         fv.kingPieceXRNeighborXY2[rf][rrs][i][j]);
      //  }
      //}
    }

    for (int r = 0; r < 9; r++) {
      if (rrs > rs) {
        for (int i = 0; i < EvalPieceIndex::End; i++) {
          func(fv.kingPieceYR[r][rs][i],
               fv.kingPieceYR[r][rrs][i]);
        }
      }

      //for (int i = 0; i < EvalPieceIndex::End; i++) {
      //  for (int j = 0; j < EvalPieceIndex::End; j++) {
      //    if (rrs > rs) {
      //      if (RelativeSquare(rrs).getFile() > 0) {
      //        int rrsx = RelativeSquare(rrs).left().raw();
      //        func(fv.kingPieceYRNeighborX[r][rs][i][j],
      //             fv.kingPieceYRNeighborX[r][rrsx][j][i]);
      //      }
      //      func(fv.kingPieceYRNeighborY[r][rs][i][j],
      //           fv.kingPieceYRNeighborY[r][rrs][i][j]);
      //    }
      //    func(fv.kingPieceYRNeighborXY[r][rs][i][j],
      //         fv.kingPieceYRNeighborXY2[r][rrs][i][j]);
      //  }
      //}
    }
  }

  // neighbor
  SQUARE_EACH(king) {
    auto rking = king.hsym();
    if (rking.raw() < king.raw()) {
      continue;
    }

    for (int n = 0; n < Neighbor3x3::NN; n++) {
      auto rn = getHSymNeighbor3x3(n);
      if (rking != king || rn > n) {
        for (int i = 0; i < EvalPieceTypeIndex::End; i++) {
          for (int h = 0; h < EvalHandIndex::End; h++) {
            func(fv.kingNeighborHand[king.raw()][n][i][h],
                 fv.kingNeighborHand[rking.raw()][rn][i][h]);
          }
        }
      }
    }
  }

  SQUARE_EACH(square) {
    auto rsquare = square.hsym();
    if (rsquare.raw() < square.raw()) {
      continue;
    }

    for (int n = 0; n < Neighbor3x3::NN; n++) {
      auto rn = getHSymNeighbor3x3(n);
      if (rsquare == square && rn < n) {
        continue;
      }

      //SQUARE_EACH(king) {
      //  auto rking = king.hsym();
      //  if (rsquare != square || rn != n || rking.raw() > king.raw()) {
      //    for (int i1 = 0; i1 < EvalPieceTypeIndex::End; i1++) {
      //      for (int i2 = 0; i2 < EvalPieceIndex::End; i2++) {
      //        func(fv.kingNeighborPiece[king.raw()][n][i1][square.raw()][i2],
      //             fv.kingNeighborPiece[rking.raw()][rn][i1][rsquare.raw()][i2]);
      //      }
      //    }
      //  }
      //}
    }
  }

  for (int n = 0; n < Neighbor3x3::NN; n++) {
    auto rn = getHSymNeighbor3x3(n);
    if (rn < n) {
      continue;
    }

    for (int rs = 0; rs < RelativeSquare::N; rs++) {
      int rrs = RelativeSquare(rs).hsym().raw();
      if (rn == n && rrs < rs) {
        continue;
      }

      //if (rn != n || rrs > rs) {
      //  for (int i1 = 0; i1 < EvalPieceTypeIndex::End; i1++) {
      //    for (int i2 = 0; i2 < EvalPieceIndex::End; i2++) {
      //      func(fv.kingNeighborPieceR[n][i1][rs][i2],
      //           fv.kingNeighborPieceR[rn][i1][rrs][i2]);
      //    }
      //  }
      //}


      //for (int f = 0; f < 9; f++) {
      //  int rf = 8 - f;
      //  if (rn != n || rrs != rs || rf > f) {
      //    for (int i1 = 0; i1 < EvalPieceTypeIndex::End; i1++) {
      //      for (int i2 = 0; i2 < EvalPieceIndex::End; i2++) {
      //        func(fv.kingNeighborPieceXR[f][n][i1][rs][i2],
      //             fv.kingNeighborPieceXR[rf][rn][i1][rrs][i2]);
      //      }
      //    }
      //  }
      //}

      //if (rn != n || rrs > rs) {
      //  for (int r = 0; r < 9; r++) {
      //    for (int i1 = 0; i1 < EvalPieceTypeIndex::End; i1++) {
      //      for (int i2 = 0; i2 < EvalPieceIndex::End; i2++) {
      //        func(fv.kingNeighborPieceYR[r][n][i1][rs][i2],
      //             fv.kingNeighborPieceYR[r][rn][i1][rrs][i2]);
      //      }
      //    }
      //  }
      //}
    }
  }

  // KKH/KKP
  SQUARE_EACH(king1) {
    auto rking1 = king1.hsym();
    if (rking1.raw() < king1.raw()) {
      continue;
    }

    SQUARE_EACH(king2) {
      auto rking2 = king2.hsym();
      if (rking1 == king1 && rking2.raw() < king2.raw()) {
        continue;
      }

      if (rking1 != king1 || rking2 != king2) {
        for (int h = 0; h < EvalHandTypeIndex::End; h++) {
          func(fv.kingKingHand[king1.raw()][king2.raw()][h],
               fv.kingKingHand[rking1.raw()][rking2.raw()][h]);
        }
      }

      SQUARE_EACH(square) {
        auto rsquare = square.hsym();
        if (rking1 == king1 && rking2 == king2 && rsquare.raw() <= square.raw()) {
          continue;
        }

        //for (int i = 0; i < EvalPieceTypeIndex::End; i++) {
        //  func(fv.kingKingPiece[king1.raw()][king2.raw()][square.raw()][i],
        //       fv.kingKingPiece[rking1.raw()][rking2.raw()][rsquare.raw()][i]);
        //}
      }
    }
  }

  // open
  symmetrizeOpen(fv.kingBRookUpR,
                 fv.kingBRookUpXR,
                 fv.kingBRookUpYR,
                 fv.kingBRookUp,
                 std::forward<T>(func));
  symmetrizeOpen(fv.kingWRookUpR,
                 fv.kingWRookUpXR,
                 fv.kingWRookUpYR,
                 fv.kingWRookUp,
                 std::forward<T>(func));
  symmetrizeOpen(fv.kingBRookDownR,
                 fv.kingBRookDownXR,
                 fv.kingBRookDownYR,
                 fv.kingBRookDown,
                 std::forward<T>(func));
  symmetrizeOpen(fv.kingWRookDownR,
                 fv.kingWRookDownXR,
                 fv.kingWRookDownYR,
                 fv.kingWRookDown,
                 std::forward<T>(func));
  symmetrizeOpenLR(fv.kingBRookLeftR,
                   fv.kingBRookLeftXR,
                   fv.kingBRookLeftYR,
                   fv.kingBRookLeft,
                   fv.kingBRookRightR,
                   fv.kingBRookRightXR,
                   fv.kingBRookRightYR,
                   fv.kingBRookRight,
                   std::forward<T>(func));
  symmetrizeOpenLR(fv.kingWRookLeftR,
                   fv.kingWRookLeftXR,
                   fv.kingWRookLeftYR,
                   fv.kingWRookLeft,
                   fv.kingWRookRightR,
                   fv.kingWRookRightXR,
                   fv.kingWRookRightYR,
                   fv.kingWRookRight,
                   std::forward<T>(func));
  symmetrizeOpenLR(fv.kingBBishopLeftUp45R,
                   fv.kingBBishopLeftUp45XR,
                   fv.kingBBishopLeftUp45YR,
                   fv.kingBBishopLeftUp45,
                   fv.kingBBishopRightUp45R,
                   fv.kingBBishopRightUp45XR,
                   fv.kingBBishopRightUp45YR,
                   fv.kingBBishopRightUp45,
                   std::forward<T>(func));
  symmetrizeOpenLR(fv.kingWBishopLeftUp45R,
                   fv.kingWBishopLeftUp45XR,
                   fv.kingWBishopLeftUp45YR,
                   fv.kingWBishopLeftUp45,
                   fv.kingWBishopRightUp45R,
                   fv.kingWBishopRightUp45XR,
                   fv.kingWBishopRightUp45YR,
                   fv.kingWBishopRightUp45,
                   std::forward<T>(func));
  symmetrizeOpenLR(fv.kingBBishopRightDown45R,
                   fv.kingBBishopRightDown45XR,
                   fv.kingBBishopRightDown45YR,
                   fv.kingBBishopRightDown45,
                   fv.kingBBishopLeftDown45R,
                   fv.kingBBishopLeftDown45XR,
                   fv.kingBBishopLeftDown45YR,
                   fv.kingBBishopLeftDown45,
                   std::forward<T>(func));
  symmetrizeOpenLR(fv.kingWBishopRightDown45R,
                   fv.kingWBishopRightDown45XR,
                   fv.kingWBishopRightDown45YR,
                   fv.kingWBishopRightDown45,
                   fv.kingWBishopLeftDown45R,
                   fv.kingWBishopLeftDown45XR,
                   fv.kingWBishopLeftDown45YR,
                   fv.kingWBishopLeftDown45,
                   std::forward<T>(func));
  symmetrizeOpen(fv.kingBLanceR,
                 fv.kingBLanceXR,
                 fv.kingBLanceYR,
                 fv.kingBLance,
                 std::forward<T>(func));
  symmetrizeOpen(fv.kingWLanceR,
                 fv.kingWLanceXR,
                 fv.kingWLanceYR,
                 fv.kingWLance,
                 std::forward<T>(func));

  // effect
  SQUARE_EACH(king) {
    auto rking = king.hsym();
    if (rking.raw() <= king.raw()) {
      continue;
    }

    for (unsigned c = 0; c <= 9; c++) {
      func(fv.kingAllyEffect9[king.raw()][c],
           fv.kingAllyEffect9[rking.raw()][c]);
      func(fv.kingEnemyEffect9[king.raw()][c],
           fv.kingEnemyEffect9[rking.raw()][c]);
    }
    for (unsigned c = 0; c <= 25; c++) {
      func(fv.kingAllyEffect25[king.raw()][c],
           fv.kingAllyEffect25[rking.raw()][c]);
      func(fv.kingEnemyEffect25[king.raw()][c],
           fv.kingEnemyEffect25[rking.raw()][c]);
    }
    for (unsigned c = 0; c <= 18; c++) {
      func(fv.kingEffect9Diff[king.raw()][c],
           fv.kingEffect9Diff[rking.raw()][c]);
    }
    for (unsigned c = 0; c <= 50; c++) {
      func(fv.kingEffect25Diff[king.raw()][c],
           fv.kingEffect25Diff[rking.raw()][c]);
    }
  }

  for (int f = 0; f <= 3; f++) {
    int rf = 8 - f;
    for (unsigned c = 0; c <= 9; c++) {
      func(fv.kingAllyEffect9X[f][c],
           fv.kingAllyEffect9X[rf][c]);
      func(fv.kingEnemyEffect9X[f][c],
           fv.kingEnemyEffect9X[rf][c]);
    }
    for (unsigned c = 0; c <= 25; c++) {
      func(fv.kingAllyEffect25X[f][c],
           fv.kingAllyEffect25X[rf][c]);
      func(fv.kingEnemyEffect25X[f][c],
           fv.kingEnemyEffect25X[rf][c]);
    }
    for (unsigned c = 0; c <= 18; c++) {
      func(fv.kingEffect9DiffX[f][c],
           fv.kingEffect9DiffX[rf][c]);
    }
    for (unsigned c = 0; c <= 50; c++) {
      func(fv.kingEffect25DiffX[f][c],
           fv.kingEffect25DiffX[rf][c]);
    }
  }
}

enum FeatureOperationType {
  Evaluate,
  Extract,
};

struct NeighborPiece {
  uint8_t n;
  uint8_t idx;
};

struct FeatureMeta {
  int bking;
  int wking;
  Bitboard bnoking;
  Bitboard wnoking;
  Bitboard noking;
  NeighborPiece bns[Neighbor3x3::NN];
  int bnn = 0;
  NeighborPiece wns[Neighbor3x3::NN];
  int wnn = 0;
};

template <FeatureOperationType type, class OFV, class T, Turn turn>
inline
T operatePiece(OFV& ofv, T delta, FeatureMeta& m, int typeIndex, int bIndex, int wIndex, int bs, int ws) {
  T sum = 0;
  if (type == FeatureOperationType::Evaluate) {
    sum += ofv.kingPiece[m.bking][bs][bIndex];
    sum -= ofv.kingPiece[m.wking][ws][wIndex];
    //for (int i = 0; i < m.bnn; i++) {
    //  sum += ofv.kingNeighborPiece[m.bking][m.bns[i].n][m.bns[i].idx][bs][bIndex];
    //}
    //for (int i = 0; i < m.wnn; i++) {
    //  sum -= ofv.kingNeighborPiece[m.wking][m.wns[i].n][m.wns[i].idx][ws][wIndex];
    //}
    //if (turn == Turn::Black) {
    //  sum += ofv.kingKingPiece[m.bking][m.wking][bs][typeIndex];
    //} else {
    //  sum -= ofv.kingKingPiece[m.wking][m.bking][ws][typeIndex];
    //}
  } else {
    ofv.kingPiece[m.bking][bs][bIndex] += delta;
    ofv.kingPiece[m.wking][ws][wIndex] -= delta;
    //for (int i = 0; i < m.bnn; i++) {
    //  ofv.kingNeighborPiece[m.bking][m.bns[i].n][m.bns[i].idx][bs][bIndex] += delta;
    //}
    //for (int i = 0; i < m.wnn; i++) {
    //  ofv.kingNeighborPiece[m.wking][m.wns[i].n][m.wns[i].idx][ws][wIndex] -= delta;
    //}
    //if (turn == Turn::Black) {
    //  ofv.kingKingPiece[m.bking][m.wking][bs][typeIndex] += delta;
    //} else {
    //  ofv.kingKingPiece[m.wking][m.bking][ws][typeIndex] -= delta;
    //}
  }
  return sum;
}

template <FeatureOperationType type, class OFV, class T, Turn turn>
inline
T operateHand(OFV& ofv, T delta, FeatureMeta& m, int n, int ti, int bi, int wi) {
  T sum = 0;
  if (n != 0) {
    if (type == FeatureOperationType::Evaluate) {
      sum += ofv.kingHand[m.bking][bi + n - 1];
      sum -= ofv.kingHand[m.wking][wi + n - 1];
      for (int i = 0; i < m.bnn; i++) {
        sum += ofv.kingNeighborHand[m.bking][m.bns[i].n][m.bns[i].idx][bi + n - 1];
      }
      for (int i = 0; i < m.wnn; i++) {
        sum -= ofv.kingNeighborHand[m.wking][m.wns[i].n][m.wns[i].idx][wi + n - 1];
      }
      if (turn == Turn::Black) {
        sum += ofv.kingKingHand[m.bking][m.wking][ti + n - 1];
      } else {
        sum -= ofv.kingKingHand[m.wking][m.bking][ti + n - 1];
      }
    } else {
      ofv.kingHand[m.bking][bi + n - 1] += delta;
      ofv.kingHand[m.wking][wi + n - 1] -= delta;
      for (int i = 0; i < m.bnn; i++) {
        ofv.kingNeighborHand[m.bking][m.bns[i].n][m.bns[i].idx][bi + n - 1] += delta;
      }
      for (int i = 0; i < m.wnn; i++) {
        ofv.kingNeighborHand[m.wking][m.wns[i].n][m.wns[i].idx][wi + n - 1] -= delta;
      }
      if (turn == Turn::Black) {
        ofv.kingKingHand[m.bking][m.wking][ti + n - 1] += delta;
      } else {
        ofv.kingKingHand[m.wking][m.bking][ti + n - 1] -= delta;
      }
    }
  }
  return sum;
}

template <FeatureOperationType type, class OFV, class T>
inline
T operate(OFV& ofv, const Position& position, T delta) {
  T sum = 0;

  FeatureMeta m;

  m.bking = position.getBlackKingSquare().raw();
  m.wking = position.getWhiteKingSquare().psym().raw();

  m.bnoking = position.getBOccupiedBitboard();
  m.wnoking = position.getWOccupiedBitboard();
  m.bnoking.unset(position.getBlackKingSquare());
  m.wnoking.unset(position.getWhiteKingSquare());

  m.noking = m.bnoking | m.wnoking;

  {
    auto bb = m.bnoking & MoveTables::king(position.getBlackKingSquare());
    m.bnn = 0;
    BB_EACH(square, bb) {
      int n = getNeighbor3x3(position.getBlackKingSquare(), square);
      auto piece = position.getPieceOnBoard(square);
      m.bns[m.bnn].n = n;
      m.bns[m.bnn].idx = getEvalPieceTypeIndex(piece.type());
      m.bnn++;
    }
  }

  {
    m.wnn = 0;
    auto bb = m.wnoking & MoveTables::king(position.getWhiteKingSquare());
    BB_EACH(square, bb) {
      int n = getNeighbor3x3R(position.getWhiteKingSquare(), square);
      auto piece = position.getPieceOnBoard(square);
      m.wns[m.wnn].n = n;
      m.wns[m.wnn].idx = getEvalPieceTypeIndex(piece.type());
      m.wnn++;
    }
  }

#define CALC_BLACK_HAND(pt, PT) \
  sum += operateHand<type, OFV, T, Turn::Black>(ofv, delta, m, \
                                                position.getBlackHand().get(PieceType::pt()), \
                                                EvalHandTypeIndex::PT, \
                                                EvalHandIndex::B ## PT, \
                                                EvalHandIndex::W ## PT)
  {
    CALC_BLACK_HAND(pawn  , Pawn  );
    CALC_BLACK_HAND(lance , Lance );
    CALC_BLACK_HAND(knight, Knight);
    CALC_BLACK_HAND(silver, Silver);
    CALC_BLACK_HAND(gold  , Gold  );
    CALC_BLACK_HAND(bishop, Bishop);
    CALC_BLACK_HAND(rook  , Rook  );
  }
#undef CALC_BLACK_HAND

#define CALC_WHITE_HAND(pt, PT) \
  sum += operateHand<type, OFV, T, Turn::White>(ofv, delta, m, \
                                                position.getWhiteHand().get(PieceType::pt()), \
                                                EvalHandTypeIndex::PT, \
                                                EvalHandIndex::W ## PT, \
                                                EvalHandIndex::B ## PT)
  {
    CALC_WHITE_HAND(pawn  , Pawn  );
    CALC_WHITE_HAND(lance , Lance );
    CALC_WHITE_HAND(knight, Knight);
    CALC_WHITE_HAND(silver, Silver);
    CALC_WHITE_HAND(gold  , Gold  );
    CALC_WHITE_HAND(bishop, Bishop);
    CALC_WHITE_HAND(rook  , Rook  );
  }
#undef CALC_WHITE_HAND

  auto bef = Bitboard::zero();
  auto wef = Bitboard::zero();

  // pawn
  {
    auto bpawn = position.getBPawnBitboard();
    bef |= bpawn.up();
    BB_EACH(square, bpawn) {
      sum += operatePiece<type, OFV, T, Turn::Black>(ofv, delta, m,
                                                     EvalPieceTypeIndex::Pawn,
                                                     EvalPieceIndex::BPawn,
                                                     EvalPieceIndex::WPawn,
                                                     square.raw(),
                                                     square.psym().raw());
    }
  }

  {
    auto wpawn = position.getWPawnBitboard();
    wef |= wpawn.down();
    BB_EACH(square, wpawn) {
      sum += operatePiece<type, OFV, T, Turn::White>(ofv, delta, m,
                                                     EvalPieceTypeIndex::Pawn,
                                                     EvalPieceIndex::WPawn,
                                                     EvalPieceIndex::BPawn,
                                                     square.raw(),
                                                     square.psym().raw());
    }
  }

  // silver
  {
    auto bsilver = position.getBSilverBitboard();
    bef |= bsilver.leftUp();
    bef |= bsilver.leftDown();
    bef |= bsilver.up();
    bef |= bsilver.rightUp();
    bef |= bsilver.rightDown();
    BB_EACH(square, bsilver) {
      sum += operatePiece<type, OFV, T, Turn::Black>(ofv, delta, m,
                                                     EvalPieceTypeIndex::Silver,
                                                     EvalPieceIndex::BSilver,
                                                     EvalPieceIndex::WSilver,
                                                     square.raw(),
                                                     square.psym().raw());
    }
  }

  {
    auto wsilver = position.getWSilverBitboard();
    wef |= wsilver.leftUp();
    wef |= wsilver.leftDown();
    wef |= wsilver.down();
    wef |= wsilver.rightUp();
    wef |= wsilver.rightDown();
    BB_EACH(square, wsilver) {
      sum += operatePiece<type, OFV, T, Turn::White>(ofv, delta, m,
                                                     EvalPieceTypeIndex::Silver,
                                                     EvalPieceIndex::WSilver,
                                                     EvalPieceIndex::BSilver,
                                                     square.raw(),
                                                     square.psym().raw());
    }
  }

  // gold, tokin, promoted-lance, promoted-knight, promoted-siver
  {
    auto bgold = position.getBGoldBitboard();
    bef |= bgold.leftUp();
    bef |= bgold.left();
    bef |= bgold.up();
    bef |= bgold.down();
    bef |= bgold.rightUp();
    bef |= bgold.right();
    BB_EACH(square, bgold) {
      sum += operatePiece<type, OFV, T, Turn::Black>(ofv, delta, m,
                                                     EvalPieceTypeIndex::Gold,
                                                     EvalPieceIndex::BGold,
                                                     EvalPieceIndex::WGold,
                                                     square.raw(),
                                                     square.psym().raw());
    }
  }

  {
    auto wgold = position.getWGoldBitboard();
    wef |= wgold.left();
    wef |= wgold.leftDown();
    wef |= wgold.up();
    wef |= wgold.down();
    wef |= wgold.right();
    wef |= wgold.rightDown();
    BB_EACH(square, wgold) {
      sum += operatePiece<type, OFV, T, Turn::White>(ofv, delta, m,
                                                     EvalPieceTypeIndex::Gold,
                                                     EvalPieceIndex::WGold,
                                                     EvalPieceIndex::BGold,
                                                     square.raw(),
                                                     square.psym().raw());
    }
  }

  auto occR45 = position.getRight45RotatedBitboard();
  auto occL45 = position.getLeft45RotatedBitboard();

  // bishop
  {
    auto bbishop = position.getBBishopBitboard();
    BB_EACH(square, bbishop) {
      int bs = square.raw();
      int ws = square.psym().raw();
      sum += operatePiece<type, OFV, T, Turn::Black>(ofv, delta, m,
                                                     EvalPieceTypeIndex::Bishop,
                                                     EvalPieceIndex::BBishop,
                                                     EvalPieceIndex::WBishop,
                                                     bs, ws);

      auto eff = MoveTables::rightUp45(occR45, square);
      bef |= eff;

      int count = eff.count();
      if (count != 0) { count--; }
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingBBishopRightUp45[m.bking][bs][count];
        sum -= ofv.kingWBishopRightUp45[m.wking][ws][count];
      } else {
        ofv.kingBBishopRightUp45[m.bking][bs][count] += delta;
        ofv.kingWBishopRightUp45[m.wking][ws][count] -= delta;
      }

      eff = MoveTables::leftDown45(occR45, square);
      bef |= eff;

      count = eff.count();
      if (count != 0) { count--; }
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingBBishopLeftDown45[m.bking][bs][count];
        sum -= ofv.kingWBishopLeftDown45[m.wking][ws][count];
      } else {
        ofv.kingBBishopLeftDown45[m.bking][bs][count] += delta;
        ofv.kingWBishopLeftDown45[m.wking][ws][count] -= delta;
      }

      eff = MoveTables::leftUp45(occL45, square);
      bef |= eff;

      count = eff.count();
      if (count != 0) { count--; }
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingBBishopLeftUp45[m.bking][bs][count];
        sum -= ofv.kingWBishopLeftUp45[m.wking][ws][count];
      } else {
        ofv.kingBBishopLeftUp45[m.bking][bs][count] += delta;
        ofv.kingWBishopLeftUp45[m.wking][ws][count] -= delta;
      }

      eff = MoveTables::rightDown45(occL45, square);
      bef |= eff;

      count = eff.count();
      if (count != 0) { count--; }
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingBBishopRightDown45[m.bking][bs][count];
        sum -= ofv.kingWBishopRightDown45[m.wking][ws][count];
      } else {
        ofv.kingBBishopRightDown45[m.bking][bs][count] += delta;
        ofv.kingWBishopRightDown45[m.wking][ws][count] -= delta;
      }
    }
  }

  {
    auto wbishop = position.getWBishopBitboard();
    BB_EACH(square, wbishop) {
      int bs = square.raw();
      int ws = square.psym().raw();
      sum += operatePiece<type, OFV, T, Turn::White>(ofv, delta, m,
                                                     EvalPieceTypeIndex::Bishop,
                                                     EvalPieceIndex::WBishop,
                                                     EvalPieceIndex::BBishop,
                                                     bs, ws);

      auto eff = MoveTables::leftDown45(occR45, square);
      wef |= eff;

      int count = eff.count();
      if (count != 0) { count--; }
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingWBishopRightUp45[m.bking][bs][count];
        sum -= ofv.kingBBishopRightUp45[m.wking][ws][count];
      } else {
        ofv.kingWBishopRightUp45[m.bking][bs][count] += delta;
        ofv.kingBBishopRightUp45[m.wking][ws][count] -= delta;
      }

      eff = MoveTables::rightUp45(occR45, square);
      wef |= eff;

      count = eff.count();
      if (count != 0) { count--; }
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingWBishopLeftDown45[m.bking][bs][count];
        sum -= ofv.kingBBishopLeftDown45[m.wking][ws][count];
      } else {
        ofv.kingWBishopLeftDown45[m.bking][bs][count] += delta;
        ofv.kingBBishopLeftDown45[m.wking][ws][count] -= delta;
      }

      eff = MoveTables::rightDown45(occL45, square);
      wef |= eff;

      count = eff.count();
      if (count != 0) { count--; }
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingWBishopLeftUp45[m.bking][bs][count];
        sum -= ofv.kingBBishopLeftUp45[m.wking][ws][count];
      } else {
        ofv.kingWBishopLeftUp45[m.bking][bs][count] += delta;
        ofv.kingBBishopLeftUp45[m.wking][ws][count] -= delta;
      }

      eff = MoveTables::leftUp45(occL45, square);
      wef |= eff;

      count = eff.count();
      if (count != 0) { count--; }
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingWBishopRightDown45[m.bking][bs][count];
        sum -= ofv.kingBBishopRightDown45[m.wking][ws][count];
      } else {
        ofv.kingWBishopRightDown45[m.bking][bs][count] += delta;
        ofv.kingBBishopRightDown45[m.wking][ws][count] -= delta;
      }
    }
  }

  // horse
  {
    auto bhorse = position.getBHorseBitboard();
    bef |= bhorse.left();
    bef |= bhorse.up();
    bef |= bhorse.down();
    bef |= bhorse.right();
    BB_EACH(square, bhorse) {
      int bs = square.raw();
      int ws = square.psym().raw();
      sum += operatePiece<type, OFV, T, Turn::Black>(ofv, delta, m,
                                                     EvalPieceTypeIndex::Horse,
                                                     EvalPieceIndex::BHorse,
                                                     EvalPieceIndex::WHorse,
                                                     bs, ws);

      auto eff = MoveTables::rightUp45(occR45, square);
      bef |= eff;

      int count = eff.count();
      if (count != 0) { count--; }
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingBBishopRightUp45[m.bking][bs][count];
        sum -= ofv.kingWBishopRightUp45[m.wking][ws][count];
      } else {
        ofv.kingBBishopRightUp45[m.bking][bs][count] += delta;
        ofv.kingWBishopRightUp45[m.wking][ws][count] -= delta;
      }

      eff = MoveTables::leftDown45(occR45, square);
      bef |= eff;

      count = eff.count();
      if (count != 0) { count--; }
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingBBishopLeftDown45[m.bking][bs][count];
        sum -= ofv.kingWBishopLeftDown45[m.wking][ws][count];
      } else {
        ofv.kingBBishopLeftDown45[m.bking][bs][count] += delta;
        ofv.kingWBishopLeftDown45[m.wking][ws][count] -= delta;
      }

      eff = MoveTables::leftUp45(occL45, square);
      bef |= eff;

      count = eff.count();
      if (count != 0) { count--; }
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingBBishopLeftUp45[m.bking][bs][count];
        sum -= ofv.kingWBishopLeftUp45[m.wking][ws][count];
      } else {
        ofv.kingBBishopLeftUp45[m.bking][bs][count] += delta;
        ofv.kingWBishopLeftUp45[m.wking][ws][count] -= delta;
      }

      eff = MoveTables::rightDown45(occL45, square);
      bef |= eff;

      count = eff.count();
      if (count != 0) { count--; }
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingBBishopRightDown45[m.bking][bs][count];
        sum -= ofv.kingWBishopRightDown45[m.wking][ws][count];
      } else {
        ofv.kingBBishopRightDown45[m.bking][bs][count] += delta;
        ofv.kingWBishopRightDown45[m.wking][ws][count] -= delta;
      }
    }
  }

  {
    auto whorse = position.getWHorseBitboard();
    wef |= whorse.left();
    wef |= whorse.up();
    wef |= whorse.down();
    wef |= whorse.right();
    BB_EACH(square, whorse) {
      int bs = square.raw();
      int ws = square.psym().raw();
      sum += operatePiece<type, OFV, T, Turn::White>(ofv, delta, m,
                                                     EvalPieceTypeIndex::Horse,
                                                     EvalPieceIndex::WHorse,
                                                     EvalPieceIndex::BHorse,
                                                     bs, ws);

      auto eff = MoveTables::leftDown45(occR45, square);
      wef |= eff;

      int count = eff.count();
      if (count != 0) { count--; }
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingWBishopRightUp45[m.bking][bs][count];
        sum -= ofv.kingBBishopRightUp45[m.wking][ws][count];
      } else {
        ofv.kingWBishopRightUp45[m.bking][bs][count] += delta;
        ofv.kingBBishopRightUp45[m.wking][ws][count] -= delta;
      }

      eff = MoveTables::rightUp45(occR45, square);
      wef |= eff;

      count = eff.count();
      if (count != 0) { count--; }
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingWBishopLeftDown45[m.bking][bs][count];
        sum -= ofv.kingBBishopLeftDown45[m.wking][ws][count];
      } else {
        ofv.kingWBishopLeftDown45[m.bking][bs][count] += delta;
        ofv.kingBBishopLeftDown45[m.wking][ws][count] -= delta;
      }

      eff = MoveTables::rightDown45(occL45, square);
      wef |= eff;

      count = eff.count();
      if (count != 0) { count--; }
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingWBishopLeftUp45[m.bking][bs][count];
        sum -= ofv.kingBBishopLeftUp45[m.wking][ws][count];
      } else {
        ofv.kingWBishopLeftUp45[m.bking][bs][count] += delta;
        ofv.kingBBishopLeftUp45[m.wking][ws][count] -= delta;
      }

      eff = MoveTables::leftUp45(occL45, square);
      wef |= eff;

      count = eff.count();
      if (count != 0) { count--; }
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingWBishopRightDown45[m.bking][bs][count];
        sum -= ofv.kingBBishopRightDown45[m.wking][ws][count];
      } else {
        ofv.kingWBishopRightDown45[m.bking][bs][count] += delta;
        ofv.kingBBishopRightDown45[m.wking][ws][count] -= delta;
      }
    }
  }

  auto occ = position.getBOccupiedBitboard() | position.getWOccupiedBitboard();
  auto occ90 = position.get90RotatedBitboard();

  // rook
  {
    auto brook = position.getBRookBitboard();
    BB_EACH(square, brook) {
      int bs = square.raw();
      int ws = square.psym().raw();
      sum += operatePiece<type, OFV, T, Turn::Black>(ofv, delta, m,
                                                     EvalPieceTypeIndex::Rook,
                                                     EvalPieceIndex::BRook,
                                                     EvalPieceIndex::WRook,
                                                     bs, ws);

      auto eff = MoveTables::up(occ, square);
      bef |= eff;

      int count = eff.count();
      if (count != 0) { count--; }
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingBRookUp[m.bking][bs][count];
        sum -= ofv.kingWRookUp[m.wking][ws][count];
      } else {
        ofv.kingBRookUp[m.bking][bs][count] += delta;
        ofv.kingWRookUp[m.wking][ws][count] -= delta;
      }

      eff = MoveTables::down(occ, square);
      bef |= eff;

      count = eff.count();
      if (count != 0) { count--; }
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingBRookDown[m.bking][bs][count];
        sum -= ofv.kingWRookDown[m.wking][ws][count];
      } else {
        ofv.kingBRookDown[m.bking][bs][count] += delta;
        ofv.kingWRookDown[m.wking][ws][count] -= delta;
      }

      eff = MoveTables::left(occ90, square);
      bef |= eff;

      count = eff.count();
      if (count != 0) { count--; }
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingBRookLeft[m.bking][bs][count];
        sum -= ofv.kingWRookLeft[m.wking][ws][count];
      } else {
        ofv.kingBRookLeft[m.bking][bs][count] += delta;
        ofv.kingWRookLeft[m.wking][ws][count] -= delta;
      }

      eff = MoveTables::right(occ90, square);
      bef |= eff;

      count = eff.count();
      if (count != 0) { count--; }
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingBRookRight[m.bking][bs][count];
        sum -= ofv.kingWRookRight[m.wking][ws][count];
      } else {
        ofv.kingBRookRight[m.bking][bs][count] += delta;
        ofv.kingWRookRight[m.wking][ws][count] -= delta;
      }
    }
  }

  {
    auto wrook = position.getWRookBitboard();
    BB_EACH(square, wrook) {
      int bs = square.raw();
      int ws = square.psym().raw();
      sum += operatePiece<type, OFV, T, Turn::White>(ofv, delta, m,
                                                     EvalPieceTypeIndex::Rook,
                                                     EvalPieceIndex::WRook,
                                                     EvalPieceIndex::BRook,
                                                     bs, ws);

      auto eff = MoveTables::down(occ, square);
      wef |= eff;

      int count = eff.count();
      if (count != 0) { count--; }
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingWRookUp[m.bking][bs][count];
        sum -= ofv.kingBRookUp[m.wking][ws][count];
      } else {
        ofv.kingWRookUp[m.bking][bs][count] += delta;
        ofv.kingBRookUp[m.wking][ws][count] -= delta;
      }

      eff = MoveTables::up(occ, square);
      wef |= eff;

      count = eff.count();
      if (count != 0) { count--; }
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingWRookDown[m.bking][bs][count];
        sum -= ofv.kingBRookDown[m.wking][ws][count];
      } else {
        ofv.kingWRookDown[m.bking][bs][count] += delta;
        ofv.kingBRookDown[m.wking][ws][count] -= delta;
      }

      eff = MoveTables::right(occ90, square);
      wef |= eff;

      count = eff.count();
      if (count != 0) { count--; }
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingWRookLeft[m.bking][bs][count];
        sum -= ofv.kingBRookLeft[m.wking][ws][count];
      } else {
        ofv.kingWRookLeft[m.bking][bs][count] += delta;
        ofv.kingBRookLeft[m.wking][ws][count] -= delta;
      }

      eff = MoveTables::left(occ90, square);
      wef |= eff;

      count = eff.count();
      if (count != 0) { count--; }
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingWRookRight[m.bking][bs][count];
        sum -= ofv.kingBRookRight[m.wking][ws][count];
      } else {
        ofv.kingWRookRight[m.bking][bs][count] += delta;
        ofv.kingBRookRight[m.wking][ws][count] -= delta;
      }
    }
  }

#if 0
  // dragon
  {
    auto bdragon = position.getBDragonBitboard();
    bef |= bdragon.leftUp();
    bef |= bdragon.leftDown();
    bef |= bdragon.rightUp();
    bef |= bdragon.rightDown();
    BB_EACH(square, bdragon) {
      int bs = square.raw();
      int ws = square.psym().raw();
      sum += operatePiece<type, OFV, T, Turn::Black>(ofv, delta, m,
                                                     EvalPieceTypeIndex::Dragon,
                                                     EvalPieceIndex::BDragon,
                                                     EvalPieceIndex::WDragon,
                                                     bs, ws);

      auto eff = MoveTables::up(occ, square);
      bef |= eff;

      int count = eff.count();
      if (count != 0) { count--; }
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingBRookUp[m.bking][bs][count];
        sum -= ofv.kingWRookUp[m.wking][ws][count];
      } else {
        ofv.kingBRookUp[m.bking][bs][count] += delta;
        ofv.kingWRookUp[m.wking][ws][count] -= delta;
      }

      eff = MoveTables::down(occ, square);
      bef |= eff;

      count = eff.count();
      if (count != 0) { count--; }
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingBRookDown[m.bking][bs][count];
        sum -= ofv.kingWRookDown[m.wking][ws][count];
      } else {
        ofv.kingBRookDown[m.bking][bs][count] += delta;
        ofv.kingWRookDown[m.wking][ws][count] -= delta;
      }

      eff = MoveTables::left(occ90, square);
      bef |= eff;

      count = eff.count();
      if (count != 0) { count--; }
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingBRookLeft[m.bking][bs][count];
        sum -= ofv.kingWRookLeft[m.wking][ws][count];
      } else {
        ofv.kingBRookLeft[m.bking][bs][count] += delta;
        ofv.kingWRookLeft[m.wking][ws][count] -= delta;
      }

      eff = MoveTables::right(occ90, square);
      bef |= eff;

      count = eff.count();
      if (count != 0) { count--; }
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingBRookRight[m.bking][bs][count];
        sum -= ofv.kingWRookRight[m.wking][ws][count];
      } else {
        ofv.kingBRookRight[m.bking][bs][count] += delta;
        ofv.kingWRookRight[m.wking][ws][count] -= delta;
      }
    }
  }

  {
    auto wdragon = position.getWDragonBitboard();
    wef |= wdragon.leftUp();
    wef |= wdragon.leftDown();
    wef |= wdragon.rightUp();
    wef |= wdragon.rightDown();
    BB_EACH(square, wdragon) {
      int bs = square.raw();
      int ws = square.psym().raw();
      sum += operatePiece<type, OFV, T, Turn::White>(ofv, delta, m,
                                                     EvalPieceTypeIndex::Dragon,
                                                     EvalPieceIndex::WDragon,
                                                     EvalPieceIndex::BDragon,
                                                     bs, ws);

      auto eff = MoveTables::down(occ, square);
      wef |= eff;

      int count = eff.count();
      if (count != 0) { count--; }
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingWRookUp[m.bking][bs][count];
        sum -= ofv.kingBRookUp[m.wking][ws][count];
      } else {
        ofv.kingWRookUp[m.bking][bs][count] += delta;
        ofv.kingBRookUp[m.wking][ws][count] -= delta;
      }

      eff = MoveTables::up(occ, square);
      wef |= eff;

      count = eff.count();
      if (count != 0) { count--; }
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingWRookDown[m.bking][bs][count];
        sum -= ofv.kingBRookDown[m.wking][ws][count];
      } else {
        ofv.kingWRookDown[m.bking][bs][count] += delta;
        ofv.kingBRookDown[m.wking][ws][count] -= delta;
      }

      eff = MoveTables::right(occ90, square);
      wef |= eff;

      count = eff.count();
      if (count != 0) { count--; }
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingWRookLeft[m.bking][bs][count];
        sum -= ofv.kingBRookLeft[m.wking][ws][count];
      } else {
        ofv.kingWRookLeft[m.bking][bs][count] += delta;
        ofv.kingBRookLeft[m.wking][ws][count] -= delta;
      }

      eff = MoveTables::left(occ90, square);
      wef |= eff;

      count = eff.count();
      if (count != 0) { count--; }
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingWRookRight[m.bking][bs][count];
        sum -= ofv.kingBRookRight[m.wking][ws][count];
      } else {
        ofv.kingWRookRight[m.bking][bs][count] += delta;
        ofv.kingBRookRight[m.wking][ws][count] -= delta;
      }
    }
  }
#endif

  // lance
  {
    auto blance = position.getBLanceBitboard();
    BB_EACH(square, blance) {
      int bs = square.raw();
      int ws = square.psym().raw();
      sum += operatePiece<type, OFV, T, Turn::Black>(ofv, delta, m,
                                                     EvalPieceTypeIndex::Lance,
                                                     EvalPieceIndex::BLance,
                                                     EvalPieceIndex::WLance,
                                                     bs, ws);

      auto eff = MoveTables::blackLance(occ, square);
      bef |= eff;

      int count = eff.count() - 1;
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingBLance[m.bking][bs][count];
        sum -= ofv.kingWLance[m.wking][ws][count];
      } else {
        ofv.kingBLance[m.bking][bs][count] += delta;
        ofv.kingWLance[m.wking][ws][count] -= delta;
      }
    }
  }

  {
    auto wlance = position.getWLanceBitboard();
    BB_EACH(square, wlance) {
      int bs = square.raw();
      int ws = square.psym().raw();
      sum += operatePiece<type, OFV, T, Turn::White>(ofv, delta, m,
                                                     EvalPieceTypeIndex::Lance,
                                                     EvalPieceIndex::WLance,
                                                     EvalPieceIndex::BLance,
                                                     bs, ws);

      auto eff = MoveTables::whiteLance(occ, square);
      wef |= eff;

      int count = eff.count() - 1;
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingWLance[m.bking][bs][count];
        sum -= ofv.kingBLance[m.wking][ws][count];
      } else {
        ofv.kingWLance[m.bking][bs][count] += delta;
        ofv.kingBLance[m.wking][ws][count] -= delta;
      }
    }
  }

  // knight
  {
    auto bknight = position.getBKnightBitboard();
    bef |= bknight.leftUpKnight();
    bef |= bknight.rightUpKnight();
    BB_EACH(square, bknight) {
      int bs = square.raw();
      int ws = square.psym().raw();
      sum += operatePiece<type, OFV, T, Turn::Black>(ofv, delta, m,
                                                     EvalPieceTypeIndex::Knight,
                                                     EvalPieceIndex::BKnight,
                                                     EvalPieceIndex::WKnight,
                                                     bs, ws);
    }
  }

  {
    auto wknight = position.getWKnightBitboard();
    wef |= wknight.leftDownKnight();
    wef |= wknight.rightDownKnight();
    BB_EACH(square, wknight) {
      int bs = square.raw();
      int ws = square.psym().raw();
      sum += operatePiece<type, OFV, T, Turn::White>(ofv, delta, m,
                                                     EvalPieceTypeIndex::Knight,
                                                     EvalPieceIndex::WKnight,
                                                     EvalPieceIndex::BKnight,
                                                     bs, ws);
    }
  }

  // neighbor-x
  //{
  //  auto bb = m.noking & m.noking.left();
  //  BB_EACH(square, bb) {
  //    int bs = square.raw();
  //    int ws = square.right().psym().raw();
  //    Piece piece1 = position.getPieceOnBoard(square);
  //    Piece piece2 = position.getPieceOnBoard(square.right());
  //    ASSERT(!piece1.isEmpty());
  //    ASSERT(!piece2.isEmpty());
  //    int bIndex1 = getEvalPieceIndex(piece1);
  //    int bIndex2 = getEvalPieceIndex(piece2);
  //    int wIndex1 = getEvalPieceIndex(piece2.enemy());
  //    int wIndex2 = getEvalPieceIndex(piece1.enemy());
  //    if (type == FeatureOperationType::Evaluate) {
  //      sum += ofv.kingPieceNeighborX[m.bking][bs][bIndex1][bIndex2];
  //      sum -= ofv.kingPieceNeighborX[m.wking][ws][wIndex1][wIndex2];
  //    } else {
  //      ofv.kingPieceNeighborX[m.bking][bs][bIndex1][bIndex2] += delta;
  //      ofv.kingPieceNeighborX[m.wking][ws][wIndex1][wIndex2] -= delta;
  //    }
  //  }
  //}

  // neighbor-y
  //{
  //  auto bb = m.noking & m.noking.down();
  //  BB_EACH(square, bb) {
  //    int bs = square.raw();
  //    int ws = square.up().psym().raw();
  //    Piece piece1 = position.getPieceOnBoard(square);
  //    Piece piece2 = position.getPieceOnBoard(square.up());
  //    ASSERT(!piece1.isEmpty());
  //    ASSERT(!piece2.isEmpty());
  //    int bIndex1 = getEvalPieceIndex(piece1);
  //    int bIndex2 = getEvalPieceIndex(piece2);
  //    int wIndex1 = getEvalPieceIndex(piece2.enemy());
  //    int wIndex2 = getEvalPieceIndex(piece1.enemy());
  //    if (type == FeatureOperationType::Evaluate) {
  //      sum += ofv.kingPieceNeighborY[m.bking][bs][bIndex1][bIndex2];
  //      sum -= ofv.kingPieceNeighborY[m.wking][ws][wIndex1][wIndex2];
  //    } else {
  //      ofv.kingPieceNeighborY[m.bking][bs][bIndex1][bIndex2] += delta;
  //      ofv.kingPieceNeighborY[m.wking][ws][wIndex1][wIndex2] -= delta;
  //    }
  //  }
  //}

  // neighbor-xy
  //{
  //  auto bb = m.noking & m.noking.leftDown();
  //  BB_EACH(square, bb) {
  //    int bs = square.raw();
  //    int ws = square.rightUp().psym().raw();
  //    Piece piece1 = position.getPieceOnBoard(square);
  //    Piece piece2 = position.getPieceOnBoard(square.rightUp());
  //    ASSERT(!piece1.isEmpty());
  //    ASSERT(!piece2.isEmpty());
  //    int bIndex1 = getEvalPieceIndex(piece1);
  //    int bIndex2 = getEvalPieceIndex(piece2);
  //    int wIndex1 = getEvalPieceIndex(piece2.enemy());
  //    int wIndex2 = getEvalPieceIndex(piece1.enemy());
  //    if (type == FeatureOperationType::Evaluate) {
  //      sum += ofv.kingPieceNeighborXY[m.bking][bs][bIndex1][bIndex2];
  //      sum -= ofv.kingPieceNeighborXY[m.wking][ws][wIndex1][wIndex2];
  //    } else {
  //      ofv.kingPieceNeighborXY[m.bking][bs][bIndex1][bIndex2] += delta;
  //      ofv.kingPieceNeighborXY[m.wking][ws][wIndex1][wIndex2] -= delta;
  //    }
  //  }
  //}

  // neighbor-xy2
  //{
  //  auto bb = m.noking & m.noking.rightDown();
  //  BB_EACH(square, bb) {
  //    int bs = square.raw();
  //    int ws = square.leftUp().psym().raw();
  //    Piece piece1 = position.getPieceOnBoard(square);
  //    Piece piece2 = position.getPieceOnBoard(square.leftUp());
  //    ASSERT(!piece1.isEmpty());
  //    ASSERT(!piece2.isEmpty());
  //    int bIndex1 = getEvalPieceIndex(piece1);
  //    int bIndex2 = getEvalPieceIndex(piece2);
  //    int wIndex1 = getEvalPieceIndex(piece2.enemy());
  //    int wIndex2 = getEvalPieceIndex(piece1.enemy());
  //    if (type == FeatureOperationType::Evaluate) {
  //      sum += ofv.kingPieceNeighborXY2[m.bking][bs][bIndex1][bIndex2];
  //      sum -= ofv.kingPieceNeighborXY2[m.wking][ws][wIndex1][wIndex2];
  //    } else {
  //      ofv.kingPieceNeighborXY2[m.bking][bs][bIndex1][bIndex2] += delta;
  //      ofv.kingPieceNeighborXY2[m.wking][ws][wIndex1][wIndex2] -= delta;
  //    }
  //  }
  //}

  {
    auto& mask = MoveTables::neighbor3x3(position.getBlackKingSquare());

    auto bc = (bef & mask).count();
    auto wc = (wef & mask).count();
    if (type == FeatureOperationType::Evaluate) {
      sum += ofv.kingAllyEffect9[m.bking][bc];
      sum += ofv.kingEnemyEffect9[m.bking][wc];
      sum += ofv.kingEffect9Diff[m.bking][9+bc-wc];
    } else {
      ofv.kingAllyEffect9[m.bking][bc] += delta;
      ofv.kingEnemyEffect9[m.bking][wc] += delta;
      ofv.kingEffect9Diff[m.bking][9+bc-wc] += delta;
    }
  }

  {
    auto& mask = MoveTables::neighbor5x5(position.getBlackKingSquare());

    auto bc = (bef & mask).count();
    auto wc = (wef & mask).count();
    if (type == FeatureOperationType::Evaluate) {
      sum += ofv.kingAllyEffect25[m.bking][bc];
      sum += ofv.kingEnemyEffect25[m.bking][wc];
      sum += ofv.kingEffect25Diff[m.bking][25+bc-wc];
    } else {
      ofv.kingAllyEffect25[m.bking][bc] += delta;
      ofv.kingEnemyEffect25[m.bking][wc] += delta;
      ofv.kingEffect25Diff[m.bking][25+bc-wc] += delta;
    }
  }

  {
    auto& mask = MoveTables::neighbor3x3(position.getWhiteKingSquare());

    auto bc = (bef & mask).count();
    auto wc = (wef & mask).count();
    if (type == FeatureOperationType::Evaluate) {
      sum -= ofv.kingAllyEffect9[m.wking][wc];
      sum -= ofv.kingEnemyEffect9[m.wking][bc];
      sum -= ofv.kingEffect9Diff[m.wking][9+wc-bc];
    } else {
      ofv.kingAllyEffect9[m.wking][wc] -= delta;
      ofv.kingEnemyEffect9[m.wking][bc] -= delta;
      ofv.kingEffect9Diff[m.wking][9+wc-bc] -= delta;
    }
  }

  {
    auto& mask = MoveTables::neighbor5x5(position.getWhiteKingSquare());

    auto bc = (bef & mask).count();
    auto wc = (wef & mask).count();
    if (type == FeatureOperationType::Evaluate) {
      sum -= ofv.kingAllyEffect25[m.wking][wc];
      sum -= ofv.kingEnemyEffect25[m.wking][bc];
      sum -= ofv.kingEffect25Diff[m.wking][25+wc-bc];
    } else {
      ofv.kingAllyEffect25[m.wking][wc] -= delta;
      ofv.kingEnemyEffect25[m.wking][bc] -= delta;
      ofv.kingEffect25Diff[m.wking][25+wc-bc] -= delta;
    }
  }

  return sum;
}

template <class T>
struct FVSummary {
  using Type = T;

  std::string name;
  int num;
  int zero;
  int nonZero;
  Type max;
  Type min;
  Type maxAbs;
  Type aveAbs;
};

template <class SummaryType, class T>
inline
SummaryType summarizePart(const char* name, const T* array, size_t size) {
  SummaryType summary;
  summary.name = name;
  summary.num = size;
  summary.zero = 0;
  summary.max = array[0];
  summary.min = array[0];
  double sum = 0;

  for (size_t i = 0; i < size; i++) {
    if (array[i] == static_cast<T>(0)) {
      summary.zero++;
    }
    summary.max = std::max(summary.max, array[i]);
    summary.min = std::min(summary.min, array[i]);
    sum += std::abs(array[i]);
  }
  summary.nonZero = summary.num - summary.zero;
  summary.maxAbs = std::max(static_cast<T>(summary.max),
                            static_cast<T>(std::abs(summary.min)));
  summary.aveAbs = sum / size;

  return summary;
}

template <class FV,
          class SummaryType = FVSummary<typename FV::Type>,
          class SummaryListType = std::vector<SummaryType>>
inline
SummaryListType summarize(const FV& fv) {
#define FV_PART_PTR(part)  (reinterpret_cast<const typename FV::Type*>(fv.part))
#define FV_PART_SIZE(part) (sizeof(fv.part) / sizeof(typename FV::Type))
#define FV_SUMMARIZE(part) (summarizePart<SummaryType>(#part, FV_PART_PTR(part), FV_PART_SIZE(part)))
  return SummaryListType {
    FV_SUMMARIZE(kingHand),
    FV_SUMMARIZE(kingPieceR),
    FV_SUMMARIZE(kingPieceXR),
    FV_SUMMARIZE(kingPieceYR),
    FV_SUMMARIZE(kingPiece),
    //FV_SUMMARIZE(kingPieceRNeighborX),
    //FV_SUMMARIZE(kingPieceXRNeighborX),
    //FV_SUMMARIZE(kingPieceYRNeighborX),
    //FV_SUMMARIZE(kingPieceNeighborX),
    //FV_SUMMARIZE(kingPieceRNeighborY),
    //FV_SUMMARIZE(kingPieceXRNeighborY),
    //FV_SUMMARIZE(kingPieceYRNeighborY),
    //FV_SUMMARIZE(kingPieceNeighborY),
    //FV_SUMMARIZE(kingPieceRNeighborXY),
    //FV_SUMMARIZE(kingPieceXRNeighborXY),
    //FV_SUMMARIZE(kingPieceYRNeighborXY),
    //FV_SUMMARIZE(kingPieceNeighborXY),
    //FV_SUMMARIZE(kingPieceRNeighborXY2),
    //FV_SUMMARIZE(kingPieceXRNeighborXY2),
    //FV_SUMMARIZE(kingPieceYRNeighborXY2),
    //FV_SUMMARIZE(kingPieceNeighborXY2),
    FV_SUMMARIZE(kingNeighborHand),
    //FV_SUMMARIZE(kingNeighborPieceR),
    //FV_SUMMARIZE(kingNeighborPieceXR),
    //FV_SUMMARIZE(kingNeighborPieceYR),
    //FV_SUMMARIZE(kingNeighborPiece),
    FV_SUMMARIZE(kingKingHand),
    //FV_SUMMARIZE(kingKingPiece),
    FV_SUMMARIZE(kingBRookUpR),
    FV_SUMMARIZE(kingBRookUpXR),
    FV_SUMMARIZE(kingBRookUpYR),
    FV_SUMMARIZE(kingBRookUp),
    FV_SUMMARIZE(kingWRookUpR),
    FV_SUMMARIZE(kingWRookUpXR),
    FV_SUMMARIZE(kingWRookUpYR),
    FV_SUMMARIZE(kingWRookUp),
    FV_SUMMARIZE(kingBRookDownR),
    FV_SUMMARIZE(kingBRookDownXR),
    FV_SUMMARIZE(kingBRookDownYR),
    FV_SUMMARIZE(kingBRookDown),
    FV_SUMMARIZE(kingWRookDownR),
    FV_SUMMARIZE(kingWRookDownXR),
    FV_SUMMARIZE(kingWRookDownYR),
    FV_SUMMARIZE(kingWRookDown),
    FV_SUMMARIZE(kingBRookLeftR),
    FV_SUMMARIZE(kingBRookLeftXR),
    FV_SUMMARIZE(kingBRookLeftYR),
    FV_SUMMARIZE(kingBRookLeft),
    FV_SUMMARIZE(kingWRookLeftR),
    FV_SUMMARIZE(kingWRookLeftXR),
    FV_SUMMARIZE(kingWRookLeftYR),
    FV_SUMMARIZE(kingWRookLeft),
    FV_SUMMARIZE(kingBRookRightR),
    FV_SUMMARIZE(kingBRookRightXR),
    FV_SUMMARIZE(kingBRookRightYR),
    FV_SUMMARIZE(kingBRookRight),
    FV_SUMMARIZE(kingWRookRightR),
    FV_SUMMARIZE(kingWRookRightXR),
    FV_SUMMARIZE(kingWRookRightYR),
    FV_SUMMARIZE(kingWRookRight),
    FV_SUMMARIZE(kingBBishopLeftUp45R),
    FV_SUMMARIZE(kingBBishopLeftUp45XR),
    FV_SUMMARIZE(kingBBishopLeftUp45YR),
    FV_SUMMARIZE(kingBBishopLeftUp45),
    FV_SUMMARIZE(kingWBishopLeftUp45R),
    FV_SUMMARIZE(kingWBishopLeftUp45XR),
    FV_SUMMARIZE(kingWBishopLeftUp45YR),
    FV_SUMMARIZE(kingWBishopLeftUp45),
    FV_SUMMARIZE(kingBBishopRightDown45R),
    FV_SUMMARIZE(kingBBishopRightDown45XR),
    FV_SUMMARIZE(kingBBishopRightDown45YR),
    FV_SUMMARIZE(kingBBishopRightDown45),
    FV_SUMMARIZE(kingWBishopRightDown45R),
    FV_SUMMARIZE(kingWBishopRightDown45XR),
    FV_SUMMARIZE(kingWBishopRightDown45YR),
    FV_SUMMARIZE(kingWBishopRightDown45),
    FV_SUMMARIZE(kingBBishopRightUp45R),
    FV_SUMMARIZE(kingBBishopRightUp45XR),
    FV_SUMMARIZE(kingBBishopRightUp45YR),
    FV_SUMMARIZE(kingBBishopRightUp45),
    FV_SUMMARIZE(kingWBishopRightUp45R),
    FV_SUMMARIZE(kingWBishopRightUp45XR),
    FV_SUMMARIZE(kingWBishopRightUp45YR),
    FV_SUMMARIZE(kingWBishopRightUp45),
    FV_SUMMARIZE(kingBBishopLeftDown45R),
    FV_SUMMARIZE(kingBBishopLeftDown45XR),
    FV_SUMMARIZE(kingBBishopLeftDown45YR),
    FV_SUMMARIZE(kingBBishopLeftDown45),
    FV_SUMMARIZE(kingWBishopLeftDown45R),
    FV_SUMMARIZE(kingWBishopLeftDown45XR),
    FV_SUMMARIZE(kingWBishopLeftDown45YR),
    FV_SUMMARIZE(kingWBishopLeftDown45),
    FV_SUMMARIZE(kingBLanceR),
    FV_SUMMARIZE(kingBLanceXR),
    FV_SUMMARIZE(kingBLanceYR),
    FV_SUMMARIZE(kingBLance),
    FV_SUMMARIZE(kingWLanceR),
    FV_SUMMARIZE(kingWLanceXR),
    FV_SUMMARIZE(kingWLanceYR),
    FV_SUMMARIZE(kingWLance),
    FV_SUMMARIZE(kingAllyEffect9X),
    FV_SUMMARIZE(kingAllyEffect9Y),
    FV_SUMMARIZE(kingAllyEffect9),
    FV_SUMMARIZE(kingEnemyEffect9X),
    FV_SUMMARIZE(kingEnemyEffect9Y),
    FV_SUMMARIZE(kingEnemyEffect9),
    FV_SUMMARIZE(kingAllyEffect25X),
    FV_SUMMARIZE(kingAllyEffect25Y),
    FV_SUMMARIZE(kingAllyEffect25),
    FV_SUMMARIZE(kingEnemyEffect25X),
    FV_SUMMARIZE(kingEnemyEffect25Y),
    FV_SUMMARIZE(kingEnemyEffect25),
    FV_SUMMARIZE(kingEffect9DiffX),
    FV_SUMMARIZE(kingEffect9DiffY),
    FV_SUMMARIZE(kingEffect9Diff),
    FV_SUMMARIZE(kingEffect25DiffX),
    FV_SUMMARIZE(kingEffect25DiffY),
    FV_SUMMARIZE(kingEffect25Diff),
    summarizePart<SummaryType>("total",
                               reinterpret_cast<const typename FV::Type*>(&fv),
                               sizeof(fv) / sizeof(typename FV::Type)),
  };
#undef FV_PART_PTR
#undef FV_PART_SIZE
#undef FV_SUMMARIZE
}

} // namespace

#endif // SUNFISH_SEARCH_EVAL_FEATURETEMPLATES_HPP__
