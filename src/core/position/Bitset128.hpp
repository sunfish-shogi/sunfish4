/* Bitset128.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_CORE_BASE_BITSET128_HPP__
#define SUNFISH_CORE_BASE_BITSET128_HPP__

#include "core/Def.hpp"
#include <cstdint>

namespace sunfish {

/**
 * set of 128 bits
 */
template <class T, int W1, int W2>
class Bitset128 {
protected:

  static CONSTEXPR_CONST int Width1 = W1;
  static CONSTEXPR_CONST int Width2 = W2;
  static CONSTEXPR_CONST uint64_t Mask1 = (1ULL<<W1)-1;
  static CONSTEXPR_CONST uint64_t Mask2 = (1ULL<<W2)-1;

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

  u128 bb_;

  /**
   * get mutable reference of 1st quad word
   */
  uint64_t& firstRef() {
    return bb_.u64[0];
  }

  /**
   * get mutable reference of 2nd quad word
   */
  uint64_t& secondRef() {
    return bb_.u64[1];
  }

public:

  /**
   * default constructor
   */
  Bitset128() {
  }

  /**
   * copy constructor
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
   * constructor
   */
  Bitset128(const __m128i& m) : bb_(m) {}
#endif

  /**
   * constructor
   */
  explicit CONSTEXPR Bitset128(uint64_t first, uint64_t second) : bb_(first, second) {
  }

  /**
   * get zero-initilized object.
   */
  static CONSTEXPR T zero() {
    return T(0x00LL, 0x00LL);
  }

  /**
   * initialize all bits to zero
   */
  void init() {
#if USE_SSE2
    bb_.m = zero().bb_.m;
#else
    firstRef() = 0x00LL;
    secondRef() = 0x00LL;
#endif
  }

  /**
   * assignment operator
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
   * bitwise OR assignment operator
   */
  T& operator|=(const T& bb) {
#if USE_SSE2
    bb_.m = _mm_or_si128(bb_.m, bb.bb_.m);
#else
    firstRef() |= src.firstRef();
    secondRef() |= src.secondRef();
#endif
    return *(T*)this;
  }

  /**
   * bitwise AND assignment operator
   */
  T& operator&=(const T& bb) {
#if USE_SSE2
    bb_.m = _mm_and_si128(bb_.m, bb.bb_.m);
#else
    firstRef() &= src.firstRef();
    secondRef() &= src.secondRef();
#endif
    return *(T*)this;
  }

  /**
   * bitwise XOR assignment operator
   */
  T& operator^=(const T& bb) {
#if USE_SSE2
    bb_.m = _mm_xor_si128(bb_.m, bb.bb_.m);
#else
    firstRef() ^= src.firstRef();
    secondRef() ^= src.secondRef();
#endif
    return *(T*)this;
  }

  /**
   * bitwise OR operator
   */
  T operator|(const T& bb) const {
#if USE_SSE2
    return T(_mm_or_si128(bb_.m, bb.bb_.m));
#else
    return Bitboard(first() | bb.first(), second() | bb.second());
#endif
  }

  /**
   * bitwise AND operator
   */
  T operator&(const T& bb) const {
#if USE_SSE2
    return T(_mm_and_si128(bb_.m, bb.bb_.m));
#else
    return Bitboard(first() & bb.first(), second() & bb.second());
#endif
  }

  /**
   * bitwise XOR operator
   */
  T operator^(const T& bb) const {
#if USE_SSE2
    return T(_mm_xor_si128(bb_.m, bb.bb_.m));
#else
    return Bitboard(first() ^ bb.first(), second() ^ bb.second());
#endif
  }

  /**
   * bitwise NOT operator
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
    return Bitboard((~first()) & Mask1, (~second()) & Mask2);
#endif
  }

  /**
   * bitwise AND-NOT operation
   */
  T andNot(const T& bb) const{
#if USE_SSE2
    return T(_mm_andnot_si128(bb_.m, bb.bb_.m));
#else
    return Bitboard((~first()) & bb.first(), (~second()) & bb.second());
#endif
  }

  /**
   * shifts as the two unsgined 64-bit integers.
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
   * shifts as the two unsgined 64-bit integers.
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
   * left shift assignment operator.
   * This function uses <leftShift64> from the inside.
   */
  const T& operator<<=(int n) {
    leftShift64(n);
    return *(T*)this;
  }

  /**
   * right shift assignment operator.
   * This function uses <rightShift64> from the inside.
   */
  const T& operator>>=(int n) {
    rightShift64(n);
    return *(T*)this;
  }

  /**
   * left shift operator.
   * This function uses <leftShift64> from the inside.
   */
  T operator<<(int n) const {
    return T(*(T*)this) <<= n;
  }

  /**
   * right shift operator.
   * This function uses <rightShift64> from the inside.
   */
  T operator>>(int n) const {
    return T(*(T*)this) >>= n;
  }

  /**
   * get 1st quad word
   */
  uint64_t first() const {
    return bb_.u64[0];
  }

  /**
   * get 2nd quad word
   */
  uint64_t second() const {
    return bb_.u64[1];
  }

};

} // namespace sunfish

#endif // SUNFISH_CORE_BASE_BITSET128_HPP__
