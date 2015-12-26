/* Tree.cpp
 *
 * Kubo Ryosuke
 */

#include "search/tree/Tree.hpp"
#include "search/eval/Evaluator.hpp"
#include "core/record/Record.hpp"
#include "logger/Logger.hpp"
#include <sstream>

namespace {

using namespace sunfish;

void initializeShekTable(ShekTable& shekTable, const Record* record) {
  shekTable.initialize();

  if (record == nullptr) {
    return;
  }

  Position pos = record->initialPosition;
  for (auto& move : record->moveList) {
    shekTable.retain(pos);

    Piece captured;
    if (!pos.doMove(move, captured)) {
      LOG(error) << "illegal move: " << move.toString();
      return;
    }
  }
}

} // namespace

namespace sunfish {

void initializeTree(Tree& tree,
                    const Position& position,
                    Score score,
                    Worker* worker,
                    const Record* record) {
  tree.position = position;
  tree.worker = worker;
  tree.ply = 0;
  tree.nodes[0].score = score;

  // SHEK
  initializeShekTable(tree.shekTable, record);

  // successive checks repetition detector
  if (record != nullptr) {
    tree.scr.registerRecord(*record);
  } else {
    tree.scr.clear();
  }
}

bool doMove(Tree& tree, Move& move, Evaluator& eval) {
  auto& node = tree.nodes[tree.ply];
  node.hash = tree.position.getHash();
  tree.shekTable.retain(tree.position);

  if (!tree.position.doMove(move, node.captured)) {
    tree.shekTable.release(tree.position);
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
  tree.shekTable.release(tree.position);
}

void doNullMove(Tree& tree) {
  auto& node = tree.nodes[tree.ply];
  node.hash = tree.position.getHash();

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

bool isImproving(const Tree& tree) {
  if (tree.ply < 2) {
    return false;
  }

  auto& curr = tree.nodes[tree.ply];
  auto& front = tree.nodes[tree.ply-2];

  if (tree.position.getTurn() == Turn::Black) {
    return  curr.score >= front.score;
  } else {
    return  curr.score <= front.score;
  }
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
