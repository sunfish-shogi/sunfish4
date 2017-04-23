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

#define FV_PART_ZERO(fv, part) memset( \
    reinterpret_cast<typename FV::Type*>(fv.part), \
    0, \
    sizeof(fv.part))

namespace {

using namespace sunfish;

template <class Open,
          class KingOpenR,
          class KingOpenXR,
          class KingOpenYR,
          class KingOpen,
          class OKingOpen>
inline
void optimizeOpen(Open open,
                  KingOpenR kingOpenR,
                  KingOpenXR kingOpenXR,
                  KingOpenYR kingOpenYR,
                  KingOpen kingOpen,
                  OKingOpen oKingOpen) {
  // open, kingOpenR, kingOpenXR, kingOpenYR, kingOpen
  //   => kingOpen
  SQUARE_EACH(king) {
    SQUARE_EACH(square) {
      for (int i = 0; i < 8; i++) {
        oKingOpen[king.raw()][square.raw()][i]
          = open[square.raw()][i]
          + kingOpenR[RelativeSquare(king, square).raw()][i]
          + kingOpenXR[king.getFile()-1][RelativeSquare(king, square).raw()][i]
          + kingOpenYR[king.getRank()-1][RelativeSquare(king, square).raw()][i]
          + kingOpen[king.raw()][square.raw()][i];
      }
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

  FV_PART_COPY(ofv, fv, kingNeighborHand);

  // kingNeighborPieceR, kingNeighborPieceXR, kingNeighborPieceYR, kingNeighborPiece
  //   => kingNeighborPiece
  SQUARE_EACH(king) {
    for (int n = 0; n < Neighbor3x3::NN; n++) {
      for (int i1 = 0; i1 < EvalPieceTypeIndex::End; i1++) {
        SQUARE_EACH(square) {
          for (int i2 = 0; i2 < EvalPieceIndex::End; i2++) {
            ofv.kingNeighborPiece[king.raw()][n][i1][square.raw()][i2]
              = fv.kingNeighborPiece[king.raw()][n][i1][square.raw()][i2]
              + fv.kingNeighborPieceR[n][i1][RelativeSquare(king, square).raw()][i2]
              + fv.kingNeighborPieceXR[king.getFile()-1][n][i1][RelativeSquare(king, square).raw()][i2]
              + fv.kingNeighborPieceYR[king.getRank()-1][n][i1][RelativeSquare(king, square).raw()][i2];
          }
        }
      }
    }
  }

  FV_PART_COPY(ofv, fv, kingKingHand);
  FV_PART_COPY(ofv, fv, kingKingPiece);

  optimizeOpen(fv.bRookVer,
               fv.kingBRookVerR,
               fv.kingBRookVerXR,
               fv.kingBRookVerYR,
               fv.kingBRookVer,
               ofv.kingBRookVer);
  optimizeOpen(fv.wRookVer,
               fv.kingWRookVerR,
               fv.kingWRookVerXR,
               fv.kingWRookVerYR,
               fv.kingWRookVer,
               ofv.kingWRookVer);
  optimizeOpen(fv.bRookHor,
               fv.kingBRookHorR,
               fv.kingBRookHorXR,
               fv.kingBRookHorYR,
               fv.kingBRookHor,
               ofv.kingBRookHor);
  optimizeOpen(fv.wRookHor,
               fv.kingWRookHorR,
               fv.kingWRookHorXR,
               fv.kingWRookHorYR,
               fv.kingWRookHor,
               ofv.kingWRookHor);
  optimizeOpen(fv.bBishopDiagL45,
               fv.kingBBishopDiagL45R,
               fv.kingBBishopDiagL45XR,
               fv.kingBBishopDiagL45YR,
               fv.kingBBishopDiagL45,
               ofv.kingBBishopDiagL45);
  optimizeOpen(fv.wBishopDiagL45,
               fv.kingWBishopDiagL45R,
               fv.kingWBishopDiagL45XR,
               fv.kingWBishopDiagL45YR,
               fv.kingWBishopDiagL45,
               ofv.kingWBishopDiagL45);
  optimizeOpen(fv.bBishopDiagR45,
               fv.kingBBishopDiagR45R,
               fv.kingBBishopDiagR45XR,
               fv.kingBBishopDiagR45YR,
               fv.kingBBishopDiagR45,
               ofv.kingBBishopDiagR45);
  optimizeOpen(fv.wBishopDiagR45,
               fv.kingWBishopDiagR45R,
               fv.kingWBishopDiagR45XR,
               fv.kingWBishopDiagR45YR,
               fv.kingWBishopDiagR45,
               ofv.kingWBishopDiagR45);
  optimizeOpen(fv.bLance,
               fv.kingBLanceR,
               fv.kingBLanceXR,
               fv.kingBLanceYR,
               fv.kingBLance,
               ofv.kingBLance);
  optimizeOpen(fv.wLance,
               fv.kingWLanceR,
               fv.kingWLanceXR,
               fv.kingWLanceYR,
               fv.kingWLance,
               ofv.kingWLance);

  FV_PART_COPY(ofv, fv, kingAllyEffect9);
  FV_PART_COPY(ofv, fv, kingEnemyEffect9);
  FV_PART_COPY(ofv, fv, kingAllyEffect25);
  FV_PART_COPY(ofv, fv, kingEnemyEffect25);
}

template <class Open,
          class KingOpenR,
          class KingOpenXR,
          class KingOpenYR,
          class KingOpen,
          class OKingOpen>
inline
void expandOpen(Open open,
                KingOpenR kingOpenR,
                KingOpenXR kingOpenXR,
                KingOpenYR kingOpenYR,
                KingOpen kingOpen,
                OKingOpen oKingOpen) {
  // kingOpen
  //   => open, kingOpenR, kingOpenXR, kingOpenYR, kingOpen
  memset(reinterpret_cast<char*>(open), 0, sizeof(Open));
  memset(reinterpret_cast<char*>(kingOpenXR), 0, sizeof(KingOpenXR));
  memset(reinterpret_cast<char*>(kingOpenYR), 0, sizeof(KingOpenYR));
  memset(reinterpret_cast<char*>(kingOpenR), 0, sizeof(KingOpenR));
  SQUARE_EACH(king) {
    SQUARE_EACH(square) {
      for (int i = 0; i < 8; i++) {
        auto val = oKingOpen[king.raw()][square.raw()][i];
        open[square.raw()][i] += val;
        kingOpenR[RelativeSquare(king, square).raw()][i] += val;
        kingOpenXR[king.getFile()-1][RelativeSquare(king, square).raw()][i] += val;
        kingOpenYR[king.getRank()-1][RelativeSquare(king, square).raw()][i] += val;
        kingOpen[king.raw()][square.raw()][i] = val;
      }
    }
  }
}

template <class FV, class OFV>
inline
void expand(FV& fv, OFV& ofv) {
  FV_PART_COPY(fv, ofv, kingHand);

  // kingPiece
  //   => kingPieceR, kingPieceXR, kingPieceYR, kingPiece,
  FV_PART_ZERO(fv, kingPieceR);
  FV_PART_ZERO(fv, kingPieceXR);
  FV_PART_ZERO(fv, kingPieceYR);
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

  FV_PART_COPY(fv, ofv, kingNeighborHand);

  // kingNeighborPiece
  //   => kingNeighborPieceR, kingNeighborPieceXR, kingNeighborPieceYR, kingNeighborPiece
  FV_PART_ZERO(fv, kingNeighborPieceR);
  FV_PART_ZERO(fv, kingNeighborPieceXR);
  FV_PART_ZERO(fv, kingNeighborPieceYR);
  SQUARE_EACH(king) {
    for (int n = 0; n < Neighbor3x3::NN; n++) {
      for (int i1 = 0; i1 < EvalPieceTypeIndex::End; i1++) {
        SQUARE_EACH(square) {
          for (int i2 = 0; i2 < EvalPieceIndex::End; i2++) {
            auto val = ofv.kingNeighborPiece[king.raw()][n][i1][square.raw()][i2];
            fv.kingNeighborPieceR[n][i1][RelativeSquare(king, square).raw()][i2] += val;
            fv.kingNeighborPieceXR[king.getFile()-1][n][i1][RelativeSquare(king, square).raw()][i2] += val;
            fv.kingNeighborPieceYR[king.getRank()-1][n][i1][RelativeSquare(king, square).raw()][i2] += val;
            fv.kingNeighborPiece[king.raw()][n][i1][square.raw()][i2] = val;
          }
        }
      }
    }
  }

  FV_PART_COPY(fv, ofv, kingKingHand);
  FV_PART_COPY(fv, ofv, kingKingPiece);

  expandOpen(fv.bRookVer,
               fv.kingBRookVerR,
               fv.kingBRookVerXR,
               fv.kingBRookVerYR,
               fv.kingBRookVer,
               ofv.kingBRookVer);
  expandOpen(fv.wRookVer,
               fv.kingWRookVerR,
               fv.kingWRookVerXR,
               fv.kingWRookVerYR,
               fv.kingWRookVer,
               ofv.kingWRookVer);
  expandOpen(fv.bRookHor,
               fv.kingBRookHorR,
               fv.kingBRookHorXR,
               fv.kingBRookHorYR,
               fv.kingBRookHor,
               ofv.kingBRookHor);
  expandOpen(fv.wRookHor,
               fv.kingWRookHorR,
               fv.kingWRookHorXR,
               fv.kingWRookHorYR,
               fv.kingWRookHor,
               ofv.kingWRookHor);
  expandOpen(fv.bBishopDiagL45,
               fv.kingBBishopDiagL45R,
               fv.kingBBishopDiagL45XR,
               fv.kingBBishopDiagL45YR,
               fv.kingBBishopDiagL45,
               ofv.kingBBishopDiagL45);
  expandOpen(fv.wBishopDiagL45,
               fv.kingWBishopDiagL45R,
               fv.kingWBishopDiagL45XR,
               fv.kingWBishopDiagL45YR,
               fv.kingWBishopDiagL45,
               ofv.kingWBishopDiagL45);
  expandOpen(fv.bBishopDiagR45,
               fv.kingBBishopDiagR45R,
               fv.kingBBishopDiagR45XR,
               fv.kingBBishopDiagR45YR,
               fv.kingBBishopDiagR45,
               ofv.kingBBishopDiagR45);
  expandOpen(fv.wBishopDiagR45,
               fv.kingWBishopDiagR45R,
               fv.kingWBishopDiagR45XR,
               fv.kingWBishopDiagR45YR,
               fv.kingWBishopDiagR45,
               ofv.kingWBishopDiagR45);
  expandOpen(fv.bLance,
               fv.kingBLanceR,
               fv.kingBLanceXR,
               fv.kingBLanceYR,
               fv.kingBLance,
               ofv.kingBLance);
  expandOpen(fv.wLance,
               fv.kingWLanceR,
               fv.kingWLanceXR,
               fv.kingWLanceYR,
               fv.kingWLance,
               ofv.kingWLance);

  FV_PART_COPY(fv, ofv, kingAllyEffect9);
  FV_PART_COPY(fv, ofv, kingEnemyEffect9);
  FV_PART_COPY(fv, ofv, kingAllyEffect25);
  FV_PART_COPY(fv, ofv, kingEnemyEffect25);
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

template <class Open,
          class KingOpenR,
          class KingOpenXR,
          class KingOpenYR,
          class KingOpen,
          class T>
inline
void symmetrizeOpen(Open open,
                    KingOpenR kingOpenR,
                    KingOpenXR kingOpenXR,
                    KingOpenYR kingOpenYR,
                    KingOpen kingOpen,
                    T&& func) {
  SQUARE_EACH(square) {
    auto rsquare = square.hsym();
    if (rsquare.raw() > square.raw()) {
      for (int i = 0; i < 8; i++) {
        func(open[square.raw()][i],
             open[rsquare.raw()][i]);
      }
    }
  }

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
    if (rking.raw() < king.raw()) {
      continue;
    }

    SQUARE_EACH(square) {
      auto rsquare = square.hsym();
      if (rking != king || rsquare.raw() > square.raw()) {
        for (int i = 0; i < EvalPieceIndex::End; i++) {
          func(fv.kingPiece[king.raw()][square.raw()][i],
               fv.kingPiece[rking.raw()][rsquare.raw()][i]);
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
      for (int i = 0; i < EvalPieceIndex::End; i++) {
        func(fv.kingPieceR[rs][i],
             fv.kingPieceR[rrs][i]);
      }
    }

    for (int f = 0; f < 9; f++) {
      int rf = 8 - f;
      if (rrs != rs || rf > f) {
        for (int i = 0; i < EvalPieceIndex::End; i++) {
          func(fv.kingPieceXR[f][rs][i],
               fv.kingPieceXR[rf][rrs][i]);
        }
      }
    }

    if (rrs > rs) {
      for (int r = 0; r < 9; r++) {
        for (int i = 0; i < EvalPieceIndex::End; i++) {
          func(fv.kingPieceYR[r][rs][i],
               fv.kingPieceYR[r][rrs][i]);
        }
      }
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

      SQUARE_EACH(king) {
        auto rking = king.hsym();
        if (rsquare != square || rn != n || rking.raw() > king.raw()) {
          for (int i1 = 0; i1 < EvalPieceTypeIndex::End; i1++) {
            for (int i2 = 0; i2 < EvalPieceIndex::End; i2++) {
              func(fv.kingNeighborPiece[king.raw()][n][i1][square.raw()][i2],
                   fv.kingNeighborPiece[rking.raw()][rn][i1][rsquare.raw()][i2]);
            }
          }
        }
      }
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

      if (rn != n || rrs > rs) {
        for (int i1 = 0; i1 < EvalPieceTypeIndex::End; i1++) {
          for (int i2 = 0; i2 < EvalPieceIndex::End; i2++) {
            func(fv.kingNeighborPieceR[n][i1][rs][i2],
                 fv.kingNeighborPieceR[rn][i1][rrs][i2]);
          }
        }
      }


      for (int f = 0; f < 9; f++) {
        int rf = 8 - f;
        if (rn != n || rrs != rs || rf > f) {
          for (int i1 = 0; i1 < EvalPieceTypeIndex::End; i1++) {
            for (int i2 = 0; i2 < EvalPieceIndex::End; i2++) {
              func(fv.kingNeighborPieceXR[f][n][i1][rs][i2],
                   fv.kingNeighborPieceXR[rf][rn][i1][rrs][i2]);
            }
          }
        }
      }

      if (rn != n || rrs > rs) {
        for (int r = 0; r < 9; r++) {
          for (int i1 = 0; i1 < EvalPieceTypeIndex::End; i1++) {
            for (int i2 = 0; i2 < EvalPieceIndex::End; i2++) {
              func(fv.kingNeighborPieceYR[r][n][i1][rs][i2],
                   fv.kingNeighborPieceYR[r][rn][i1][rrs][i2]);
            }
          }
        }
      }
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

        for (int i = 0; i < EvalPieceTypeIndex::End; i++) {
          func(fv.kingKingPiece[king1.raw()][king2.raw()][square.raw()][i],
               fv.kingKingPiece[rking1.raw()][rking2.raw()][rsquare.raw()][i]);
        }
      }
    }
  }

  // open
  symmetrizeOpen(fv.bRookVer,
                 fv.kingBRookVerR,
                 fv.kingBRookVerXR,
                 fv.kingBRookVerYR,
                 fv.kingBRookVer,
                 std::forward<T>(func));
  symmetrizeOpen(fv.wRookVer,
                 fv.kingWRookVerR,
                 fv.kingWRookVerXR,
                 fv.kingWRookVerYR,
                 fv.kingWRookVer,
                 std::forward<T>(func));
  symmetrizeOpen(fv.bRookHor,
                 fv.kingBRookHorR,
                 fv.kingBRookHorXR,
                 fv.kingBRookHorYR,
                 fv.kingBRookHor,
                 std::forward<T>(func));
  symmetrizeOpen(fv.wRookHor,
                 fv.kingWRookHorR,
                 fv.kingWRookHorXR,
                 fv.kingWRookHorYR,
                 fv.kingWRookHor,
                 std::forward<T>(func));
  symmetrizeOpen(fv.bBishopDiagL45,
                 fv.kingBBishopDiagL45R,
                 fv.kingBBishopDiagL45XR,
                 fv.kingBBishopDiagL45YR,
                 fv.kingBBishopDiagL45,
                 std::forward<T>(func));
  symmetrizeOpen(fv.wBishopDiagL45,
                 fv.kingWBishopDiagL45R,
                 fv.kingWBishopDiagL45XR,
                 fv.kingWBishopDiagL45YR,
                 fv.kingWBishopDiagL45,
                 std::forward<T>(func));
  symmetrizeOpen(fv.bBishopDiagR45,
                 fv.kingBBishopDiagR45R,
                 fv.kingBBishopDiagR45XR,
                 fv.kingBBishopDiagR45YR,
                 fv.kingBBishopDiagR45,
                 std::forward<T>(func));
  symmetrizeOpen(fv.wBishopDiagR45,
                 fv.kingWBishopDiagR45R,
                 fv.kingWBishopDiagR45XR,
                 fv.kingWBishopDiagR45YR,
                 fv.kingWBishopDiagR45,
                 std::forward<T>(func));
  symmetrizeOpen(fv.bLance,
                 fv.kingBLanceR,
                 fv.kingBLanceXR,
                 fv.kingBLanceYR,
                 fv.kingBLance,
                 std::forward<T>(func));
  symmetrizeOpen(fv.wLance,
                 fv.kingWLanceR,
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
    for (int i = 0; i < m.bnn; i++) {
      sum += ofv.kingNeighborPiece[m.bking][m.bns[i].n][m.bns[i].idx][bs][bIndex];
    }
    for (int i = 0; i < m.wnn; i++) {
      sum -= ofv.kingNeighborPiece[m.wking][m.wns[i].n][m.wns[i].idx][ws][wIndex];
    }
    if (turn == Turn::Black) {
      sum += ofv.kingKingPiece[m.bking][m.wking][bs][typeIndex];
    } else {
      sum -= ofv.kingKingPiece[m.wking][m.bking][ws][typeIndex];
    }
  } else {
    ofv.kingPiece[m.bking][bs][bIndex] += delta;
    ofv.kingPiece[m.wking][ws][wIndex] -= delta;
    for (int i = 0; i < m.bnn; i++) {
      ofv.kingNeighborPiece[m.bking][m.bns[i].n][m.bns[i].idx][bs][bIndex] += delta;
    }
    for (int i = 0; i < m.wnn; i++) {
      ofv.kingNeighborPiece[m.wking][m.wns[i].n][m.wns[i].idx][ws][wIndex] -= delta;
    }
    if (turn == Turn::Black) {
      ofv.kingKingPiece[m.bking][m.wking][bs][typeIndex] += delta;
    } else {
      ofv.kingKingPiece[m.wking][m.bking][ws][typeIndex] -= delta;
    }
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
    bef |= bpawn >> 1;
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
    wef |= wpawn << 1;
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
    bef |= (bsilver >> 10) & Bitboard::rank1to8();
    bef |= (bsilver >> 8) & Bitboard::rank2to9();
    bef |= (bsilver >> 1) & Bitboard::rank1to8();
    bef |= (bsilver << 8) & Bitboard::rank1to8();
    bef |= (bsilver << 10) & Bitboard::rank2to9();
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
    wef |= (wsilver >> 10) & Bitboard::rank1to8();
    wef |= (wsilver >> 8) & Bitboard::rank2to9();
    wef |= (wsilver << 1) & Bitboard::rank2to9();
    wef |= (wsilver << 8) & Bitboard::rank1to8();
    wef |= (wsilver << 10) & Bitboard::rank2to9();
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
    bef |= (bgold >> 10) & Bitboard::rank1to8();
    bef |= bgold >> 9;
    bef |= (bgold >> 1) & Bitboard::rank1to8();
    bef |= (bgold << 1) & Bitboard::rank2to9();
    bef |= (bgold << 8) & Bitboard::rank1to8();
    bef |= bgold << 9;
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
    wef |= wgold >> 9;
    wef |= (wgold >> 8) & Bitboard::rank2to9();
    wef |= (wgold >> 1) & Bitboard::rank1to8();
    wef |= (wgold << 1) & Bitboard::rank2to9();
    wef |= wgold << 9;
    wef |= (wgold << 10) & Bitboard::rank2to9();
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

      auto eff = MoveTables::diagR45(occR45, square);
      bef |= eff;

      int count = eff.count() - 1;
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingBBishopDiagR45[m.bking][bs][count];
        sum -= ofv.kingWBishopDiagR45[m.wking][ws][count];
      } else {
        ofv.kingBBishopDiagR45[m.bking][bs][count] += delta;
        ofv.kingWBishopDiagR45[m.wking][ws][count] -= delta;
      }

      eff = MoveTables::diagL45(occL45, square);
      bef |= eff;

      count = eff.count() - 1;
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingBBishopDiagL45[m.bking][bs][count];
        sum -= ofv.kingWBishopDiagL45[m.wking][ws][count];
      } else {
        ofv.kingBBishopDiagL45[m.bking][bs][count] += delta;
        ofv.kingWBishopDiagL45[m.wking][ws][count] -= delta;
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

      auto eff = MoveTables::diagR45(occR45, square);
      wef |= eff;

      int count = eff.count() - 1;
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingWBishopDiagR45[m.bking][bs][count];
        sum -= ofv.kingBBishopDiagR45[m.wking][ws][count];
      } else {
        ofv.kingWBishopDiagR45[m.bking][bs][count] += delta;
        ofv.kingBBishopDiagR45[m.wking][ws][count] -= delta;
      }

      eff = MoveTables::diagL45(occL45, square);
      wef |= eff;

      count = eff.count() - 1;
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingWBishopDiagL45[m.bking][bs][count];
        sum -= ofv.kingBBishopDiagL45[m.wking][ws][count];
      } else {
        ofv.kingWBishopDiagL45[m.bking][bs][count] += delta;
        ofv.kingBBishopDiagL45[m.wking][ws][count] -= delta;
      }
    }
  }

  // horse
  {
    auto bhorse = position.getBHorseBitboard();
    bef |= bhorse >> 9;
    bef |= (bhorse >> 1) & Bitboard::rank1to8();
    bef |= (bhorse << 1) & Bitboard::rank2to9();
    bef |= bhorse << 9;
    BB_EACH(square, bhorse) {
      int bs = square.raw();
      int ws = square.psym().raw();
      sum += operatePiece<type, OFV, T, Turn::Black>(ofv, delta, m,
                                                     EvalPieceTypeIndex::Horse,
                                                     EvalPieceIndex::BHorse,
                                                     EvalPieceIndex::WHorse,
                                                     bs, ws);

      auto eff = MoveTables::diagR45(occR45, square);
      bef |= eff;

      int count = eff.count() - 1;
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingBBishopDiagR45[m.bking][bs][count];
        sum -= ofv.kingWBishopDiagR45[m.wking][ws][count];
      } else {
        ofv.kingBBishopDiagR45[m.bking][bs][count] += delta;
        ofv.kingWBishopDiagR45[m.wking][ws][count] -= delta;
      }

      eff = MoveTables::diagL45(occL45, square);
      bef |= eff;

      count = eff.count() - 1;
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingBBishopDiagL45[m.bking][bs][count];
        sum -= ofv.kingWBishopDiagL45[m.wking][ws][count];
      } else {
        ofv.kingBBishopDiagL45[m.bking][bs][count] += delta;
        ofv.kingWBishopDiagL45[m.wking][ws][count] -= delta;
      }
    }
  }

  {
    auto whorse = position.getWHorseBitboard();
    wef |= whorse >> 9;
    wef |= (whorse >> 1) & Bitboard::rank1to8();
    wef |= (whorse << 1) & Bitboard::rank2to9();
    wef |= whorse << 9;
    BB_EACH(square, whorse) {
      int bs = square.raw();
      int ws = square.psym().raw();
      sum += operatePiece<type, OFV, T, Turn::White>(ofv, delta, m,
                                                     EvalPieceTypeIndex::Horse,
                                                     EvalPieceIndex::WHorse,
                                                     EvalPieceIndex::BHorse,
                                                     bs, ws);

      auto eff = MoveTables::diagR45(occR45, square);
      wef |= eff;

      int count = eff.count() - 1;
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingWBishopDiagR45[m.bking][bs][count];
        sum -= ofv.kingBBishopDiagR45[m.wking][ws][count];
      } else {
        ofv.kingWBishopDiagR45[m.bking][bs][count] += delta;
        ofv.kingBBishopDiagR45[m.wking][ws][count] -= delta;
      }

      eff = MoveTables::diagL45(occL45, square);
      wef |= eff;

      count = eff.count() - 1;
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingWBishopDiagL45[m.bking][bs][count];
        sum -= ofv.kingBBishopDiagL45[m.wking][ws][count];
      } else {
        ofv.kingWBishopDiagL45[m.bking][bs][count] += delta;
        ofv.kingBBishopDiagL45[m.wking][ws][count] -= delta;
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

      auto eff = MoveTables::ver(occ, square);
      bef |= eff;

      int count = eff.count() - 1;
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingBRookVer[m.bking][bs][count];
        sum -= ofv.kingWRookVer[m.wking][ws][count];
      } else {
        ofv.kingBRookVer[m.bking][bs][count] += delta;
        ofv.kingWRookVer[m.wking][ws][count] -= delta;
      }

      eff = MoveTables::hor(occ90, square);
      bef |= eff;

      count = eff.count() - 1;
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingBRookHor[m.bking][bs][count];
        sum -= ofv.kingWRookHor[m.wking][ws][count];
      } else {
        ofv.kingBRookHor[m.bking][bs][count] += delta;
        ofv.kingWRookHor[m.wking][ws][count] -= delta;
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

      auto eff = MoveTables::ver(occ, square);
      wef |= eff;

      int count = eff.count() - 1;
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingWRookVer[m.bking][bs][count];
        sum -= ofv.kingBRookVer[m.wking][ws][count];
      } else {
        ofv.kingWRookVer[m.bking][bs][count] += delta;
        ofv.kingBRookVer[m.wking][ws][count] -= delta;
      }

      eff = MoveTables::hor(occ90, square);
      wef |= eff;

      count = eff.count() - 1;
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingWRookHor[m.bking][bs][count];
        sum -= ofv.kingBRookHor[m.wking][ws][count];
      } else {
        ofv.kingWRookHor[m.bking][bs][count] += delta;
        ofv.kingBRookHor[m.wking][ws][count] -= delta;
      }
    }
  }

  // dragon
  {
    auto bdragon = position.getBDragonBitboard();
    bef |= (bdragon >> 10) & Bitboard::rank1to8();
    bef |= (bdragon >> 8) & Bitboard::rank2to9();
    bef |= (bdragon << 8) & Bitboard::rank1to8();
    bef |= (bdragon << 10) & Bitboard::rank2to9();
    BB_EACH(square, bdragon) {
      int bs = square.raw();
      int ws = square.psym().raw();
      sum += operatePiece<type, OFV, T, Turn::Black>(ofv, delta, m,
                                                     EvalPieceTypeIndex::Dragon,
                                                     EvalPieceIndex::BDragon,
                                                     EvalPieceIndex::WDragon,
                                                     bs, ws);

      auto eff = MoveTables::ver(occ, square);
      bef |= eff;

      int count = eff.count() - 1;
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingBRookVer[m.bking][bs][count];
        sum -= ofv.kingWRookVer[m.wking][ws][count];
      } else {
        ofv.kingBRookVer[m.bking][bs][count] += delta;
        ofv.kingWRookVer[m.wking][ws][count] -= delta;
      }

      eff = MoveTables::hor(occ90, square);
      bef |= eff;

      count = eff.count() - 1;
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingBRookHor[m.bking][bs][count];
        sum -= ofv.kingWRookHor[m.wking][ws][count];
      } else {
        ofv.kingBRookHor[m.bking][bs][count] += delta;
        ofv.kingWRookHor[m.wking][ws][count] -= delta;
      }
    }
  }

  {
    auto wdragon = position.getWDragonBitboard();
    wef |= (wdragon >> 10) & Bitboard::rank1to8();
    wef |= (wdragon >> 8) & Bitboard::rank2to9();
    wef |= (wdragon << 8) & Bitboard::rank1to8();
    wef |= (wdragon << 10) & Bitboard::rank2to9();
    BB_EACH(square, wdragon) {
      int bs = square.raw();
      int ws = square.psym().raw();
      sum += operatePiece<type, OFV, T, Turn::White>(ofv, delta, m,
                                                     EvalPieceTypeIndex::Dragon,
                                                     EvalPieceIndex::WDragon,
                                                     EvalPieceIndex::BDragon,
                                                     bs, ws);

      auto eff = MoveTables::ver(occ, square);
      wef |= eff;

      int count = eff.count() - 1;
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingWRookVer[m.bking][bs][count];
        sum -= ofv.kingBRookVer[m.wking][ws][count];
      } else {
        ofv.kingWRookVer[m.bking][bs][count] += delta;
        ofv.kingBRookVer[m.wking][ws][count] -= delta;
      }

      eff = MoveTables::hor(occ90, square);
      wef |= eff;

      count = eff.count() - 1;
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingWRookHor[m.bking][bs][count];
        sum -= ofv.kingBRookHor[m.wking][ws][count];
      } else {
        ofv.kingWRookHor[m.bking][bs][count] += delta;
        ofv.kingBRookHor[m.wking][ws][count] -= delta;
      }
    }
  }

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
    bef |= bknight >> 11;
    bef |= bknight << 7;
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
    wef |= wknight >> 7;
    wef |= wknight << 11;
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

  {
    auto& mask = MoveTables::neighbor3x3(position.getBlackKingSquare());

    auto bc = (bef & mask).count();
    auto wc = (wef & mask).count();
    if (type == FeatureOperationType::Evaluate) {
      sum += ofv.kingAllyEffect9[m.bking][bc];
      sum += ofv.kingEnemyEffect9[m.bking][wc];
    } else {
      ofv.kingAllyEffect9[m.bking][bc] += delta;
      ofv.kingEnemyEffect9[m.bking][wc] += delta;
    }
  }

  {
    auto& mask = MoveTables::neighbor5x5(position.getBlackKingSquare());

    auto bc = (bef & mask).count();
    auto wc = (wef & mask).count();
    if (type == FeatureOperationType::Evaluate) {
      sum += ofv.kingAllyEffect25[m.bking][bc];
      sum += ofv.kingEnemyEffect25[m.bking][wc];
    } else {
      ofv.kingAllyEffect25[m.bking][bc] += delta;
      ofv.kingEnemyEffect25[m.bking][wc] += delta;
    }
  }

  {
    auto& mask = MoveTables::neighbor3x3(position.getWhiteKingSquare());

    auto bc = (bef & mask).count();
    auto wc = (wef & mask).count();
    if (type == FeatureOperationType::Evaluate) {
      sum -= ofv.kingAllyEffect9[m.wking][wc];
      sum -= ofv.kingEnemyEffect9[m.wking][bc];
    } else {
      ofv.kingAllyEffect9[m.wking][wc] -= delta;
      ofv.kingEnemyEffect9[m.wking][bc] -= delta;
    }
  }

  {
    auto& mask = MoveTables::neighbor5x5(position.getWhiteKingSquare());

    auto bc = (bef & mask).count();
    auto wc = (wef & mask).count();
    if (type == FeatureOperationType::Evaluate) {
      sum -= ofv.kingAllyEffect25[m.wking][wc];
      sum -= ofv.kingEnemyEffect25[m.wking][bc];
    } else {
      ofv.kingAllyEffect25[m.wking][wc] -= delta;
      ofv.kingEnemyEffect25[m.wking][bc] -= delta;
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
    FV_SUMMARIZE(kingNeighborHand),
    FV_SUMMARIZE(kingNeighborPieceR),
    FV_SUMMARIZE(kingNeighborPieceXR),
    FV_SUMMARIZE(kingNeighborPieceYR),
    FV_SUMMARIZE(kingNeighborPiece),
    FV_SUMMARIZE(kingKingHand),
    FV_SUMMARIZE(kingKingPiece),
    FV_SUMMARIZE(bRookVer),
    FV_SUMMARIZE(kingBRookVerR),
    FV_SUMMARIZE(kingBRookVerXR),
    FV_SUMMARIZE(kingBRookVerYR),
    FV_SUMMARIZE(kingBRookVer),
    FV_SUMMARIZE(wRookVer),
    FV_SUMMARIZE(kingWRookVerR),
    FV_SUMMARIZE(kingWRookVerXR),
    FV_SUMMARIZE(kingWRookVerYR),
    FV_SUMMARIZE(kingWRookVer),
    FV_SUMMARIZE(bRookHor),
    FV_SUMMARIZE(kingBRookHorR),
    FV_SUMMARIZE(kingBRookHorXR),
    FV_SUMMARIZE(kingBRookHorYR),
    FV_SUMMARIZE(kingBRookHor),
    FV_SUMMARIZE(wRookHor),
    FV_SUMMARIZE(kingWRookHorR),
    FV_SUMMARIZE(kingWRookHorXR),
    FV_SUMMARIZE(kingWRookHorYR),
    FV_SUMMARIZE(kingWRookHor),
    FV_SUMMARIZE(bBishopDiagL45),
    FV_SUMMARIZE(kingBBishopDiagL45R),
    FV_SUMMARIZE(kingBBishopDiagL45XR),
    FV_SUMMARIZE(kingBBishopDiagL45YR),
    FV_SUMMARIZE(kingBBishopDiagL45),
    FV_SUMMARIZE(wBishopDiagL45),
    FV_SUMMARIZE(kingWBishopDiagL45R),
    FV_SUMMARIZE(kingWBishopDiagL45XR),
    FV_SUMMARIZE(kingWBishopDiagL45YR),
    FV_SUMMARIZE(kingWBishopDiagL45),
    FV_SUMMARIZE(bBishopDiagR45),
    FV_SUMMARIZE(kingBBishopDiagR45R),
    FV_SUMMARIZE(kingBBishopDiagR45XR),
    FV_SUMMARIZE(kingBBishopDiagR45YR),
    FV_SUMMARIZE(kingBBishopDiagR45),
    FV_SUMMARIZE(wBishopDiagR45),
    FV_SUMMARIZE(kingWBishopDiagR45R),
    FV_SUMMARIZE(kingWBishopDiagR45XR),
    FV_SUMMARIZE(kingWBishopDiagR45YR),
    FV_SUMMARIZE(kingWBishopDiagR45),
    FV_SUMMARIZE(bLance),
    FV_SUMMARIZE(kingBLanceR),
    FV_SUMMARIZE(kingBLanceXR),
    FV_SUMMARIZE(kingBLanceYR),
    FV_SUMMARIZE(kingBLance),
    FV_SUMMARIZE(wLance),
    FV_SUMMARIZE(kingWLanceR),
    FV_SUMMARIZE(kingWLanceXR),
    FV_SUMMARIZE(kingWLanceYR),
    FV_SUMMARIZE(kingWLance),
    FV_SUMMARIZE(kingAllyEffect9),
    FV_SUMMARIZE(kingEnemyEffect9),
    FV_SUMMARIZE(kingAllyEffect25),
    FV_SUMMARIZE(kingEnemyEffect25),
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
