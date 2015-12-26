/* ShekTest.cpp
 *
 * Kubo Ryosuke
 */

#if !defined(NDEBUG)

#include "test/Test.hpp"
#include "core/util/PositionUtil.hpp"
#include "search/shek/ShekTable.hpp"

using namespace sunfish;

TEST(ShekTest, testBlack) {
  ShekTable table;

  Position pos = PositionUtil::createPositionFromCsaString(
    "P1 *  *  *  * -OU *  *  *  * \n"
    "P2 *  *  *  *  *  *  *  *  * \n"
    "P3 *  * -KA *  *  * -FU *  * \n"
    "P4 *  *  *  *  * -FU * -FU * \n"
    "P5 *  *  *  *  *  *  *  *  * \n"
    "P6 *  * +FU *  *  * +GI *  * \n"
    "P7 *  *  * +FU *  *  *  *  * \n"
    "P8 *  *  *  *  *  *  *  *  * \n"
    "P9 *  *  *  * +OU *  *  *  * \n"
    "P+00KI00KE00FU00FU\n"
    "P-00HI00KY00FU\n"
    "+\n");

  // superior (lance)
  Position supPos1 = PositionUtil::createPositionFromCsaString(
    "P1 *  *  *  * -OU *  *  *  * \n"
    "P2 *  *  *  *  *  *  *  *  * \n"
    "P3 *  * -KA *  *  * -FU *  * \n"
    "P4 *  *  *  *  * -FU * -FU * \n"
    "P5 *  *  *  *  *  *  *  *  * \n"
    "P6 *  * +FU *  *  * +GI *  * \n"
    "P7 *  *  * +FU *  *  *  *  * \n"
    "P8 *  *  *  *  *  *  *  *  * \n"
    "P9 *  *  *  * +OU *  *  *  * \n"
    "P+00KI00KE00KY00FU00FU\n"
    "P-00HI00FU\n"
    "+\n");

  // superior (turn)
  Position supPos2 = PositionUtil::createPositionFromCsaString(
    "P1 *  *  *  * -OU *  *  *  * \n"
    "P2 *  *  *  *  *  *  *  *  * \n"
    "P3 *  * -KA *  *  * -FU *  * \n"
    "P4 *  *  *  *  * -FU * -FU * \n"
    "P5 *  *  *  *  *  *  *  *  * \n"
    "P6 *  * +FU *  *  * +GI *  * \n"
    "P7 *  *  * +FU *  *  *  *  * \n"
    "P8 *  *  *  *  *  *  *  *  * \n"
    "P9 *  *  *  * +OU *  *  *  * \n"
    "P+00KI00KE00FU00FU\n"
    "P-00HI00KY00FU\n"
    "-\n");

  // superior (turn, pawn)
  Position supPos3 = PositionUtil::createPositionFromCsaString(
    "P1 *  *  *  * -OU *  *  *  * \n"
    "P2 *  *  *  *  *  *  *  *  * \n"
    "P3 *  * -KA *  *  * -FU *  * \n"
    "P4 *  *  *  *  * -FU * -FU * \n"
    "P5 *  *  *  *  *  *  *  *  * \n"
    "P6 *  * +FU *  *  * +GI *  * \n"
    "P7 *  *  * +FU *  *  *  *  * \n"
    "P8 *  *  *  *  *  *  *  *  * \n"
    "P9 *  *  *  * +OU *  *  *  * \n"
    "P+00KI00KE00FU\n"
    "P-00HI00KY00FU00FU\n"
    "-\n");

  // inferior (gold)
  Position infPos1 = PositionUtil::createPositionFromCsaString(
    "P1 *  *  *  * -OU *  *  *  * \n"
    "P2 *  *  *  *  *  *  *  *  * \n"
    "P3 *  * -KA *  *  * -FU *  * \n"
    "P4 *  *  *  *  * -FU * -FU * \n"
    "P5 *  *  *  *  *  *  *  *  * \n"
    "P6 *  * +FU *  *  * +GI *  * \n"
    "P7 *  *  * +FU *  *  *  *  * \n"
    "P8 *  *  *  *  *  *  *  *  * \n"
    "P9 *  *  *  * +OU *  *  *  * \n"
    "P+00KE00FU00FU\n"
    "P-00HI00KI00KY00FU\n"
    "+\n");

  // inferior (gold, pawn)
  Position infPos2 = PositionUtil::createPositionFromCsaString(
    "P1 *  *  *  * -OU *  *  *  * \n"
    "P2 *  *  *  *  *  *  *  *  * \n"
    "P3 *  * -KA *  *  * -FU *  * \n"
    "P4 *  *  *  *  * -FU * -FU * \n"
    "P5 *  *  *  *  *  *  *  *  * \n"
    "P6 *  * +FU *  *  * +GI *  * \n"
    "P7 *  *  * +FU *  *  *  *  * \n"
    "P8 *  *  *  *  *  *  *  *  * \n"
    "P9 *  *  *  * +OU *  *  *  * \n"
    "P+00KE00FU\n"
    "P-00HI00KI00KY00FU00FU\n"
    "+\n");

  // n/a (turn, pawn)
  Position nonPos1 = PositionUtil::createPositionFromCsaString(
    "P1 *  *  *  * -OU *  *  *  * \n"
    "P2 *  *  *  *  *  *  *  *  * \n"
    "P3 *  * -KA *  *  * -FU *  * \n"
    "P4 *  *  *  *  * -FU * -FU * \n"
    "P5 *  *  *  *  *  *  *  *  * \n"
    "P6 *  * +FU *  *  * +GI *  * \n"
    "P7 *  *  * +FU *  *  *  *  * \n"
    "P8 *  *  *  *  *  *  *  *  * \n"
    "P9 *  *  *  * +OU *  *  *  * \n"
    "P+00KI00KE00FU00FU00FU\n"
    "P-00HI00KY\n"
    "-\n");

  // n/a (gold, rook)
  Position nonPos2 = PositionUtil::createPositionFromCsaString(
    "P1 *  *  *  * -OU *  *  *  * \n"
    "P2 *  *  *  *  *  *  *  *  * \n"
    "P3 *  * -KA *  *  * -FU *  * \n"
    "P4 *  *  *  *  * -FU * -FU * \n"
    "P5 *  *  *  *  *  *  *  *  * \n"
    "P6 *  * +FU *  *  * +GI *  * \n"
    "P7 *  *  * +FU *  *  *  *  * \n"
    "P8 *  *  *  *  *  *  *  *  * \n"
    "P9 *  *  *  * +OU *  *  *  * \n"
    "P+00HI00KE00FU00FU\n"
    "P-00KI00KY00FU\n"
    "+\n");

  ASSERT_EQ(ShekState::None, table.check(pos));

  table.retain(pos); // 0 => 1
  ASSERT_EQ(ShekState::Equal   , table.check(pos));
  ASSERT_EQ(ShekState::Superior, table.check(supPos1));
  ASSERT_EQ(ShekState::Superior, table.check(supPos2));
  ASSERT_EQ(ShekState::Superior, table.check(supPos3));
  ASSERT_EQ(ShekState::Inferior, table.check(infPos1));
  ASSERT_EQ(ShekState::Inferior, table.check(infPos2));
  ASSERT_EQ(ShekState::None    , table.check(nonPos1));
  ASSERT_EQ(ShekState::None    , table.check(nonPos2));

  table.retain(pos); // 1 => 2
  ASSERT_EQ(ShekState::Equal, table.check(pos));
  table.retain(pos); // 2 => 3
  ASSERT_EQ(ShekState::Equal4, table.check(pos));

  table.release(pos); // 3 => 2
  ASSERT_EQ(ShekState::Equal, table.check(pos));
  table.release(pos); // 2 => 1
  ASSERT_EQ(ShekState::Equal, table.check(pos));

  table.release(pos); // 1 => 0
  ASSERT_EQ(ShekState::None, table.check(pos));
}

TEST(ShekTest, testWhite) {
  ShekTable table;

  Position pos = PositionUtil::createPositionFromCsaString(
    "P1 *  *  *  *  *  *  *  *  * \n"
    "P2 *  *  *  * +TO *  *  *  * \n"
    "P3 *  *  * -OU+FU *  *  *  * \n"
    "P4 *  *  *  *  *  *  *  *  * \n"
    "P5 *  *  *  *  * +HI *  *  * \n"
    "P6 *  *  *  *  *  *  *  *  * \n"
    "P7 *  * +OU *  *  *  *  *  * \n"
    "P8 *  *  *  *  *  * -UM *  * \n"
    "P9 *  *  *  *  *  *  *  *  * \n"
    "P+00HI00HI00KI00FU\n"
    "P-00KI00GI00KE00KY\n"
    "-\n");

  // superior (gold)
  Position supPos1 = PositionUtil::createPositionFromCsaString(
    "P1 *  *  *  *  *  *  *  *  * \n"
    "P2 *  *  *  * +TO *  *  *  * \n"
    "P3 *  *  * -OU+FU *  *  *  * \n"
    "P4 *  *  *  *  *  *  *  *  * \n"
    "P5 *  *  *  *  * +HI *  *  * \n"
    "P6 *  *  *  *  *  *  *  *  * \n"
    "P7 *  * +OU *  *  *  *  *  * \n"
    "P8 *  *  *  *  *  * -UM *  * \n"
    "P9 *  *  *  *  *  *  *  *  * \n"
    "P+00HI00HI00FU\n"
    "P-00KI00KI00GI00KE00KY\n"
    "-\n");

  // superior (turn)
  Position supPos2 = PositionUtil::createPositionFromCsaString(
    "P1 *  *  *  *  *  *  *  *  * \n"
    "P2 *  *  *  * +TO *  *  *  * \n"
    "P3 *  *  * -OU+FU *  *  *  * \n"
    "P4 *  *  *  *  *  *  *  *  * \n"
    "P5 *  *  *  *  * +HI *  *  * \n"
    "P6 *  *  *  *  *  *  *  *  * \n"
    "P7 *  * +OU *  *  *  *  *  * \n"
    "P8 *  *  *  *  *  * -UM *  * \n"
    "P9 *  *  *  *  *  *  *  *  * \n"
    "P+00HI00HI00KI00FU\n"
    "P-00KI00GI00KE00KY\n"
    "+\n");

  // superior (turn, lance)
  Position supPos3 = PositionUtil::createPositionFromCsaString(
    "P1 *  *  *  *  *  *  *  *  * \n"
    "P2 *  *  *  * +TO *  *  *  * \n"
    "P3 *  *  * -OU+FU *  *  *  * \n"
    "P4 *  *  *  *  *  *  *  *  * \n"
    "P5 *  *  *  *  * +HI *  *  * \n"
    "P6 *  *  *  *  *  *  *  *  * \n"
    "P7 *  * +OU *  *  *  *  *  * \n"
    "P8 *  *  *  *  *  * -UM *  * \n"
    "P9 *  *  *  *  *  *  *  *  * \n"
    "P+00HI00HI00KI00KY00FU\n"
    "P-00KI00GI00KE\n"
    "+\n");

  // inferior (knight)
  Position infPos1 = PositionUtil::createPositionFromCsaString(
    "P1 *  *  *  *  *  *  *  *  * \n"
    "P2 *  *  *  * +TO *  *  *  * \n"
    "P3 *  *  * -OU+FU *  *  *  * \n"
    "P4 *  *  *  *  *  *  *  *  * \n"
    "P5 *  *  *  *  * +HI *  *  * \n"
    "P6 *  *  *  *  *  *  *  *  * \n"
    "P7 *  * +OU *  *  *  *  *  * \n"
    "P8 *  *  *  *  *  * -UM *  * \n"
    "P9 *  *  *  *  *  *  *  *  * \n"
    "P+00HI00HI00KI00KE00FU\n"
    "P-00KI00GI00KY\n"
    "-\n");

  // inferior (silver, lance)
  Position infPos2 = PositionUtil::createPositionFromCsaString(
    "P1 *  *  *  *  *  *  *  *  * \n"
    "P2 *  *  *  * +TO *  *  *  * \n"
    "P3 *  *  * -OU+FU *  *  *  * \n"
    "P4 *  *  *  *  *  *  *  *  * \n"
    "P5 *  *  *  *  * +HI *  *  * \n"
    "P6 *  *  *  *  *  *  *  *  * \n"
    "P7 *  * +OU *  *  *  *  *  * \n"
    "P8 *  *  *  *  *  * -UM *  * \n"
    "P9 *  *  *  *  *  *  *  *  * \n"
    "P+00HI00HI00KI00GI00KY00FU\n"
    "P-00KI00KE\n"
    "-\n");

  // n/a (turn, pawn)
  Position nonPos1 = PositionUtil::createPositionFromCsaString(
    "P1 *  *  *  *  *  *  *  *  * \n"
    "P2 *  *  *  * +TO *  *  *  * \n"
    "P3 *  *  * -OU+FU *  *  *  * \n"
    "P4 *  *  *  *  *  *  *  *  * \n"
    "P5 *  *  *  *  * +HI *  *  * \n"
    "P6 *  *  *  *  *  *  *  *  * \n"
    "P7 *  * +OU *  *  *  *  *  * \n"
    "P8 *  *  *  *  *  * -UM *  * \n"
    "P9 *  *  *  *  *  *  *  *  * \n"
    "P+00HI00HI00KI\n"
    "P-00KI00GI00KE00KY00FU\n"
    "+\n");

  // n/a (pawn, silver)
  Position nonPos2 = PositionUtil::createPositionFromCsaString(
    "P1 *  *  *  *  *  *  *  *  * \n"
    "P2 *  *  *  * +TO *  *  *  * \n"
    "P3 *  *  * -OU+FU *  *  *  * \n"
    "P4 *  *  *  *  *  *  *  *  * \n"
    "P5 *  *  *  *  * +HI *  *  * \n"
    "P6 *  *  *  *  *  *  *  *  * \n"
    "P7 *  * +OU *  *  *  *  *  * \n"
    "P8 *  *  *  *  *  * -UM *  * \n"
    "P9 *  *  *  *  *  *  *  *  * \n"
    "P+00HI00HI00KI00GI\n"
    "P-00KI00KE00KY00FU\n"
    "-\n");

  ASSERT_EQ(ShekState::None, table.check(pos));

  table.retain(pos); // 0 => 1
  ASSERT_EQ(ShekState::Equal   , table.check(pos));
  ASSERT_EQ(ShekState::Superior, table.check(supPos1));
  ASSERT_EQ(ShekState::Superior, table.check(supPos2));
  ASSERT_EQ(ShekState::Superior, table.check(supPos3));
  ASSERT_EQ(ShekState::Inferior, table.check(infPos1));
  ASSERT_EQ(ShekState::Inferior, table.check(infPos2));
  ASSERT_EQ(ShekState::None    , table.check(nonPos1));
  ASSERT_EQ(ShekState::None    , table.check(nonPos2));
}

TEST(ShekTest, testSlot) {
  ShekTable table;

  Position pos1 = PositionUtil::createPositionFromCsaString(
    "P1 *  *  *  *  * -KI *  *  * \n"
    "P2 *  *  * -KI *  *  *  *  * \n"
    "P3 *  *  *  *  *  * -FU *  * \n"
    "P4 * -KA-OU *  * -FU * -FU * \n"
    "P5 *  *  *  *  *  *  *  *  * \n"
    "P6 *  *  * +FU+FU *  * +HI * \n"
    "P7 *  * +FU *  * +OU *  *  * \n"
    "P8 *  *  * +GI *  *  *  *  * \n"
    "P9 *  * +KI *  *  *  *  *  * \n"
    "P+00KA\n"
    "P-00FU\n"
    "-\n");

  Position pos2 = PositionUtil::createPositionFromCsaString(
    "P1 *  *  *  *  * -KI *  *  * \n"
    "P2 *  *  * -KI *  *  *  *  * \n"
    "P3 *  *  *  *  *  * -FU *  * \n"
    "P4 * -KA-OU *  * -FU * -FU * \n"
    "P5 *  *  *  *  *  *  *  *  * \n"
    "P6 *  *  * +FU+FU *  * +HI * \n"
    "P7 *  * +FU *  * +OU *  *  * \n"
    "P8 *  *  * +GI *  *  *  *  * \n"
    "P9 *  * +KI *  *  *  *  *  * \n"
    "P+\n"
    "P-00FU00KA\n"
    "-\n");

  table.retain(pos1);
  ASSERT_EQ(ShekState::Equal   , table.check(pos1));
  ASSERT_EQ(ShekState::Superior, table.check(pos2));

  table.retain(pos2);
  ASSERT_EQ(ShekState::Equal   , table.check(pos1));
  ASSERT_EQ(ShekState::Superior, table.check(pos2));

  table.release(pos1);
  ASSERT_EQ(ShekState::Inferior, table.check(pos1));
  ASSERT_EQ(ShekState::Equal   , table.check(pos2));

  table.release(pos2);
  ASSERT_EQ(ShekState::None    , table.check(pos1));
  ASSERT_EQ(ShekState::None    , table.check(pos2));
}

#endif // !defined(NDEBUG)
