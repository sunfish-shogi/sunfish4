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
  Square from = move.from();
  Square to = move.to();
  Piece piece = position.getPieceOnBoard(from);
  Piece captured = position.getPieceOnBoard(to);

  ASSERT(!piece.isEmpty());
  ASSERT(position.getTurn() == Turn::Black ? piece.isBlack() : piece.isWhite());
  ASSERT(position.getTurn() == Turn::Black ? !captured.isBlack() : !captured.isWhite());

  Score score = Score::zero();

  if (move.isPromotion()) {
    score += material::promotionScore(piece);
    piece = piece.promote();
  }

  if (!captured.isEmpty()) {
    score += material::exchangeScore(captured);
  }

  auto bb = extractAttackers(position, from, to);

  if (position.getTurn() == Turn::Black) {
    return search(position, bb, to, score, material::exchangeScore(piece));
  } else {
    return -search(position, bb, to, -score, material::exchangeScore(piece));
  }
}

Bitboard SEE::extractAttackers(const Position& position,
                               Square from,
                               Square to) {
  Bitboard occ = position.getBOccupiedBitboard() | position.getWOccupiedBitboard();
  RotatedBitboard occ90 = position.get90RotatedBitboard();
  RotatedBitboard occR45 = position.getRight45RotatedBitboard();
  RotatedBitboard occL45 = position.getLeft45RotatedBitboard();

  occ.unset(from);
  occ90.unset(from.rotate90());
  occR45.unset(from.rotateRight45());
  occL45.unset(from.rotateLeft45());

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
  bb |= MoveTables::king(to) &
       (position.getBDragonBitboard() |
        position.getWDragonBitboard()); // TODO: king

  bb.unset(from);

  return bb;
}

Bitboard SEE::extractShadowAttacker(const Position& position,
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
    masked = MoveTables::diagR45(occR45, to);
    break;
  case Direction::LeftUp:
  case Direction::RightDown:
    masked = MoveTables::diagL45(occL45, to);
    break;
  default:
    ASSERT(false);
  }

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

      Attacker atk = pickAttacker<Turn::Black>(position, bb, to);

      if (atk.piece.isEmpty()) { return alpha; }

      score = score + materialScore + atk.prom;

      if (score <= alpha) { return alpha; }

      bb = extractShadowAttacker(position, bb, atk.square, to);

      materialScore = atk.exch;
      turn = Turn::White;

    } else {
      beta = std::min(beta, score);

      Attacker atk = pickAttacker<Turn::White>(position, bb, to);

      if (atk.piece.isEmpty()) { return beta; }

      score = score - materialScore - atk.prom;

      if (score >= beta) { return beta; }

      bb = extractShadowAttacker(position, bb, atk.square, to);

      materialScore = atk.exch;
      turn = Turn::Black;
    }
  }
}

template <Turn turn>
SEE::Attacker SEE::pickAttacker(const Position& position,
                                Bitboard& bb,
                                Square to) {
  bool promotable = to.isPromotable<Turn::Black>();

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

  return { PieceType::empty(), Square::invalid(), Score::zero(), Score::zero() };
}

void SEE::sortMoves(const Position& position,
                    Moves& moves,
                    Moves::iterator begin,
                    bool excludeNegative) {
  for (auto ite = begin; ite != moves.end(); ) {
    auto& move = *ite;

    Score score = calculate(position, move);

    if (excludeNegative && score < Score::zero()) {
      ite = moves.remove(ite);
      continue;
    }

    move.setExtData(static_cast<Move::RawType16>(score.raw()));

    ite++;
  }

  std::sort(begin, moves.end(), [](const Move& lhs, const Move& rhs) {
    return moveToScore(lhs) > moveToScore(rhs);
  });
}

} // namespace sunfish
