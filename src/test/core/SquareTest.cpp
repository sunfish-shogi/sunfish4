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
    ASSERT_EQ(S76, sq76);
  }

  {
    Square sq46(S46);
    ASSERT_EQ(4, sq46.getFile());
    ASSERT_EQ(6, sq46.getRank());
  }
}

TEST(SquareTest, testIsPromotable) {
  {
    Square sq43(S43);
    ASSERT_EQ(true, sq43.isPromotable<true>());
    ASSERT_EQ(false, sq43.isPromotable<false>());

    Square sq94(S94);
    ASSERT_EQ(false, sq94.isPromotable<true>());
    ASSERT_EQ(false, sq94.isPromotable<false>());

    Square sq26(S26);
    ASSERT_EQ(false, sq26.isPromotable<true>());
    ASSERT_EQ(false, sq26.isPromotable<false>());

    Square sq87(S87);
    ASSERT_EQ(false, sq87.isPromotable<true>());
    ASSERT_EQ(true, sq87.isPromotable<false>());
  }

  {
    Square sq31(S31);
    ASSERT_EQ(false, sq31.isPawnMovable<true>());
    ASSERT_EQ(true, sq31.isPawnMovable<false>());

    Square sq32(S32);
    ASSERT_EQ(true, sq32.isPawnMovable<true>());
    ASSERT_EQ(true, sq32.isPawnMovable<false>());

    Square sq38(S38);
    ASSERT_EQ(true, sq38.isPawnMovable<true>());
    ASSERT_EQ(true, sq38.isPawnMovable<false>());

    Square sq39(S39);
    ASSERT_EQ(true, sq39.isPawnMovable<true>());
    ASSERT_EQ(false, sq39.isPawnMovable<false>());
  }

  {
    Square sq81(S81);
    ASSERT_EQ(false, sq81.isLanceMovable<true>());
    ASSERT_EQ(true, sq81.isLanceMovable<false>());

    Square sq82(S82);
    ASSERT_EQ(true, sq82.isLanceMovable<true>());
    ASSERT_EQ(true, sq82.isLanceMovable<false>());

    Square sq88(S88);
    ASSERT_EQ(true, sq88.isLanceMovable<true>());
    ASSERT_EQ(true, sq88.isLanceMovable<false>());

    Square sq89(S89);
    ASSERT_EQ(true, sq89.isLanceMovable<true>());
    ASSERT_EQ(false, sq89.isLanceMovable<false>());
  }

  {
    Square sq12(S12);
    ASSERT_EQ(false, sq12.isKnightMovable<true>());
    ASSERT_EQ(true, sq12.isKnightMovable<false>());

    Square sq13(S13);
    ASSERT_EQ(true, sq13.isKnightMovable<true>());
    ASSERT_EQ(true, sq13.isKnightMovable<false>());

    Square sq17(S17);
    ASSERT_EQ(true, sq17.isKnightMovable<true>());
    ASSERT_EQ(true, sq17.isKnightMovable<false>());

    Square sq18(S18);
    ASSERT_EQ(true, sq18.isKnightMovable<true>());
    ASSERT_EQ(false, sq18.isKnightMovable<false>());
  }
}

TEST(SquareTest, testReverse) {
  {
    Square sq34(3, 4);
    ASSERT_EQ(S76, sq34.reverse());
    ASSERT_EQ(S74, sq34.sym());
  }
}

TEST(SquareTest, testDirections) {
  {
    Square sq55(5, 5);
    ASSERT_EQ(S54, sq55.up());
    ASSERT_EQ(S56, sq55.down());
    ASSERT_EQ(S65, sq55.left());
    ASSERT_EQ(S45, sq55.right());
    ASSERT_EQ(S53, sq55.up(2));
    ASSERT_EQ(S57, sq55.down(2));
    ASSERT_EQ(S75, sq55.left(2));
    ASSERT_EQ(S35, sq55.right(2));
    ASSERT_EQ(S64, sq55.leftUp());
    ASSERT_EQ(S66, sq55.leftDown());
    ASSERT_EQ(S44, sq55.rightUp());
    ASSERT_EQ(S46, sq55.rightDown());
    ASSERT_EQ(S82, sq55.leftUp(3));
    ASSERT_EQ(S88, sq55.leftDown(3));
    ASSERT_EQ(S22, sq55.rightUp(3));
    ASSERT_EQ(S28, sq55.rightDown(3));
    ASSERT_EQ(S63, sq55.leftUpKnight());
    ASSERT_EQ(S67, sq55.leftDownKnight());
    ASSERT_EQ(S43, sq55.rightUpKnight());
    ASSERT_EQ(S47, sq55.rightDownKnight());
  }

  {
    Square sq55(5, 5);
    ASSERT_EQ(S54, sq55.safetyUp());
    ASSERT_EQ(S56, sq55.safetyDown());
    ASSERT_EQ(S65, sq55.safetyLeft());
    ASSERT_EQ(S45, sq55.safetyRight());
    ASSERT_EQ(S53, sq55.safetyUp(2));
    ASSERT_EQ(S57, sq55.safetyDown(2));
    ASSERT_EQ(S75, sq55.safetyLeft(2));
    ASSERT_EQ(S35, sq55.safetyRight(2));
    ASSERT_EQ(S64, sq55.safetyLeftUp());
    ASSERT_EQ(S66, sq55.safetyLeftDown());
    ASSERT_EQ(S44, sq55.safetyRightUp());
    ASSERT_EQ(S46, sq55.safetyRightDown());
    ASSERT_EQ(S82, sq55.safetyLeftUp(3));
    ASSERT_EQ(S88, sq55.safetyLeftDown(3));
    ASSERT_EQ(S22, sq55.safetyRightUp(3));
    ASSERT_EQ(S28, sq55.safetyRightDown(3));
    ASSERT_EQ(S63, sq55.safetyLeftUpKnight());
    ASSERT_EQ(S67, sq55.safetyLeftDownKnight());
    ASSERT_EQ(S43, sq55.safetyRightUpKnight());
    ASSERT_EQ(S47, sq55.safetyRightDownKnight());

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
    ASSERT_EQ(S56, sq55.next());

    Square sq59(5, 9);
    ASSERT_EQ(S41, sq59.next());

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
    ASSERT_EQ("11", Square(S11).toString());
    ASSERT_EQ("76", Square(S76).toString());
  }
}

TEST(SquareTest, testReversedDir) {
  ASSERT_EQ(Direction::Up, getReversedDir(Direction::Down));
}

#endif // !defined(NDEBUG)
