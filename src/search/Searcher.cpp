/* Searcher.cpp
 *
 * Kubo Ryosuke
 */

#include "search/Searcher.hpp"
#include "search/see/SEE.hpp"
#include "core/move/MoveGenerator.hpp"
#include "logger/Logger.hpp"
#include <algorithm>
#include <cstring>

namespace {

using namespace sunfish;

CONSTEXPR_CONST int AspirationSearchMinDepth = 4 * Searcher::Depth1Ply;

} // namespace

namespace sunfish {

Searcher::Searcher() :
  config_ (getDefaultSearchConfig()),
  handler_(nullptr) {
}

void Searcher::onSearchStarted() {
  interrupted_ = false;

  result_.move = Move::empty();
  result_.score = Score::zero();
  result_.pv.clear();

  initializeWorker(workerOnMainThread_);

  timer_.start();

  if (handler_ != nullptr) {
    handler_->onStart();
  }
}

void Searcher::updateInfo() {
  initializeSearchInfo(info_);
  mergeSearchInfo(info_, workerOnMainThread_.info);
}

bool Searcher::search(const Position& pos,
                      int depth,
                      Score alpha,
                      Score beta) {
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
    Move move = nextMoveOnRoot(node);
    if (move.isEmpty()) {
      break;
    }

    bool moveOk = doMove(tree, move);
    if (!moveOk) {
      continue;
    }

    int newDepth = depth - Depth1Ply;

    Score score;
    if (isFirst) {
      score = -search(tree,
                      newDepth,
                      -beta,
                      -alpha);
    } else {
      // nega-scout
      score = -search(tree,
                      newDepth,
                      -(alpha + 1),
                      -alpha);

      if (!isInterrupted() && score > alpha && score < beta) {
        score = -search(tree,
                        newDepth,
                        -beta,
                        -alpha);
      }
    }

    undoMove(tree);

    updateInfo();

    if (isInterrupted()) {
      break;
    }

    if (score > alpha) {
      alpha = score;
      bestMove = move;
      auto& childNode = tree.nodes[tree.ply+1];
      node.pv.set(move, childNode.pv);

      if (score >= beta) {
        break;
      }
    }

    isFirst = false;
  }

  bool hasBestMove = !bestMove.isEmpty();

  result_.move = bestMove;
  result_.score = alpha;
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

  for (Moves::size_type i = 0; i < node.moves.size();) {
    Move move = node.moves[i];

    bool moveOk = doMove(tree, move);
    if (!moveOk) {
      node.moves.remove(i);
      continue;
    }

    Score score = -quies(tree,
                         -Score::infinity(),
                         Score::infinity());
    setScoreToMove(node.moves[i], score);

    undoMove(tree);

    i++;
  }

  if (node.moves.size() == 0) {
    return false;
  }

  if (isInterrupted()) {
    return false;
  }

  std::sort(node.moves.begin(), node.moves.end(), [](Move lhs, Move rhs) {
    return moveToScore(lhs) > moveToScore(rhs);
  });

  for (int currDepth = Depth1Ply;; currDepth += Depth1Ply) {
    ok = aspsearch(tree, currDepth);

    if (!ok || isInterrupted() || currDepth >= depth) {
      break;
    }
  }

  result_.move = node.moves[0].excludeExtData();
  result_.score = moveToScore(node.moves[0]);
  result_.pv = node.pv;

  return ok;
}

bool Searcher::aspsearch(Tree& tree,
                         int depth) {
  auto& node = tree.nodes[tree.ply];

  if (node.moves.size() == 0) {
    return false;
  }

  bool doAsp = depth >= AspirationSearchMinDepth;

  Score prevScore = moveToScore(node.moves[0]);
  Score alphas[] = {
    prevScore - 128,
    prevScore - 256,
    -Score::infinity()
  };
  Score betas[] = {
    prevScore + 128,
    prevScore + 256,
    Score::infinity()
  };
  int alphaIndex = doAsp ? 0 : 2;
  int betaIndex = doAsp ? 0 : 2;

  Score bestScore = -Score::infinity();

  bool isFirst = true;

  // expand the branches
  for (Moves::size_type i = 0; i < node.moves.size();) {
    Score alpha = std::max(alphas[alphaIndex], bestScore);
    Score beta = betas[betaIndex];

    if (bestScore >= beta) {
      LOG(warning) << "invalid state.";
    }

    Move move = node.moves[i];

    bool moveOk = doMove(tree, move);
    if (!moveOk) {
      LOG(warning) << "invalid state.";
      node.moves.remove(i);
      continue;
    }

    int newDepth = depth - Depth1Ply;

    Score score;
    if (isFirst) {
      score = -search(tree,
                      newDepth,
                      -beta,
                      -alpha);
    } else {
      // nega-scout
      score = -search(tree,
                      newDepth,
                      -(alpha + 1),
                      -alpha);

      if (!isInterrupted() && score > alpha && score < beta) {
        score = -search(tree,
                        newDepth,
                        -beta,
                        -alpha);
      }
    }

    undoMove(tree);

    updateInfo();

    if (isInterrupted()) {
      break;
    }

    // fail-low
    if (score <= alphas[alphaIndex] && score >= bestScore) {
      alphaIndex++;

      auto& childNode = tree.nodes[tree.ply+1];
      node.pv.set(move, childNode.pv);

      if (handler_ != nullptr) {
        handler_->onFailLow(node.pv, timer_.getElapsed(), depth, score);
      }
      continue;
    }

    // fail-high
    if (score >= beta && beta != Score::infinity()) {
      betaIndex++;

      auto& childNode = tree.nodes[tree.ply+1];
      node.pv.set(move, childNode.pv);

      if (handler_ != nullptr) {
        handler_->onFailHigh(node.pv, timer_.getElapsed(), depth, score);
      }
      continue;
    }

    if (score > bestScore) {
      bestScore = score;

      auto& childNode = tree.nodes[tree.ply+1];
      node.pv.set(move, childNode.pv);
    }

    // insertion
    int j;
    for (j = i - 1; j >= 0; j--) {
      if (moveToScore(node.moves[j]) >= score) { break; }
      node.moves[j+1] = node.moves[j];
    }
    node.moves[j+1] = move;
    setScoreToMove(node.moves[j+1], score);

    i++;

    isFirst = false;
  }

  if (handler_ != nullptr &&
      node.pv.size() != 0 &&
      bestScore != -Score::infinity()) {
    handler_->onUpdatePV(node.pv, timer_.getElapsed(), depth, bestScore);
  }

  return bestScore > -Score::mate() && bestScore < Score::mate();
}

Score Searcher::search(Tree& tree,
                       int depth,
                       Score alpha,
                       Score beta) {
#if 0
  bool isDebug = false;
  if (getPath(tree, tree.ply) == "9394 9796") {
    LOG(message) << "debugging node";
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
    Score standPat = evaluator_.evaluateMaterial(tree.position);
    return turn == Turn::Black ? standPat : -standPat;
  }

  // distance pruning
  {
    Score lowerScore = -Score::infinity() + tree.ply;
    Score upperScore = Score::infinity() - tree.ply - 1;
    if (lowerScore >= beta) {
      return lowerScore;
    } else if (lowerScore > alpha) {
      alpha = lowerScore;
    } else if (upperScore <= alpha) {
      return upperScore;
    }
  }

  generateMoves(tree);

  bool isNullWindow = alpha + 1 == beta;

  bool isFirst = true;

  // expand the branches
  for (;;) {
    Move move = nextMove(tree);
    if (move.isEmpty()) {
      break;
    }

    bool moveOk = doMove(tree, move);
    if (!moveOk) {
      continue;
    }

    int newDepth = depth - Depth1Ply;

    Score score;
    if (isFirst) {
      score = -search(tree,
                      newDepth,
                      -beta,
                      -alpha);
    } else {
      // nega-scout
      score = -search(tree,
                      newDepth,
                      -(alpha + 1),
                      -alpha);

      if (!isInterrupted() && score > alpha && score < beta && !isNullWindow) {
        score = -search(tree,
                        newDepth,
                        -beta,
                        -alpha);
      }
    }

    undoMove(tree);

    if (isInterrupted()) {
      return Score::zero();
    }

    if (score > alpha) {
      alpha = score;

      auto& childNode = tree.nodes[tree.ply+1];
      node.pv.set(move, childNode.pv);

      // beta cut
      if (score >= beta) {
        break;
      }
    }

    isFirst = false;
  }

  return alpha;
}

Score Searcher::quies(Tree& tree,
                      Score alpha,
                      Score beta) {
  auto& node = tree.nodes[tree.ply];
  arrive(node);

  auto& worker = *tree.worker;
  worker.info.nodes++;

  Turn turn = tree.position.getTurn();

  Score standPat = evaluator_.evaluateMaterial(tree.position);
  standPat = turn == Turn::Black ? standPat : -standPat;

  if (standPat >= beta) {
    return standPat;
  }

  alpha = std::max(alpha, standPat);

  generateMovesOnQuies(tree);

  // expand the branches
  for (;;) {
    Move move = nextMoveOnQuies(node);
    if (move.isEmpty()) {
      break;
    }

    bool moveOk = doMove(tree, move);
    if (!moveOk) {
      continue;
    }

    Score score = -quies(tree,
                         -beta,
                         -alpha);

    undoMove(tree);

    if (score > alpha) {
      alpha = score;

      auto& childNode = tree.nodes[tree.ply+1];
      node.pv.set(move, childNode.pv);

      // beta cut
      if (score >= beta) {
        break;
      }
    }
  }

  return alpha;
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

#if 1
  random_.shuffle(node.moves.begin(), node.moves.end());
#endif
}

Move Searcher::nextMoveOnRoot(Node& node) {
  if (node.moveIterator == node.moves.end()) {
    return Move::empty();
  }

  return *(node.moveIterator++);
}

void Searcher::generateMoves(Tree& tree) {
  auto& node = tree.nodes[tree.ply];

  node.checkState = tree.position.getCheckState();

  // generate moves
  if (!isChecking(node.checkState)) {
    node.genPhase = GenPhase::CapturingMoves;
  } else {
    node.genPhase = GenPhase::Evasions;
  }
}

Move Searcher::nextMove(Tree& tree) {
  auto& node = tree.nodes[tree.ply];

  for (;;) {
    if (node.moveIterator != node.moves.end()) {
      return *(node.moveIterator++);
    }

    switch (node.genPhase) {
    case GenPhase::CapturingMoves:
      MoveGenerator::generateCapturingMoves(tree.position, node.moves);
      SEE::sortMoves(tree);
      node.genPhase = GenPhase::NotCapturingMoves;
      break;

    case GenPhase::NotCapturingMoves:
      MoveGenerator::generateNotCapturingMoves(tree.position, node.moves);
      node.genPhase = GenPhase::End;
      break;

    case GenPhase::Evasions:
      MoveGenerator::generateEvasions(tree.position, node.checkState, node.moves);
      node.genPhase = GenPhase::End;
      break;

    case GenPhase::End:
      return Move::empty();

    }
  }
}

void Searcher::generateMovesOnQuies(Tree& tree) {
  auto& node = tree.nodes[tree.ply];

  node.checkState = tree.position.getCheckState();

  // generate moves
  if (!isChecking(node.checkState)) {
    MoveGenerator::generateCapturingMoves(tree.position, node.moves);
    SEE::sortMoves(tree);
  } else {
    MoveGenerator::generateEvasions(tree.position, node.checkState, node.moves);
  }
}

Move Searcher::nextMoveOnQuies(Node& node) {
  if (node.moveIterator == node.moves.end()) {
    return Move::empty();
  }

  return *(node.moveIterator++);
}

} // namespace sunfish
