/* Tree.cpp
 *
 * Kubo Ryosuke
 */

#include "search/tree/Tree.hpp"
#include "search/tt/TT.hpp"
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
    shekTable.retain(pos, false);

    Piece captured;
    if (!pos.doMove(move, captured)) {
      LOG(error) << "illegal move: " << move.toString();
      return;
    }
  }
}

} // namespace

namespace sunfish {

void insertRootPV(std::list<RootPV>& rootPVs, Move move, int depth, const PV& pv, Score score, int capacity) {
  auto ite = rootPVs.begin();
  for (; ite != rootPVs.end(); ite++) {
    if (ite->score < score) {
      break;
    }
  }
  ite = rootPVs.insert(ite, RootPV{});
  ite->pv.set(move, depth, pv);
  ite->score = score;
  if (rootPVs.size() > capacity) {
    rootPVs.pop_back();
  }
}

void initializeTree(Tree& tree,
                    const Position& position,
                    Evaluator& eval,
                    const Record* record) {
  tree.position = position;
  tree.ply = 0;

  initializeSearchInfo(tree.info);

  tree.nodes[0].materialScore = eval.calculateMaterialScore(tree.position);
  tree.nodes[0].score = Score::invalid();
  tree.nodes[0].killerMove1 = Move::none();
  tree.nodes[0].killerMove2 = Move::none();
  tree.nodes[0].excludedMove = Move::none();

  // SHEK
  initializeShekTable(tree.shekTable, record);

  // successive checks repetition detector
  if (record != nullptr) {
    tree.scr.registerRecord(*record);
  } else {
    tree.scr.clear();
  }
}

template <bool root>
void visit(Tree& tree) {
  ASSERT(tree.ply <= Tree::StackSize - 2);

  Node& node = tree.nodes[tree.ply];
  node.isHistorical = false;
  node.ttMove = Move::none();
  node.quietsSearched.clear();
  if (!root) {
    node.pv.clear();
  }

  Node& childNode = tree.nodes[tree.ply+1];
  childNode.killerMove1 = Move::none();
  childNode.killerMove2 = Move::none();
  childNode.excludedMove = Move::none();
}
template void visit<true>(Tree& tree);
template void visit<false>(Tree& tree);

template <bool root>
void revisit(Tree& tree) {
  ASSERT(tree.ply <= Tree::StackSize - 2);

  Node& node = tree.nodes[tree.ply];
  node.isHistorical = false;
  node.quietsSearched.clear();
  if (!root) {
    node.pv.clear();
  }

  Node& childNode = tree.nodes[tree.ply+1];
  childNode.killerMove1 = Move::none();
  childNode.killerMove2 = Move::none();
  childNode.excludedMove = Move::none();
}
template void revisit<true>(Tree& tree);
template void revisit<false>(Tree& tree);

void sortKiller(Node& node) {
  if (node.killerCount2 > node.killerCount1) {
    std::swap(node.killerMove1, node.killerMove2);
    std::swap(node.killerCount1, node.killerCount2);
  }
}

void addKiller(Tree& tree, Move move) {
  auto& node = tree.nodes[tree.ply];

  if (node.killerMove1.isNone()) {
    node.killerMove1 = move;
    node.killerCount1 = 4;
    node.killerCount2 -= 1;
  } else if (node.killerMove2.isNone()) {
    node.killerMove2 = move;
    node.killerCount2 = 4;
    node.killerCount1 -= 1;
    sortKiller(node);
  } else if (move == node.killerMove1) {
    node.killerCount1 += 1;
    node.killerCount2 -= 1;
  } else if (move == node.killerMove2) {
    node.killerCount2 += 1;
    node.killerCount1 -= 1;
    sortKiller(node);
  } else if (node.killerCount2 < 0) {
    node.killerMove2 = move;
    node.killerCount2 = 4;
    node.killerCount1 -= 1;
    sortKiller(node);
  }
}

template <bool shek>
bool doMove(Tree& tree, Move& move, Evaluator& eval, TT& tt) {
  auto& node = tree.nodes[tree.ply];
  node.hash = tree.position.getHash();
  if (shek) {
    tree.shekTable.retain(tree.position, true);
  }

  if (!tree.position.doMove(move, node.captured)) {
    if (shek) {
      tree.shekTable.release(tree.position);
    }
    return false;
  }

  tt.prefetch(tree.position.getHash());

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
template bool doMove<true>(Tree& tree, Move& move, Evaluator& eval, TT& tt);
template bool doMove<false>(Tree& tree, Move& move, Evaluator& eval, TT& tt);

template <bool shek>
void undoMove(Tree& tree) {
  ASSERT(tree.ply > 0);
  tree.ply--;
  auto& node = tree.nodes[tree.ply];
  tree.position.undoMove(node.move, node.captured);
  if (shek) {
    tree.shekTable.release(tree.position);
  }
}
template void undoMove<true>(Tree& tree);
template void undoMove<false>(Tree& tree);

void doNullMove(Tree& tree, TT& tt) {
  auto& node = tree.nodes[tree.ply];
  node.hash = tree.position.getHash();

  tree.position.doNullMove();

  tt.prefetch(tree.position.getHash());

  node.move = Move::none();
  tree.ply++;

  auto& childNode = tree.nodes[tree.ply];
  childNode.materialScore = node.materialScore;
  childNode.score = node.score;
}

void undoNullMove(Tree& tree) {
  ASSERT(tree.ply > 0);
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
