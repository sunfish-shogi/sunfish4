/* SquareTest.cpp
 *
 * Kubo Ryosuke
 */

#if !defined(NDEBUG)

#include "test/Test.hpp"
#include "core/base/Square.hpp"

using namespace sunfish;

TEST(SquareTest, test) {
  {
    Square sq76(7, 6);
    ASSERT_EQ(Square::S76, sq76);
  }

  {
    Square sq46(Square::S46);
    ASSERT_EQ(4, sq46.getFile());
    ASSERT_EQ(6, sq46.getRank());
  }
}

TEST(SquareTest, testIsPromotable) {
  {
    Square sq43(Square::S43);
    ASSERT_EQ(true, sq43.isPromotable<true>());
    ASSERT_EQ(false, sq43.isPromotable<false>());

    Square sq94(Square::S94);
    ASSERT_EQ(false, sq94.isPromotable<true>());
    ASSERT_EQ(false, sq94.isPromotable<false>());

    Square sq26(Square::S26);
    ASSERT_EQ(false, sq26.isPromotable<true>());
    ASSERT_EQ(false, sq26.isPromotable<false>());

    Square sq87(Square::S87);
    ASSERT_EQ(false, sq87.isPromotable<true>());
    ASSERT_EQ(true, sq87.isPromotable<false>());
  }

  {
    Square sq31(Square::S31);
    ASSERT_EQ(false, sq31.isPawnMovable<true>());
    ASSERT_EQ(true, sq31.isPawnMovable<false>());

    Square sq32(Square::S32);
    ASSERT_EQ(true, sq32.isPawnMovable<true>());
    ASSERT_EQ(true, sq32.isPawnMovable<false>());

    Square sq38(Square::S38);
    ASSERT_EQ(true, sq38.isPawnMovable<true>());
    ASSERT_EQ(true, sq38.isPawnMovable<false>());

    Square sq39(Square::S39);
    ASSERT_EQ(true, sq39.isPawnMovable<true>());
    ASSERT_EQ(false, sq39.isPawnMovable<false>());
  }

  {
    Square sq81(Square::S81);
    ASSERT_EQ(false, sq81.isLanceMovable<true>());
    ASSERT_EQ(true, sq81.isLanceMovable<false>());

    Square sq82(Square::S82);
    ASSERT_EQ(true, sq82.isLanceMovable<true>());
    ASSERT_EQ(true, sq82.isLanceMovable<false>());

    Square sq88(Square::S88);
    ASSERT_EQ(true, sq88.isLanceMovable<true>());
    ASSERT_EQ(true, sq88.isLanceMovable<false>());

    Square sq89(Square::S89);
    ASSERT_EQ(true, sq89.isLanceMovable<true>());
    ASSERT_EQ(false, sq89.isLanceMovable<false>());
  }

  {
    Square sq12(Square::S12);
    ASSERT_EQ(false, sq12.isKnightMovable<true>());
    ASSERT_EQ(true, sq12.isKnightMovable<false>());

    Square sq13(Square::S13);
    ASSERT_EQ(true, sq13.isKnightMovable<true>());
    ASSERT_EQ(true, sq13.isKnightMovable<false>());

    Square sq17(Square::S17);
    ASSERT_EQ(true, sq17.isKnightMovable<true>());
    ASSERT_EQ(true, sq17.isKnightMovable<false>());

    Square sq18(Square::S18);
    ASSERT_EQ(true, sq18.isKnightMovable<true>());
    ASSERT_EQ(false, sq18.isKnightMovable<false>());
  }
}

TEST(SquareTest, testReverse) {
  {
    Square sq34(3, 4);
    ASSERT_EQ(Square::S76, sq34.dsym());
    ASSERT_EQ(Square::S74, sq34.hsym());
    ASSERT_EQ(Square::S36, sq34.vsym());
  }
}

TEST(SquareTest, testDirections) {
  {
    Square sq55(5, 5);
    ASSERT_EQ(Square::S54, sq55.up());
    ASSERT_EQ(Square::S56, sq55.down());
    ASSERT_EQ(Square::S65, sq55.left());
    ASSERT_EQ(Square::S45, sq55.right());
    ASSERT_EQ(Square::S53, sq55.up(2));
    ASSERT_EQ(Square::S57, sq55.down(2));
    ASSERT_EQ(Square::S75, sq55.left(2));
    ASSERT_EQ(Square::S35, sq55.right(2));
    ASSERT_EQ(Square::S64, sq55.leftUp());
    ASSERT_EQ(Square::S66, sq55.leftDown());
    ASSERT_EQ(Square::S44, sq55.rightUp());
    ASSERT_EQ(Square::S46, sq55.rightDown());
    ASSERT_EQ(Square::S82, sq55.leftUp(3));
    ASSERT_EQ(Square::S88, sq55.leftDown(3));
    ASSERT_EQ(Square::S22, sq55.rightUp(3));
    ASSERT_EQ(Square::S28, sq55.rightDown(3));
    ASSERT_EQ(Square::S63, sq55.leftUpKnight());
    ASSERT_EQ(Square::S67, sq55.leftDownKnight());
    ASSERT_EQ(Square::S43, sq55.rightUpKnight());
    ASSERT_EQ(Square::S47, sq55.rightDownKnight());
  }

  {
    Square sq55(5, 5);
    ASSERT_EQ(Square::S54, sq55.safetyUp());
    ASSERT_EQ(Square::S56, sq55.safetyDown());
    ASSERT_EQ(Square::S65, sq55.safetyLeft());
    ASSERT_EQ(Square::S45, sq55.safetyRight());
    ASSERT_EQ(Square::S53, sq55.safetyUp(2));
    ASSERT_EQ(Square::S57, sq55.safetyDown(2));
    ASSERT_EQ(Square::S75, sq55.safetyLeft(2));
    ASSERT_EQ(Square::S35, sq55.safetyRight(2));
    ASSERT_EQ(Square::S64, sq55.safetyLeftUp());
    ASSERT_EQ(Square::S66, sq55.safetyLeftDown());
    ASSERT_EQ(Square::S44, sq55.safetyRightUp());
    ASSERT_EQ(Square::S46, sq55.safetyRightDown());
    ASSERT_EQ(Square::S82, sq55.safetyLeftUp(3));
    ASSERT_EQ(Square::S88, sq55.safetyLeftDown(3));
    ASSERT_EQ(Square::S22, sq55.safetyRightUp(3));
    ASSERT_EQ(Square::S28, sq55.safetyRightDown(3));
    ASSERT_EQ(Square::S63, sq55.safetyLeftUpKnight());
    ASSERT_EQ(Square::S67, sq55.safetyLeftDownKnight());
    ASSERT_EQ(Square::S43, sq55.safetyRightUpKnight());
    ASSERT_EQ(Square::S47, sq55.safetyRightDownKnight());

    ASSERT(sq55.safetyUp(4).isValid());
    ASSERT(sq55.safetyDown(4).isValid());
    ASSERT(sq55.safetyLeft(4).isValid());
    ASSERT(sq55.safetyRight(4).isValid());
    ASSERT(sq55.safetyLeftUp(4).isValid());
    ASSERT(sq55.safetyLeftDown(4).isValid());
    ASSERT(sq55.safetyRightUp(4).isValid());
    ASSERT(sq55.safetyRightDown(4).isValid());
    ASSERT(sq55.safetyUp(5).isInvalid());
    ASSERT(sq55.safetyDown(5).isInvalid());
    ASSERT(sq55.safetyLeft(5).isInvalid());
    ASSERT(sq55.safetyRight(5).isInvalid());
    ASSERT(sq55.safetyLeftUp(5).isInvalid());
    ASSERT(sq55.safetyLeftDown(5).isInvalid());
    ASSERT(sq55.safetyRightUp(5).isInvalid());
    ASSERT(sq55.safetyRightDown(5).isInvalid());

    Square sq91(9, 1);
    ASSERT(sq91.safetyUp().isInvalid());
    ASSERT(sq91.safetyDown().isValid());
    ASSERT(sq91.safetyLeft().isInvalid());
    ASSERT(sq91.safetyRight().isValid());
    ASSERT(sq91.safetyLeftUp().isInvalid());
    ASSERT(sq91.safetyLeftDown().isInvalid());
    ASSERT(sq91.safetyRightUp().isInvalid());
    ASSERT(sq91.safetyRightDown().isValid());
  }
}

TEST(SquareTest, testNext) {
  {
    Square sq55(5, 5);
    ASSERT_EQ(Square::S56, sq55.next());

    Square sq59(5, 9);
    ASSERT_EQ(Square::S41, sq59.next());

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

TEST(SquareTest, testToString) {
  {
    ASSERT_EQ("11", Square(Square::S11).toString());
    ASSERT_EQ("76", Square(Square::S76).toString());
  }
}

TEST(SquareTest, testReversedDir) {
  ASSERT_EQ(Direction::Up, getReversedDir(Direction::Down));
}

TEST(SquareTest, testRotate) {
  {
    ASSERT_EQ(20, Square(Square::S62).rotate0().raw());
    ASSERT_EQ(54, Square(Square::S39).rotate0().raw());
  }

  {
    ASSERT_EQ(4, Square(Square::S62).rotate90().raw());
    ASSERT_EQ(0, Square(Square::S39).rotate90().raw());
  }

  {
    ASSERT_EQ(1, Square(Square::S82).rotateRight45().raw());
    ASSERT_EQ(2, Square(Square::S83).rotateRight45().raw());
    ASSERT_EQ(3, Square(Square::S72).rotateRight45().raw());
    ASSERT_EQ(49, Square(Square::S28).rotateRight45().raw());

    ASSERT_EQ(0, Square(Square::S14).rotateRight45().raw());
    ASSERT_EQ(0, Square(Square::S79).rotateRight45().raw());
  }

  {
    ASSERT_EQ(1, Square(Square::S88).rotateLeft45().raw());
    ASSERT_EQ(2, Square(Square::S87).rotateLeft45().raw());
    ASSERT_EQ(3, Square(Square::S78).rotateLeft45().raw());
    ASSERT_EQ(49, Square(Square::S22).rotateLeft45().raw());

    ASSERT_EQ(0, Square(Square::S14).rotateLeft45().raw());
    ASSERT_EQ(0, Square(Square::S79).rotateLeft45().raw());
  }
}

#endif // !defined(NDEBUG)
