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
                    Score score,
                    Worker* worker,
                    const Record* record);

bool doMove(Tree& tree, Move& move, Evaluator& eval);

void undoMove(Tree& tree);

void doNullMove(Tree& tree);

void undoNullMove(Tree& tree);

bool isImproving(const Tree& tree);

std::string getPath(const Tree& tree, int ply);

} // namespace sunfish

#endif // SUNFISH_SEARCH_TREE_TREE_HPP__
