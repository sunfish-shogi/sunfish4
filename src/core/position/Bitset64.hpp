/* Bitset64.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_CORE_POSITION_BITSET64_HPP__
#define SUNFISH_CORE_POSITION_BITSET64_HPP__

#include "common/Def.hpp"
#include "common/bitope/BitOpe.hpp"
#include <cstdint>
#include <cassert>

namespace sunfish {

template <class T, class U>
class Bitset64 {
public:

  /**
   * Default constructor
   */
  Bitset64() {
  }

  /**
   * Copy constructor
   */
  Bitset64(const T& src) : bb_(src.bb_) {}

  /**
   * Constructor
   */
  explicit CONSTEXPR Bitset64(uint64_t bitset) : bb_(bitset) {
  }

  /**
   * Get zero-initilized object.
   */
  static CONSTEXPR T zero() {
    return T(0x00LL);
  }

  /**
   * Assignment operator
   */
  T& operator=(const T& src) {
    bb_ = src.bb_;
  }

  /**
   * Bitwise OR assignment operator
   */
  T& operator|=(const T& rhs) {
    bb_ |= rhs.bb_;
    return *(static_cast<T*>(this));
  }

  /**
   * Bitwise AND assignment operator
   */
  T& operator&=(const T& rhs) {
    bb_ &= rhs.bb_;
    return *(static_cast<T*>(this));
  }

  /**
   * Bitwise XOR assignment operator
   */
  T& operator^=(const T& rhs) {
    bb_ ^= rhs.bb_;
    return *(static_cast<T*>(this));
  }

  /**
   * Bitwise OR operator
   */
  T operator|(const T& rhs) const {
    return T(bb_ | rhs.bb_);
  }

  /**
   * Bitwise AND operator
   */
  T operator&(const T& rhs) const {
    return T(bb_ & rhs.bb_);
  }

  /**
   * Bitwise XOR operator
   */
  T operator^(const T& rhs) const {
    return T(bb_ ^ rhs.bb_);
  }

  /**
   * Bitwise NOT operator
   */
  T operator~() const{
    return T(~bb_);
  }

  /**
   * Left shift assignment operator.
   */
  const T& operator<<=(U n) {
    bb_ <<= n;
    return *(static_cast<T*>(this));
  }

  /**
   * Right shift assignment operator.
   */
  const T& operator>>=(U n) {
    bb_ >>= n;
    return *(static_cast<T*>(this));
  }

  /**
   * Left shift operator.
   */
  T operator<<(U n) const {
    return T(*(static_cast<const T*>(this))) <<= n;
  }

  /**
   * Right shift operator.
   */
  T operator>>(U n) const {
    return T(*(static_cast<const T*>(this))) >>= n;
  }

#ifndef NDEBUG
  /**
   * EQUAL operator
   */
  bool operator==(const T& rhs) const {
    return bb_ == rhs.bb_;
  }

  /**
   * NOT-EQUAL operator
   */
  bool operator!=(const T& rhs) const {
    return !operator==(rhs);
  }
#endif

  /**
   * Returns the number of 1-bits.
   */
  int count() const {
    return popcount(bb_);
  }

  /**
   * Get the raw value.
   */
  uint64_t raw() const {
    return bb_;
  }

protected:

  /**
   * Set the specified bit.
   */
  T& set(U offset) {
    ASSERT(offset < 64);
    bb_ |= 1LLU << offset;
    return *(static_cast<T*>(this));
  }

  /**
   * Unset the specified bit.
   */
  T& unset(U offset) {
    ASSERT(offset < 64);
    bb_ &= ~(1LLU << offset);
    return *(static_cast<T*>(this));
  }

  /**
   * Check the specified bit.
   */
  bool check(U offset) const {
    ASSERT(offset < 64);
    return bb_ & (1LLU << offset);
  }

  uint64_t bb_;

};

} // namespace sunfish

#endif // SUNFISH_CORE_POSITION_BITSET64_HPP__
