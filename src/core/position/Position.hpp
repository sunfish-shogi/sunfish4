/* Position.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_CORE_POSITION_POSITION_HPP__
#define SUNFISH_CORE_POSITION_POSITION_HPP__

#include "core/base/Turn.hpp"
#include "core/base/Piece.hpp"
#include "core/move/Move.hpp"
#include "core/position/Bitboard.hpp"
#include "core/position/Hand.hpp"
#include "core/position/Zobrist.hpp"

#include <array>

namespace sunfish {

class Position {
public:

  enum class Handicap {
    Even,
    TwoPieces,
  };

  /**
   * Default constructor
   */
  Position();

  /**
   * Default constructor
   */
  Position(Handicap handicap);

  /**
   * Constructor
   */
  template <class T>
  Position(const T& board, Turn turn) {
    initialize(board, turn);
  }

  /**
   * Constructor
   */
  template <class T>
  Position(const T& board, const Hand& blackHand, const Hand& whiteHand, Turn turn) {
    initialize(board, blackHand, whiteHand, turn);
  }

  /**
   * Copy constructor
   */
  Position(const Position& src) = default;

  /**
   * Initialization function
   */
  template <class T>
  void initialize(const T& board, Turn turn) {
    SQUARE_EACH(square) {
      board_[square.raw()] = board[square.raw()];
    }
    HAND_EACH(piece) {
      blackHand_.set(piece, 0);
      whiteHand_.set(piece, 0);
    }
    turn_ = turn;

    onChanged();
  }

  /**
   * Initialization function
   */
  template <class T>
  void initialize(const T& board, const Hand& blackHand, const Hand& whiteHand, Turn turn) {
    SQUARE_EACH(square) {
      board_[square.raw()] = board[square.raw()];
    }
    blackHand_ = blackHand;
    whiteHand_ = whiteHand;
    turn_ = turn;

    onChanged();
  }

  /**
   * Initialization function
   */
  void initialize(Handicap handicap);

#define POSITION_BB_GETTER(piece) \
  const Bitboard& get ## piece ## Bitboard() const { \
    return bb ## piece ## _; \
  }
  POSITION_BB_GETTER(BPawn);
  POSITION_BB_GETTER(BLance);
  POSITION_BB_GETTER(BKnight);
  POSITION_BB_GETTER(BSilver);
  POSITION_BB_GETTER(BGold);
  POSITION_BB_GETTER(BBishop);
  POSITION_BB_GETTER(BRook);
  POSITION_BB_GETTER(BTokin);
  POSITION_BB_GETTER(BProLance);
  POSITION_BB_GETTER(BProKnight);
  POSITION_BB_GETTER(BProSilver);
  POSITION_BB_GETTER(BHorse);
  POSITION_BB_GETTER(BDragon);

  POSITION_BB_GETTER(WPawn);
  POSITION_BB_GETTER(WLance);
  POSITION_BB_GETTER(WKnight);
  POSITION_BB_GETTER(WSilver);
  POSITION_BB_GETTER(WGold);
  POSITION_BB_GETTER(WBishop);
  POSITION_BB_GETTER(WRook);
  POSITION_BB_GETTER(WTokin);
  POSITION_BB_GETTER(WProLance);
  POSITION_BB_GETTER(WProKnight);
  POSITION_BB_GETTER(WProSilver);
  POSITION_BB_GETTER(WHorse);
  POSITION_BB_GETTER(WDragon);
#undef POSITION_BB_GEWTER

  /**
   * Get occupied bitboard of black pieces
   */
  const Bitboard& getBOccupiedBitboard() const {
    return bbBOccupied_;
  }

  /**
   * Get occupied bitboard of white pieces
   */
  const Bitboard& getWOccupiedBitboard() const {
    return bbWOccupied_;
  }

  /**
   * Get rotated bitboard
   */
  const RotatedBitboard& get90RotatedBitboard() const {
    return bbRotatedR90_;
  }

  /**
   * Get rotated bitboard
   */
  const RotatedBitboard& getRight45RotatedBitboard() const {
    return bbRotatedRR45_;
  }

  /**
   * Get rotated bitboard
   */
  const RotatedBitboard& getLeft45RotatedBitboard() const {
    return bbRotatedRL45_;
  }

  /**
   * Get piece of the specified square on board
   */
  Piece getPieceOnBoard(const Square& square) const {
    return board_[square.raw()];
  }

  /**
   * Get the piece count of the black hand
   */
  Hand::Type getBlackHandPieceCount(const PieceType& piece) const {
    return blackHand_.get(piece);
  }

  /**
   * Get the piece count of the white hand
   */
  Hand::Type getWhiteHandPieceCount(const PieceType& piece) const {
    return whiteHand_.get(piece);
  }

  /**
   * Get the turn of the current position.
   */
  Turn getTurn() const {
    return turn_;
  }

  /**
   * Get the hash value.
   */
  Zobrist::Type getHash() const {
    return getBoardHash() ^ getHandHash() ^ getTurnHash();
  }

  /**
   * Get the hash value of the board.
   */
  Zobrist::Type getBoardHash() const {
    return boardHash_;
  }

  /**
   * Get the hash value of hands.
   */
  Zobrist::Type getHandHash() const {
    return handHash_;
  }

  /**
   * Get the hash value of the turn.
   */
  Zobrist::Type getTurnHash() const {
    return (turn_ == Turn::Black) ? Zobrist::black() : 0x00ULL;
  }

  /**
   * Make move
   */
  bool makeMove(Move& move) {
    if (turn_ == Turn::Black) {
      return makeMove<Turn::Black>(move);
    } else {
      return makeMove<Turn::White>(move);
    }
  }

  std::string toString() const;

private:

  Bitboard& getBitboard(Piece piece);

  template <class T>
  void operateEachBitboard(T&& ope) {
    ope(bbBPawn_);
    ope(bbBLance_);
    ope(bbBKnight_);
    ope(bbBSilver_);
    ope(bbBGold_);
    ope(bbBBishop_);
    ope(bbBRook_);
    ope(bbBTokin_);
    ope(bbBProLance_);
    ope(bbBProKnight_);
    ope(bbBProSilver_);
    ope(bbBHorse_);
    ope(bbBDragon_);

    ope(bbWPawn_);
    ope(bbWLance_);
    ope(bbWKnight_);
    ope(bbWSilver_);
    ope(bbWGold_);
    ope(bbWBishop_);
    ope(bbWRook_);
    ope(bbWTokin_);
    ope(bbWProLance_);
    ope(bbWProKnight_);
    ope(bbWProSilver_);
    ope(bbWHorse_);
    ope(bbWDragon_);

    ope(bbBOccupied_);
    ope(bbWOccupied_);
  }

  template <class T>
  void operateBitboard(const Piece& piece, T&& ope) {
    switch (piece.raw()) {
    case PieceNumber::BPawn     : ope(bbBPawn_); return;
    case PieceNumber::BLance    : ope(bbBLance_); return;
    case PieceNumber::BKnight   : ope(bbBKnight_); return;
    case PieceNumber::BSilver   : ope(bbBSilver_); return;
    case PieceNumber::BGold     : ope(bbBGold_); return;
    case PieceNumber::BBishop   : ope(bbBBishop_); return;
    case PieceNumber::BRook     : ope(bbBRook_); return;
    case PieceNumber::BTokin    : ope(bbBTokin_); return;
    case PieceNumber::BProLance : ope(bbBProLance_); return;
    case PieceNumber::BProKnight: ope(bbBProKnight_); return;
    case PieceNumber::BProSilver: ope(bbBProSilver_); return;
    case PieceNumber::BHorse    : ope(bbBHorse_); return;
    case PieceNumber::BDragon   : ope(bbBDragon_); return;

    case PieceNumber::WPawn     : ope(bbWPawn_); return;
    case PieceNumber::WLance    : ope(bbWLance_); return;
    case PieceNumber::WKnight   : ope(bbWKnight_); return;
    case PieceNumber::WSilver   : ope(bbWSilver_); return;
    case PieceNumber::WGold     : ope(bbWGold_); return;
    case PieceNumber::WBishop   : ope(bbWBishop_); return;
    case PieceNumber::WRook     : ope(bbWRook_); return;
    case PieceNumber::WTokin    : ope(bbWTokin_); return;
    case PieceNumber::WProLance : ope(bbWProLance_); return;
    case PieceNumber::WProKnight: ope(bbWProKnight_); return;
    case PieceNumber::WProSilver: ope(bbWProSilver_); return;
    case PieceNumber::WHorse    : ope(bbWHorse_); return;
    case PieceNumber::WDragon   : ope(bbWDragon_); return;

    case PieceNumber::BKing:
    case PieceNumber::WKing:
      assert(false);
    }
  }

  void onChanged();

  template <Turn turn>
  bool makeMove(Move& move);

private:

  Bitboard bbBPawn_;
  Bitboard bbBLance_;
  Bitboard bbBKnight_;
  Bitboard bbBSilver_;
  Bitboard bbBGold_;
  Bitboard bbBBishop_;
  Bitboard bbBRook_;
  Bitboard bbBTokin_;
  Bitboard bbBProLance_;
  Bitboard bbBProKnight_;
  Bitboard bbBProSilver_;
  Bitboard bbBHorse_;
  Bitboard bbBDragon_;

  Bitboard bbWPawn_;
  Bitboard bbWLance_;
  Bitboard bbWKnight_;
  Bitboard bbWSilver_;
  Bitboard bbWGold_;
  Bitboard bbWBishop_;
  Bitboard bbWRook_;
  Bitboard bbWTokin_;
  Bitboard bbWProLance_;
  Bitboard bbWProKnight_;
  Bitboard bbWProSilver_;
  Bitboard bbWHorse_;
  Bitboard bbWDragon_;

  Bitboard bbBOccupied_;
  Bitboard bbWOccupied_;

  RotatedBitboard bbRotatedR90_;
  RotatedBitboard bbRotatedRR45_;
  RotatedBitboard bbRotatedRL45_;

  Square blackKingSquare_;
  Square whiteKingSquare_;

  std::array<Piece, Square::N> board_;

  Hand blackHand_;
  Hand whiteHand_;

  Turn turn_;

  Zobrist::Type boardHash_;
  Zobrist::Type handHash_;

};

}

inline std::ostream& operator<<(std::ostream& os, const sunfish::Position& position) {
  os << position.toString();
  return os;
}

#endif // SUNFISH_CORE_POSITION_POSITION_HPP__
