/* SfenParserTest.cpp
 *
 * Kubo Ryosuke
 */

#if !defined(NDEBUG)

#include "test/Test.hpp"
#include "core/position/Position.hpp"
#include "core/record/SfenParser.hpp"
#include "core/util/PositionUtil.hpp"

using namespace sunfish;

TEST(SfenParserTest, parsePosition) {
  {
    const char* data = "lnsgkgsn1/1r5b1/ppppppppp/9/9/9/PPPPPPPPP/1B5R1/LNSGKGSNL w - 1";
    Position pos;
    bool ok = SfenParser::parsePosition(data, pos);
    ASSERT_EQ(true, ok);
    ASSERT_EQ(
      "P1-KY-KE-GI-KI-OU-KI-GI-KE * \n"
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
      "-\n", pos.toString());
  }

  {
    const char* data = "4k4/6+B2/9/9/9/3+p5/9/9/4K4 b P2G15p3n 1";
    Position pos;
    bool ok = SfenParser::parsePosition(data, pos);
    ASSERT_EQ(true, ok);
    ASSERT_EQ(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  * +UM *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  * -TO *  *  *  *  * \n"
      "P7 *  *  *  *  *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+00FU00KI00KI\n"
      "P-00FU00FU00FU00FU00FU00FU00FU00FU00FU00FU00FU00FU00FU00FU00FU00KE00KE00KE\n"
      "+\n", pos.toString());
  }
}

TEST(SfenParserTest, parseMove) {
  {
    const char* data = "7g7f";
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
    Move move;
    bool ok = SfenParser::parseMove(data, pos, move);
    ASSERT_EQ(true, ok);
    ASSERT_EQ(Move(Piece::blackPawn(), Square::s77(), Square::s76(), false), move);
  }

  {
    const char* data = "2b8h";
    Position pos = PositionUtil::createPositionFromCsaString(
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
    Move move;
    bool ok = SfenParser::parseMove(data, pos, move);
    ASSERT_EQ(true, ok);
    ASSERT_EQ(Move(Piece::whiteBishop(), Square::s22(), Square::s88(), false), move);
  }

  {
    const char* data = "G*4e";
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
      "P+00KA\n"
      "P-00KI\n"
      "-\n");
    Move move;
    bool ok = SfenParser::parseMove(data, pos, move);
    ASSERT_EQ(true, ok);
    ASSERT_EQ(Move(Piece::whiteGold(), Square::s45()), move);
  }
}

#endif // !defined(NDEBUG)
