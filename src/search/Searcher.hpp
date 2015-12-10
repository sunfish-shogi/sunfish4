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
#include "search/tree/Worker.hpp"
#include "search/eval/Evaluator.hpp"
#include "common/math/Random.hpp"
#include "common/time/Timer.hpp"
#include <atomic>
#include <array>

namespace sunfish {

class Position;
class Move;

class Searcher {
public:

  using ScoreArray = std::array<Score, MAX_NUMBER_OF_MOVES>;

  static CONSTEXPR_CONST int Depth1Ply = 8;

  Searcher();

  bool search(const Position& pos,
              int depth) {
    return search(pos,
                  depth,
                  -Score::infinity(),
                  Score::infinity());
  }

  bool search(const Position& pos,
              int depth,
              Score alpha,
              Score beta);

  /**
   * iterative deepening search.
   */
  bool idsearch(const Position& pos,
                int depth);

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
                 int depth,
                 ScoreArray& scores);

  Score search(Tree& tree,
               int depth,
               Score alpha,
               Score beta);

  Score quies(Tree& tree,
              Score alpha,
              Score beta);

  void generateMovesOnRoot(Tree& tree);

  Move nextMoveOnRoot(Node& node);

  void generateMoves(Tree& tree);

  Move nextMove(Tree& tree);

  void generateMovesOnQuies(Tree& tree);

  Move nextMoveOnQuies(Node& node);

  bool isInterrupted() const {
    if (interrupted_) {
      return true;
    }

    if (timer_.getElapsedInt() >= config_.maximumTimeSeconds) {
      return true;
    }

    return false;
  }

  Evaluator evaluator_;

  Tree treeOnMainThread_;
  Worker workerOnMainThread_;

  SearchConfig config_;
  SearchResult result_;
  SearchInfo info_;

  std::atomic<bool> interrupted_;
  Timer timer_;

  Random random_;

  SearchHandler* handler_;

};

} // namespace sunfish

#endif // SUNFISH_SEARCH_SEARCHER_HPP__
