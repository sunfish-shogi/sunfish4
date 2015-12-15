/* Node.cpp
 *
 * Kubo Ryosuke
 */

#include "search/tree/Node.hpp"

namespace sunfish {

void arrive(Node& node) {
  node.hashMove = Move::empty();
  node.moves.clear();
  node.moveIterator = node.moves.begin();
  node.pv.clear();
}

} // namespace sunfish
