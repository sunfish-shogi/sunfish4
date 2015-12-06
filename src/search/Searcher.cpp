/* Searcher.cpp
 *
 * Kubo Ryosuke
 */

#include "search/Searcher.hpp"
#include "core/move/MoveGenerator.hpp"
#include "logger/Logger.hpp"
#include <algorithm>

namespace sunfish {

Searcher::Searcher() {
}

bool Searcher::search(const Position& pos,
                      int depth,
                      Value alpha,
                      Value beta) {
  auto& tree = treeOnMainThread_;
  initializeTree(tree, pos);

  auto& node = tree.nodes[tree.ply];
  arrive(node);

  node.checkState = tree.position.getCheckState();

  // generate moves
  if (!isChecking(node.checkState)) {
    MoveGenerator::generateCapturingMoves(tree.position, node.moves);
    MoveGenerator::generateNotCapturingMoves(tree.position, node.moves);
  } else {
    MoveGenerator::generateEvasions(tree.position, node.checkState, node.moves);
  }

  random_.shuffle(node.moves.begin(), node.moves.end());

  Move bestMove = Move::empty();
  PV pv;

  // expand the branches
  for (;;) {
    Move move = nextMove(node);
    if (move.isEmpty()) {
      break;
    }

    bool moveOk = doMove(tree, move);
    if (!moveOk) {
      continue;
    }

    Value value = -search(tree,
                          depth - Depth1Ply,
                          -beta,
                          -alpha);

    undoMove(tree, move);

    if (value > alpha) {
      alpha = value;
      bestMove = move;
      pv.set(move, node.pv);

      if (value >= beta) {
        break;
      }
    }
  }

  bool hasBestMove = !bestMove.isEmpty();

  info_.move = bestMove;
  info_.value = alpha;
  info_.pv = pv;

  return hasBestMove;
}

Value Searcher::search(Tree& tree,
                       int depth,
                       Value alpha,
                       Value beta) {
#if 0
  bool isDebug = false;
  if (getPath(tree, ply) == "-0068KA +5968OU -9394FU") {
    isDebug = true;
  }
#endif

  Turn turn = tree.position.getTurn();

  // quiesence search
  if (depth <= 0) {
    return quies(tree,
                 alpha,
                 beta);
  }

  // static evaluation
  if (tree.ply >= Tree::StackSize) {
    Value standPat = evaluator_.evaluateMaterial(tree.position);
    return turn == Turn::Black ? standPat : -standPat;
  }

  // distance pruning
  {
    Value lowerValue = -Value::infinity() + tree.ply;
    Value upperValue = Value::infinity() - tree.ply - 1;
    if (lowerValue >= beta) {
      return lowerValue;
    } else if (lowerValue > alpha) {
      alpha = lowerValue;
    } else if (upperValue <= alpha) {
      return upperValue;
    }
  }

  auto& node = tree.nodes[tree.ply];
  arrive(node);

  node.checkState = tree.position.getCheckState();

  // generate moves
  if (!isChecking(node.checkState)) {
    MoveGenerator::generateCapturingMoves(tree.position, node.moves);
    MoveGenerator::generateNotCapturingMoves(tree.position, node.moves);
  } else {
    MoveGenerator::generateEvasions(tree.position, node.checkState, node.moves);
  }

  // expand the branches
  for (;;) {
    Move move = nextMove(node);
    if (move.isEmpty()) {
      break;
    }

    bool moveOk = doMove(tree, move);
    if (!moveOk) {
      continue;
    }

    Value value = -search(tree,
                          depth - Depth1Ply,
                          -beta,
                          -alpha);

    undoMove(tree, move);

    if (value > alpha) {
      alpha = value;

      auto& frontierNode = tree.nodes[tree.ply-1];
      frontierNode.pv.set(move, node.pv);

      // beta cut
      if (value >= beta) {
        break;
      }
    }
  }

  return alpha;
}

Value Searcher::quies(Tree& tree,
                      Value alpha,
                      Value beta) {
  Turn turn = tree.position.getTurn();

  Value standPat = evaluator_.evaluateMaterial(tree.position);
  standPat = turn == Turn::Black ? standPat : -standPat;

  if (standPat >= beta) {
    return standPat;
  }

  alpha = std::max(alpha, standPat);

  auto& node = tree.nodes[tree.ply];
  arrive(node);

  node.checkState = tree.position.getCheckState();

  // generate moves
  if (!isChecking(node.checkState)) {
    MoveGenerator::generateCapturingMoves(tree.position, node.moves);
  } else {
    MoveGenerator::generateEvasions(tree.position, node.checkState, node.moves);
  }

  // expand the branches
  for (;;) {
    Move move = nextMove(node);
    if (move.isEmpty()) {
      break;
    }

    bool moveOk = doMove(tree, move);
    if (!moveOk) {
      continue;
    }

    Value value = -quies(tree,
                         -beta,
                         -alpha);

    undoMove(tree, move);

    if (value > alpha) {
      alpha = value;

      auto& frontierNode = tree.nodes[tree.ply-1];
      frontierNode.pv.set(move, node.pv);

      // beta cut
      if (value >= beta) {
        break;
      }
    }
  }

  return alpha;
}

Move Searcher::nextMove(Node& node) {
  if (node.currentMove == node.moves.end()) {
    return Move::empty();
  }

  return *(node.currentMove++);
}

} // namespace sunfish
