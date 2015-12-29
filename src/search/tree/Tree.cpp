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
                    ClassifiedScores scores,
                    Worker* worker,
                    const Record* record) {
  tree.position = position;
  tree.worker = worker;
  tree.ply = 0;
  tree.nodes[0].scores = scores;

  // SHEK
  initializeShekTable(tree.shekTable, record);

  // successive checks repetition detector
  if (record != nullptr) {
    tree.scr.registerRecord(*record);
  } else {
    tree.scr.clear();
  }
}

void sortKiller(Node& node) {
  if (node.killerCount2 > node.killerCount1) {
    std::swap(node.killerMove1, node.killerMove2);
    std::swap(node.killerCount1, node.killerCount2);
  }
}

void addKiller(Tree& tree, Move move) {
  auto& node = tree.nodes[tree.ply-1];

  if (node.killerMove1.isEmpty()) {
    node.killerMove1 = move;
    node.killerCount1 = 0;
    node.killerCount2 -= 1;
  } else if (node.killerMove2.isEmpty()) {
    node.killerMove2 = move;
    node.killerCount2 = 0;
    node.killerCount1 -= 1;
    sortKiller(node);
  } else if (move == node.killerMove1) {
    node.killerCount1 += 2;
    node.killerCount2 -= 1;
  } else if (move == node.killerMove2) {
    node.killerCount2 += 2;
    node.killerCount1 -= 1;
    sortKiller(node);
  } else if (node.killerCount2 < 0) {
    node.killerMove2 = move;
    node.killerCount2 = 0;
    node.killerCount1 -= 1;
    sortKiller(node);
  } else if (node.killerCount1 < 0) {
    node.killerMove1 = move;
    node.killerCount1 = 0;
    node.killerCount2 -= 1;
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
  childNode.scores = eval.evaluateDiff(node.scores, tree.position, move, node.captured);

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
  childNode.scores = node.scores;
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

  auto currScore = calculateScore(curr.scores);
  auto frontScore = calculateScore(front.scores);
  if (tree.position.getTurn() == Turn::Black) {
    return  currScore >= frontScore;
  } else {
    return  currScore <= frontScore;
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
