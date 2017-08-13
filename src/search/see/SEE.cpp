/* SEE.cpp
 *
 * Kubo Ryosuke
 */

#include "search/see/SEE.hpp"
#include "search/eval/Material.hpp"
#include "core/move/MoveTables.hpp"
#include <algorithm>

namespace sunfish {

Score SEE::calculate(const Position& position,
                     Move move) {
  Square from;
  Square to = move.to();
  Piece piece;
  Piece captured = position.getPieceOnBoard(to);
  Score score = Score::zero();

  if (move.isDrop()) {
    from = Square::invalid();
    piece = move.droppingPieceType().black();

  } else {
    from = move.from();
    piece = position.getPieceOnBoard(from);
    ASSERT(!piece.isEmpty());
    ASSERT(position.getTurn() == Turn::Black ? piece.isBlack() : piece.isWhite());
    ASSERT(position.getTurn() == Turn::Black ? !captured.isBlack() : !captured.isWhite());

    if (move.isPromotion()) {
      score += material::promotionScore(piece);
      piece = piece.promote();
    }

    if (!captured.isEmpty()) {
      score += material::exchangeScore(captured);
    }
  }

  auto bb = extractAggressors(position, from, to);

  if (position.getTurn() == Turn::Black) {
    return search(position, bb, to, score, material::exchangeScore(piece));
  } else {
    return -search(position, bb, to, -score, material::exchangeScore(piece));
  }
}

Bitboard SEE::extractAggressors(const Position& position,
                                Square from,
                                Square to) {
  Bitboard occ = position.getBOccupiedBitboard() | position.getWOccupiedBitboard();
  RotatedBitboard occ90 = position.get90RotatedBitboard();
  RotatedBitboard occR45 = position.getRight45RotatedBitboard();
  RotatedBitboard occL45 = position.getLeft45RotatedBitboard();

  if (from.isValid()) {
    occ.unset(from);
    occ90.unset(from.rotate90());
    occR45.unset(from.rotateRight45());
    occL45.unset(from.rotateLeft45());
  }

  Bitboard bb = Bitboard::zero();
  bb |= (Bitboard::mask(to) << 1) & position.getBPawnBitboard();
  bb |= (Bitboard::mask(to) >> 1) & position.getWPawnBitboard();
  bb |= MoveTables::whiteLance(occ, to) & position.getBLanceBitboard();
  bb |= MoveTables::blackLance(occ, to) & position.getWLanceBitboard();
  bb |= MoveTables::whiteKnight(to) & position.getBKnightBitboard();
  bb |= MoveTables::blackKnight(to) & position.getWKnightBitboard();
  bb |= MoveTables::whiteSilver(to) & position.getBSilverBitboard();
  bb |= MoveTables::blackSilver(to) & position.getWSilverBitboard();
  bb |= MoveTables::whiteGold(to) & position.getBGoldBitboard();
  bb |= MoveTables::blackGold(to) & position.getWGoldBitboard();
  bb |= (MoveTables::ver(occ, to) |
         MoveTables::hor(occ90, to)) &
        (position.getBRookBitboard() |
         position.getBDragonBitboard() |
         position.getWRookBitboard() |
         position.getWDragonBitboard());
  bb |= (MoveTables::diagR45(occR45, to) |
         MoveTables::diagL45(occL45, to)) &
        (position.getBBishopBitboard() |
         position.getBHorseBitboard() |
         position.getWBishopBitboard() |
         position.getWHorseBitboard());
  Bitboard king = position.getBDragonBitboard() | position.getWDragonBitboard();
  king.set(position.getBlackKingSquare());
  king.set(position.getWhiteKingSquare());
  bb |= MoveTables::king(to) & king;

  if (from.isValid()) {
    bb.unset(from);
  }

  return bb;
}

Bitboard SEE::extractShadowAggressor(const Position& position,
                                     Bitboard bb,
                                     Square from,
                                     Square to) {
  Direction dir = from.dir(to);
  if (dir >= Direction::EndS) {
    return bb;
  }

  Bitboard occ = position.getBOccupiedBitboard() | position.getWOccupiedBitboard();
  RotatedBitboard occ90 = position.get90RotatedBitboard();
  RotatedBitboard occR45 = position.getRight45RotatedBitboard();
  RotatedBitboard occL45 = position.getLeft45RotatedBitboard();

  Bitboard masked;
  switch (dir) {
  case Direction::Up:
  case Direction::Down:
    masked = MoveTables::ver(occ, from);
    break;
  case Direction::Left:
  case Direction::Right:
    masked = MoveTables::hor(occ90, from);
    break;
  case Direction::RightUp:
  case Direction::LeftDown:
    masked = MoveTables::diagR45(occR45, from);
    break;
  case Direction::LeftUp:
  case Direction::RightDown:
    masked = MoveTables::diagL45(occL45, from);
    break;
  default:
    ASSERT(false);
  }

  masked = bb.andNot(masked);
  BB_EACH(square, masked) {
    Piece piece = position.getPieceOnBoard(square);
    if (square.dir(from) == dir && MoveTables::isMovableInLongStep(piece, dir)) {
      bb.set(square);
      break;
    }
  }

  return bb;
}

Score SEE::search(const Position& position,
                  Bitboard bb,
                  Square to,
                  Score score,
                  Score materialScore) {
  Turn turn = position.getTurn() == Turn::Black ? Turn::White : Turn::Black;
  Score alpha = -Score::infinity();
  Score beta = Score::infinity();
  for (;;) {
    if (turn == Turn::Black) {
      alpha = std::max(alpha, score);

      Aggressor agr = pickAggressor<Turn::Black>(position, bb, to);

      if (agr.piece.isEmpty()) { return alpha; }

      score = score + materialScore + agr.prom;

      if (score <= alpha) { return alpha; }

      if (agr.piece == PieceType::king()) {
        auto agg = bb & position.getWOccupiedBitboard();
        if (agg.first() != 0 || agg.second() != 0) {
          return alpha;
        }

        return score;
      }

      bb = extractShadowAggressor(position, bb, agr.square, to);

      materialScore = agr.exch;
      turn = Turn::White;

    } else {
      beta = std::min(beta, score);

      Aggressor agr = pickAggressor<Turn::White>(position, bb, to);

      if (agr.piece.isEmpty()) { return beta; }

      score = score - materialScore - agr.prom;

      if (score >= beta) { return beta; }

      if (agr.piece == PieceType::king()) {
        auto agg = bb & position.getBOccupiedBitboard();
        if (agg.first() != 0 || agg.second() != 0) {
          return beta;
        }

        return score;
      }

      bb = extractShadowAggressor(position, bb, agr.square, to);

      materialScore = agr.exch;
      turn = Turn::Black;
    }
  }
}

template <Turn turn>
SEE::Aggressor SEE::pickAggressor(const Position& position,
                                  Bitboard& bb,
                                  Square to) {
  bool promotable = turn == Turn::Black ? to.isPromotable<Turn::Black>()
                                        : to.isPromotable<Turn::White>();
  auto masked = bb & (turn == Turn::Black ? position.getBPawnBitboard()
                                          : position.getWPawnBitboard());
  Square square = Square(masked.findForward());
  if (square.isValid()) {
    bb.unset(square);
    return {
      PieceType::pawn(),
      square,
      promotable ? material::tokin() - material::pawn() : Score::zero(),
      promotable ? material::tokinEx() : material::pawnEx()
    };
  }

  masked = bb & (turn == Turn::Black ? position.getBLanceBitboard()
                                     : position.getWLanceBitboard());
  square = Square(masked.findForward());
  if (square.isValid()) {
    bb.unset(square);
    return {
      PieceType::lance(),
      square,
      promotable ? material::proLance() - material::lance() : Score::zero(),
      promotable ? material::proLanceEx() : material::lanceEx()
    };
  }

  masked = bb & (turn == Turn::Black ? position.getBKnightBitboard()
                                     : position.getWKnightBitboard());
  square = Square(masked.findForward());
  if (square.isValid()) {
    bb.unset(square);
    return {
      PieceType::knight(),
      square,
      promotable ? material::proKnight() - material::knight() : Score::zero(),
      promotable ? material::proKnightEx() : material::knightEx()
    };
  }

  masked = bb & (turn == Turn::Black ? position.getBSilverBitboard()
                                     : position.getWSilverBitboard());
  square = Square(masked.findForward());
  if (square.isValid()) {
    bb.unset(square);
    return {
      PieceType::silver(),
      square,
      promotable ? material::proSilver() - material::silver() : Score::zero(),
      promotable ? material::proSilverEx() : material::silverEx()
    };
  }

  masked = bb & (turn == Turn::Black ? position.getBGoldBitboard()
                                     : position.getWGoldBitboard());
  square = Square(masked.findForward());
  if (square.isValid()) {
    bb.unset(square);
    return {
      PieceType::gold(),
      square,
      Score::zero(),
      material::exchangeScore(position.getPieceOnBoard(square))
    };
  }

  masked = bb & (turn == Turn::Black ? position.getBBishopBitboard()
                                     : position.getWBishopBitboard());
  square = Square(masked.findForward());
  if (square.isValid()) {
    bb.unset(square);
    return {
      PieceType::bishop(),
      square,
      promotable ? material::horse() - material::bishop() : Score::zero(),
      promotable ? material::horseEx() : material::bishopEx()
    };
  }

  masked = bb & (turn == Turn::Black ? position.getBRookBitboard()
                                     : position.getWRookBitboard());
  square = Square(masked.findForward());
  if (square.isValid()) {
    bb.unset(square);
    return {
      PieceType::rook(),
      square,
      promotable ? material::dragon() - material::rook() : Score::zero(),
      promotable ? material::dragonEx() : material::rookEx()
    };
  }

  masked = bb & (turn == Turn::Black ? position.getBHorseBitboard()
                                     : position.getWHorseBitboard());
  square = Square(masked.findForward());
  if (square.isValid()) {
    bb.unset(square);
    return {
      PieceType::horse(),
      square,
      Score::zero(),
      material::horseEx()
    };
  }

  masked = bb & (turn == Turn::Black ? position.getBDragonBitboard()
                                     : position.getWDragonBitboard());
  square = Square(masked.findForward());
  if (square.isValid()) {
    bb.unset(square);
    return {
      PieceType::dragon(),
      square,
      Score::zero(),
      material::dragonEx()
    };
  }

  square = turn == Turn::Black ? position.getBlackKingSquare()
                               : position.getWhiteKingSquare();
  if (bb.check(square)) {
    bb.unset(square);
    return {
      PieceType::king(),
      square,
      Score::zero(),
      0,
    };
  }

  return { PieceType::empty(), Square::invalid(), Score::zero(), Score::zero() };
}

} // namespace sunfish
