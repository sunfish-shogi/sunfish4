/* SEE.cpp
 *
 * Kubo Ryosuke
 */

#include "search/see/SEE.hpp"
#include "search/eval/Material.hpp"

namespace sunfish {

Score SEE::calculate(const Position& position,
                     Move move) {
  Score score = Score::zero();

  auto captured = position.getPieceOnBoard(move.to());

  score += material::exchangeScore(captured);

  // TODO

  return score;
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
