/* SEE.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_SEARCH_SEE_SEE_HPP__
#define SUNFISH_SEARCH_SEE_SEE_HPP__

#include "core/position/Position.hpp"
#include "core/move/Moves.hpp"
#include "core/move/Move.hpp"
#include "search/eval/Score.hpp"
#include <utility>

void test_method_ScoreTest_testGenerateAttackers();

namespace sunfish {

class SEE {
public:

  struct Attacker {
    Score prom;
    Score exch;
  };

  struct Attackers {
    int num;
    Attacker list[10];
  };

  using AttackerSet = std::tuple<Attackers, Attackers>;

  SEE() = delete;

  static Score calculate(const Position& position,
                         Move move);

  static void sortMoves(const Position& position,
                        Moves& moves,
                        Moves::iterator begin,
                        bool excludeSmallCaptures);

private:

  static AttackerSet generateAttackers(const Position& position,
                                       Square from,
                                       Square to);

  static Score search(const Attackers& ba,
                      const Attackers& wa,
                      int b,
                      int w,
                      Score score,
                      Score materialScore);

  friend void ::test_method_ScoreTest_testGenerateAttackers();

};

} // namespace sunfish

#endif // SUNFISH_SEARCH_SEE_SEE_HPP__
