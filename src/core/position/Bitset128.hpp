/* Bitset128.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_CORE_POSITION_BITSET128_HPP__
#define SUNFISH_CORE_POSITION_BITSET128_HPP__

#include "common/Def.hpp"
#include "common/bitope/BitOpe.hpp"
#include <cstdint>
#include <cassert>
#if defined(WIN32) && !defined(__MINGW32__)
#include <intrin.h> 
#endif

namespace sunfish {

template <class T, class U, U W1, U W2, U I>
class Bitset128 {
public:

  static_assert(W1 <= 64, "invalid bit width");
  static_assert(W2 <= 64, "invalid bit width");

  static CONSTEXPR_CONST U Width1 = W1;
  static CONSTEXPR_CONST U Width2 = W2;
  static CONSTEXPR_CONST U InvalidOffset = I;
  static CONSTEXPR_CONST uint64_t Mask1 = (1ULL<<W1)-1;
  static CONSTEXPR_CONST uint64_t Mask2 = (1ULL<<W2)-1;

public:

  /**
   * Default constructor
   */
  Bitset128() {
  }

  /**
   * Copy constructor
   */
  Bitset128(const T& src) :
    bb_(src.firstRef(), src.secondRef())
  {
  }

  /**
   * Constructor
   */
  explicit CONSTEXPR Bitset128(uint64_t first, uint64_t second) : bb_{first, second} {
  }

  /**
   * Get zero-initilized object.
   */
  static CONSTEXPR T zero() {
    return T(0x00LL, 0x00LL);
  }

  /**
   * Assignment operator
   */
  T& operator=(const T& src) {
    firstRef() = src.firstRef();
    secondRef() = src.secondRef();
  }

  /**
   * Bitwise OR assignment operator
   */
  T& operator|=(const T& rhs) {
    firstRef() |= rhs.first();
    secondRef() |= rhs.second();
    return *(static_cast<T*>(this));
  }

  /**
   * Bitwise AND assignment operator
   */
  T& operator&=(const T& rhs) {
    firstRef() &= rhs.first();
    secondRef() &= rhs.second();
    return *(static_cast<T*>(this));
  }

  /**
   * Bitwise XOR assignment operator
   */
  T& operator^=(const T& rhs) {
    firstRef() ^= rhs.first();
    secondRef() ^= rhs.second();
    return *(static_cast<T*>(this));
  }

  /**
   * Bitwise OR operator
   */
  T operator|(const T& rhs) const {
    return T(first() | rhs.first(), second() | rhs.second());
  }

  /**
   * Bitwise AND operator
   */
  T operator&(const T& rhs) const {
    return T(first() & rhs.first(), second() & rhs.second());
  }

  /**
   * Bitwise XOR operator
   */
  T operator^(const T& rhs) const {
    return T(first() ^ rhs.first(), second() ^ rhs.second());
  }

  /**
   * Bitwise NOT operator
   */
  T operator~() const{
    return T((~first()) & Mask1, (~second()) & Mask2);
  }

  /**
   * Bitwise AND-NOT operation
   */
  T andNot(const T& rhs) const{
    return T((~first()) & rhs.first(), (~second()) & rhs.second());
  }

#ifndef NDEBUG
  /**
   * EQUAL operator
   */
  bool operator==(const T& rhs) const {
    return first() == rhs.first() && second() == rhs.second();
  }

  /**
   * NOT-EQUAL operator
   */
  bool operator!=(const T& rhs) const {
    return !operator==(rhs);
  }
#endif

  /**
   * Get 1st quad word
   */
  uint64_t first() const {
    return bb_[0];
  }

  /**
   * Get 2nd quad word
   */
  uint64_t second() const {
    return bb_[1];
  }

  /**
   * find the first set bit.
   */
  U findForward() const {
    if (first()) {
      return findForward(first()) - 1;
    } else if (second()) {
      return findForward(second()) + (Width1 - 1);
    } else {
      return InvalidOffset;
    }
  }

  /**
   * Pick the first set bit.
   */
  U pickForward() {
    U offset;
    if (first()) {
      offset = findForward(first()) - 1;
      unset1(offset);
    } else if (second()) {
      offset = findForward(second()) - 1;
      unset2(offset);
      offset += Width1;
    } else {
      offset = InvalidOffset;
    }
    return offset;
  }

  /**
   * Returns the number of 1-bits.
   */
  int count() const {
    return popcount(first()) + popcount(second());
  }

protected:

  /**
   * Set the specified bit.
   */
  T& set(U offset) {
    if (offset < Width1) {
      firstRef() |= 1LLU << offset;
    } else {
      ASSERT(offset < Width1 + Width2);
      secondRef() |= 1LLU << (offset - Width1);
    }
    return *(static_cast<T*>(this));
  }

  /**
   * Unset the specified bit.
   */
  T& unset(U offset) {
    if (offset < Width1) {
      firstRef() &= ~(1LLU << offset);
    } else {
      ASSERT(offset < Width1 + Width2);
      secondRef() &= ~(1LLU << (offset - Width1));
    }
    return *(static_cast<T*>(this));
  }

  /**
   * Check the specified bit.
   */
  bool check(U offset) const {
    if (offset < Width1) {
      return first() & (1LLU << offset);
    } else {
      ASSERT(offset < Width1 + Width2);
      return second() & (1LLU << (offset - Width1));
    }
  }

  /**
   * Set the specified bit of the first quad ward.
   */
  T& set1(U offset) {
    ASSERT(offset < Width1);
    firstRef() |= 1LLU << offset;
    return *(static_cast<T*>(this));
  }

  /**
   * Unset the specified bit of the first quad ward.
   */
  T& unset1(U offset) {
    ASSERT(offset < Width1);
    firstRef() &= ~(1LLU << offset);
    return *(static_cast<T*>(this));
  }

  /**
   * Check the specified bit of the first quad ward.
   */
  bool check1(U offset) const {
    ASSERT(offset < Width1);
    return first() & (1LLU << offset);
  }

  /**
   * Set the specified bit of the first quad ward.
   */
  T& set2(U offset) {
    ASSERT(offset < Width2);
    secondRef() |= 1LLU << offset;
    return *(static_cast<T*>(this));
  }

  /**
   * Unset the specified bit of the first quad ward.
   */
  T& unset2(U offset) {
    ASSERT(offset < Width2);
    secondRef() &= ~(1LLU << offset);
    return *(static_cast<T*>(this));
  }

  /**
   * Check the specified bit of the first quad ward.
   */
  bool check2(U offset) const {
    ASSERT(offset < Width2);
    return second() & (1LLU << offset);
  }

  /**
   * Shifts as the two unsgined 64-bit integers.
   */
  T leftShift64(U n) const {
    T x(*(static_cast<const T*>(this)));
    x.firstRef() <<= n;
    x.secondRef() <<= n;
    return x;
  }

  /**
   * Shifts as the two unsgined 64-bit integers.
   */
  T rightShift64(U n) const {
    T x(*(static_cast<const T*>(this)));
    x.firstRef() >>= n;
    x.secondRef() >>= n;
    return x;
  }

  T leftShift(U n) const {
    T x;
    uint64_t f = first();
    uint64_t s = second();
    x.firstRef() = (f << n) & Mask1;
    x.secondRef() = ((s << n) | f >> (W1 - n)) & Mask2;
    return x;
  }

  T rightShift(U n) const {
    T x;
    uint64_t f = first();
    uint64_t s = second();
    x.firstRef() = ((f >> n) | (s << (W1 - n))) & Mask1;
    x.secondRef() = (s >> n) & Mask2;
    return x;
  }

  /**
   * Get mutable reference of 1st quad word
   */
  uint64_t& firstRef() {
    return bb_[0];
  }

  /**
   * Get mutable reference of 2nd quad word
   */
  uint64_t& secondRef() {
    return bb_[1];
  }

  static U findForward(uint64_t data) {
#if defined(WIN32) && !defined(__MINGW32__)
    unsigned long offset;
    return _BitScanForward64((unsigned long*)&offset, data) ? (offset + 1) : 0;
#elif defined(UNIX)
    return data == 0x00LL ? 0 : __builtin_ffsll(data);
#else
    static const int8_t bfirst_[256] = {
       0, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1, 5, 1, 2, 1,
       3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1, 6, 1, 2, 1, 3, 1, 2, 1,
       4, 1, 2, 1, 3, 1, 2, 1, 5, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1,
       3, 1, 2, 1, 7, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1,
       5, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1, 6, 1, 2, 1,
       3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1, 5, 1, 2, 1, 3, 1, 2, 1,
       4, 1, 2, 1, 3, 1, 2, 1, 8, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1,
       3, 1, 2, 1, 5, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1,
       6, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1, 5, 1, 2, 1,
       3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1, 7, 1, 2, 1, 3, 1, 2, 1,
       4, 1, 2, 1, 3, 1, 2, 1, 5, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1,
       3, 1, 2, 1, 6, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1,
       5, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1,
    };
    U offset;
    if (data == 0x00) { return 0; }
    if ((offset = bfirst_[ data     &0xff]) != 0) { return offset; }
    if ((offset = bfirst_[(data>> 8)&0xff]) != 0) { return offset +  8; }
    if ((offset = bfirst_[(data>>16)&0xff]) != 0) { return offset + 16; }
    if ((offset = bfirst_[(data>>24)&0xff]) != 0) { return offset + 24; }
    if ((offset = bfirst_[ data>>32 &0xff]) != 0) { return offset + 32; }
    if ((offset = bfirst_[(data>>40)&0xff]) != 0) { return offset + 40; }
    if ((offset = bfirst_[(data>>48)&0xff]) != 0) { return offset + 48; }
    return        bfirst_[(data>>56)     ] + 56;
#endif
  }

  uint64_t bb_[2];

};

} // namespace sunfish

#endif // SUNFISH_CORE_POSITION_BITSET128_HPP__
