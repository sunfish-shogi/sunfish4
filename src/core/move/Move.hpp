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

private:

  static CONSTEXPR_CONST RawType S16Hand  = 0x8000;
  static CONSTEXPR_CONST RawType S16Empty = 0xffff;
  static CONSTEXPR_CONST RawType S16HandOffset = 7;

  static CONSTEXPR_CONST RawType FromMask    = 0x0000007f;
  static CONSTEXPR_CONST RawType ToMask      = 0x00003f80;
  static CONSTEXPR_CONST RawType PromoteMask = 0x00004000;
  static CONSTEXPR_CONST RawType PieceMask   = 0x000f8000;
  static CONSTEXPR_CONST RawType CaptureMask = 0x01f00000;
  static CONSTEXPR_CONST RawType UnusedMask  = 0xfe000000;
  static CONSTEXPR_CONST RawType Empty       = 0xffffffff;

  static CONSTEXPR_CONST int ToOffset = 7;
  static CONSTEXPR_CONST int PieceOffset = 15;
  static CONSTEXPR_CONST int CaptureOffset = 20;

  explicit CONSTEXPR Move(RawType move) : move_(move) {
  }

public:

  /**
   * Default constructor
   */
  Move() {
  }

  /**
   * Constructor
   */
  Move(const Piece& piece, const Square& from, const Square& to, bool promote) {
    assert(!piece.isEmpty());
    assert(piece.isUnpromoted() || !promote);
    assert(from.isValid());
    assert(to.isValid());
    move_ = (static_cast<RawType>(from.raw()) + 1)
          | (static_cast<RawType>(to.raw()) << ToOffset)
          | (static_cast<RawType>(piece.raw()) << PieceOffset);
    if (promote) {
      move_ |= PromoteMask;
    }
  }

  /**
   * Constructor
   */
  Move(const Piece& piece, const Square& to) {
    assert(!piece.isEmpty());
    assert(piece == piece.unpromote());
    move_ = (static_cast<RawType>(to.raw()) << ToOffset)
          | (static_cast<RawType>(piece.raw()) << PieceOffset);
  }

  /**
   * Get an object representing empty.
   */
  static Move empty() {
    return Move(Empty);
  }

  /**
   * Check the current object is empty.
   */
  bool isEmpty() const {
    return move_ == Empty;
  }

  /**
   * Get the starting square
   */
  Square from() const {
    return Square((move_ & FromMask) - 1);
  }

  /**
   * Get the ending square
   */
  Square to() const {
    return Square((move_ & ToMask) >> ToOffset);
  }

  /**
   * Check this move is promotion.
   */
  bool isPromotion() const {
    return move_ & PromoteMask;
  }

  /**
   * Get the target piece type.
   */
  Piece piece() const {
    return (move_ & PieceMask) >> PieceOffset;
  }

  /**
   * Set the captured piece type.
   */
  void setCapturedPiece(const Piece& cap) {
    assert(cap.isEmpty() || piece().isBlack() != cap.isBlack());
    move_ = (move_ & (~CaptureMask)) | ((static_cast<RawType>(cap.raw()) + 1U) << CaptureOffset);
  }

  /**
   * Get the captured piece type.
   */
  Piece capturedPiece() const {
    RawType cap = move_ & CaptureMask;
    return cap ? ((cap >> CaptureOffset) - 1U) : Piece::empty();
  }

  /**
   * Check this is capturing move.
   */
  bool isCapturing() const {
    return move_ & CaptureMask;
  }

  /**
   * Check this is dropping move.
   */
  bool isDrop() const {
    return !(move_ & FromMask);
  }

  /**
   * EQUAL operator
   */
  bool operator==(const Move& obj) const {
    return (move_ & ~(UnusedMask | CaptureMask)) == (obj.move_ & ~(UnusedMask | CaptureMask));
  }

  /**
   * NOT-EQUAL operator
   */
  bool operator!=(const Move& obj) const {
    return !operator==(obj);
  }

  /**
   * Serialize
   */
  static uint32_t serialize(const Move& obj) {
    return obj.move_ & (FromMask | ToMask | PromoteMask | PieceMask);
  }

  /**
   * Deserialize
   */
  static Move deserialize(uint32_t value) {
    return Move(value);
  }

  /**
   * Serialize
   */
  static uint16_t serialize16(const Move& obj);

  /**
   * Deserialize
   */
  static Move deserialize16(uint16_t value, const Position& position);

  /**
   * Swap two Move objects.
   */
  static void swap(Move& move1, Move& move2) {
    Move temp = move1;
    move1 = move2;
    move2 = temp;
  }

  /**
   * Get a string of CSA format.
   */
  std::string toString() const;

private:

  RawType move_;

};

} // namespace sunfish

inline std::ostream& operator<<(std::ostream& os, const sunfish::Move& move) {
  os << move.toString();
  return os;
}

#endif // SUNFISH_CORE_MOVE_MOVE_HPP__
