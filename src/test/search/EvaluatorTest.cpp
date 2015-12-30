/* EvaluatorTest.cpp
 *
 * Kubo Ryosuke
 */

#if !defined(NDEBUG)

#include "test/Test.hpp"
#include "search/eval/Evaluator.hpp"
#include "search/eval/Material.hpp"
#include "core/position/Position.hpp"
#include "core/util/PositionUtil.hpp"
#include "common/math/Random.hpp"

using namespace sunfish;

namespace {

Evaluator g_eval;

} // namespace

TEST_BEFORE(EvaluatorTest) {
  Random r;
  g_eval.initializeRandom([&r]() {
    return r.int16();
  });
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

    auto scores = g_eval.evaluate(pos);
    ASSERT_EQ(Score::zero(), scores.materialScore);
    ASSERT_EQ(0            , scores.positionalScore);
    ASSERT_EQ(Score::zero(), calculateScore(scores));
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

    auto scores = g_eval.evaluate(pos);
    ASSERT_EQ(Score::zero(), scores.materialScore);
    ASSERT_EQ(0            , scores.positionalScore);
    ASSERT_EQ(Score::zero(), calculateScore(scores));
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
    auto scores = g_eval.evaluate(pos);
    ASSERT_EQ(expect, scores.materialScore);
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
    auto scores = g_eval.evaluate(posBefore);
    scores = g_eval.evaluateDiff(scores,
                                 posAfter,
                                 move,
                                 captured);
    auto expect = g_eval.evaluate(posAfter);
    ASSERT_EQ(expect.materialScore, scores.materialScore);
    ASSERT_EQ(expect.positionalScore, scores.positionalScore);
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
    auto scores = g_eval.evaluate(posBefore);
    scores = g_eval.evaluateDiff(scores,
                                 posAfter,
                                 move,
                                 captured);
    auto expect = g_eval.evaluate(posAfter);
    ASSERT_EQ(expect.materialScore, scores.materialScore);
    ASSERT_EQ(expect.positionalScore, scores.positionalScore);
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
    auto scores = g_eval.evaluate(posBefore);
    scores = g_eval.evaluateDiff(scores,
                                 posAfter,
                                 move,
                                 captured);
    auto expect = g_eval.evaluate(posAfter);
    ASSERT_EQ(expect.materialScore, scores.materialScore);
    ASSERT_EQ(expect.positionalScore, scores.positionalScore);
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
    auto scores = g_eval.evaluate(posBefore);
    scores = g_eval.evaluateDiff(scores,
                                 posAfter,
                                 move,
                                 captured);
    auto expect = g_eval.evaluate(posAfter);
    ASSERT_EQ(expect.materialScore, scores.materialScore);
    ASSERT_EQ(expect.positionalScore, scores.positionalScore);
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
    auto scores = g_eval.evaluate(posBefore);
    scores = g_eval.evaluateDiff(scores,
                                 posAfter,
                                 move,
                                 captured);
    auto expect = g_eval.evaluate(posAfter);
    ASSERT_EQ(expect.materialScore, scores.materialScore);
    ASSERT_EQ(expect.positionalScore, scores.positionalScore);
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
    auto scores = g_eval.evaluate(posBefore);
    scores = g_eval.evaluateDiff(scores,
                                 posAfter,
                                 move,
                                 captured);
    auto expect = g_eval.evaluate(posAfter);
    ASSERT_EQ(expect.materialScore, scores.materialScore);
    ASSERT_EQ(expect.positionalScore, scores.positionalScore);
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
    auto scores = g_eval.evaluate(posBefore);
    scores = g_eval.evaluateDiff(scores,
                                 posAfter,
                                 move,
                                 captured);
    auto expect = g_eval.evaluate(posAfter);
    ASSERT_EQ(expect.materialScore, scores.materialScore);
    ASSERT_EQ(expect.positionalScore, scores.positionalScore);
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
    auto scores = g_eval.evaluate(posBefore);
    scores = g_eval.evaluateDiff(scores,
                                 posAfter,
                                 move,
                                 captured);
    auto expect = g_eval.evaluate(posAfter);
    ASSERT_EQ(expect.materialScore, scores.materialScore);
    ASSERT_EQ(expect.positionalScore, scores.positionalScore);
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
    auto scores = g_eval.evaluate(posBefore);
    scores = g_eval.evaluateDiff(scores,
                                 posAfter,
                                 move,
                                 captured);
    auto expect = g_eval.evaluate(posAfter);
    ASSERT_EQ(expect.materialScore, scores.materialScore);
    ASSERT_EQ(expect.positionalScore, scores.positionalScore);
  }
}

TEST(EvaluatorTest, testSymmetric) {
  ASSERT_EQ(KKP::HPawn   +  0, symmetricalKkpIndex(KKP::HPawn   +  0));
  ASSERT_EQ(KKP::HPawn   + 17, symmetricalKkpIndex(KKP::HPawn   + 17));

  ASSERT_EQ(KKP::HLance  +  0, symmetricalKkpIndex(KKP::HLance  +  0));

  ASSERT_EQ(KKP::HRook   +  1, symmetricalKkpIndex(KKP::HRook   +  1));

  ASSERT_EQ(KKP::BPawn   + 64, symmetricalKkpIndex(KKP::BPawn   +  0));
  ASSERT_EQ(KKP::BPawn   + 50, symmetricalKkpIndex(KKP::BPawn   + 18));
  ASSERT_EQ(KKP::BPawn   + 18, symmetricalKkpIndex(KKP::BPawn   + 50));
  ASSERT_EQ(KKP::BPawn   +  7, symmetricalKkpIndex(KKP::BPawn   + 71));

  ASSERT_EQ(KKP::BLance  + 64, symmetricalKkpIndex(KKP::BLance  +  0));
  ASSERT_EQ(KKP::BLance  + 50, symmetricalKkpIndex(KKP::BLance  + 18));
  ASSERT_EQ(KKP::BLance  + 18, symmetricalKkpIndex(KKP::BLance  + 50));
  ASSERT_EQ(KKP::BLance  +  7, symmetricalKkpIndex(KKP::BLance  + 71));

  ASSERT_EQ(KKP::BKnight + 56, symmetricalKkpIndex(KKP::BKnight +  0));
  ASSERT_EQ(KKP::BKnight + 52, symmetricalKkpIndex(KKP::BKnight + 10));
  ASSERT_EQ(KKP::BKnight + 10, symmetricalKkpIndex(KKP::BKnight + 52));
  ASSERT_EQ(KKP::BKnight +  6, symmetricalKkpIndex(KKP::BKnight + 62));

  ASSERT_EQ(KKP::BSilver + 72, symmetricalKkpIndex(KKP::BSilver +  0));
  ASSERT_EQ(KKP::BSilver +  8, symmetricalKkpIndex(KKP::BSilver + 80));

  ASSERT_EQ(KKP::BRook   + 72, symmetricalKkpIndex(KKP::BRook   +  0));
  ASSERT_EQ(KKP::BRook   +  8, symmetricalKkpIndex(KKP::BRook   + 80));
}

#endif // !defined(NDEBUG)
