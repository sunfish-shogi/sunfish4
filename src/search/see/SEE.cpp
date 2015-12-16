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

  auto& pos = position;
  auto captured = pos.getPieceOnBoard(move.to());

  score += material::exchangeScore(captured);

  // TODO

  return score;
}

void SEE::sortMoves(const Position& position,
                    Moves::iterator begin,
                    Moves::iterator end) {
  for (auto ite = begin; ite != end; ite++) {
    Move move = *ite;
    Score score = calculate(position, move);
    *ite = move;
    ite->setExtData(static_cast<Move::RawType16>(score.raw()));
  }

  std::sort(begin, end, [](const Move& lhs, const Move& rhs) {
    return moveToScore(lhs) > moveToScore(rhs);
  });
}

} // namespace sunfish
