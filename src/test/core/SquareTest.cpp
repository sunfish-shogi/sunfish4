/* SquareTest.cpp
 *
 * Kubo Ryosuke
 */

#include "test/Test.hpp"
#include "core/base/Square.hpp"
#include <sstream>

using namespace sunfish;

TEST(SquareTest, test) {
  {
    Square sq76(7, 6);
    ASSERT_EQ(Square::s76(), sq76);
  }

  {
    Square sq46(Square::s46());
    ASSERT_EQ(4, sq46.getFile());
    ASSERT_EQ(6, sq46.getRank());
  }

  {
    ASSERT_EQ(true , Square::isValidFile(1));
    ASSERT_EQ(true , Square::isValidFile(9));
    ASSERT_EQ(false, Square::isValidFile(10));

    ASSERT_EQ(true , Square::isValidRank(1));
    ASSERT_EQ(true , Square::isValidRank(9));
    ASSERT_EQ(false, Square::isValidRank(10));
  }
}

TEST(SquareTest, testIsPromotable) {
  {
    Square sq43(Square::s43());
    ASSERT_EQ(true , sq43.isPromotable<Turn::Black>());
    ASSERT_EQ(false, sq43.isPromotable<Turn::White>());

    Square sq94(Square::s94());
    ASSERT_EQ(false, sq94.isPromotable<Turn::Black>());
    ASSERT_EQ(false, sq94.isPromotable<Turn::White>());

    Square sq26(Square::s26());
    ASSERT_EQ(false, sq26.isPromotable<Turn::Black>());
    ASSERT_EQ(false, sq26.isPromotable<Turn::White>());

    Square sq87(Square::s87());
    ASSERT_EQ(false, sq87.isPromotable<Turn::Black>());
    ASSERT_EQ(true , sq87.isPromotable<Turn::White>());
  }

  {
    Square sq31(Square::s31());
    ASSERT_EQ(false, sq31.isPawnMovable<Turn::Black>());
    ASSERT_EQ(true , sq31.isPawnMovable<Turn::White>());

    Square sq32(Square::s32());
    ASSERT_EQ(true , sq32.isPawnMovable<Turn::Black>());
    ASSERT_EQ(true , sq32.isPawnMovable<Turn::White>());

    Square sq38(Square::s38());
    ASSERT_EQ(true , sq38.isPawnMovable<Turn::Black>());
    ASSERT_EQ(true , sq38.isPawnMovable<Turn::White>());

    Square sq39(Square::s39());
    ASSERT_EQ(true , sq39.isPawnMovable<Turn::Black>());
    ASSERT_EQ(false, sq39.isPawnMovable<Turn::White>());
  }

  {
    Square sq81(Square::s81());
    ASSERT_EQ(false, sq81.isLanceMovable<Turn::Black>());
    ASSERT_EQ(true , sq81.isLanceMovable<Turn::White>());

    Square sq82(Square::s82());
    ASSERT_EQ(true , sq82.isLanceMovable<Turn::Black>());
    ASSERT_EQ(true , sq82.isLanceMovable<Turn::White>());

    Square sq88(Square::s88());
    ASSERT_EQ(true , sq88.isLanceMovable<Turn::Black>());
    ASSERT_EQ(true , sq88.isLanceMovable<Turn::White>());

    Square sq89(Square::s89());
    ASSERT_EQ(true , sq89.isLanceMovable<Turn::Black>());
    ASSERT_EQ(false, sq89.isLanceMovable<Turn::White>());
  }

  {
    Square sq12(Square::s12());
    ASSERT_EQ(false, sq12.isKnightMovable<Turn::Black>());
    ASSERT_EQ(true , sq12.isKnightMovable<Turn::White>());

    Square sq13(Square::s13());
    ASSERT_EQ(true , sq13.isKnightMovable<Turn::Black>());
    ASSERT_EQ(true , sq13.isKnightMovable<Turn::White>());

    Square sq17(Square::s17());
    ASSERT_EQ(true , sq17.isKnightMovable<Turn::Black>());
    ASSERT_EQ(true , sq17.isKnightMovable<Turn::White>());

    Square sq18(Square::s18());
    ASSERT_EQ(true , sq18.isKnightMovable<Turn::Black>());
    ASSERT_EQ(false, sq18.isKnightMovable<Turn::White>());
  }
}

TEST(SquareTest, testReverse) {
  {
    Square sq34(3, 4);
    ASSERT_EQ(Square::s76(), sq34.psym());
    ASSERT_EQ(Square::s74(), sq34.hsym());
    ASSERT_EQ(Square::s36(), sq34.vsym());
  }
}

TEST(SquareTest, testDirections) {
  {
    Square sq55(5, 5);
    ASSERT_EQ(Square::s54(), sq55.up());
    ASSERT_EQ(Square::s56(), sq55.down());
    ASSERT_EQ(Square::s65(), sq55.left());
    ASSERT_EQ(Square::s45(), sq55.right());
    ASSERT_EQ(Square::s53(), sq55.up(2));
    ASSERT_EQ(Square::s57(), sq55.down(2));
    ASSERT_EQ(Square::s75(), sq55.left(2));
    ASSERT_EQ(Square::s35(), sq55.right(2));
    ASSERT_EQ(Square::s64(), sq55.leftUp());
    ASSERT_EQ(Square::s66(), sq55.leftDown());
    ASSERT_EQ(Square::s44(), sq55.rightUp());
    ASSERT_EQ(Square::s46(), sq55.rightDown());
    ASSERT_EQ(Square::s82(), sq55.leftUp(3));
    ASSERT_EQ(Square::s88(), sq55.leftDown(3));
    ASSERT_EQ(Square::s22(), sq55.rightUp(3));
    ASSERT_EQ(Square::s28(), sq55.rightDown(3));
    ASSERT_EQ(Square::s63(), sq55.leftUpKnight());
    ASSERT_EQ(Square::s67(), sq55.leftDownKnight());
    ASSERT_EQ(Square::s43(), sq55.rightUpKnight());
    ASSERT_EQ(Square::s47(), sq55.rightDownKnight());
  }

  {
    Square sq55(5, 5);
    ASSERT_EQ(Square::s54(), sq55.safetyUp());
    ASSERT_EQ(Square::s56(), sq55.safetyDown());
    ASSERT_EQ(Square::s65(), sq55.safetyLeft());
    ASSERT_EQ(Square::s45(), sq55.safetyRight());
    ASSERT_EQ(Square::s53(), sq55.safetyUp(2));
    ASSERT_EQ(Square::s57(), sq55.safetyDown(2));
    ASSERT_EQ(Square::s75(), sq55.safetyLeft(2));
    ASSERT_EQ(Square::s35(), sq55.safetyRight(2));
    ASSERT_EQ(Square::s64(), sq55.safetyLeftUp());
    ASSERT_EQ(Square::s66(), sq55.safetyLeftDown());
    ASSERT_EQ(Square::s44(), sq55.safetyRightUp());
    ASSERT_EQ(Square::s46(), sq55.safetyRightDown());
    ASSERT_EQ(Square::s82(), sq55.safetyLeftUp(3));
    ASSERT_EQ(Square::s88(), sq55.safetyLeftDown(3));
    ASSERT_EQ(Square::s22(), sq55.safetyRightUp(3));
    ASSERT_EQ(Square::s28(), sq55.safetyRightDown(3));
    ASSERT_EQ(Square::s63(), sq55.safetyLeftUpKnight());
    ASSERT_EQ(Square::s67(), sq55.safetyLeftDownKnight());
    ASSERT_EQ(Square::s43(), sq55.safetyRightUpKnight());
    ASSERT_EQ(Square::s47(), sq55.safetyRightDownKnight());

    ASSERT_TRUE(sq55.safetyUp(4).isValid());
    ASSERT_TRUE(sq55.safetyDown(4).isValid());
    ASSERT_TRUE(sq55.safetyLeft(4).isValid());
    ASSERT_TRUE(sq55.safetyRight(4).isValid());
    ASSERT_TRUE(sq55.safetyLeftUp(4).isValid());
    ASSERT_TRUE(sq55.safetyLeftDown(4).isValid());
    ASSERT_TRUE(sq55.safetyRightUp(4).isValid());
    ASSERT_TRUE(sq55.safetyRightDown(4).isValid());
    ASSERT_TRUE(sq55.safetyUp(5).isInvalid());
    ASSERT_TRUE(sq55.safetyDown(5).isInvalid());
    ASSERT_TRUE(sq55.safetyLeft(5).isInvalid());
    ASSERT_TRUE(sq55.safetyRight(5).isInvalid());
    ASSERT_TRUE(sq55.safetyLeftUp(5).isInvalid());
    ASSERT_TRUE(sq55.safetyLeftDown(5).isInvalid());
    ASSERT_TRUE(sq55.safetyRightUp(5).isInvalid());
    ASSERT_TRUE(sq55.safetyRightDown(5).isInvalid());

    Square sq91(9, 1);
    ASSERT_TRUE(sq91.safetyUp().isInvalid());
    ASSERT_TRUE(sq91.safetyDown().isValid());
    ASSERT_TRUE(sq91.safetyLeft().isInvalid());
    ASSERT_TRUE(sq91.safetyRight().isValid());
    ASSERT_TRUE(sq91.safetyLeftUp().isInvalid());
    ASSERT_TRUE(sq91.safetyLeftDown().isInvalid());
    ASSERT_TRUE(sq91.safetyRightUp().isInvalid());
    ASSERT_TRUE(sq91.safetyRightDown().isValid());
  }
}

TEST(SquareTest, testNext) {
  {
    Square sq55(5, 5);
    ASSERT_EQ(Square::s56(), sq55.next());

    Square sq59(5, 9);
    ASSERT_EQ(Square::s41(), sq59.next());

    Square sq19(1, 9);
    ASSERT_EQ(Square::End, sq19.next());
  }
}

TEST(SquareTest, testDistance) {
  {
    Square sq34(3, 4);
    Square sq38(3, 8);
    Square sq89(8, 9);
    ASSERT_EQ(0, sq34.distance(sq34));
    ASSERT_EQ(4, sq34.distance(sq38));
    ASSERT_EQ(5, sq34.distance(sq89));
  }
}

TEST(SquareTest, testDir) {
  {
    Square sq36(3, 6);
    Square sq63(6, 3);
    Square sq73(7, 3);
    Square sq38(3, 8);
    ASSERT_EQ(Direction::LeftUp, sq36.dir(sq63));
    ASSERT_EQ(Direction::None, sq36.dir(sq73));
    ASSERT_EQ(Direction::Down, sq36.dir(sq38));
  }
}

TEST(SquareTest, testMove) {
  {
    Square sq36(4, 6);
    ASSERT_EQ(Square::s35(), sq36.move(Direction::RightUp));
    ASSERT_EQ(Square::s56(), sq36.move(Direction::Left));
    ASSERT_EQ(Square::s58(), sq36.move(Direction::LeftDownKnight));
  }

  {
    Square sq79(7, 9);
    ASSERT_EQ(Square::s78(), sq79.safetyMove(Direction::Up));
    ASSERT_EQ(Square::invalid(), sq79.safetyMove(Direction::Down));
    ASSERT_EQ(Square::invalid(), sq79.safetyMove(Direction::RightDownKnight));
  }
}

TEST(SquareTest, testToString) {
  {
    ASSERT_EQ("11", Square::s11().toString());
    ASSERT_EQ("76", Square::s76().toString());
  }

  {
    std::ostringstream oss;
    oss << Square::s68();
    ASSERT_EQ("68", oss.str());
  }
}

TEST(SquareTest, testParse) {
  {
    ASSERT_EQ(Square::s36(), Square::parse("36"));
    ASSERT_EQ(Square::s88(), Square::parse("88"));
    ASSERT_EQ(false, Square::parse("hoge").isValid());
  }
}

TEST(SquareTest, testToStringSFEN) {
  {
    ASSERT_EQ("1a", Square::s11().toStringSFEN());
    ASSERT_EQ("7f", Square::s76().toStringSFEN());
  }
}

TEST(SquareTest, testParseSFEN) {
  {
    ASSERT_EQ(Square::s36(), Square::parseSFEN("3f"));
    ASSERT_EQ(Square::s88(), Square::parseSFEN("8h"));
    ASSERT_EQ(false, Square::parseSFEN("hoge").isValid());
  }
}

TEST(SquareTest, testReversedDir) {
  ASSERT_EQ(Direction::LeftUp, getReversedDir(Direction::RightDown));
  ASSERT_EQ(Direction::Up, getReversedDir(Direction::Down));
  ASSERT_EQ(Direction::RightUp, getReversedDir(Direction::LeftDown));
  ASSERT_EQ(Direction::Right, getReversedDir(Direction::Left));
  ASSERT_EQ(Direction::Left, getReversedDir(Direction::Right));
  ASSERT_EQ(Direction::LeftDown, getReversedDir(Direction::RightUp));
  ASSERT_EQ(Direction::Down, getReversedDir(Direction::Up));
  ASSERT_EQ(Direction::RightDown, getReversedDir(Direction::LeftUp));
  ASSERT_EQ(Direction::LeftUpKnight, getReversedDir(Direction::RightDownKnight));
  ASSERT_EQ(Direction::RightUpKnight, getReversedDir(Direction::LeftDownKnight));
  ASSERT_EQ(Direction::LeftDownKnight, getReversedDir(Direction::RightUpKnight));
  ASSERT_EQ(Direction::RightDownKnight, getReversedDir(Direction::LeftUpKnight));
  ASSERT_EQ(Direction::None, getReversedDir(Direction::None));
}

TEST(SquareTest, testRotate) {
  {
    ASSERT_EQ(10, Square(Square::s62()).rotate90().raw());
    ASSERT_EQ(62, Square(Square::s39()).rotate90().raw());
  }

  {
    ASSERT_EQ(1, Square(Square::s82()).rotateRight45().raw());
    ASSERT_EQ(2, Square(Square::s83()).rotateRight45().raw());
    ASSERT_EQ(3, Square(Square::s72()).rotateRight45().raw());
    ASSERT_EQ(49, Square(Square::s28()).rotateRight45().raw());

    ASSERT_EQ(0, Square(Square::s14()).rotateRight45().raw());
    ASSERT_EQ(0, Square(Square::s79()).rotateRight45().raw());
  }

  {
    ASSERT_EQ(1, Square(Square::s88()).rotateLeft45().raw());
    ASSERT_EQ(2, Square(Square::s87()).rotateLeft45().raw());
    ASSERT_EQ(3, Square(Square::s78()).rotateLeft45().raw());
    ASSERT_EQ(49, Square(Square::s22()).rotateLeft45().raw());

    ASSERT_EQ(0, Square(Square::s14()).rotateLeft45().raw());
    ASSERT_EQ(0, Square(Square::s79()).rotateLeft45().raw());
  }
}

TEST(SquareTest, testRelativeSquare) {
  // constructor
  ASSERT_EQ(RelativeSquare(144), RelativeSquare(Square::s62(), Square::s62()));
  ASSERT_EQ(RelativeSquare(73), RelativeSquare(Square::s48(), Square::s85()));
  ASSERT_EQ(RelativeSquare(285), RelativeSquare(Square::s93(), Square::s18()));
  ASSERT_EQ(RelativeSquare(Square::s88(), Square::s35()), RelativeSquare(Square::s76(), Square::s23()));

  // hsym
  ASSERT_EQ(RelativeSquare(Square::s88(), Square::s35()), RelativeSquare(Square::s37(), Square::s84()).hsym());
}
