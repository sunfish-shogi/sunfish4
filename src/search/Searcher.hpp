/* Searcher.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_SEARCH_SEARCHER_HPP__
#define SUNFISH_SEARCH_SEARCHER_HPP__

#include "eval/Score.hpp"
#include "search/SearchConfig.hpp"
#include "search/SearchInfo.hpp"
#include "search/SearchResult.hpp"
#include "search/SearchHandler.hpp"
#include "search/tree/Tree.hpp"
#include "search/tree/NodeStat.hpp"
#include "search/tree/Worker.hpp"
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
class Record;
class Evaluator;

class Searcher {
public:

  static CONSTEXPR_CONST int Depth1Ply = 8;
  static CONSTEXPR_CONST int DepthInfinity = INT_MAX;

  static void initialize();

  Searcher();

  Searcher(std::shared_ptr<Evaluator> evaluator);

  bool search(const Position& pos,
              int depth,
              Record* record = nullptr) {
    return search(pos,
                  depth,
                  -Score::infinity(),
                  Score::infinity(),
                  record);
  }

  bool search(const Position& pos,
              int depth,
              Score alpha,
              Score beta,
              Record* record = nullptr);

  /**
   * iterative deepening search.
   */
  bool idsearch(const Position& pos,
                int depth,
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

private:

  void onSearchStarted();

  void updateInfo();

  bool aspsearch(Tree& tree,
                 int depth);

  Score search(Tree& tree,
               int depth,
               Score alpha,
               Score beta,
               NodeStat nodeStat);

  Score quies(Tree& tree,
              int qply,
              Score alpha,
              Score beta);

  void generateMoves(Tree& tree);

  Move nextMove(Tree& tree);

  void generateMovesOnQuies(Tree& tree, int qply);

  Move nextMoveOnQuies(Node& node);

  void sortMovesOnHistory(Tree& tree);

  void storePV(Tree& tree, const PV& pv, unsigned ply);

  bool isInterrupted() const {
    if (interrupted_) {
      return true;
    }

    if (timer_.elapsedMilliSeconds() >= config_.maximumMilliSeconds) {
      return true;
    }

    return false;
  }

  SearchConfig config_;
  SearchResult result_;
  SearchInfo info_;

  std::atomic<bool> interrupted_;
  Timer timer_;

  std::shared_ptr<Evaluator> evaluator_;

  TT tt_;

  History history_;

  Tree treeOnMainThread_;
  Worker workerOnMainThread_;

  Random random_;

  SearchHandler* handler_;

};

} // namespace sunfish

#endif // SUNFISH_SEARCH_SEARCHER_HPP__
