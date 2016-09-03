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

void initializeShekTable(ShekTable& shekTable,
                         const Record* record) {
  shekTable.clear();

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
                    Evaluator& eval,
                    Worker* worker,
                    const Record* record) {
  tree.position = position;
  tree.worker = worker;
  tree.ply = 0;

  tree.nodes[0].materialScore = eval.calculateMaterialScore(tree.position);
  tree.nodes[0].score = Score::invalid();

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

  if (node.killerMove1.isNone()) {
    node.killerMove1 = move;
    node.killerCount1 = 0;
    node.killerCount2 -= 1;
  } else if (node.killerMove2.isNone()) {
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
  childNode.materialScore = eval.calculateMaterialScoreDiff(node.materialScore,
                                                            tree.position,
                                                            move,
                                                            node.captured);
  childNode.score = Score::invalid();

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

  node.move = Move::none();
  tree.ply++;

  auto& childNode = tree.nodes[tree.ply];
  childNode.materialScore = node.materialScore;
  childNode.score = node.score;
}

void undoNullMove(Tree& tree) {
  tree.ply--;
  tree.position.undoNullMove();
}

Score calculateStandPat(Tree& tree,
                        Evaluator& eval) {
  auto& node = tree.nodes[tree.ply];

  if (node.score == Score::invalid()) {
    node.score = eval.calculateTotalScore(node.materialScore,
                                          tree.position);
  }

  if (tree.position.getTurn() == Turn::Black) {
    return node.score;
  } else {
    return -node.score;
  }
}

Score estimateScore(Tree& tree,
                    const Move& move,
                    Evaluator& eval) {
  auto& node = tree.nodes[tree.ply];
  ASSERT(node.score != Score::invalid());
  Score score = eval.estimateScore(node.score,
                                   tree.position,
                                   move);
  if (tree.position.getTurn() == Turn::Black) {
    return score;
  } else {
    return -score;
  }
}

bool isImproving(Tree& tree,
                 Evaluator& eval) {
  if (tree.ply < 2) {
    return false;
  }

  auto& curr = tree.nodes[tree.ply];
  auto& front = tree.nodes[tree.ply-2];

  calculateStandPat(tree, eval);

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
