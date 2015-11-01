/* Position.cpp
 *
 * Kubo Ryosuke
 */

#include "Position.hpp"

namespace {

using namespace sunfish;

#define EM Piece::empty()
#define BP Piece::blackPawn()
#define BL Piece::blackLance()
#define BN Piece::blackKnight()
#define BS Piece::blackSilver()
#define BG Piece::blackGold()
#define BB Piece::blackBishop()
#define BR Piece::blackRook()
#define BK Piece::blackKing()
#define WP Piece::whitePawn()
#define WL Piece::whiteLance()
#define WN Piece::whiteKnight()
#define WS Piece::whiteSilver()
#define WG Piece::whiteGold()
#define WB Piece::whiteBishop()
#define WR Piece::whiteRook()
#define WK Piece::whiteKing()

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
    board_[Square::s22().raw()] = Piece::empty();
    board_[Square::s82().raw()] = Piece::empty();
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

      if (piece == Piece::blackKing()) {
        // black king
        blackKingSquare_ = square;

      } else if (piece == Piece::whiteKing()) {
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
  case PieceNumber::BPawn     : return bbBPawn_;
  case PieceNumber::BLance    : return bbBLance_;
  case PieceNumber::BKnight   : return bbBKnight_;
  case PieceNumber::BSilver   : return bbBSilver_;
  case PieceNumber::BGold     : return bbBGold_;
  case PieceNumber::BBishop   : return bbBBishop_;
  case PieceNumber::BRook     : return bbBRook_;
  case PieceNumber::BTokin    : return bbBTokin_;
  case PieceNumber::BProLance : return bbBProLance_;
  case PieceNumber::BProKnight: return bbBProKnight_;
  case PieceNumber::BProSilver: return bbBProSilver_;
  case PieceNumber::BHorse    : return bbBHorse_;
  case PieceNumber::BDragon   : return bbBDragon_;
  case PieceNumber::WPawn     : return bbWPawn_;
  case PieceNumber::WLance    : return bbWLance_;
  case PieceNumber::WKnight   : return bbWKnight_;
  case PieceNumber::WSilver   : return bbWSilver_;
  case PieceNumber::WGold     : return bbWGold_;
  case PieceNumber::WBishop   : return bbWBishop_;
  case PieceNumber::WRook     : return bbWRook_;
  case PieceNumber::WTokin    : return bbWTokin_;
  case PieceNumber::WProLance : return bbWProLance_;
  case PieceNumber::WProKnight: return bbWProKnight_;
  case PieceNumber::WProSilver: return bbWProSilver_;
  case PieceNumber::WHorse    : return bbWHorse_;
  case PieceNumber::WDragon   : return bbWDragon_;
  }

  assert(false);
}

}
