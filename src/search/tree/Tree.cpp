/* Tree.cpp
 *
 * Kubo Ryosuke
 */

#include "search/tree/Tree.hpp"
#include "search/eval/Evaluator.hpp"
#include "logger/Logger.hpp"
#include <sstream>

namespace sunfish {

void initializeTree(Tree& tree,
                    const Position& position,
                    Score score,
                    Worker* worker) {
  tree.position = position;
  tree.ply = 0;
  tree.worker = worker;
  tree.nodes[0].score = score;
}

bool doMove(Tree& tree, Move& move, Evaluator& eval) {
  auto& node = tree.nodes[tree.ply];
  if (!tree.position.doMove(move, node.captured)) {
    return false;
  }

  node.move = move;
  tree.ply++;

  auto& childNode = tree.nodes[tree.ply];
  childNode.score = node.score + eval.evaluateDiff(tree.position, move, node.captured);

  return true;

}

void undoMove(Tree& tree) {
  tree.ply--;
  auto& node = tree.nodes[tree.ply];
  tree.position.undoMove(node.move, node.captured);
}

void doNullMove(Tree& tree) {
  auto& node = tree.nodes[tree.ply];
  tree.position.doNullMove();
  node.move = Move::empty();
  tree.ply++;

  auto& childNode = tree.nodes[tree.ply];
  childNode.score = node.score;
}

void undoNullMove(Tree& tree) {
  tree.ply--;
  tree.position.undoNullMove();
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
