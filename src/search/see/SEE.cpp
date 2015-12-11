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

  Score scores[MAX_NUMBER_OF_MOVES];

  for (Moves::size_type i = 0; i < moves.size(); i++) {
    Move move = moves[i];
    Score score = calculate(tree, move);

    // insertion
    int j;
    for (j = i - 1; j >= 0; j--) {
      if (scores[j] >= score) {
        break;
      }

      moves[j+1] = moves[j];
      scores[j+1] = scores[j];
    }
    moves[j+1] = move;
    scores[j+1] = score;
  }
}

} // namespace sunfish
