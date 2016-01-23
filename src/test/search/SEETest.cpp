/* SEETest.cpp
 *
 * Kubo Ryosuke
 */

#include "test/Test.hpp"
#include "core/util/PositionUtil.hpp"
#include "search/see/SEE.hpp"
#include "search/eval/Material.hpp"

using namespace sunfish;

TEST(ScoreTest, testGenerateAttackers) {
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

    SEE::Attackers ba;
    SEE::Attackers wa;
    std::tie(ba, wa) = SEE::generateAttackers(pos, from, to);

    ASSERT_EQ(2, ba.num);
    ASSERT_EQ(Score::zero()     , ba.list[0].prom);
    ASSERT_EQ(material::BishopEx, ba.list[0].exch);
    ASSERT_EQ(Score::zero()     , ba.list[1].prom);
    ASSERT_EQ(material::DragonEx, ba.list[1].exch);

    ASSERT_EQ(2, wa.num);
    ASSERT_EQ(Score::zero()     , wa.list[0].prom);
    ASSERT_EQ(material::KnightEx, wa.list[0].exch);
    ASSERT_EQ(Score::zero()     , wa.list[1].prom);
    ASSERT_EQ(material::GoldEx  , wa.list[1].exch);
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

    SEE::Attackers ba;
    SEE::Attackers wa;
    std::tie(ba, wa) = SEE::generateAttackers(pos, from, to);

    ASSERT_EQ(3, ba.num);
    ASSERT_EQ(Score::zero()     , ba.list[0].prom);
    ASSERT_EQ(material::PawnEx  , ba.list[0].exch);
    ASSERT_EQ(Score::zero()     , ba.list[1].prom);
    ASSERT_EQ(material::KnightEx, ba.list[1].exch);
    ASSERT_EQ(Score::zero()     , ba.list[2].prom);
    ASSERT_EQ(material::RookEx  , ba.list[2].exch);

    ASSERT_EQ(2, wa.num);
    ASSERT_EQ(Score::zero()     , wa.list[0].prom);
    ASSERT_EQ(material::LanceEx , wa.list[0].exch);
    ASSERT_EQ(Score::zero()     , wa.list[1].prom);
    ASSERT_EQ(material::BishopEx, wa.list[1].exch);
  }
}

TEST(ScoreTest, testCalculate) {
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
}

TEST(ScoreTest, testSortMoves) {
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

    SEE::sortMoves(pos, moves, moves.begin(), false);

    ASSERT_EQ(5, moves.size());
    ASSERT_EQ(Move(Square::s74(), Square::s71(), true ), moves[0]);
    ASSERT_EQ(Move(Square::s85(), Square::s84(), false), moves[1]);
    ASSERT_EQ(Move(Square::s46(), Square::s45(), false), moves[2]);
    ASSERT_EQ(Move(Square::s56(), Square::s45(), false), moves[3]);
    ASSERT_EQ(Move(Square::s74(), Square::s84(), false), moves[4]);
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

    SEE::sortMoves(pos, moves, moves.begin(), true);

    ASSERT_EQ(4, moves.size());
    ASSERT_EQ(Move(Square::s56(), Square::s55(), false), moves[0]);
    ASSERT_EQ(Move(Square::s74(), Square::s73(), true ), moves[1]);
    ASSERT_EQ(Move(Square::s23(), Square::s22(), true ), moves[2]);
    ASSERT_EQ(Move(Square::s65(), Square::s73(), true ), moves[3]);
  }
}
