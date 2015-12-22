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
  ASSERT(!captured.isEmpty());
  ASSERT(position.getTurn() ? piece.isBlack() : piece.isWhite());
  ASSERT(position.getTurn() ? captured.isWhite() : captured.isBlack());

  Score score = Score::zero();
  if (move.isPromotion()) {
    score += material::promotionScore(piece);
    piece = piece.promote();
  }
  score += material::exchangeScore(captured);

  Attackers ba;
  Attackers wa;
  std::tie(ba, wa) = generateAttackers(position, from, to);

  if (position.getTurn() == Turn::Black) {
    return -search(wa, ba, 0, 0, -score, material::exchangeScore(piece));
  } else {
    return -search(ba, wa, 0, 0, -score, material::exchangeScore(piece));
  }
}

SEE::AttackerSet SEE::generateAttackers(const Position& position,
                                        Square from,
                                        Square to) {
  Attackers ba;
  Attackers wa;
  ba.num = 0;
  wa.num = 0;

  Bitboard occ = nosseOr(position.getBOccupiedBitboard(),
                         position.getWOccupiedBitboard());
  RotatedBitboard occ90 = position.get90RotatedBitboard();
  RotatedBitboard occR45 = position.getRight45RotatedBitboard();
  RotatedBitboard occL45 = position.getLeft45RotatedBitboard();

  occ.unset(from);
  occ90.unset(from.rotate90());
  occR45.unset(from.rotateRight45());
  occL45.unset(from.rotateLeft45());

  Bitboard bb = MoveTables::ver(occ, to) & occ;
  bb |= MoveTables::hor(occ90, to) & occ;
  bb |= MoveTables::diagR45(occR45, to) & occ;
  bb |= MoveTables::diagL45(occL45, to) & occ;
  BB_EACH(square, bb) {
    Piece piece = position.getPieceOnBoard(square);
    Direction dir = square.dir(to);
    ASSERT(!piece.isEmpty());
    if (MoveTables::isMovableInLongStep(piece, dir) ||
        (square.move(dir) == to && MoveTables::isMovableInOneStep(piece, dir))) {
      if (piece.isBlack()) {
        ba.scores[ba.num++] = material::exchangeScore(piece);
      } else {
        wa.scores[wa.num++] = material::exchangeScore(piece);
      }
    }
  }

  Square square = to.safetyLeftDownKnight();
  if (square.isValid() &&
      position.getPieceOnBoard(square) == Piece::blackKnight()) {
    ba.scores[ba.num++] = material::KnightEx;
  }

  square = to.safetyRightDownKnight();
  if (square.isValid() &&
      position.getPieceOnBoard(square) == Piece::blackKnight()) {
    ba.scores[ba.num++] = material::KnightEx;
  }

  square = to.safetyLeftUpKnight();
  if (square.isValid() &&
      position.getPieceOnBoard(square) == Piece::whiteKnight()) {
    wa.scores[wa.num++] = material::KnightEx;
  }

  square = to.safetyRightUpKnight();
  if (square.isValid() &&
      position.getPieceOnBoard(square) == Piece::whiteKnight()) {
    wa.scores[wa.num++] = material::KnightEx;
  }

  std::sort(ba.scores, ba.scores + ba.num);
  std::sort(wa.scores, wa.scores + wa.num);

  return std::make_tuple(ba, wa);
}

Score SEE::search(const Attackers& ba,
                  const Attackers& wa,
                  int b,
                  int w,
                  Score score,
                  Score materialScore) {
  if (b == ba.num) {
    return score;
  }

  Score newScore = score + materialScore;
  return std::max(score, -search(wa, ba, w, b + 1, -newScore, ba.scores[b]));
}

void SEE::sortMoves(const Position& position,
                    Moves& moves,
                    Moves::iterator begin,
                    bool excludeSmallCaptures) {
  for (auto ite = begin; ite != moves.end(); ) {
    Move move = *ite;

    if (excludeSmallCaptures) {
      auto piece = position.getPieceOnBoard(move.from());
      auto captured = position.getPieceOnBoard(move.to());
      if ((captured.type() == PieceType::pawn() && !move.isPromotion()) ||
          (captured.isEmpty() && piece.type() != PieceType::pawn())) {
        ite = moves.remove(ite);
        continue;
      }
    }

    Score score = calculate(position, move);
    *ite = move;
    ite->setExtData(static_cast<Move::RawType16>(score.raw()));

    ite++;
  }

  std::sort(begin, moves.end(), [](const Move& lhs, const Move& rhs) {
    return moveToScore(lhs) > moveToScore(rhs);
  });
}

} // namespace sunfish
