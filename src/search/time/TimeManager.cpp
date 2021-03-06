/* TimeManager.cpp
 *
 * Kubo Ryosuke
 */

#include "search/time/TimeManager.hpp"
#include "search/Searcher.hpp"

namespace {

using namespace sunfish;

PV::SizeType getPvStability(const PV& pv, const PV& pv2) {
  PV::SizeType pvStability = 0;
  for (; pvStability < pv.size() &&
         pvStability < pv2.size(); pvStability++) {
    if (pv.getMove(pvStability) != pv2.getMove(pvStability)) {
      break;
    }
  }
  return pvStability;
}

} // namespace

namespace sunfish {

TimeManager::TimeManager() :
    previous2_(new History),
    previous_(new History),
    current_(new History) {
}

void TimeManager::clearGame() {
}

void TimeManager::clearPosition(SearchConfig::TimeType optimumTimeMs,
                                SearchConfig::TimeType maximumTimeMs) {
  optimumTimeMs_ = optimumTimeMs;
  maximumTimeMs_ = maximumTimeMs;

  shouldInterrupt_ = false;

  previous2_->depth = 0;
  previous_->depth = 0;
  current_->depth = 0;
}

void TimeManager::update(uint32_t elapsedMs,
                         int depth,
                         Score score,
                         const PV& pv) {
  if (current_->depth != 0 &&
      current_->depth != depth) {
    *previous2_ = *previous_;
    *previous_ = *current_;
  }

  current_->depth = depth;
  current_->score = score;
  current_->pv = pv;

  // if 80% of maximumTimeMs is already used
  if (maximumTimeMs_ != SearchConfig::InfinityTime &&
      elapsedMs * 100 >= maximumTimeMs_ * 80) {
    shouldInterrupt_ = true;
    LOG(info) << "TimeManager: interrupt(97% of MAX)";
    return;
  }

  if (previous2_->depth == 0 || previous_->depth == 0) {
    // do nothing
    return;
  }

  if (optimumTimeMs_ == SearchConfig::InfinityTime) {
    // do nothing
    return;
  }

  Score scoreDiff = current_->score - previous_->score;

  PV::SizeType pvStability = getPvStability(current_->pv, previous_->pv);
  PV::SizeType pvStability2 = getPvStability(current_->pv, previous2_->pv);

  // 10% of optimumTimeMs_
  if (elapsedMs * 100 >= optimumTimeMs_ * 10 &&
      depth >= Searcher::Depth1Ply * 25 &&
      scoreDiff < 32 && scoreDiff > -16) {
    if (pvStability >= 14 || pvStability2 >= 15) {
      shouldInterrupt_ = true;
      LOG(info) << "TimeManager: interrupt(10% of OPTIM, L" << __LINE__ << ")";
      return;
    }
  }

  // 20% of optimumTimeMs_
  if (elapsedMs * 100 >= optimumTimeMs_ * 20 &&
      depth >= Searcher::Depth1Ply * 23 &&
      scoreDiff < 64 && scoreDiff > -32) {
    if (pvStability >= 12 || pvStability2 >= 13) {
      shouldInterrupt_ = true;
      LOG(info) << "TimeManager: interrupt(20% of OPTIM, L" << __LINE__ << ")";
      return;
    }
  }

  // 50% of optimumTimeMs_
  if (elapsedMs * 100 >= optimumTimeMs_ * 70 &&
      depth >= Searcher::Depth1Ply * 18 &&
      scoreDiff < 256 && scoreDiff > -64) {
    if (pvStability >= 4 || pvStability2 >= 5) {
      shouldInterrupt_ = true;
      LOG(info) << "TimeManager: interrupt(50% of OPTIM, L" << __LINE__ << ")";
      return;
    }
  }

  // 100% of optimumTimeMs_
  if (elapsedMs >= optimumTimeMs_ &&
      depth >= Searcher::Depth1Ply * 18 &&
      scoreDiff < 512 && scoreDiff > -128) {
    if (pvStability >= 3 || pvStability2 >= 3) {
      shouldInterrupt_ = true;
      LOG(info) << "TimeManager: interrupt(100% of OPTIM, L" << __LINE__ << ")";
      return;
    }
  }

  // 200% of optimumTimeMs_
  if (elapsedMs * 100 >= optimumTimeMs_ * 200 &&
      depth >= Searcher::Depth1Ply * 18 &&
      scoreDiff < 1024 && scoreDiff > -256) {
    if (pvStability >= 2 || pvStability2 >= 2) {
      shouldInterrupt_ = true;
      LOG(info) << "TimeManager: interrupt(200% of OPTIM, L" << __LINE__ << ")";
      return;
    }
  }

  // 400% of optimumTimeMs_
  if (elapsedMs * 100 >= optimumTimeMs_ * 400 &&
      depth >= Searcher::Depth1Ply * 18 &&
      scoreDiff < 2048 && scoreDiff > -512) {
    if (pvStability >= 1 || pvStability2 >= 1) {
      shouldInterrupt_ = true;
      LOG(info) << "TimeManager: interrupt(400% of OPTIM, L" << __LINE__ << ")";
      return;
    }
  }
}

} // namespace sunfish
