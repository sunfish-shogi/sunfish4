/* Searcher.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_SEARCH_SEARCHER_HPP__
#define SUNFISH_SEARCH_SEARCHER_HPP__

#include "search/eval/Score.hpp"
#include "search/SearchConfig.hpp"
#include "search/SearchInfo.hpp"
#include "search/SearchResult.hpp"
#include "search/SearchHandler.hpp"
#include "search/time/TimeManager.hpp"
#include "search/tree/Tree.hpp"
#include "search/tree/NodeStat.hpp"
#include "search/tt/TT.hpp"
#include "search/history/History.hpp"
#include "common/math/Random.hpp"
#include "common/time/Timer.hpp"
#include <memory>
#include <atomic>
#include <array>
#include <climits>

namespace sunfish {

class Position;
class Move;
struct Record;
class Evaluator;

class Searcher {
public:

  static CONSTEXPR_CONST int Depth1Ply = 8;
  static CONSTEXPR_CONST int DepthInfinity = INT_MAX;

  static void initialize();

  Searcher();

  Searcher(std::shared_ptr<Evaluator> evaluator);

  void clean();

  void search(const Position& pos,
              int depth,
              Record* record = nullptr) {
    search(pos,
           depth,
           -Score::infinity(),
           Score::infinity(),
           record);
  }

  void search(const Position& pos,
              int depth,
              Score alpha,
              Score beta,
              Record* record = nullptr);

  /**
   * iterative deepening search.
   */
  void idsearch(const Position& pos,
                int maxDepth,
                Record* record = nullptr);

  const SearchConfig& getConfig() const {
    return config_;
  }

  void setConfig(const SearchConfig& config) {
    config_ = config;
  }

  const SearchResult& getResult() const {
    return result_;
  }

  const SearchInfo& getInfo() const {
    return info_;
  }

  void interrupt() {
    interrupted_ = true;
  }

  void setHandler(SearchHandler* handler) {
    handler_ = handler;
  }

  float ttUsageRates() const {
    return tt_.usageRates();
  }

  void ttResizeMB(unsigned mebiBytes) {
    tt_.resizeMB(mebiBytes);
  }

private:

  void onSearchStarted();
  void onSearchStopped();

  void updateInfo();

  void idsearch(Tree& tree,
                const Position& pos,
                int maxDepth,
                Record* record);

  bool aspsearch(Tree& tree,
                 int depth);

  Score search(Tree& tree,
               int depth,
               Score alpha,
               Score beta,
               NodeStat nodeStat);

  Score quies(Tree& tree,
              int depth,
              Score alpha,
              Score beta);

  template <bool isRootNode>
  void generateMoves(Tree& tree);

  Move nextMove(Tree& tree);

  void generateMovesOnQuies(Tree& tree,
                            int depth,
                            Score alpha);

  Move nextMoveOnQuies(Node& node);

  void sortMovesOnHistory(Tree& tree);

  void sortRootMoves(Tree& tree);

  void storePV(Tree& tree,
               const PV& pv,
               unsigned ply,
               Score score);

  bool isInterrupted() const {
    if (interrupted_.load(std::memory_order_relaxed)) {
      return true;
    }

    if (timer_.elapsedMs() >= config_.maximumTimeMs) {
      return true;
    }

    return false;
  }

  SearchConfig config_;
  SearchResult result_;
  SearchInfo info_;

  std::atomic_bool interrupted_;
  Timer timer_;

  std::shared_ptr<Evaluator> evaluator_;

  TT tt_;

  History history_;

  Tree* trees_;
  int treeSize_;

  Random random_;

  TimeManager timeManager_;

  SearchHandler* handler_;

};

} // namespace sunfish

#endif // SUNFISH_SEARCH_SEARCHER_HPP__
