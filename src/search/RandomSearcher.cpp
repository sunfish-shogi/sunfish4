/* RandomSearcher.cpp
 * 
 * Kubo Ryosuke
 */

#include "RandomSearcher.hpp"
#include "core/position/Position.hpp"
#include "core/move/Move.hpp"
#include "core/move/Moves.hpp"
#include "core/move/MoveGenerator.hpp"

namespace sunfish {

RandomSearcher::RandomSearcher() {
}

bool RandomSearcher::search(const Position& pos, Move& move) {
  Moves moves;

  auto cs = pos.getCheckState();

  if (!cs.from1.isValid()) {
    MoveGenerator::generateCapturingMoves(pos, moves);
    MoveGenerator::generateNotCapturingMoves(pos, moves);
  } else {
    MoveGenerator::generateEvasions(pos, cs, moves);
  }

  for (auto ite = moves.begin(); ite != moves.end(); ) {
    if (Position(pos).doMove(*ite)) {
      ite++;
    } else {
      ite = moves.remove(ite);
    }
  }

  if (moves.size() == 0) {
    return false;
  }

  auto index = random_.getInt16(moves.size());

  move = moves[index];

  return true;
}

} // namespace sunfish
