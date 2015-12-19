/* MoveGenerationTest.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_EXPT_MGTEST_MOVEGENERATION_HPP__
#define SUNFISH_EXPT_MGTEST_MOVEGENERATION_HPP__

#include "common/math/Random.hpp"

namespace sunfish {

class Position;

class MoveGenerationTest {
public:

  bool test(int repeatCount, int maximumNumberOfMoves);

private:

  enum class TestStatus {
    Continue,
    Mate,
    Error,
  };

  bool testOnce(int count, int length);

  TestStatus testAndDoMove(Position& position);

private:

  Random random_;

};

} // namespace sunfish

#endif // SUNFISH_EXPT_MGTEST_MOVEGENERATION_HPP__
