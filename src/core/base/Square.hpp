/* Square.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_CORE_BASE_SQUARE_HPP__
#define SUNFISH_CORE_BASE_SQUARE_HPP__

#include "common/Def.hpp"
#include "core/base/Turn.hpp"
#include "logger/Logger.hpp"
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
  End,
};

#define DIR_EACH(dir)    for (sunfish::Direction dir = Direction::Up; dir != sunfish::Direction::End; dir = static_cast<sunfish::Direction>(static_cast<int32_t>(dir) + 1))

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

using SquareRawType = int32_t;

class RotatedSquare {
public:

  static CONSTEXPR_CONST SquareRawType Invalid = -1;

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
  CONSTEXPR RotatedSquare(SquareRawType number) : number_(number) {
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
  CONSTEXPR bool operator==(SquareRawType rhs) const {
    return number_ == rhs;
  }

  /**
   * NOT-EQUALS operator which takes an integer value as an argument.
   */
  CONSTEXPR bool operator!=(SquareRawType rhs) const {
    return number_ != rhs;
  }

  /**
   * Get square number as an integer.
   */
  CONSTEXPR SquareRawType raw() const {
    return number_;
  }

private:

  SquareRawType number_;

};

/**
 * Numbers corresponding to each of the squares.
 */
enum class SquareNumber : SquareRawType {
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

class Square {
public:

  static CONSTEXPR_CONST SquareRawType Invalid = -1;
  static CONSTEXPR_CONST SquareRawType N = NUMBER_OF_SQUARES;
  static CONSTEXPR_CONST SquareRawType FileMax = SQUARE_FILE_MAX;
  static CONSTEXPR_CONST SquareRawType RankMax = SQUARE_RANK_MAX;
  static CONSTEXPR_CONST SquareRawType Begin = 0;
  static CONSTEXPR_CONST SquareRawType End = Begin + N;

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
  explicit CONSTEXPR Square(SquareRawType number) : number_(number) {
  }

  /**
   * Constructor which takes the file number and the rank number as arguments.
   */
  CONSTEXPR Square(SquareRawType file, SquareRawType rank) :
    number_((9 - file) * RankMax + rank - 1) {
  }

  static CONSTEXPR Square invalid() { return Square(Invalid); }
  static CONSTEXPR Square begin() { return Square(Begin); }
  static CONSTEXPR Square end() { return Square(End); }
  static CONSTEXPR Square s11() { return Square(static_cast<SquareRawType>(SquareNumber::S11)); }
  static CONSTEXPR Square s12() { return Square(static_cast<SquareRawType>(SquareNumber::S12)); }
  static CONSTEXPR Square s13() { return Square(static_cast<SquareRawType>(SquareNumber::S13)); }
  static CONSTEXPR Square s14() { return Square(static_cast<SquareRawType>(SquareNumber::S14)); }
  static CONSTEXPR Square s15() { return Square(static_cast<SquareRawType>(SquareNumber::S15)); }
  static CONSTEXPR Square s16() { return Square(static_cast<SquareRawType>(SquareNumber::S16)); }
  static CONSTEXPR Square s17() { return Square(static_cast<SquareRawType>(SquareNumber::S17)); }
  static CONSTEXPR Square s18() { return Square(static_cast<SquareRawType>(SquareNumber::S18)); }
  static CONSTEXPR Square s19() { return Square(static_cast<SquareRawType>(SquareNumber::S19)); }
  static CONSTEXPR Square s21() { return Square(static_cast<SquareRawType>(SquareNumber::S21)); }
  static CONSTEXPR Square s22() { return Square(static_cast<SquareRawType>(SquareNumber::S22)); }
  static CONSTEXPR Square s23() { return Square(static_cast<SquareRawType>(SquareNumber::S23)); }
  static CONSTEXPR Square s24() { return Square(static_cast<SquareRawType>(SquareNumber::S24)); }
  static CONSTEXPR Square s25() { return Square(static_cast<SquareRawType>(SquareNumber::S25)); }
  static CONSTEXPR Square s26() { return Square(static_cast<SquareRawType>(SquareNumber::S26)); }
  static CONSTEXPR Square s27() { return Square(static_cast<SquareRawType>(SquareNumber::S27)); }
  static CONSTEXPR Square s28() { return Square(static_cast<SquareRawType>(SquareNumber::S28)); }
  static CONSTEXPR Square s29() { return Square(static_cast<SquareRawType>(SquareNumber::S29)); }
  static CONSTEXPR Square s31() { return Square(static_cast<SquareRawType>(SquareNumber::S31)); }
  static CONSTEXPR Square s32() { return Square(static_cast<SquareRawType>(SquareNumber::S32)); }
  static CONSTEXPR Square s33() { return Square(static_cast<SquareRawType>(SquareNumber::S33)); }
  static CONSTEXPR Square s34() { return Square(static_cast<SquareRawType>(SquareNumber::S34)); }
  static CONSTEXPR Square s35() { return Square(static_cast<SquareRawType>(SquareNumber::S35)); }
  static CONSTEXPR Square s36() { return Square(static_cast<SquareRawType>(SquareNumber::S36)); }
  static CONSTEXPR Square s37() { return Square(static_cast<SquareRawType>(SquareNumber::S37)); }
  static CONSTEXPR Square s38() { return Square(static_cast<SquareRawType>(SquareNumber::S38)); }
  static CONSTEXPR Square s39() { return Square(static_cast<SquareRawType>(SquareNumber::S39)); }
  static CONSTEXPR Square s41() { return Square(static_cast<SquareRawType>(SquareNumber::S41)); }
  static CONSTEXPR Square s42() { return Square(static_cast<SquareRawType>(SquareNumber::S42)); }
  static CONSTEXPR Square s43() { return Square(static_cast<SquareRawType>(SquareNumber::S43)); }
  static CONSTEXPR Square s44() { return Square(static_cast<SquareRawType>(SquareNumber::S44)); }
  static CONSTEXPR Square s45() { return Square(static_cast<SquareRawType>(SquareNumber::S45)); }
  static CONSTEXPR Square s46() { return Square(static_cast<SquareRawType>(SquareNumber::S46)); }
  static CONSTEXPR Square s47() { return Square(static_cast<SquareRawType>(SquareNumber::S47)); }
  static CONSTEXPR Square s48() { return Square(static_cast<SquareRawType>(SquareNumber::S48)); }
  static CONSTEXPR Square s49() { return Square(static_cast<SquareRawType>(SquareNumber::S49)); }
  static CONSTEXPR Square s51() { return Square(static_cast<SquareRawType>(SquareNumber::S51)); }
  static CONSTEXPR Square s52() { return Square(static_cast<SquareRawType>(SquareNumber::S52)); }
  static CONSTEXPR Square s53() { return Square(static_cast<SquareRawType>(SquareNumber::S53)); }
  static CONSTEXPR Square s54() { return Square(static_cast<SquareRawType>(SquareNumber::S54)); }
  static CONSTEXPR Square s55() { return Square(static_cast<SquareRawType>(SquareNumber::S55)); }
  static CONSTEXPR Square s56() { return Square(static_cast<SquareRawType>(SquareNumber::S56)); }
  static CONSTEXPR Square s57() { return Square(static_cast<SquareRawType>(SquareNumber::S57)); }
  static CONSTEXPR Square s58() { return Square(static_cast<SquareRawType>(SquareNumber::S58)); }
  static CONSTEXPR Square s59() { return Square(static_cast<SquareRawType>(SquareNumber::S59)); }
  static CONSTEXPR Square s61() { return Square(static_cast<SquareRawType>(SquareNumber::S61)); }
  static CONSTEXPR Square s62() { return Square(static_cast<SquareRawType>(SquareNumber::S62)); }
  static CONSTEXPR Square s63() { return Square(static_cast<SquareRawType>(SquareNumber::S63)); }
  static CONSTEXPR Square s64() { return Square(static_cast<SquareRawType>(SquareNumber::S64)); }
  static CONSTEXPR Square s65() { return Square(static_cast<SquareRawType>(SquareNumber::S65)); }
  static CONSTEXPR Square s66() { return Square(static_cast<SquareRawType>(SquareNumber::S66)); }
  static CONSTEXPR Square s67() { return Square(static_cast<SquareRawType>(SquareNumber::S67)); }
  static CONSTEXPR Square s68() { return Square(static_cast<SquareRawType>(SquareNumber::S68)); }
  static CONSTEXPR Square s69() { return Square(static_cast<SquareRawType>(SquareNumber::S69)); }
  static CONSTEXPR Square s71() { return Square(static_cast<SquareRawType>(SquareNumber::S71)); }
  static CONSTEXPR Square s72() { return Square(static_cast<SquareRawType>(SquareNumber::S72)); }
  static CONSTEXPR Square s73() { return Square(static_cast<SquareRawType>(SquareNumber::S73)); }
  static CONSTEXPR Square s74() { return Square(static_cast<SquareRawType>(SquareNumber::S74)); }
  static CONSTEXPR Square s75() { return Square(static_cast<SquareRawType>(SquareNumber::S75)); }
  static CONSTEXPR Square s76() { return Square(static_cast<SquareRawType>(SquareNumber::S76)); }
  static CONSTEXPR Square s77() { return Square(static_cast<SquareRawType>(SquareNumber::S77)); }
  static CONSTEXPR Square s78() { return Square(static_cast<SquareRawType>(SquareNumber::S78)); }
  static CONSTEXPR Square s79() { return Square(static_cast<SquareRawType>(SquareNumber::S79)); }
  static CONSTEXPR Square s81() { return Square(static_cast<SquareRawType>(SquareNumber::S81)); }
  static CONSTEXPR Square s82() { return Square(static_cast<SquareRawType>(SquareNumber::S82)); }
  static CONSTEXPR Square s83() { return Square(static_cast<SquareRawType>(SquareNumber::S83)); }
  static CONSTEXPR Square s84() { return Square(static_cast<SquareRawType>(SquareNumber::S84)); }
  static CONSTEXPR Square s85() { return Square(static_cast<SquareRawType>(SquareNumber::S85)); }
  static CONSTEXPR Square s86() { return Square(static_cast<SquareRawType>(SquareNumber::S86)); }
  static CONSTEXPR Square s87() { return Square(static_cast<SquareRawType>(SquareNumber::S87)); }
  static CONSTEXPR Square s88() { return Square(static_cast<SquareRawType>(SquareNumber::S88)); }
  static CONSTEXPR Square s89() { return Square(static_cast<SquareRawType>(SquareNumber::S89)); }
  static CONSTEXPR Square s91() { return Square(static_cast<SquareRawType>(SquareNumber::S91)); }
  static CONSTEXPR Square s92() { return Square(static_cast<SquareRawType>(SquareNumber::S92)); }
  static CONSTEXPR Square s93() { return Square(static_cast<SquareRawType>(SquareNumber::S93)); }
  static CONSTEXPR Square s94() { return Square(static_cast<SquareRawType>(SquareNumber::S94)); }
  static CONSTEXPR Square s95() { return Square(static_cast<SquareRawType>(SquareNumber::S95)); }
  static CONSTEXPR Square s96() { return Square(static_cast<SquareRawType>(SquareNumber::S96)); }
  static CONSTEXPR Square s97() { return Square(static_cast<SquareRawType>(SquareNumber::S97)); }
  static CONSTEXPR Square s98() { return Square(static_cast<SquareRawType>(SquareNumber::S98)); }
  static CONSTEXPR Square s99() { return Square(static_cast<SquareRawType>(SquareNumber::S99)); }

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
  CONSTEXPR bool operator==(SquareRawType rhs) const {
    return number_ == rhs;
  }

  /**
   * NOT-EQUALS operator which takes an integer value as an argument.
   */
  CONSTEXPR bool operator!=(SquareRawType rhs) const {
    return number_ != rhs;
  }

  /**
   * Get square number as an integer.
   */
  CONSTEXPR SquareRawType raw() const {
    return number_;
  }

  /**
   * Get a file number.
   */
  CONSTEXPR SquareRawType getFile() const {
    return 9 - (number_ / RankMax);
  }

  /**
   * Get a rank number.
   */
  CONSTEXPR SquareRawType getRank() const {
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
  static CONSTEXPR bool isValidFile(SquareRawType file) {
    return file >= 1 && file <= 9;
  }

  /**
   * Check if the rank number is in valid range.
   */
  static CONSTEXPR bool isValidRank(SquareRawType rank) {
    return rank >= 1 && rank <= 9;
  }

  /**
   * Check if the square is promotable range.
   */
  template<Turn turn>
  CONSTEXPR bool isPromotable() const {
    return (turn == Turn::Black) ? getRank() - 1 <= 2 : getRank() - 1 >= 6;
  }

  /**
   * Check if the square is movable range for Pawn.
   */
  template<Turn turn>
  CONSTEXPR bool isPawnMovable() const {
    return (turn == Turn::Black) ? getRank() - 1 != 0 : getRank() - 1 != 8;
  }

  /**
   * Check if the square is movable range for Lance.
   */
  template<Turn turn>
  CONSTEXPR bool isLanceMovable() const {
    return isPawnMovable<turn>();
  }

  /**
   * Check if the square is movable range for Knight.
   */
  template<Turn turn>
  CONSTEXPR bool isKnightMovable() const {
    return (turn == Turn::Black) ? getRank() - 1 >= 2 : getRank() - 1 <= 6;
  }

  /**
   * Returns the square of point symmetry.
   */
  CONSTEXPR Square psym() const {
    return Square(N - 1 - number_);
  }

  /**
   * Returns the square of line symmetry in horizontal direction.
   */
  CONSTEXPR Square hsym() const {
    return Square(FileMax + 1 - getFile(), getRank());
  }

  /**
   * Returns the square of line symmetry in vertical direction.
   */
  CONSTEXPR Square vsym() const {
    return Square(getFile(), RankMax + 1 - getRank());
  }

  /**
   * Get a square located over of the current square.
   */
  CONSTEXPR Square up(SquareRawType distance = 1) const {
    return Square(number_ - distance);
  }

  /**
   * Get a square located under of the current square.
   */
  CONSTEXPR Square down(SquareRawType distance = 1) const {
    return Square(number_ + distance);
  }

  /**
   * Get a square located to the left of the current square.
   */
  CONSTEXPR Square left(SquareRawType distance = 1) const {
    return Square(number_ - distance * RankMax);
  }

  /**
   * Get a square located to the right of the current square.
   */
  CONSTEXPR Square right(SquareRawType distance = 1) const {
    return Square(number_ + distance * RankMax);
  }

  /**
   * Get a square equals to "square.left(d).up(d)"
   */
  CONSTEXPR Square leftUp(SquareRawType distance = 1) const {
    return (*this).left(distance).up(distance);
  }

  /**
   * Get a square equals to "square.left(d).down(d)"
   */
  CONSTEXPR Square leftDown(SquareRawType distance = 1) const {
    return (*this).left(distance).down(distance);
  }

  /**
   * Get a square equals to "square.right(d).up(d)"
   */
  CONSTEXPR Square rightUp(SquareRawType distance = 1) const {
    return (*this).right(distance).up(distance);
  }

  /**
   * Get a square equals to "square.right(d).down(d)"
   */
  CONSTEXPR Square rightDown(SquareRawType distance = 1) const {
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
  CONSTEXPR Square safetyUp(SquareRawType distance = 1) const {
    return (!isInvalid() && getRank() - distance >= 1) ?
      up(distance) : invalid();
  }

  /**
   * Get a square located under of the current square.
   * If the result is out of range, return Invalid.
   */
  CONSTEXPR Square safetyDown(SquareRawType distance = 1) const {
    return (!isInvalid() && getRank() + distance <= 9) ?
      down(distance) : invalid();
  }

  /**
   * Get a square located to the left of the current square.
   * If the result is out of range, return Invalid.
   */
  CONSTEXPR Square safetyLeft(SquareRawType distance = 1) const {
    return (!isInvalid() && getFile() + distance <= 9) ?
      left(distance) : invalid();
  }

  /**
   * Get a square located to the right of the current square.
   * If the result is out of range, return Invalid.
   */
  CONSTEXPR Square safetyRight(SquareRawType distance = 1) const {
    return (!isInvalid() && getFile() - distance >= 1) ?
      right(distance) : invalid();
  }

  /**
   * Get a square equals to "square.left(d).up(d)"
   * If the result is out of range, return Invalid.
   */
  CONSTEXPR Square safetyLeftUp(SquareRawType distance = 1) const {
    return (*this).safetyLeft(distance).safetyUp(distance);
  }

  /**
   * Get a square equals to "square.left(d).down(d)"
   * If the result is out of range, return Invalid.
   */
  CONSTEXPR Square safetyLeftDown(SquareRawType distance = 1) const {
    return (*this).safetyLeft(distance).safetyDown(distance);
  }

  /**
   * Get a square equals to "square.right(d).up(d)"
   * If the result is out of range, return Invalid.
   */
  CONSTEXPR Square safetyRightUp(SquareRawType distance = 1) const {
    return (*this).safetyRight(distance).safetyUp(distance);
  }

  /**
   * Get a square equals to "square.right(d).down(d)"
   * If the result is out of range, return Invalid.
   */
  CONSTEXPR Square safetyRightDown(SquareRawType distance = 1) const {
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
  SquareRawType distance(const Square& to) const;

  /**
   * Get a direction from the current square to the specified square.
   */
  Direction dir(const Square& to) const;

  Square move(Direction dir) const {
    switch (dir) {
    case Direction::Up:              return up();
    case Direction::Down:            return down();
    case Direction::Left:            return left();
    case Direction::Right:           return right();
    case Direction::LeftUp:          return leftUp();
    case Direction::LeftDown:        return leftDown();
    case Direction::RightUp:         return rightUp();
    case Direction::RightDown:       return rightDown();
    case Direction::LeftUpKnight:    return leftUpKnight();
    case Direction::LeftDownKnight:  return leftDownKnight();
    case Direction::RightUpKnight:   return rightUpKnight();
    case Direction::RightDownKnight: return rightDownKnight();
    default: return invalid();
    }
  }

  Square safetyMove(Direction dir) const {
    switch (dir) {
    case Direction::Up:              return safetyUp();
    case Direction::Down:            return safetyDown();
    case Direction::Left:            return safetyLeft();
    case Direction::Right:           return safetyRight();
    case Direction::LeftUp:          return safetyLeftUp();
    case Direction::LeftDown:        return safetyLeftDown();
    case Direction::RightUp:         return safetyRightUp();
    case Direction::RightDown:       return safetyRightDown();
    case Direction::LeftUpKnight:    return safetyLeftUpKnight();
    case Direction::LeftDownKnight:  return safetyLeftDownKnight();
    case Direction::RightUpKnight:   return safetyRightUpKnight();
    case Direction::RightDownKnight: return safetyRightDownKnight();
    default: return invalid();
    }
  }

  /**
   * Get the rotated square.
   */
  RotatedSquare rotate90() const {
    ASSERT(isStrictValid());
    return rotate90Table[number_];
  }

  /**
   * Get the rotated square.
   */
  RotatedSquare rotateRight45() const {
    ASSERT(isStrictValid());
    return rotateRight45Table[number_];
  }

  /**
   * Get the rotated square.
   */
  RotatedSquare rotateLeft45() const {
    ASSERT(isStrictValid());
    return rotateLeft45Table[number_];
  }

  /**
   * Get a string representing the square.
   */
  std::string toString() const;

  /**
   * Get square object from the specified string.
   */
  static Square parse(const char* str);

  /**
   * Get a string of SFEN format representing the square.
   */
  std::string toStringSFEN() const;

  /**
   * Get square object from the specified string of SFEN format.
   */
  static Square parseSFEN(const char* str);

  friend std::ostream& operator<<(std::ostream& os, const sunfish::Square& square) {
    os << square.toString();
    return os;
  }

private:

  static const uint8_t rotate90Table[81];
  static const uint8_t rotateRight45Table[81];
  static const uint8_t rotateLeft45Table[81];

  SquareRawType number_;

};

} // namespace sunfish

#define SQUARE_EACH(sq)    for (sunfish::Square (sq) = sunfish::Square::begin(); (sq) != sunfish::Square::end(); (sq) = (sq).next())

inline bool operator==(uint8_t lhs, const sunfish::Square& rhs) {
  return lhs == rhs.raw();
}

inline bool operator!=(uint8_t lhs, const sunfish::Square& rhs) {
  return lhs != rhs.raw();
}

#endif // SUNFISH_CORE_BASE_SQUARE_HPP__
