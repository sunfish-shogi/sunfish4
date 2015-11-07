/* CsaReaderTest.cpp
 *
 * Kubo Ryosuke
 */

#if !defined(NDEBUG)

#endif // !defined(NDEBUG)

#include "test/Test.hpp"
#include "core/record/CsaReader.hpp"

using namespace sunfish;

TEST(CsaReaderTest, testReadPosition) {
  {
    std::string src =
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
      "+\n";
    std::istringstream iss(src);
    Position pos;
    CsaReader::readPosition(iss, pos);

    ASSERT_EQ(
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
      "+\n", pos.toString());
  }

  {
    std::string src =
      "P1-KY-KE * -KI *  *  *  *  * \n"
      "P2 * -OU-GI-GI *  *  *  *  * \n"
      "P3 * -FU-FU-FU * -FU-KE *  * \n"
      "P4-FU *  *  * +GI *  *  * -FU\n"
      "P5 *  * +GI *  *  * -FU *  * \n"
      "P6+FU+HI *  * +UM *  *  *  * \n"
      "P7 * +FU+KE+FU+OU * +FU * +KE\n"
      "P8 *  * +KI *  *  *  *  *  * \n"
      "P9+KY *  * -RY *  *  *  * +KY\n"
      "P+00FU00FU00FU00KI00KI\n"
      "P-00FU00FU00FU00FU00KY00KA\n"
      "-\n";
    std::istringstream iss(src);
    Position pos;
    CsaReader::readPosition(iss, pos);

    ASSERT_EQ(
      "P1-KY-KE * -KI *  *  *  *  * \n"
      "P2 * -OU-GI-GI *  *  *  *  * \n"
      "P3 * -FU-FU-FU * -FU-KE *  * \n"
      "P4-FU *  *  * +GI *  *  * -FU\n"
      "P5 *  * +GI *  *  * -FU *  * \n"
      "P6+FU+HI *  * +UM *  *  *  * \n"
      "P7 * +FU+KE+FU+OU * +FU * +KE\n"
      "P8 *  * +KI *  *  *  *  *  * \n"
      "P9+KY *  * -RY *  *  *  * +KY\n"
      "P+00FU00FU00FU00KI00KI\n"
      "P-00FU00FU00FU00FU00KY00KA\n"
      "-\n", pos.toString());
  }
}
