/* Searcher.cpp
 *
 * Kubo Ryosuke
 */

#include "search/Searcher.hpp"
#include "search/see/SEE.hpp"
#include "search/mate/Mate.hpp"
#include "search/eval/Evaluator.hpp"
#include "search/tree/Measure.hpp"
#include "core/move/MoveGenerator.hpp"
#include "logger/Logger.hpp"
#include <algorithm>
#include <cstring>
#include <cmath>

namespace {

using namespace sunfish;

CONSTEXPR_CONST int AspirationSearchMinDepth = 4 * Searcher::Depth1Ply;

// extensions
CONSTEXPR_CONST int ExtensionDepthForCheck     = Searcher::Depth1Ply * 1;
CONSTEXPR_CONST int ExtensionDepthForOneReply  = Searcher::Depth1Ply * 1 / 2;
CONSTEXPR_CONST int ExtensionDepthForRecapture = Searcher::Depth1Ply * 1 / 4;

/**
 * Check whether the recursive-iterative deepening should be run.
 */
inline bool shouldRecursiveIDSearch(int depth) {
  return depth >= 3 * Searcher::Depth1Ply;
}

/**
 * Calculate a depth of recursive iterative deepening.
 */
inline int recursiveIDSearchDepth(int depth) {
  return depth < Searcher::Depth1Ply * 9 / 2 ?  Searcher::Depth1Ply * 3 / 2 :
                                                depth - Searcher::Depth1Ply * 3;
}

/**
 * Calculate a depth of null move search.
 */
inline int nullDepth(int depth) {
  return depth <= Searcher::Depth1Ply * 26 / 4 ? depth - Searcher::Depth1Ply * 12 / 4 :
        (depth <= Searcher::Depth1Ply * 30 / 4 ? Searcher::Depth1Ply * 14 / 4 :
                                                 depth - Searcher::Depth1Ply * 16 / 4); 
}

/**
 * values for reducing from the depth.
 */
uint8_t ReductionDepth[32][2][2];

void initializeReductionDepth() {
  for (int hist = 0; hist < 32; hist++) {
    float r = std::pow(1.0f - hist/32.0f, 2.2f) * Searcher::Depth1Ply;
    ReductionDepth[hist][0][0] = r * 0.8f;
    ReductionDepth[hist][0][1] = r * 1.6f;
    ReductionDepth[hist][1][0] = r * 1.8f;
    ReductionDepth[hist][1][1] = r * 2.4f;
  }
}

/**
 * Returns reducing depth.
 */
int reductionDepth(int depth,
                   History::CountType hist,
                   bool isNullWindow,
                   bool improving) {
  static_assert(History::Scale >> 8 == 32, "invalid range");
  return ReductionDepth[hist >> 8]
                       [(!improving && depth < 9 * Searcher::Depth1Ply) ? 1 : 0]
                       [isNullWindow ? 1: 0];
}

/**
 * the maximum depth of futility pruning.
 */
CONSTEXPR_CONST int FutilityPruningMaxDepth = 9 * Searcher::Depth1Ply;

Score FutilityPruningMargin[9][32];

void initializeFutilityPruningMargin() {
  for (int depth = 0; depth < 9; depth++) {
    for (int count = 0; count < 32; count++) {
      Score margin = 72 * std::log(2.0f * (depth + 1.0f)) - 8 * count;
      FutilityPruningMargin[depth][count] = std::max(margin, Score(60));
    }
  }
}

/**
 * Returns the margin of futility pruning.
 */
Score futilityPruningMargin(int depth,
                            int count) {
  return FutilityPruningMargin[std::max(depth / Searcher::Depth1Ply, 0)]
                              [std::min(count / 4, 31)];
}

#if 0
void printMoves(const Position& pos,
                const Moves& moves) {
  std::ostringstream oss;
  for (const auto& move : moves) {
    Score score = moveToScore(move);
    oss << move.toString(pos) << '(' << score << ") ";
  }
  OUT(info) << oss.str();
}
#endif

} // namespace

namespace sunfish {

void Searcher::initialize() {
  initializeReductionDepth();
  initializeFutilityPruningMargin();
}

Searcher::Searcher() :
  config_ (getDefaultSearchConfig()),
  evaluator_(Evaluator::sharedEvaluator()),
  handler_(nullptr) {
}

Searcher::Searcher(std::shared_ptr<Evaluator> evaluator) :
  config_ (getDefaultSearchConfig()),
  evaluator_(evaluator),
  handler_(nullptr) {
}

void Searcher::clean() {
  tt_.clear();
  history_.clear();
}

void Searcher::onSearchStarted() {
  timer_.start();

  interrupted_ = false;

  result_.move = Move::none();
  result_.score = -Score::infinity();
  result_.pv.clear();
  result_.depth = 0;
  result_.elapsed = 0.0f;

  initializeWorker(mainThreadWorker_);

  history_.reduce();

  gain_.clear();

  if (handler_ != nullptr) {
    handler_->onStart(*this);
  }
}

void Searcher::updateInfo() {
  initializeSearchInfo(info_);
  mergeSearchInfo(info_, mainThreadWorker_.info);
  // TODO: other threads
}

/**
 * search from root node
 */
void Searcher::search(const Position& pos,
                      int depth,
                      Score alpha,
                      Score beta,
                      Record* record /*= nullptr*/) {
  onSearchStarted();

  auto& tree = mainThreadTree_;
  auto& worker = mainThreadWorker_;
  initializeTree(tree,
                 pos,
                 *evaluator_,
                 &worker,
                 record);

  auto& node = tree.nodes[tree.ply];
  arrive(node);

  node.checkState = tree.position.getCheckState();

  generateMoves<true>(tree);

  Move bestMove = Move::none();

  bool isFirst = true;

  // expand the branches
  for (;;) {
    Move move = nextMove(tree);
    if (move.isNone()) {
      break;
    }

    int newDepth = depth - Depth1Ply;

    // late move reduction
    int reduced = 0;
    if (!isFirst &&
        !isCheck(node.checkState) &&
        newDepth >= Depth1Ply &&
        !isTacticalMove(tree.position, move)) {
      auto turn = tree.position.getTurn();
      reduced = reductionDepth(newDepth,
                               history_.ratio(turn, move),
                               false,
                               true);
      newDepth = newDepth - reduced;
    }

    bool moveOk = doMove(tree, move, *evaluator_);
    if (!moveOk) {
      continue;
    }

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

#if ENABLE_MEASUREMENT
      if (reduced != 0 && MEASURE_SHOULD(LMR, newDepth)) {
        auto realScore = -search(tree,
                                 newDepth + reduced,
                                 -(alpha + 1),
                                 -alpha,
                                 newNodeStat);
        if (score <= alpha && realScore <= alpha) {
          MEASURE_TRUE_POSITIVE(LMR, newDepth + reduced);
        } else if (score <= alpha && realScore > alpha) {
          MEASURE_FALSE_POSITIVE(LMR, newDepth + reduced);
        } else if (score > alpha && realScore > alpha) {
          MEASURE_TRUE_NEGATIVE(LMR, newDepth + reduced);
        } else {
          MEASURE_FALSE_NEGATIVE(LMR, newDepth + reduced);
        }
      }
#endif // ENABLE_MEASUREMENT

      if (!isInterrupted() && score > alpha && reduced != 0) {
        newDepth = newDepth + reduced;
        score = -search(tree,
                        newDepth,
                        -(alpha + 1),
                        -alpha,
                        newNodeStat);
      }

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

  result_.move = bestMove;
  result_.score = alpha;
  result_.pv = node.pv;
  result_.depth = depth;
  result_.elapsed = timer_.elapsed();
}

/**
 * iterative deepening search
 */
void Searcher::idsearch(const Position& pos,
                        int depth,
                        Record* record /*= nullptr*/) {
  onSearchStarted();

  auto& tree = mainThreadTree_;
  auto& worker = mainThreadWorker_;
  initializeTree(tree,
                 pos,
                 *evaluator_,
                 &worker,
                 record);

  auto& node = tree.nodes[tree.ply];
  arrive(node);

  node.checkState = tree.position.getCheckState();

  // generate moves
  node.moves.clear();
  if (!isCheck(node.checkState)) {
    MoveGenerator::generateCapturingMoves(tree.position, node.moves);
    MoveGenerator::generateNotCapturingMoves(tree.position, node.moves);
  } else {
    MoveGenerator::generateEvasions(tree.position, node.checkState, node.moves);
  }

  sortRootMoves(tree);

  if (node.moves.size() == 0) {
    return;
  }

  if (isInterrupted()) {
    return;
  }

  int completedDepth = 0;
  for (int currDepth = Depth1Ply * 3 / 2; ; currDepth += Depth1Ply) {
    bool cont = aspsearch(tree, currDepth);

    if (isInterrupted()) {
      break;
    }

    completedDepth = currDepth;

    if (!cont || currDepth >= depth) {
      break;
    }
  }

  result_.move = node.moves[0].excludeExtData();
  result_.score = moveToScore(node.moves[0]);
  result_.pv = node.pv;
  result_.depth = completedDepth;
  result_.elapsed = timer_.elapsed();
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
    prevScore - 512,
    -Score::infinity()
  };
  Score betas[] = {
    prevScore + 128,
    prevScore + 512,
    Score::infinity()
  };
  int alphaIndex = doAsp ? 0 : 2;
  int betaIndex = doAsp ? 0 : 2;

  Score bestScore = -Score::infinity();

  bool isFirst = true;

  for (Moves::size_type i = 1; i < node.moves.size(); i++) {
    setScoreToMove(node.moves[i], -Score::infinity());
  }

  // expand branches
  for (Moves::size_type moveCount = 0; moveCount < node.moves.size();) {
    Score alpha = std::max(alphas[alphaIndex], bestScore);
    Score beta = betas[betaIndex];

    if (bestScore >= beta) {
      LOG(warning) << "invalid state.";
    }

    Move move = node.moves[moveCount];
    int newDepth = depth - Depth1Ply;

    // late move reduction
    int reduced = 0;
    if (!isFirst &&
        !isCheck(node.checkState) &&
        newDepth >= Depth1Ply &&
        !isTacticalMove(tree.position, move)) {
      auto turn = tree.position.getTurn();
      reduced = reductionDepth(newDepth,
                               history_.ratio(turn, move),
                               false,
                               true);
      newDepth = newDepth - reduced;
    }

    bool moveOk = doMove(tree, move, *evaluator_);
    if (!moveOk) {
      LOG(warning) << "invalid state.";
      node.moves.remove(moveCount);
      continue;
    }

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

#if ENABLE_MEASUREMENT
      if (reduced != 0 && MEASURE_SHOULD(LMR, newDepth)) {
        auto realScore = -search(tree,
                                 newDepth + reduced,
                                 -(alpha + 1),
                                 -alpha,
                                 newNodeStat);
        if (score <= alpha && realScore <= alpha) {
          MEASURE_TRUE_POSITIVE(LMR, newDepth + reduced);
        } else if (score <= alpha && realScore > alpha) {
          MEASURE_FALSE_POSITIVE(LMR, newDepth + reduced);
        } else if (score > alpha && realScore > alpha) {
          MEASURE_TRUE_NEGATIVE(LMR, newDepth + reduced);
        } else {
          MEASURE_FALSE_NEGATIVE(LMR, newDepth + reduced);
        }
      }
#endif // ENABLE_MEASUREMENT

      if (!isInterrupted() && score > alpha && reduced != 0) {
        newDepth = newDepth + reduced;
        score = -search(tree,
                        newDepth,
                        -(alpha + 1),
                        -alpha,
                        newNodeStat);
      }

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
    if (score <= alphas[alphaIndex] && score >= bestScore && score > -Score::mate()) {
      for (; score <= alphas[alphaIndex]; alphaIndex++) {}

      auto& childNode = tree.nodes[tree.ply+1];
      node.pv.set(move, depth, childNode.pv);

      if (handler_ != nullptr) {
        handler_->onFailLow(*this, node.pv, timer_.elapsed(), depth, score);
      }
      continue;
    }

    setScoreToMove(node.moves[moveCount], score);

    // fail-high
    if (score >= beta && beta != Score::infinity()) {
      for (; score >= betas[betaIndex]; betaIndex++) {}

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

    moveCount++;

    isFirst = false;
  }

  std::stable_sort(node.moves.begin(), node.moves.end(), [](Move lhs, Move rhs) {
    return moveToScore(lhs) > moveToScore(rhs);
  });

  if (node.pv.size() != 0 && bestScore != -Score::infinity()) {
    storePV(tree,
            node.pv,
            0,
            bestScore);

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
  if (getPath(tree, tree.ply) == "4647+ 5847 46FU") {
    LOG(info) << "debugging node:"
              << " depth=" << depth
              << " alpha=" << alpha
              << " beta =" << beta;
    isDebug = true;
  }
#endif

  auto& node = tree.nodes[tree.ply];
  arrive(node);

  // SHEK(strong horizontal effect killer)
  switch (tree.shekTable.check(tree.position)) {
  case ShekState::Equal4:
    node.isHistorical = true;
    switch (tree.scr.detect(tree)) {
    case SCRState::Draw:
      return Score::zero();
    case SCRState::Win:
      return Score::infinity() - tree.ply;
    case SCRState::Lose:
      return -Score::infinity() + tree.ply;
    case SCRState::None:
      break;
    }

  case ShekState::Equal:
    node.isHistorical = true;
    return Score::zero();

  case ShekState::Superior:
    node.isHistorical = true;
    return Score::infinity() - tree.ply;

  case ShekState::Inferior:
    node.isHistorical = true;
    return -Score::infinity() + tree.ply;

  case ShekState::None:
    break;
  }

  // quiesence search
  if (depth < Depth1Ply) {
    return quies(tree,
                 0,
                 alpha,
                 beta);
  }

  auto& worker = *tree.worker;
  worker.info.nodes++;

  if (tree.ply == Tree::StackSize - 2) {
    node.isHistorical = true;
    return calculateStandPat(tree, *evaluator_);
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
    if (nodeStat.isHashCut() &&
        isNullWindow &&
        (ttDepth >= depth || isMate)) {
      if (ttScoreType == TTScoreType::Exact ||
         (ttScoreType == TTScoreType::Upper && ttScore <= oldAlpha) ||
         (ttScoreType == TTScoreType::Lower && ttScore >= beta)) {
        worker.info.hashCut++;
        return ttScore;
      }
    }

    // if the score is larger than beta by a considerable margin.
    if (nodeStat.isHashCut() &&
        isNullWindow &&
        (ttScoreType == TTScoreType::Exact ||
         ttScoreType == TTScoreType::Lower) &&
        !isCheck(node.checkState) &&
        !isCheck(tree.nodes[tree.ply-1].checkState) &&
        depth < FutilityPruningMaxDepth &&
        ttScore >= beta + futilityPruningMargin(depth, 0)) {
      return beta;
    }

    if (!shouldRecursiveIDSearch(depth) ||
        ttDepth >= recursiveIDSearchDepth(depth)) {
      if (ttScoreType == TTScoreType::Exact ||
          ttScoreType == TTScoreType::Upper) {
        // if the score is smaller than alpha, exclude recursive iterative deepening search.
        if (ttScore < alpha && ttDepth >= recursiveIDSearchDepth(depth)) {
          nodeStat.unsetRecursiveIDSearch();
        }
        // if the score is smaller than beta, exclude null window search.
        if (ttScore < beta && ttDepth >= nullDepth(depth)) {
          nodeStat.unsetNullMoveSearch();
        }
      }

      // previous best move
      Move ttMove = tte.move();
      if (tree.position.isLegalMoveMaybe(ttMove, node.checkState)) {
        node.hashMove = ttMove;
      }
    }

    if (tte.isMateThreat()) {
      nodeStat.setMateThreat();
    }
  }

  if (nodeStat.isMateDetection() &&
      !isCheck(node.checkState) &&
      Mate::mate1Ply(tree.position)) {
    return Score::infinity() - tree.ply - 1;
  }

  Score standPat = calculateStandPat(tree, *evaluator_);

  // null move pruning
  if (isNullWindow &&
      depth >= Depth1Ply * 2 &&
      standPat >= beta &&
      nodeStat.isNullMoveSearch() &&
      !isCheck(node.checkState) &&
      !nodeStat.isMateThreat()) {
    int newDepth = nullDepth(depth);
    NodeStat newNodeStat = NodeStat::normal().unsetNullMoveSearch();

    doNullMove(tree);

    Score score = -search(tree,
                          newDepth,
                          -beta,
                          -beta+1,
                          newNodeStat);

#if ENABLE_MEASUREMENT
    if (MEASURE_SHOULD(NullMovePruning, depth)) {
      auto realScore = -search(tree,
                               depth - Depth1Ply,
                               -beta,
                               -beta+1,
                               newNodeStat);
      if (score >= beta && realScore >= beta) {
        MEASURE_TRUE_POSITIVE(NullMovePruning, depth);
      } else if (score >= beta && realScore < beta) {
        MEASURE_FALSE_POSITIVE(NullMovePruning, depth);
      } else if (score < beta && realScore < beta) {
        MEASURE_TRUE_NEGATIVE(NullMovePruning, depth);
      } else {
        MEASURE_FALSE_NEGATIVE(NullMovePruning, depth);
      }
    }
#endif // ENABLE_MEASUREMENT

    undoNullMove(tree);

    if (score >= beta) {
      auto& childNode = tree.nodes[tree.ply+1];
      node.isHistorical = childNode.isHistorical;
      worker.info.nullMovePruning++;
      tt_.store(tree.position.getHash(),
                oldAlpha,
                beta,
                score,
                depth,
                tree.ply,
                Move::none(),
                false);
      return score;
    }

    if (score < -Score::mate()) {
      nodeStat.setMateThreat();
    }
  }

  // recursive iterative deepening
  if (node.hashMove.isNone() &&
      nodeStat.isRecursiveIDSearch() &&
      shouldRecursiveIDSearch(depth)) {
    int newDepth = recursiveIDSearchDepth(depth);
    NodeStat newNodeStat = NodeStat::normal().unsetNullMoveSearch()
                                             .unsetHashCut()
                                             .unsetMateDetection();

    search(tree,
           newDepth,
           alpha,
           beta,
           newNodeStat);

    if (isInterrupted()) {
      return Score::zero();
    }

    TTElement tte;
    if (tt_.get(tree.position.getHash(), tte)) {
      Move ttMove = tte.move();
      if (tree.position.isLegalMoveMaybe(ttMove, node.checkState)) {
        node.hashMove = ttMove;
      }
    }

    arrive(node);
  }

  bool isFirst = true;
  bool improving = isImproving(tree, *evaluator_);
  Move bestMove = Move::none();

  generateMoves<false>(tree);

  // expand branches
  for (int moveCount = 0; ; moveCount++) {
    Move move = nextMove(tree);
    if (move.isNone()) {
      break;
    }

    bool currentMoveIsCheck = tree.position.isCheck(move);
    int newDepth = depth - Depth1Ply;
    NodeStat newNodeStat = NodeStat::normal();

    // extensions
    if (currentMoveIsCheck) {
      newDepth += ExtensionDepthForCheck;

    } else if (isFirst &&
               isCheck(node.checkState) &&
               node.moveIterator == node.moves.end()) {
      newDepth += ExtensionDepthForOneReply;

    } else if (!isCheck(node.checkState) &&
               nodeStat.isRecaptureExtension() &&
               isRecapture(tree, move)) {
      newDepth += ExtensionDepthForRecapture;
      nodeStat.unsetRecaptureExtension();
      newNodeStat.unsetRecaptureExtension();
    }

    // late move reduction
    int reduced = 0;
    if (!isFirst &&
        newDepth >= Depth1Ply &&
        !nodeStat.isMateThreat() &&
        !isCheck(node.checkState) &&
        !isPriorMove(tree, move) &&
        !isTacticalMove(tree.position, move)) {
      auto turn = tree.position.getTurn();
      reduced = reductionDepth(newDepth,
                               history_.ratio(turn, move),
                               isNullWindow,
                               improving);
      newDepth = newDepth - reduced;
    }

    // futility pruning
    Score estScore = estimateScore(tree, move, *evaluator_);
    if (!currentMoveIsCheck &&
        !isCheck(node.checkState) &&
        newDepth < FutilityPruningMaxDepth &&
        alpha > -Score::mate()) {
      Score futAlpha = alpha - futilityPruningMargin(newDepth, moveCount);
      if (estScore + gain_.get(move, targetPiece(tree, move)) <= futAlpha) {
        isFirst = false;
        worker.info.futilityPruning++;
#if ENABLE_MEASUREMENT
        if (MEASURE_SHOULD(FutilityPruning, depth) && doMove(tree, move, *evaluator_)) {
          if (-search(tree, newDepth, -beta, -alpha, newNodeStat) <= alpha) {
            MEASURE_TRUE_POSITIVE(FutilityPruning, newDepth);
          } else {
            MEASURE_FALSE_POSITIVE(FutilityPruning, newDepth);
          }
          undoMove(tree);
        }
#endif // ENABLE_MEASUREMENT
        continue;
      }
    }
#if ENABLE_MEASUREMENT
    if (MEASURE_SHOULD(FutilityPruning, depth) && doMove(tree, move, *evaluator_)) {
      if (-search(tree, newDepth, -beta, -alpha, newNodeStat) <= alpha) {
        MEASURE_FALSE_NEGATIVE(FutilityPruning, newDepth);
      } else {
        MEASURE_TRUE_NEGATIVE(FutilityPruning, newDepth);
      }
      undoMove(tree);
    }
#endif // ENABLE_MEASUREMENT

    // prune negative SEE moves
    if (!currentMoveIsCheck &&
        !isCheck(node.checkState) &&
        newDepth < Depth1Ply * 2 &&
        !isPriorMove(tree, move) &&
        !isTacticalMove(tree.position, move) &&
        SEE::calculate(tree.position, move) < Score::zero()) {
      isFirst = false;
      continue;
    }

    bool moveOk = doMove(tree, move, *evaluator_);
    if (!moveOk) {
      node.moveIterator = node.moves.remove(node.moveIterator-1);
      moveCount--;
      continue;
    }

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

#if ENABLE_MEASUREMENT
      if (reduced != 0 && MEASURE_SHOULD(LMR, newDepth)) {
        auto realScore = -search(tree,
                                 newDepth + reduced,
                                 -(alpha + 1),
                                 -alpha,
                                 newNodeStat);
        if (score <= alpha && realScore <= alpha) {
          MEASURE_TRUE_POSITIVE(LMR, newDepth + reduced);
        } else if (score <= alpha && realScore > alpha) {
          MEASURE_FALSE_POSITIVE(LMR, newDepth + reduced);
        } else if (score > alpha && realScore > alpha) {
          MEASURE_TRUE_NEGATIVE(LMR, newDepth + reduced);
        } else {
          MEASURE_FALSE_NEGATIVE(LMR, newDepth + reduced);
        }
      }
#endif // ENABLE_MEASUREMENT

      if (!isInterrupted() &&
          score > alpha &&
          reduced != 0) {
        newDepth = newDepth + reduced;
        score = -search(tree,
                        newDepth,
                        -(alpha + 1),
                        -alpha,
                        newNodeStat);
      }

      if (!isInterrupted() &&
          score > alpha &&
          score < beta &&
          !isNullWindow) {
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

    auto& childNode = tree.nodes[tree.ply+1];

    if (childNode.score != Score::invalid()) {
      auto turn = tree.position.getTurn();
      auto newStandPat = turn == Turn::Black
                       ? childNode.score
                       : -childNode.score;
      gain_.update(move,
                   targetPiece(tree, move),
                   newStandPat - estScore);
    }

    if (score > alpha) {
      alpha = score;
      bestMove = move;

      // beta cut
      if (score >= beta) {
        node.isHistorical = childNode.isHistorical;
        worker.info.failHigh++;
        if (isFirst) {
          worker.info.failHighFirst++;
        }
        break;
      }

      node.pv.set(move, depth, childNode.pv);
    }

    node.isHistorical |= childNode.isHistorical;

    isFirst = false;
  }

  if (!bestMove.isNone() &&
      !isCheck(node.checkState)) {
    // killer move
    addKiller(tree, bestMove);

    // history heuristics
    unsigned hval = std::max(depth * 8 / Depth1Ply, 1);
    for (auto& move : node.moves) {
      history_.add(tree.position.getTurn(),
                   move,
                   hval,
                   move == bestMove ? hval : 0);
    }
  }

  if (!node.isHistorical) {
    tt_.store(tree.position.getHash(),
              oldAlpha,
              beta,
              alpha,
              depth,
              tree.ply,
              bestMove,
              nodeStat.isMateThreat());
  }

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
  worker.info.quiesNodes++;

  Score standPat = calculateStandPat(tree, *evaluator_);

  if (standPat >= beta) {
    return standPat;
  }

  if (tree.ply == Tree::StackSize - 2) {
    node.isHistorical = true;
    return standPat;
  }

  node.checkState = tree.position.getCheckState();

  if (!isCheck(node.checkState) &&
      Mate::mate1Ply(tree.position)) {
    return Score::infinity() - tree.ply - 1;
  }

  alpha = std::max(alpha, standPat);

  generateMovesOnQuies(tree,
                       qply,
                       alpha);

  // expand the branches
  for (;;) {
    Move move = nextMoveOnQuies(node);
    if (move.isNone()) {
      break;
    }

    bool moveOk = doMove(tree, move, *evaluator_);
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

/**
 * generate moves for full expanding nodes
 */
template <bool isRootNode>
void Searcher::generateMoves(Tree& tree) {
  auto& node = tree.nodes[tree.ply];

  node.moves.clear();
  node.moveIterator = node.moves.begin();

  if (!node.hashMove.isNone()) {
    node.moves.add(node.hashMove);
  }

  if (!isRootNode &&
      !isCheck(node.checkState) &&
      hasKiller1(tree) &&
      isKiller1Good(tree) &&
      isKiller1Legal(tree)) {
    auto& parentNode = tree.nodes[tree.ply-1];
    node.moves.add(parentNode.killerMove1);
  }

  if (!isRootNode &&
      !isCheck(node.checkState) &&
      hasKiller2(tree) &&
      isKiller2Good(tree) &&
      isKiller2Legal(tree)) {
    auto& parentNode = tree.nodes[tree.ply-1];
    node.moves.add(parentNode.killerMove2);
  }

  if (!isCheck(node.checkState)) {
    node.genPhase = GenPhase::CapturingMoves;
  } else {
    node.genPhase = GenPhase::Evasions;
  }
}

Move Searcher::nextMove(Tree& tree) {
  auto& node = tree.nodes[tree.ply];

  for (;;) {
    if (node.moveIterator != node.moves.end()/* &&
        // if the move has minus SEE value, carry foward it to NotCapturingMoves phase.
        (node.genPhase != GenPhase::NotCapturingMoves ||
         moveToScore(*node.moveIterator) >= Score::zero())*/) {
      return *(node.moveIterator++);
    }

    switch (node.genPhase) {
    case GenPhase::CapturingMoves:
      MoveGenerator::generateCapturingMoves(tree.position, node.moves);
      remove(node.moves, node.moveIterator, [&tree](const Move& move) {
        return isPriorMove(tree, move);
      });
      SEE::sortMoves(tree.position,
                     node.moves,
                     node.moveIterator,
                     false, /* excludeNegative */
                     false  /* excludeSmallCaptures */);
      node.genPhase = GenPhase::NotCapturingMoves;
      break;

    case GenPhase::NotCapturingMoves:
      MoveGenerator::generateNotCapturingMoves(tree.position,
                                               node.moves);
      remove(node.moves, node.moveIterator, [&tree](const Move& move) {
        return isPriorMove(tree, move);
      });
      sortMovesOnHistory(tree);
      node.genPhase = GenPhase::End;
      break;

    case GenPhase::Evasions:
      MoveGenerator::generateEvasions(tree.position, node.checkState, node.moves);
      sortMovesOnHistory(tree);
      node.genPhase = GenPhase::End;
      break;

    case GenPhase::End:
      return Move::none();

    }
  }
}

/**
 * generate moves for quiesence search
 */
void Searcher::generateMovesOnQuies(Tree& tree,
                                    int qply,
                                    Score alpha) {
  auto& node = tree.nodes[tree.ply];
  auto& worker = *tree.worker;

  node.moves.clear();
  node.moveIterator = node.moves.begin();

  if (!isCheck(node.checkState)) {
    MoveGenerator::generateCapturingMoves(tree.position, node.moves);

    // futility pruning
    for (auto ite = node.moveIterator; ite != node.moves.end(); ) {
      auto& move = *ite;

      Score estScore = estimateScore(tree, move, *evaluator_)
                     + gain_.get(move, targetPiece(tree, move));
      if (estScore <= alpha) {
        ite = node.moves.remove(ite);
        worker.info.futilityPruning++;
        continue;
      }

      ite++;
    }

    SEE::sortMoves(tree.position,
                   node.moves,
                   node.moveIterator,
                   true, /* excludeNegative */
                   qply >= 6 /* excludeSmallCaptures */);
  } else {
    MoveGenerator::generateEvasions(tree.position, node.checkState, node.moves);
    sortMovesOnHistory(tree);
  }
}

Move Searcher::nextMoveOnQuies(Node& node) {
  if (node.moveIterator == node.moves.end()) {
    return Move::none();
  }

  return *(node.moveIterator++);
}

void Searcher::sortMovesOnHistory(Tree& tree) {
  auto& node = tree.nodes[tree.ply];
  auto turn = tree.position.getTurn();

  for (auto ite = node.moveIterator; ite != node.moves.end(); ite++) {
    auto& move = *ite;
    auto r = history_.ratio(turn, move);
    move.setExtData(static_cast<Move::RawType16>(r));
  }

  std::sort(node.moveIterator, node.moves.end(), [](const Move& lhs, const Move& rhs) {
    return lhs.extData() > rhs.extData();
  });
}

void Searcher::sortRootMoves(Tree& tree) {
  auto& node = tree.nodes[tree.ply];

  random_.shuffle(node.moves.begin(), node.moves.end());

  Move ttMove = Move::none();
  TTElement tte;
  if (tt_.get(tree.position.getHash(), tte)) {
    ttMove = tte.move();
  }

  for (Moves::size_type moveCount = 0; moveCount < node.moves.size();) {
    Move move = node.moves[moveCount];

    bool moveOk = doMove(tree, move, *evaluator_);
    if (!moveOk) {
      node.moves.remove(moveCount);
      continue;
    }

    if (move == ttMove) {
      undoMove(tree);
      setScoreToMove(node.moves[moveCount], Score::infinity());
      moveCount++;
      continue;
    }

    Score score = quies(tree,
                        0,
                        -Score::infinity(),
                        Score::infinity());

    if (move.isPromotion()) {
      score += 1;
    }

    setScoreToMove(node.moves[moveCount], -score);

    undoMove(tree);

    moveCount++;
  }

  std::sort(node.moves.begin(), node.moves.end(), [](Move lhs, Move rhs) {
    return moveToScore(lhs) > moveToScore(rhs);
  });
}

void Searcher::storePV(Tree& tree,
                       const PV& pv,
                       unsigned ply,
                       Score score) {
  if (ply >= pv.size()) {
    return;
  }

  int depth = pv.getDepth(ply);
  if (depth <= 0) {
    return;
  }

  Move move = pv.getMove(ply);
  if (move.isNone()) {
    LOG(warning) << "the PV contain an invalid move.";
    return;
  }

  if (doMove(tree, move, *evaluator_)) {
    storePV(tree,
            pv,
            ply + 1,
            -score);
    undoMove(tree);
    tt_.storePV(tree.position.getHash(),
                score,
                depth,
                move);
  } else {
    LOG(warning) << "the PV contain an illegal move.";
  }
}

} // namespace sunfish
