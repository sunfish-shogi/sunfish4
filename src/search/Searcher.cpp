/* Searcher.cpp
 *
 * Kubo Ryosuke
 */

#include "search/Searcher.hpp"
#include "core/move/MoveGenerator.hpp"
#include "logger/Logger.hpp"
#include <algorithm>
#include <cstring>

namespace {

using namespace sunfish;

CONSTEXPR_CONST int AspirationSearchMinDepth = 4 * Searcher::Depth1Ply;

}

namespace sunfish {

Searcher::Searcher() :
  handler_(nullptr) {
}

void Searcher::onSearchStarted() {
  interrupted_ = false;

  result_.move = Move::empty();
  result_.value = Value::zero();
  result_.pv.clear();

  initializeWorker(workerOnMainThread_);
}

void Searcher::updateInfo() {
  initializeSearchInfo(info_);
  mergeSearchInfo(info_, workerOnMainThread_.info);
}

void Searcher::generateMovesOnRoot(Tree& tree) {
  auto& node = tree.nodes[tree.ply];

  node.checkState = tree.position.getCheckState();

  // generate moves
  if (!isChecking(node.checkState)) {
    MoveGenerator::generateCapturingMoves(tree.position, node.moves);
    MoveGenerator::generateNotCapturingMoves(tree.position, node.moves);
  } else {
    MoveGenerator::generateEvasions(tree.position, node.checkState, node.moves);
  }

  random_.shuffle(node.moves.begin(), node.moves.end());
}

bool Searcher::search(const Position& pos,
                      int depth,
                      Value alpha,
                      Value beta) {
  onSearchStarted();

  auto& tree = treeOnMainThread_;
  auto& worker = workerOnMainThread_;
  initializeTree(tree, pos, &worker);

  auto& node = tree.nodes[tree.ply];
  arrive(node);

  generateMovesOnRoot(tree);

  Move bestMove = Move::empty();

  bool isFirst = true;

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

    int newDepth = depth - Depth1Ply;

    Value value;
    if (isFirst) {
      value = -search(tree,
                      newDepth,
                      -beta,
                      -alpha);
    } else {
      // nega-scout
      value = -search(tree,
                      newDepth,
                      -(alpha + 1),
                      -alpha);

      if (!interrupted_ && value > alpha && value < beta) {
        value = -search(tree,
                        newDepth,
                        -beta,
                        -alpha);
      }
    }

    undoMove(tree, move);

    updateInfo();

    if (interrupted_) {
      break;
    }

    if (value > alpha) {
      alpha = value;
      bestMove = move;
      auto& childNode = tree.nodes[tree.ply+1];
      node.pv.set(move, childNode.pv);

      if (value >= beta) {
        break;
      }
    }

    isFirst = false;
  }

  bool hasBestMove = !bestMove.isEmpty();

  result_.move = bestMove;
  result_.value = alpha;
  result_.pv = node.pv;

  return hasBestMove;
}

bool Searcher::idsearch(const Position& pos,
                        int depth) {
  onSearchStarted();

  auto& tree = treeOnMainThread_;
  auto& worker = workerOnMainThread_;
  initializeTree(tree, pos, &worker);

  auto& node = tree.nodes[tree.ply];
  arrive(node);

  generateMovesOnRoot(tree);

  bool ok = false;
  ValueArray values;

  for (size_t i = 0; i < node.moves.size();) {
    Move move = node.moves[i];

    bool moveOk = doMove(tree, move);
    if (!moveOk) {
      node.moves.remove(i);
      continue;
    }

    values[i] = quies(tree,
                      -Value::infinity(),
                      Value::infinity());

    undoMove(tree, move);

    i++;
  }

  if (node.moves.size() == 0) {
    return false;
  }

  for (int currDepth = Depth1Ply;; currDepth += Depth1Ply) {
    ok = aspsearch(tree, currDepth, values);

    if (!ok) {
      break;
    }

    if (currDepth >= depth) {
      break;
    }
  }

  result_.move = node.moves[0];
  result_.value = values[0];
  result_.pv = node.pv;

  return ok;
}

bool Searcher::aspsearch(Tree& tree,
                         int depth,
                         ValueArray& values) {
  auto& node = tree.nodes[tree.ply];

  if (node.moves.size() == 0) {
    return false;
  }

  bool doAsp = depth >= AspirationSearchMinDepth;

  Value prevValue = values[0];
  Value alphas[] = {
    prevValue - 128,
    prevValue - 256,
    -Value::infinity()
  };
  Value betas[] = {
    prevValue + 128,
    prevValue + 256,
    Value::infinity()
  };
  int alphaIndex = doAsp ? 0 : 2;
  int betaIndex = doAsp ? 0 : 2;

  Value bestValue = -Value::infinity();

  bool isFirst = true;

  // expand the branches
  for (size_t i = 0; i < node.moves.size();) {
    Value alpha = std::max(alphas[alphaIndex], bestValue);
    Value beta = betas[betaIndex];

    if (bestValue >= beta) {
      Loggers::warning << "invalid state: " << __FILE__ << ':' << __LINE__;
    }

    Move move = node.moves[i];

    bool moveOk = doMove(tree, move);
    if (!moveOk) {
      Loggers::warning << "invalid state: " << __FILE__ << ':' << __LINE__;
      i++;
      continue;
    }

    int newDepth = depth - Depth1Ply;

    Value value;
    if (isFirst) {
      value = -search(tree,
                      newDepth,
                      -beta,
                      -alpha);
    } else {
      // nega-scout
      value = -search(tree,
                      newDepth,
                      -(alpha + 1),
                      -alpha);

      if (!interrupted_ && value > alpha && value < beta) {
        value = -search(tree,
                        newDepth,
                        -beta,
                        -alpha);
      }
    }

    undoMove(tree, move);

    updateInfo();

    if (interrupted_) {
      break;
    }

    // fail-low
    if (value <= alphas[alphaIndex] && value >= bestValue) {
      alphaIndex++;

      auto& childNode = tree.nodes[tree.ply+1];
      node.pv.set(move, childNode.pv);

      if (handler_ != nullptr) {
        handler_->onFailLow(node.pv, depth, value);
      }
      continue;
    }

    // fail-high
    if (value >= beta && beta != Value::infinity()) {
      betaIndex++;

      auto& childNode = tree.nodes[tree.ply+1];
      node.pv.set(move, childNode.pv);

      if (handler_ != nullptr) {
        handler_->onFailHigh(node.pv, depth, value);
      }
      continue;
    }

    if (value > bestValue) {
      bestValue = value;

      auto& childNode = tree.nodes[tree.ply+1];
      node.pv.set(move, childNode.pv);

      if (handler_ != nullptr) {
        handler_->onUpdatePV(node.pv, depth, value);
      }
    }

    values[i] = value;

    // insertion
    for (int j = i - 1; j >= 0; j--) {
      if (values[j] >= values[j+1]) {
        break;
      }

      Move tm = node.moves[j];
      node.moves[j] = node.moves[j+1];
      node.moves[j+1] = tm;

      Value tv = values[j];
      values[j] = values[j+1];
      values[j+1] = tv;
    }

    i++;

    isFirst = false;
  }

  return bestValue > -Value::mate() && bestValue < Value::mate();
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

  // quiesence search
  if (depth <= 0) {
    return quies(tree,
                 alpha,
                 beta);
  }

  auto& node = tree.nodes[tree.ply];
  arrive(node);

  auto& worker = *tree.worker;
  worker.info.nodes++;

  Turn turn = tree.position.getTurn();

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

  node.checkState = tree.position.getCheckState();

  // generate moves
  if (!isChecking(node.checkState)) {
    MoveGenerator::generateCapturingMoves(tree.position, node.moves);
    MoveGenerator::generateNotCapturingMoves(tree.position, node.moves);
  } else {
    MoveGenerator::generateEvasions(tree.position, node.checkState, node.moves);
  }

  bool isNullWindow = alpha + 1 == beta;

  bool isFirst = true;

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

    int newDepth = depth - Depth1Ply;

    Value value;
    if (isFirst) {
      value = -search(tree,
                      newDepth,
                      -beta,
                      -alpha);
    } else {
      // nega-scout
      value = -search(tree,
                      newDepth,
                      -(alpha + 1),
                      -alpha);

      if (!interrupted_ && value > alpha && value < beta && !isNullWindow) {
        value = -search(tree,
                        newDepth,
                        -beta,
                        -alpha);
      }
    }

    undoMove(tree, move);

    if (interrupted_) {
      return Value::zero();
    }

    if (value > alpha) {
      alpha = value;

      auto& childNode = tree.nodes[tree.ply+1];
      node.pv.set(move, childNode.pv);

      // beta cut
      if (value >= beta) {
        break;
      }
    }

    isFirst = false;
  }

  return alpha;
}

Value Searcher::quies(Tree& tree,
                      Value alpha,
                      Value beta) {
  auto& node = tree.nodes[tree.ply];
  arrive(node);

  auto& worker = *tree.worker;
  worker.info.nodes++;

  Turn turn = tree.position.getTurn();

  Value standPat = evaluator_.evaluateMaterial(tree.position);
  standPat = turn == Turn::Black ? standPat : -standPat;

  if (standPat >= beta) {
    return standPat;
  }

  alpha = std::max(alpha, standPat);

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

      auto& childNode = tree.nodes[tree.ply+1];
      node.pv.set(move, childNode.pv);

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
