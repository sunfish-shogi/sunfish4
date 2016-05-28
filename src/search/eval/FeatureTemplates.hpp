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

template <class FV, class OFV>
inline
void optimize(FV& fv, OFV& ofv) {
  FV_PART_COPY(ofv, fv, kingNeighborHand);

  // kingNeighborPieceR, kingNeighborPieceXR, kingNeighborPieceYR, kingNeighborPiece
  //   => kingNeighborPiece
  SQUARE_EACH(king) {
    DIR_EACH_S(dir) {
      int d = static_cast<int>(dir);
      for (int i1 = 0; i1 < EvalPieceIndex::End; i1++) {
        SQUARE_EACH(square) {
          for (int i2 = 0; i2 < EvalPieceIndex::End; i2++) {
            ofv.kingNeighborPiece[king.raw()][d][i1][square.raw()][i2]
              = fv.kingNeighborPiece[king.raw()][d][i1][square.raw()][i2]
              + fv.kingNeighborPieceR[d][i1][RelativeSquare(king, square).raw()][i2]
              + fv.kingNeighborPieceXR[king.getFile()-1][d][i1][RelativeSquare(king, square).raw()][i2]
              + fv.kingNeighborPieceYR[king.getRank()-1][d][i1][RelativeSquare(king, square).raw()][i2];
          }
        }
      }
    }
  }

  FV_PART_COPY(ofv, fv, kingBRookVer);
  FV_PART_COPY(ofv, fv, kingWRookVer);
  FV_PART_COPY(ofv, fv, kingBRookHor);
  FV_PART_COPY(ofv, fv, kingWRookHor);
  FV_PART_COPY(ofv, fv, kingBBishopDiagL45);
  FV_PART_COPY(ofv, fv, kingWBishopDiagL45);
  FV_PART_COPY(ofv, fv, kingBBishopDiagR45);
  FV_PART_COPY(ofv, fv, kingWBishopDiagR45);
  FV_PART_COPY(ofv, fv, kingBLance);
  FV_PART_COPY(ofv, fv, kingWLance);

  // kingHand, kingSafetyHand
  //   => kingSafetyHand
  SQUARE_EACH(king) {
    for (int s = 0; s < KingSafetyLen; s++) {
      for (int h = 0; h < EvalHandIndex::End; h++) {
        ofv.kingSafetyHand[king.raw()][s][h]
          = fv.kingHand[king.raw()][h]
          + fv.kingSafetyHand[king.raw()][s][h];
      }
    }
  }

  // kingPieceR, kingPieceXR, kingPieceYR, kingPiece,
  // kingSafetyPieceR, kingSafetyPieceXR, kingSafetyPieceYR, kingSafetyPiece
  //   => kingSafetyPiece
  SQUARE_EACH(king) {
    for (int s = 0; s < KingSafetyLen; s++) {
      SQUARE_EACH(square) {
        for (int i = 0; i < EvalPieceIndex::End; i++) {
          ofv.kingSafetyPiece[king.raw()][s][square.raw()][i]
            = fv.kingPiece[king.raw()][square.raw()][i]
            + fv.kingPieceR[RelativeSquare(king, square).raw()][i]
            + fv.kingPieceXR[king.getFile()-1][RelativeSquare(king, square).raw()][i]
            + fv.kingPieceYR[king.getRank()-1][RelativeSquare(king, square).raw()][i]
            + fv.kingSafetyPiece[king.raw()][s][square.raw()][i]
            + fv.kingSafetyPieceR[s][RelativeSquare(king, square).raw()][i]
            + fv.kingSafetyPieceXR[king.getFile()-1][s][RelativeSquare(king, square).raw()][i]
            + fv.kingSafetyPieceYR[king.getRank()-1][s][RelativeSquare(king, square).raw()][i];
        }
      }
    }
  }
}

template <class FV, class OFV>
inline
void expand(FV& fv, OFV& ofv) {
  FV_PART_COPY(fv, ofv, kingNeighborHand);

  // kingNeighborPiece
  //   => kingNeighborPieceR, kingNeighborPieceXR, kingNeighborPieceYR, kingNeighborPiece
  FV_PART_ZERO(fv, kingNeighborPieceR);
  FV_PART_ZERO(fv, kingNeighborPieceXR);
  FV_PART_ZERO(fv, kingNeighborPieceYR);
  SQUARE_EACH(king) {
    DIR_EACH_S(dir) {
      int d = static_cast<int>(dir);
      for (int i1 = 0; i1 < EvalPieceIndex::End; i1++) {
        SQUARE_EACH(square) {
          for (int i2 = 0; i2 < EvalPieceIndex::End; i2++) {
            auto val = ofv.kingNeighborPiece[king.raw()][d][i1][square.raw()][i2];
            fv.kingNeighborPieceR[d][i1][RelativeSquare(king, square).raw()][i2] += val;
            fv.kingNeighborPieceXR[king.getFile()-1][d][i1][RelativeSquare(king, square).raw()][i2] += val;
            fv.kingNeighborPieceYR[king.getRank()-1][d][i1][RelativeSquare(king, square).raw()][i2] += val;
            fv.kingNeighborPiece[king.raw()][d][i1][square.raw()][i2] = val;
          }
        }
      }
    }
  }

  FV_PART_COPY(fv, ofv, kingBRookVer);
  FV_PART_COPY(fv, ofv, kingWRookVer);
  FV_PART_COPY(fv, ofv, kingBRookHor);
  FV_PART_COPY(fv, ofv, kingWRookHor);
  FV_PART_COPY(fv, ofv, kingBBishopDiagL45);
  FV_PART_COPY(fv, ofv, kingWBishopDiagL45);
  FV_PART_COPY(fv, ofv, kingBBishopDiagR45);
  FV_PART_COPY(fv, ofv, kingWBishopDiagR45);
  FV_PART_COPY(fv, ofv, kingBLance);
  FV_PART_COPY(fv, ofv, kingWLance);

  // kingSafetyHand
  //   => kingHand, kingSafetyHand
  FV_PART_ZERO(fv, kingHand);
  SQUARE_EACH(king) {
    for (int s = 0; s < KingSafetyLen; s++) {
      for (int h = 0; h < EvalHandIndex::End; h++) {
        auto val = ofv.kingSafetyHand[king.raw()][s][h];
        fv.kingHand[king.raw()][h] += val;
        fv.kingSafetyHand[king.raw()][s][h] = val;
      }
    }
  }

  // kingSafetyPiece
  //   => kingPieceR, kingPieceXR, kingPieceYR, kingPiece,
  //      kingSafetyPieceR, kingSafetyPieceXR, kingSafetyPieceYR, kingSafetyPiece
  FV_PART_ZERO(fv, kingPieceR);
  FV_PART_ZERO(fv, kingPieceXR);
  FV_PART_ZERO(fv, kingPieceYR);
  FV_PART_ZERO(fv, kingPiece);
  FV_PART_ZERO(fv, kingSafetyPieceR);
  FV_PART_ZERO(fv, kingSafetyPieceXR);
  FV_PART_ZERO(fv, kingSafetyPieceYR);
  SQUARE_EACH(king) {
    for (int s = 0; s < KingSafetyLen; s++) {
      SQUARE_EACH(square) {
        for (int i = 0; i < EvalPieceIndex::End; i++) {
          auto val = ofv.kingSafetyPiece[king.raw()][s][square.raw()][i];
          fv.kingPieceR[RelativeSquare(king, square).raw()][i] += val;
          fv.kingPieceXR[king.getFile()-1][RelativeSquare(king, square).raw()][i] += val;
          fv.kingPieceYR[king.getRank()-1][RelativeSquare(king, square).raw()][i] += val;
          fv.kingPiece[king.raw()][square.raw()][i] += val;
          fv.kingSafetyPieceR[s][RelativeSquare(king, square).raw()][i] += val;
          fv.kingSafetyPieceXR[king.getFile()-1][s][RelativeSquare(king, square).raw()][i] += val;
          fv.kingSafetyPieceYR[king.getRank()-1][s][RelativeSquare(king, square).raw()][i] += val;
          fv.kingSafetyPiece[king.raw()][s][square.raw()][i] = val;
        }
      }
    }
  }
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

template <class FV, class T>
inline
void symmetrize(FV& fv, T&& func) {
  SQUARE_EACH(king) {
    auto rking = king.hsym();
    if (rking.raw() < king.raw()) {
      continue;
    }

    if (rking != king) {
      for (int h = 0; h < EvalHandIndex::End; h++) {
        func(fv.kingHand[king.raw()][h],
             fv.kingHand[rking.raw()][h]);
      }
    }

    SQUARE_EACH(square) {
      auto rsquare = square.hsym();
      if (rking == king && rsquare.raw() <= square.raw()) {
        continue;
      }

      for (int i = 0; i < EvalPieceIndex::End; i++) {
        func(fv.kingPieceR[RelativeSquare(king, square).raw()][i],
             fv.kingPieceR[RelativeSquare(rking, rsquare).raw()][i]);
        func(fv.kingPieceXR[king.getFile()-1][RelativeSquare(king, square).raw()][i],
             fv.kingPieceXR[rking.getFile()-1][RelativeSquare(rking, rsquare).raw()][i]);
        func(fv.kingPieceYR[king.getRank()-1][RelativeSquare(king, square).raw()][i],
             fv.kingPieceYR[rking.getRank()-1][RelativeSquare(rking, rsquare).raw()][i]);
        func(fv.kingPiece[king.raw()][square.raw()][i],
             fv.kingPiece[rking.raw()][rsquare.raw()][i]);
      }
    }
  }

  // neighbor
  SQUARE_EACH(king) {
    auto rking = king.hsym();
    if (rking.raw() < king.raw()) {
      continue;
    }

    DIR_EACH_S(dir) {
      int d = static_cast<int>(dir);
      auto rd = static_cast<int>(getHSymDir(dir));
      if (rking == king && rd <= d) {
        continue;
      }

      for (int i = 0; i < EvalPieceIndex::End; i++) {
        for (int h = 0; h < EvalHandIndex::End; h++) {
          func(fv.kingNeighborHand[king.raw()][d][i][h],
               fv.kingNeighborHand[rking.raw()][rd][i][h]);
        }
      }
    }

    SQUARE_EACH(square) {
      auto rsquare = square.hsym();
      if (rking == king && rsquare.raw() < square.raw()) {
        continue;
      }

      DIR_EACH_S(dir) {
        int d = static_cast<int>(dir);
        auto rd = static_cast<int>(getHSymDir(dir));
        if (rking == king && rsquare == square && rd <= d) {
          continue;
        }

        for (int i1 = 0; i1 < EvalPieceIndex::End; i1++) {
          for (int i2 = 0; i2 < EvalPieceIndex::End; i2++) {
            func(fv.kingNeighborPieceR[d][i1][RelativeSquare(king, square).raw()][i2],
                 fv.kingNeighborPieceR[rd][i1][RelativeSquare(rking, rsquare).raw()][i2]);
            func(fv.kingNeighborPieceXR[king.getFile()-1][d][i1][RelativeSquare(king, square).raw()][i2],
                 fv.kingNeighborPieceXR[rking.getFile()-1][rd][i1][RelativeSquare(rking, rsquare).raw()][i2]);
            func(fv.kingNeighborPieceYR[king.getRank()-1][d][i1][RelativeSquare(king, square).raw()][i2],
                 fv.kingNeighborPieceYR[rking.getRank()-1][rd][i1][RelativeSquare(rking, rsquare).raw()][i2]);
            func(fv.kingNeighborPiece[king.raw()][d][i1][square.raw()][i2],
                 fv.kingNeighborPiece[rking.raw()][rd][i1][rsquare.raw()][i2]);
          }
        }
      }
    }
  }

  // open
  SQUARE_EACH(king) {
    auto rking = king.hsym();
    if (rking.raw() < king.raw()) {
      continue;
    }

    SQUARE_EACH(s) {
      auto rs = s.hsym();
      if (rking == king && rs.raw() <= s.raw()) {
        continue;
      }

      for (int i = 0; i < 8; i++) {
        func(fv.kingBRookVer[king.raw()][s.raw()][i],
             fv.kingBRookVer[rking.raw()][rs.raw()][i]);
        func(fv.kingWRookVer[king.raw()][s.raw()][i],
             fv.kingWRookVer[rking.raw()][rs.raw()][i]);
        func(fv.kingBRookHor[king.raw()][s.raw()][i],
             fv.kingBRookHor[rking.raw()][rs.raw()][i]);
        func(fv.kingWRookHor[king.raw()][s.raw()][i],
             fv.kingWRookHor[rking.raw()][rs.raw()][i]);
        func(fv.kingBBishopDiagL45[king.raw()][s.raw()][i],
             fv.kingBBishopDiagR45[rking.raw()][rs.raw()][i]);
        func(fv.kingWBishopDiagL45[king.raw()][s.raw()][i],
             fv.kingWBishopDiagR45[rking.raw()][rs.raw()][i]);
        func(fv.kingBBishopDiagR45[king.raw()][s.raw()][i],
             fv.kingBBishopDiagL45[rking.raw()][rs.raw()][i]);
        func(fv.kingWBishopDiagR45[king.raw()][s.raw()][i],
             fv.kingWBishopDiagL45[rking.raw()][rs.raw()][i]);
        func(fv.kingBLance[king.raw()][s.raw()][i],
             fv.kingBLance[rking.raw()][rs.raw()][i]);
        func(fv.kingWLance[king.raw()][s.raw()][i],
             fv.kingWLance[rking.raw()][rs.raw()][i]);
      }
    }
  }

  // safety
  SQUARE_EACH(king) {
    auto rking = king.hsym();
    if (rking.raw() < king.raw()) {
      continue;
    }

    for (int s = 0; s < KingSafetyLen; s++) {
      if (rking != king) {
        for (int h = 0; h < EvalHandIndex::End; h++) {
          func(fv.kingSafetyHand[king.raw()][s][h],
               fv.kingSafetyHand[rking.raw()][s][h]);
        }
      }

      SQUARE_EACH(square) {
        auto rsquare = square.hsym();
        if (rking == king && rsquare.raw() <= square.raw()) {
          continue;
        }

        for (int i = 0; i < EvalPieceIndex::End; i++) {
          func(fv.kingSafetyPieceR[s][RelativeSquare(king, square).raw()][i],
               fv.kingSafetyPieceR[s][RelativeSquare(rking, rsquare).raw()][i]);
          func(fv.kingSafetyPieceXR[king.getFile()-1][s][RelativeSquare(king, square).raw()][i],
               fv.kingSafetyPieceXR[rking.getFile()-1][s][RelativeSquare(rking, rsquare).raw()][i]);
          func(fv.kingSafetyPieceYR[king.getRank()-1][s][RelativeSquare(king, square).raw()][i],
               fv.kingSafetyPieceYR[rking.getRank()-1][s][RelativeSquare(rking, rsquare).raw()][i]);
          func(fv.kingSafetyPiece[king.raw()][s][square.raw()][i],
               fv.kingSafetyPiece[rking.raw()][s][rsquare.raw()][i]);
        }
      }
    }
  }
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
    uint8_t dir;
    uint8_t idx;
  };
  NeighborPiece bns[8];
  int bnn = 0;
  NeighborPiece wns[8];
  int wnn = 0;

  auto noking = nosseOr(position.getBOccupiedBitboard(),
                        position.getWOccupiedBitboard());
  noking.unset(position.getBlackKingSquare());
  noking.unset(position.getWhiteKingSquare());

  {
    auto bb = nosseAnd(noking,
                       MoveTables::king(position.getBlackKingSquare()));
    BB_EACH(square, bb) {
      Direction dir = position.getBlackKingSquare().dir(square);
      auto piece = position.getPieceOnBoard(square);
      bns[bnn].dir = static_cast<uint8_t>(dir);
      bns[bnn].idx = getEvalPieceIndex(piece);
      bnn++;
    }
  }

  {
    auto bb = nosseAnd(noking,
                       MoveTables::king(position.getWhiteKingSquare()));
    BB_EACH(square, bb) {
      Direction dir = square.dir(position.getWhiteKingSquare());
      auto piece = position.getPieceOnBoard(square);
      wns[wnn].dir = static_cast<uint8_t>(dir);
      wns[wnn].idx = getEvalPieceIndex(piece.enemy());
      wnn++;
    }
  }

  auto bsafety = safetyOfKing<Turn::Black>(position);
  auto wsafety = safetyOfKing<Turn::White>(position);

#define CALC_BLACK_HAND(t, T) do { \
  auto n = blackHand.get(PieceType::t()); \
  if (n != 0) { \
    if (type == FeatureOperationType::Evaluate) { \
      sum += ofv.kingSafetyHand[bking][bsafety][EvalHandIndex::B ## T + n - 1]; \
      sum -= ofv.kingSafetyHand[wking][wsafety][EvalHandIndex::W ## T + n - 1]; \
      for (int i = 0; i < bnn; i++) { \
        sum += ofv.kingNeighborHand[bking][bns[i].dir][bns[i].idx][EvalHandIndex::B ## T + n - 1]; \
      } \
      for (int i = 0; i < wnn; i++) { \
        sum -= ofv.kingNeighborHand[wking][wns[i].dir][wns[i].idx][EvalHandIndex::W ## T + n - 1]; \
      } \
    } else { \
      ofv.kingSafetyHand[bking][bsafety][EvalHandIndex::B ## T + n - 1] += delta; \
      ofv.kingSafetyHand[wking][wsafety][EvalHandIndex::W ## T + n - 1] -= delta; \
      for (int i = 0; i < bnn; i++) { \
        ofv.kingNeighborHand[bking][bns[i].dir][bns[i].idx][EvalHandIndex::B ## T + n - 1] += delta; \
      } \
      for (int i = 0; i < wnn; i++) { \
        ofv.kingNeighborHand[wking][wns[i].dir][wns[i].idx][EvalHandIndex::W ## T + n - 1] -= delta; \
      } \
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
      sum += ofv.kingSafetyHand[bking][bsafety][EvalHandIndex::W ## T + n - 1]; \
      sum -= ofv.kingSafetyHand[wking][wsafety][EvalHandIndex::B ## T + n - 1]; \
      for (int i = 0; i < bnn; i++) { \
        sum += ofv.kingNeighborHand[bking][bns[i].dir][bns[i].idx][EvalHandIndex::W ## T + n - 1]; \
      } \
      for (int i = 0; i < wnn; i++) { \
        sum -= ofv.kingNeighborHand[wking][wns[i].dir][wns[i].idx][EvalHandIndex::B ## T + n - 1]; \
      } \
    } else { \
      ofv.kingSafetyHand[bking][bsafety][EvalHandIndex::W ## T + n - 1] += delta; \
      ofv.kingSafetyHand[wking][wsafety][EvalHandIndex::B ## T + n - 1] -= delta; \
      for (int i = 0; i < bnn; i++) { \
        ofv.kingNeighborHand[bking][bns[i].dir][bns[i].idx][EvalHandIndex::W ## T + n - 1] += delta; \
      } \
      for (int i = 0; i < wnn; i++) { \
        ofv.kingNeighborHand[wking][wns[i].dir][wns[i].idx][EvalHandIndex::B ## T + n - 1] -= delta; \
      } \
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
        sum += ofv.kingSafetyPiece[bking][bsafety][bs][bIndex];
        sum -= ofv.kingSafetyPiece[wking][wsafety][ws][wIndex];
        for (int i = 0; i < bnn; i++) {
          sum += ofv.kingNeighborPiece[bking][bns[i].dir][bns[i].idx][bs][bIndex];
        }
        for (int i = 0; i < wnn; i++) {
          sum -= ofv.kingNeighborPiece[wking][wns[i].dir][wns[i].idx][ws][wIndex];
        }
      } else {
        ofv.kingSafetyPiece[bking][bsafety][bs][bIndex] += delta;
        ofv.kingSafetyPiece[wking][wsafety][ws][wIndex] -= delta;
        for (int i = 0; i < bnn; i++) {
          ofv.kingNeighborPiece[bking][bns[i].dir][bns[i].idx][bs][bIndex] += delta;
        }
        for (int i = 0; i < wnn; i++) {
          ofv.kingNeighborPiece[wking][wns[i].dir][wns[i].idx][ws][wIndex] -= delta;
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
      int wIndex = square.psym().raw();

      int count = MoveTables::diagR45(occR45, square).count() - 1;
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingBBishopDiagR45[bking][bIndex][count];
        sum -= ofv.kingWBishopDiagR45[wking][wIndex][count];
      } else {
        ofv.kingBBishopDiagR45[bking][bIndex][count] += delta;
        ofv.kingWBishopDiagR45[wking][wIndex][count] -= delta;
      }

      count = MoveTables::diagL45(occL45, square).count() - 1;
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingBBishopDiagL45[bking][bIndex][count];
        sum -= ofv.kingWBishopDiagL45[wking][wIndex][count];
      } else {
        ofv.kingBBishopDiagL45[bking][bIndex][count] += delta;
        ofv.kingWBishopDiagL45[wking][wIndex][count] -= delta;
      }
    }
  }

  {
    auto wbishop = nosseAnd(nosseOr(position.getWBishopBitboard(),
                                    position.getWHorseBitboard()),
                            Bitboard::nocorner());
    BB_EACH(square, wbishop) {
      int bIndex = square.raw();
      int wIndex = square.psym().raw();

      int count = MoveTables::diagR45(occR45, square).count() - 1;
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingWBishopDiagR45[bking][bIndex][count];
        sum -= ofv.kingBBishopDiagR45[wking][wIndex][count];
      } else {
        ofv.kingWBishopDiagR45[bking][bIndex][count] += delta;
        ofv.kingBBishopDiagR45[wking][wIndex][count] -= delta;
      }

      count = MoveTables::diagL45(occL45, square).count() - 1;
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingWBishopDiagL45[bking][bIndex][count];
        sum -= ofv.kingBBishopDiagL45[wking][wIndex][count];
      } else {
        ofv.kingWBishopDiagL45[bking][bIndex][count] += delta;
        ofv.kingBBishopDiagL45[wking][wIndex][count] -= delta;
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
      int wIndex = square.psym().raw();

      int count = MoveTables::ver(occ, square).count() - 1;
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingBRookVer[bking][bIndex][count];
        sum -= ofv.kingWRookVer[wking][wIndex][count];
      } else {
        ofv.kingBRookVer[bking][bIndex][count] += delta;
        ofv.kingWRookVer[wking][wIndex][count] -= delta;
      }

      count = MoveTables::hor(occ90, square).count() - 1;
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingBRookHor[bking][bIndex][count];
        sum -= ofv.kingWRookHor[wking][wIndex][count];
      } else {
        ofv.kingBRookHor[bking][bIndex][count] += delta;
        ofv.kingWRookHor[wking][wIndex][count] -= delta;
      }
    }
  }

  {
    auto wrook = nosseOr(position.getWRookBitboard(),
                         position.getWDragonBitboard());
    BB_EACH(square, wrook) {
      int bIndex = square.raw();
      int wIndex = square.psym().raw();

      int count = MoveTables::ver(occ, square).count() - 1;
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingWRookVer[bking][bIndex][count];
        sum -= ofv.kingBRookVer[wking][wIndex][count];
      } else {
        ofv.kingWRookVer[bking][bIndex][count] += delta;
        ofv.kingBRookVer[wking][wIndex][count] -= delta;
      }

      count = MoveTables::hor(occ90, square).count() - 1;
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingWRookHor[bking][bIndex][count];
        sum -= ofv.kingBRookHor[wking][wIndex][count];
      } else {
        ofv.kingWRookHor[bking][bIndex][count] += delta;
        ofv.kingBRookHor[wking][wIndex][count] -= delta;
      }
    }
  }

  {
    auto blance = position.getBLanceBitboard();
    BB_EACH(square, blance) {
      int bIndex = square.raw();
      int wIndex = square.psym().raw();

      int count = MoveTables::blackLance(occ, square).count() - 1;
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingBLance[bking][bIndex][count];
        sum -= ofv.kingWLance[wking][wIndex][count];
      } else {
        ofv.kingBLance[bking][bIndex][count] += delta;
        ofv.kingWLance[wking][wIndex][count] -= delta;
      }
    }
  }

  {
    auto wlance = position.getWLanceBitboard();
    BB_EACH(square, wlance) {
      int bIndex = square.raw();
      int wIndex = square.psym().raw();

      int count = MoveTables::whiteLance(occ, square).count() - 1;
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingWLance[bking][bIndex][count];
        sum -= ofv.kingBLance[wking][wIndex][count];
      } else {
        ofv.kingWLance[bking][bIndex][count] += delta;
        ofv.kingBLance[wking][wIndex][count] -= delta;
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
    FV_SUMMARIZE(kingPieceR),
    FV_SUMMARIZE(kingPieceXR),
    FV_SUMMARIZE(kingPieceYR),
    FV_SUMMARIZE(kingPiece),
    FV_SUMMARIZE(kingNeighborHand),
    FV_SUMMARIZE(kingNeighborPieceR),
    FV_SUMMARIZE(kingNeighborPieceXR),
    FV_SUMMARIZE(kingNeighborPieceYR),
    FV_SUMMARIZE(kingNeighborPiece),
    FV_SUMMARIZE(kingBRookVer),
    FV_SUMMARIZE(kingWRookVer),
    FV_SUMMARIZE(kingBRookHor),
    FV_SUMMARIZE(kingWRookHor),
    FV_SUMMARIZE(kingBBishopDiagL45),
    FV_SUMMARIZE(kingWBishopDiagL45),
    FV_SUMMARIZE(kingBBishopDiagR45),
    FV_SUMMARIZE(kingWBishopDiagR45),
    FV_SUMMARIZE(kingBLance),
    FV_SUMMARIZE(kingWLance),
    FV_SUMMARIZE(kingSafetyHand),
    FV_SUMMARIZE(kingSafetyPieceR),
    FV_SUMMARIZE(kingSafetyPieceXR),
    FV_SUMMARIZE(kingSafetyPieceYR),
    FV_SUMMARIZE(kingSafetyPiece),
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
