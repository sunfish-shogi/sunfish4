/* Tree.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_SEARCH_TREE_TREE_HPP__
#define SUNFISH_SEARCH_TREE_TREE_HPP__

#include "common/Def.hpp"
#include "search/tree/Node.hpp"
#include "core/position/Position.hpp"

namespace sunfish {

struct Tree {
  static CONSTEXPR_CONST int StackSize = 64;

  Position position;
  Node nodes[StackSize];
};

void initializeTree(Tree& tree, const Position& position);

} // namespace sunfish

#endif // SUNFISH_SEARCH_TREE_TREE_HPP__
