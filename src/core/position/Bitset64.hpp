/* Bitset64.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_CORE_POSITION_BITSET64_HPP__
#define SUNFISH_CORE_POSITION_BITSET64_HPP__

#include "common/Def.hpp"
#include <cstdint>
#include <cassert>

namespace sunfish {

template <class T>
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
    return *(T*)this;
  }

  /**
   * Bitwise AND assignment operator
   */
  T& operator&=(const T& rhs) {
    bb_ &= rhs.bb_;
    return *(T*)this;
  }

  /**
   * Bitwise XOR assignment operator
   */
  T& operator^=(const T& rhs) {
    bb_ ^= rhs.bb_;
    return *(T*)this;
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
   * Bitwise AND-NOT operation
   */
  T andNot(const T& rhs) const{
    return T((~bb_) & rhs.bb_);
  }

  /**
   * Left shift assignment operator.
   */
  const T& operator<<=(int n) {
    bb_ <<= n;
    return *(T*)this;
  }

  /**
   * Right shift assignment operator.
   */
  const T& operator>>=(int n) {
    bb_ >>= n;
    return *(T*)this;
  }

  /**
   * Left shift operator.
   */
  T operator<<(int n) const {
    return T(*(T*)this) <<= n;
  }

  /**
   * Right shift operator.
   */
  T operator>>(int n) const {
    return T(*(T*)this) >>= n;
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
  void set(int offset) {
    assert(offset < 64);
    bb_ |= 1LLU << offset;
  }

  /**
   * Unset the specified bit.
   */
  void unset(int offset) {
    assert(offset < 64);
    bb_ &= ~(1LLU << offset);
  }

  /**
   * Check the specified bit.
   */
  bool check(int offset) const {
    assert(offset < 64);
    return bb_ & (1LLU << offset);
  }

  uint64_t bb_;

};

} // namespace sunfish

#endif // SUNFISH_CORE_POSITION_BITSET64_HPP__
