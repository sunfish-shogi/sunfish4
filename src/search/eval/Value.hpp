/* Value.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_SEARCH_EVAL_VALUE_HPP__
#define SUNFISH_SEARCH_EVAL_VALUE_HPP__

#include "common/Def.hpp"
#include <iostream>
#include <cstdint>

namespace sunfish {

class Value {
public:

  using RawType = int16_t;

  static CONSTEXPR_CONST RawType Zero = 0;
  static CONSTEXPR_CONST RawType Infinity = 20000;
  static CONSTEXPR_CONST RawType Mate = 15000;
  static CONSTEXPR_CONST RawType MaterialInfinity = 5000;

  static CONSTEXPR Value zero() {
    return Value(Zero);
  }

  static CONSTEXPR Value infinity() {
    return Value(Infinity);
  }

  static CONSTEXPR Value mate() {
    return Value(Mate);
  }

  static CONSTEXPR Value materialInfinity() {
    return Value(MaterialInfinity);
  }

  Value() {
  }

  CONSTEXPR Value(RawType value) : value_(value) {
  }

  Value& operator+=(const Value& val) {
    value_ += val.value_;
    return *this;
  }

  Value& operator-=(const Value& val) {
    value_ -= val.value_;
    return *this;
  }

  Value& operator*=(const Value& val) {
    value_ *= val.value_;
    return *this;
  }

  Value& operator/=(const Value& val) {
    value_ /= val.value_;
    return *this;
  }

  Value operator+() const {
    return Value(value_);
  }

  Value operator-() const {
    return Value(-value_);
  }

  Value operator+(const Value& val) const {
    return Value(value_ + val.value_);
  }

  Value operator-(const Value& val) const {
    return Value(value_ - val.value_);
  }

  Value operator*(const Value& val) const {
    return Value(value_ * val.value_);
  }

  Value operator/(const Value& val) const {
    return Value(value_ / val.value_);
  }

  bool operator==(const Value& val) const {
    return value_ == val.value_;
  }

  bool operator!=(const Value& val) const {
    return value_ != val.value_;
  }

  bool operator>(const Value& val) const {
    return value_ > val.value_;
  }

  bool operator<(const Value& val) const {
    return value_ < val.value_;
  }

  bool operator>=(const Value& val) const {
    return value_ >= val.value_;
  }

  bool operator<=(const Value& val) const {
    return value_ <= val.value_;
  }

  explicit CONSTEXPR operator RawType() const {
    return value_;
  }

  CONSTEXPR RawType raw() const {
    return value_;
  }

  friend std::ostream& operator<<(std::ostream& os, const sunfish::Value& value) {
    os << value.raw();
    return os;
  }

private:

  RawType value_;

};

} // namespace sunfish

#endif // SUNFISH_SEARCH_EVAL_VALUE_HPP__
