/* TimeManagerTest.cpp
 *
 * Kubo Ryosuke
 */

#include "test/Test.hpp"
#include "search/time/TimeManager.hpp"
#include "search/Searcher.hpp"

using namespace sunfish;

TEST(TimeManagerTest, testTimeManager) {
  Move move1(Square::s27(), Square::s26(), false);
  Move move2(Square::s83(), Square::s84(), false);
  Move moves1[] = { move1, move2, move1, move2, move1, move2, move1, move2, move1, move2, };
  Move moves2[] = { move2, move1, move2, move1, move2, move1, move2, move1, move2, move1, };
  PV pv1(10, moves1);
  PV pv2(10, moves2);

  auto inf = SearchConfig::InfinityTime;
  struct Scenario {
    bool clear;
    SearchConfig::TimeType optimumTimeMs;
    SearchConfig::TimeType maximumTimeMs;
    uint32_t elapsedMs;
    int depth;
    Score score;
    PV pv;
    bool shouldInterrupt;
  };
  Scenario scenario[] = {
    // clear optimumTimeMs maximumTimeMs elapsedMs depth score   pv shouldInterrupt
    {   true,          inf,          inf,    10000,   10,  100, pv1,          false },
    {  false,          inf,          inf,   581000,   11,  100, pv1,          false },
    {  false,          inf,          inf,   582000,   12,  100, pv2,          false }, // 97% of maximumTimeMs
    {   true,          inf,       600000,    10000,   10,  100, pv1,          false },
    {  false,          inf,       600000,   581000,   11,  100, pv1,          false },
    {  false,          inf,       600000,   582000,   12,  100, pv2,           true }, // 97% of maximumTimeMs
    {   true,       100000,       600000,    10000,   10,  100, pv1,          false },
    {  false,       100000,       600000,   581000,   11,  100, pv2,          false },
    {  false,       100000,       600000,   582000,   12,  100, pv2,           true }, // 97% of maximumTimeMs
  };

  TimeManager timeManager;

  timeManager.clearGame();

  for (size_t i = 0; i < sizeof(scenario)/sizeof(scenario[0]); i++) {
    auto s = scenario[i];
    if (s.clear) {
      timeManager.clearPosition(s.optimumTimeMs, s.maximumTimeMs);
    }
    timeManager.update(s.elapsedMs,
                       s.depth * Searcher::Depth1Ply,
                       s.score,
                       s.pv);
    ASSERT_EQ(s.shouldInterrupt, timeManager.shouldInterrupt());
  }
}
