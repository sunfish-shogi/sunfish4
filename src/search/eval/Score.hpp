/* Score.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_SEARCH_EVAL_SCORE_HPP__
#define SUNFISH_SEARCH_EVAL_SCORE_HPP__

#include "common/Def.hpp"
#include "core/move/Move.hpp"
#include <iostream>
#include <cstdint>

namespace sunfish {

class Score {
public:

  using RawType = int16_t;

  static CONSTEXPR_CONST RawType Zero = 0;
  static CONSTEXPR_CONST RawType Infinity = 20000;
  static CONSTEXPR_CONST RawType Mate = 15000;
  static CONSTEXPR_CONST RawType MaterialInfinity = 5000;
  static CONSTEXPR_CONST RawType Invalid = 30000;

  static CONSTEXPR Score zero() {
    return Score(Zero);
  }

  static CONSTEXPR Score infinity() {
    return Score(Infinity);
  }

  static CONSTEXPR Score mate() {
    return Score(Mate);
  }

  static CONSTEXPR Score materialInfinity() {
    return Score(MaterialInfinity);
  }

  static CONSTEXPR Score invalid() {
    return Score(Invalid);
  }

  Score() {
  }

  CONSTEXPR Score(RawType score) : score_(score) {
  }

  Score& operator+=(const Score& s) {
    score_ += s.score_;
    return *this;
  }

  Score& operator-=(const Score& s) {
    score_ -= s.score_;
    return *this;
  }

  Score& operator*=(const Score& s) {
    score_ *= s.score_;
    return *this;
  }

  Score& operator/=(const Score& s) {
    score_ /= s.score_;
    return *this;
  }

  Score operator+() const {
    return Score(score_);
  }

  Score operator-() const {
    return Score(-score_);
  }

  Score operator+(const Score& s) const {
    return Score(score_ + s.score_);
  }

  Score operator-(const Score& s) const {
    return Score(score_ - s.score_);
  }

  Score operator*(const Score& s) const {
    return Score(score_ * s.score_);
  }

  Score operator/(const Score& s) const {
    return Score(score_ / s.score_);
  }

  bool operator==(const Score& s) const {
    return score_ == s.score_;
  }

  bool operator!=(const Score& s) const {
    return score_ != s.score_;
  }

  bool operator>(const Score& s) const {
    return score_ > s.score_;
  }

  bool operator<(const Score& s) const {
    return score_ < s.score_;
  }

  bool operator>=(const Score& s) const {
    return score_ >= s.score_;
  }

  bool operator<=(const Score& s) const {
    return score_ <= s.score_;
  }

  CONSTEXPR RawType raw() const {
    return score_;
  }

  friend std::ostream& operator<<(std::ostream& os, const sunfish::Score& score) {
    os << score.raw();
    return os;
  }

private:

  RawType score_;

};

inline void setScoreToMove(Move& move, const Score& score) {
  move.setExtData(static_cast<Move::RawType16>(score.raw()));
}

inline Score moveToScore(const Move& move) {
  return Score(static_cast<Score::RawType>(move.extData()));
}

} // namespace sunfish

#endif // SUNFISH_SEARCH_EVAL_SCORE_HPP__
