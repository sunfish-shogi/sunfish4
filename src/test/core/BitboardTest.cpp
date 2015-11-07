/* BitboardTest.cpp
 *
 * Kubo Ryosuke
 */

#if !defined(NDEBUG)

#endif // !defined(NDEBUG)

#include "test/Test.hpp"
#include "core/position/Bitboard.hpp"

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

TEST(BitboardTest, testSet) {
  {
    Bitboard bb(0x3LL, 0xcLL);
    bb.set(3);
    ASSERT_EQ(0xbLL, bb.first());
    ASSERT_EQ(0xcLL, bb.second());

    RotatedBitboard rbb(0x3LL);
    rbb.set(3);
    ASSERT_EQ(0xbLL, rbb.raw());
  }

  {
    Bitboard bb(0x3LL, 0xcLL);
    bb.set(46);
    ASSERT_EQ(0x3LL, bb.first());
    ASSERT_EQ(0xeLL, bb.second());
  }

  {
    Bitboard bb(0x3LL, 0xcLL);
    bb.unset(1);
    ASSERT_EQ(0x1LL, bb.first());
    ASSERT_EQ(0xcLL, bb.second());

    RotatedBitboard rbb(0x3LL);
    rbb.unset(1);
    ASSERT_EQ(0x1LL, rbb.raw());
  }

  {
    Bitboard bb(0x3LL, 0xcLL);
    bb.unset(47);
    ASSERT_EQ(0x3LL, bb.first());
    ASSERT_EQ(0x8LL, bb.second());
  }

  {
    Bitboard bb(0x3LL, 0xcLL);
    ASSERT_EQ(true, bb.check(0));
    ASSERT_EQ(true, bb.check(1));
    ASSERT_EQ(false, bb.check(2));
    ASSERT_EQ(false, bb.check(3));
    ASSERT_EQ(false, bb.check(4));
    ASSERT_EQ(false, bb.check(44));
    ASSERT_EQ(false, bb.check(45));
    ASSERT_EQ(false, bb.check(46));
    ASSERT_EQ(true, bb.check(47));
    ASSERT_EQ(true, bb.check(48));

    RotatedBitboard rbb(0x3LL);
    ASSERT_EQ(true, rbb.check(0));
    ASSERT_EQ(true, rbb.check(1));
    ASSERT_EQ(false, rbb.check(2));
    ASSERT_EQ(false, rbb.check(3));
    ASSERT_EQ(false, rbb.check(4));
  }
}
