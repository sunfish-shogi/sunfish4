/* Piece.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_CORE_BASE_PIECE__
#define SUNFISH_CORE_BASE_PIECE__

#include "common/Def.hpp"
#include "core/base/Turn.hpp"
#include <iostream>
#include <string>
#include <cstdint>

namespace sunfish {

using PieceRawType = uint8_t;

namespace PieceNumber_ {

enum Type : PieceRawType {
  Promotion  = 0x08,
  White      = 0x10,
  Empty      = 0x20,
            
  HandMask   = 0x27,
  TypeMask   = 0x2f,
            
  Pawn       = 0,
  Lance      = 1,
  Knight     = 2,
  Silver     = 3,
  Gold       = 4,
  Bishop     = 5,
  Rook       = 6,
  King       = 7,
  Tokin      = Promotion | Pawn,
  ProLance   = Promotion | Lance,
  ProKnight  = Promotion | Knight,
  ProSilver  = Promotion | Silver,
  Horse      = Promotion | Bishop,
  Dragon     = Promotion | Rook,
            
  TypeNum    = Dragon + 1,
  TypeBegin  = Pawn,
  TypeEnd    = Dragon + 1,
            
  HandNum    = Rook + 1,
  HandBegin  = Pawn,
  HandEnd    = Rook + 1,
            
  BPawn      = Pawn,
  BLance     = Lance,
  BKnight    = Knight,
  BSilver    = Silver,
  BGold      = Gold,
  BBishop    = Bishop,
  BRook      = Rook,
  BKing      = King,
  BTokin     = Tokin,
  BProLance  = ProLance,
  BProKnight = ProKnight,
  BProSilver = ProSilver,
  BHorse     = Horse,
  BDragon    = Dragon,

  WPawn      = White | Pawn,
  WLance     = White | Lance,
  WKnight    = White | Knight,
  WSilver    = White | Silver,
  WGold      = White | Gold,
  WBishop    = White | Bishop,
  WRook      = White | Rook,
  WKing      = White | King,
  WTokin     = White | Tokin,
  WProLance  = White | ProLance,
  WProKnight = White | ProKnight,
  WProSilver = White | ProSilver,
  WHorse     = White | Horse,
  WDragon    = White | Dragon,

  Num        = WDragon + 1,
  Begin      = BPawn,
  End        = WDragon + 1,
};

} // PieceNumber_

using PieceNumber = PieceNumber_::Type;

class Piece;

template <class T>
class AbstractPieceType {
public:

  /**
   * Default constructor
   */
  AbstractPieceType() = default;

  /**
   * Copy constructor
   */
  explicit CONSTEXPR AbstractPieceType(const T& src) : number_(src.number_) {
  }

  /**
   * Move constructor
   */
  explicit CONSTEXPR AbstractPieceType(T&& src) : number_(src.number_) {
  }

  /**
   * Constructor which takes the piece number as an argument.
   */
  CONSTEXPR AbstractPieceType(PieceRawType number) : number_(number) {
  }

  static CONSTEXPR T empty() {
    return T(PieceNumber::Empty);
  }

  /**
   * FIXME
   * Get piece number as unsigned 8-bit integer
   */
  CONSTEXPR PieceRawType raw() const {
    return number_;
  }

  /**
   * Assignment operator
   */
  T& operator=(const T& rhs) {
    number_ == rhs.number_;
    return *(static_cast<T*>(this));
  }

  /**
   * EQUALS operator.
   */
  CONSTEXPR bool operator==(const T& rhs) const {
    return number_ == rhs.number_;
  }

  /**
   * NOT-EQUALS operator.
   */
  CONSTEXPR bool operator!=(const T& rhs) const {
    return number_ != rhs.number_;
  }

  /**
   * Check the value is equal to Empty
   */
  CONSTEXPR bool isEmpty() const {
    return number_ == PieceNumber::Empty;
  }

  /**
   * Get a piece type which is converted to hand type range.
   */
  CONSTEXPR T hand() const {
    return T(number_ & PieceNumber::HandMask);
  }

  /**
   * Get a promoted piece type
   */
  CONSTEXPR T promote() const {
    return T(number_ | PieceNumber::Promotion);
  }

  /**
   * Get a unpromoted piece type
   */
  CONSTEXPR T unpromote() const {
    return T(number_ & ~PieceNumber::Promotion);
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
    return number_ & PieceNumber::Promotion;
  }

  CONSTEXPR Piece black() const;

  CONSTEXPR Piece white() const;

protected:

  PieceRawType number_;

};

class PieceType : public AbstractPieceType<PieceType> {
public:

  using SuperType = AbstractPieceType<PieceType>;
  using SuperType::AbstractPieceType;

  static CONSTEXPR PieceType pawn     () { return PieceType(PieceNumber::Pawn     ); }
  static CONSTEXPR PieceType lance    () { return PieceType(PieceNumber::Lance    ); }
  static CONSTEXPR PieceType knight   () { return PieceType(PieceNumber::Knight   ); }
  static CONSTEXPR PieceType silver   () { return PieceType(PieceNumber::Silver   ); }
  static CONSTEXPR PieceType gold     () { return PieceType(PieceNumber::Gold     ); }
  static CONSTEXPR PieceType bishop   () { return PieceType(PieceNumber::Bishop   ); }
  static CONSTEXPR PieceType rook     () { return PieceType(PieceNumber::Rook     ); }
  static CONSTEXPR PieceType king     () { return PieceType(PieceNumber::King     ); }
  static CONSTEXPR PieceType tokin    () { return PieceType(PieceNumber::Tokin    ); }
  static CONSTEXPR PieceType proLance () { return PieceType(PieceNumber::ProLance ); }
  static CONSTEXPR PieceType proKnight() { return PieceType(PieceNumber::ProKnight); }
  static CONSTEXPR PieceType proSilver() { return PieceType(PieceNumber::ProSilver); }
  static CONSTEXPR PieceType horse    () { return PieceType(PieceNumber::Horse    ); }
  static CONSTEXPR PieceType dragon   () { return PieceType(PieceNumber::Dragon   ); }

  /**
   * Get pieces in numerical order.
   * See TYPE_EACH macro.
   */
  CONSTEXPR PieceType next() const {
    return SuperType::number_ == (PieceNumber::Promotion | PieceNumber::Gold) - 1U ?
           SuperType::number_ + 2U : SuperType::number_ + 1U;
  }

  /**
   * Get pieces in numerical order.
   * See HAND_EACH macro.
   */
  CONSTEXPR PieceType nextUnsafe() const {
    return PieceType(SuperType::number_ + 1U);
  }

  /**
   * Get a piece name in the CSA format.
   */
  std::string toString() const;

  /**
   * Get piece from the specified piece name(CSA format).
   */
  static PieceType parse(const char* str);

};

template <class T>
class AbstractPiece : public AbstractPieceType<T> {
public:

  using SuperType = AbstractPieceType<T>;
  using SuperType::AbstractPieceType;

  /**
   * Get a piece type which is converted to hand type range.
   */
  CONSTEXPR PieceType hand() const {
    return PieceType(SuperType::number_ & PieceNumber::HandMask);
  }

  /**
   * Get a PieceType object.
   */
  CONSTEXPR PieceType type() const {
    return PieceType(SuperType::number_ & PieceNumber::TypeMask);
  }

  /**
   * Get a black piece.
   */
  CONSTEXPR T black() const {
    return T(SuperType::number_ & ~PieceNumber::White);
  }

  /**
   * Get a white piece.
   */
  CONSTEXPR T white() const {
    return T(SuperType::number_ | PieceNumber::White);
  }

  /**
   * Check the piece is black.
   */
  CONSTEXPR bool isBlack() const {
    return !(SuperType::number_ & (PieceNumber::Empty | PieceNumber::White));
  }

  /**
   * Check the piece is white.
   */
  CONSTEXPR bool isWhite() const {
    return SuperType::number_ & PieceNumber::White;
  }

};

class Piece : public AbstractPiece<Piece> {
public:

  using SuperType = AbstractPiece<Piece>;
  using SuperType::AbstractPiece;

  static CONSTEXPR Piece blackPawn     () { return Piece(PieceNumber::BPawn     ); }
  static CONSTEXPR Piece blackLance    () { return Piece(PieceNumber::BLance    ); }
  static CONSTEXPR Piece blackKnight   () { return Piece(PieceNumber::BKnight   ); }
  static CONSTEXPR Piece blackSilver   () { return Piece(PieceNumber::BSilver   ); }
  static CONSTEXPR Piece blackGold     () { return Piece(PieceNumber::BGold     ); }
  static CONSTEXPR Piece blackBishop   () { return Piece(PieceNumber::BBishop   ); }
  static CONSTEXPR Piece blackRook     () { return Piece(PieceNumber::BRook     ); }
  static CONSTEXPR Piece blackKing     () { return Piece(PieceNumber::BKing     ); }
  static CONSTEXPR Piece blackTokin    () { return Piece(PieceNumber::BTokin    ); }
  static CONSTEXPR Piece blackProLance () { return Piece(PieceNumber::BProLance ); }
  static CONSTEXPR Piece blackProKnight() { return Piece(PieceNumber::BProKnight); }
  static CONSTEXPR Piece blackProSilver() { return Piece(PieceNumber::BProSilver); }
  static CONSTEXPR Piece blackHorse    () { return Piece(PieceNumber::BHorse    ); }
  static CONSTEXPR Piece blackDragon   () { return Piece(PieceNumber::BDragon   ); }

  static CONSTEXPR Piece whitePawn     () { return Piece(PieceNumber::WPawn     ); }
  static CONSTEXPR Piece whiteLance    () { return Piece(PieceNumber::WLance    ); }
  static CONSTEXPR Piece whiteKnight   () { return Piece(PieceNumber::WKnight   ); }
  static CONSTEXPR Piece whiteSilver   () { return Piece(PieceNumber::WSilver   ); }
  static CONSTEXPR Piece whiteGold     () { return Piece(PieceNumber::WGold     ); }
  static CONSTEXPR Piece whiteBishop   () { return Piece(PieceNumber::WBishop   ); }
  static CONSTEXPR Piece whiteRook     () { return Piece(PieceNumber::WRook     ); }
  static CONSTEXPR Piece whiteKing     () { return Piece(PieceNumber::WKing     ); }
  static CONSTEXPR Piece whiteTokin    () { return Piece(PieceNumber::WTokin    ); }
  static CONSTEXPR Piece whiteProLance () { return Piece(PieceNumber::WProLance ); }
  static CONSTEXPR Piece whiteProKnight() { return Piece(PieceNumber::WProKnight); }
  static CONSTEXPR Piece whiteProSilver() { return Piece(PieceNumber::WProSilver); }
  static CONSTEXPR Piece whiteHorse    () { return Piece(PieceNumber::WHorse    ); }
  static CONSTEXPR Piece whiteDragon   () { return Piece(PieceNumber::WDragon   ); }

  /**
   * Get pieces in numerical order.
   * See PIECE_EACH macro.
   */
  CONSTEXPR Piece next() const {
    return ((SuperType::number_ == (PieceNumber::Promotion | PieceNumber::BGold) - 1U) ||
            (SuperType::number_ == (PieceNumber::Promotion | PieceNumber::WGold) - 1U) ||
            (SuperType::number_ == (PieceNumber::Promotion | PieceNumber::BKing) - 1U)) ?
           SuperType::number_ + 2U : SuperType::number_ + 1U;
  }

  /**
   * Get a piece name(CSA format).
   */
  std::string toString() const;

  /**
   * Get piece from the specified piece name(CSA format).
   */
  static Piece parse(const char* str);

};

template <class T>
inline
CONSTEXPR Piece AbstractPieceType<T>::black() const {
  return Piece(number_);
}

template <class T>
inline
CONSTEXPR Piece AbstractPieceType<T>::white() const {
  return Piece(number_ | PieceNumber::White);
}

} // namespace sunfish

#define PIECE_EACH(piece)        for (sunfish::Piece (piece) = sunfish::PieceNumber::Begin; (piece) != sunfish::PieceNumber::End; (piece) = (piece).next())
#define PIECE_TYPE_EACH(piece)   for (sunfish::PieceType (piece) = sunfish::PieceNumber::TypeBegin; (piece) != sunfish::PieceNumber::TypeEnd; (piece) = (piece).next())
#define HAND_EACH(piece)         for (sunfish::PieceType (piece) = sunfish::PieceNumber::HandBegin; (piece) != sunfish::PieceNumber::HandEnd; (piece) = (piece).nextUnsafe())

inline std::ostream& operator<<(std::ostream& os, const sunfish::PieceType& piece) {
  os << piece.toString();
  return os;
}

inline std::ostream& operator<<(std::ostream& os, const sunfish::Piece& piece) {
  os << piece.toString();
  return os;
}

#endif // SUNFISH_CORE_BASE_PIECE__
