/* Hand.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_CORE_POSITION_HAND_HPP__
#define SUNFISH_CORE_POSITION_HAND_HPP__

#include "core/base/Piece.hpp"
#include <array>
#include <algorithm>
#include <cassert>

namespace sunfish {

class Hand {
public:

  using Type = uint8_t;

  /**
   * Constructor
   */
  Hand() : counts_{{0}} {
  }

  /**
   * Copy constructor
   */
  Hand(const Hand&) = default;

  /**
   * Assignment operator
   */
  Hand& operator=(const Hand&) = default;

  /**
   * Increment the count of specified piece.
   */
  Type inc(const PieceType& piece) {
    return incUnsafe(piece.unpromote());
  }

  /**
   * Increment the count of specified piece.
   */
  Type incUnsafe(const PieceType& piece) {
    assert(piece == piece.unpromote());
    assert(counts_[piece.raw()] < 18);
    assert(piece != PieceType::lance() || counts_[piece.raw()] < 4);
    assert(piece != PieceType::knight() || counts_[piece.raw()] < 4);
    assert(piece != PieceType::silver() || counts_[piece.raw()] < 4);
    assert(piece != PieceType::gold() || counts_[piece.raw()] < 4);
    assert(piece != PieceType::bishop() || counts_[piece.raw()] < 2);
    assert(piece != PieceType::rook() || counts_[piece.raw()] < 2);
    return ++counts_[piece.raw()];
  }

  /**
   * Decrement the count of specified piece.
   */
  Type dec(const PieceType& piece) {
    return decUnsafe(piece.unpromote());
  }

  /**
   * Decrement the count of specified piece.
   */
  Type decUnsafe(const PieceType& piece) {
    assert(piece == piece.unpromote());
    assert(counts_[piece.raw()] > 0);
    return --counts_[piece.raw()];
  }

  /**
   * Get the count of specified piece.
   */
  Type get(const PieceType& piece) const {
    return counts_[piece.raw()];
  }

  /**
   * Set the count of specified piece.
   */
  void set(const PieceType& piece, Type val) {
    counts_[piece.raw()] = val;
  }

private:

  std::array<Type, PieceNumber::HandNum> counts_;

};

}

#endif // SUNFISH_CORE_POSITION_HAND_HPP__
