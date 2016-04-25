/* Position.cpp
 *
 * Kubo Ryosuke
 */

#include "core/position/Position.hpp"
#include "core/move/MoveTables.hpp"
#include "logger/Logger.hpp"
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

template <Turn turn>
Square detectShortEffect(const Position& pos, const Square& square) {
  const auto& board = pos.getBoard();

  auto isEffected = [] (const Position::BoardArrayType& board, const Square& from, Direction dir) {
    if (!from.isValid()) {
      return false;
    }

    Piece piece = board[from.raw()];
    if ((turn == Turn::Black && !piece.isBlack()) ||
        (turn == Turn::White && !piece.isWhite())) {
      return false;
    }

    return MoveTables::isMovableInOneStep(piece, dir);
  };

  if (isEffected(board, square.safetyLeftUp(), Direction::RightDown)) {
    return square.leftUp();
  } else if (isEffected(board, square.safetyUp(), Direction::Down)) {
    return square.up();
  } else if (isEffected(board, square.safetyRightUp(), Direction::LeftDown)) {
    return square.rightUp();
  } else if (isEffected(board, square.safetyLeft(), Direction::Right)) {
    return square.left();
  } else if (isEffected(board, square.safetyRight(), Direction::Left)) {
    return square.right();
  } else if (isEffected(board, square.safetyLeftDown(), Direction::RightUp)) {
    return square.leftDown();
  } else if (isEffected(board, square.safetyDown(), Direction::Up)) {
    return square.down();
  } else if (isEffected(board, square.safetyRightDown(), Direction::LeftUp)) {
    return square.rightDown();
  }

  if (turn == Turn::Black) {
    Square from = square.safetyLeftDownKnight();
    if (from.isValid() && board[from.raw()] == Piece::blackKnight()) {
      return from;
    }

    from = square.safetyRightDownKnight();
    if (from.isValid() && board[from.raw()] == Piece::blackKnight()) {
      return from;
    }

  } else {
    Square from = square.safetyLeftUpKnight();
    if (from.isValid() && board[from.raw()] == Piece::whiteKnight()) {
      return from;
    }

    from = square.safetyRightUpKnight();
    if (from.isValid() && board[from.raw()] == Piece::whiteKnight()) {
      return from;
    }

  }

  return Square::invalid();
}

enum class LongEffectType {
  Ver, Hor, DiagRight, DiagLeft
};

template <Turn turn, LongEffectType type>
static Square detectLongEffect(const Position& pos, const Square& to) {
  const auto& board = pos.getBoard();
  const auto& attacher = turn == Turn::Black ? pos.getBOccupiedBitboard()
                                             : pos.getWOccupiedBitboard();
  const auto& maskShort = MoveTables::king(to);
  Bitboard bb;

  if (type == LongEffectType::Ver) {
    const auto& occ = pos.getBOccupiedBitboard() | pos.getWOccupiedBitboard();
    bb = maskShort.andNot(MoveTables::ver(occ, to) & attacher);
  } else if (type == LongEffectType::Hor) {
    const auto& bbRotated90 = pos.get90RotatedBitboard();
    bb = maskShort.andNot(MoveTables::hor(bbRotated90, to) & attacher);
  } else if (type == LongEffectType::DiagRight) {
    const auto& bbRotatedR45 = pos.getRight45RotatedBitboard();
    bb = maskShort.andNot(MoveTables::diagR45(bbRotatedR45, to) & attacher);
  } else if (type == LongEffectType::DiagLeft) {
    const auto& bbRotatedL45 = pos.getLeft45RotatedBitboard();
    bb = maskShort.andNot(MoveTables::diagL45(bbRotatedL45, to) & attacher);
  }

  BB_EACH(from, bb) {
    Piece piece = board[from.raw()];
    ASSERT(!piece.isEmpty());

    ASSERT(turn != Turn::Black || piece.isBlack());
    ASSERT(turn != Turn::White || piece.isWhite());

    if (turn == Turn::Black) {
      if (type == LongEffectType::Ver) {
        // rook or dragon
        if (piece == Piece::blackRook() || piece == Piece::blackDragon()) {
          return from;
        }

        // lance
        if (from.raw() > to.raw() && piece == Piece::blackLance()) {
          return from;
        }

      } else if (type == LongEffectType::Hor) {
        // rook or dragon
        if (piece == Piece::blackRook() || piece == Piece::blackDragon()) {
          return from;
        }

      } else if (type == LongEffectType::DiagRight || type == LongEffectType::DiagLeft) {
        // bishop or horse
        if (piece == Piece::blackBishop() || piece == Piece::blackHorse()) {
          return from;
        }

      }

    } else {
      if (type == LongEffectType::Ver) {
        // rook or dragon
        if (piece == Piece::whiteRook() || piece == Piece::whiteDragon()) {
          return from;
        }

        // lance
        if (from.raw() < to.raw() && piece == Piece::whiteLance()) {
          return from;
        }

      } else if (type == LongEffectType::Hor) {
        // rook or dragon
        if (piece == Piece::whiteRook() || piece == Piece::whiteDragon()) {
          return from;
        }

      } else if (type == LongEffectType::DiagRight || type == LongEffectType::DiagLeft) {
        // bishop or horse
        if (piece == Piece::whiteBishop() || piece == Piece::whiteHorse()) {
          return from;
        }

      }

    }
  }

  return Square::invalid();
}

template <Turn turn, LongEffectType type>
bool isPinned(const Position& pos, Square square) {
  Bitboard bb;
  const auto& occ = pos.getBOccupiedBitboard() | pos.getWOccupiedBitboard();
  if (type == LongEffectType::Ver) {
    bb = MoveTables::ver(occ, square) & occ;
  } else if (type == LongEffectType::Hor) {
    const auto& bbRotated90 = pos.get90RotatedBitboard();
    bb = MoveTables::hor(bbRotated90, square) & occ;
  } else if (type == LongEffectType::DiagRight) {
    const auto& bbRotatedR45 = pos.getRight45RotatedBitboard();
    bb = MoveTables::diagR45(bbRotatedR45, square) & occ;
  } else if (type == LongEffectType::DiagLeft) {
    const auto& bbRotatedL45 = pos.getLeft45RotatedBitboard();
    bb = MoveTables::diagL45(bbRotatedL45, square) & occ;
  }

  auto square1 = Square(bb.pickForward());
  auto square2 = Square(bb.pickForward());
  if (!square1.isValid() || !square2.isValid()) {
    return false;
  }

  auto piece1 = pos.getPieceOnBoard(square1);
  auto piece2 = pos.getPieceOnBoard(square2);

  Piece myKing = turn == Turn::Black ? Piece::blackKing() : Piece::whiteKing();
  Piece piece0;
  Square square0;
  if (piece1 == myKing) {
    piece0 = piece2;
    square0 = square2;
  } else if (piece2 == myKing) {
    piece0 = piece1;
    square0 = square1;
  } else {
    return false;
  }

  if (turn == Turn::Black) {
    if (type == LongEffectType::Ver) {
      if (piece0 == Piece::whiteRook() || piece0 == Piece::whiteDragon()) {
        return true;
      }
      if (square0.raw() < square.raw() && piece0 == Piece::whiteLance()) {
        return true;
      }
    } else if (type == LongEffectType::Hor) {
      if (piece0 == Piece::whiteRook() || piece0 == Piece::whiteDragon()) {
        return true;
      }
    } else if (type == LongEffectType::DiagRight || type == LongEffectType::DiagLeft) {
      if (piece0 == Piece::whiteBishop() || piece0 == Piece::whiteHorse()) {
        return true;
      }
    }
  } else {
    if (type == LongEffectType::Ver) {
      if (piece0 == Piece::blackRook() || piece0 == Piece::blackDragon()) {
        return true;
      }
      if (square0.raw() > square.raw() && piece0 == Piece::blackLance()) {
        return true;
      }
    } else if (type == LongEffectType::Hor) {
      if (piece0 == Piece::blackRook() || piece0 == Piece::blackDragon()) {
        return true;
      }
    } else if (type == LongEffectType::DiagRight || type == LongEffectType::DiagLeft) {
      if (piece0 == Piece::blackBishop() || piece0 == Piece::blackHorse()) {
        return true;
      }
    }
  }

  return false;
}

} // namespace

namespace sunfish {

void initializeMutablePosition(MutablePosition& mp) {
  SQUARE_EACH(square) {
    mp.board[square.raw()] = Piece::empty();
  }

  HAND_EACH(piece) {
    mp.blackHand.set(piece, 0);
    mp.whiteHand.set(piece, 0);
  }

  mp.turn = Turn::Black;
}

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
  bbRotated90_ = RotatedBitboard::zero();
  bbRotatedR45_ = RotatedBitboard::zero();
  bbRotatedL45_ = RotatedBitboard::zero();

  blackKingSquare_ = Square::invalid();
  whiteKingSquare_ = Square::invalid();

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
      bbRotated90_.set(square.rotate90());
      bbRotatedR45_.set(square.rotateRight45());
      bbRotatedL45_.set(square.rotateLeft45());

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
        handHash_ += Zobrist::blackHand(pieceType);
      }
    }

    {
      // zobrist hash
      auto num = whiteHand_.get(pieceType);
      for (int n = 0; n < num; n++) {
        handHash_ += Zobrist::whiteHand(pieceType);
      }
    }
  }
}

MutablePosition Position::getMutablePosition() const {
  MutablePosition mp;
  SQUARE_EACH(square) {
    mp.board[square.raw()] = board_[square.raw()];
  }
  mp.blackHand = blackHand_;
  mp.whiteHand = whiteHand_;
  mp.turn = turn_;
  return mp;
}

Bitboard& Position::getBitboard(Piece piece) {
  switch (piece.raw()) {
  case PieceNumber::BPawn     : return bbBPawn_;
  case PieceNumber::BLance    : return bbBLance_;
  case PieceNumber::BKnight   : return bbBKnight_;
  case PieceNumber::BSilver   : return bbBSilver_;
  case PieceNumber::BGold     : // fall through
  case PieceNumber::BTokin    : // fall through
  case PieceNumber::BProLance : // fall through
  case PieceNumber::BProKnight: // fall through
  case PieceNumber::BProSilver: return bbBGold_;
  case PieceNumber::BBishop   : return bbBBishop_;
  case PieceNumber::BRook     : return bbBRook_;
  case PieceNumber::BHorse    : return bbBHorse_;
  case PieceNumber::BDragon   : return bbBDragon_;
  case PieceNumber::WPawn     : return bbWPawn_;
  case PieceNumber::WLance    : return bbWLance_;
  case PieceNumber::WKnight   : return bbWKnight_;
  case PieceNumber::WSilver   : return bbWSilver_;
  case PieceNumber::WGold     : // fall through
  case PieceNumber::WTokin    : // fall through
  case PieceNumber::WProLance : // fall through
  case PieceNumber::WProKnight: // fall through
  case PieceNumber::WProSilver: return bbWGold_;
  case PieceNumber::WBishop   : return bbWBishop_;
  case PieceNumber::WRook     : return bbWRook_;
  case PieceNumber::WHorse    : return bbWHorse_;
  case PieceNumber::WDragon   : return bbWDragon_;
  }

  ASSERT(false);
  return *static_cast<Bitboard*>(nullptr);
}

template <Turn turn>
bool Position::hasPawnInFile(int file) const {
  const auto& pawn = turn == Turn::Black ?  bbBPawn_ : bbWPawn_;
  return pawn.checkFile(file);
}
template bool Position::hasPawnInFile<Turn::Black>(int) const;
template bool Position::hasPawnInFile<Turn::White>(int) const;

template <Turn turn>
bool Position::isLegalMoveMaybe(Move move, const CheckState& checkState) const {
  Square to = move.to();
  bool isDrop = move.isDrop();
  bool isKing = false;

  if (isDrop) {
    if (isDoubleCheck(checkState)) {
      return false;
    }

    PieceType pieceType = move.droppingPieceType();

    int handNum = turn == Turn::Black ?
        blackHand_.get(pieceType) :
        whiteHand_.get(pieceType);
    if (handNum == 0) {
      return false;
    }

    if (!board_[to.raw()].isEmpty()) {
      return false;
    }

    if (pieceType == PieceType::pawn() &&
        hasPawnInFile<turn>(to.getFile())) {
      return false;
    }

    ASSERT(!(turn == Turn::Black &&
             (pieceType == PieceType::pawn() ||
              pieceType == PieceType::lance()) &&
             to.getRank() == 1));
    ASSERT(!(turn == Turn::White &&
             (pieceType == PieceType::pawn() ||
              pieceType == PieceType::lance()) &&
             to.getRank() == 9));
    ASSERT(!(turn == Turn::Black &&
             pieceType == PieceType::knight() &&
             to.getRank() <= 2));
    ASSERT(!(turn == Turn::White &&
             pieceType == PieceType::knight() &&
             to.getRank() >= 8));

  } else {
    Square from = move.from();
    Piece piece = board_[from.raw()];

    isKing = piece == (turn == Turn::Black ? Piece::blackKing() : Piece::whiteKing());

    if (isDoubleCheck(checkState)) {
      if (!isKing) {
        return false;
      }
    } else {
      if (turn == Turn::Black ? !piece.isBlack() : !piece.isWhite()) {
        return false;
      }
    }

    Piece captured = board_[to.raw()];

    if (turn == Turn::Black ? captured.isBlack() : captured.isWhite()) {
      return false;
    }

    Direction dir = from.dir(to);

    ASSERT(dir != Direction::None);
    ASSERT(from.distance(to) != 0);

    if (from.distance(to) == 1) {
      if (!MoveTables::isMovableInOneStep(piece, dir)) {
        return false;
      }
    } else {
      if (!MoveTables::isMovableInLongStep(piece, dir)) {
        return false;
      }

      // TODO: use rotated bitboard
      for (Square square = from.move(dir); square != to; square = square.move(dir)) {
        ASSERT(square.isValid());
        if (!board_[square.raw()].isEmpty()) {
          return false;
        }
      }
    }

    if (move.isPromotion() && !piece.isPromotable()) {
      return false;
    }
  }

  if (sunfish::isCheck(checkState) &&
      !isDoubleCheck(checkState) &&
      !isKing) {
    Square kingSquare = turn == Turn::Black ? blackKingSquare_ : whiteKingSquare_;

    Direction targetDir = kingSquare.dir(to);
    Direction checkDir = kingSquare.dir(checkState.from1);

    if (targetDir != checkDir) {
      return false;
    }

    auto targetDistance = kingSquare.distance(to);
    auto checkDistance = kingSquare.distance(checkState.from1);

    if (targetDistance > checkDistance) {
      return false;
    }
  }

  return true;
}
template bool Position::isLegalMoveMaybe<Turn::Black>(Move, const CheckState&) const;
template bool Position::isLegalMoveMaybe<Turn::White>(Move, const CheckState&) const;

template <Turn turn>
bool Position::doMove(Move move, Piece& wbCaptured) {
  bool isDrop = move.isDrop();
  Square to = move.to();
  Piece captured;

  if (isDrop) {
    PieceType pieceType = move.droppingPieceType();
    Piece piece = turn == Turn::Black ? pieceType.black() : pieceType.white();
    captured = Piece::empty();

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
    bbRotated90_.set(to.rotate90());
    bbRotatedR45_.set(to.rotateRight45());
    bbRotatedL45_.set(to.rotateLeft45());

    // update count of pieces in hand
    if (turn == Turn::Black) {
      blackHand_.decUnsafe(pieceType);
    } else {
      whiteHand_.decUnsafe(pieceType);
    }

    // zobrist hash
    boardHash_ ^= Zobrist::board(to, piece);
    if (turn == Turn::Black) {
      handHash_ -= Zobrist::blackHand(pieceType);
    } else {
      handHash_ -= Zobrist::whiteHand(pieceType);
    }

  } else {
    Square from = move.from();
    Piece piece = board_[from.raw()];
    captured = board_[to.raw()];

    if (!captured.isEmpty()) {
      PieceType handType = captured.type().unpromote();

      // update count of pieces in hand
      if (turn == Turn::Black) {
        blackHand_.incUnsafe(handType);
      } else {
        whiteHand_.incUnsafe(handType);
      }

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
        bbWOccupied_ = maskTo.andNot(bbWOccupied_);
      } else {
        bbWOccupied_ = maskFrom.andNot(bbWOccupied_);
        bbWOccupied_ |= maskTo;
        bbBOccupied_ = maskTo.andNot(bbBOccupied_);
      }
      bbRotated90_.unset(from.rotate90());
      bbRotatedR45_.unset(from.rotateRight45());
      bbRotatedL45_.unset(from.rotateLeft45());

      // zobrist hash
      boardHash_ ^= Zobrist::board(from, piece);
      boardHash_ ^= Zobrist::board(to, pieceAfter);
      boardHash_ ^= Zobrist::board(to, captured);
      if (turn == Turn::Black) {
        handHash_ += Zobrist::blackHand(handType);
      } else {
        handHash_ += Zobrist::whiteHand(handType);
      }

    } else {
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
      } else {
        bbWOccupied_ = maskFrom.andNot(bbWOccupied_);
        bbWOccupied_ |= maskTo;
      }
      bbRotated90_.unset(from.rotate90()).set(to.rotate90());
      bbRotatedR45_.unset(from.rotateRight45()).set(to.rotateRight45());
      bbRotatedL45_.unset(from.rotateLeft45()).set(to.rotateLeft45());

      // zobrist hash
      boardHash_ ^= Zobrist::board(from, piece);
      boardHash_ ^= Zobrist::board(to, pieceAfter);

    }

    if (turn == Turn::Black) {
      if (piece == Piece::blackKing()) {
        blackKingSquare_ = to;
      }
    } else {
      if (piece == Piece::whiteKing()) {
        whiteKingSquare_ = to;
      }
    }
  }

  turn_ = turn == Turn::Black ? Turn::White : Turn::Black;

  if (inCheck<turn>()) {
    undoMove(move, captured);
    return false;
  }

  wbCaptured = captured;

  return true;
}
template bool Position::doMove<Turn::Black>(Move, Piece&);
template bool Position::doMove<Turn::White>(Move, Piece&);

template <Turn turn>
void Position::undoMove(Move move, Piece captured) {
  bool isDrop = move.isDrop();
  Square to = move.to();

  if (isDrop) {
    PieceType pieceType = move.droppingPieceType();
    Piece piece = turn == Turn::Black ? pieceType.black() : pieceType.white();

    // update piece number array
    board_[to.raw()] = Piece::empty();

    // update occupied bitboard
    auto maskTo = Bitboard::mask(to);
    operateBitboard(piece, [&maskTo](Bitboard& bb) {
      bb = maskTo.andNot(bb);
    });
    if (turn == Turn::Black) {
      bbBOccupied_ = maskTo.andNot(bbBOccupied_);
    } else {
      bbWOccupied_ = maskTo.andNot(bbWOccupied_);
    }
    bbRotated90_.unset(to.rotate90());
    bbRotatedR45_.unset(to.rotateRight45());
    bbRotatedL45_.unset(to.rotateLeft45());

    // update count of pieces in hand
    if (turn == Turn::Black) {
      blackHand_.incUnsafe(pieceType);
    } else {
      whiteHand_.incUnsafe(pieceType);
    }

    // zobrist hash
    boardHash_ ^= Zobrist::board(to, piece);
    if (turn == Turn::Black) {
      handHash_ += Zobrist::blackHand(pieceType);
    } else {
      handHash_ += Zobrist::whiteHand(pieceType);
    }

  } else {
    bool isPromotion = move.isPromotion();
    Square from = move.from();
    Piece pieceAfter = board_[to.raw()];
    Piece piece = isPromotion ? pieceAfter.unpromote() : pieceAfter;

    if (!captured.isEmpty()) {
      PieceType handType = captured.type().unpromote();

      // update count of pieces in hand
      if (turn == Turn::Black) {
        blackHand_.decUnsafe(handType);
      } else {
        whiteHand_.decUnsafe(handType);
      }

      // update piece number array
      board_[from.raw()] = piece;
      board_[to.raw()] = captured;

      // update occupied bitboard
      auto maskFrom = Bitboard::mask(from);
      auto maskTo = Bitboard::mask(to);
      if (!isPromotion) {
        operateBitboard(piece, [&maskFrom, &maskTo](Bitboard& bb) {
          bb |= maskFrom;
          bb = maskTo.andNot(bb);
        });
      } else {
        operateBitboard(piece, [&maskFrom](Bitboard& bb) {
          bb |= maskFrom;
        });
        operateBitboard(pieceAfter, [&maskTo](Bitboard& bb) {
          bb = maskTo.andNot(bb);
        });
      }
      operateBitboard(captured, [&maskTo](Bitboard& bb) {
        bb |= maskTo;
      });
      if (turn == Turn::Black) {
        bbBOccupied_ |= maskFrom;
        bbBOccupied_ = maskTo.andNot(bbBOccupied_);
        bbWOccupied_ |= maskTo;
      } else {
        bbWOccupied_ |= maskFrom;
        bbWOccupied_ = maskTo.andNot(bbWOccupied_);
        bbBOccupied_ |= maskTo;
      }
      bbRotated90_.set(from.rotate90());
      bbRotatedR45_.set(from.rotateRight45());
      bbRotatedL45_.set(from.rotateLeft45());

      // zobrist hash
      boardHash_ ^= Zobrist::board(from, piece);
      boardHash_ ^= Zobrist::board(to, pieceAfter);
      boardHash_ ^= Zobrist::board(to, captured);
      if (turn == Turn::Black) {
        handHash_ -= Zobrist::blackHand(handType);
      } else {
        handHash_ -= Zobrist::whiteHand(handType);
      }

    } else {
      Piece pieceAfter = board_[to.raw()];

      // update piece number array
      board_[from.raw()] = piece;
      board_[to.raw()] = Piece::empty();

      // update occupied bitboard
      auto maskFrom = Bitboard::mask(from);
      auto maskTo = Bitboard::mask(to);
      if (!isPromotion) {
        operateBitboard(piece, [&maskFrom, &maskTo](Bitboard& bb) {
          bb |= maskFrom;
          bb = maskTo.andNot(bb);
        });
      } else {
        operateBitboard(piece, [&maskFrom](Bitboard& bb) {
          bb |= maskFrom;
        });
        operateBitboard(pieceAfter, [&maskTo](Bitboard& bb) {
          bb = maskTo.andNot(bb);
        });
      }
      if (turn == Turn::Black) {
        bbBOccupied_ |= maskFrom;
        bbBOccupied_ = maskTo.andNot(bbBOccupied_);
      } else {
        bbWOccupied_ |= maskFrom;
        bbWOccupied_ = maskTo.andNot(bbWOccupied_);
      }
      bbRotated90_.set(from.rotate90()).unset(to.rotate90());
      bbRotatedR45_.set(from.rotateRight45()).unset(to.rotateRight45());
      bbRotatedL45_.set(from.rotateLeft45()).unset(to.rotateLeft45());

      // zobrist hash
      boardHash_ ^= Zobrist::board(from, piece);
      boardHash_ ^= Zobrist::board(to, pieceAfter);

    }

    if (turn == Turn::Black) {
      if (piece == Piece::blackKing()) {
        blackKingSquare_ = from;
      }
    } else {
      if (piece == Piece::whiteKing()) {
        whiteKingSquare_ = from;
      }
    }
  }

  turn_ = turn;
}
template void Position::undoMove<Turn::Black>(Move, Piece);
template void Position::undoMove<Turn::White>(Move, Piece);

void Position::doNullMove() {
  turn_ = turn_ == Turn::Black ? Turn::White : Turn::Black;
}

void Position::undoNullMove() {
  turn_ = turn_ == Turn::Black ? Turn::White : Turn::Black;
}

template <Turn turn>
std::tuple<Square, Square> Position::detectLongEffects(const Square& square, Square from1) const {
  Square from2;

  from2 = detectLongEffect<turn, LongEffectType::Ver>(*this, square);
  if (from2.isValid()) {
    if (from1.isValid()) {
      return std::make_tuple(from1, from2);
    }
    from1 = from2;
    from2 = Square::invalid();
  }

  from2 = detectLongEffect<turn, LongEffectType::Hor>(*this, square);
  if (from2.isValid()) {
    if (from1.isValid()) {
      return std::make_tuple(from1, from2);
    }
    from1 = from2;
    from2 = Square::invalid();
  }

  from2 = detectLongEffect<turn, LongEffectType::DiagRight>(*this, square);
  if (from2.isValid()) {
    if (from1.isValid()) {
      return std::make_tuple(from1, from2);
    }
    from1 = from2;
    from2 = Square::invalid();
  }

  from2 = detectLongEffect<turn, LongEffectType::DiagLeft>(*this, square);
  if (from1.isValid()) {
    return std::make_tuple(from1, from2);
  } else {
    return std::make_tuple(from2, from1);
  }
}

template <Turn turn>
bool Position::hasLongEffect(const Square& square) const {
  Square from;

  from = detectLongEffect<turn, LongEffectType::Ver>(*this, square);
  if (from.isValid()) {
    return true;
  }

  from = detectLongEffect<turn, LongEffectType::Hor>(*this, square);
  if (from.isValid()) {
    return true;
  }

  from = detectLongEffect<turn, LongEffectType::DiagRight>(*this, square);
  if (from.isValid()) {
    return true;
  }

  from = detectLongEffect<turn, LongEffectType::DiagLeft>(*this, square);

  return from.isValid();
}

template <Turn turn>
bool Position::isDirectCheck(Move move) const {
  auto king = turn == Turn::Black ? whiteKingSquare_
                                  : blackKingSquare_;
  auto to = move.to();
  Direction dir = to.dir(king);
  auto distance = to.distance(king);
  Piece piece;
  if (move.isDrop()) {
    auto pieceType = move.droppingPieceType();
    piece = turn == Turn::Black ? pieceType.black()
                                : pieceType.white();
  } else {
    piece = board_[move.from().raw()];
    piece = move.isPromotion() ? piece.promote()
                               : piece;
  }

  if (distance == 1) {
    return MoveTables::isMovableInOneStep(piece, dir);
  }

  if (MoveTables::isMovableInLongStep(piece, dir)) {
    // TODO: use rotated bitboard
    for (Square square = to.move(dir); square != king; square = square.move(dir)) {
      ASSERT(square.isValid());
      if (!board_[square.raw()].isEmpty()) {
        return false;
      }
    }
    return true;
  }

  return false;
}

template <Turn turn, Turn enemy>
bool Position::isDiscoveryCheck(Move move) const {
  if (move.isDrop()) {
    return false;
  }

  auto king = turn == Turn::Black ? whiteKingSquare_
                                  : blackKingSquare_;
  auto from = move.from();
  auto to = move.to();
  Direction dir = from.dir(king);

  if (dir == to.dir(king)) {
    return false;
  }

  switch (dir) {
  case Direction::Up:
  case Direction::Down:
    return ::isPinned<enemy, LongEffectType::Ver>(*this, from);
  case Direction::Left:
  case Direction::Right:
    return ::isPinned<enemy, LongEffectType::Hor>(*this, from);
  case Direction::RightUp:
  case Direction::LeftDown:
    return ::isPinned<enemy, LongEffectType::DiagRight>(*this, from);
  case Direction::LeftUp:
  case Direction::RightDown:
    return ::isPinned<enemy, LongEffectType::DiagLeft>(*this, from);
  default:
    return false;
  }
}

template <Turn turn>
bool Position::isCheck(Move move) const {
  return isDirectCheck<turn>(move) ||
         isDiscoveryCheck<turn>(move);
}
template bool Position::isCheck<Turn::Black>(Move) const;
template bool Position::isCheck<Turn::White>(Move) const;

template <Turn turn>
bool Position::inCheck() const {
  if (turn == Turn::Black) {
    const Square& square = blackKingSquare_;
    return detectShortEffect<Turn::White>(*this, square).isValid()
        || hasLongEffect<Turn::White>(square);
  } else {
    const Square& square = whiteKingSquare_;
    return detectShortEffect<Turn::Black>(*this, square).isValid()
        || hasLongEffect<Turn::Black>(square);
  }
}
template bool Position::inCheck<Turn::Black>() const;
template bool Position::inCheck<Turn::White>() const;

template <Turn turn>
CheckState Position::getCheckState() const {
  const Square& square = turn == Turn::Black ? blackKingSquare_ : whiteKingSquare_;
  CheckState checkState;

  Square from;
  if (turn == Turn::Black) {
    from = detectShortEffect<Turn::White>(*this, square);
  } else {
    from = detectShortEffect<Turn::Black>(*this, square);
  }

  Square from1;
  Square from2;
  if (turn == Turn::Black) {
    std::tie(from1, from2) = detectLongEffects<Turn::White>(square, from);
  } else {
    std::tie(from1, from2) = detectLongEffects<Turn::Black>(square, from);
  }
  checkState.from1 = from1;
  checkState.from2 = from2;

  return checkState;
}
template CheckState Position::getCheckState<Turn::Black>() const;
template CheckState Position::getCheckState<Turn::White>() const;

template <Turn turn>
bool Position::isForced(const Square& square) const {
  return detectShortEffect<turn>(*this, square).isValid()
    || detectLongEffect<turn, LongEffectType::Ver>(*this, square).isValid()
    || detectLongEffect<turn, LongEffectType::Hor>(*this, square).isValid()
    || detectLongEffect<turn, LongEffectType::DiagRight>(*this, square).isValid()
    || detectLongEffect<turn, LongEffectType::DiagLeft>(*this, square).isValid();
}
template bool Position::isForced<Turn::Black>(const Square& square) const;
template bool Position::isForced<Turn::White>(const Square& square) const;

template <Turn turn>
bool Position::isPinned(const Square& square) const {
  auto kingSquare = turn == Turn::Black ? blackKingSquare_ : whiteKingSquare_;
  Direction dir = kingSquare.dir(square);

  switch (dir) {
  case Direction::Up:
  case Direction::Down:
    return ::isPinned<turn, LongEffectType::Ver>(*this, square);
  case Direction::Left:
  case Direction::Right:
    return ::isPinned<turn, LongEffectType::Hor>(*this, square);
  case Direction::RightUp:
  case Direction::LeftDown:
    return ::isPinned<turn, LongEffectType::DiagRight>(*this, square);
  case Direction::LeftUp:
  case Direction::RightDown:
    return ::isPinned<turn, LongEffectType::DiagLeft>(*this, square);
  default:
    return false;
  }
}
template bool Position::isPinned<Turn::Black>(const Square& square) const;
template bool Position::isPinned<Turn::White>(const Square& square) const;

template <Turn turn>
bool Position::isDroppable(const Bitboard& mask) const {
  const auto& hand = turn == Turn::Black ? blackHand_ : whiteHand_;
  bool has = hand.get(PieceType::silver()) != 0
          || hand.get(PieceType::gold()) != 0
          || hand.get(PieceType::bishop()) != 0
          || hand.get(PieceType::rook()) != 0;
  bool hasLance = hand.get(PieceType::lance()) != 0;
  bool hasKnight = hand.get(PieceType::knight()) != 0;
  bool hasPawn = hand.get(PieceType::pawn()) != 0;

  if (!has && !hasLance && !hasKnight && !hasPawn) {
    return false;
  }

  auto occ = bbBOccupied_ | bbWOccupied_;
  auto nooccMasked = occ.andNot(mask);

  if (hasKnight || hasLance || has) {
    Bitboard rank3to9 = turn == Turn::Black ?  Bitboard::rank3to9() : Bitboard::rank1to7();
    rank3to9 &= nooccMasked;
    if (rank3to9.first() || rank3to9.second()) {
      return true;
    }
  }

  if (hasLance || has) {
    Bitboard rank2 = turn == Turn::Black ?  Bitboard::rank2() : Bitboard::rank8();
    rank2 &= nooccMasked;
    if (rank2.first() || rank2.second()) {
      return true;
    }
  }

  if (has) {
    Bitboard rank1 = turn == Turn::Black ?  Bitboard::rank1() : Bitboard::rank9();
    rank1 &= nooccMasked;
    if (rank1.first() || rank1.second()) {
      return true;
    }
  }

  if (hasPawn) {
    Bitboard rank2to9 = turn == Turn::Black ?  Bitboard::rank2to9() : Bitboard::rank1to8();
    rank2to9 &= nooccMasked;

    const auto& pawn = turn == Turn::Black ? bbBPawn_ : bbWPawn_;
    if (pawn.checkFile(1)) { rank2to9 = Bitboard::file1().andNot(rank2to9); }
    if (pawn.checkFile(2)) { rank2to9 = Bitboard::file2().andNot(rank2to9); }
    if (pawn.checkFile(3)) { rank2to9 = Bitboard::file3().andNot(rank2to9); }
    if (pawn.checkFile(4)) { rank2to9 = Bitboard::file4().andNot(rank2to9); }
    if (pawn.checkFile(5)) { rank2to9 = Bitboard::file5().andNot(rank2to9); }
    if (pawn.checkFile(6)) { rank2to9 = Bitboard::file6().andNot(rank2to9); }
    if (pawn.checkFile(7)) { rank2to9 = Bitboard::file7().andNot(rank2to9); }
    if (pawn.checkFile(8)) { rank2to9 = Bitboard::file8().andNot(rank2to9); }
    if (pawn.checkFile(9)) { rank2to9 = Bitboard::file9().andNot(rank2to9); }

    if (rank2to9.first() || rank2to9.second()) {
      return true;
    }
  }

  return false;
}
template bool Position::isDroppable<Turn::Black>(const Bitboard&) const;
template bool Position::isDroppable<Turn::White>(const Bitboard&) const;

template <Turn turn, bool exceptKing>
bool Position::isMovable(const Bitboard& mask) const {
  // pawn
  {
    auto fbb = turn == Turn::Black ? bbBPawn_ : bbWPawn_;
    fbb &= turn == Turn::Black ? mask << 1 : mask >> 1;
    BB_EACH(from, fbb) {
      if (!isPinned<turn>(from)) {
        return true;
      }
    }
  }

  // silver
  {
    auto fbb = turn == Turn::Black ? bbBSilver_ : bbWSilver_;
    BB_EACH(from, fbb) {
      if (!isPinned<turn>(from)) {
        auto tbb = turn == Turn::Black
          ? MoveTables::blackSilver(from)
          : MoveTables::whiteSilver(from);
        tbb &= mask;
        if (tbb.first() || tbb.second()) {
          return true;
        }
      }
    }
  }

  // gold, tokin, promoted-lance, promoted-knight, promoted-siver
  {
    auto fbb = turn == Turn::Black ? bbBGold_ : bbWGold_;
    BB_EACH(from, fbb) {
      if (!isPinned<turn>(from)) {
        auto tbb = turn == Turn::Black
          ? MoveTables::blackGold(from)
          : MoveTables::whiteGold(from);
        tbb &= mask;
        if (tbb.first() || tbb.second()) {
          return true;
        }
      }
    }
  }

  static_assert(exceptKing, "not implemented");

  // bishop
  {
    auto fbb = turn == Turn::Black ? bbBBishop_ : bbWBishop_;
    BB_EACH(from, fbb) {
      if (!isPinned<turn>(from)) {
        auto tbb =
          MoveTables::diagR45(bbRotatedR45_, from) |
          MoveTables::diagL45(bbRotatedL45_, from);
        tbb &= mask;
        if (tbb.first() || tbb.second()) {
          return true;
        }
      }
    }
  }

  // horse
  {
    auto fbb = turn == Turn::Black ? bbBHorse_ : bbWHorse_;
    BB_EACH(from, fbb) {
      if (!isPinned<turn>(from)) {
        auto tbb =
          MoveTables::diagR45(bbRotatedR45_, from) |
          MoveTables::diagL45(bbRotatedL45_, from) |
          MoveTables::king(from);
        tbb &= mask;
        if (tbb.first() || tbb.second()) {
          return true;
        }
      }
    }
  }

  auto occ = bbBOccupied_ | bbWOccupied_;

  // rook
  {
    auto fbb = turn == Turn::Black ? bbBRook_ : bbWRook_;
    BB_EACH(from, fbb) {
      if (!isPinned<turn>(from)) {
        auto tbb =
          MoveTables::ver(occ, from) |
          MoveTables::hor(bbRotated90_, from);
        tbb &= mask;
        if (tbb.first() || tbb.second()) {
          return true;
        }
      }
    }
  }

  // dragon
  {
    auto fbb = turn == Turn::Black ? bbBDragon_ : bbWDragon_;
    BB_EACH(from, fbb) {
      if (!isPinned<turn>(from)) {
        auto tbb =
          MoveTables::ver(occ, from) |
          MoveTables::hor(bbRotated90_, from) |
          MoveTables::king(from);
        tbb &= mask;
        if (tbb.first() || tbb.second()) {
          return true;
        }
      }
    }
  }

  // lance
  {
    auto fbb = turn == Turn::Black ? bbBLance_ : bbWLance_;
    BB_EACH(from, fbb) {
      if (!isPinned<turn>(from)) {
        auto tbb = turn == Turn::Black
          ? MoveTables::blackLance(occ, from)
          : MoveTables::whiteLance(occ, from);
        tbb &= mask;
        if (tbb.first() || tbb.second()) {
          return true;
        }
      }
    }
  }

  // knight
  {
    auto fbb = turn == Turn::Black ? bbBKnight_ : bbWKnight_;
    BB_EACH(from, fbb) {
      if (!isPinned<turn>(from)) {
        auto tbb = turn == Turn::Black
          ? MoveTables::blackKnight(from)
          : MoveTables::whiteKnight(from);
        tbb &= mask;
        if (tbb.first() || tbb.second()) {
          return true;
        }
      }
    }
  }

  return false;
}
template bool Position::isMovable<Turn::Black, true>(const Bitboard&) const;
template bool Position::isMovable<Turn::White, true>(const Bitboard&) const;

template <Turn turn>
bool Position::isMate(const CheckState& checkState) {
  ASSERT(sunfish::isCheck(checkState));

  auto kingSquare = turn == Turn::Black ? blackKingSquare_ : whiteKingSquare_;

  if (!checkState.from2.isValid()) {
    auto from = checkState.from1;
    ASSERT(turn != Turn::Black || board_[from.raw()].isWhite());
    ASSERT(turn != Turn::White || board_[from.raw()].isBlack());

    const auto& tbb = Bitboard::mask(kingSquare, from);

    if (from.distance(kingSquare) != 1 && isDroppable<turn>(tbb)) {
      return false;
    }

    if (isMovable<turn, true>(tbb)) {
      return false;
    }
  }

  auto tbb = MoveTables::king(kingSquare);
  auto mate = true;

  if (turn == Turn::Black) {
    bbBOccupied_ = Bitboard::mask(kingSquare).andNot(bbBOccupied_);
  } else {
    bbWOccupied_ = Bitboard::mask(kingSquare).andNot(bbWOccupied_);
  }
  bbRotated90_.unset(kingSquare.rotate90());
  bbRotatedR45_.unset(kingSquare.rotateRight45());
  bbRotatedL45_.unset(kingSquare.rotateLeft45());

  BB_EACH(to, tbb) {
    if (turn == Turn::Black) {
      if (!board_[to.raw()].isBlack() && !isForced<Turn::White>(to)) {
        mate = false;
        break;
      }
    } else {
      if (!board_[to.raw()].isWhite() && !isForced<Turn::Black>(to)) {
        mate = false;
        break;
      }
    }
  }

  if (turn == Turn::Black) {
    bbBOccupied_ |= Bitboard::mask(kingSquare);
  } else {
    bbWOccupied_ |= Bitboard::mask(kingSquare);
  }
  bbRotated90_.set(kingSquare.rotate90());
  bbRotatedR45_.set(kingSquare.rotateRight45());
  bbRotatedL45_.set(kingSquare.rotateLeft45());

  return mate;
}
template bool Position::isMate<Turn::Black>(const CheckState&);
template bool Position::isMate<Turn::White>(const CheckState&);

template <Turn turn>
bool Position::isMateWithPawnDrop() {
  auto king = turn == Turn::Black ? whiteKingSquare_ : blackKingSquare_;
  auto to = turn == Turn::Black ? king.safetyDown() : king.safetyUp();

  if (!to.isValid() || !board_[to.raw()].isEmpty()) {
    return false;
  }

  // do move
  if (turn == Turn::Black) {
    bbBOccupied_ |= Bitboard::mask(to);
  } else {
    bbWOccupied_ |= Bitboard::mask(to);
  }
  bbRotated90_.set(to.rotate90());
  bbRotatedR45_.set(to.rotateRight45());
  bbRotatedL45_.set(to.rotateLeft45());
  board_[to.raw()] = turn == Turn::Black ? Piece::blackPawn() : Piece::whitePawn();

  // detect whether checkmate
  CheckState checkState;
  checkState.from1 = to;
  checkState.from2 = Square::invalid();
  bool result = turn == Turn::Black
      ? isMate<Turn::White>(checkState)
      : isMate<Turn::Black>(checkState);

  // undo move
  if (turn == Turn::Black) {
    bbBOccupied_ = Bitboard::mask(to).andNot(bbBOccupied_);
  } else {
    bbWOccupied_ = Bitboard::mask(to).andNot(bbWOccupied_);
  }
  bbRotated90_.unset(to.rotate90());
  bbRotatedR45_.unset(to.rotateRight45());
  bbRotatedL45_.unset(to.rotateLeft45());
  board_[to.raw()] = Piece::empty();

  return result;
}
template bool Position::isMateWithPawnDrop<Turn::Black>();
template bool Position::isMateWithPawnDrop<Turn::White>();

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

std::string Position::toStringSFEN() const {
  std::ostringstream oss;

  for (int rank = 1; rank <= 9; rank++) {
    int emptyCount = 0;
    for (int file = 9; file >= 1; file--) {
      auto piece = getPieceOnBoard(Square(file, rank));
      if (!piece.isEmpty()) {
        if (emptyCount != 0) {
          oss << emptyCount;
          emptyCount = 0;
        }
        oss << piece.toStringSFEN();
      } else {
        emptyCount++;
      }
    }

    if (emptyCount != 0) {
      oss << emptyCount;
    }

    if (rank != 9) {
      oss << '/';
    }
  }

  oss << ' ';

  oss << (turn_ == Turn::Black ? 'b' : 'w');

  oss << ' ';

  bool handIsEmpty = true;

  HAND_EACH(pieceType) {
    int handNum = blackHand_.get(pieceType);
    if (handNum >= 1) {
      handIsEmpty = false;
      if (handNum != 1) {
        oss << handNum;
      }
      oss << pieceType.black().toStringSFEN();
    }
  }

  HAND_EACH(pieceType) {
    int handNum = whiteHand_.get(pieceType);
    if (handNum >= 1) {
      handIsEmpty = false;
      if (handNum != 1) {
        oss << handNum;
      }
      oss << pieceType.white().toStringSFEN();
    }
  }

  if (handIsEmpty) {
    oss << '-';
  }

  oss << ' ';

  oss << '1';

  return oss.str();
}

}
