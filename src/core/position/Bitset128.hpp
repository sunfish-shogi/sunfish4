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

template <class T, int W1, int W2>
class Bitset128 {
public:

  static_assert(W1 <= 64, "invalid bit width");
  static_assert(W2 <= 64, "invalid bit width");

  static CONSTEXPR_CONST int Width1 = W1;
  static CONSTEXPR_CONST int Width2 = W2;
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
    return *(T*)this;
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
    return *(T*)this;
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
    return *(T*)this;
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
  void leftShift64(int n) {
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
  void rightShift64(int n) {
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
  const T& operator<<=(int n) {
    leftShift64(n);
    return *(T*)this;
  }

  /**
   * Right shift assignment operator.
   * This function uses <rightShift64> from the inside.
   */
  const T& operator>>=(int n) {
    rightShift64(n);
    return *(T*)this;
  }

  /**
   * Left shift operator.
   * This function uses <leftShift64> from the inside.
   */
  T operator<<(int n) const {
    return T(*(T*)this) <<= n;
  }

  /**
   * Right shift operator.
   * This function uses <rightShift64> from the inside.
   */
  T operator>>(int n) const {
    return T(*(T*)this) >>= n;
  }

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

protected:

  /**
   * Set the specified bit.
   */
  void set(int offset) {
    if (offset < Width1) {
      firstRef() |= 1LLU << offset;
    } else {
      assert(offset < Width1 + Width2);
      secondRef() |= 1LLU << (offset - Width1);
    }
  }

  /**
   * Unset the specified bit.
   */
  void unset(int offset) {
    if (offset < Width1) {
      firstRef() &= ~(1LLU << offset);
    } else {
      assert(offset < Width1 + Width2);
      secondRef() &= ~(1LLU << (offset - Width1));
    }
  }

  /**
   * Check the specified bit.
   */
  bool check(int offset) const {
    if (offset < Width1) {
      return first() & (1LLU << offset);
    } else {
      assert(offset < Width1 + Width2);
      return second() & (1LLU << (offset - Width1));
    }
  }

  /**
   * Set the specified bit of the first quad ward.
   */
  void set1(int offset) {
    assert(offset < Width1);
    firstRef() |= 1LLU << offset;
  }

  /**
   * Unset the specified bit of the first quad ward.
   */
  void unset1(int offset) {
    assert(offset < Width1);
    firstRef() &= ~(1LLU << offset);
  }

  /**
   * Check the specified bit of the first quad ward.
   */
  bool check1(int offset) const {
    assert(offset < Width1);
    return first() & (1LLU << offset);
  }

  /**
   * Set the specified bit of the first quad ward.
   */
  void set2(int offset) {
    assert(offset < Width2);
    secondRef() |= 1LLU << offset;
  }

  /**
   * Unset the specified bit of the first quad ward.
   */
  void unset2(int offset) {
    assert(offset < Width2);
    secondRef() &= ~(1LLU << offset);
  }

  /**
   * Check the specified bit of the first quad ward.
   */
  bool check2(int offset) const {
    assert(offset < Width2);
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

  u128 bb_;

};

} // namespace sunfish

#endif // SUNFISH_CORE_POSITION_BITSET128_HPP__
