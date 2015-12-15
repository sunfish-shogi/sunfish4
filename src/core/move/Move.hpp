/* Move.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_CORE_MOVE_MOVE_HPP__
#define SUNFISH_CORE_MOVE_MOVE_HPP__

#include "common/Def.hpp"
#include "core/base/Square.hpp"
#include "core/base/Piece.hpp"
#include <string>
#include <cassert>

namespace sunfish {

class Position;

class Move {
public:

  using RawType = uint32_t;
  using RawType16 = uint16_t;

private:

  static CONSTEXPR_CONST RawType ToMask   = 0x0000007f;
  static CONSTEXPR_CONST RawType FromMask = 0x00003f80;
  static CONSTEXPR_CONST RawType Promote  = 0x00004000;
  static CONSTEXPR_CONST RawType Drop     = 0x00008000;
  static CONSTEXPR_CONST RawType ExtMask  = 0xffff0000;
  static CONSTEXPR_CONST RawType Empty    = 0xffffffff;

  static CONSTEXPR_CONST int FromOffset = 7;
  static CONSTEXPR_CONST int ExtOffset = 16;

  explicit CONSTEXPR Move(RawType move) : move_(move) {
  }

public:

  /**
   * Default constructor
   */
  Move() {
  }

  /**
   * Construct a move of a piece on board.
   */
  CONSTEXPR Move(const Square& from, const Square& to, bool promote) :
      move_((static_cast<RawType>(to.raw())) |
            (static_cast<RawType>(from.raw()) << FromOffset) |
            (promote ? Promote : 0)) {
  }

  /**
   * Construct a drop
   */
  CONSTEXPR Move(const PieceType& pieceType, const Square& to) :
      move_((static_cast<RawType>(to.raw())) |
            (static_cast<RawType>(pieceType.raw()) << FromOffset) |
            Drop) {
  }

  /**
   * Get an object representing empty.
   */
  static CONSTEXPR Move empty() {
    return Move(Empty);
  }

  /**
   * Check the current object is empty.
   */
  bool CONSTEXPR isEmpty() const {
    return move_ == Empty;
  }

  /**
   * Get the starting square
   */
  CONSTEXPR Square from() const {
    return Square((move_ & FromMask) >> FromOffset);
  }

  /**
   * Get the ending square
   */
  CONSTEXPR Square to() const {
    return Square(move_ & ToMask);
  }

  /**
   * Check this move is promotion.
   */
  CONSTEXPR bool isPromotion() const {
    return move_ & Promote;
  }

  /**
   * Get the dropping piece type.
   */
  CONSTEXPR PieceType droppingPieceType() const {
    return PieceType((move_ & FromMask) >> FromOffset);
  }

  /**
   * Check this is dropping move.
   */
  CONSTEXPR bool isDrop() const {
    return move_ & Drop;
  }

  /**
   * Set an extension data.
   */
  void setExtData(RawType16 data) {
    move_ = (move_ & ~ExtMask) | (static_cast<RawType>(data) << ExtOffset);
  }

  /**
   * Get an extension data.
   */
  CONSTEXPR RawType16 extData() const {
    return static_cast<RawType16>(move_ >> ExtOffset);
  }

  CONSTEXPR Move excludeExtData() const {
    return Move(move_ & ~ExtMask);
  }

  CONSTEXPR bool operator==(const Move& obj) const {
    return ((move_ ^ obj.move_) & ~ExtMask) == 0;
  }

  CONSTEXPR bool operator!=(const Move& obj) const {
    return !operator==(obj);
  }

  /**
   * Get a raw value.
   */
  CONSTEXPR RawType serialize() const {
    return move_;
  }

  /**
   * Get a raw value which excluding a extension data of 16 bits.
   */
  CONSTEXPR RawType16 serialize16() const {
    return static_cast<RawType16>(move_);
  }

  /**
   * Get a move from a raw value.
   */
  static CONSTEXPR Move deserialize(RawType move) {
    return Move(move);
  }

  /**
   * Get a move from a raw value of 16 bits.
   */
  static CONSTEXPR Move deserialize(RawType16 move) {
    return Move(static_cast<RawType>(move));
  }

  /**
   * Swap two Move objects.
   */
  static void swap(Move& move1, Move& move2) {
    Move temp = move1;
    move1 = move2;
    move2 = temp;
  }

  /**
   * Get a string of proprietary format that is similar to CSA.
   */
  std::string toString() const;

  /**
   * Get a string of CSA format.
   */
  std::string toString(const Position& position) const;

  /**
   * Get a string of SFEN format.
   */
  std::string toStringSFEN() const;

  friend std::ostream& operator<<(std::ostream& os, const sunfish::Move& obj) {
    os << obj.toString() << '(' << obj.extData() << ')';

    return os;
  }

private:

  RawType move_;

};

} // namespace sunfish

#endif // SUNFISH_CORE_MOVE_MOVE_HPP__
