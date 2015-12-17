/* Bitset128.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_CORE_POSITION_BITSET128_HPP__
#define SUNFISH_CORE_POSITION_BITSET128_HPP__

#include "common/Def.hpp"
#include "core/Sse.hpp"
#include <cstdint>
#include <cassert>

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

protected:

  /**
   * 128-bit wide union type
   */
  union u128 {
    uint64_t u64[2];

    CONSTEXPR u128(uint64_t first, uint64_t second) : u64{first, second} {}

#if USE_SSE2
    __m128i m;
    CONSTEXPR u128(const __m128i& m) : m(m) {}
#endif

    u128() {}
  };
  static_assert(sizeof(u128) == 16, "invalid size");

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
#if USE_SSE2
    bb_(src.bb_.m)
#else
    bb_(src.firstRef(), src.secondRef())
#endif
  {
  }

#if USE_SSE2
  /**
   * Constructor
   */
  Bitset128(const __m128i& m) : bb_(m) {}
#endif

  /**
   * Constructor
   */
  explicit CONSTEXPR Bitset128(uint64_t first, uint64_t second) : bb_(first, second) {
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
#if USE_SSE2
    bb_.m = src.bb_.m;
#else
    firstRef() = src.firstRef();
    secondRef() = src.secondRef();
#endif
  }

  /**
   * Bitwise OR assignment operator
   */
  T& operator|=(const T& rhs) {
#if USE_SSE2
    bb_.m = _mm_or_si128(bb_.m, rhs.bb_.m);
#else
    firstRef() |= rhs.first();
    secondRef() |= rhs.second();
#endif
    return *(static_cast<T*>(this));
  }

  /**
   * Bitwise AND assignment operator
   */
  T& operator&=(const T& rhs) {
#if USE_SSE2
    bb_.m = _mm_and_si128(bb_.m, rhs.bb_.m);
#else
    firstRef() &= rhs.first();
    secondRef() &= rhs.second();
#endif
    return *(static_cast<T*>(this));
  }

  /**
   * Bitwise XOR assignment operator
   */
  T& operator^=(const T& rhs) {
#if USE_SSE2
    bb_.m = _mm_xor_si128(bb_.m, rhs.bb_.m);
#else
    firstRef() ^= rhs.first();
    secondRef() ^= rhs.second();
#endif
    return *(static_cast<T*>(this));
  }

  /**
   * Bitwise OR operator
   */
  T operator|(const T& rhs) const {
#if USE_SSE2
    return T(_mm_or_si128(bb_.m, rhs.bb_.m));
#else
    return T(first() | rhs.first(), second() | rhs.second());
#endif
  }

  /**
   * Bitwise AND operator
   */
  T operator&(const T& rhs) const {
#if USE_SSE2
    return T(_mm_and_si128(bb_.m, rhs.bb_.m));
#else
    return T(first() & rhs.first(), second() & rhs.second());
#endif
  }

  /**
   * Bitwise XOR operator
   */
  T operator^(const T& rhs) const {
#if USE_SSE2
    return T(_mm_xor_si128(bb_.m, rhs.bb_.m));
#else
    return T(first() ^ rhs.first(), second() ^ rhs.second());
#endif
  }

  /**
   * Bitwise NOT operator
   */
  T operator~() const{
#if USE_SSE2
    return T(_mm_andnot_si128(bb_.m,
#if BYTE_ORDER == BIG_ENDIAN
          _mm_set_epi64((__m64)Mask1, (__m64)Mask2)
#else
          _mm_set_epi64((__m64)Mask2, (__m64)Mask1)
#endif
          ));
#else
    return T((~first()) & Mask1, (~second()) & Mask2);
#endif
  }

  /**
   * Bitwise AND-NOT operation
   */
  T andNot(const T& rhs) const{
#if USE_SSE2
    return T(_mm_andnot_si128(bb_.m, rhs.bb_.m));
#else
    return T((~first()) & rhs.first(), (~second()) & rhs.second());
#endif
  }

  /**
   * Shifts as the two unsgined 64-bit integers.
   */
  void leftShift64(U n) {
#if USE_SSE2
    bb_.m = _mm_slli_epi64(bb_.m, n);
#else
    firstRef() <<= n;
    secondRef() <<= n;
#endif
  }

  /**
   * Shifts as the two unsgined 64-bit integers.
   */
  void rightShift64(U n) {
#if USE_SSE2
    bb_.m = _mm_srli_epi64(bb_.m, n);
#else
    firstRef() >>= n;
    secondRef() >>= n;
#endif
  }

  /**
   * Left shift assignment operator.
   * This function uses <leftShift64> from the inside.
   */
  const T& operator<<=(U n) {
    leftShift64(n);
    return *(static_cast<T*>(this));
  }

  /**
   * Right shift assignment operator.
   * This function uses <rightShift64> from the inside.
   */
  const T& operator>>=(U n) {
    rightShift64(n);
    return *(static_cast<T*>(this));
  }

  /**
   * Left shift operator.
   * This function uses <leftShift64> from the inside.
   */
  T operator<<(U n) const {
    return T(*(static_cast<const T*>(this))) <<= n;
  }

  /**
   * Right shift operator.
   * This function uses <rightShift64> from the inside.
   */
  T operator>>(U n) const {
    return T(*(static_cast<const T*>(this))) >>= n;
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
    return bb_.u64[0];
  }

  /**
   * Get 2nd quad word
   */
  uint64_t second() const {
    return bb_.u64[1];
  }

  /**
   * Pick the first set bit.
   */
  U pickFirst() {
    U offset;
    if (first()) {
      offset = getFirst(first()) - 1;
      unset1(offset);
    } else if (second()) {
      offset = getFirst(second()) - 1;
      unset2(offset);
      offset += Width1;
    } else {
      offset = InvalidOffset;
    }
    return offset;
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
   * Get mutable reference of 1st quad word
   */
  uint64_t& firstRef() {
    return bb_.u64[0];
  }

  /**
   * Get mutable reference of 2nd quad word
   */
  uint64_t& secondRef() {
    return bb_.u64[1];
  }

  static U getFirst(uint64_t data) {
#if defined(WIN32) && !defined(__MINGW32__)
    unsigned long offset;
    return _BitScanForward64((DWORD*)&offset, data) ? (offset + 1) : 0;
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

  u128 bb_;

};

} // namespace sunfish

#endif // SUNFISH_CORE_POSITION_BITSET128_HPP__
