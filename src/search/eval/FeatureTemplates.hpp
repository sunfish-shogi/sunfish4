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
#define GEN_KKPC_HAND(pt, n) do { \
  typename FV::Type cum = 0; \
  for (int i = 0; i < n; i++) { \
    cum += fv.kkp[bking.raw()][wking.raw()][KKP::H ## pt + i]; \
    fv.kkpc[bking.raw()][wking.raw()][KKP::H ## pt + i] = cum; \
  } \
} while (false)
  SQUARE_EACH(bking) {
    SQUARE_EACH(wking) {
      GEN_KKPC_HAND(Pawn  , 18);
      GEN_KKPC_HAND(Lance ,  4);
      GEN_KKPC_HAND(Knight,  4);
      GEN_KKPC_HAND(Silver,  4);
      GEN_KKPC_HAND(Gold  ,  4);
      GEN_KKPC_HAND(Bishop,  4);
      GEN_KKPC_HAND(Rook  ,  4);
    }
  }
#undef GEN_KKPC_HAND
}

enum FeatureOperationType {
  Evaluate,
  Extract,
};

template <class FV, class T, FeatureOperationType type>
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
