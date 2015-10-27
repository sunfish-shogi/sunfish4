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

#define SQUARE_FILES 9
#define SQUARE_RANKS 9
#define NUMBER_OF_SQUARES (SQUARE_FILES*SQUARE_RANKS)

#define SQUARE_FILE_MAX 9
#define SQUARE_RANK_MAX 9

namespace sunfish {

/**
 * Numbers corresponding to each of the squares.
 */
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

/**
 * Get reversed direction of specified that.
 */
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

/**
 * Output stream operator for Direction
 */
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
public:

  static CONSTEXPR_CONST int32_t Invalid = -1;
  static CONSTEXPR_CONST int32_t N = NUMBER_OF_SQUARES;
  static CONSTEXPR_CONST int32_t FileMax = SQUARE_FILE_MAX;
  static CONSTEXPR_CONST int32_t RankMax = SQUARE_RANK_MAX;
  static CONSTEXPR_CONST int32_t Begin = 0;
  static CONSTEXPR_CONST int32_t End = Begin + N;

  /**
   * Get square number from the file number and the rank number.
   */
  static CONSTEXPR int32_t number(int file, int rank) {
    return (9 - file) * RankMax + rank - 1;
  }

  /**
   * Default constructor
   */
  CONSTEXPR Square() : number_(Invalid) {
  }

  /**
   * Copy constructor
   */
  CONSTEXPR Square(const Square&) = default;

  /**
   * Move constructor
   */
  CONSTEXPR Square(Square&&) = default;

  /**
   * Constructor which takes the square number as an argument.
   */
  CONSTEXPR Square(int32_t number) : number_(number) {
  }

  /**
   * Constructor which takes the file number and the rank number as arguments.
   */
  CONSTEXPR Square(int32_t file, int32_t rank) :
    number_(number(file, rank)) {
  }

  /**
   * Assignment operator
   */
  Square& operator=(const Square&) = default;

  /**
   * Move assignment operator
   */
  Square& operator=(Square&&) = default;

  /**
   * EQUALS operator
   */
  CONSTEXPR bool operator==(const Square& sq) const {
    return number_ == sq.number_;
  }

  /**
   * NOT-EQUALS operator
   */
  CONSTEXPR bool operator!=(const Square& sq) const {
    return number_ != sq.number_;
  }

  /**
   * EQUALS operator which takes an integer value as an argument.
   */
  CONSTEXPR bool operator==(int32_t i) const {
    return number_ == i;
  }

  /**
   * NOT-EQUALS operator which takes an integer value as an argument.
   */
  CONSTEXPR bool operator!=(int32_t i) const {
    return number_ != i;
  }

  /**
   * Cast operator for an integer value.
   */
  explicit CONSTEXPR operator int32_t() const {
    return number_;
  }

  /**
   * Get square number as an integer.
   */
  CONSTEXPR int32_t raw() const {
    return number_;
  }

  /**
   * Set value with a square number.
   */
  Square& set(int32_t i) {
    number_ = i;
    return *this;
  }

  /**
   * Set value with a file number and a rank number.
   */
  Square& set(int32_t file, int32_t rank) {
    number_ = number(file, rank);
    return *this;
  }

  /**
   * Get a file number.
   */
  CONSTEXPR int32_t getFile() const {
    return 9 - (number_ / RankMax);
  }

  /**
   * Get a rank number.
   */
  CONSTEXPR int32_t getRank() const {
    return number_ % RankMax + 1;
  }

  /**
   * Check if the square number is valid.
   */
  CONSTEXPR bool isValid() const {
    return number_ != Invalid;
  }

  /**
   * Check if the square number is NOT valid.
   */
  CONSTEXPR bool isInvalid() const {
    return !isValid();
  }

  /**
   * Check if the square number is in valid range.
   */
  CONSTEXPR bool isStrictValid() const {
    return number_ >= 0 && number_ < End;
  }

  /**
   * Check if the file number is in valid range.
   */
  static CONSTEXPR bool isValidFile(int32_t file) {
    return file >= 1 && file <= 9;
  }

  /**
   * Check if the rank number is in valid range.
   */
  static CONSTEXPR bool isValidRank(int32_t rank) {
    return rank >= 1 && rank <= 9;
  }

  /**
   * Check if the square is promotable range.
   */
  template<bool black>
  CONSTEXPR bool isPromotable() const {
    return black ? getRank() - 1 <= 2 : getRank() - 1 >= 6;
  }

  /**
   * Check if the square is movable range for Pawn.
   */
  template<bool black>
  CONSTEXPR bool isPawnMovable() const {
    return black ? getRank() - 1 != 0 : getRank() - 1 != 8;
  }

  /**
   * Check if the square is movable range for Lance.
   */
  template<bool black>
  CONSTEXPR bool isLanceMovable() const {
    return isPawnMovable<black>();
  }

  /**
   * Check if the square is movable range for Knight.
   */
  template<bool black>
  CONSTEXPR bool isKnightMovable() const {
    return black ? getRank() - 1 >= 2 : getRank() - 1 <= 6;
  }

  /**
   * Get a symmetrical square diagonally.
   */
  CONSTEXPR Square dsym() const {
    return N - 1 - number_;
  }

  /**
   * Get a symmetrical square in the horizontal direction.
   */
  CONSTEXPR Square hsym() const {
    return Square(FileMax + 1 - getFile(), getRank());
  }

  /**
   * Get a symmetrical square in the vertical direction.
   */
  CONSTEXPR Square vsym() const {
    return Square(getFile(), RankMax + 1 - getRank());
  }

  /**
   * Get a square located over of the current square.
   */
  CONSTEXPR Square up(int32_t distance = 1) const {
    return number_ - distance;
  }

  /**
   * Get a square located under of the current square.
   */
  CONSTEXPR Square down(int32_t distance = 1) const {
    return number_ + distance;
  }

  /**
   * Get a square located to the left of the current square.
   */
  CONSTEXPR Square left(int32_t distance = 1) const {
    return number_ - distance * RankMax;
  }

  /**
   * Get a square located to the right of the current square.
   */
  CONSTEXPR Square right(int32_t distance = 1) const {
    return number_ + distance * RankMax;
  }

  /**
   * Get a square equals to "square.left(d).up(d)"
   */
  CONSTEXPR Square leftUp(int32_t distance = 1) const {
    return (*this).left(distance).up(distance);
  }

  /**
   * Get a square equals to "square.left(d).down(d)"
   */
  CONSTEXPR Square leftDown(int32_t distance = 1) const {
    return (*this).left(distance).down(distance);
  }

  /**
   * Get a square equals to "square.right(d).up(d)"
   */
  CONSTEXPR Square rightUp(int32_t distance = 1) const {
    return (*this).right(distance).up(distance);
  }

  /**
   * Get a square equals to "square.right(d).down(d)"
   */
  CONSTEXPR Square rightDown(int32_t distance = 1) const {
    return (*this).right(distance).down(distance);
  }

  /**
   * Get a left side square of squares that black-Knight can move.
   */
  CONSTEXPR Square leftUpKnight() const {
    return (*this).left().up(2);
  }

  /**
   * Get a right side square of squares that black-Knight can move.
   */
  CONSTEXPR Square rightUpKnight() const {
    return (*this).right().up(2);
  }

  /**
   * Get a left side square of squares that white-Knight can move.
   */
  CONSTEXPR Square leftDownKnight() const {
    return (*this).left().down(2);
  }

  /**
   * Get a right side square of squares that white-Knight can move.
   */
  CONSTEXPR Square rightDownKnight() const {
    return (*this).right().down(2);
  }

  /**
   * Get a square located over of the current square.
   * If the result is out of range, return Invalid.
   */
  CONSTEXPR Square safetyUp(int32_t distance = 1) const {
    return (!isInvalid() && getRank() - distance >= 1) ?
      up(distance) : Square(Invalid);
  }

  /**
   * Get a square located under of the current square.
   * If the result is out of range, return Invalid.
   */
  CONSTEXPR Square safetyDown(int32_t distance = 1) const {
    return (!isInvalid() && getRank() + distance <= 9) ?
      down(distance) : Square(Invalid);
  }

  /**
   * Get a square located to the left of the current square.
   * If the result is out of range, return Invalid.
   */
  CONSTEXPR Square safetyLeft(int32_t distance = 1) const {
    return (!isInvalid() && getFile() + distance <= 9) ?
      left(distance) : Square(Invalid);
  }

  /**
   * Get a square located to the right of the current square.
   * If the result is out of range, return Invalid.
   */
  CONSTEXPR Square safetyRight(int32_t distance = 1) const {
    return (!isInvalid() && getFile() - distance >= 1) ?
      right(distance) : Square(Invalid);
  }

  /**
   * Get a square equals to "square.left(d).up(d)"
   * If the result is out of range, return Invalid.
   */
  CONSTEXPR Square safetyLeftUp(int32_t distance = 1) const {
    return (*this).safetyLeft(distance).safetyUp(distance);
  }

  /**
   * Get a square equals to "square.left(d).down(d)"
   * If the result is out of range, return Invalid.
   */
  CONSTEXPR Square safetyLeftDown(int32_t distance = 1) const {
    return (*this).safetyLeft(distance).safetyDown(distance);
  }

  /**
   * Get a square equals to "square.right(d).up(d)"
   * If the result is out of range, return Invalid.
   */
  CONSTEXPR Square safetyRightUp(int32_t distance = 1) const {
    return (*this).safetyRight(distance).safetyUp(distance);
  }

  /**
   * Get a square equals to "square.right(d).down(d)"
   * If the result is out of range, return Invalid.
   */
  CONSTEXPR Square safetyRightDown(int32_t distance = 1) const {
    return (*this).safetyRight(distance).safetyDown(distance);
  }

  /**
   * Get a left side square of squares that black-Knight can move.
   * If the result is out of range, return Invalid.
   */
  CONSTEXPR Square safetyLeftUpKnight() const {
    return (*this).safetyLeft().safetyUp(2);
  }

  /**
   * Get a right side square of squares that black-Knight can move.
   * If the result is out of range, return Invalid.
   */
  CONSTEXPR Square safetyLeftDownKnight() const {
    return (*this).safetyLeft().safetyDown(2);
  }

  /**
   * Get a left side square of squares that white-Knight can move.
   * If the result is out of range, return Invalid.
   */
  CONSTEXPR Square safetyRightUpKnight() const {
    return (*this).safetyRight().safetyUp(2);
  }

  /**
   * Get a right side square of squares that white-Knight can move.
   * If the result is out of range, return Invalid.
   */
  CONSTEXPR Square safetyRightDownKnight() const {
    return (*this).safetyRight().safetyDown(2);
  }

  /**
   * Get squares in numerical order.
   * See SQUARE_EACH macro.
   */
  CONSTEXPR Square next() const {
    return Square(number_ + 1);
  }

  /**
   * Get a distance from the current square to the specified square.
   */
  int32_t distance(const Square& to) const;

  /**
   * Get a direction from the current square to the specified square.
   */
  Direction dir(const Square& to) const;

  /**
   * Get a square as a string type.
   */
  std::string toString() const;

  /**
   * Get square object from the specified string.
   */
  static Square parse(const char* str);

private:

  int32_t number_;

};

} // namespace sunfish

#define SQUARE_EACH(sq)    for (sunfish::Square (sq) = sunfish::Square::Begin; (sq) != sunfish::Square::End; (sq) = (sq).next())

inline bool operator==(uint8_t number, const sunfish::Square& square) {
  return number == square.raw();
}

inline bool operator!=(uint8_t number, const sunfish::Square& square) {
  return number != square.raw();
}

inline std::ostream& operator<<(std::ostream& os, const sunfish::Square& square) {
  os << square.raw();
  return os;
}

#endif // SUNFISH_CORE_BASE_SQUARE_HPP__
