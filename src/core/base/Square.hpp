/* Square.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_CORE_BASE_SQUARE_HPP__
#define SUNFISH_CORE_BASE_SQUARE_HPP__

#include "core/Def.hpp"
#include <string>
#include <iostream>
#include <cstdint>
#include <cassert>

namespace sunfish {

enum {
  S91 = 0, S92, S93, S94, S95, S96, S97, S98, S99,
  S81, S82, S83, S84, S85, S86, S87, S88, S89,
  S71, S72, S73, S74, S75, S76, S77, S78, S79,
  S61, S62, S63, S64, S65, S66, S67, S68, S69,
  S51, S52, S53, S54, S55, S56, S57, S58, S59,
  S41, S42, S43, S44, S45, S46, S47, S48, S49,
  S31, S32, S33, S34, S35, S36, S37, S38, S39,
  S21, S22, S23, S24, S25, S26, S27, S28, S29,
  S11, S12, S13, S14, S15, S16, S17, S18, S19,
};

enum class Direction : int32_t {
  None,
  Up, Down, Left, Right,
  LeftUp, LeftDown,
  RightUp, RightDown,
  LeftUpKnight, LeftDownKnight,
  RightUpKnight, RightDownKnight,
};

inline Direction getReversedDir(Direction dir) {
  switch (dir) {
    case Direction::Up:              return Direction::Down;
    case Direction::Down:            return Direction::Up;
    case Direction::Left:            return Direction::Right;
    case Direction::Right:           return Direction::Left;
    case Direction::LeftUp:          return Direction::RightDown;
    case Direction::LeftDown:        return Direction::RightUp;
    case Direction::RightUp:         return Direction::LeftDown;
    case Direction::RightDown:       return Direction::LeftUp;
    case Direction::LeftUpKnight:    return Direction::RightDownKnight;
    case Direction::LeftDownKnight:  return Direction::RightUpKnight;
    case Direction::RightUpKnight:   return Direction::LeftDownKnight;
    case Direction::RightDownKnight: return Direction::LeftUpKnight;
    default: return Direction::None;
  }
}

inline std::ostream& operator<<(std::ostream& os, Direction dir) {
  switch (dir) {
    case Direction::Up:              os << "Up";              break;
    case Direction::Down:            os << "Down";            break;
    case Direction::Left:            os << "Left";            break;
    case Direction::Right:           os << "Right";           break;
    case Direction::LeftUp:          os << "LeftUp";          break;
    case Direction::LeftDown:        os << "LeftDown";        break;
    case Direction::RightUp:         os << "RightUp";         break;
    case Direction::RightDown:       os << "RightDown";       break;
    case Direction::LeftUpKnight:    os << "LeftUpKnight";    break;
    case Direction::LeftDownKnight:  os << "LeftDownKnight";  break;
    case Direction::RightUpKnight:   os << "RightUpKnight";   break;
    case Direction::RightDownKnight: os << "RightDownKnight"; break;
    default:                         os << "None";            break;
  }
  return os;
}

class Square {
private:

  int32_t index_;

public:

  static CONSTEXPR_CONST int32_t Invalid = -1;
  static CONSTEXPR_CONST int32_t N = 81;
  static CONSTEXPR_CONST int32_t RankN = 9;
  static CONSTEXPR_CONST int32_t FileN = 9;
  static CONSTEXPR_CONST int32_t Begin = 0;
  static CONSTEXPR_CONST int32_t End = Begin + N;

  static CONSTEXPR int32_t index(int file, int rank) {
    return (9 - file) * RankN + rank - 1;
  }

  CONSTEXPR Square() : index_(Invalid) {
  }
  CONSTEXPR Square(const Square&) = default;
  CONSTEXPR Square(Square&&) = default;
  CONSTEXPR Square(int32_t index) : index_(index) {
  }
  explicit CONSTEXPR Square(int32_t file, int32_t rank) :
    index_(index(file, rank)) {
  }

  Square& operator=(const Square&) = default;
  Square& operator=(Square&&) = default;

  CONSTEXPR bool operator==(const Square& sq) const {
    return index_ == sq.index_;
  }
  CONSTEXPR bool operator!=(const Square& sq) const {
    return index_ != sq.index_;
  }

  CONSTEXPR bool operator==(int32_t i) const {
    return index_ == i;
  }
  CONSTEXPR bool operator!=(int32_t i) const {
    return index_ != i;
  }

  explicit CONSTEXPR operator int32_t() const {
    return index_;
  }
  CONSTEXPR int32_t index() const {
    return index_;
  }

  Square& set(int32_t i) {
    index_ = i;
    return *this;
  }
  Square& set(int32_t file, int32_t rank) {
    index_ = index(file, rank);
    return *this;
  }

  CONSTEXPR int32_t getFile() const {
    return 9 - (index_ / RankN);
  }
  CONSTEXPR int32_t getRank() const {
    return index_ % RankN + 1;
  }

  CONSTEXPR bool isValid() const {
    return index_ != Invalid;
  }
  CONSTEXPR bool isInvalid() const {
    return !isValid();
  }
  CONSTEXPR bool isStrictValid() const {
    return index_ >= 0 && index_ < End;
  }
  static CONSTEXPR bool isValidFile(int32_t file) {
    return file >= 1 && file <= 9;
  }
  static CONSTEXPR bool isValidRank(int32_t rank) {
    return rank >= 1 && rank <= 9;
  }
  template<bool black>
  CONSTEXPR bool isPromotable() const {
    return black ? getRank() - 1 <= 2 : getRank() - 1 >= 6;
  }
  template<bool black>
  CONSTEXPR bool isPawnMovable() const {
    return black ? getRank() - 1 != 0 : getRank() - 1 != 8;
  }
  template<bool black>
  CONSTEXPR bool isLanceMovable() const {
    return isPawnMovable<black>();
  }
  template<bool black>
  CONSTEXPR bool isKnightMovable() const {
    return black ? getRank() - 1 >= 2 : getRank() - 1 <= 6;
  }
  CONSTEXPR Square reverse() const {
    return N - 1 - index_;
  }
  CONSTEXPR Square sym() const {
    return Square(FileN + 1 - getFile(), getRank());
  }
  CONSTEXPR Square up(int32_t distance = 1) const {
    return index_ - distance;
  }
  CONSTEXPR Square down(int32_t distance = 1) const {
    return index_ + distance;
  }
  CONSTEXPR Square left(int32_t distance = 1) const {
    return index_ - distance * RankN;
  }
  CONSTEXPR Square right(int32_t distance = 1) const {
    return index_ + distance * RankN;
  }
  CONSTEXPR Square leftUp(int32_t distance = 1) const {
    return (*this).left(distance).up(distance);
  }
  CONSTEXPR Square leftDown(int32_t distance = 1) const {
    return (*this).left(distance).down(distance);
  }
  CONSTEXPR Square rightUp(int32_t distance = 1) const {
    return (*this).right(distance).up(distance);
  }
  CONSTEXPR Square rightDown(int32_t distance = 1) const {
    return (*this).right(distance).down(distance);
  }
  CONSTEXPR Square leftUpKnight() const {
    return (*this).left().up(2);
  }
  CONSTEXPR Square leftDownKnight() const {
    return (*this).left().down(2);
  }
  CONSTEXPR Square rightUpKnight() const {
    return (*this).right().up(2);
  }
  CONSTEXPR Square rightDownKnight() const {
    return (*this).right().down(2);
  }
  CONSTEXPR Square safetyUp(int32_t distance = 1) const {
    return (!isInvalid() && getRank() - distance >= 1) ?
      up(distance) : Square(Invalid);
  }
  CONSTEXPR Square safetyDown(int32_t distance = 1) const {
    return (!isInvalid() && getRank() + distance <= 9) ?
      down(distance) : Square(Invalid);
  }
  CONSTEXPR Square safetyLeft(int32_t distance = 1) const {
    return (!isInvalid() && getFile() + distance <= 9) ?
      left(distance) : Square(Invalid);
  }
  CONSTEXPR Square safetyRight(int32_t distance = 1) const {
    return (!isInvalid() && getFile() - distance >= 1) ?
      right(distance) : Square(Invalid);
  }
  CONSTEXPR Square safetyLeftUp(int32_t distance = 1) const {
    return (*this).safetyLeft(distance).safetyUp(distance);
  }
  CONSTEXPR Square safetyLeftDown(int32_t distance = 1) const {
    return (*this).safetyLeft(distance).safetyDown(distance);
  }
  CONSTEXPR Square safetyRightUp(int32_t distance = 1) const {
    return (*this).safetyRight(distance).safetyUp(distance);
  }
  CONSTEXPR Square safetyRightDown(int32_t distance = 1) const {
    return (*this).safetyRight(distance).safetyDown(distance);
  }
  CONSTEXPR Square safetyLeftUpKnight() const {
    return (*this).safetyLeft().safetyUp(2);
  }
  CONSTEXPR Square safetyLeftDownKnight() const {
    return (*this).safetyLeft().safetyDown(2);
  }
  CONSTEXPR Square safetyRightUpKnight() const {
    return (*this).safetyRight().safetyUp(2);
  }
  CONSTEXPR Square safetyRightDownKnight() const {
    return (*this).safetyRight().safetyDown(2);
  }
  CONSTEXPR Square next() const {
    return Square(index_ + 1);
  }
  int32_t distance(const Square& to) const;
  Direction dir(const Square& to) const;

  std::string toString() const;
  static Square parse(const char* str);

};

} // namespace sunfish

#define SQUARE_EACH(sq)    for (sunfish::Square (sq) = sunfish::Square::Begin; (sq) != sunfish::Square::End; (sq) = (sq).next())

inline bool operator==(uint8_t index, const sunfish::Square& square) {
  return index == square.index();
}

inline bool operator!=(uint8_t index, const sunfish::Square& square) {
  return index != square.index();
}

inline std::ostream& operator<<(std::ostream& os, const sunfish::Square& square) {
  os << square.index();
  return os;
}

#endif // SUNFISH_CORE_BASE_SQUARE_HPP__
