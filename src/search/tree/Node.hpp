/* Node.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_SEARCH_TREE_NODE_HPP__
#define SUNFISH_SEARCH_TREE_NODE_HPP__

#include "search/tree/PV.hpp"
#include "search/eval/Score.hpp"
#include "core/move/Moves.hpp"
#include "core/position/Position.hpp"

namespace sunfish {

enum GenPhase {
  CapturingMoves,
  NotCapturingMoves,
  Evasions,
  End,
};

struct Node {
  CheckState checkState;
  Score standPat;

  Move move;
  Piece captured;
  Move hashMove;

  GenPhase genPhase;
  Moves::iterator moveIterator;
  Moves moves;

  PV pv;
};

void arrive(Node& node);

} // namespace sunfish

#endif // SUNFISH_SEARCH_TREE_NODE_HPP__
