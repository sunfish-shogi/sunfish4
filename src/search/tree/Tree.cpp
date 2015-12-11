/* Tree.cpp
 *
 * Kubo Ryosuke
 */

#include "search/tree/Tree.hpp"
#include <sstream>

namespace sunfish {

void initializeTree(Tree& tree,
                    const Position& position,
                    Worker* worker) {
  tree.position = position;
  tree.ply = 0;
  tree.worker = worker;
}

bool doMove(Tree& tree, Move& move) {
  if (tree.position.doMove(move)) {
    tree.nodes[tree.ply].move = move;
    tree.ply++;
    return true;
  }
  return false;
}

void undoMove(Tree& tree, Move move) {
  tree.position.undoMove(move);
  tree.ply--;
}

std::string getPath(const Tree& tree, int ply) {
  std::ostringstream oss;

  for (int i = 0; i < ply; i++) {
    if (i != 0) {
      oss << ' ';
    }

    oss << tree.nodes[i].move.toString();
  }

  return oss.str();
}

} // namespace sunfish
