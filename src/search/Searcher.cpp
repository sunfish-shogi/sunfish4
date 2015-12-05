/* Searcher.cpp
 *
 * Kubo Ryosuke
 */

#include "search/Searcher.hpp"
#include "core/move/MoveGenerator.hpp"
#include "logger/Logger.hpp"

namespace sunfish {

Searcher::Searcher() {
}

bool Searcher::search(const Position& pos,
                      int depth,
                      Value alpha,
                      Value beta) {
  int ply = 0;
  auto& tree = treeOnMainThread_;
  auto& node = tree.nodes[ply];

  initializeTree(tree, pos);

  arrive(node);

  node.checkState = tree.position.getCheckState();

  if (!isChecking(node.checkState)) {
    MoveGenerator::generateCapturingMoves(tree.position, node.moves);
    MoveGenerator::generateNotCapturingMoves(tree.position, node.moves);
  } else {
    MoveGenerator::generateEvasions(tree.position, node.checkState, node.moves);
  }

  random_.shuffle(node.moves.begin(), node.moves.end());

  Move bestMove = Move::empty();

  for (Move& move : node.moves) {
    bool moveOk = tree.position.doMove(move);
    if (!moveOk) {
      continue;
    }

    Value value = -search(tree,
                          ply + 1,
                          depth - Depth1Ply,
                          -beta,
                          -alpha);

    tree.position.undoMove(move);

    if (value > alpha) {
      alpha = value;
      bestMove = move;

      if (value >= beta) {
        break;
      }
    }
  }

  bool hasBestMove = !bestMove.isEmpty();

  info_.move = bestMove;
  info_.value = alpha;
  if (hasBestMove) {
    info_.pv.set(bestMove, node.pv);
  } else {
    info_.pv.clear();
  }

  return hasBestMove;
}

Value Searcher::search(Tree& tree,
                       int ply,
                       int depth,
                       Value alpha,
                       Value beta) {
  Turn turn = tree.position.getTurn();

  if (depth <= 0 || ply >= Tree::StackSize) {
    Value standPat = evaluator_.evaluateMaterial(tree.position);
    return turn == Turn::Black ? standPat : -standPat;
  }

  auto& node = tree.nodes[ply];

  arrive(node);

  node.checkState = tree.position.getCheckState();

  if (!isChecking(node.checkState)) {
    MoveGenerator::generateCapturingMoves(tree.position, node.moves);
    MoveGenerator::generateNotCapturingMoves(tree.position, node.moves);
  } else {
    MoveGenerator::generateEvasions(tree.position, node.checkState, node.moves);
  }

  for (Move& move : node.moves) {
    bool moveOk = tree.position.doMove(move);
    if (!moveOk) {
      continue;
    }

    Value value = -search(tree,
                          ply + 1,
                          depth - Depth1Ply,
                          -beta,
                          -alpha);

    tree.position.undoMove(move);

    if (value > alpha) {
      alpha = value;

      auto& frontierNode = tree.nodes[ply-1];
      frontierNode.pv.set(move, node.pv);

      if (value >= beta) {
        break;
      }
    }
  }

  return alpha;
}

} // namespace sunfish
