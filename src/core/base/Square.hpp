/* Square.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_CORE_BASE_SQUARE_HPP__
#define SUNFISH_CORE_BASE_SQUARE_HPP__

#include "common/Def.hpp"
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

class RotatedSquare {
public:

  using RawType = int32_t;

  static CONSTEXPR_CONST RawType Invalid = -1;

  /**
   * Default constructor
   */
  RotatedSquare() {
  }

  /**
   * Copy constructor
   */
  CONSTEXPR RotatedSquare(const RotatedSquare&) = default;

  /**
   * Move constructor
   */
  CONSTEXPR RotatedSquare(RotatedSquare&&) = default;

  /**
   * Constructor which takes the square number as an argument.
   */
  CONSTEXPR RotatedSquare(RawType number) : number_(number) {
  }

  /**
   * Assignment operator
   */
  RotatedSquare& operator=(const RotatedSquare&) = default;

  /**
   * Move assignment operator
   */
  RotatedSquare& operator=(RotatedSquare&&) = default;

  /**
   * EQUALS operator
   */
  CONSTEXPR bool operator==(const RotatedSquare& rhs) const {
    return number_ == rhs.number_;
  }

  /**
   * NOT-EQUALS operator
   */
  CONSTEXPR bool operator!=(const RotatedSquare& rhs) const {
    return number_ != rhs.number_;
  }

  /**
   * EQUALS operator which takes an integer value as an argument.
   */
  CONSTEXPR bool operator==(RawType rhs) const {
    return number_ == rhs;
  }

  /**
   * NOT-EQUALS operator which takes an integer value as an argument.
   */
  CONSTEXPR bool operator!=(RawType rhs) const {
    return number_ != rhs;
  }

  /**
   * Cast operator for an integer value.
   */
  explicit CONSTEXPR operator RawType() const {
    return number_;
  }

  /**
   * Get square number as an integer.
   */
  CONSTEXPR RawType raw() const {
    return number_;
  }

private:

  RawType number_;

};

class Square {
public:

  using RawType = int32_t;

  /**
   * Numbers corresponding to each of the squares.
   */
  enum {
    S91 = 0,
         S92, S93, S94, S95, S96, S97, S98, S99,
    S81, S82, S83, S84, S85, S86, S87, S88, S89,
    S71, S72, S73, S74, S75, S76, S77, S78, S79,
    S61, S62, S63, S64, S65, S66, S67, S68, S69,
    S51, S52, S53, S54, S55, S56, S57, S58, S59,
    S41, S42, S43, S44, S45, S46, S47, S48, S49,
    S31, S32, S33, S34, S35, S36, S37, S38, S39,
    S21, S22, S23, S24, S25, S26, S27, S28, S29,
    S11, S12, S13, S14, S15, S16, S17, S18, S19,
  };

  static CONSTEXPR_CONST RawType Invalid = -1;
  static CONSTEXPR_CONST RawType N = NUMBER_OF_SQUARES;
  static CONSTEXPR_CONST RawType FileMax = SQUARE_FILE_MAX;
  static CONSTEXPR_CONST RawType RankMax = SQUARE_RANK_MAX;
  static CONSTEXPR_CONST RawType Begin = 0;
  static CONSTEXPR_CONST RawType End = Begin + N;

  /**
   * Default constructor
   */
  Square() {
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
  CONSTEXPR Square(RawType number) : number_(number) {
  }

  /**
   * Constructor which takes the file number and the rank number as arguments.
   */
  CONSTEXPR Square(RawType file, RawType rank) :
    number_((9 - file) * RankMax + rank - 1) {
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
  CONSTEXPR bool operator==(const Square& rhs) const {
    return number_ == rhs.number_;
  }

  /**
   * NOT-EQUALS operator
   */
  CONSTEXPR bool operator!=(const Square& rhs) const {
    return number_ != rhs.number_;
  }

  /**
   * EQUALS operator which takes an integer value as an argument.
   */
  CONSTEXPR bool operator==(RawType rhs) const {
    return number_ == rhs;
  }

  /**
   * NOT-EQUALS operator which takes an integer value as an argument.
   */
  CONSTEXPR bool operator!=(RawType rhs) const {
    return number_ != rhs;
  }

  /**
   * Cast operator for an integer value.
   */
  explicit CONSTEXPR operator RawType() const {
    return number_;
  }

  /**
   * Get square number as an integer.
   */
  CONSTEXPR RawType raw() const {
    return number_;
  }

  /**
   * Get a file number.
   */
  CONSTEXPR RawType getFile() const {
    return 9 - (number_ / RankMax);
  }

  /**
   * Get a rank number.
   */
  CONSTEXPR RawType getRank() const {
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
  static CONSTEXPR bool isValidFile(RawType file) {
    return file >= 1 && file <= 9;
  }

  /**
   * Check if the rank number is in valid range.
   */
  static CONSTEXPR bool isValidRank(RawType rank) {
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
  CONSTEXPR Square up(RawType distance = 1) const {
    return number_ - distance;
  }

  /**
   * Get a square located under of the current square.
   */
  CONSTEXPR Square down(RawType distance = 1) const {
    return number_ + distance;
  }

  /**
   * Get a square located to the left of the current square.
   */
  CONSTEXPR Square left(RawType distance = 1) const {
    return number_ - distance * RankMax;
  }

  /**
   * Get a square located to the right of the current square.
   */
  CONSTEXPR Square right(RawType distance = 1) const {
    return number_ + distance * RankMax;
  }

  /**
   * Get a square equals to "square.left(d).up(d)"
   */
  CONSTEXPR Square leftUp(RawType distance = 1) const {
    return (*this).left(distance).up(distance);
  }

  /**
   * Get a square equals to "square.left(d).down(d)"
   */
  CONSTEXPR Square leftDown(RawType distance = 1) const {
    return (*this).left(distance).down(distance);
  }

  /**
   * Get a square equals to "square.right(d).up(d)"
   */
  CONSTEXPR Square rightUp(RawType distance = 1) const {
    return (*this).right(distance).up(distance);
  }

  /**
   * Get a square equals to "square.right(d).down(d)"
   */
  CONSTEXPR Square rightDown(RawType distance = 1) const {
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
  CONSTEXPR Square safetyUp(RawType distance = 1) const {
    return (!isInvalid() && getRank() - distance >= 1) ?
      up(distance) : Square(Invalid);
  }

  /**
   * Get a square located under of the current square.
   * If the result is out of range, return Invalid.
   */
  CONSTEXPR Square safetyDown(RawType distance = 1) const {
    return (!isInvalid() && getRank() + distance <= 9) ?
      down(distance) : Square(Invalid);
  }

  /**
   * Get a square located to the left of the current square.
   * If the result is out of range, return Invalid.
   */
  CONSTEXPR Square safetyLeft(RawType distance = 1) const {
    return (!isInvalid() && getFile() + distance <= 9) ?
      left(distance) : Square(Invalid);
  }

  /**
   * Get a square located to the right of the current square.
   * If the result is out of range, return Invalid.
   */
  CONSTEXPR Square safetyRight(RawType distance = 1) const {
    return (!isInvalid() && getFile() - distance >= 1) ?
      right(distance) : Square(Invalid);
  }

  /**
   * Get a square equals to "square.left(d).up(d)"
   * If the result is out of range, return Invalid.
   */
  CONSTEXPR Square safetyLeftUp(RawType distance = 1) const {
    return (*this).safetyLeft(distance).safetyUp(distance);
  }

  /**
   * Get a square equals to "square.left(d).down(d)"
   * If the result is out of range, return Invalid.
   */
  CONSTEXPR Square safetyLeftDown(RawType distance = 1) const {
    return (*this).safetyLeft(distance).safetyDown(distance);
  }

  /**
   * Get a square equals to "square.right(d).up(d)"
   * If the result is out of range, return Invalid.
   */
  CONSTEXPR Square safetyRightUp(RawType distance = 1) const {
    return (*this).safetyRight(distance).safetyUp(distance);
  }

  /**
   * Get a square equals to "square.right(d).down(d)"
   * If the result is out of range, return Invalid.
   */
  CONSTEXPR Square safetyRightDown(RawType distance = 1) const {
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
  RawType distance(const Square& to) const;

  /**
   * Get a direction from the current square to the specified square.
   */
  Direction dir(const Square& to) const;

  /**
   * Get the rotated square.
   */
  RotatedSquare rotate0() const {
    assert(isStrictValid());
    return rotate0Table[number_];
  }

  /**
   * Get the rotated square.
   */
  RotatedSquare rotate90() const {
    assert(isStrictValid());
    return rotate90Table[number_];
  }

  /**
   * Get the rotated square.
   */
  RotatedSquare rotateRight45() const {
    assert(isStrictValid());
    return rotateRight45Table[number_];
  }

  /**
   * Get the rotated square.
   */
  RotatedSquare rotateLeft45() const {
    assert(isStrictValid());
    return rotateLeft45Table[number_];
  }

  /**
   * Get a square as a string type.
   */
  std::string toString() const;

  /**
   * Get square object from the specified string.
   */
  static Square parse(const char* str);

private:

  static const uint8_t rotate0Table[81];
  static const uint8_t rotate90Table[81];
  static const uint8_t rotateRight45Table[81];
  static const uint8_t rotateLeft45Table[81];

  RawType number_;

};

} // namespace sunfish

#define SQUARE_EACH(sq)    for (sunfish::Square (sq) = sunfish::Square::Begin; (sq) != sunfish::Square::End; (sq) = (sq).next())

inline bool operator==(uint8_t lhs, const sunfish::Square& rhs) {
  return lhs == rhs.raw();
}

inline bool operator!=(uint8_t lhs, const sunfish::Square& rhs) {
  return lhs != rhs.raw();
}

inline std::ostream& operator<<(std::ostream& os, const sunfish::Square& square) {
  os << square.raw();
  return os;
}

#endif // SUNFISH_CORE_BASE_SQUARE_HPP__
