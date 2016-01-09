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

namespace {

using namespace sunfish;

template <class FV, class CV>
inline
void cumulate(FV& fv, CV& cv) {
  // generate khpc
#define CUM_KKPC_HAND(pt, n) do { \
  typename FV::Type cum = 0; \
  for (int i = 0; i < n; i++) { \
    cum += fv.kingHand[king.raw()][KingHand::pt + i]; \
    cv.kingHand[king.raw()][KingHand::pt + i] = cum; \
  } \
} while (false)
  SQUARE_EACH(king) {
    CUM_KKPC_HAND(BPawn  , 18);
    CUM_KKPC_HAND(WPawn  , 18);
    CUM_KKPC_HAND(BLance ,  4);
    CUM_KKPC_HAND(WLance ,  4);
    CUM_KKPC_HAND(BKnight,  4);
    CUM_KKPC_HAND(WKnight,  4);
    CUM_KKPC_HAND(BSilver,  4);
    CUM_KKPC_HAND(WSilver,  4);
    CUM_KKPC_HAND(BGold  ,  4);
    CUM_KKPC_HAND(WGold  ,  4);
    CUM_KKPC_HAND(BBishop,  2);
    CUM_KKPC_HAND(WBishop,  2);
    CUM_KKPC_HAND(BRook  ,  2);
    CUM_KKPC_HAND(WRook  ,  2);
  }
#undef CUM_KKPC_HAND
}

template <class FV, class CV>
inline
void rcumulate(FV& fv, CV& cv) {
  // generate khpc
#define RCUM_KKPC_HAND(pt, n) do { \
  typename FV::Type cum = 0; \
  for (int i = n - 1; i >= 0; i--) { \
    cum += cv.kingHand[king.raw()][KingHand:: pt + i]; \
    fv.kingHand[king.raw()][KingHand:: pt + i] = cum; \
  } \
} while (false)
  SQUARE_EACH(king) {
    RCUM_KKPC_HAND(BPawn  , 18);
    RCUM_KKPC_HAND(WPawn  , 18);
    RCUM_KKPC_HAND(BLance ,  4);
    RCUM_KKPC_HAND(WLance ,  4);
    RCUM_KKPC_HAND(BKnight,  4);
    RCUM_KKPC_HAND(WKnight,  4);
    RCUM_KKPC_HAND(BSilver,  4);
    RCUM_KKPC_HAND(WSilver,  4);
    RCUM_KKPC_HAND(BGold  ,  4);
    RCUM_KKPC_HAND(WGold  ,  4);
    RCUM_KKPC_HAND(BBishop,  2);
    RCUM_KKPC_HAND(WBishop,  2);
    RCUM_KKPC_HAND(BRook  ,  2);
    RCUM_KKPC_HAND(WRook  ,  2);
  }
#undef RCUM_KKPC_HAND
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

    // KingHand
    if (rking != king) {
      for (int x = 0; x < KingHand::End; x++) {
        func(fv.kingHand[king.raw()][x],
             fv.kingHand[rking.raw()][x]);
      }
    }

    for (int x = 0; x < KingPiece::End; x++) {
      int rx = symmetricalKingPieceIndex(x);
      if (rking == king && rx < x) {
        continue;
      }

      // KingGoldPiece
      for (int y = 0; y < KingGold::End; y++) {
        int ry = symmetricalKingGoldIndex(y);
        if (rking == king && rx == x && ry <= y) {
          continue;
        }

        func(fv.kingGoldPiece[king.raw()][y][x],
             fv.kingGoldPiece[rking.raw()][ry][rx]);
      }

      if (rking == king && rx <= x) {
        continue;
      }

      // KingPiece
      func(fv.kingPiece[king.raw()][x],
           fv.kingPiece[rking.raw()][rx]);
    }

    SQUARE_EACH(s) {
      auto rs = s.hsym();
      if (rking == king && rs.raw() <= s.raw()) {
        continue;
      }

      for (int i = 0; i < 8; i++) {
        // KingOpen
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

    if (rking.raw() <= king.raw()) {
      continue;
    }

    for (int i = 0; i <= 8; i++) {
      func(fv.kingNumGold[king.raw()][i],
           fv.kingNumGold[rking.raw()][i]);
    }
  }
}

enum FeatureOperationType {
  Evaluate,
  Extract,
};

template <FeatureOperationType type, class FV, class CV, class T>
inline
T operate(FV& fv, CV& cv, const Position& position, T delta) {
  T sum = 0;
  auto bking = position.getBlackKingSquare().raw();
  auto wking = position.getWhiteKingSquare().dsym().raw();

#define CALC_BLACK_HAND(t, T) do { \
  auto n = blackHand.get(PieceType::t()); \
  if (n != 0) { \
    if (type == FeatureOperationType::Evaluate) { \
      sum += cv.kingHand[bking][KingHand::B ## T + n - 1]; \
      sum -= cv.kingHand[wking][KingHand::W ## T + n - 1]; \
    } else { \
      cv.kingHand[bking][KingHand::B ## T + n - 1] += delta; \
      cv.kingHand[wking][KingHand::W ## T + n - 1] -= delta; \
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
      sum += cv.kingHand[bking][KingHand::W ## T + n - 1]; \
      sum -= cv.kingHand[wking][KingHand::B ## T + n - 1]; \
    } else { \
      cv.kingHand[bking][KingHand::W ## T + n - 1] += delta; \
      cv.kingHand[wking][KingHand::B ## T + n - 1] -= delta; \
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

  int bgolds[8];
  int bgoldn = 0;
  int wgolds[8];
  int wgoldn = 0;

  {
    auto bgold = nosseAnd(position.getBGoldBitboard(),
                          MoveTables::king(position.getBlackKingSquare()));
    BB_EACH(square, bgold) {
      Direction dir = position.getBlackKingSquare().dir(square);
      bgolds[bgoldn++] = kingGoldIndex(dir);
    }
  }

  {
    auto bsilver = nosseAnd(position.getBSilverBitboard(),
                            MoveTables::king(position.getBlackKingSquare()));
    BB_EACH(square, bsilver) {
      Direction dir = position.getBlackKingSquare().dir(square);
      bgolds[bgoldn++] = kingSilverIndex(dir);
    }
  }

  {
    auto wgold = nosseAnd(position.getWGoldBitboard(),
                          MoveTables::king(position.getWhiteKingSquare()));
    BB_EACH(square, wgold) {
      Direction dir = square.dir(position.getWhiteKingSquare());
      wgolds[wgoldn++] = kingGoldIndex(dir);
    }
  }

  {
    auto wsilver = nosseAnd(position.getWSilverBitboard(),
                            MoveTables::king(position.getWhiteKingSquare()));
    BB_EACH(square, wsilver) {
      Direction dir = square.dir(position.getWhiteKingSquare());
      wgolds[wgoldn++] = kingSilverIndex(dir);
    }
  }

  {
    if (type == FeatureOperationType::Evaluate) {
      sum += fv.kingNumGold[bking][bgoldn];
      sum -= fv.kingNumGold[wking][wgoldn];
    } else {
      fv.kingNumGold[bking][bgoldn] += delta;
      fv.kingNumGold[wking][wgoldn] -= delta;
    }
  }

  {
    auto bb = nosseOr(position.getBOccupiedBitboard(),
                      position.getWOccupiedBitboard());
    bb.unset(position.getBlackKingSquare());
    bb.unset(position.getWhiteKingSquare());

    BB_EACH(square, bb) {
      auto piece = position.getPieceOnBoard(square);
      int bkpIndex = kingPieceIndex(piece, square);
      int wkpIndex = kingPieceIndex(piece.enemy(), square.dsym());

      if (type == FeatureOperationType::Evaluate) {
        sum += fv.kingPiece[bking][bkpIndex];
        sum -= fv.kingPiece[wking][wkpIndex];
      } else {
        fv.kingPiece[bking][bkpIndex] += delta;
        fv.kingPiece[wking][wkpIndex] -= delta;
      }

      for (int i = 0; i < bgoldn; i++) {
        if (type == FeatureOperationType::Evaluate) {
          sum += fv.kingGoldPiece[bking][bgolds[i]][bkpIndex];
        } else {
          fv.kingGoldPiece[bking][bgolds[i]][bkpIndex] += delta;
        }
      }

      for (int i = 0; i < wgoldn; i++) {
        if (type == FeatureOperationType::Evaluate) {
          sum -= fv.kingGoldPiece[wking][wgolds[i]][wkpIndex];
        } else {
          fv.kingGoldPiece[wking][wgolds[i]][wkpIndex] -= delta;
        }
      }
    }
  }

  auto occ = nosseOr(position.getBOccupiedBitboard(),
                     position.getWOccupiedBitboard());
  auto occ90 = position.get90RotatedBitboard();
  auto occR45 = position.getRight45RotatedBitboard();
  auto occL45 = position.getLeft45RotatedBitboard();

  {
    auto bbishop = position.getBBishopBitboard() |
                   position.getBHorseBitboard();
    BB_EACH(square, bbishop) {
      int bIndex = square.raw();
      int wIndex = square.dsym().raw();

      int count = MoveTables::diagR45(occR45, square).count() - 1;
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += fv.kingBBishopDiagR45[bking][bIndex][count];
        sum -= fv.kingWBishopDiagR45[wking][wIndex][count];
      } else {
        fv.kingBBishopDiagR45[bking][bIndex][count] += delta;
        fv.kingWBishopDiagR45[wking][wIndex][count] -= delta;
      }

      count = MoveTables::diagL45(occL45, square).count() - 1;
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += fv.kingBBishopDiagL45[bking][bIndex][count];
        sum -= fv.kingWBishopDiagL45[wking][wIndex][count];
      } else {
        fv.kingBBishopDiagL45[bking][bIndex][count] += delta;
        fv.kingWBishopDiagL45[wking][wIndex][count] -= delta;
      }
    }
  }

  {
    auto wbishop = position.getWBishopBitboard() |
                   position.getWHorseBitboard();
    BB_EACH(square, wbishop) {
      int bIndex = square.raw();
      int wIndex = square.dsym().raw();

      int count = MoveTables::diagR45(occR45, square).count() - 1;
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += fv.kingWBishopDiagR45[bking][bIndex][count];
        sum -= fv.kingBBishopDiagR45[wking][wIndex][count];
      } else {
        fv.kingWBishopDiagR45[bking][bIndex][count] += delta;
        fv.kingBBishopDiagR45[wking][wIndex][count] -= delta;
      }

      count = MoveTables::diagL45(occL45, square).count() - 1;
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += fv.kingWBishopDiagL45[bking][bIndex][count];
        sum -= fv.kingBBishopDiagL45[wking][wIndex][count];
      } else {
        fv.kingWBishopDiagL45[bking][bIndex][count] += delta;
        fv.kingBBishopDiagL45[wking][wIndex][count] -= delta;
      }
    }
  }

  {
    auto brook = position.getBRookBitboard() |
                 position.getBDragonBitboard();
    BB_EACH(square, brook) {
      int bIndex = square.raw();
      int wIndex = square.dsym().raw();

      int count = MoveTables::ver(occ, square).count() - 1;
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += fv.kingBRookVer[bking][bIndex][count];
        sum -= fv.kingWRookVer[wking][wIndex][count];
      } else {
        fv.kingBRookVer[bking][bIndex][count] += delta;
        fv.kingWRookVer[wking][wIndex][count] -= delta;
      }

      count = MoveTables::hor(occ90, square).count() - 1;
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += fv.kingBRookHor[bking][bIndex][count];
        sum -= fv.kingWRookHor[wking][wIndex][count];
      } else {
        fv.kingBRookHor[bking][bIndex][count] += delta;
        fv.kingWRookHor[wking][wIndex][count] -= delta;
      }
    }
  }

  {
    auto wrook = position.getWRookBitboard();
    BB_EACH(square, wrook) {
      int bIndex = square.raw();
      int wIndex = square.dsym().raw();

      int count = MoveTables::ver(occ, square).count() - 1;
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += fv.kingWRookVer[bking][bIndex][count];
        sum -= fv.kingBRookVer[wking][wIndex][count];
      } else {
        fv.kingWRookVer[bking][bIndex][count] += delta;
        fv.kingBRookVer[wking][wIndex][count] -= delta;
      }

      count = MoveTables::hor(occ90, square).count() - 1;
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += fv.kingWRookHor[bking][bIndex][count];
        sum -= fv.kingBRookHor[wking][wIndex][count];
      } else {
        fv.kingWRookHor[bking][bIndex][count] += delta;
        fv.kingBRookHor[wking][wIndex][count] -= delta;
      }
    }
  }

  {
    auto blance = position.getBLanceBitboard();
    BB_EACH(square, blance) {
      int bIndex = square.raw();
      int wIndex = square.dsym().raw();

      int count = MoveTables::blackLance(occ, square).count() - 1;
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += fv.kingBLance[bking][bIndex][count];
        sum -= fv.kingWLance[wking][wIndex][count];
      } else {
        fv.kingBLance[bking][bIndex][count] += delta;
        fv.kingWLance[wking][wIndex][count] -= delta;
      }
    }
  }

  {
    auto wlance = position.getWLanceBitboard();
    BB_EACH(square, wlance) {
      int bIndex = square.raw();
      int wIndex = square.dsym().raw();

      int count = MoveTables::whiteLance(occ, square).count() - 1;
      ASSERT(count >= 0 && count < 8);
      if (type == FeatureOperationType::Evaluate) {
        sum += fv.kingWLance[bking][bIndex][count];
        sum -= fv.kingBLance[wking][wIndex][count];
      } else {
        fv.kingWLance[bking][bIndex][count] += delta;
        fv.kingBLance[wking][wIndex][count] -= delta;
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
  summary.nonZero = 0;
  summary.max = array[0];
  summary.min = array[0];
  summary.maxAbs = std::abs(array[0]);
  double sum = 0;

  for (size_t i = 0; i < size; i++) {
    if (array[i] == static_cast<T>(0)) {
      summary.zero++;
    } else {
      summary.nonZero++;
    }
    summary.max = std::max(summary.max, array[i]);
    summary.min = std::min(summary.min, array[i]);
    sum += std::abs(array[i]);
    summary.maxAbs = std::max(summary.maxAbs,
                              static_cast<T>(std::abs(array[i])));
  }
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
    FV_SUMMARIZE(kingPiece),
    FV_SUMMARIZE(kingGoldPiece),
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
    FV_SUMMARIZE(kingNumGold),
  };
#undef FV_PART_PTR
#undef FV_PART_SIZE
#undef FV_SUMMARIZE
}

} // namespace

#endif // SUNFISH_SEARCH_EVAL_FEATURETEMPLATES_HPP__
