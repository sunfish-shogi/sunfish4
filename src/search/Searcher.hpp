/* Searcher.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_SEARCH_SEARCHER_HPP__
#define SUNFISH_SEARCH_SEARCHER_HPP__

#include "eval/Value.hpp"
#include "search/tree/Tree.hpp"
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

  struct Info {
    Move move;
    Value value;
    PV pv;
  };

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

  const Info& getInfo() {
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

  Info info_;

  std::atomic<bool> interrupted_;

  Handler* handler_;

};

} // namespace sunfish

#endif // SUNFISH_SEARCH_SEARCHER_HPP__
