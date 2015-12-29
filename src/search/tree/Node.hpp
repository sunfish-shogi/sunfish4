/* Node.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_SEARCH_TREE_NODE_HPP__
#define SUNFISH_SEARCH_TREE_NODE_HPP__

#include "search/tree/PV.hpp"
#include "search/eval/Evaluator.hpp"
#include "core/move/Moves.hpp"
#include "core/position/Position.hpp"
#include <cstdint>

namespace sunfish {

enum GenPhase : uint8_t {
  CapturingMoves,
  NotCapturingMoves,
  Evasions,
  End,
};

struct Node {
  Zobrist::Type hash;
  ClassifiedScores scores;
  CheckState checkState;
  bool isHistorical;

  Piece captured;
  Move move;
  Move hashMove;

  Move killerMove1;
  Move killerMove2;
  int16_t killerCount1;
  int16_t killerCount2;

  GenPhase genPhase;
  Moves::iterator moveIterator;
  Moves moves;

  PV pv;
};

void arrive(Node& node);

} // namespace sunfish

#endif // SUNFISH_SEARCH_TREE_NODE_HPP__
