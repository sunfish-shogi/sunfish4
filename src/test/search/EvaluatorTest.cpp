/* EvaluatorTest.cpp
 *
 * Kubo Ryosuke
 */

#include "test/Test.hpp"
#include "search/eval/Evaluator.hpp"
#include "search/eval/Material.hpp"
#include "search/eval/FeatureTemplates.hpp"
#include "core/position/Position.hpp"
#include "core/util/PositionUtil.hpp"
#include "common/math/Random.hpp"
#include <memory>

using namespace sunfish;

namespace {

Evaluator g_eval(Evaluator::InitType::Zero);

} // namespace

TEST_BEFORE(EvaluatorTest) {
  Random r;
  auto fv = std::unique_ptr<Evaluator::FVType>(new Evaluator::FVType);
  each(*fv, [&r](int16_t& v) {
    v = r.int16() % 21 - 10;
  });
  symmetrize(*fv, [](int16_t& e1, int16_t& e2) {
    e1 = e2;
  });
  optimize(*fv, g_eval.ofv());
  g_eval.onChanged(Evaluator::DataSourceType::Custom);
}

TEST(EvaluatorTest, testSymmetry) {
  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1-KY-KE-GI-KI-OU-KI-GI-KE-KY\n"
      "P2 * -HI *  *  *  *  * -KA * \n"
      "P3-FU-FU-FU-FU-FU-FU-FU-FU-FU\n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7+FU+FU+FU+FU+FU+FU+FU+FU+FU\n"
      "P8 * +KA *  *  *  *  * +HI * \n"
      "P9+KY+KE+GI+KI+OU+KI+GI+KE+KY\n"
      "P+\n"
      "P-\n"
      "+\n");

    auto materialScore = g_eval.calculateMaterialScore(pos);
    auto score = g_eval.calculateTotalScore(materialScore,
                                            pos);
    ASSERT_EQ(Score::zero(), materialScore);
    ASSERT_EQ(Score::zero(), score);
  }

  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  *  *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+00HI00KA00KI00GI00KE00KY00FU00FU\n"
      "P-00HI00KA00KI00GI00KE00KY00FU00FU\n"
      "+\n");

    auto materialScore = g_eval.calculateMaterialScore(pos);
    auto score = g_eval.calculateTotalScore(materialScore,
                                            pos);
    ASSERT_EQ(Score::zero(), materialScore);
    ASSERT_EQ(Score::zero(), score);
  }

  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1-KY *  *  *  *  *  * +KI-KY\n"
      "P2 * -HI *  *  *  *  *  *  * \n"
      "P3 *  * -KE *  * -KI-KI-FU-OU\n"
      "P4-KE * -FU * -GI-FU-FU * -FU\n"
      "P5 *  *  *  * -FU *  * +FU+FU\n"
      "P6-FU+GI+FU+FU *  * +FU *  * \n"
      "P7 * +FU * +GI+FU * +KA *  * \n"
      "P8+FU+OU+KI *  *  *  *  *  * \n"
      "P9+KY+KE * -HI *  *  * +KE+KY\n"
      "P+00KA00FU\n"
      "P-00GI00FU00FU\n"
      "+\n");
    Position pos2 = PositionUtil::createPositionFromCsaString(
      "P1-KY+KI *  *  *  *  *  * -KY\n"
      "P2 *  *  *  *  *  *  * -HI * \n"
      "P3-OU-FU-KI-KI *  * -KE *  * \n"
      "P4-FU * -FU-FU-GI * -FU * -KE\n"
      "P5+FU+FU *  * -FU *  *  *  * \n"
      "P6 *  * +FU *  * +FU+FU+GI-FU\n"
      "P7 *  * +KA * +FU+GI * +FU * \n"
      "P8 *  *  *  *  *  * +KI+OU+FU\n"
      "P9+KY+KE *  *  * -HI * +KE+KY\n"
      "P+00KA00FU\n"
      "P-00GI00FU00FU\n"
      "+\n");

    auto materialScore = g_eval.calculateMaterialScore(pos);
    auto score = g_eval.calculateTotalScore(materialScore,
                                            pos);
    auto materialScore2 = g_eval.calculateMaterialScore(pos2);
    auto score2 = g_eval.calculateTotalScore(materialScore2,
                                             pos2);
    ASSERT_EQ(materialScore, materialScore2);
    ASSERT_EQ(score, score2);
  }
}

TEST(EvaluatorTest, testMaterial) {
  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1-KY-KE-GI-KI-OU-KI-GI-KE-KY\n"
      "P2 * -HI *  *  *  *  * +UM * \n"
      "P3-FU-FU-FU-FU-FU-FU * -FU-FU\n"
      "P4 *  *  *  *  *  * -FU *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  * +FU *  *  *  *  *  * \n"
      "P7+FU+FU * +FU+FU+FU+FU+FU+FU\n"
      "P8 *  *  *  *  *  *  * +HI * \n"
      "P9+KY+KE+GI+KI+OU+KI+GI+KE+KY\n"
      "P+00KA\n"
      "P-\n"
      "-\n");

    auto expect = material::Bishop + material::Horse;
    auto materialScore = g_eval.calculateMaterialScore(pos);
    ASSERT_EQ(expect, materialScore);
  }
}

TEST(EvaluatorTest, testDiff) {
  {
    // black
    Position posBefore = PositionUtil::createPositionFromCsaString(
      "P1-KY-KE-GI-KI-OU-KI-GI-KE-KY\n"
      "P2 * -HI *  *  *  *  * -KA * \n"
      "P3-FU-FU-FU-FU-FU-FU * -FU-FU\n"
      "P4 *  *  *  *  *  * -FU *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  * +FU *  *  *  *  *  * \n"
      "P7+FU+FU * +FU+FU+FU+FU+FU+FU\n"
      "P8 * +KA *  *  *  *  * +HI * \n"
      "P9+KY+KE+GI+KI+OU+KI+GI+KE+KY\n"
      "P+\n"
      "P-\n"
      "+\n");
    Position posAfter = PositionUtil::createPositionFromCsaString(
      "P1-KY-KE-GI-KI-OU-KI-GI-KE-KY\n"
      "P2 * -HI *  *  *  *  * -KA * \n"
      "P3-FU-FU-FU-FU-FU-FU * -FU-FU\n"
      "P4 *  *  *  *  *  * -FU *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  * +FU *  *  *  * +FU * \n"
      "P7+FU+FU * +FU+FU+FU+FU * +FU\n"
      "P8 * +KA *  *  *  *  * +HI * \n"
      "P9+KY+KE+GI+KI+OU+KI+GI+KE+KY\n"
      "P+\n"
      "P-\n"
      "-\n");

    auto move = Move(Square::s27(), Square::s26(), false);
    auto captured = Piece::empty();
    auto scoreBefore = g_eval.calculateMaterialScore(posBefore);
    auto scoreAfter = g_eval.calculateMaterialScoreDiff(scoreBefore,
                                                        posAfter,
                                                        move,
                                                        captured);
    auto expect = g_eval.calculateMaterialScore(posAfter);
    ASSERT_EQ(expect, scoreAfter);
  }

  {
    // black - promotion, capture
    Position posBefore = PositionUtil::createPositionFromCsaString(
      "P1-KY-KE-GI-KI-OU-KI-GI-KE-KY\n"
      "P2 * -HI *  *  *  *  * -KA * \n"
      "P3-FU-FU-FU-FU-FU-FU * -FU-FU\n"
      "P4 *  *  *  *  *  * -FU *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  * +FU *  *  *  *  *  * \n"
      "P7+FU+FU * +FU+FU+FU+FU+FU+FU\n"
      "P8 * +KA *  *  *  *  * +HI * \n"
      "P9+KY+KE+GI+KI+OU+KI+GI+KE+KY\n"
      "P+\n"
      "P-\n"
      "+\n");
    Position posAfter = PositionUtil::createPositionFromCsaString(
      "P1-KY-KE-GI-KI-OU-KI-GI-KE-KY\n"
      "P2 * -HI *  *  *  *  * +UM * \n"
      "P3-FU-FU-FU-FU-FU-FU * -FU-FU\n"
      "P4 *  *  *  *  *  * -FU *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  * +FU *  *  *  *  *  * \n"
      "P7+FU+FU * +FU+FU+FU+FU+FU+FU\n"
      "P8 *  *  *  *  *  *  * +HI * \n"
      "P9+KY+KE+GI+KI+OU+KI+GI+KE+KY\n"
      "P+00KA\n"
      "P-\n"
      "-\n");

    auto move = Move(Square::s88(), Square::s22(), true);
    auto captured = Piece::whiteBishop();
    auto scoreBefore = g_eval.calculateMaterialScore(posBefore);
    auto scoreAfter = g_eval.calculateMaterialScoreDiff(scoreBefore,
                                                        posAfter,
                                                        move,
                                                        captured);
    auto expect = g_eval.calculateMaterialScore(posAfter);
    ASSERT_EQ(expect, scoreAfter);
  }

  {
    // black - drop
    Position posBefore = PositionUtil::createPositionFromCsaString(
      "P1-KY *  * -HI *  *  * -KE-KY\n"
      "P2 *  * -KI * -OU * -KI-GI * \n"
      "P3 *  *  *  * -FU-FU-KA-FU-FU\n"
      "P4 *  * -GI *  *  *  *  *  * \n"
      "P5-FU *  * -KE+FU * -FU+FU * \n"
      "P6 * +FU *  *  * +GI *  *  * \n"
      "P7+FU+GI+KE+KI * +FU *  * +FU\n"
      "P8 *  * +KI *  *  *  * +HI * \n"
      "P9+KY * +OU * +KA *  * +KE+KY\n"
      "P+00FU00FU\n"
      "P-00FU00FU00FU00FU\n"
      "+\n");
    Position posAfter = PositionUtil::createPositionFromCsaString(
      "P1-KY *  * -HI *  *  * -KE-KY\n"
      "P2 *  * -KI * -OU * -KI-GI * \n"
      "P3 *  *  *  * -FU-FU-KA-FU-FU\n"
      "P4 *  * -GI *  *  *  *  *  * \n"
      "P5-FU *  * -KE+FU * -FU+FU * \n"
      "P6 * +FU+FU *  * +GI *  *  * \n"
      "P7+FU+GI+KE+KI * +FU *  * +FU\n"
      "P8 *  * +KI *  *  *  * +HI * \n"
      "P9+KY * +OU * +KA *  * +KE+KY\n"
      "P+00FU\n"
      "P-00FU00FU00FU00FU\n"
      "-\n");

    auto move = Move(PieceType::pawn(), Square::s76());
    auto captured = Piece::empty();
    auto scoreBefore = g_eval.calculateMaterialScore(posBefore);
    auto scoreAfter = g_eval.calculateMaterialScoreDiff(scoreBefore,
                                                        posAfter,
                                                        move,
                                                        captured);
    auto expect = g_eval.calculateMaterialScore(posAfter);
    ASSERT_EQ(expect, scoreAfter);
  }

  {
    // black - king
    Position posBefore = PositionUtil::createPositionFromCsaString(
      "P1-KY-KE-GI-KI-OU-KI-GI-KE-KY\n"
      "P2 * -HI *  *  *  *  * -KA * \n"
      "P3-FU-FU-FU-FU-FU-FU * -FU-FU\n"
      "P4 *  *  *  *  *  * -FU *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  * +FU *  *  *  *  *  * \n"
      "P7+FU+FU * +FU+FU+FU+FU+FU+FU\n"
      "P8 * +KA *  *  *  *  * +HI * \n"
      "P9+KY+KE+GI+KI+OU+KI+GI+KE+KY\n"
      "P+\n"
      "P-\n"
      "+\n");
    Position posAfter = PositionUtil::createPositionFromCsaString(
      "P1-KY-KE-GI-KI-OU-KI-GI-KE-KY\n"
      "P2 * -HI *  *  *  *  * -KA * \n"
      "P3-FU-FU-FU-FU-FU-FU * -FU-FU\n"
      "P4 *  *  *  *  *  * -FU *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  * +FU *  *  *  *  *  * \n"
      "P7+FU+FU * +FU+FU+FU+FU+FU+FU\n"
      "P8 * +KA * +OU *  *  * +HI * \n"
      "P9+KY+KE+GI+KI * +KI+GI+KE+KY\n"
      "P+\n"
      "P-\n"
      "-\n");

    auto move = Move(Square::s59(), Square::s68(), false);
    auto captured = Piece::empty();
    auto scoreBefore = g_eval.calculateMaterialScore(posBefore);
    auto scoreAfter = g_eval.calculateMaterialScoreDiff(scoreBefore,
                                                        posAfter,
                                                        move,
                                                        captured);
    auto expect = g_eval.calculateMaterialScore(posAfter);
    ASSERT_EQ(expect, scoreAfter);
  }

  {
    // black - king, capture
    Position posBefore = PositionUtil::createPositionFromCsaString(
      "P1-KY-KE-GI-KI * -KI * -KE-KY\n"
      "P2 *  * -OU *  * -HI * -GI * \n"
      "P3-FU-FU-FU-FU-FU-FU * -FU-FU\n"
      "P4 *  *  *  *  *  * -FU *  * \n"
      "P5 *  *  *  *  *  *  * +FU * \n"
      "P6 *  * +FU *  *  *  *  *  * \n"
      "P7+FU+FU-UM+FU+FU+FU+FU * +FU\n"
      "P8 *  * +KI+OU * +GI * +HI * \n"
      "P9+KY+KE+GI *  * +KI * +KE+KY\n"
      "P+\n"
      "P-00KA\n"
      "+\n");
    Position posAfter = PositionUtil::createPositionFromCsaString(
      "P1-KY-KE-GI-KI * -KI * -KE-KY\n"
      "P2 *  * -OU *  * -HI * -GI * \n"
      "P3-FU-FU-FU-FU-FU-FU * -FU-FU\n"
      "P4 *  *  *  *  *  * -FU *  * \n"
      "P5 *  *  *  *  *  *  * +FU * \n"
      "P6 *  * +FU *  *  *  *  *  * \n"
      "P7+FU+FU+OU+FU+FU+FU+FU * +FU\n"
      "P8 *  * +KI *  * +GI * +HI * \n"
      "P9+KY+KE+GI *  * +KI * +KE+KY\n"
      "P+00KA\n"
      "P-00KA\n"
      "-\n");

    auto move = Move(Square::s68(), Square::s77(), false);
    auto captured = Piece::whiteHorse();
    auto scoreBefore = g_eval.calculateMaterialScore(posBefore);
    auto scoreAfter = g_eval.calculateMaterialScoreDiff(scoreBefore,
                                                        posAfter,
                                                        move,
                                                        captured);
    auto expect = g_eval.calculateMaterialScore(posAfter);
    ASSERT_EQ(expect, scoreAfter);
  }

  {
    // white
    Position posBefore = PositionUtil::createPositionFromCsaString(
      "P1-KY-KE-GI-KI-OU-KI-GI-KE-KY\n"
      "P2 * -HI *  *  *  *  * -KA * \n"
      "P3-FU-FU-FU-FU-FU-FU * -FU-FU\n"
      "P4 *  *  *  *  *  * -FU *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  * +FU *  *  *  * +FU * \n"
      "P7+FU+FU * +FU+FU+FU+FU * +FU\n"
      "P8 * +KA *  *  *  *  * +HI * \n"
      "P9+KY+KE+GI+KI+OU+KI+GI+KE+KY\n"
      "P+\n"
      "P-\n"
      "-\n");
    Position posAfter = PositionUtil::createPositionFromCsaString(
      "P1-KY-KE-GI * -OU-KI-GI-KE-KY\n"
      "P2 * -HI *  * -KI *  * -KA * \n"
      "P3-FU-FU-FU-FU-FU-FU * -FU-FU\n"
      "P4 *  *  *  *  *  * -FU *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  * +FU *  *  *  * +FU * \n"
      "P7+FU+FU * +FU+FU+FU+FU * +FU\n"
      "P8 * +KA *  *  *  *  * +HI * \n"
      "P9+KY+KE+GI+KI+OU+KI+GI+KE+KY\n"
      "P+\n"
      "P-\n"
      "+\n");

    auto move = Move(Square::s61(), Square::s52(), false);
    auto captured = Piece::empty();
    auto scoreBefore = g_eval.calculateMaterialScore(posBefore);
    auto scoreAfter = g_eval.calculateMaterialScoreDiff(scoreBefore,
                                                        posAfter,
                                                        move,
                                                        captured);
    auto expect = g_eval.calculateMaterialScore(posAfter);
    ASSERT_EQ(expect, scoreAfter);
  }

  {
    // white - capture
    Position posBefore = PositionUtil::createPositionFromCsaString(
      "P1-KY-KE-GI-KI-OU * -GI-KE-KY\n"
      "P2 * -HI *  *  *  * -KI-KA * \n"
      "P3-FU * -FU-FU-FU-FU *  * -FU\n"
      "P4 *  *  *  *  *  * -FU+HI * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 * +FU+FU *  *  *  *  *  * \n"
      "P7+FU *  * +FU+FU+FU+FU * +FU\n"
      "P8 * +KA+KI *  *  *  *  *  * \n"
      "P9+KY+KE+GI * +OU+KI+GI+KE+KY\n"
      "P+00FU00FU\n"
      "P-00FU\n"
      "-\n");
    Position posAfter = PositionUtil::createPositionFromCsaString(
      "P1-KY-KE-GI-KI-OU * -GI-KE-KY\n"
      "P2 *  *  *  *  *  * -KI-KA * \n"
      "P3-FU * -FU-FU-FU-FU *  * -FU\n"
      "P4 *  *  *  *  *  * -FU+HI * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 * -HI+FU *  *  *  *  *  * \n"
      "P7+FU *  * +FU+FU+FU+FU * +FU\n"
      "P8 * +KA+KI *  *  *  *  *  * \n"
      "P9+KY+KE+GI * +OU+KI+GI+KE+KY\n"
      "P+00FU00FU\n"
      "P-00FU00FU\n"
      "+\n");

    auto move = Move(Square::s82(), Square::s86(), false);
    auto captured = Piece::blackPawn();
    auto scoreBefore = g_eval.calculateMaterialScore(posBefore);
    auto scoreAfter = g_eval.calculateMaterialScoreDiff(scoreBefore,
                                                        posAfter,
                                                        move,
                                                        captured);
    auto expect = g_eval.calculateMaterialScore(posAfter);
    ASSERT_EQ(expect, scoreAfter);
  }

  {
    // white - promotion 
    Position posBefore = PositionUtil::createPositionFromCsaString(
      "P1 * -KE * -KI-OU *  * -KE-KY\n"
      "P2 * -HI *  *  *  * -KI *  * \n"
      "P3 *  * -FU-FU-FU-FU-GI-FU-FU\n"
      "P4 *  *  *  *  *  * -FU *  * \n"
      "P5-KY-FU *  *  *  *  * +FU * \n"
      "P6 *  * +FU *  * +FU *  *  * \n"
      "P7+FU+FU * +FU+FU+GI+FU * +FU\n"
      "P8-FU+GI+KI * +KI *  * +HI * \n"
      "P9 * +KE *  * +OU *  * +KE+KY\n"
      "P+00KA00GI\n"
      "P-00KA00KY\n"
      "-\n");
    Position posAfter = PositionUtil::createPositionFromCsaString(
      "P1 * -KE * -KI-OU *  * -KE-KY\n"
      "P2 * -HI *  *  *  * -KI *  * \n"
      "P3 *  * -FU-FU-FU-FU-GI-FU-FU\n"
      "P4 *  *  *  *  *  * -FU *  * \n"
      "P5-KY-FU *  *  *  *  * +FU * \n"
      "P6 *  * +FU *  * +FU *  *  * \n"
      "P7+FU+FU * +FU+FU+GI+FU * +FU\n"
      "P8 * +GI+KI * +KI *  * +HI * \n"
      "P9-TO+KE *  * +OU *  * +KE+KY\n"
      "P+00KA00GI\n"
      "P-00KA00KY\n"
      "+\n");

    auto move = Move(Square::s98(), Square::s99(), true);
    auto captured = Piece::empty();
    auto scoreBefore = g_eval.calculateMaterialScore(posBefore);
    auto scoreAfter = g_eval.calculateMaterialScoreDiff(scoreBefore,
                                                        posAfter,
                                                        move,
                                                        captured);
    auto expect = g_eval.calculateMaterialScore(posAfter);
    ASSERT_EQ(expect, scoreAfter);
  }

  {
    // white - drop
    Position posBefore = PositionUtil::createPositionFromCsaString(
      "P1-KY-KE * -FU * -KI *  * -KY\n"
      "P2 *  *  * -GI * -OU-GI+TO * \n"
      "P3-FU *  *  * -FU *  *  * -FU\n"
      "P4 *  * -FU *  *  * -KI-FU * \n"
      "P5 *  * +FU * +KA * -FU *  * \n"
      "P6 * +FU-KI-TO+FU *  *  *  * \n"
      "P7+FU-FU *  *  * +FU *  * +FU\n"
      "P8+OU * +GI *  *  *  * +HI * \n"
      "P9+KY+KE+GI *  *  *  * +KE+KY\n"
      "P+00KA00KE00FU\n"
      "P-00HI00KI00FU\n"
      "-\n");
    Position posAfter = PositionUtil::createPositionFromCsaString(
      "P1-KY-KE * -FU * -KI *  * -KY\n"
      "P2 *  *  * -GI * -OU-GI+TO * \n"
      "P3-FU *  *  * -FU *  *  * -FU\n"
      "P4 *  * -FU *  *  * -KI-FU * \n"
      "P5 *  * +FU * +KA * -FU *  * \n"
      "P6 * +FU-KI-TO+FU *  *  *  * \n"
      "P7+FU-FU *  *  * +FU *  * +FU\n"
      "P8+OU-KI+GI *  *  *  * +HI * \n"
      "P9+KY+KE+GI *  *  *  * +KE+KY\n"
      "P+00KA00KE00FU\n"
      "P-00HI00FU\n"
      "+\n");

    auto move = Move(PieceType::gold(), Square::s88());
    auto captured = Piece::empty();
    auto scoreBefore = g_eval.calculateMaterialScore(posBefore);
    auto scoreAfter = g_eval.calculateMaterialScoreDiff(scoreBefore,
                                                        posAfter,
                                                        move,
                                                        captured);
    auto expect = g_eval.calculateMaterialScore(posAfter);
    ASSERT_EQ(expect, scoreAfter);
  }

  {
    // white - king
    Position posBefore = PositionUtil::createPositionFromCsaString(
      "P1-KY-KE-GI-KI-OU-KI-GI-KE-KY\n"
      "P2 * -HI *  *  *  *  * -KA * \n"
      "P3-FU-FU-FU-FU-FU-FU * -FU-FU\n"
      "P4 *  *  *  *  *  * -FU *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  * +FU *  *  *  *  *  * \n"
      "P7+FU+FU * +FU+FU+FU+FU+FU+FU\n"
      "P8 * +KA * +OU *  *  * +HI * \n"
      "P9+KY+KE+GI+KI * +KI+GI+KE+KY\n"
      "P+\n"
      "P-\n"
      "-\n");
    Position posAfter = PositionUtil::createPositionFromCsaString(
      "P1-KY-KE-GI-KI * -KI-GI-KE-KY\n"
      "P2 * -HI *  * -OU *  * -KA * \n"
      "P3-FU-FU-FU-FU-FU-FU * -FU-FU\n"
      "P4 *  *  *  *  *  * -FU *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  * +FU *  *  *  *  *  * \n"
      "P7+FU+FU * +FU+FU+FU+FU+FU+FU\n"
      "P8 * +KA * +OU *  *  * +HI * \n"
      "P9+KY+KE+GI+KI * +KI+GI+KE+KY\n"
      "P+\n"
      "P-\n"
      "+\n");

    auto move = Move(Square::s51(), Square::s52(), false);
    auto captured = Piece::empty();
    auto scoreBefore = g_eval.calculateMaterialScore(posBefore);
    auto scoreAfter = g_eval.calculateMaterialScoreDiff(scoreBefore,
                                                        posAfter,
                                                        move,
                                                        captured);
    auto expect = g_eval.calculateMaterialScore(posAfter);
    ASSERT_EQ(expect, scoreAfter);
  }

  {
    // white - king, capture
    Position posBefore = PositionUtil::createPositionFromCsaString(
      "P1-KY-KE * -KI *  *  * -KE-KY\n"
      "P2 * -HI * -GI * -KI-OU-GI * \n"
      "P3-FU * -FU-FU * -FU+FU-FU-FU\n"
      "P4 *  *  *  * -FU-KA-FU *  * \n"
      "P5 * -FU *  *  *  *  *  *  * \n"
      "P6 *  * +FU * +FU+GI *  *  * \n"
      "P7+FU+FU+GI+FU * +FU+KE * +FU\n"
      "P8 *  * +OU *  *  * +HI *  * \n"
      "P9+KY+KE+KA+KI * +KI *  * +KY\n"
      "P+00FU\n"
      "P-\n"
      "-\n");
    Position posAfter = PositionUtil::createPositionFromCsaString(
      "P1-KY-KE * -KI *  *  * -KE-KY\n"
      "P2 * -HI * -GI * -KI * -GI * \n"
      "P3-FU * -FU-FU * -FU-OU-FU-FU\n"
      "P4 *  *  *  * -FU-KA-FU *  * \n"
      "P5 * -FU *  *  *  *  *  *  * \n"
      "P6 *  * +FU * +FU+GI *  *  * \n"
      "P7+FU+FU+GI+FU * +FU+KE * +FU\n"
      "P8 *  * +OU *  *  * +HI *  * \n"
      "P9+KY+KE+KA+KI * +KI *  * +KY\n"
      "P+00FU\n"
      "P-00FU\n"
      "+\n");

    auto move = Move(Square::s32(), Square::s33(), false);
    auto captured = Piece::blackPawn();
    auto scoreBefore = g_eval.calculateMaterialScore(posBefore);
    auto scoreAfter = g_eval.calculateMaterialScoreDiff(scoreBefore,
                                                        posAfter,
                                                        move,
                                                        captured);
    auto expect = g_eval.calculateMaterialScore(posAfter);
    ASSERT_EQ(expect, scoreAfter);
  }
}

TEST(EvaluatorTest, testSymmetrize) {
  auto fv = std::unique_ptr<Evaluator::FVType>(new Evaluator::FVType);
  each(*fv, [](int16_t& v) {
    v = 1;
  });
  symmetrize(*fv, [](int16_t& e1, int16_t& e2) {
    e1 = e2 = e1 + e2;
  });
  each(*fv, [](int16_t& v) {
    ASSERT_TRUE(v == 1 || v == 2);
  });
}
