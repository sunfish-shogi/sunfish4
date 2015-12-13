/* SEE.cpp
 *
 * Kubo Ryosuke
 */

#include "search/see/SEE.hpp"
#include "search/eval/Material.hpp"

namespace sunfish {

Score SEE::calculate(const Tree& tree, Move move) {
  Score score = Score::zero();

  auto& pos = tree.position;
  auto captured = pos.getPieceOnBoard(move.to());

  score += material::pieceExchangeScore(captured);

  // TODO

  return score;
}

void SEE::sortMoves(Tree& tree) {
  auto& node = tree.nodes[tree.ply];
  auto& moves = node.moves;

  for (Moves::size_type i = 0; i < moves.size(); i++) {
    Move move = moves[i];
    Score score = calculate(tree, move);

    moves[i] = move;
    moves[i].setExtData(static_cast<Move::RawType16>(score.raw()));
  }

  std::sort(moves.begin(), moves.end(), [](const Move& lhs, const Move& rhs) {
    return moveToScore(lhs) > moveToScore(rhs);
  });
}

} // namespace sunfish
