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

namespace {

using namespace sunfish;

template <class FV, class OFV>
inline
void optimize(FV& fv, OFV& ofv) {
  FV_PART_COPY(ofv, fv, kingGoldPiece);
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

  // kingHand, kingNumGoldHand
  //   => kingNumGoldHand
  auto optimKingNumGoldHand = [&fv, &ofv](Square king, KingHand kh, int n) {
    for (int ng = 0; ng <= 8; ng++) {
      typename FV::Type cum = 0;
      for (int i = 0; i < n; i++) {
        cum += fv.kingNumGoldHand[king.raw()][ng][kh + i]
             + fv.kingHand[king.raw()][kh + i];
        ofv.kingNumGoldHand[king.raw()][ng][kh + i] = cum;
      }
    }
  };
  SQUARE_EACH(king) {
    optimKingNumGoldHand(king, KingHand::BPawn  , 18);
    optimKingNumGoldHand(king, KingHand::WPawn  , 18);
    optimKingNumGoldHand(king, KingHand::BLance ,  4);
    optimKingNumGoldHand(king, KingHand::WLance ,  4);
    optimKingNumGoldHand(king, KingHand::BKnight,  4);
    optimKingNumGoldHand(king, KingHand::WKnight,  4);
    optimKingNumGoldHand(king, KingHand::BSilver,  4);
    optimKingNumGoldHand(king, KingHand::WSilver,  4);
    optimKingNumGoldHand(king, KingHand::BGold  ,  4);
    optimKingNumGoldHand(king, KingHand::WGold  ,  4);
    optimKingNumGoldHand(king, KingHand::BBishop,  2);
    optimKingNumGoldHand(king, KingHand::WBishop,  2);
    optimKingNumGoldHand(king, KingHand::BRook  ,  2);
    optimKingNumGoldHand(king, KingHand::WRook  ,  2);
  }

  // kingPiece, kingNumGoldPiece
  //   => kingNumGoldPiece
  SQUARE_EACH(king) {
    for (int i = 0; i < KingPiece::End; i++) {
      for (int ng = 0; ng <= 8; ng++) {
        ofv.kingNumGoldPiece[king.raw()][ng][i]
          = fv.kingNumGoldPiece[king.raw()][ng][i]
          + fv.kingPiece[king.raw()][i];
      }
    }
  }
}

template <class FV, class OFV>
inline
void expand(FV& fv, OFV& ofv) {
  FV_PART_COPY(fv, ofv, kingGoldPiece);
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

  // kingNumGoldHand
  //   => kingHand, kingNumGoldHand
  auto expandKingNumGoldHand = [&fv, &ofv](Square king, KingHand kh, int n) {
    for (int i = n - 1; i >= 0; i--) {
      fv.kingHand[king.raw()][kh + i] = 0;
    }
    for (int ng = 0; ng <= 8; ng++) {
      typename FV::Type cum = 0;
      for (int i = n - 1; i >= 0; i--) {
        cum += ofv.kingNumGoldHand[king.raw()][ng][kh + i];
        fv.kingHand[king.raw()][kh + i] += cum;
        fv.kingNumGoldHand[king.raw()][ng][kh + i] = cum;
      }
    }
  };
  SQUARE_EACH(king) {
    expandKingNumGoldHand(king, KingHand::BPawn  , 18);
    expandKingNumGoldHand(king, KingHand::WPawn  , 18);
    expandKingNumGoldHand(king, KingHand::BLance ,  4);
    expandKingNumGoldHand(king, KingHand::WLance ,  4);
    expandKingNumGoldHand(king, KingHand::BKnight,  4);
    expandKingNumGoldHand(king, KingHand::WKnight,  4);
    expandKingNumGoldHand(king, KingHand::BSilver,  4);
    expandKingNumGoldHand(king, KingHand::WSilver,  4);
    expandKingNumGoldHand(king, KingHand::BGold  ,  4);
    expandKingNumGoldHand(king, KingHand::WGold  ,  4);
    expandKingNumGoldHand(king, KingHand::BBishop,  2);
    expandKingNumGoldHand(king, KingHand::WBishop,  2);
    expandKingNumGoldHand(king, KingHand::BRook  ,  2);
    expandKingNumGoldHand(king, KingHand::WRook  ,  2);
  }

  // kingNumGoldPiece
  //   => kingPiece, kingNumGoldPiece
  SQUARE_EACH(king) {
    for (int i = 0; i < KingPiece::End; i++) {
      fv.kingPiece[king.raw()][i] = 0;
      for (int ng = 0; ng <= 8; ng++) {
        fv.kingPiece[king.raw()][i] += ofv.kingNumGoldPiece[king.raw()][ng][i];
        fv.kingNumGoldPiece[king.raw()][ng][i] = ofv.kingNumGoldPiece[king.raw()][ng][i];
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
      for (int x = 0; x < KingHand::End; x++) {
        // King Hand
        func(fv.kingHand[king.raw()][x],
             fv.kingHand[rking.raw()][x]);

        // King NumberOfGolds Hand
        for (int ng = 0; ng <= 8; ng++) {
          func(fv.kingNumGoldHand[king.raw()][ng][x],
               fv.kingNumGoldHand[rking.raw()][ng][x]);
        }
      }
    }

    for (int x = 0; x < KingPiece::End; x++) {
      int rx = symmetricalKingPieceIndex(x);
      if (rking == king && rx < x) {
        continue;
      }

      // King Gold Piece
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

      // King Piece
      func(fv.kingPiece[king.raw()][x],
           fv.kingPiece[rking.raw()][rx]);

      // King NumberOfGolds Piece
      for (int ng = 0; ng <= 8; ng++) {
        func(fv.kingNumGoldPiece[king.raw()][ng][x],
             fv.kingNumGoldPiece[rking.raw()][ng][rx]);
      }
    }

    SQUARE_EACH(s) {
      auto rs = s.hsym();
      if (rking == king && rs.raw() <= s.raw()) {
        continue;
      }

      for (int i = 0; i < 8; i++) {
        // King Open
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

#define CALC_BLACK_HAND(t, T) do { \
  auto n = blackHand.get(PieceType::t()); \
  if (n != 0) { \
    if (type == FeatureOperationType::Evaluate) { \
      sum += ofv.kingNumGoldHand[bking][bgoldn][KingHand::B ## T + n - 1]; \
      sum -= ofv.kingNumGoldHand[wking][wgoldn][KingHand::W ## T + n - 1]; \
    } else { \
      ofv.kingNumGoldHand[bking][bgoldn][KingHand::B ## T + n - 1] += delta; \
      ofv.kingNumGoldHand[wking][wgoldn][KingHand::W ## T + n - 1] -= delta; \
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
      sum += ofv.kingNumGoldHand[bking][bgoldn][KingHand::W ## T + n - 1]; \
      sum -= ofv.kingNumGoldHand[wking][wgoldn][KingHand::B ## T + n - 1]; \
    } else { \
      ofv.kingNumGoldHand[bking][bgoldn][KingHand::W ## T + n - 1] += delta; \
      ofv.kingNumGoldHand[wking][wgoldn][KingHand::B ## T + n - 1] -= delta; \
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
    auto bb = nosseOr(position.getBOccupiedBitboard(),
                      position.getWOccupiedBitboard());
    bb.unset(position.getBlackKingSquare());
    bb.unset(position.getWhiteKingSquare());

    BB_EACH(square, bb) {
      auto piece = position.getPieceOnBoard(square);
      int bkpIndex = kingPieceIndex(piece, square);
      int wkpIndex = kingPieceIndex(piece.enemy(), square.psym());

      if (type == FeatureOperationType::Evaluate) {
        sum += ofv.kingNumGoldPiece[bking][bgoldn][bkpIndex];
        sum -= ofv.kingNumGoldPiece[wking][wgoldn][wkpIndex];
      } else {
        ofv.kingNumGoldPiece[bking][bgoldn][bkpIndex] += delta;
        ofv.kingNumGoldPiece[wking][wgoldn][wkpIndex] -= delta;
      }

      for (int i = 0; i < bgoldn; i++) {
        if (type == FeatureOperationType::Evaluate) {
          sum += ofv.kingGoldPiece[bking][bgolds[i]][bkpIndex];
        } else {
          ofv.kingGoldPiece[bking][bgolds[i]][bkpIndex] += delta;
        }
      }

      for (int i = 0; i < wgoldn; i++) {
        if (type == FeatureOperationType::Evaluate) {
          sum -= ofv.kingGoldPiece[wking][wgolds[i]][wkpIndex];
        } else {
          ofv.kingGoldPiece[wking][wgolds[i]][wkpIndex] -= delta;
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
    FV_SUMMARIZE(kingNumGoldHand),
    FV_SUMMARIZE(kingNumGoldPiece),
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
