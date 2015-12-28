/* Node.cpp
 *
 * Kubo Ryosuke
 */

#include "search/tree/Node.hpp"

namespace sunfish {

void arrive(Node& node) {
  node.isHistorical = false;

  node.hashMove = Move::empty();

  node.killerMove1 = Move::empty();
  node.killerMove2 = Move::empty();

  node.moveIterator = node.moves.begin();
  node.moves.clear();

  node.pv.clear();
}

} // namespace sunfish
