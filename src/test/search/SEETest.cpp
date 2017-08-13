/* SEETest.cpp
 *
 * Kubo Ryosuke
 */

#include "test/Test.hpp"
#include "core/util/PositionUtil.hpp"
#include "search/see/SEE.hpp"
#include "search/eval/Material.hpp"

using namespace sunfish;

TEST(SEETest, testExtractAggressors) {
  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  * +RY *  *  * \n"
      "P4 *  *  *  *  *  * -KE *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  * -KI-KY *  *  * \n"
      "P7 *  *  *  *  *  * +GI *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  * +KA\n"
      "P+\n"
      "P-\n"
      "+\n");
    Square from = Square::s37();
    Square to = Square::s46();

    auto bb = SEE::extractAggressors(pos, from, to);

    ASSERT_EQ(4, bb.count());
    ASSERT_TRUE(bb.check(Square::s19()));
    ASSERT_TRUE(bb.check(Square::s43()));
    ASSERT_TRUE(bb.check(Square::s34()));
    ASSERT_TRUE(bb.check(Square::s56()));
  }

  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  * -KY * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3-KA *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  * +FU *  * +HI *  *  * \n"
      "P6 * -GI+FU *  *  *  *  *  * \n"
      "P7 *  *  * +KE *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+\n"
      "P-\n"
      "-\n");
    Square from = Square::s86();
    Square to = Square::s75();

    auto bb = SEE::extractAggressors(pos, from, to);

    ASSERT_EQ(5, bb.count());
    ASSERT_TRUE(bb.check(Square::s45()));
    ASSERT_TRUE(bb.check(Square::s67()));
    ASSERT_TRUE(bb.check(Square::s76()));
    ASSERT_TRUE(bb.check(Square::s71()));
    ASSERT_TRUE(bb.check(Square::s93()));
  }

  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  * -KY *  *  *  *  *  * \n"
      "P5 *  *  * -GI *  *  *  *  * \n"
      "P6 *  * -KI *  *  *  *  *  * \n"
      "P7 *  * +FU+OU *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  * +KY *  *  *  *  *  * \n"
      "P+\n"
      "P-\n"
      "+\n");
    Square from = Square::s77();
    Square to = Square::s76();

    auto bb = SEE::extractAggressors(pos, from, to);

    ASSERT_EQ(4, bb.count());
    ASSERT_TRUE(bb.check(Square::s67()));
    ASSERT_TRUE(bb.check(Square::s79()));
    ASSERT_TRUE(bb.check(Square::s65()));
    ASSERT_TRUE(bb.check(Square::s74()));
  }

  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  * -KI-OU-KI *  *  * \n"
      "P2 *  *  *  * +KI *  *  *  * \n"
      "P3 *  *  *  * +FU *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  *  *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+\n"
      "P-\n"
      "-\n");
    Square from = Square::s41();
    Square to = Square::s52();

    auto bb = SEE::extractAggressors(pos, from, to);

    ASSERT_EQ(3, bb.count());
    ASSERT_TRUE(bb.check(Square::s53()));
    ASSERT_TRUE(bb.check(Square::s51()));
    ASSERT_TRUE(bb.check(Square::s61()));
  }
}

TEST(SEETest, testCalculate) {
  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  * +RY *  *  * \n"
      "P4 *  *  *  *  *  * -KE *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  * -KI-KY *  *  * \n"
      "P7 *  *  *  *  *  * +GI *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  * +KA\n"
      "P+\n"
      "P-\n"
      "+\n");
    Move move(Square::s37(), Square::s46(), false);
    ASSERT_EQ(material::lanceEx()
            - material::silverEx()
            + material::knightEx()
            - material::bishopEx()
            + material::goldEx(),
              SEE::calculate(pos, move));
  }

  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  * -KY * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3-KA *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  * +FU *  * +HI *  *  * \n"
      "P6 * -GI+FU *  *  *  *  *  * \n"
      "P7 *  *  * +KE *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+\n"
      "P-\n"
      "-\n");
    Move move(Square::s86(), Square::s75(), false);
    ASSERT_EQ(material::pawnEx()
            - material::silverEx(),
              SEE::calculate(pos, move));
  }

  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  * -KY *  *  * \n"
      "P3 *  *  *  *  * -KY *  *  * \n"
      "P4 *  *  *  *  * -FU *  *  * \n"
      "P5 *  *  *  *  * +FU *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  *  * +KY *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU+KY *  *  * \n"
      "P+\n"
      "P-\n"
      "+\n");
    Move move(Square::s45(), Square::s44(), false);
    ASSERT_EQ(material::pawnEx(),
              SEE::calculate(pos, move));
  }

  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  * -KY *  *  * \n"
      "P3 *  *  *  *  * -KY *  *  * \n"
      "P4 *  *  *  *  * -FU *  *  * \n"
      "P5 *  *  *  *  * +FU *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  *  * +KY *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+\n"
      "P-\n"
      "+\n");
    Move move(Square::s45(), Square::s44(), false);
    ASSERT_EQ(Score::zero(),
              SEE::calculate(pos, move));
  }

  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  * -TO * +NG+HI-HI * \n"
      "P7 *  *  *  *  *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+\n"
      "P-\n"
      "+\n");
    Move move(Square::s46(), Square::s56(), false);
    ASSERT_EQ(-material::proSilverEx(),
              SEE::calculate(pos, move));
  }

  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  * -TO * +NG+HI *  * \n"
      "P7 *  *  *  *  *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+\n"
      "P-\n"
      "+\n");
    Move move(Square::s46(), Square::s56(), false);
    ASSERT_EQ(-material::proSilverEx()
             + material::tokinEx(),
              SEE::calculate(pos, move));
  }

  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  *  *  * -FU *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU * +HI+KI * \n"
      "P+\n"
      "P-00HI\n"
      "-\n");
    Move move(PieceType::rook(), Square::s38());
    ASSERT_EQ(-material::rookEx()
             + material::goldEx()
             - material::pawnEx(),
              SEE::calculate(pos, move));
  }

  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  * +OU * -GI * \n"
      "P7 *  *  *  *  *  * -TO *  * \n"
      "P8 *  *  *  *  *  * +KI *  * \n"
      "P9 *  *  *  *  *  *  *  *  * \n"
      "P+\n"
      "P-\n"
      "+\n");
    Move move(Square::s38(), Square::s37(), false);
    ASSERT_EQ(material::tokinEx()
            - material::goldEx()
            + material::silverEx(),
              SEE::calculate(pos, move));
  }

  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  * +UM *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  * -KA\n"
      "P6 *  *  *  *  * +OU * -GI * \n"
      "P7 *  *  *  *  *  * -TO *  * \n"
      "P8 *  *  *  *  *  * +KI *  * \n"
      "P9 *  *  *  *  *  *  *  *  * \n"
      "P+\n"
      "P-\n"
      "+\n");
    Move move(Square::s38(), Square::s37(), false);
    ASSERT_EQ(material::tokinEx()
            - material::goldEx() - (material::proSilver() - material::silver()),
              SEE::calculate(pos, move));
  }

  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  *  *  *  *  * +KA\n"
      "P2 *  *  *  *  *  *  * +KA * \n"
      "P3 *  *  *  * -KI-KI *  *  * \n"
      "P4 *  *  *  *  * +FU-OU *  * \n"
      "P5 *  *  *  * +GI *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  *  *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+\n"
      "P-\n"
      "-\n");
    Move move(Square::s43(), Square::s44(), false);
    ASSERT_EQ(material::pawnEx()
            - material::goldEx(),
              SEE::calculate(pos, move));
  }

  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  *  *  *  *  * +KA\n"
      "P2 *  *  *  *  *  *  * +KA * \n"
      "P3 *  *  *  * -KI-KI *  *  * \n"
      "P4 *  * -HI *  * +FU-OU *  * \n"
      "P5 *  *  *  * +GI *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  *  *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+\n"
      "P-\n"
      "-\n");
    Move move(Square::s43(), Square::s44(), false);
    ASSERT_EQ(material::pawnEx()
            - material::goldEx()
            + material::silverEx()
            - material::goldEx()
            + material::bishopEx()
            - material::rookEx()
            + material::bishopEx(),
              SEE::calculate(pos, move));
  }
}
