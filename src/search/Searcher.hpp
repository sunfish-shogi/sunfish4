/* Searcher.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_SEARCH_SEARCHER_HPP__
#define SUNFISH_SEARCH_SEARCHER_HPP__

#include "eval/Value.hpp"
#include "search/SearchInfo.hpp"
#include "search/SearchResult.hpp"
#include "search/tree/Tree.hpp"
#include "search/tree/Worker.hpp"
#include "search/eval/Evaluator.hpp"
#include "common/math/Random.hpp"
#include <atomic>
#include <array>

namespace sunfish {

class Position;
class Move;

class Searcher {
public:

  using ValueArray = std::array<Value, MAX_NUMBER_OF_MOVES>;

  static CONSTEXPR_CONST int Depth1Ply = 8;

  class Handler {
  public:
    virtual void onUpdatePV(const PV& pv, int depth, Value value) = 0;
    virtual void onFailLow(const PV& pv, int depth, Value value) = 0;
    virtual void onFailHigh(const PV& pv, int depth, Value value) = 0;
  };

  Searcher();

  bool search(const Position& pos,
              int depth) {
    return search(pos,
                  depth,
                  -Value::infinity(),
                  Value::infinity());
  }

  bool search(const Position& pos,
              int depth,
              Value alpha,
              Value beta);

  /**
   * iterative deepening search.
   */
  bool idsearch(const Position& pos,
                int depth);

  const SearchResult& getResult() {
    return result_;
  }

  const SearchInfo& getInfo() {
    return info_;
  }

  void interrupt() {
    interrupted_ = true;
  }

  void setHandler(Handler* handler) {
    handler_ = handler;
  }

private:

  void onSearchStarted();

  void updateInfo();

  void generateMovesOnRoot(Tree& tree);

  bool aspsearch(Tree& tree,
                 int depth,
                 ValueArray& values);

  Value search(Tree& tree,
               int depth,
               Value alpha,
               Value beta);

  Value quies(Tree& tree,
              Value alpha,
              Value beta);

  Move nextMove(Node& node);

  Random random_;

  Evaluator evaluator_;

  Tree treeOnMainThread_;
  Worker workerOnMainThread_;

  SearchResult result_;
  SearchInfo info_;

  std::atomic<bool> interrupted_;

  Handler* handler_;

};

} // namespace sunfish

#endif // SUNFISH_SEARCH_SEARCHER_HPP__
