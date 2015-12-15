/* MoveTest.cpp
 *
 * Kubo Ryosuke
 */

#if !defined(NDEBUG)

#include "test/Test.hpp"
#include "core/move/Move.hpp"
#include "core/record/CsaReader.hpp"
#include "core/util/PositionUtil.hpp"
#include <sstream>

using namespace sunfish;

TEST(MoveTest, test) {
  {
    Move move(Square::s57(), Square::s56(), false);
    ASSERT_EQ(Square::s57(), move.from().raw());
    ASSERT_EQ(Square::s56(), move.to().raw());
    ASSERT_EQ(false, move.isPromotion());
    ASSERT_EQ(false, move.isDrop());
    ASSERT_EQ(0, move.extData());
  }

  {
    Move move(Square::s54(), Square::s53(), true);
    ASSERT_EQ(Square::s54(), move.from().raw());
    ASSERT_EQ(Square::s53(), move.to().raw());
    ASSERT_EQ(true , move.isPromotion());
    ASSERT_EQ(false, move.isDrop());
    ASSERT_EQ(0, move.extData());
  }

  {
    Move move(PieceType::pawn(), Square::s55());
    ASSERT_EQ(Square::s55(), move.to().raw());
    ASSERT_EQ(false, move.isPromotion());
    ASSERT_EQ(PieceType::pawn(), move.droppingPieceType());
    ASSERT_EQ(true , move.isDrop());
    ASSERT_EQ(0, move.extData());
  }

  {
    Move move(Square::s57(), Square::s56(), false);
    move.setExtData(55555);
    ASSERT_EQ(Square::s57(), move.from().raw());
    ASSERT_EQ(Square::s56(), move.to().raw());
    ASSERT_EQ(false, move.isPromotion());
    ASSERT_EQ(false, move.isDrop());
    ASSERT_EQ(55555, move.extData());

    move = move.excludeExtData();
    ASSERT_EQ(Square::s57(), move.from().raw());
    ASSERT_EQ(Square::s56(), move.to().raw());
    ASSERT_EQ(false, move.isPromotion());
    ASSERT_EQ(false, move.isDrop());
    ASSERT_EQ(0, move.extData());
  }
}

TEST(MoveTest, testSerialize) {
  {
    Move move(Square::s57(), Square::s56(), false);
    auto rawValue = move.serialize();
    ASSERT_EQ(move, Move::deserialize(rawValue));
  }

  {
    Move move(Square::s14(), Square::s89(), true);
    auto rawValue = move.serialize();
    ASSERT_EQ(move, Move::deserialize(rawValue));
  }

  {
    Move move(PieceType::lance(), Square::s28());
    auto rawValue = move.serialize();
    ASSERT_EQ(move, Move::deserialize(rawValue));
  }
}

TEST(MoveTest, testSerialize16) {
  {
    Move move(Square::s57(), Square::s56(), false);
    auto rawValue = move.serialize16();
    ASSERT_EQ(move, Move::deserialize(rawValue));
  }

  {
    Move move(Square::s14(), Square::s89(), true);
    auto rawValue = move.serialize16();
    ASSERT_EQ(move, Move::deserialize(rawValue));
  }

  {
    Move move(PieceType::lance(), Square::s28());
    auto rawValue = move.serialize16();
    ASSERT_EQ(move, Move::deserialize(rawValue));
  }
}

TEST(MoveTest, testToString) {
  {
    ASSERT_EQ("3736", Move(Square::s37(), Square::s36(), false).toString());
    ASSERT_EQ("2822+", Move(Square::s28(), Square::s22(), true).toString());
    ASSERT_EQ("63KE", Move(PieceType::knight(), Square::s63()).toString());
  }

  {
    std::ostringstream oss;
    Move move(Square::s37(), Square::s36(), false);
    move.setExtData(12548);
    oss << move;
    ASSERT_EQ("3736(12548)", oss.str());
  }
}

TEST(MoveTest, testToStringCsa) {
  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  * -GI * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  *  *  * +FU *  * \n"
      "P8-KA *  *  *  *  *  * +HI * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+00KI00KE00FU\n"
      "P-00KA00FU\n"
      "+\n");
    ASSERT_EQ("+3736FU", Move(Square::s37(), Square::s36(), false).toString(pos));
    ASSERT_EQ("+2822RY", Move(Square::s28(), Square::s22(), true).toString(pos));
    ASSERT_EQ("+0063KE", Move(PieceType::knight(), Square::s63()).toString(pos));
  }

  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  * -GI * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  *  *  * +FU *  * \n"
      "P8-KA *  *  *  *  *  * +HI * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+00KI00KE00FU\n"
      "P-00KA00FU\n"
      "-\n");
    ASSERT_EQ("-5162OU", Move(Square::s51(), Square::s62(), false).toString(pos));
    ASSERT_EQ("-9854UM", Move(Square::s98(), Square::s54(), true).toString(pos));
    ASSERT_EQ("-0065KA", Move(PieceType::bishop(), Square::s65()).toString(pos));
  }
}

TEST(MoveTest, testToStringSFEN) {
  {
    ASSERT_EQ("3g3f", Move(Square::s37(), Square::s36(), false).toStringSFEN());
    ASSERT_EQ("5a6b", Move(Square::s51(), Square::s62(), false).toStringSFEN());
    ASSERT_EQ("2h2b+", Move(Square::s28(), Square::s22(), true).toStringSFEN());
    ASSERT_EQ("B*6e", Move(PieceType::bishop(), Square::s65()).toStringSFEN());
  }
}

#endif // !defined(NDEBUG)
