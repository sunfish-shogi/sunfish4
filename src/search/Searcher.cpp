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

#if 0
void printMoves(const Position& pos, const Moves& moves) {
  std::ostringstream oss;
  for (const auto& move : moves) {
    Score score = moveToScore(move);
    oss << move.toString(pos) << '(' << score << ") ";
  }
  OUT(info) << oss.str();
}
#endif

// constants
CONSTEXPR_CONST int AspirationSearchMinDepth = 4 * Searcher::Depth1Ply;

inline int nullDepth(int depth) {
  return (depth <  Searcher::Depth1Ply * 26 / 4 ? depth - Searcher::Depth1Ply * 12 / 4 :
         (depth <= Searcher::Depth1Ply * 30 / 4 ? Searcher::Depth1Ply * 14 / 4 :
                                                  depth - Searcher::Depth1Ply * 16 / 4)); 
}

} // namespace

namespace sunfish {

Searcher::Searcher() :
  config_ (getDefaultSearchConfig()),
  handler_(nullptr) {
}

void Searcher::onSearchStarted() {
  timer_.start();

  interrupted_ = false;

  result_.move = Move::empty();
  result_.score = Score::zero();
  result_.pv.clear();
  result_.elapsed = 0.0f;

  initializeWorker(workerOnMainThread_);

  tt_.evolve();

  if (handler_ != nullptr) {
    handler_->onStart(*this);
  }
}

void Searcher::updateInfo() {
  initializeSearchInfo(info_);
  mergeSearchInfo(info_, workerOnMainThread_.info);
}

/**
 * search of root node
 */
bool Searcher::search(const Position& pos,
                      int depth,
                      Score alpha,
                      Score beta) {
  onSearchStarted();

  auto& tree = treeOnMainThread_;
  auto& worker = workerOnMainThread_;
  Score rootScore = evaluator_.evaluateMaterial(pos);
  initializeTree(tree, pos, rootScore, &worker);

  auto& node = tree.nodes[tree.ply];
  arrive(node);

  node.checkState = tree.position.getCheckState();

  generateMoves(tree);

  Move bestMove = Move::empty();

  bool isFirst = true;

  // expand the branches
  for (;;) {
    Move move = nextMove(tree);
    if (move.isEmpty()) {
      break;
    }

    bool moveOk = doMove(tree, move, evaluator_);
    if (!moveOk) {
      continue;
    }

    int newDepth = depth - Depth1Ply;
    NodeStat newNodeStat = NodeStat::normal();

    Score score;
    if (isFirst) {
      score = -search(tree,
                      newDepth,
                      -beta,
                      -alpha,
                      newNodeStat);
    } else {
      // nega-scout
      score = -search(tree,
                      newDepth,
                      -(alpha + 1),
                      -alpha,
                      newNodeStat);

      if (!isInterrupted() && score > alpha && score < beta) {
        score = -search(tree,
                        newDepth,
                        -beta,
                        -alpha,
                        newNodeStat);
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
      node.pv.set(move, depth, childNode.pv);

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
  result_.elapsed = timer_.elapsed();

  return hasBestMove;
}

/**
 * iterative deepening search
 */
bool Searcher::idsearch(const Position& pos,
                        int depth) {
  onSearchStarted();

  auto& tree = treeOnMainThread_;
  auto& worker = workerOnMainThread_;
  Score rootScore = evaluator_.evaluateMaterial(pos);
  initializeTree(tree, pos, rootScore, &worker);

  auto& node = tree.nodes[tree.ply];
  arrive(node);

  node.checkState = tree.position.getCheckState();

  // generate moves
  if (!isCheck(node.checkState)) {
    MoveGenerator::generateCapturingMoves(tree.position, node.moves);
    MoveGenerator::generateNotCapturingMoves(tree.position, node.moves);
  } else {
    MoveGenerator::generateEvasions(tree.position, node.checkState, node.moves);
  }

#if 1
  random_.shuffle(node.moves.begin(), node.moves.end());
#endif

  bool ok = false;

  for (Moves::size_type i = 0; i < node.moves.size();) {
    Move move = node.moves[i];

    bool moveOk = doMove(tree, move, evaluator_);
    if (!moveOk) {
      node.moves.remove(i);
      continue;
    }

    Score score = -quies(tree,
                         0,
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
  result_.elapsed = timer_.elapsed();

  return ok;
}

/**
 * aspiration search
 */
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

    bool moveOk = doMove(tree, move, evaluator_);
    if (!moveOk) {
      LOG(warning) << "invalid state.";
      node.moves.remove(i);
      continue;
    }

    int newDepth = depth - Depth1Ply;
    NodeStat newNodeStat = NodeStat::normal();

    Score score;
    if (isFirst) {
      score = -search(tree,
                      newDepth,
                      -beta,
                      -alpha,
                      newNodeStat);
    } else {
      // nega-scout
      score = -search(tree,
                      newDepth,
                      -(alpha + 1),
                      -alpha,
                      newNodeStat);

      if (!isInterrupted() && score > alpha && score < beta) {
        score = -search(tree,
                        newDepth,
                        -beta,
                        -alpha,
                        newNodeStat);
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
      node.pv.set(move, depth, childNode.pv);

      if (handler_ != nullptr) {
        handler_->onFailLow(*this, node.pv, timer_.elapsed(), depth, score);
      }
      continue;
    }

    // fail-high
    if (score >= beta && beta != Score::infinity()) {
      betaIndex++;

      auto& childNode = tree.nodes[tree.ply+1];
      node.pv.set(move, depth, childNode.pv);

      if (handler_ != nullptr) {
        handler_->onFailHigh(*this, node.pv, timer_.elapsed(), depth, score);
      }
      continue;
    }

    if (score > bestScore) {
      bestScore = score;

      auto& childNode = tree.nodes[tree.ply+1];
      node.pv.set(move, depth, childNode.pv);
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

  if (node.pv.size() != 0 && bestScore != -Score::infinity()) {
    storePV(tree, node.pv, 0);

    if (handler_ != nullptr) {
      handler_->onUpdatePV(*this, node.pv, timer_.elapsed(), depth, bestScore);
    }
  }

  return bestScore > -Score::mate() && bestScore < Score::mate();
}

/**
 * search of internal nodes
 */
Score Searcher::search(Tree& tree,
                       int depth,
                       Score alpha,
                       Score beta,
                       NodeStat nodeStat) {
#if 0
  bool isDebug = false;
  if (getPath(tree, tree.ply) == "42GI") {
    LOG(message) << "debugging node:"
        << " depth=" << depth
        << " alpha=" << alpha
        << " beta =" << beta;
    isDebug = true;
  }
#endif

  // quiesence search
  if (depth <= 0) {
    return quies(tree,
                 0,
                 alpha,
                 beta);
  }

  auto& node = tree.nodes[tree.ply];
  arrive(node);

  auto& worker = *tree.worker;
  worker.info.nodes++;

  // static evaluation
  if (tree.ply >= Tree::StackSize) {
    Turn turn = tree.position.getTurn();
    return turn == Turn::Black ? node.score : -node.score;
  }

  const Score oldAlpha = alpha;

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

  bool isNullWindow = oldAlpha + 1 == beta;

  node.checkState = tree.position.getCheckState();

  // transposition table
  TTElement tte;
  if (tt_.get(tree.position.getHash(), tte)) {
    auto ttScoreType = tte.scoreType();
    Score ttScore = tte.score(tree.ply);
    int ttDepth = tte.depth();

    bool isMate = (ttScore <= -Score::mate() && (ttScoreType == TTScoreType::Exact ||
                                                 ttScoreType == TTScoreType::Upper)) ||
                  (ttScore >=  Score::mate() && (ttScoreType == TTScoreType::Exact ||
                                                 ttScoreType == TTScoreType::Lower));

    // cut
    if (isNullWindow && (ttDepth >= depth || isMate)) {
      if (ttScoreType == TTScoreType::Exact ||
         (ttScoreType == TTScoreType::Upper && ttScore <= oldAlpha) ||
         (ttScoreType == TTScoreType::Lower && ttScore >= beta)) {
        worker.info.hashCut++;
        return ttScore;
      }
    }

    if (ttScoreType == TTScoreType::Exact ||
        ttScoreType == TTScoreType::Upper) {
      // if the score is smaller than beta, exclude null window search .
      if (ttScore < beta && ttDepth >= nullDepth(depth)) {
        nodeStat.unsetNullMove();
      }
    }

    // previous best move
    Move ttMove = tte.move();
    if (tree.position.isLegalMoveMaybe(ttMove, node.checkState)) {
      node.hashMove = ttMove;
    }
  }

  Turn turn = tree.position.getTurn();
  Score standPat = turn == Turn::Black ? node.score : -node.score;

  bool isFirst = true;
  Move bestMove = Move::empty();

  // null move pruning
  if (isNullWindow &&
      nodeStat.isNullMove() &&
      !isCheck(node.checkState) &&
      standPat >= beta &&
      depth >= Depth1Ply * 2) {
    int newDepth = nullDepth(depth);
    NodeStat newNodeStat = NodeStat::normal().unsetNullMove();

    doNullMove(tree);

    Score score = -search(tree,
                          newDepth,
                          -beta,
                          -alpha,
                          newNodeStat);

    undoNullMove(tree);

    if (score >= beta) {
      alpha = score;
      worker.info.nullMovePruning++;
      goto hash_store; // XXX
    }
  }

  generateMoves(tree);

  // expand the branches
  for (;;) {
    Move move = nextMove(tree);
    if (move.isEmpty()) {
      break;
    }

    bool moveOk = doMove(tree, move, evaluator_);
    if (!moveOk) {
      continue;
    }

    int newDepth = depth - Depth1Ply;
    NodeStat newNodeStat = NodeStat::normal();

    Score score;
    if (isFirst) {
      score = -search(tree,
                      newDepth,
                      -beta,
                      -alpha,
                      newNodeStat);
    } else {
      // nega-scout
      score = -search(tree,
                      newDepth,
                      -(alpha + 1),
                      -alpha,
                      newNodeStat);

      if (!isInterrupted() && score > alpha && score < beta && !isNullWindow) {
        score = -search(tree,
                        newDepth,
                        -beta,
                        -alpha,
                        newNodeStat);
      }
    }

    undoMove(tree);

    if (isInterrupted()) {
      return Score::zero();
    }

    if (score > alpha) {
      alpha = score;
      bestMove = move;

      auto& childNode = tree.nodes[tree.ply+1];
      node.pv.set(move, depth, childNode.pv);

      // beta cut
      if (score >= beta) {
        break;
      }
    }

    isFirst = false;
  }

hash_store:

  tt_.store(tree.position.getHash(),
            oldAlpha,
            beta,
            alpha,
            depth,
            tree.ply,
            bestMove);

  return alpha;
}

/**
 * quiesence search
 */
Score Searcher::quies(Tree& tree,
                      int qply,
                      Score alpha,
                      Score beta) {
  auto& node = tree.nodes[tree.ply];
  arrive(node);

  auto& worker = *tree.worker;
  worker.info.nodes++;

  Turn turn = tree.position.getTurn();
  Score standPat = turn == Turn::Black ? node.score : -node.score;

  if (standPat >= beta) {
    return standPat;
  }

  alpha = std::max(alpha, standPat);

  node.checkState = tree.position.getCheckState();

  generateMovesOnQuies(tree, qply);

  // expand the branches
  for (;;) {
    Move move = nextMoveOnQuies(node);
    if (move.isEmpty()) {
      break;
    }

    bool moveOk = doMove(tree, move, evaluator_);
    if (!moveOk) {
      continue;
    }

    Score score = -quies(tree,
                         qply + 1,
                         -beta,
                         -alpha);

    undoMove(tree);

    if (score > alpha) {
      alpha = score;

      auto& childNode = tree.nodes[tree.ply+1];
      node.pv.set(move, 0, childNode.pv);

      // beta cut
      if (score >= beta) {
        break;
      }
    }
  }

  return alpha;
}

void Searcher::generateMoves(Tree& tree) {
  auto& node = tree.nodes[tree.ply];

  if (!node.hashMove.isEmpty()) {
    node.moves.add(node.hashMove);
  }

  // generate moves
  if (!isCheck(node.checkState)) {
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
      remove(node.moves, node.moveIterator, node.hashMove);
      SEE::sortMoves(tree.position,
                     node.moves,
                     node.moveIterator,
                     false /* excludeSmallCaptures */);
      node.genPhase = GenPhase::NotCapturingMoves;
      break;

    case GenPhase::NotCapturingMoves:
      MoveGenerator::generateNotCapturingMoves(tree.position,
                                               node.moves);
      remove(node.moves, node.moveIterator, node.hashMove);
      // TODO: ordering
      node.genPhase = GenPhase::End;
      break;

    case GenPhase::Evasions:
      MoveGenerator::generateEvasions(tree.position, node.checkState, node.moves);
      // TODO: ordering
      node.genPhase = GenPhase::End;
      break;

    case GenPhase::End:
      return Move::empty();

    }
  }
}

void Searcher::generateMovesOnQuies(Tree& tree, int qply) {
  auto& node = tree.nodes[tree.ply];

  // generate moves
  if (!isCheck(node.checkState)) {
    MoveGenerator::generateCapturingMoves(tree.position, node.moves);
    bool excludeSmallCaptures = qply >= 7;
    SEE::sortMoves(tree.position,
                   node.moves,
                   node.moveIterator,
                   excludeSmallCaptures);
  } else {
    MoveGenerator::generateEvasions(tree.position, node.checkState, node.moves);
    // TODO: ordering
  }
}

Move Searcher::nextMoveOnQuies(Node& node) {
  if (node.moveIterator == node.moves.end()) {
    return Move::empty();
  }

  return *(node.moveIterator++);
}

void Searcher::storePV(Tree& tree, const PV& pv, unsigned ply) {
  if (ply >= pv.size()) {
    return;
  }

  int depth = pv.getDepth(ply);
  if (depth <= 0) {
    return;
  }

  Move move = pv.getMove(ply);
  if (move.isEmpty()) {
    LOG(warning) << "the PV contain an invalid move.";
    return;
  }

  if (doMove(tree, move, evaluator_)) {
    storePV(tree, pv, ply + 1);
    undoMove(tree);
  } else {
    LOG(warning) << "the PV contain an illegal move.";
  }

  tt_.storePV(tree.position.getHash(),
              depth,
              move);
}

} // namespace sunfish
