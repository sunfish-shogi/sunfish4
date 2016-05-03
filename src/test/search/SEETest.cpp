/* SEETest.cpp
 *
 * Kubo Ryosuke
 */

#include "test/Test.hpp"
#include "core/util/PositionUtil.hpp"
#include "search/see/SEE.hpp"
#include "search/eval/Material.hpp"

using namespace sunfish;

TEST(SEETest, testExtractAttackers) {
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

    auto bb = SEE::extractAttackers(pos, from, to);

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

    auto bb = SEE::extractAttackers(pos, from, to);

    ASSERT_EQ(5, bb.count());
    ASSERT_TRUE(bb.check(Square::s45()));
    ASSERT_TRUE(bb.check(Square::s67()));
    ASSERT_TRUE(bb.check(Square::s76()));
    ASSERT_TRUE(bb.check(Square::s71()));
    ASSERT_TRUE(bb.check(Square::s93()));
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
    ASSERT_EQ(material::LanceEx
            - material::SilverEx
            + material::KnightEx
            - material::BishopEx
            + material::GoldEx,
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
    ASSERT_EQ(material::PawnEx
            - material::SilverEx,
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
    ASSERT_EQ(material::PawnEx,
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
    ASSERT_EQ(-material::ProSilverEx,
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
    ASSERT_EQ(-material::ProSilverEx
             + material::TokinEx,
              SEE::calculate(pos, move));
  }
}

TEST(SEETest, testSortMoves) {
  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  * -GI * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 * -FU *  *  * -KY *  *  * \n"
      "P4 * -KE+HI *  *  *  *  *  * \n"
      "P5 * +FU *  *  * -FU *  *  * \n"
      "P6 *  *  *  * +KI+FU *  *  * \n"
      "P7 *  *  *  *  *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+\n"
      "P-\n"
      "+\n");
    Moves moves;
    moves.add(Move(Square::s46(), Square::s45(), false));
    moves.add(Move(Square::s56(), Square::s45(), false));
    moves.add(Move(Square::s74(), Square::s71(), true ));
    moves.add(Move(Square::s74(), Square::s84(), false));
    moves.add(Move(Square::s85(), Square::s84(), false));

    SEE::sortMoves(pos, moves, moves.begin(), false, false);

    ASSERT_EQ(5, moves.size());
    ASSERT_EQ(Move(Square::s74(), Square::s71(), true ), moves[0]);
    ASSERT_EQ(Move(Square::s85(), Square::s84(), false), moves[1]);
    ASSERT_EQ(Move(Square::s46(), Square::s45(), false), moves[2]);
    ASSERT_EQ(Move(Square::s56(), Square::s45(), false), moves[3]);
    ASSERT_EQ(Move(Square::s74(), Square::s84(), false), moves[4]);
  }

  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  * -GI * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 * -FU *  *  * -KY *  *  * \n"
      "P4 * -KE+HI *  *  *  *  *  * \n"
      "P5 * +FU *  *  * -FU *  *  * \n"
      "P6 *  *  *  * +KI+FU *  *  * \n"
      "P7 *  *  *  *  *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+\n"
      "P-\n"
      "+\n");
    Moves moves;
    moves.add(Move(Square::s46(), Square::s45(), false));
    moves.add(Move(Square::s56(), Square::s45(), false));
    moves.add(Move(Square::s74(), Square::s71(), true ));
    moves.add(Move(Square::s74(), Square::s84(), false));
    moves.add(Move(Square::s85(), Square::s84(), false));

    SEE::sortMoves(pos, moves, moves.begin(), true, false);

    ASSERT_EQ(3, moves.size());
    ASSERT_EQ(Move(Square::s74(), Square::s71(), true ), moves[0]);
    ASSERT_EQ(Move(Square::s85(), Square::s84(), false), moves[1]);
    ASSERT_EQ(Move(Square::s46(), Square::s45(), false), moves[2]);
  }

  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 * -KE *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  * -FU *  * -KY * +FU * \n"
      "P4 *  * +FU *  *  *  *  *  * \n"
      "P5 *  *  * +KE-KI-FU *  *  * \n"
      "P6 *  *  *  * +KI+FU *  *  * \n"
      "P7 *  *  *  *  *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+\n"
      "P-\n"
      "+\n");
    Moves moves;
    moves.add(Move(Square::s23(), Square::s22(), true ));
    moves.add(Move(Square::s46(), Square::s45(), false));
    moves.add(Move(Square::s56(), Square::s45(), false));
    moves.add(Move(Square::s56(), Square::s55(), false));
    moves.add(Move(Square::s65(), Square::s53(), true ));
    moves.add(Move(Square::s65(), Square::s73(), false));
    moves.add(Move(Square::s65(), Square::s73(), true ));
    moves.add(Move(Square::s74(), Square::s73(), true ));

    SEE::sortMoves(pos, moves, moves.begin(), true, true);

    ASSERT_EQ(4, moves.size());
    ASSERT_EQ(Move(Square::s56(), Square::s55(), false), moves[0]);
    ASSERT_EQ(Move(Square::s74(), Square::s73(), true ), moves[1]);
    ASSERT_EQ(Move(Square::s23(), Square::s22(), true ), moves[2]);
    ASSERT_EQ(Move(Square::s65(), Square::s73(), true ), moves[3]);
  }
}
