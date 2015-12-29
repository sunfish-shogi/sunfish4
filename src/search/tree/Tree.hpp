/* Tree.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_SEARCH_TREE_TREE_HPP__
#define SUNFISH_SEARCH_TREE_TREE_HPP__

#include "common/Def.hpp"
#include "search/tree/Node.hpp"
#include "search/shek/ShekTable.hpp"
#include "search/shek/SCRDetector.hpp"
#include "core/position/Position.hpp"
#include <string>

namespace sunfish {

class Record;
class Evaluator;
struct Worker;

struct Tree {
  static CONSTEXPR_CONST int StackSize = 64;

  Position position;
  ShekTable shekTable;
  Worker* worker;
  int ply;
  Node nodes[StackSize];
  SCRDetector scr;
};

void initializeTree(Tree& tree,
                    const Position& position,
                    ClassifiedScores scores,
                    Worker* worker,
                    const Record* record);

inline bool hasKiller1(const Tree& tree) {
  auto& parentNode = tree.nodes[tree.ply-1];
  return !parentNode.killerMove1.isEmpty();
}

inline bool hasKiller2(const Tree& tree) {
  auto& parentNode = tree.nodes[tree.ply-1];
  return !parentNode.killerMove2.isEmpty();
}

inline bool isKiller1Good(const Tree& tree) {
  auto& parentNode = tree.nodes[tree.ply-1];
  return parentNode.killerCount1 >= 0;
}

inline bool isKiller2Good(const Tree& tree) {
  auto& parentNode = tree.nodes[tree.ply-1];
  return parentNode.killerCount2 >= 0;
}

inline bool isKiller1Legal(const Tree& tree) {
  auto& node = tree.nodes[tree.ply];
  auto& parentNode = tree.nodes[tree.ply-1];
  return tree.position.isLegalMoveMaybe(parentNode.killerMove1,
                                        node.checkState);
}

inline bool isKiller2Legal(const Tree& tree) {
  auto& node = tree.nodes[tree.ply];
  auto& parentNode = tree.nodes[tree.ply-1];
  return tree.position.isLegalMoveMaybe(parentNode.killerMove2,
                                        node.checkState);
}

inline bool isPriorMove(const Tree& tree, const Move& move) {
  auto& node = tree.nodes[tree.ply];
  auto& parentNode = tree.nodes[tree.ply-1];
  return move == node.hashMove ||
         (isKiller1Good(tree) &&
          move == parentNode.killerMove1) ||
         (isKiller2Good(tree) &&
          move == parentNode.killerMove2);
}

void addKiller(Tree& tree, Move move);

bool doMove(Tree& tree, Move& move, Evaluator& eval);

void undoMove(Tree& tree);

void doNullMove(Tree& tree);

void undoNullMove(Tree& tree);

inline Score calculateStandPat(Tree& tree) {
  auto& node = tree.nodes[tree.ply];
  if (tree.position.getTurn() == Turn::Black) {
    return calculateScore(node.scores);
  } else {
    return -calculateScore(node.scores);
  }
}

bool isImproving(const Tree& tree);

std::string getPath(const Tree& tree, int ply);

} // namespace sunfish

#endif // SUNFISH_SEARCH_TREE_TREE_HPP__
