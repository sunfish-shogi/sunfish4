/* ScoreTest.cpp
 *
 * Kubo Ryosuke
 */

#if !defined(NDEBUG)

#include "test/Test.hpp"
#include "search/eval/Score.hpp"
#include <sstream>

using namespace sunfish;

TEST(ScoreTest, testConstants) {
  ASSERT_EQ(Score(0), Score::zero());
  ASSERT_EQ(Score(20000), Score::infinity());
  ASSERT_EQ(Score(15000), Score::mate());
  ASSERT_EQ(Score(5000), Score::materialInfinity());
}

TEST(ScoreTest, testOperators) {
  {
    Score score(128);
    score += Score(63);
    ASSERT_EQ(Score(191), score);
  }

  {
    Score score(128);
    score -= Score(63);
    ASSERT_EQ(Score(65), score);
  }

  {
    Score score(128);
    score *= Score(-2);
    ASSERT_EQ(Score(-256), score);
  }

  {
    Score score(128);
    score /= Score(2);
    ASSERT_EQ(Score(64), score);
  }

  {
    ASSERT_EQ(Score(191)  , Score(128) + Score(63));
    ASSERT_EQ(Score(65)   , Score(128) - Score(63));
    ASSERT_EQ(Score(-256) , Score(128) * Score(-2));
    ASSERT_EQ(Score(64)   , Score(128) / Score(2));
  }

  {
    ASSERT_EQ(true , Score(107) == Score(107));
    ASSERT_EQ(false, Score(107) == Score(106));

    ASSERT_EQ(false, Score(107) != Score(107));
    ASSERT_EQ(true , Score(107) != Score(106));

    ASSERT_EQ(false, Score(107) > Score(108));
    ASSERT_EQ(false, Score(107) > Score(107));
    ASSERT_EQ(true , Score(107) > Score(106));

    ASSERT_EQ(true , Score(107) < Score(108));
    ASSERT_EQ(false, Score(107) < Score(107));
    ASSERT_EQ(false, Score(107) < Score(106));

    ASSERT_EQ(false, Score(107) >= Score(108));
    ASSERT_EQ(true , Score(107) >= Score(107));
    ASSERT_EQ(true , Score(107) >= Score(106));

    ASSERT_EQ(true , Score(107) <= Score(108));
    ASSERT_EQ(true , Score(107) <= Score(107));
    ASSERT_EQ(false, Score(107) <= Score(106));
  }

  {
    std::ostringstream oss;
    oss << Score(-762);
    ASSERT_EQ("-762", oss.str());
  }
}

TEST(ScoreTest, testRawType) {
  Score score(891);
  ASSERT_EQ(891, score.raw());
}

TEST(ScoreTest, testSetToMove) {
  {
    Score score(32107);
    Move move(Square::s77(), Square::s76(), false);
    setScoreToMove(move, score);
    ASSERT_EQ(32107, moveToScore(move));
  }

  {
    Score score(-30725);
    Move move(Square::s77(), Square::s76(), false);
    setScoreToMove(move, score);
    ASSERT_EQ(-30725, moveToScore(move));
  }
}

#endif // !defined(NDEBUG)
