/* Tree.cpp
 *
 * Kubo Ryosuke
 */

#include "search/tree/Tree.hpp"

namespace sunfish {

void initializeTree(Tree& tree,
                    const Position& position) {
  tree.position = position;
}

} // namespace sunfish
