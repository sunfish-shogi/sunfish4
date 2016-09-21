/* BitboardTest.cpp
 *
 * Kubo Ryosuke
 */

#include "test/Test.hpp"
#include "core/position/Bitboard.hpp"
#include <sstream>

using namespace sunfish;

TEST(BitboardTest, test) {
  {
    Bitboard bb(0x00LL, 0x00LL);
    ASSERT_EQ(0x00LL, bb.first());
    ASSERT_EQ(0x00LL, bb.second());

    Bitboard bb2(0x8e3fLL, 0x76a1LL);
    ASSERT_EQ(0x8e3fLL, bb2.first());
    ASSERT_EQ(0x76a1LL, bb2.second());
  }

  {
    Bitboard bb(Bitboard::zero());
    ASSERT_EQ(0x00LL, bb.first());
    ASSERT_EQ(0x00LL, bb.second());

    Bitboard bb2(0x8e3fLL, 0x76a1LL);
    Bitboard bb3(bb2);
    ASSERT_EQ(0x8e3fLL, bb3.first());
    ASSERT_EQ(0x76a1LL, bb3.second());
  }

  {
    RotatedBitboard bb(0x00LL);
    ASSERT_EQ(0x00LL, bb.raw());

    RotatedBitboard bb2(0x8e3fLL);
    RotatedBitboard bb3(bb2);
    ASSERT_EQ(0x8e3fLL, bb3.raw());
  }
}

TEST(BitboardTest, testConstants) {
  {
    ASSERT_EQ(Bitboard::zero().set(Square::s34()), Bitboard::mask(Square::s34()));
    ASSERT_EQ(Bitboard::zero().set(Square::s58()), Bitboard::mask(Square::s58()));
    ASSERT_EQ(Bitboard::zero().set(Square::s96()), Bitboard::mask(Square::s96()));
  }

  {
    ASSERT_EQ(
        "111111111\n"
        "111111111\n"
        "111111111\n"
        "000000000\n"
        "000000000\n"
        "000000000\n"
        "000000000\n"
        "000000000\n"
        "000000000\n",
        Bitboard::blackPromotable().toString());

    ASSERT_EQ(
        "000000000\n"
        "000000000\n"
        "000000000\n"
        "111111111\n"
        "111111111\n"
        "111111111\n"
        "111111111\n"
        "111111111\n"
        "111111111\n",
        Bitboard::blackNotPromotable().toString());
  }

  {
    ASSERT_EQ(
        "000000000\n"
        "000000000\n"
        "000000000\n"
        "000000000\n"
        "000000000\n"
        "000000000\n"
        "111111111\n"
        "111111111\n"
        "111111111\n",
        Bitboard::whitePromotable().toString());
  }

  {
    ASSERT_EQ(
        "111111111\n"
        "111111111\n"
        "111111111\n"
        "111111111\n"
        "111111111\n"
        "111111111\n"
        "000000000\n"
        "000000000\n"
        "000000000\n",
        Bitboard::whiteNotPromotable().toString());
  }

  {
    ASSERT_EQ(
        "111111111\n"
        "111111111\n"
        "000000000\n"
        "000000000\n"
        "000000000\n"
        "000000000\n"
        "000000000\n"
        "000000000\n"
        "000000000\n",
        Bitboard::blackPromotable2().toString());
  }

  {
    ASSERT_EQ(
        "000000000\n"
        "000000000\n"
        "000000000\n"
        "000000000\n"
        "000000000\n"
        "000000000\n"
        "000000000\n"
        "111111111\n"
        "111111111\n",
        Bitboard::whitePromotable2().toString());
  }

  {
    ASSERT_EQ(
        "111111111\n"
        "000000000\n"
        "000000000\n"
        "000000000\n"
        "000000000\n"
        "000000000\n"
        "000000000\n"
        "000000000\n"
        "000000000\n",
        Bitboard::rank1().toString());
  }

  {
    ASSERT_EQ(
        "000000000\n"
        "111111111\n"
        "000000000\n"
        "000000000\n"
        "000000000\n"
        "000000000\n"
        "000000000\n"
        "000000000\n"
        "000000000\n",
        Bitboard::rank2().toString());
  }

  {
    ASSERT_EQ(
        "000000000\n"
        "000000000\n"
        "111111111\n"
        "111111111\n"
        "111111111\n"
        "111111111\n"
        "111111111\n"
        "111111111\n"
        "111111111\n",
        Bitboard::rank3to9().toString());
  }

  {
    ASSERT_EQ(
        "000000000\n"
        "111111111\n"
        "111111111\n"
        "111111111\n"
        "111111111\n"
        "111111111\n"
        "111111111\n"
        "111111111\n"
        "111111111\n",
        Bitboard::rank2to9().toString());
  }

  {
    ASSERT_EQ(
        "000000000\n"
        "000000000\n"
        "000000000\n"
        "000000000\n"
        "000000000\n"
        "000000000\n"
        "000000000\n"
        "000000000\n"
        "111111111\n",
        Bitboard::rank9().toString());
  }

  {
    ASSERT_EQ(
        "000000000\n"
        "000000000\n"
        "000000000\n"
        "000000000\n"
        "000000000\n"
        "000000000\n"
        "000000000\n"
        "111111111\n"
        "000000000\n",
        Bitboard::rank8().toString());
  }

  {
    ASSERT_EQ(
        "111111111\n"
        "111111111\n"
        "111111111\n"
        "111111111\n"
        "111111111\n"
        "111111111\n"
        "111111111\n"
        "000000000\n"
        "000000000\n",
        Bitboard::rank1to7().toString());
  }

  {
    ASSERT_EQ(
        "111111111\n"
        "111111111\n"
        "111111111\n"
        "111111111\n"
        "111111111\n"
        "111111111\n"
        "111111111\n"
        "111111111\n"
        "000000000\n",
        Bitboard::rank1to8().toString());
  }

  {
    ASSERT_EQ(
        "000000001\n"
        "000000001\n"
        "000000001\n"
        "000000001\n"
        "000000001\n"
        "000000001\n"
        "000000001\n"
        "000000001\n"
        "000000001\n",
        Bitboard::file1().toString());
  }

  {
    ASSERT_EQ(
        "000000010\n"
        "000000010\n"
        "000000010\n"
        "000000010\n"
        "000000010\n"
        "000000010\n"
        "000000010\n"
        "000000010\n"
        "000000010\n",
        Bitboard::file2().toString());
  }

  {
    ASSERT_EQ(
        "000000100\n"
        "000000100\n"
        "000000100\n"
        "000000100\n"
        "000000100\n"
        "000000100\n"
        "000000100\n"
        "000000100\n"
        "000000100\n",
        Bitboard::file3().toString());
  }

  {
    ASSERT_EQ(
        "000001000\n"
        "000001000\n"
        "000001000\n"
        "000001000\n"
        "000001000\n"
        "000001000\n"
        "000001000\n"
        "000001000\n"
        "000001000\n",
        Bitboard::file4().toString());
  }

  {
    ASSERT_EQ(
        "000010000\n"
        "000010000\n"
        "000010000\n"
        "000010000\n"
        "000010000\n"
        "000010000\n"
        "000010000\n"
        "000010000\n"
        "000010000\n",
        Bitboard::file5().toString());
  }

  {
    ASSERT_EQ(
        "000100000\n"
        "000100000\n"
        "000100000\n"
        "000100000\n"
        "000100000\n"
        "000100000\n"
        "000100000\n"
        "000100000\n"
        "000100000\n",
        Bitboard::file6().toString());
  }

  {
    ASSERT_EQ(
        "001000000\n"
        "001000000\n"
        "001000000\n"
        "001000000\n"
        "001000000\n"
        "001000000\n"
        "001000000\n"
        "001000000\n"
        "001000000\n",
        Bitboard::file7().toString());
  }

  {
    ASSERT_EQ(
        "010000000\n"
        "010000000\n"
        "010000000\n"
        "010000000\n"
        "010000000\n"
        "010000000\n"
        "010000000\n"
        "010000000\n"
        "010000000\n",
        Bitboard::file8().toString());
  }

  {
    ASSERT_EQ(
        "100000000\n"
        "100000000\n"
        "100000000\n"
        "100000000\n"
        "100000000\n"
        "100000000\n"
        "100000000\n"
        "100000000\n"
        "100000000\n",
        Bitboard::file9().toString());
  }

  {
    ASSERT_EQ(
        "011111110\n"
        "111111111\n"
        "111111111\n"
        "111111111\n"
        "111111111\n"
        "111111111\n"
        "111111111\n"
        "111111111\n"
        "011111110\n",
        Bitboard::nocorner().toString());
  }
}

TEST(BitboardTest, testOperators) {
  const Bitboard bb1(0x3LL, 0xcLL); // 0011b, 1100b
  const Bitboard bb2(0x9LL, 0x6LL); // 1001b, 0110b
  const RotatedBitboard rbb1(0x3LL); // 0011b
  const RotatedBitboard rbb2(0x9LL); // 1001b

  {
    // assignment operator
    Bitboard bb;
    bb = bb1;
    ASSERT_EQ(0x3LL, bb.first());
    ASSERT_EQ(0xcLL, bb.second());

    RotatedBitboard rbb;
    rbb = rbb1;
    ASSERT_EQ(0x3LL, rbb.raw());
  }

  {
    // bitwise OR assignment operator
    Bitboard bb(bb1);
    bb |= bb2;
    ASSERT_EQ(0xbLL, bb.first());
    ASSERT_EQ(0xeLL, bb.second());

    RotatedBitboard rbb(rbb1);
    rbb |= rbb2;
    ASSERT_EQ(0xbLL, rbb.raw());
  }

  {
    // bitwise AND assignment operator
    Bitboard bb(bb1);
    bb &= bb2;
    ASSERT_EQ(0x1LL, bb.first());
    ASSERT_EQ(0x4LL, bb.second());

    RotatedBitboard rbb(rbb1);
    rbb &= rbb2;
    ASSERT_EQ(0x1LL, rbb.raw());
  }

  {
    // bitwise XOR assignment operator
    Bitboard bb(bb1);
    bb ^= bb2;
    ASSERT_EQ(0xaLL, bb.first());
    ASSERT_EQ(0xaLL, bb.second());

    RotatedBitboard rbb(rbb1);
    rbb ^= rbb2;
    ASSERT_EQ(0xaLL, rbb.raw());
  }

  {
    // bitwise OR operator
    Bitboard bb = bb1 | bb2;
    ASSERT_EQ(0xbLL, bb.first());
    ASSERT_EQ(0xeLL, bb.second());

    RotatedBitboard rbb = rbb1 | rbb2;
    ASSERT_EQ(0xbLL, rbb.raw());
  }

  {
    // bitwise AND operator
    Bitboard bb = bb1 & bb2;
    ASSERT_EQ(0x1LL, bb.first());
    ASSERT_EQ(0x4LL, bb.second());

    RotatedBitboard rbb = rbb1 & rbb2;
    ASSERT_EQ(0x1LL, rbb.raw());
  }

  {
    // bitwise XOR operator
    Bitboard bb = bb1 ^ bb2;
    ASSERT_EQ(0xaLL, bb.first());
    ASSERT_EQ(0xaLL, bb.second());

    RotatedBitboard rbb = rbb1 ^ rbb2;
    ASSERT_EQ(0xaLL, rbb.raw());
  }

  {
    // bitwise NOT operator
    Bitboard bb = ~bb1;
    ASSERT_EQ(0x00001ffffffffffcLL, bb.first());
    ASSERT_EQ(0x0000000ffffffff3LL, bb.second());

    RotatedBitboard rbb = ~rbb1;
    ASSERT_EQ(0xfffffffffffffffcLL, rbb.raw());
  }

  {
    // bitwise AND-NOT operation
    Bitboard bb = bb1.andNot(bb2);
    ASSERT_EQ(0x8LL, bb.first());
    ASSERT_EQ(0x2LL, bb.second());
  }

  {
    // left shift assignment operator
    Bitboard bb = bb1;
    bb <<= 3;
    ASSERT_EQ(0x18LL, bb.first());
    ASSERT_EQ(0x60LL, bb.second());

    RotatedBitboard rbb = rbb1;
    rbb <<= 3;
    ASSERT_EQ(0x18LL, rbb.raw());
  }

  {
    // right shift assignment operator
    Bitboard bb = bb1;
    bb >>= 1;
    ASSERT_EQ(0x1LL, bb.first());
    ASSERT_EQ(0x6LL, bb.second());

    RotatedBitboard rbb = rbb1;
    rbb >>= 1;
    ASSERT_EQ(0x1LL, rbb.raw());
  }

  {
    // left shift operator
    Bitboard bb = bb1 << 3;
    ASSERT_EQ(0x18LL, bb.first());
    ASSERT_EQ(0x60LL, bb.second());

    RotatedBitboard rbb = rbb1 << 3;
    ASSERT_EQ(0x18LL, rbb.raw());
  }

  {
    // right shift operator
    Bitboard bb = bb1 >> 1;
    ASSERT_EQ(0x1LL, bb.first());
    ASSERT_EQ(0x6LL, bb.second());

    RotatedBitboard rbb = rbb1 >> 1;
    ASSERT_EQ(0x1LL, rbb.raw());
  }
}

TEST(BitboardTest, testPerfectShift) {
  {
    Bitboard bb = Bitboard::zero();
    bb.set(Square::s19());
    bb.set(Square::s29());
    bb.set(Square::s39());
    bb.set(Square::s49());
    bb.set(Square::s59());
    bb.set(Square::s69());
    bb.set(Square::s79());
    bb.set(Square::s89());
    bb.set(Square::s99());
    bb.leftShift(1);
    ASSERT_EQ(
        "011111111\n"
        "000000000\n"
        "000000000\n"
        "000000000\n"
        "000000000\n"
        "000000000\n"
        "000000000\n"
        "000000000\n"
        "000000000\n",
        bb.toString());
  }

  {
    Bitboard bb = Bitboard::zero();
    bb.set(Square::s17());
    bb.set(Square::s26());
    bb.set(Square::s34());
    bb.set(Square::s49());
    bb.set(Square::s55());
    bb.set(Square::s61());
    bb.set(Square::s72());
    bb.set(Square::s89());
    bb.set(Square::s96());
    bb.leftShift(9);
    ASSERT_EQ(
        "000010000\n"
        "000100000\n"
        "000000000\n"
        "000000010\n"
        "000001000\n"
        "010000001\n"
        "000000000\n"
        "000000000\n"
        "001000100\n",
        bb.toString());
  }

  {
    Bitboard bb = Bitboard::zero();
    bb.set(Square::s17());
    bb.set(Square::s26());
    bb.set(Square::s34());
    bb.set(Square::s49());
    bb.set(Square::s55());
    bb.set(Square::s61());
    bb.set(Square::s72());
    bb.set(Square::s89());
    bb.set(Square::s96());
    bb.rightShift(9);
    ASSERT_EQ(
        "001000000\n"
        "010000000\n"
        "000000000\n"
        "000001000\n"
        "000100000\n"
        "000000100\n"
        "000000010\n"
        "000000000\n"
        "100010000\n",
        bb.toString());
  }
}

TEST(BitboardTest, testSet) {
  {
    Bitboard bb(0x3LL, 0xcLL);
    bb.set(Square(3));
    ASSERT_EQ(0xbLL, bb.first());
    ASSERT_EQ(0xcLL, bb.second());

    RotatedBitboard rbb(0x3LL);
    rbb.set(3);
    ASSERT_EQ(0xbLL, rbb.raw());
  }

  {
    Bitboard bb(0x3LL, 0xcLL);
    bb.set(Square(46));
    ASSERT_EQ(0x3LL, bb.first());
    ASSERT_EQ(0xeLL, bb.second());
  }

  {
    Bitboard bb(0x3LL, 0xcLL);
    bb.unset(Square(1));
    ASSERT_EQ(0x1LL, bb.first());
    ASSERT_EQ(0xcLL, bb.second());

    RotatedBitboard rbb(0x3LL);
    rbb.unset(1);
    ASSERT_EQ(0x1LL, rbb.raw());
  }

  {
    Bitboard bb(0x3LL, 0xcLL);
    bb.unset(Square(47));
    ASSERT_EQ(0x3LL, bb.first());
    ASSERT_EQ(0x8LL, bb.second());
  }

  {
    Bitboard bb(0x3LL, 0xcLL);
    ASSERT_EQ(true , bb.check(Square(0)));
    ASSERT_EQ(true , bb.check(Square(1)));
    ASSERT_EQ(false, bb.check(Square(2)));
    ASSERT_EQ(false, bb.check(Square(3)));
    ASSERT_EQ(false, bb.check(Square(4)));
    ASSERT_EQ(false, bb.check(Square(44)));
    ASSERT_EQ(false, bb.check(Square(45)));
    ASSERT_EQ(false, bb.check(Square(46)));
    ASSERT_EQ(true , bb.check(Square(47)));
    ASSERT_EQ(true , bb.check(Square(48)));

    RotatedBitboard rbb(0x3LL);
    ASSERT_EQ(true , rbb.check(0));
    ASSERT_EQ(true , rbb.check(1));
    ASSERT_EQ(false, rbb.check(2));
    ASSERT_EQ(false, rbb.check(3));
    ASSERT_EQ(false, rbb.check(4));
  }

  {
    ASSERT_EQ(0 , Bitboard(0x000LL, 0x000LL).count());
    ASSERT_EQ(9 , Bitboard(0x803LL, 0x74cLL).count());
    ASSERT_EQ(0 , RotatedBitboard(0x000LL).count());
    ASSERT_EQ(6 , RotatedBitboard(0x74cLL).count());
  }
}

TEST(BitboardTest, testCheckFile) {
  {
    Bitboard bb = Bitboard::zero();
    bb.set(Square::s19());
    bb.set(Square::s41());
    bb.set(Square::s59());
    bb.set(Square::s91());
    ASSERT_EQ(true ,  bb.checkFile(1));
    ASSERT_EQ(false, bb.checkFile(2));
    ASSERT_EQ(false, bb.checkFile(3));
    ASSERT_EQ(true ,  bb.checkFile(4));
    ASSERT_EQ(true ,  bb.checkFile(5));
    ASSERT_EQ(false, bb.checkFile(6));
    ASSERT_EQ(false, bb.checkFile(7));
    ASSERT_EQ(false, bb.checkFile(8));
    ASSERT_EQ(true ,  bb.checkFile(9));
  }

  {
    Bitboard bb = Bitboard::zero();
    bb.set(Square::s29());
    bb.set(Square::s49());
    bb.set(Square::s61());
    bb.set(Square::s89());
    ASSERT_EQ(false, bb.checkFile(1));
    ASSERT_EQ(true ,  bb.checkFile(2));
    ASSERT_EQ(false, bb.checkFile(3));
    ASSERT_EQ(true ,  bb.checkFile(4));
    ASSERT_EQ(false, bb.checkFile(5));
    ASSERT_EQ(true ,  bb.checkFile(6));
    ASSERT_EQ(false, bb.checkFile(7));
    ASSERT_EQ(true ,  bb.checkFile(8));
    ASSERT_EQ(false, bb.checkFile(9));
  }
}

TEST(BitboardTest, testToString) {
  {
    std::ostringstream oss;
    oss << Bitboard(0x3LL, 0xcLL);
    ASSERT_EQ(
        "100000000\n"
        "100000000\n"
        "000001000\n"
        "000001000\n"
        "000000000\n"
        "000000000\n"
        "000000000\n"
        "000000000\n"
        "000000000\n", oss.str());
  }

  {
    std::ostringstream oss;
    oss << RotatedBitboard(0x3LL);
    ASSERT_EQ("0000000000000003", oss.str());
  }
}
