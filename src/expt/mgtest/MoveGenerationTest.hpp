/* MoveGenerationTest.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_EXPT_MGTEST_MOVEGENERATION_HPP__
#define SUNFISH_EXPT_MGTEST_MOVEGENERATION_HPP__

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

};

} // namespace sunfish

#endif // SUNFISH_EXPT_MGTEST_MOVEGENERATION_HPP__
