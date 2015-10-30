/* Position.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_CORE_BASE_POSITION_HPP__
#define SUNFISH_CORE_BASE_POSITION_HPP__

#include "core/base/Piece.hpp"
#include "core/position/Bitboard.hpp"

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
   * Copy constructor
   */
  Position(const Position& src) = default;

  template <class T>
  void initialize(const T& board, bool blackTurn) {
    SQUARE_EACH(square) {
      board_[square.raw()] = board[square.raw()];
    }
    blackTurn_ = blackTurn;

    onBoardArrayChanged();
  }

  void initialize(Handicap handicap);

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

  Bitboard bbBOccupied_;
  Bitboard bbWOccupied_;

  RotatedBitboard bbOccupiedR90_;
  RotatedBitboard bbOccupiedR45_;

  Square bKingSquare;
  Square wKingSquare;

  std::array<Piece, Square::N> board_;

  bool blackTurn_;

};

}

#endif // SUNFISH_CORE_BASE_POSITION_HPP__
