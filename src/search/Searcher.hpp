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

namespace sunfish {

class Position;
class Move;

class Searcher {
public:

  static CONSTEXPR_CONST int Depth1Ply = 8;

  struct Info {
    Move move;
    Value value;
    PV pv;
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

  const Info& getInfo() {
    return info_;
  }

private:

  Value search(Tree& tree,
               int ply,
               int depth,
               Value alpha,
               Value beta);

  Random random_;

  Evaluator evaluator_;
  Tree treeOnMainThread_;

  Info info_;

};

} // namespace sunfish

#endif // SUNFISH_SEARCH_SEARCHER_HPP__
