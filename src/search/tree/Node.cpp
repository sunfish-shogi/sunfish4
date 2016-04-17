/* Node.cpp
 *
 * Kubo Ryosuke
 */

#include "search/tree/Node.hpp"

namespace sunfish {

void arrive(Node& node) {
  node.isHistorical = false;

  node.hashMove = Move::none();

  node.killerMove1 = Move::none();
  node.killerMove2 = Move::none();

  node.pv.clear();
}

} // namespace sunfish
