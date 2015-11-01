/* Position.cpp
 *
 * Kubo Ryosuke
 */

#include "Position.hpp"

namespace {

using namespace sunfish;

#define EM Piece::Empty
#define BP Piece::BPawn
#define BL Piece::BLance
#define BN Piece::BKnight
#define BS Piece::BSilver
#define BG Piece::BGold
#define BB Piece::BBishop
#define BR Piece::BRook
#define BK Piece::BKing
#define WP Piece::WPawn
#define WL Piece::WLance
#define WN Piece::WKnight
#define WS Piece::WSilver
#define WG Piece::WGold
#define WB Piece::WBishop
#define WR Piece::WRook
#define WK Piece::WKing

Piece EmptyBoardArray[Square::N] = {
  EM, EM, EM, EM, EM, EM, EM, EM, EM,
  EM, EM, EM, EM, EM, EM, EM, EM, EM,
  EM, EM, EM, EM, EM, EM, EM, EM, EM,
  EM, EM, EM, EM, EM, EM, EM, EM, EM,
  EM, EM, EM, EM, EM, EM, EM, EM, EM,
  EM, EM, EM, EM, EM, EM, EM, EM, EM,
  EM, EM, EM, EM, EM, EM, EM, EM, EM,
  EM, EM, EM, EM, EM, EM, EM, EM, EM,
  EM, EM, EM, EM, EM, EM, EM, EM, EM,
};

Piece EvenBoardArray[Square::N] = {
  WL, EM, WP, EM, EM, EM, BP, EM, BL,
  WN, WR, WP, EM, EM, EM, BP, BB, BN,
  WS, EM, WP, EM, EM, EM, BP, EM, BS,
  WG, EM, WP, EM, EM, EM, BP, EM, BG,
  WK, EM, WP, EM, EM, EM, BP, EM, BK,
  WG, EM, WP, EM, EM, EM, BP, EM, BG,
  WS, EM, WP, EM, EM, EM, BP, EM, BS,
  WN, WB, WP, EM, EM, EM, BP, BR, BN,
  WL, EM, WP, EM, EM, EM, BP, EM, BL,
};

#undef EM
#undef BP
#undef BL
#undef BN
#undef BS
#undef BG
#undef BB
#undef BR
#undef BK
#undef WP
#undef WL
#undef WN
#undef WS
#undef WG
#undef WB
#undef WR
#undef WK

}

namespace sunfish {

Position::Position() {
  initialize(EmptyBoardArray, true);
}

Position::Position(Handicap handicap) {
  initialize(handicap);
}

void Position::initialize(Handicap handicap) {
  SQUARE_EACH(square) {
    board_[square.raw()] = EvenBoardArray[square.raw()];
  }

  HAND_EACH(piece) {
    blackHand_.set(piece, 0);
    whiteHand_.set(piece, 0);
  }

  if (handicap == Handicap::TwoPieces) {
    board_[Square::S22] = Piece::Empty;
    board_[Square::S82] = Piece::Empty;
    blackTurn_ = false;
  } else {
    blackTurn_ = true;
  }

  onBoardArrayChanged();
}

void Position::onBoardArrayChanged() {

  operateEachBitboard([](Bitboard& bb) {
    bb = Bitboard::zero();
  });
  bbRotatedR90_ = RotatedBitboard::zero();
  bbRotatedRR45_ = RotatedBitboard::zero();
  bbRotatedRL45_ = RotatedBitboard::zero();

  blackKingSquare_ = Square::Invalid;
  whiteKingSquare_ = Square::Invalid;

  // generate occupied bitboard
  SQUARE_EACH(square) {

    auto piece = board_[square.raw()];
    if (!piece.isEmpty()) {

      if (piece == Piece::BKing) {
        // black king
        blackKingSquare_ = square;

      } else if (piece == Piece::WKing) {
        // white king
        whiteKingSquare_ = square;

      } else {
        // other piece type
        getBitboard(piece).set(square);

      }

      // rotated bitboard
      if (piece.isBlack()) {
        bbBRotated_.set(square);
      } else {
        bbWRotated_.set(square);
      }
      bbRotatedR90_.set(square.rotate90());
      bbRotatedRR45_.set(square.rotateRight45());
      bbRotatedRL45_.set(square.rotateLeft45());

    }
  }
}

Bitboard& Position::getBitboard(Piece piece) {
  switch (piece.raw()) {
  case Piece::BPawn     : return bbBPawn_;
  case Piece::BLance    : return bbBLance_;
  case Piece::BKnight   : return bbBKnight_;
  case Piece::BSilver   : return bbBSilver_;
  case Piece::BGold     : return bbBGold_;
  case Piece::BBishop   : return bbBBishop_;
  case Piece::BRook     : return bbBRook_;
  case Piece::BTokin    : return bbBTokin_;
  case Piece::BProLance : return bbBProLance_;
  case Piece::BProKnight: return bbBProKnight_;
  case Piece::BProSilver: return bbBProSilver_;
  case Piece::BHorse    : return bbBHorse_;
  case Piece::BDragon   : return bbBDragon_;
  case Piece::WPawn     : return bbWPawn_;
  case Piece::WLance    : return bbWLance_;
  case Piece::WKnight   : return bbWKnight_;
  case Piece::WSilver   : return bbWSilver_;
  case Piece::WGold     : return bbWGold_;
  case Piece::WBishop   : return bbWBishop_;
  case Piece::WRook     : return bbWRook_;
  case Piece::WTokin    : return bbWTokin_;
  case Piece::WProLance : return bbWProLance_;
  case Piece::WProKnight: return bbWProKnight_;
  case Piece::WProSilver: return bbWProSilver_;
  case Piece::WHorse    : return bbWHorse_;
  case Piece::WDragon   : return bbWDragon_;
  }

  assert(false);
}

}
