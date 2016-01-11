/* CsaReaderTest.cpp
 *
 * Kubo Ryosuke
 */

#if !defined(NDEBUG)

#include "test/Test.hpp"
#include "core/record/CsaReader.hpp"

using namespace sunfish;

TEST(CsaReaderTest, testRead) {
  {
    std::string src =
      "$EVENT:WCSC2016\n"
      "N+Sunfish\n"
      "N-Firefly\n"
      "$TIME_LIMIT:01:25+30\n"
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
      "+2726FU\n"
      "-2288UM\n";
    std::istringstream iss(src);
    Record record;
    RecordInfo ri;
    CsaReader::read(iss, record, &ri);

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
      "+\n", record.initialPosition.toString());
    ASSERT_EQ("WCSC2016", ri.title);
    ASSERT_EQ("Sunfish", ri.blackName);
    ASSERT_EQ("Firefly", ri.whiteName);
    ASSERT_EQ(1, ri.timeLimitHours);
    ASSERT_EQ(25, ri.timeLimitMinutes);
    ASSERT_EQ(30, ri.timeLimitReadoff);
    ASSERT_EQ(4, record.moveList.size());
    ASSERT_EQ(Move(Square::s77(), Square::s76(), false), record.moveList[0]);
    ASSERT_EQ(Move(Square::s33(), Square::s34(), false), record.moveList[1]);
    ASSERT_EQ(Move(Square::s27(), Square::s26(), false), record.moveList[2]);
    ASSERT_EQ(Move(Square::s22(), Square::s88(), true ), record.moveList[3]);
  }
}

TEST(CsaReaderTest, testReadPosition) {
  {
    std::string src =
      "$EVENT:WCSC2016\n"
      "N+Sunfish\n"
      "N-Firefly\n"
      "$TIME_LIMIT:01:25+30\n"
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
    RecordInfo ri;
    CsaReader::readPosition(iss, pos, &ri);

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
    ASSERT_EQ("WCSC2016", ri.title);
    ASSERT_EQ("Sunfish", ri.blackName);
    ASSERT_EQ("Firefly", ri.whiteName);
    ASSERT_EQ(1, ri.timeLimitHours);
    ASSERT_EQ(25, ri.timeLimitMinutes);
    ASSERT_EQ(30, ri.timeLimitReadoff);
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

  {
    const char* src[] = {
      "$EVENT:WCSC2016",
      "N+Sunfish",
      "N-Firefly",
      "$TIME_LIMIT:01:25+30",
      "P1-KY-KE-GI-KI-OU-KI-GI-KE-KY",
      "P2 * -HI *  *  *  *  * -KA * ",
      "P3-FU-FU-FU-FU-FU-FU-FU-FU-FU",
      "P4 *  *  *  *  *  *  *  *  * ",
      "P5 *  *  *  *  *  *  *  *  * ",
      "P6 *  *  *  *  *  *  *  *  * ",
      "P7+FU+FU+FU+FU+FU+FU+FU+FU+FU",
      "P8 * +KA *  *  *  *  * +HI * ",
      "P9+KY+KE+GI+KI+OU+KI+GI+KE+KY",
      "P+",
      "P-",
      "+"};
    MutablePosition mp;
    RecordInfo ri;
    initializeMutablePosition(mp);
    for (const char* line : src) {
      CsaReader::readPosition(line, mp, &ri);
    }

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
      "+\n", Position(mp).toString());
    ASSERT_EQ("WCSC2016", ri.title);
    ASSERT_EQ("Sunfish", ri.blackName);
    ASSERT_EQ("Firefly", ri.whiteName);
    ASSERT_EQ(1, ri.timeLimitHours);
    ASSERT_EQ(25, ri.timeLimitMinutes);
    ASSERT_EQ(30, ri.timeLimitReadoff);
  }

  {
    const char* src[] = {
      "P1-KY-KE * -KI *  *  *  *  * ",
      "P2 * -OU-GI-GI *  *  *  *  * ",
      "P3 * -FU-FU-FU * -FU-KE *  * ",
      "P4-FU *  *  * +GI *  *  * -FU",
      "P5 *  * +GI *  *  * -FU *  * ",
      "P6+FU+HI *  * +UM *  *  *  * ",
      "P7 * +FU+KE+FU+OU * +FU * +KE",
      "P8 *  * +KI *  *  *  *  *  * ",
      "P9+KY *  * -RY *  *  *  * +KY",
      "P+00FU00FU00FU00KI00KI",
      "P-00FU00FU00FU00FU00KY00KA",
      "-"};
    MutablePosition mp;
    initializeMutablePosition(mp);
    for (const char* line : src) {
      CsaReader::readPosition(line, mp);
    }

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
      "-\n", Position(mp).toString());
  }
}

#endif // !defined(NDEBUG)
