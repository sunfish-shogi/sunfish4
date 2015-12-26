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
  Score score;
  CheckState checkState;
  bool isHistorical;

  Piece captured;
  Move move;
  Move hashMove;

  GenPhase genPhase;
  Moves::iterator moveIterator;
  Moves moves;

  PV pv;
};

void arrive(Node& node);

inline bool isPriorMove(const Node& node, const Move& move) {
  return move != node.hashMove;
}

} // namespace sunfish

#endif // SUNFISH_SEARCH_TREE_NODE_HPP__
