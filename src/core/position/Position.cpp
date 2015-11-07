/* Position.cpp
 *
 * Kubo Ryosuke
 */

#include "core/position/Position.hpp"
#include <sstream>

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
  initialize(EmptyBoardArray, Turn::Black);
}

Position::Position(Handicap handicap) {
  initialize(handicap);
}

void Position::initialize(Handicap handicap) {
  SQUARE_EACH(square) {
    board_[square.raw()] = EvenBoardArray[square.raw()];
  }

  HAND_EACH(pieceType) {
    blackHand_.set(pieceType, 0);
    whiteHand_.set(pieceType, 0);
  }

  if (handicap == Handicap::TwoPieces) {
    board_[Square::s22().raw()] = Piece::empty();
    board_[Square::s82().raw()] = Piece::empty();
    turn_ = Turn::White;
  } else {
    turn_ = Turn::Black;
  }

  onChanged();
}

void Position::onChanged() {

  // initialize
  operateEachBitboard([](Bitboard& bb) {
    bb = Bitboard::zero();
  });
  bbRotatedR90_ = RotatedBitboard::zero();
  bbRotatedRR45_ = RotatedBitboard::zero();
  bbRotatedRL45_ = RotatedBitboard::zero();

  blackKingSquare_ = Square::Invalid;
  whiteKingSquare_ = Square::Invalid;

  boardHash_ = 0x00ULL;
  handHash_ = 0x00ULL;

  // board
  SQUARE_EACH(square) {
    auto piece = board_[square.raw()];

    if (!piece.isEmpty()) {

      if (piece == Piece::blackKing()) {
        // the square of black king
        blackKingSquare_ = square;

      } else if (piece == Piece::whiteKing()) {
        // the square of white king
        whiteKingSquare_ = square;

      } else {
        // occupied bitboard
        getBitboard(piece).set(square);

      }

      // rotated bitboard
      if (piece.isBlack()) {
        bbBOccupied_.set(square);
      } else {
        bbWOccupied_.set(square);
      }
      bbRotatedR90_.set(square.rotate90());
      bbRotatedRR45_.set(square.rotateRight45());
      bbRotatedRL45_.set(square.rotateLeft45());

      // zobrist hash
      boardHash_ ^= Zobrist::board(square, piece);
    }
  }

  // hand
  HAND_EACH(pieceType) {
    {
      // zobrist hash
      auto num = blackHand_.get(pieceType);
      for (int n = 0; n < num; n++) {
        handHash_ ^= Zobrist::blackHand(pieceType, n);
      }
    }

    {
      // zobrist hash
      auto num = whiteHand_.get(pieceType);
      for (int n = 0; n < num; n++) {
        handHash_ ^= Zobrist::whiteHand(pieceType, n);
      }
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

template <Turn turn>
bool Position::makeMove(Move& move) {
  bool isDrop = move.isDrop();
  Piece piece = move.piece();
  Square to = move.to();

  assert(!piece.isEmpty());
  assert(piece.isBlack() == (turn == Turn::Black));
  assert(piece.isWhite() == (turn == Turn::White));

  if (isDrop) {
    // update piece number array
    board_[to.raw()] = piece;

    // update occupied bitboard
    auto maskTo = Bitboard::mask(to);
    operateBitboard(piece, [&maskTo](Bitboard& bb) {
      bb |= maskTo;
    });
    if (turn == Turn::Black) {
      bbBOccupied_ |= maskTo;
    } else {
      bbWOccupied_ |= maskTo;
    }
    bbRotatedR90_.set(to.rotate90());
    bbRotatedRR45_.set(to.rotateRight45());
    bbRotatedRL45_.set(to.rotateLeft45());

    PieceType pieceType = piece.type();
    Hand::Type handNum;

    // update count of pieces in hand
    if (turn == Turn::Black) {
      handNum = blackHand_.decUnsafe(pieceType);
    } else {
      handNum = whiteHand_.decUnsafe(piece.type());
    }

    // zobrist hash
    boardHash_ ^= Zobrist::board(to, piece);
    if (turn == Turn::Black) {
      handHash_ ^= Zobrist::blackHand(pieceType, handNum);
    } else {
      handHash_ ^= Zobrist::whiteHand(pieceType, handNum);
    }

  } else {
    Piece captured = board_[to.raw()];

    if (!captured.isEmpty()) {
      move.setCapturedPiece(captured);

      int handNum;
      PieceType handType = captured.type().unpromote();

      // update count of pieces in hand
      if (turn == Turn::Black) {
        handNum = blackHand_.incUnsafe(handType);
      } else {
        handNum = whiteHand_.incUnsafe(handType);
      }

      Square from = move.from();
      Piece pieceAfter = move.isPromotion() ? piece.promote() : piece;

      // update piece number array
      board_[from.raw()] = Piece::empty();
      board_[to.raw()] = pieceAfter;

      // update occupied bitboard
      auto maskFrom = Bitboard::mask(from);
      auto maskTo = Bitboard::mask(to);
      if (piece == pieceAfter) {
        operateBitboard(piece, [&maskFrom, &maskTo](Bitboard& bb) {
          bb = maskFrom.andNot(bb);
          bb |= maskTo;
        });
      } else {
        operateBitboard(piece, [&maskFrom](Bitboard& bb) {
          bb = maskFrom.andNot(bb);
        });
        operateBitboard(pieceAfter, [&maskTo](Bitboard& bb) {
          bb |= maskTo;
        });
      }
      operateBitboard(captured, [&maskTo](Bitboard& bb) {
        bb = maskTo.andNot(bb);
      });
      if (turn == Turn::Black) {
        bbBOccupied_ = maskFrom.andNot(bbBOccupied_);
        bbBOccupied_ |= maskTo;
        bbWOccupied_ = maskFrom.andNot(bbWOccupied_);
        bbWOccupied_ = maskTo.andNot(bbWOccupied_);
      } else {
        bbWOccupied_ = maskFrom.andNot(bbWOccupied_);
        bbWOccupied_ |= maskTo;
        bbBOccupied_ = maskFrom.andNot(bbBOccupied_);
        bbBOccupied_ = maskTo.andNot(bbBOccupied_);
      }
      bbRotatedR90_.unset(from.rotate90());
      bbRotatedRR45_.unset(from.rotateRight45());
      bbRotatedRL45_.unset(from.rotateLeft45());

      // zobrist hash
      boardHash_ ^= Zobrist::board(from, piece);
      boardHash_ ^= Zobrist::board(to, pieceAfter);
      boardHash_ ^= Zobrist::board(to, captured);
      if (turn == Turn::Black) {
        handHash_ ^= Zobrist::blackHand(handType, handNum - 1);
      } else {
        handHash_ ^= Zobrist::whiteHand(handType, handNum - 1);
      }

    } else {
      Square from = move.from();
      Piece pieceAfter = move.isPromotion() ? piece.promote() : piece;

      // update piece number array
      board_[from.raw()] = Piece::empty();
      board_[to.raw()] = pieceAfter;

      // update occupied bitboard
      auto maskFrom = Bitboard::mask(from);
      auto maskTo = Bitboard::mask(to);
      if (piece == pieceAfter) {
        operateBitboard(piece, [&maskFrom, &maskTo](Bitboard& bb) {
          bb = maskFrom.andNot(bb);
          bb |= maskTo;
        });
      } else {
        operateBitboard(piece, [&maskFrom](Bitboard& bb) {
          bb = maskFrom.andNot(bb);
        });
        operateBitboard(pieceAfter, [&maskTo](Bitboard& bb) {
          bb |= maskTo;
        });
      }
      if (turn == Turn::Black) {
        bbBOccupied_ = maskFrom.andNot(bbBOccupied_);
        bbBOccupied_ |= maskTo;
        bbWOccupied_ = maskFrom.andNot(bbWOccupied_);
        bbWOccupied_ = maskTo.andNot(bbWOccupied_);
      } else {
        bbWOccupied_ = maskFrom.andNot(bbWOccupied_);
        bbWOccupied_ |= maskTo;
        bbBOccupied_ = maskFrom.andNot(bbBOccupied_);
        bbBOccupied_ = maskTo.andNot(bbBOccupied_);
      }
      bbRotatedR90_.unset(from.rotate90()).set(to.rotate90());
      bbRotatedRR45_.unset(from.rotateRight45()).set(to.rotateRight45());
      bbRotatedRL45_.unset(from.rotateLeft45()).set(to.rotateLeft45());

      // zobrist hash
      boardHash_ ^= Zobrist::board(from, piece);
      boardHash_ ^= Zobrist::board(to, pieceAfter);

    }

  }

  turn_ = turn_ == Turn::Black ? Turn::White : Turn::Black;

  return true;
}
template bool Position::makeMove<Turn::Black>(Move& move);
template bool Position::makeMove<Turn::White>(Move& move);

std::string Position::toString() const {
  std::ostringstream oss;

  for (int rank = 1; rank <= 9; rank++) {
    oss << 'P' << rank;
    for (int file = 9; file >= 1; file--) {
      auto piece = getPieceOnBoard(Square(file, rank));
      if (!piece.isEmpty()) {
        oss << piece.toString();
      } else {
        oss << " * ";
      }
    }
    oss << '\n';
  }

  oss << "P+";
  HAND_EACH(pieceType) {
    auto handNum = blackHand_.get(pieceType);
    for (auto n = handNum; n > 0; n--) {
      oss << "00";
      oss << pieceType.toString();
    }
  }
  oss << '\n';

  oss << "P-";
  HAND_EACH(pieceType) {
    auto handNum = whiteHand_.get(pieceType);
    for (auto n = handNum; n > 0; n--) {
      oss << "00";
      oss << pieceType.toString();
    }
  }
  oss << '\n';

  if (turn_ == Turn::Black) {
    oss << "+\n";
  } else {
    oss << "-\n";
  }

  return oss.str();
}

}
