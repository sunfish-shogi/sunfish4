/* Node.cpp
 *
 * Kubo Ryosuke
 */

#include "search/tree/Node.hpp"

namespace sunfish {

void arrive(Node& node) {
  node.pv.clear();
  node.moves.clear();
  node.moveIterator = node.moves.begin();
}

} // namespace sunfish
