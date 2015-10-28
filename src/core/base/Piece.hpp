/* Piece.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_CORE_BASE_PIECE__
#define SUNFISH_CORE_BASE_PIECE__

#include "common/Def.hpp"
#include <iostream>
#include <string>
#include <cstdint>

namespace sunfish {

class Piece {
public:
  static CONSTEXPR_CONST uint8_t Promotion = 0x08;
  static CONSTEXPR_CONST uint8_t Empty = 0x20;
  static CONSTEXPR_CONST uint8_t White = 0x10;

  static CONSTEXPR_CONST uint8_t HandMask = 0x27;
  static CONSTEXPR_CONST uint8_t KindMask = 0x2f;

  static CONSTEXPR_CONST uint8_t Pawn = 0;
  static CONSTEXPR_CONST uint8_t Lance = 1;
  static CONSTEXPR_CONST uint8_t Knight = 2;
  static CONSTEXPR_CONST uint8_t Silver = 3;
  static CONSTEXPR_CONST uint8_t Gold = 4;
  static CONSTEXPR_CONST uint8_t Bishop = 5;
  static CONSTEXPR_CONST uint8_t Rook = 6;
  static CONSTEXPR_CONST uint8_t King = 7;
  static CONSTEXPR_CONST uint8_t Tokin = Promotion | Pawn;
  static CONSTEXPR_CONST uint8_t ProLance = Promotion | Lance;
  static CONSTEXPR_CONST uint8_t ProKnight = Promotion | Knight;
  static CONSTEXPR_CONST uint8_t ProSilver = Promotion | Silver;
  static CONSTEXPR_CONST uint8_t Horse = Promotion | Bishop;
  static CONSTEXPR_CONST uint8_t Dragon = Promotion | Rook;

  static CONSTEXPR_CONST uint8_t BPawn = Pawn;
  static CONSTEXPR_CONST uint8_t BLance = Lance;
  static CONSTEXPR_CONST uint8_t BKnight = Knight;
  static CONSTEXPR_CONST uint8_t BSilver = Silver;
  static CONSTEXPR_CONST uint8_t BGold = Gold;
  static CONSTEXPR_CONST uint8_t BBishop = Bishop;
  static CONSTEXPR_CONST uint8_t BRook = Rook;
  static CONSTEXPR_CONST uint8_t BKing = King;
  static CONSTEXPR_CONST uint8_t BTokin = Tokin;
  static CONSTEXPR_CONST uint8_t BProLance = ProLance;
  static CONSTEXPR_CONST uint8_t BProKnight = ProKnight;
  static CONSTEXPR_CONST uint8_t BProSilver = ProSilver;
  static CONSTEXPR_CONST uint8_t BHorse = Horse;
  static CONSTEXPR_CONST uint8_t BDragon = Dragon;

  static CONSTEXPR_CONST uint8_t WPawn = White | Pawn;
  static CONSTEXPR_CONST uint8_t WLance = White | Lance;
  static CONSTEXPR_CONST uint8_t WKnight = White | Knight;
  static CONSTEXPR_CONST uint8_t WSilver = White | Silver;
  static CONSTEXPR_CONST uint8_t WGold = White | Gold;
  static CONSTEXPR_CONST uint8_t WBishop = White | Bishop;
  static CONSTEXPR_CONST uint8_t WRook = White | Rook;
  static CONSTEXPR_CONST uint8_t WKing = White | King;
  static CONSTEXPR_CONST uint8_t WTokin = White | Tokin;
  static CONSTEXPR_CONST uint8_t WProLance = White | ProLance;
  static CONSTEXPR_CONST uint8_t WProKnight = White | ProKnight;
  static CONSTEXPR_CONST uint8_t WProSilver = White | ProSilver;
  static CONSTEXPR_CONST uint8_t WHorse = White | Horse;
  static CONSTEXPR_CONST uint8_t WDragon = White | Dragon;

  static CONSTEXPR_CONST uint8_t Num = WDragon + 1;
  static CONSTEXPR_CONST uint8_t Begin = BPawn;
  static CONSTEXPR_CONST uint8_t End = WDragon + 1;

  static CONSTEXPR_CONST uint8_t HandNum = Rook + 1;
  static CONSTEXPR_CONST uint8_t HandBegin = Pawn;
  static CONSTEXPR_CONST uint8_t HandEnd = Rook + 1;

  static CONSTEXPR_CONST uint8_t KindNum = Dragon + 1;
  static CONSTEXPR_CONST uint8_t KindBegin = Pawn;
  static CONSTEXPR_CONST uint8_t KindEnd = Dragon + 1;

public:

  /**
   * Default constructor
   */
  CONSTEXPR Piece() : number_(Empty) {
  }

  /**
   * Constructor which takes the piece number as an argument.
   */
  CONSTEXPR Piece(uint8_t number) : number_(number) {
  }

  /**
   * Get piece number as unsigned 8-bit integer
   */
  explicit CONSTEXPR operator uint8_t() const {
    return number_;
  }

  /**
   * Get piece number as unsigned 8-bit integer
   */
  CONSTEXPR uint8_t raw() const {
    return number_;
  }

  /**
   * EQUALS operator.
   */
  bool operator==(const Piece& rhs) const {
    return number_ == rhs.number_;
  }

  /**
   * NOT-EQUALS operator.
   */
  bool operator!=(const Piece& rhs) const {
    return number_ != rhs.number_;
  }

  /**
   * Check the value is NOT equal to Empty
   */
  CONSTEXPR bool exists() const {
    return number_ != Empty;
  }

  /**
   * Check the value is equal to Empty
   */
  CONSTEXPR bool isEmpty() const {
    return number_ == Empty;
  }

  /**
   * Get a piece type which is converted to hand type range.
   */
  CONSTEXPR Piece hand() const {
    return Piece(number_ & HandMask);
  }

  /**
   * Get a promoted piece type
   */
  CONSTEXPR Piece promote() const {
    return Piece(number_ | Promotion);
  }

  /**
   * Get a unpromoted piece type
   */
  CONSTEXPR Piece unpromote() const {
    return Piece(number_ & ~Promotion);
  }

  /**
   * Get a piece not containing turn info.
   */
  CONSTEXPR Piece kindOnly() const {
    return Piece(number_ & KindMask);
  }

  /**
   * Get a black piece.
   */
  CONSTEXPR Piece black() const {
    return Piece(number_ & ~White);
  }

  /**
   * Get a white piece.
   */
  CONSTEXPR Piece white() const {
    return Piece(number_ | White);
  }

  /**
   * Check the piece is NOT promoted.
   */
  CONSTEXPR bool isUnpromoted() const {
    return !isPromoted();
  }

  /**
   * Check the piece is promoted.
   */
  CONSTEXPR bool isPromoted() const {
    return number_ & Promotion;
  }

  /**
   * Check the piece is black.
   */
  CONSTEXPR bool isBlack() const {
    return !(number_ & (Empty | White));
  }

  /**
   * Check the piece is white.
   */
  CONSTEXPR bool isWhite() const {
    return number_ & White;
  }

  /**
   * Get pieces in numerical order.
   * See PIECE_EACH macro.
   */
  CONSTEXPR Piece next() const {
    return ((number_ == (Promotion | BGold) - 1U) ||
            (number_ == (Promotion | WGold) - 1U) ||
            (number_ == (Promotion | BKing) - 1U)) ?
           number_ + 2U : number_ + 1U;
  }

  /**
   * Get pieces in numerical order.
   * See PIECE_EACH_UNSAFE macro.
   */
  CONSTEXPR Piece nextUnsafe() const {
    return Piece(number_ + 1U);
  }

  /**
   * Get a piece name(CSA format).
   */
  std::string toString(bool kind_only = false) const;

  /**
   * Get piece from the specified piece name(CSA format).
   */
  static Piece parse(const char* str);

private:

  uint8_t number_;

};

} // namespace sunfish

#define PIECE_EACH(piece)        for (sunfish::Piece (piece) = sunfish::Piece::Begin; (piece) != sunfish::Piece::End; (piece) = (piece).next())
#define PIECE_EACH_UNSAFE(piece) for (sunfish::Piece (piece) = sunfish::Piece::Begin; (piece) != sunfish::Piece::End; (piece) = (piece).nextUnsafe())
#define PIECE_KIND_EACH(piece)   for (sunfish::Piece (piece) = sunfish::Piece::KindBegin; (piece) != sunfish::Piece::KindEnd; (piece) = (piece).nextUnsafe())
#define HAND_EACH(piece)         for (sunfish::Piece (piece) = sunfish::Piece::HandBegin; (piece) != sunfish::Piece::HandEnd; (piece) = (piece).nextUnsafe())

inline bool operator==(uint8_t lhs, const sunfish::Piece& rhs) {
  return lhs == rhs.raw();
}

inline bool operator!=(uint8_t lhs, const sunfish::Piece& rhs) {
  return lhs != rhs.raw();
}

inline std::ostream& operator<<(std::ostream& os, const sunfish::Piece& piece) {
  os << piece.toString();
  return os;
}

#endif // SUNFISH_CORE_BASE_PIECE__
