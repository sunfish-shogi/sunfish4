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

  // piece, kingPieceR, kingPieceXR, kingPieceYR, kingPiece,
  //   => kingPiece
  SQUARE_EACH(king) {
    SQUARE_EACH(square) {
      for (int i = 0; i < EvalPieceIndex::End; i++) {
        ofv.kingPiece[king.raw()][square.raw()][i]
          = fv.kingPiece[king.raw()][square.raw()][i]
          + fv.piece[square.raw()][i]
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

  optimizeOpen(fv.rookVer,
               fv.kingRookVerR,
               fv.kingRookVerXR,
               fv.kingRookVerYR,
               fv.kingRookVer,
               ofv.kingRookVer);
  optimizeOpen(fv.rookHor,
               fv.kingRookHorR,
               fv.kingRookHorXR,
               fv.kingRookHorYR,
               fv.kingRookHor,
               ofv.kingRookHor);
  optimizeOpen(fv.bishopDiagL45,
               fv.kingBishopDiagL45R,
               fv.kingBishopDiagL45XR,
               fv.kingBishopDiagL45YR,
               fv.kingBishopDiagL45,
               ofv.kingBishopDiagL45);
  optimizeOpen(fv.bishopDiagR45,
               fv.kingBishopDiagR45R,
               fv.kingBishopDiagR45XR,
               fv.kingBishopDiagR45YR,
               fv.kingBishopDiagR45,
               ofv.kingBishopDiagR45);
  optimizeOpen(fv.lance,
               fv.kingLanceR,
               fv.kingLanceXR,
               fv.kingLanceYR,
               fv.kingLance,
               ofv.kingLance);
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
  //   => piece, kingPieceR, kingPieceXR, kingPieceYR, kingPiece,
  FV_PART_ZERO(fv, piece);
  FV_PART_ZERO(fv, kingPieceR);
  FV_PART_ZERO(fv, kingPieceXR);
  FV_PART_ZERO(fv, kingPieceYR);
  SQUARE_EACH(king) {
    SQUARE_EACH(square) {
      for (int i = 0; i < EvalPieceIndex::End; i++) {
        auto val = ofv.kingPiece[king.raw()][square.raw()][i];
        fv.piece[square.raw()][i] += val;
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

  expandOpen(fv.rookVer,
               fv.kingRookVerR,
               fv.kingRookVerXR,
               fv.kingRookVerYR,
               fv.kingRookVer,
               ofv.kingRookVer);
  expandOpen(fv.rookHor,
               fv.kingRookHorR,
               fv.kingRookHorXR,
               fv.kingRookHorYR,
               fv.kingRookHor,
               ofv.kingRookHor);
  expandOpen(fv.bishopDiagL45,
               fv.kingBishopDiagL45R,
               fv.kingBishopDiagL45XR,
               fv.kingBishopDiagL45YR,
               fv.kingBishopDiagL45,
               ofv.kingBishopDiagL45);
  expandOpen(fv.bishopDiagR45,
               fv.kingBishopDiagR45R,
               fv.kingBishopDiagR45XR,
               fv.kingBishopDiagR45YR,
               fv.kingBishopDiagR45,
               ofv.kingBishopDiagR45);
  expandOpen(fv.lance,
               fv.kingLanceR,
               fv.kingLanceXR,
               fv.kingLanceYR,
               fv.kingLance,
               ofv.kingLance);
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
  SQUARE_EACH(square) {
    auto rsquare = square.hsym();
    if (rsquare.raw() > square.raw()) {
      for (int i = 0; i < EvalPieceIndex::End; i++) {
        func(fv.piece[square.raw()][i],
             fv.piece[rsquare.raw()][i]);
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
  symmetrizeOpen(fv.rookVer,
                 fv.kingRookVerR,
                 fv.kingRookVerXR,
                 fv.kingRookVerYR,
                 fv.kingRookVer,
                 std::forward<T>(func));
  symmetrizeOpen(fv.rookHor,
                 fv.kingRookHorR,
                 fv.kingRookHorXR,
                 fv.kingRookHorYR,
                 fv.kingRookHor,
                 std::forward<T>(func));
  symmetrizeOpen(fv.bishopDiagL45,
                 fv.kingBishopDiagL45R,
                 fv.kingBishopDiagL45XR,
                 fv.kingBishopDiagL45YR,
                 fv.kingBishopDiagL45,
                 std::forward<T>(func));
  symmetrizeOpen(fv.bishopDiagR45,
                 fv.kingBishopDiagR45R,
                 fv.kingBishopDiagR45XR,
                 fv.kingBishopDiagR45YR,
                 fv.kingBishopDiagR45,
                 std::forward<T>(func));
  symmetrizeOpen(fv.lance,
                 fv.kingLanceR,
                 fv.kingLanceXR,
                 fv.kingLanceYR,
                 fv.kingLance,
                 std::forward<T>(func));
}

enum FeatureOperationType {
  Evaluate,
  Extract,
};

template <FeatureOperationType type, class OFV, class T>
inline
T operate(OFV& ofv, const Position& position, T delta) {
  T sum = 0;
  auto bking = position.getBlackKingSquare().raw();
  auto wking = position.getWhiteKingSquare().psym().raw();

  struct NeighborPiece {
    uint8_t n;
    uint8_t idx;
  };
  NeighborPiece bns[Neighbor3x3::NN];
  int bnn = 0;
  NeighborPiece wns[Neighbor3x3::NN];
  int wnn = 0;

  auto bnoking = position.getBOccupiedBitboard();
  auto wnoking = position.getWOccupiedBitboard();
  bnoking.unset(position.getBlackKingSquare());
  wnoking.unset(position.getWhiteKingSquare());

  auto noking = nosseOr(bnoking, wnoking);

  {
    auto bb = nosseAnd(bnoking,
                       MoveTables::king(position.getBlackKingSquare()));
    BB_EACH(square, bb) {
      int n = getNeighbor3x3(position.getBlackKingSquare(), square);
      auto piece = position.getPieceOnBoard(square);
      bns[bnn].n = n;
      bns[bnn].idx = getEvalPieceTypeIndex(piece.type());
      bnn++;
    }
  }

  {
    auto bb = nosseAnd(wnoking,
                       MoveTables::king(position.getWhiteKingSquare()));
    BB_EACH(square, bb) {
      int n = getNeighbor3x3R(position.getWhiteKingSquare(), square);
      auto piece = position.getPieceOnBoard(square);
      wns[wnn].n = n;
      wns[wnn].idx = getEvalPieceTypeIndex(piece.type());
      wnn++;
    }
  }

#define CALC_BLACK_HAND(t, T) do { \
  auto n = blackHand.get(PieceType::t()); \
  if (n != 0) { \
    if (type == FeatureOperationType::Evaluate) { \
      sum += ofv.kingHand[bking][EvalHandIndex::B ## T + n - 1]; \
      sum -= ofv.kingHand[wking][EvalHandIndex::W ## T + n - 1]; \
      for (int i = 0; i < bnn; i++) { \
        sum += ofv.kingNeighborHand[bking][bns[i].n][bns[i].idx][EvalHandIndex::B ## T + n - 1]; \
      } \
      for (int i = 0; i < wnn; i++) { \
        sum -= ofv.kingNeighborHand[wking][wns[i].n][wns[i].idx][EvalHandIndex::W ## T + n - 1]; \
      } \
      sum += ofv.kingKingHand[bking][wking][EvalHandTypeIndex:: T + n - 1]; \
    } else { \
      ofv.kingHand[bking][EvalHandIndex::B ## T + n - 1] += delta; \
      ofv.kingHand[wking][EvalHandIndex::W ## T + n - 1] -= delta; \
      for (int i = 0; i < bnn; i++) { \
        ofv.kingNeighborHand[bking][bns[i].n][bns[i].idx][EvalHandIndex::B ## T + n - 1] += delta; \
      } \
      for (int i = 0; i < wnn; i++) { \
        ofv.kingNeighborHand[wking][wns[i].n][wns[i].idx][EvalHandIndex::W ## T + n - 1] -= delta; \
      } \
      ofv.kingKingHand[bking][wking][EvalHandTypeIndex:: T + n - 1] += delta; \
    } \
  } \
} while (false)
  {
    auto& blackHand = position.getBlackHand();
    CALC_BLACK_HAND(pawn  , Pawn  );
    CALC_BLACK_HAND(lance , Lance );
    CALC_BLACK_HAND(knight, Knight);
    CALC_BLACK_HAND(silver, Silver);
    CALC_BLACK_HAND(gold  , Gold  );
    CALC_BLACK_HAND(bishop, Bishop);
    CALC_BLACK_HAND(rook  , Rook  );
  }
#undef CALC_BLACK_HAND

#define CALC_WHITE_HAND(t, T) do { \
  auto n = whiteHand.get(PieceType::t()); \
  if (n != 0) { \
    if (type == FeatureOperationType::Evaluate) { \
      sum += ofv.kingHand[bking][EvalHandIndex::W ## T + n - 1]; \
      sum -= ofv.kingHand[wking][EvalHandIndex::B ## T + n - 1]; \
      for (int i = 0; i < bnn; i++) { \
        sum += ofv.kingNeighborHand[bking][bns[i].n][bns[i].idx][EvalHandIndex::W ## T + n - 1]; \
      } \
      for (int i = 0; i < wnn; i++) { \
        sum -= ofv.kingNeighborHand[wking][wns[i].n][wns[i].idx][EvalHandIndex::B ## T + n - 1]; \
      } \
      sum -= ofv.kingKingHand[wking][bking][EvalHandTypeIndex:: T + n - 1]; \
    } else { \
      ofv.kingHand[bking][EvalHandIndex::W ## T + n - 1] += delta; \
      ofv.kingHand[wking][EvalHandIndex::B ## T + n - 1] -= delta; \
      for (int i = 0; i < bnn; i++) { \
        ofv.kingNeighborHand[bking][bns[i].n][bns[i].idx][EvalHandIndex::W ## T + n - 1] += delta; \
      } \
      for (int i = 0; i < wnn; i++) { \
        ofv.kingNeighborHand[wking][wns[i].n][wns[i].idx][EvalHandIndex::B ## T + n - 1] -= delta; \
      } \
      ofv.kingKingHand[wking][bking][EvalHandTypeIndex:: T + n - 1] -= delta; \
    } \
  } \
} while (false)
  {
    auto& whiteHand = position.getWhiteHand();
    CALC_WHITE_HAND(pawn  , Pawn  );
    CALC_WHITE_HAND(lance , Lance );
    CALC_WHITE_HAND(knight, Knight);
    CALC_WHITE_HAND(silver, Silver);
    CALC_WHITE_HAND(gold  , Gold  );
    CALC_WHITE_HAND(bishop, Bishop);
    CALC_WHITE_HAND(rook  , Rook  );
  }
#undef CALC_WHITE_HAND

  {
    auto bb = noking;
    BB_EACH(square, bb) {
      auto piece = position.getPieceOnBoard(square);
      int bs = square.raw();
      int ws = square.psym().raw();
      int bIndex = getEvalPieceIndex(piece);
      int wIndex = getEvalPieceIndex(piece.enemy());

      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingPiece[bking][bs][bIndex];
        sum -= ofv.kingPiece[wking][ws][wIndex];
        for (int i = 0; i < bnn; i++) {
          sum += ofv.kingNeighborPiece[bking][bns[i].n][bns[i].idx][bs][bIndex];
        }
        for (int i = 0; i < wnn; i++) {
          sum -= ofv.kingNeighborPiece[wking][wns[i].n][wns[i].idx][ws][wIndex];
        }
        if (piece.isBlack()) {
          sum += ofv.kingKingPiece[bking][wking][bs][getEvalPieceTypeIndex(piece.type())];
        } else {
          sum -= ofv.kingKingPiece[wking][bking][ws][getEvalPieceTypeIndex(piece.type())];
        }
      } else {
        ofv.kingPiece[bking][bs][bIndex] += delta;
        ofv.kingPiece[wking][ws][wIndex] -= delta;
        for (int i = 0; i < bnn; i++) {
          ofv.kingNeighborPiece[bking][bns[i].n][bns[i].idx][bs][bIndex] += delta;
        }
        for (int i = 0; i < wnn; i++) {
          ofv.kingNeighborPiece[wking][wns[i].n][wns[i].idx][ws][wIndex] -= delta;
        }
        if (piece.isBlack()) {
          ofv.kingKingPiece[bking][wking][bs][getEvalPieceTypeIndex(piece.type())] += delta;
        } else {
          ofv.kingKingPiece[wking][bking][ws][getEvalPieceTypeIndex(piece.type())] -= delta;
        }
      }
    }
  }

  auto occR45 = position.getRight45RotatedBitboard();
  auto occL45 = position.getLeft45RotatedBitboard();

  {
    auto bbishop = nosseAnd(nosseOr(position.getBBishopBitboard(),
                                    position.getBHorseBitboard()),
                            Bitboard::nocorner());
    BB_EACH(square, bbishop) {
      int bIndex = square.raw();

      int count = MoveTables::diagR45(occR45, square).count() - 1;
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingBishopDiagR45[bking][bIndex][count];
      } else {
        ofv.kingBishopDiagR45[bking][bIndex][count] += delta;
      }

      count = MoveTables::diagL45(occL45, square).count() - 1;
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingBishopDiagL45[bking][bIndex][count];
      } else {
        ofv.kingBishopDiagL45[bking][bIndex][count] += delta;
      }
    }
  }

  {
    auto wbishop = nosseAnd(nosseOr(position.getWBishopBitboard(),
                                    position.getWHorseBitboard()),
                            Bitboard::nocorner());
    BB_EACH(square, wbishop) {
      int wIndex = square.psym().raw();

      int count = MoveTables::diagR45(occR45, square).count() - 1;
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum -= ofv.kingBishopDiagR45[wking][wIndex][count];
      } else {
        ofv.kingBishopDiagR45[wking][wIndex][count] -= delta;
      }

      count = MoveTables::diagL45(occL45, square).count() - 1;
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum -= ofv.kingBishopDiagL45[wking][wIndex][count];
      } else {
        ofv.kingBishopDiagL45[wking][wIndex][count] -= delta;
      }
    }
  }

  auto occ = nosseOr(position.getBOccupiedBitboard(),
                     position.getWOccupiedBitboard());
  auto occ90 = position.get90RotatedBitboard();

  {
    auto brook = nosseOr(position.getBRookBitboard(),
                         position.getBDragonBitboard());
    BB_EACH(square, brook) {
      int bIndex = square.raw();

      int count = MoveTables::ver(occ, square).count() - 1;
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingRookVer[bking][bIndex][count];
      } else {
        ofv.kingRookVer[bking][bIndex][count] += delta;
      }

      count = MoveTables::hor(occ90, square).count() - 1;
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingRookHor[bking][bIndex][count];
      } else {
        ofv.kingRookHor[bking][bIndex][count] += delta;
      }
    }
  }

  {
    auto wrook = nosseOr(position.getWRookBitboard(),
                         position.getWDragonBitboard());
    BB_EACH(square, wrook) {
      int wIndex = square.psym().raw();

      int count = MoveTables::ver(occ, square).count() - 1;
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum -= ofv.kingRookVer[wking][wIndex][count];
      } else {
        ofv.kingRookVer[wking][wIndex][count] -= delta;
      }

      count = MoveTables::hor(occ90, square).count() - 1;
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum -= ofv.kingRookHor[wking][wIndex][count];
      } else {
        ofv.kingRookHor[wking][wIndex][count] -= delta;
      }
    }
  }

  {
    auto blance = position.getBLanceBitboard();
    BB_EACH(square, blance) {
      int bIndex = square.raw();

      int count = MoveTables::blackLance(occ, square).count() - 1;
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingLance[bking][bIndex][count];
      } else {
        ofv.kingLance[bking][bIndex][count] += delta;
      }
    }
  }

  {
    auto wlance = position.getWLanceBitboard();
    BB_EACH(square, wlance) {
      int wIndex = square.psym().raw();

      int count = MoveTables::whiteLance(occ, square).count() - 1;
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum -= ofv.kingLance[wking][wIndex][count];
      } else {
        ofv.kingLance[wking][wIndex][count] -= delta;
      }
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
    FV_SUMMARIZE(piece),
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
    FV_SUMMARIZE(rookVer),
    FV_SUMMARIZE(kingRookVerR),
    FV_SUMMARIZE(kingRookVerXR),
    FV_SUMMARIZE(kingRookVerYR),
    FV_SUMMARIZE(kingRookVer),
    FV_SUMMARIZE(rookHor),
    FV_SUMMARIZE(kingRookHorR),
    FV_SUMMARIZE(kingRookHorXR),
    FV_SUMMARIZE(kingRookHorYR),
    FV_SUMMARIZE(kingRookHor),
    FV_SUMMARIZE(bishopDiagL45),
    FV_SUMMARIZE(kingBishopDiagL45R),
    FV_SUMMARIZE(kingBishopDiagL45XR),
    FV_SUMMARIZE(kingBishopDiagL45YR),
    FV_SUMMARIZE(kingBishopDiagL45),
    FV_SUMMARIZE(bishopDiagR45),
    FV_SUMMARIZE(kingBishopDiagR45R),
    FV_SUMMARIZE(kingBishopDiagR45XR),
    FV_SUMMARIZE(kingBishopDiagR45YR),
    FV_SUMMARIZE(kingBishopDiagR45),
    FV_SUMMARIZE(lance),
    FV_SUMMARIZE(kingLanceR),
    FV_SUMMARIZE(kingLanceXR),
    FV_SUMMARIZE(kingLanceYR),
    FV_SUMMARIZE(kingLance),
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
