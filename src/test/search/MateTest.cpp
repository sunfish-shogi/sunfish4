/* MateTest.cpp
 *
 * Kubo Ryosuke
 */

#include "test/Test.hpp"
#include "search/mate/Mate.hpp"
#include "core/util/PositionUtil.hpp"
#include <sstream>

using namespace sunfish;

TEST(MateTest, testMate1Ply) {
  {
    // mate
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  * +FU *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  *  *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+00KI\n"
      "P-\n"
      "+\n");
    bool mate = Mate::mate1Ply(pos);
    ASSERT_EQ(true, mate);
  }

  {
    // mate
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  * -FU *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+\n"
      "P-00KI\n"
      "-\n");
    bool mate = Mate::mate1Ply(pos);
    ASSERT_EQ(true, mate);
  }

  {
    // mate
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  * -FU * -FU *  *  * \n"
      "P3 *  *  *  * +FU *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  *  *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+00GI\n"
      "P-\n"
      "+\n");
    bool mate = Mate::mate1Ply(pos);
    ASSERT_EQ(true, mate);
  }

  {
    // no mate
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  * +FU *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  *  *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+00GI\n"
      "P-\n"
      "+\n");
    bool mate = Mate::mate1Ply(pos);
    ASSERT_EQ(false, mate);
  }

  {
    // no mate
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  * -FU *  *  *  * \n"
      "P8 *  *  * +FU * +FU *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+\n"
      "P-00GI\n"
      "-\n");
    bool mate = Mate::mate1Ply(pos);
    ASSERT_EQ(true, mate);
  }

  {
    // no mate
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  * -FU *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+\n"
      "P-00GI\n"
      "-\n");
    bool mate = Mate::mate1Ply(pos);
    ASSERT_EQ(false, mate);
  }

  {
    // mate
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  *  *  *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  * +GI\n"
      "P3 *  *  *  *  *  *  *  * -FU\n"
      "P4 *  *  *  *  *  *  * +FU-OU\n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  * +RY * \n"
      "P7 *  *  *  *  *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+\n"
      "P-\n"
      "+\n");
    bool mate = Mate::mate1Ply(pos);
    ASSERT_EQ(true, mate);
  }

  {
    // mate
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  *  *  *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  * +NG * \n"
      "P4 *  *  *  *  *  *  *  * -OU\n"
      "P5 *  *  *  *  *  *  * +FU-FU\n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  *  *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+00FU\n"
      "P-00FU\n"
      "+\n");
    bool mate = Mate::mate1Ply(pos);
    ASSERT_EQ(true, mate);
  }

  {
    // mate
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  *  *  *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  * +GI *  * \n"
      "P4 *  *  *  *  *  *  *  * -OU\n"
      "P5 *  *  *  *  *  *  * +FU-FU\n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  *  *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+00FU\n"
      "P-00FU\n"
      "+\n");
    bool mate = Mate::mate1Ply(pos);
    ASSERT_EQ(true, mate);
  }

  {
    // mate
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  *  *  *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  * +GI *  * \n"
      "P5 *  *  *  *  *  *  *  * -OU\n"
      "P6 *  *  *  *  *  *  * +FU-FU\n"
      "P7 *  *  *  *  *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+00FU\n"
      "P-00FU\n"
      "+\n");
    bool mate = Mate::mate1Ply(pos);
    ASSERT_EQ(false, mate);
  }

  {
    // mate
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  * -FU+FU\n"
      "P6 *  *  *  *  *  *  *  * +OU\n"
      "P7 *  *  *  *  *  *  * -NG * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  *  *  *  *  *  * \n"
      "P+00FU\n"
      "P-00FU\n"
      "-\n");
    bool mate = Mate::mate1Ply(pos);
    ASSERT_EQ(true, mate);
  }

  {
    // no mate
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  *  *  *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  * +NG * \n"
      "P4 *  *  *  *  *  *  *  * -OU\n"
      "P5 *  *  *  *  *  *  * +FU * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  *  *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+00FU\n"
      "P-00FU\n"
      "+\n");
    bool mate = Mate::mate1Ply(pos);
    ASSERT_EQ(false, mate);
  }

  {
    // no mate
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  * -FU * \n"
      "P6 *  *  *  *  *  *  *  * +OU\n"
      "P7 *  *  *  *  *  *  * -NG * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  *  *  *  *  *  * \n"
      "P+00FU\n"
      "P-00FU\n"
      "-\n");
    bool mate = Mate::mate1Ply(pos);
    ASSERT_EQ(false, mate);
  }

  {
    // mate
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  * -KA *  *  * \n"
      "P7 *  *  *  *  *  * -GI * +FU\n"
      "P8 *  *  *  *  *  *  *  * +OU\n"
      "P9 *  *  *  *  *  *  *  *  * \n"
      "P+00FU\n"
      "P-00FU\n"
      "-\n");
    bool mate = Mate::mate1Ply(pos);
    ASSERT_EQ(true, mate);
  }

  {
    // mate
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  *  *  *  * -KE-OU\n"
      "P2 *  *  *  *  *  *  * -KA-KY\n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  *  *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+00KE\n"
      "P-00FU\n"
      "+\n");
    bool mate = Mate::mate1Ply(pos);
    ASSERT_EQ(true, mate);
  }

  {
    // no mate
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  *  *  *  * -KE-OU\n"
      "P2 *  *  *  *  *  *  * -KA * \n"
      "P3 *  *  *  *  *  *  *  * -KY\n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  *  *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+00KE\n"
      "P-00FU\n"
      "+\n");
    bool mate = Mate::mate1Ply(pos);
    ASSERT_EQ(false, mate);
  }

  {
    // mate
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  * +FU+FU * -KI *  * \n"
      "P7 *  *  *  * +OU *  *  *  * \n"
      "P8 *  * -RY *  *  *  *  *  * \n"
      "P9 *  *  *  *  *  *  *  *  * \n"
      "P+00FU\n"
      "P-00KE\n"
      "-\n");
    bool mate = Mate::mate1Ply(pos);
    ASSERT_EQ(true, mate);
  }

  {
    // mate
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  *  *  *  * -KE-OU\n"
      "P2 *  *  *  *  *  *  * -KA-KY\n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  * +KE *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  *  *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+\n"
      "P-00FU\n"
      "+\n");
    bool mate = Mate::mate1Ply(pos);
    ASSERT_EQ(true, mate);
  }

  {
    // mate
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  * -KY *  *  *  * \n"
      "P5 *  *  *  * -KE *  *  *  * \n"
      "P6 *  *  * +FU *  * -KI *  * \n"
      "P7 *  *  *  * +OU *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  * -HI *  *  *  *  * \n"
      "P+00FU\n"
      "P-00KE\n"
      "-\n");
    bool mate = Mate::mate1Ply(pos);
    ASSERT_EQ(true, mate);
  }

  {
    // mate
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  *  *  * -OU *  * \n"
      "P2 *  *  *  *  *  * -FU *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  * +KY *  * \n"
      "P5 *  *  *  *  *  * +KY *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  *  *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+\n"
      "P-00FU\n"
      "+\n");
    bool mate = Mate::mate1Ply(pos);
    ASSERT_EQ(true, mate);
  }

  {
    // mate
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  *  *  *  *  *  * \n"
      "P2 *  *  *  *  *  * -GI * -KY\n"
      "P3 *  *  *  *  *  * -FU-OU-FU\n"
      "P4 *  *  *  *  *  *  * -FU * \n"
      "P5 *  *  *  *  *  * +GI * +FU\n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  *  *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  * +KY * \n"
      "P+\n"
      "P-00FU\n"
      "+\n");
    bool mate = Mate::mate1Ply(pos);
    ASSERT_EQ(true, mate);
  }

  {
    // mate
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 * -GI *  *  *  *  *  *  * \n"
      "P5 * -FU *  *  *  *  *  *  * \n"
      "P6+OU *  *  *  *  *  *  *  * \n"
      "P7 * +FU *  *  *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  *  *  *  *  *  * \n"
      "P+00FU\n"
      "P-00KY\n"
      "-\n");
    bool mate = Mate::mate1Ply(pos);
    ASSERT_EQ(true, mate);
  }

  {
    // mate
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  *  *  *  *  *  * \n"
      "P2 *  *  * +GI *  *  *  *  * \n"
      "P3 *  *  * -GI *  *  *  *  * \n"
      "P4 *  * -FU-OU *  *  *  *  * \n"
      "P5 *  *  * -FU *  *  *  *  * \n"
      "P6 *  *  *  * +HI *  *  *  * \n"
      "P7 *  *  *  *  *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+00KA\n"
      "P-00FU\n"
      "+\n");
    bool mate = Mate::mate1Ply(pos);
    ASSERT_EQ(true, mate);
  }

  {
    // mate
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  *  *  * +KA *  * \n"
      "P2 *  *  * +GI *  *  *  *  * \n"
      "P3 *  *  * -GI-FU *  *  *  * \n"
      "P4 *  * -FU-OU *  *  *  *  * \n"
      "P5 *  *  * -FU *  *  *  *  * \n"
      "P6 *  *  *  * +FU *  *  *  * \n"
      "P7 *  *  *  *  *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+\n"
      "P-00FU\n"
      "+\n");
    bool mate = Mate::mate1Ply(pos);
    ASSERT_EQ(true, mate);
  }

  {
    // mate
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  *  *  *  *  *  * \n"
      "P2 *  *  * +GI *  *  *  *  * \n"
      "P3 *  *  * -GI-FU+UM *  *  * \n"
      "P4 *  * -FU-OU *  *  *  *  * \n"
      "P5 *  *  * -FU *  *  *  *  * \n"
      "P6 *  *  *  * +FU *  *  *  * \n"
      "P7 *  *  *  *  *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+\n"
      "P-00FU\n"
      "+\n");
    bool mate = Mate::mate1Ply(pos);
    ASSERT_EQ(true, mate);
  }

  {
    // mate
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  * -GI *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6+FU+OU+FU *  *  *  *  *  * \n"
      "P7 * +FU *  *  *  *  *  *  * \n"
      "P8 * -KA *  *  *  *  *  *  * \n"
      "P9 *  *  *  *  *  *  *  *  * \n"
      "P+00FU\n"
      "P-00KA\n"
      "-\n");
    bool mate = Mate::mate1Ply(pos);
    ASSERT_EQ(true, mate);
  }

  {
    // mate
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4-FU *  *  *  *  *  *  *  * \n"
      "P5 *  * -FU *  *  *  *  *  * \n"
      "P6+FU+OU+FU *  *  *  *  *  * \n"
      "P7 * +FU *  *  *  *  *  *  * \n"
      "P8 * -KA *  * -KA *  *  *  * \n"
      "P9 *  *  *  *  *  *  *  *  * \n"
      "P+00FU\n"
      "P-\n"
      "-\n");
    bool mate = Mate::mate1Ply(pos);
    ASSERT_EQ(true, mate);
  }

  {
    // mate
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  *  *  *  *  *  * \n"
      "P2 *  *  * +GI *  *  *  *  * \n"
      "P3 *  *  *  * -FU *  *  *  * \n"
      "P4 *  * -FU-OU *  *  *  *  * \n"
      "P5 *  *  * -FU *  *  *  *  * \n"
      "P6 *  * +FU * +HI *  *  *  * \n"
      "P7 *  *  *  *  *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+\n"
      "P-00FU\n"
      "+\n");
    bool mate = Mate::mate1Ply(pos);
    ASSERT_EQ(true, mate);
  }

  {
    // mate
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  *  *  *  *  *  * \n"
      "P2 *  * -GI-KY * +UM *  *  * \n"
      "P3 *  *  * -OU *  *  *  *  * \n"
      "P4 *  * -FU-FU *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  * +FU *  *  *  *  *  * \n"
      "P7 *  *  *  *  *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+00HI\n"
      "P-00FU\n"
      "+\n");
    bool mate = Mate::mate1Ply(pos);
    ASSERT_EQ(true, mate);
  }

  {
    // no mate
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  *  *  *  *  *  * \n"
      "P2 *  * -GI-KY * +UM *  *  * \n"
      "P3 *  *  * -OU *  *  *  *  * \n"
      "P4 *  * -FU *  *  *  *  *  * \n"
      "P5 *  *  * -FU *  *  *  *  * \n"
      "P6 *  * +FU *  *  *  *  *  * \n"
      "P7 *  *  *  *  *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+00HI\n"
      "P-00FU\n"
      "+\n");
    bool mate = Mate::mate1Ply(pos);
    ASSERT_EQ(false, mate);
  }

  {
    // mate
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 * -FU *  *  *  *  *  *  * \n"
      "P5-FU * +FU *  *  *  *  *  * \n"
      "P6 * +OU-FU *  *  *  *  *  * \n"
      "P7 * +FU *  *  *  *  *  *  * \n"
      "P8 * -KA *  *  *  *  *  *  * \n"
      "P9 *  *  *  *  *  *  *  *  * \n"
      "P+00FU\n"
      "P-00HI\n"
      "-\n");
    bool mate = Mate::mate1Ply(pos);
    ASSERT_EQ(true, mate);
  }

  {
    // mate
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5-FU * +FU *  *  *  *  *  * \n"
      "P6 * +OU-FU *  *  *  *  *  * \n"
      "P7 * +FU *  *  *  *  *  *  * \n"
      "P8 * -KA *  *  *  *  *  *  * \n"
      "P9-RY *  *  *  *  *  *  *  * \n"
      "P+00FU\n"
      "P-\n"
      "-\n");
    bool mate = Mate::mate1Ply(pos);
    ASSERT_EQ(true, mate);
  }

  {
    // no mate
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  * -HI * \n"
      "P3 *  *  *  * +FU *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  *  *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+00KI\n"
      "P-00FU\n"
      "+\n");
    bool mate = Mate::mate1Ply(pos);
    ASSERT_EQ(false, mate);
  }

  {
    // no mate
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  * -FU *  *  *  * \n"
      "P8 * +HI *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+00FU\n"
      "P-00KI\n"
      "-\n");
    bool mate = Mate::mate1Ply(pos);
    ASSERT_EQ(false, mate);
  }

  {
    // no mate
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  * +TO *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  *  *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+\n"
      "P-\n"
      "+\n");
    bool mate = Mate::mate1Ply(pos);
    ASSERT_EQ(false, mate);
  }

  {
    // no mate
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  * -TO *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+\n"
      "P-\n"
      "-\n");
    bool mate = Mate::mate1Ply(pos);
    ASSERT_EQ(false, mate);
  }

  {
    // mate
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  * -FU-OU-FU *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  * +FU * +FU *  *  * \n"
      "P4 *  *  *  * -FU *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  *  *  *  *  *  * \n"
      "P8 *  *  *  * +HI *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+00FU\n"
      "P-\n"
      "+\n");
    bool mate = Mate::mate1Ply(pos);
    ASSERT_EQ(true, mate);
  }

  {
    // no mate
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  * -FU-OU-FU *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  * +FU * +FU *  *  * \n"
      "P4 *  *  *  * -FU *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  *  *  *  *  *  * \n"
      "P8 *  *  *  * +HI *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+00FU\n"
      "P-00FU\n"
      "+\n");
    bool mate = Mate::mate1Ply(pos);
    ASSERT_EQ(false, mate);
  }

  {
    // no mate
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  * -FU-OU-FU *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  * +FU * +KI *  *  * \n"
      "P4 *  *  *  * -FU * +HI *  * \n"
      "P5 *  *  *  * -UM *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  *  *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+00FU\n"
      "P-00FU\n"
      "+\n");
    bool mate = Mate::mate1Ply(pos);
    ASSERT_EQ(false, mate);
  }

  {
    // no mate
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1-KY * +KI * -OU *  * -KE * \n"
      "P2 *  *  *  * -HI *  *  *  * \n"
      "P3-FU * -KE+TO *  *  * -FU * \n"
      "P4 * +FU-FU * -FU * -GI *  * \n"
      "P5+FU * +GI *  * -FU *  * -FU\n"
      "P6 *  * +HI * +FU * +FU * +GI\n"
      "P7 *  * +KE * -KA+FU+KI+FU * \n"
      "P8 *  *  *  *  *  *  *  * +KY\n"
      "P9+KY *  *  *  *  * +KI+KE+OU\n"
      "P+00FU00FU00FU00GI\n"
      "P-00FU00FU00KY00KI00KI00KA\n"
      "+\n");
    bool mate = Mate::mate1Ply(pos);
    ASSERT_EQ(false, mate);
  }

  {
    // no mate
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1-KY * -KI *  * -OU-KA+HI-KY\n"
      "P2 *  *  * -GI *  *  * -GI * \n"
      "P3-FU *  * -FU * +KE-KE-FU-FU\n"
      "P4 *  *  *  *  *  * -KE *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  * -FU *  * -FU *  *  * \n"
      "P7+FU+FU-KI-KA *  * +FU * +FU\n"
      "P8 *  *  * -GI * +OU+GI *  * \n"
      "P9+KY *  *  *  *  *  * +HI+KY\n"
      "P+00KI00KI00FU00FU00FU00FU00FU00FU\n"
      "P-00KE00FU00FU\n"
      "-\n");
    bool mate = Mate::mate1Ply(pos);
    ASSERT_EQ(false, mate);
  }

  {
    // pawn push(mate)
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  *  *  *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  * -KY\n"
      "P3 *  *  *  *  *  *  *  * -OU\n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  * +RY+FU\n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  *  *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+\n"
      "P-\n"
      "+\n");
    bool mate = Mate::mate1Ply(pos);
    ASSERT_EQ(true, mate);
  }

  {
    // pawn drop(no mate)
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  *  *  *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  * -KY\n"
      "P3 *  *  *  *  *  *  *  * -OU\n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  * +RY * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  *  *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+00FU\n"
      "P-\n"
      "+\n");
    bool mate = Mate::mate1Ply(pos);
    ASSERT_EQ(false, mate);
  }
}
