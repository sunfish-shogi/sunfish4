/* Searcher.cpp
 *
 * Kubo Ryosuke
 */

#include "search/Searcher.hpp"
#include "search/see/SEE.hpp"
#include "search/mate/Mate.hpp"
#include "search/eval/Evaluator.hpp"
#include "core/move/MoveGenerator.hpp"
#include "logger/Logger.hpp"
#include <algorithm>
#include <cstring>
#include <cmath>

namespace {

using namespace sunfish;

CONSTEXPR_CONST int AspirationSearchMinDepth = 6 * Searcher::Depth1Ply;

// extensions
CONSTEXPR_CONST int ExtensionDepthForCheck     = Searcher::Depth1Ply * 3 / 4;
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
  return depth < Searcher::Depth1Ply * 4 ? Searcher::Depth1Ply * 1
                                         : depth - Searcher::Depth1Ply * 3;
}

/**
 * Calculate a depth of null move search.
 */
inline int nullDepth(int depth, Score standPat, Score beta) {
  return depth * 11 / 16
       - Searcher::Depth1Ply * 7 / 2
       - std::max(Searcher::Depth1Ply * ((int)standPat.raw() - (int)beta.raw()) / 150, 0);
}

/**
 * values for reducing from the depth.
 */
uint8_t ReductionDepth[2][64][64];

void initializeReductionDepth() {
  for (int d = 1; d < 64; d++) {
    for (int mc = 0; mc < 64; mc++) {
      double r = 0.7 * log(d) + 0.4 * log(mc + 1);
      if (r < 0.8) {
        continue;
      }

      ReductionDepth[0][d][mc] = std::max(r - 1.0, 0.0) * Searcher::Depth1Ply;
      ReductionDepth[1][d][mc] = r * Searcher::Depth1Ply;
    }
  }
}

/**
 * Returns reducing depth.
 */
inline
int reductionDepth(int depth,
                   bool isNullWindow,
                   int mc) {
  return ReductionDepth[isNullWindow ? 1: 0]
                       [std::min(depth / Searcher::Depth1Ply, 63)]
                       [std::min(mc, 63)];
}

uint16_t RazorMargin[4] = { 300, 400, 400, 450 };

/**
 * Returnes the margin of razoring
 */
inline
Score razorMargin(int depth) {
  return RazorMargin[depth / Searcher::Depth1Ply];
}

/**
 * the maximum depth of futility pruning.
 */
CONSTEXPR_CONST int FutilityPruningMaxDepth = 7 * Searcher::Depth1Ply;

/**
 * Returns the margin of futility pruning.
 */
inline
Score futilityPruningMargin(int depth) {
  return depth * (300 / Searcher::Depth1Ply);
}

const int HalfDensity[][9] = {
  { 2, 0, 1 },
  { 2, 1, 0 },
  { 4, 0, 0, 1, 1 },
  { 4, 0, 1, 1, 0 },
  { 4, 1, 1, 0, 0 },
  { 4, 1, 0, 0, 1 },
  { 6, 0, 0, 0, 1, 1, 1 },
  { 6, 0, 0, 1, 1, 1, 0 },
  { 6, 0, 1, 1, 1, 0, 0 },
  { 6, 1, 1, 1, 0, 0, 0 },
  { 6, 1, 1, 0, 0, 0, 1 },
  { 6, 1, 0, 0, 0, 1, 1 },
  { 8, 0, 0, 0, 0, 1, 1, 1, 1 },
  { 8, 0, 0, 0, 1, 1, 1, 1, 0 },
  { 8, 0, 0, 1, 1, 1, 1, 0, 0 },
  { 8, 0, 1, 1, 1, 1, 0, 0, 0 },
  { 8, 1, 1, 1, 1, 0, 0, 0, 0 },
  { 8, 1, 1, 1, 0, 0, 0, 0, 1 },
  { 8, 1, 1, 0, 0, 0, 0, 1, 1 },
  { 8, 1, 0, 0, 0, 0, 1, 1, 1 },
};

CONSTEXPR_CONST int HalfDensitySize = std::extent<decltype(HalfDensity)>::value;

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
}

Searcher::Searcher() :
  config_ (getDefaultSearchConfig()),
  evaluator_(Evaluator::sharedEvaluator()),
  treeSize_(0),
  handler_(nullptr) {
}

Searcher::Searcher(std::shared_ptr<Evaluator> evaluator) :
  config_ (getDefaultSearchConfig()),
  evaluator_(evaluator),
  treeSize_(0),
  handler_(nullptr) {
}

void Searcher::clean() {
  tt_.clear();
  history_.clear();
  timeManager_.clearGame();
}

void Searcher::onSearchStarted() {
  timer_.start();

  interrupted_ = false;

  result_.move = Move::none();
  result_.score = -Score::infinity();
  result_.pv.clear();
  result_.depth = 0;
  result_.elapsed = 0.0f;

  history_.reduce();

  timeManager_.clearPosition(config_.optimumTimeMs,
                             config_.maximumTimeMs);

  if (treeSize_ != config_.numberOfThreads) {
    if (treeSize_ != 0) {
      delete[] trees_;
    }
    treeSize_ = config_.numberOfThreads;
    trees_ = new Tree[treeSize_];
  }

  for (int ti = 0; ti < treeSize_; ti++) {
    trees_[ti].index = ti;
    trees_[ti].completedDepth = 0;
  }

  if (handler_ != nullptr) {
    handler_->onStart(*this);
  }
}

void Searcher::onSearchStopped() {
  interrupt();

  for (int ti = 0; ti < treeSize_; ti++) {
    if (trees_[ti].thread.joinable()) {
      trees_[ti].thread.join();
    }
  }
}

void Searcher::updateInfo() {
  initializeSearchInfo(info_);
  mergeSearchInfo(info_, trees_[0].info);
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

  auto& tree = trees_[0];
  initializeTree(tree,
                 pos,
                 *evaluator_,
                 record);

  Score score = search(tree,
                       depth,
                       alpha,
                       beta,
                       NodeStat::normal());

  auto& node = tree.nodes[tree.ply];

  if (node.pv.size() >= 1) {
    result_.move = node.pv.getMove(0);
  } else {
    result_.move = Move::none();
  }
  result_.score = score;
  result_.pv = node.pv;
  result_.depth = depth;
  result_.elapsed = timer_.elapsed();

  onSearchStopped();
}

/**
 * iterative deepening search
 */
void Searcher::idsearch(const Position& pos,
                        int maxDepth,
                        Record* record /*= nullptr*/) {
  onSearchStarted();

  for (int ti = 1; ti < treeSize_; ti++) {
    trees_[ti].thread = std::thread([this, ti, &pos, maxDepth, record]() {
      idsearch(trees_[ti], pos, maxDepth, record);
    });
  }

  idsearch(trees_[0],
           pos,
           maxDepth,
           record);

  onSearchStopped();

  for (int ti = 0; ti < treeSize_; ti++) {
    auto& tree = trees_[ti];
    auto& node = tree.nodes[tree.ply];
    if (tree.completedDepth > result_.depth) {
      result_.move = node.moves[0].excludeExtData();
      result_.score = moveToScore(node.moves[0]);
      result_.pv = node.pv;
      result_.depth = tree.completedDepth;
      result_.elapsed = timer_.elapsed();
      if (ti != 0 && handler_ != nullptr) {
        handler_->onUpdatePV(*this, result_.pv, result_.elapsed, result_.depth, result_.score);
      }
    }
  }
}

void Searcher::idsearch(Tree& tree,
                        const Position& pos,
                        int maxDepth,
                        Record* record) {
  bool isMainThread = tree.index == 0;

  initializeTree(tree,
                 pos,
                 *evaluator_,
                 record);

  arrive(tree);

  auto& node = tree.nodes[tree.ply];
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

  for (int depth = Depth1Ply * 3 / 2; ; depth += Depth1Ply) {
    if (!isMainThread) {
      const int* row = HalfDensity[(tree.index - 1) % HalfDensitySize];
      if (row[(depth / Depth1Ply) % row[0] + 1]) {
        continue;
      }
    }

    bool cont = aspsearch(tree, depth);

    if (isInterrupted()) {
      break;
    }

    tree.completedDepth = depth;

    if (!cont || depth >= maxDepth) {
      break;
    }
  }

  return;
}

/**
 * aspiration search
 */
bool Searcher::aspsearch(Tree& tree,
                         int depth) {
  auto& node = tree.nodes[tree.ply];
  bool isMainThread = tree.index == 0;

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
  int maxAlphaIndex = sizeof(alphas) / sizeof(alphas[0]) - 1;
  int maxBetaIndex = sizeof(betas) / sizeof(betas[0]) - 1;
  int alphaIndex = doAsp ? 0 : maxAlphaIndex;
  int betaIndex = doAsp ? 0 : maxBetaIndex;

  Score bestScore = -Score::infinity();

  bool doFullSearch = true;

  for (Moves::size_type i = 1; i < node.moves.size(); i++) {
    setScoreToMove(node.moves[i], -Score::infinity());
  }

  // expand branches
  int maxMoveCount = (int)node.moves.size() - 1;
  for (int moveCount = 0; moveCount <= maxMoveCount;) {
    Score alpha = std::max(alphas[alphaIndex], bestScore);
    Score beta = betas[betaIndex];

    Move move = node.moves[moveCount];
    int newDepth = depth - Depth1Ply;

    // late move reduction
    int reduced = 0;
    if (!doFullSearch &&
        newDepth >= Depth1Ply &&
        !isCheck(node.checkState) &&
        !isTacticalMove(tree.position, move)) {
      reduced = reductionDepth(newDepth,
                               false,
                               moveCount);
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
    if (doFullSearch) {
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

      if (!isInterrupted() &&
          score > alpha &&
          (reduced != 0 || score < beta)) {
        newDepth = newDepth + reduced;
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
    if (score <= alphas[alphaIndex] && score > bestScore) {
      for (; score <= alphas[alphaIndex] && alphaIndex < maxAlphaIndex; alphaIndex++) {}

      if (alphas[alphaIndex] < score) {
        doFullSearch = true;
        if (isMainThread && handler_ != nullptr) {
          PV pv;
          pv.set(move, depth, pv);
          handler_->onFailLow(*this, pv, timer_.elapsed(), depth, score);
        }
        continue;
      }
    }

    setScoreToMove(node.moves[moveCount], score);

    // fail-high
    if (score >= beta) {
      for (; score >= betas[betaIndex] && betaIndex < maxBetaIndex; betaIndex++) {}

      if (betas[betaIndex] > score) {
        doFullSearch = true;
        if (isMainThread && handler_ != nullptr) {
          PV pv;
          pv.set(move, depth, pv);
          handler_->onFailHigh(*this, pv, timer_.elapsed(), depth, score);
        }
        continue;
      }
    }

    if (score > bestScore) {
      bestScore = score;

      auto& childNode = tree.nodes[tree.ply+1];
      node.pv.set(move, depth, childNode.pv);
      if (isMainThread && handler_ != nullptr) {
        handler_->onUpdatePV(*this, node.pv, timer_.elapsed(), depth, bestScore);
      }
    }

    if (isMainThread) {
      timeManager_.update(timer_.elapsedMs(),
                          depth,
                          bestScore,
                          node.pv,
                          moveCount,
                          maxMoveCount);
      if (timeManager_.shouldInterrupt()) {
        interrupt();
        break;
      }
    }

    moveCount++;

    doFullSearch = false;
  }

  std::stable_sort(node.moves.begin(), node.moves.end(), [](Move lhs, Move rhs) {
    return moveToScore(lhs) > moveToScore(rhs);
  });

  if (node.pv.size() != 0 && bestScore != -Score::infinity()) {
    storePV(tree,
            node.pv,
            0,
            bestScore);
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

  arrive(tree);

  auto& node = tree.nodes[tree.ply];

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

  case ShekState::Superior:
    node.isHistorical = true;
    return Score::infinity() - tree.ply;

  case ShekState::Inferior:
    node.isHistorical = true;
    return -Score::infinity() + tree.ply;

  default:
    break;
  }

  // quiesence search
  if (depth < Depth1Ply) {
    return quies(tree,
                 0,
                 alpha,
                 beta);
  }

  tree.info.nodes++;

  if (tree.ply == Tree::StackSize - 2) {
    node.isHistorical = true;
    return calculateStandPat(tree, *evaluator_);
  }

  // distance pruning
  Score lowerScore = -Score::infinity() + tree.ply;
  Score upperScore = Score::infinity() - tree.ply - 1;
  if (lowerScore >= beta) {
    return lowerScore;
  } else if (upperScore <= alpha) {
    return upperScore;
  }

  bool isNullWindow = alpha + 1 == beta;

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
         (ttScoreType == TTScoreType::Upper && ttScore <= alpha) ||
         (ttScoreType == TTScoreType::Lower && ttScore >= beta)) {
        tree.info.hashCut++;
        return ttScore;
      }
    }

    if (!shouldRecursiveIDSearch(depth) ||
        ttDepth >= recursiveIDSearchDepth(depth)) {
      // if the score is smaller than beta, exclude null window search.
      if (ttScore < beta &&
          (ttScoreType == TTScoreType::Exact ||
           ttScoreType == TTScoreType::Upper)) {
        nodeStat.unsetNullMoveSearch();
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

  // mate 1-ply
  if (nodeStat.isMateDetection() &&
      !isCheck(node.checkState) &&
      Mate::mate1Ply(tree.position)) {
    return Score::infinity() - tree.ply - 1;
  }

  Score standPat = calculateStandPat(tree, *evaluator_);

  // futility pruning
  if (!isCheck(node.checkState) &&
      depth < FutilityPruningMaxDepth &&
      standPat - futilityPruningMargin(depth) >= beta) {
    tree.info.futilityPruning++;
    return standPat - futilityPruningMargin(depth);
  }

  // razoring
  if (isNullWindow &&
      !isCheck(node.checkState) &&
      depth < Depth1Ply * 4 &&
      standPat + razorMargin(depth) <= alpha &&
      node.hashMove.isNone()) {
    Score razorAlpha = alpha - razorMargin(depth);
    Score score = quies(tree,
                        0,
                        razorAlpha,
                        razorAlpha + 1);
    if (score <= razorAlpha) {
      tree.info.razoring++;
      return score;
    }

    rearrive(tree);
  }

  // null move pruning
  if (isNullWindow &&
      standPat >= beta &&
      nodeStat.isNullMoveSearch() &&
      !isCheck(node.checkState) &&
      !nodeStat.isMateThreat()) {
    int newDepth = nullDepth(depth, standPat, beta);
    NodeStat newNodeStat = NodeStat::normal().unsetNullMoveSearch();

    doNullMove(tree);

    Score score = newDepth < Depth1Ply
        ? -quies(tree,
                 0,
                 -beta,
                 -beta+1)
        : -search(tree,
                  newDepth,
                  -beta,
                  -beta+1,
                  newNodeStat);

    undoNullMove(tree);

    if (score >= beta) {
      auto& childNode = tree.nodes[tree.ply+1];
      node.isHistorical = childNode.isHistorical;
      tree.info.nullMovePruning++;
      tt_.store(tree.position.getHash(),
                alpha,
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
  if (shouldRecursiveIDSearch(depth) &&
      node.hashMove.isNone() &&
      nodeStat.isRecursiveIDSearch() &&
      !isCheck(node.checkState)) {
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

    rearrive(tree);

    TTElement tte;
    if (tt_.get(tree.position.getHash(), tte)) {
      Move ttMove = tte.move();
      if (tree.position.isLegalMoveMaybe(ttMove, node.checkState)) {
        node.hashMove = ttMove;
      }
    }
  }

  bool isFirst = true;
  Score bestScore = lowerScore;
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
        !isCheck(node.checkState) &&
        !isTacticalMove(tree.position, move) &&
        !isPriorMove(tree, move) &&
        !nodeStat.isMateThreat()) {
      reduced = reductionDepth(newDepth,
                               isNullWindow,
                               moveCount);
      newDepth = newDepth - reduced;
    }

    Score newAlpha = std::max(alpha, bestScore);

    // futility pruning
    if (!currentMoveIsCheck &&
        !isCheck(node.checkState) &&
        newDepth < FutilityPruningMaxDepth &&
        newAlpha > -Score::mate() &&
        !isPriorMove(tree, move)) {
      Score futScore = estimateScore(tree, move, *evaluator_)
                     + 500 + futilityPruningMargin(newDepth);
      if (futScore <= newAlpha) {
        isFirst = false;
        bestScore = std::max(bestScore, futScore);
        tree.info.futilityPruning++;
        continue;
      }
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
                      -newAlpha,
                      newNodeStat);
    } else {
      // nega-scout
      score = -search(tree,
                      newDepth,
                      -(newAlpha + 1),
                      -newAlpha,
                      newNodeStat);

      if (!isInterrupted() &&
          score > newAlpha &&
          (reduced != 0 || score < beta)) {
        newDepth = newDepth + reduced;
        score = -search(tree,
                        newDepth,
                        -beta,
                        -newAlpha,
                        newNodeStat);
      }
    }

    undoMove(tree);

    if (isInterrupted()) {
      return Score::zero();
    }

    auto& childNode = tree.nodes[tree.ply+1];

    if (score > bestScore) {
      bestScore = score;
      bestMove = move;

      // beta cut
      if (score >= beta) {
        node.isHistorical = childNode.isHistorical;
        tree.info.failHigh++;
        if (isFirst) {
          tree.info.failHighFirst++;
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
    unsigned hval = std::max(depth / (Depth1Ply / 4), 1);
    for (auto& move : node.moves) {
      history_.add(tree.position.getTurn(),
                   move,
                   hval,
                   move == bestMove ? hval : 0);
    }
  }

  if (!node.isHistorical) {
    tt_.store(tree.position.getHash(),
              alpha,
              beta,
              bestScore,
              depth,
              tree.ply,
              bestMove,
              nodeStat.isMateThreat());
  }

  return bestScore;
}

/**
 * quiesence search
 */
Score Searcher::quies(Tree& tree,
                      int depth,
                      Score alpha,
                      Score beta) {
  arrive(tree);

  auto& node = tree.nodes[tree.ply];

  tree.info.quiesNodes++;

  node.checkState = tree.position.getCheckState();

  Score bestScore = alpha;

  if (!isCheck(node.checkState)) {
    Score standPat = calculateStandPat(tree, *evaluator_);
    if (standPat > bestScore) {
      bestScore = standPat;
      if (bestScore >= beta) {
        return bestScore;
      }
    }
  } else {
    bestScore = std::max(bestScore, -Score::infinity() + tree.ply);
  }

  if (tree.ply == Tree::StackSize - 2) {
    node.isHistorical = true;
    return calculateStandPat(tree, *evaluator_);
  }

  bool isNullWindow = alpha + 1 == beta;

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
         (ttScoreType == TTScoreType::Upper && ttScore <= bestScore) ||
         (ttScoreType == TTScoreType::Lower && ttScore >= beta)) {
        tree.info.hashCut++;
        return ttScore;
      }
    }
  }

  if (!isCheck(node.checkState) &&
      Mate::mate1Ply(tree.position)) {
    bestScore = Score::infinity() - tree.ply - 1;
    return bestScore;
  }

  generateMovesOnQuies(tree,
                       depth,
                       bestScore);

  // expand branches
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
                         depth - Depth1Ply,
                         -beta,
                         -bestScore);

    undoMove(tree);

    if (isInterrupted()) {
      return Score::zero();
    }

    if (score > bestScore) {
      bestScore = score;

      auto& childNode = tree.nodes[tree.ply+1];
      node.pv.set(move, 0, childNode.pv);

      // beta cut
      if (score >= beta) {
        break;
      }
    }
  }

  tt_.store(tree.position.getHash(),
            alpha,
            beta,
            bestScore,
            depth,
            tree.ply,
            Move::none(),
            false);

  return bestScore;
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
    node.moves.add(node.killerMove1);
  }

  if (!isRootNode &&
      !isCheck(node.checkState) &&
      hasKiller2(tree) &&
      isKiller2Good(tree) &&
      isKiller2Legal(tree)) {
    node.moves.add(node.killerMove2);
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
                     false /* excludeNegative */);
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
                                    int depth,
                                    Score alpha) {
  auto& node = tree.nodes[tree.ply];
  bool excludeSmallCaptures = depth <= -6;

  node.moves.clear();
  node.moveIterator = node.moves.begin();

  if (!isCheck(node.checkState)) {
    MoveGenerator::generateCapturingMoves(tree.position, node.moves);

    for (auto ite = node.moveIterator; ite != node.moves.end(); ) {
      auto& move = *ite;

      if (excludeSmallCaptures) {
        auto piece = tree.position.getPieceOnBoard(move.from());
        auto captured = tree.position.getPieceOnBoard(move.to());
        if ((captured.type() == PieceType::pawn() && !move.isPromotion()) ||
            (captured.isEmpty() && piece.type() != PieceType::pawn())) {
          ite = node.moves.remove(ite);
          continue;
        }
      }

      // futility pruning
      Score estScore = estimateScore(tree, move, *evaluator_);
      if (estScore + 500 <= alpha) {
        ite = node.moves.remove(ite);
        tree.info.futilityPruning++;
        continue;
      }

      ite++;
    }

    SEE::sortMoves(tree.position,
                   node.moves,
                   node.moveIterator,
                   true /* excludeNegative */);
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

    Score score = -quies(tree,
                         0,
                         -Score::infinity(),
                         Score::infinity());

    if (move.isPromotion()) {
      score += 1;
    }

    setScoreToMove(node.moves[moveCount], score);

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
