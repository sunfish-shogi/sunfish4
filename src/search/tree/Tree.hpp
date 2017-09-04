/* Tree.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_SEARCH_TREE_TREE_HPP__
#define SUNFISH_SEARCH_TREE_TREE_HPP__

#include "common/Def.hpp"
#include "search/tree/PV.hpp"
#include "search/eval/Evaluator.hpp"
#include "search/shek/ShekTable.hpp"
#include "search/shek/SCRDetector.hpp"
#include "search/SearchInfo.hpp"
#include "search/tree/NodeStat.hpp"
#include "core/move/Moves.hpp"
#include "core/position/Position.hpp"
#include <string>
#include <thread>
#include <cstdint>

namespace sunfish {

struct Record;
class Evaluator;
class TT;

namespace GenPhase_ {

enum Type : uint16_t {
  Init,
  Captures,
  Killers,
  Quiets,
  BadCaptures,
  InitEvasions,
  Evasions,
  InitQuies,
  Quies,
  InitQuies2,
  Quies2,
  InitProb,
  ProbCaptures,
  InitRoot,
  End,
};

} // namespace GenPhase_

using GenPhase = GenPhase_::Type;

struct Node {
  Zobrist::Type hash;
  Score materialScore;
  Score score;
  CheckState checkState;
  bool isHistorical;

  Piece captured;
  Move move;
  Move ttMove;
  Move excludedMove;

  Move killerMove1;
  Move killerMove2;
  int16_t killerCount1;
  int16_t killerCount2;

  uint16_t genPhase;
  Score probThreshold;
  Moves::iterator moveIterator;
  Moves::iterator badCaptureEnd;
  Moves moves;
  MoveArray<128> quietsSearched;

  PV pv;
};

struct Tree {
  static CONSTEXPR_CONST int StackSize = 64;

  std::thread thread;
  int index;
  int completedDepth;
  Position position;
  ShekTable shekTable;
  SearchInfo info;
  int ply;
  Node nodes[StackSize];
  SCRDetector scr;
};

void initializeTree(Tree& tree,
                    const Position& position,
                    Evaluator& eval,
                    const Record* record);

void visit(Tree& tree, NodeStat nodeStat);

void revisit(Tree& tree, NodeStat nodeStat);

inline
bool hasKiller1(const Tree& tree) {
  auto& node = tree.nodes[tree.ply];
  return !node.killerMove1.isNone();
}

inline
bool hasKiller2(const Tree& tree) {
  auto& node = tree.nodes[tree.ply];
  return !node.killerMove2.isNone();
}

inline
bool isKiller1Good(const Tree& tree) {
  auto& node = tree.nodes[tree.ply];
  return node.killerCount1 >= 0;
}

inline
bool isKiller2Good(const Tree& tree) {
  auto& node = tree.nodes[tree.ply];
  return node.killerCount2 >= 0;
}

inline
bool isKiller1Legal(const Tree& tree) {
  auto& node = tree.nodes[tree.ply];
  return tree.position.validateMove(node.killerMove1,
                                        node.checkState);
}

inline
bool isKiller2Legal(const Tree& tree) {
  auto& node = tree.nodes[tree.ply];
  return tree.position.validateMove(node.killerMove2,
                                        node.checkState);
}

inline
bool isPriorMove(const Tree& tree,
                 const Move& move) {
  auto& node = tree.nodes[tree.ply];
  return move == node.ttMove ||
         (isKiller1Good(tree) &&
          move == node.killerMove1) ||
         (isKiller2Good(tree) &&
          move == node.killerMove2);
}

void addKiller(Tree& tree, Move move);

bool doMove(Tree& tree, Move& move, Evaluator& eval, TT& tt);

void undoMove(Tree& tree);

void doNullMove(Tree& tree, TT& tt);

void undoNullMove(Tree& tree);

Score calculateStandPat(Tree& tree,
                        Evaluator& eval);

Score estimateScore(Tree& tree,
                    const Move& move,
                    Evaluator& eval);

inline
bool isRecapture(const Tree& tree,
                 const Move& move) {
  if (tree.ply == 0) {
    return false;
  }

  auto& frontNode = tree.nodes[tree.ply-1];
  if (frontNode.move.to() != move.to()) {
    return false;
  }

  if (!frontNode.captured.isEmpty()) {
    return true;
  }

  Piece piece = tree.position.getPieceOnBoard(frontNode.move.to());
  return (frontNode.move.isPromotion() &&
          piece.type() != PieceType::proSilver());
}

inline
Piece targetPiece(Tree& tree,
                         const Move& move) {
  if (!move.isDrop()) {
    return tree.position.getPieceOnBoard(move.from());
  } else if (tree.position.getTurn() == Turn::Black) {
    return move.droppingPieceType().black();
  } else {
    return move.droppingPieceType().white();
  }
}

std::string getPath(const Tree& tree, int ply);

} // namespace sunfish

#endif // SUNFISH_SEARCH_TREE_TREE_HPP__
