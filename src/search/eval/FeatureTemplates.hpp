/* FeatureTemplates.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_SEARCH_EVAL_FEATURETEMPLATES_HPP__
#define SUNFISH_SEARCH_EVAL_FEATURETEMPLATES_HPP__

#include "core/base/Square.hpp"
#include "core/position/Position.hpp"
#include "search/eval/FeatureVector.hpp"

namespace {

using namespace sunfish;

template <class FV>
inline
void cumulate(FV& fv) {
  // generate kkpc
#define CUM_KKPC_HAND(pt, n) do { \
  typename FV::Type cum = 0; \
  for (int i = 0; i < n; i++) { \
    cum += fv.kkp[bking.raw()][wking.raw()][KKP::H ## pt + i]; \
    fv.kkpc[bking.raw()][wking.raw()][KKP::H ## pt + i] = cum; \
  } \
} while (false)
  SQUARE_EACH(bking) {
    SQUARE_EACH(wking) {
      CUM_KKPC_HAND(Pawn  , 18);
      CUM_KKPC_HAND(Lance ,  4);
      CUM_KKPC_HAND(Knight,  4);
      CUM_KKPC_HAND(Silver,  4);
      CUM_KKPC_HAND(Gold  ,  4);
      CUM_KKPC_HAND(Bishop,  4);
      CUM_KKPC_HAND(Rook  ,  4);
    }
  }
#undef CUM_KKPC_HAND
}

template <class FV>
inline
void rcumulate(FV& fv) {
  // generate kkpc
#define RCUM_KKPC_HAND(pt, n) do { \
  typename FV::Type cum = 0; \
  for (int i = n - 1; i >= 0; i--) { \
    cum += fv.kkpc[bking.raw()][wking.raw()][KKP::H ## pt + i]; \
    fv.kkp[bking.raw()][wking.raw()][KKP::H ## pt + i] = cum; \
  } \
} while (false)
  SQUARE_EACH(bking) {
    SQUARE_EACH(wking) {
      RCUM_KKPC_HAND(Pawn  , 18);
      RCUM_KKPC_HAND(Lance ,  4);
      RCUM_KKPC_HAND(Knight,  4);
      RCUM_KKPC_HAND(Silver,  4);
      RCUM_KKPC_HAND(Gold  ,  4);
      RCUM_KKPC_HAND(Bishop,  4);
      RCUM_KKPC_HAND(Rook  ,  4);
    }
  }
#undef RCUM_KKPC_HAND
}

template <class FV>
inline
void add(FV& dst, const FV& src) {
  for (int i = 0; i < KKP::Size; i++) {
    (&dst.kkp[0][0][0])[i] += (&src.kkp[0][0][0])[i];
  }
}

template <class FV1, class FV2, class T>
inline
void each(FV1& fv1, FV2& fv2, T&& func) {
  for (int i = 0; i < KKP::Size; i++) {
    func((&fv1.kkp[0][0][0])[i],
         (&fv2.kkp[0][0][0])[i]);
  }
}

template <class FV, class T>
inline
void symmetrize(FV& fv, T&& func) {
  SQUARE_EACH(bking) {
    auto rbking = bking.hsym();
    if (rbking.raw() < bking.raw()) {
      continue;
    }

    SQUARE_EACH(wking) {
      auto rwking = wking.hsym();
      if (rbking == bking &&
          rwking.raw() < wking.raw()) {
        continue;
      }

      for (int x = 0; x < KKP::End; x++) {
        int rx = symmetricalKkpIndex(x);
        if (rbking == bking &&
            rwking == wking &&
            rx <= x) {
          continue;
        }

        func(fv.kkp[bking.raw()][wking.raw()][x],
             fv.kkp[rbking.raw()][rwking.raw()][rx]);
      }
    }
  }
}

enum FeatureOperationType {
  Evaluate,
  Extract,
};

template <FeatureOperationType type, class FV, class T>
inline
T operate(FV& fv, const Position& position, T delta) {
  T sum = 0;
  auto bking = position.getBlackKingSquare().raw();
  auto wking = position.getWhiteKingSquare().raw();
  auto rbking = position.getWhiteKingSquare().dsym().raw();
  auto rwking = position.getBlackKingSquare().dsym().raw();

#define CALC_BLACK_HAND(t, T) do { \
  auto n = blackHand.get(PieceType::t()); \
  if (n != 0) { \
    if (type == FeatureOperationType::Evaluate) { \
      sum += fv.kkpc[bking][wking][KKP::H ## T + n - 1]; \
    } else { \
      fv.kkpc[bking][wking][KKP::H ## T + n - 1] += delta; \
    } \
  } \
} while (false)
  auto& blackHand = position.getBlackHand();
  CALC_BLACK_HAND(pawn  , Pawn  );
  CALC_BLACK_HAND(lance , Lance );
  CALC_BLACK_HAND(knight, Knight);
  CALC_BLACK_HAND(silver, Silver);
  CALC_BLACK_HAND(gold  , Gold  );
  CALC_BLACK_HAND(bishop, Bishop);
  CALC_BLACK_HAND(rook  , Rook  );
#undef CALC_BLACK_HAND

#define CALC_WHITE_HAND(t, T) do { \
  auto n = whiteHand.get(PieceType::t()); \
  if (n != 0) { \
    if (type == FeatureOperationType::Evaluate) { \
      sum -= fv.kkpc[rbking][rwking][KKP::H ## T + n - 1]; \
    } else { \
      fv.kkpc[rbking][rwking][KKP::H ## T + n - 1] -= delta; \
    } \
  } \
} while (false)
  auto& whiteHand = position.getWhiteHand();
  CALC_WHITE_HAND(pawn  , Pawn  );
  CALC_WHITE_HAND(lance , Lance );
  CALC_WHITE_HAND(knight, Knight);
  CALC_WHITE_HAND(silver, Silver);
  CALC_WHITE_HAND(gold  , Gold  );
  CALC_WHITE_HAND(bishop, Bishop);
  CALC_WHITE_HAND(rook  , Rook  );
#undef CALC_WHITE_HAND

  Bitboard occ = nosseOr(position.getBOccupiedBitboard(),
                         position.getWOccupiedBitboard());
  occ.unset(position.getBlackKingSquare());
  occ.unset(position.getWhiteKingSquare());

  BB_EACH(square, occ) {
    auto piece = position.getPieceOnBoard(square);
    auto pieceType = piece.type();

    if (piece.isBlack()) {
      if (type == FeatureOperationType::Evaluate) {
        sum += fv.kkp[bking][wking][kkpBoardIndex(pieceType, square)];
      } else {
        fv.kkp[bking][wking][kkpBoardIndex(pieceType, square)] += delta;
      }
    } else {
      if (type == FeatureOperationType::Evaluate) {
        sum -= fv.kkp[rbking][rwking][kkpBoardIndex(pieceType, square.dsym())];
      } else {
        fv.kkp[rbking][rwking][kkpBoardIndex(pieceType, square.dsym())] -= delta;
      }
    }
  }

  return sum;
}

} // namespace

#endif // SUNFISH_SEARCH_EVAL_FEATURETEMPLATES_HPP__
