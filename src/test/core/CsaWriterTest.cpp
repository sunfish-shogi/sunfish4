/* CsaWriterTest.cpp
 *
 * Kubo Ryosuke
 */

#if !defined(NDEBUG)

#include "test/Test.hpp"
#include "core/record/CsaWriter.hpp"
#include "core/util/PositionUtil.hpp"
#include <sstream>

using namespace sunfish;

TEST(CsaWriterTest, testWrite) {
  {
    Record record = {
      PositionUtil::createPositionFromCsaString(
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
        "+\n"),
      {
        Move(Square::s77(), Square::s76(), false),
        Move(Square::s33(), Square::s34(), false),
        Move(Square::s88(), Square::s22(), true),
        Move(Square::s31(), Square::s22(), false),
        Move(PieceType::bishop(), Square::s45()),
        Move(Square::s71(), Square::s62(), false),
      }
    };
    RecordInfo ri;
    ri.title = "WCSC2016";
    ri.blackName = "Sunfish";
    ri.whiteName = "Firefly";
    ri.timeLimitHours= 9;
    ri.timeLimitMinutes = 30;
    ri.timeLimitReadoff = 60;

    const char* expect =
      "$EVENT:WCSC2016\n"
      "N+Sunfish\n"
      "N-Firefly\n"
      "$TIME_LIMIT:09:30+60\n"
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
      "+\n"
      "+7776FU\n"
      "-3334FU\n"
      "+8822UM\n"
      "-3122GI\n"
      "+0045KA\n"
      "-7162GI\n";

    std::ostringstream oss;
    bool ok = CsaWriter::write(oss,
                               record,
                               &ri);

    ASSERT_TRUE(ok);
    ASSERT_EQ(expect, oss.str());
  }
}

#endif // !defined(NDEBUG)
