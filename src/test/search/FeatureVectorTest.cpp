/* FeatureVectorTest.cpp
 *
 * Kubo Ryosuke
 */

#include "test/Test.hpp"
#include "search/eval/FeatureVector.hpp"
#include "core/position/Position.hpp"
#include "core/util/PositionUtil.hpp"

using namespace sunfish;

TEST(FeatureVectorTest, testSafetyOfKing) {
  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1-KY-KE *  *  *  *  *  *  * \n"
      "P2 * -OU-KI *  *  *  *  *  * \n"
      "P3 * -FU-GI-KI *  *  *  *  * \n"
      "P4-FU * -FU-FU *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  * +FU\n"
      "P7 *  *  *  *  * +FU+FU+FU * \n"
      "P8 *  *  *  *  *  * +GI+OU * \n"
      "P9 *  *  *  *  * +KI * +KE+KY\n"
      "P+\n"
      "P-\n"
      "+\n");
    ASSERT_EQ(6, safetyOfKing<Turn::Black>(pos));
    ASSERT_EQ(7, safetyOfKing<Turn::White>(pos));
  }

  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1-KY-KE * +RY *  *  *  *  * \n"
      "P2 * -OU-KI *  *  *  *  *  * \n"
      "P3 * -FU-GI-KI *  *  *  *  * \n"
      "P4-FU * -FU-FU *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  * -FU+FU\n"
      "P7 *  *  *  *  * +FU+FU+FU * \n"
      "P8 *  *  *  *  * -KI+GI+OU * \n"
      "P9 *  *  *  *  * +KI * +KE+KY\n"
      "P+\n"
      "P-\n"
      "+\n");
    ASSERT_EQ(3, safetyOfKing<Turn::Black>(pos));
    ASSERT_EQ(5, safetyOfKing<Turn::White>(pos));
  }
}
