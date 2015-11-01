/* Position.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_CORE_POSITION_POSITION_HPP__
#define SUNFISH_CORE_POSITION_POSITION_HPP__

#include "core/base/Piece.hpp"
#include "core/position/Bitboard.hpp"
#include "core/position/Hand.hpp"

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
   * Copy constructor
   */
  Position(const Position& src) = default;

  /**
   * Initialization function
   */
  template <class T>
  void initialize(const T& board, bool blackTurn) {
    SQUARE_EACH(square) {
      board_[square.raw()] = board[square.raw()];
    }
    HAND_EACH(piece) {
      blackHand_.set(piece, 0);
      whiteHand_.set(piece, 0);
    }
    blackTurn_ = blackTurn;

    onBoardArrayChanged();
  }

  /**
   * Initialization function
   */
  template <class T>
  void initialize(const T& board, const Hand& blackHand, const Hand& whiteHand, bool blackTurn) {
    SQUARE_EACH(square) {
      board_[square.raw()] = board[square.raw()];
    }
    blackHand_ = blackHand;
    whiteHand_ = whiteHand;
    blackTurn_ = blackTurn;

    onBoardArrayChanged();
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
  Hand::ValueType getBlackHandPieceCount(const PieceType& piece) const {
    return blackHand_.get(piece);
  }

  /**
   * Get the piece count of the white hand
   */
  Hand::ValueType getWhiteHandPieceCount(const PieceType& piece) const {
    return whiteHand_.get(piece);
  }

  /**
   * Return true if the current position is black turn, return false if not.
   */
  bool isBlackTurn() const {
    return blackTurn_;
  }

  /**
   * Return true if the current position is white turn, return false if not.
   */
  bool isWhiteTurn() const {
    return !blackTurn_;
  }

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

    ope(bbBRotated_);
    ope(bbWRotated_);
  }

  void onBoardArrayChanged();

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

  Bitboard bbBRotated_;
  Bitboard bbWRotated_;

  RotatedBitboard bbRotatedR90_;
  RotatedBitboard bbRotatedRR45_;
  RotatedBitboard bbRotatedRL45_;

  Square blackKingSquare_;
  Square whiteKingSquare_;

  std::array<Piece, Square::N> board_;

  Hand blackHand_;
  Hand whiteHand_;

  bool blackTurn_;

};

}

#endif // SUNFISH_CORE_POSITION_POSITION_HPP__
