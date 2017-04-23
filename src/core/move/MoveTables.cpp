/* MoveTables.cpp
 *
 * Kubo Ryosuke
 */

#include "core/move/MoveTables.hpp"

namespace {

using namespace sunfish;

inline
bool isFirstQuadWord(const Square& square) {
  return square.raw() < Bitboard::Width1;
}

inline
SquareRawType verLineOffset(const Square& square) {
  return (square.raw() / 9) * 9 + 1;
}

inline
SquareRawType verLineOffsetOfFirstQuadWord(const Square& square) {
  return (square.raw() / 9) * 9 + 1;
}

inline
SquareRawType verLineOffsetOfSecondQuadWord(const Square& square) {
  return ((square.raw() - Bitboard::Width1) / 9) * 9 + 1;
}

uint8_t HorLineOffset[NUMBER_OF_SQUARES] = {
   1,  8, 15, 22, 29, 36, 43, 50, 57,
   1,  8, 15, 22, 29, 36, 43, 50, 57,
   1,  8, 15, 22, 29, 36, 43, 50, 57,
   1,  8, 15, 22, 29, 36, 43, 50, 57,
   1,  8, 15, 22, 29, 36, 43, 50, 57,
   1,  8, 15, 22, 29, 36, 43, 50, 57,
   1,  8, 15, 22, 29, 36, 43, 50, 57,
   1,  8, 15, 22, 29, 36, 43, 50, 57,
   1,  8, 15, 22, 29, 36, 43, 50, 57,
};

uint8_t DiagRightLineOffset[NUMBER_OF_SQUARES] = {
   0,  0,  1,  2,  4,  7, 11, 16, 22,
   0,  1,  2,  4,  7, 11, 16, 22, 29,
   1,  2,  4,  7, 11, 16, 22, 29, 35,
   2,  4,  7, 11, 16, 22, 29, 35, 40,
   4,  7, 11, 16, 22, 29, 35, 40, 44,
   7, 11, 16, 22, 29, 35, 40, 44, 47,
  11, 16, 22, 29, 35, 40, 44, 47, 49,
  16, 22, 29, 35, 40, 44, 47, 49,  0,
  22, 29, 35, 40, 44, 47, 49,  0,  0,
};

uint8_t DiagLeftLineOffset[NUMBER_OF_SQUARES] = {
  22, 16, 11,  7,  4,  2,  1,  0,  0,
  29, 22, 16, 11,  7,  4,  2,  1,  0,
  35, 29, 22, 16, 11,  7,  4,  2,  1,
  40, 35, 29, 22, 16, 11,  7,  4,  2,
  44, 40, 35, 29, 22, 16, 11,  7,  4,
  47, 44, 40, 35, 29, 22, 16, 11,  7,
  49, 47, 44, 40, 35, 29, 22, 16, 11,
   0, 49, 47, 44, 40, 35, 29, 22, 16,
   0,  0, 49, 47, 44, 40, 35, 29, 22,
};

} // namespace

namespace sunfish {

MoveTables::MovableInOneStepType MoveTables::MovableInOneStep;
MoveTables::MovableInLongStepType MoveTables::MovableInLongStep;
MoveTables::OneStepTableType MoveTables::BlackKnight;
MoveTables::OneStepTableType MoveTables::WhiteKnight;
MoveTables::OneStepTableType MoveTables::BlackSilver;
MoveTables::OneStepTableType MoveTables::WhiteSilver;
MoveTables::OneStepTableType MoveTables::BlackGold;
MoveTables::OneStepTableType MoveTables::WhiteGold;
MoveTables::OneStepTableType MoveTables::King;
MoveTables::OneStepTableType MoveTables::Cross;
MoveTables::OneStepTableType MoveTables::X;
MoveTables::OneStepTableType MoveTables::Neighbor3x3;
MoveTables::OneStepTableType MoveTables::Neighbor5x5;
MoveTables::VerTableType MoveTables::BlackLance;
MoveTables::VerTableType MoveTables::WhiteLance;
MoveTables::VerTableType MoveTables::Ver;
MoveTables::HorTableType MoveTables::Hor;
MoveTables::DiagTableType MoveTables::DiagRight45;
MoveTables::DiagTableType  MoveTables::DiagLeft45;

void MoveTables::initialize() {
  initializeDirectionTable();
  initializeBitboards();
}

void MoveTables::initializeDirectionTable() {
  for (PieceRawType p = 0; p < PieceNumber::Num; p++) {
    MovableInOneStep[p] = 0x00;
    MovableInLongStep[p] = 0x00;
  }

  auto flag = [](Direction dir) {
    return static_cast<uint16_t>(0x01) << static_cast<int32_t>(dir);
  };

  // 1. one step move

  // black pawn
  MovableInOneStep[PieceNumber::BPawn] |= flag(Direction::Up);

  // black lance
  MovableInOneStep[PieceNumber::BLance] |= flag(Direction::Up);

  // black knight
  MovableInOneStep[PieceNumber::BKnight] |= flag(Direction::LeftUpKnight);
  MovableInOneStep[PieceNumber::BKnight] |= flag(Direction::RightUpKnight);

  // black silver
  MovableInOneStep[PieceNumber::BSilver] |= flag(Direction::LeftUp);
  MovableInOneStep[PieceNumber::BSilver] |= flag(Direction::Up);
  MovableInOneStep[PieceNumber::BSilver] |= flag(Direction::RightUp);
  MovableInOneStep[PieceNumber::BSilver] |= flag(Direction::LeftDown);
  MovableInOneStep[PieceNumber::BSilver] |= flag(Direction::RightDown);

  // black gold
  MovableInOneStep[PieceNumber::BGold] |= flag(Direction::LeftUp);
  MovableInOneStep[PieceNumber::BGold] |= flag(Direction::Up);
  MovableInOneStep[PieceNumber::BGold] |= flag(Direction::RightUp);
  MovableInOneStep[PieceNumber::BGold] |= flag(Direction::Left);
  MovableInOneStep[PieceNumber::BGold] |= flag(Direction::Right);
  MovableInOneStep[PieceNumber::BGold] |= flag(Direction::Down);

  // black bishop
  MovableInOneStep[PieceNumber::BBishop] |= flag(Direction::LeftUp);
  MovableInOneStep[PieceNumber::BBishop] |= flag(Direction::RightUp);
  MovableInOneStep[PieceNumber::BBishop] |= flag(Direction::LeftDown);
  MovableInOneStep[PieceNumber::BBishop] |= flag(Direction::RightDown);

  // black rook
  MovableInOneStep[PieceNumber::BRook] |= flag(Direction::Up);
  MovableInOneStep[PieceNumber::BRook] |= flag(Direction::Left);
  MovableInOneStep[PieceNumber::BRook] |= flag(Direction::Right);
  MovableInOneStep[PieceNumber::BRook] |= flag(Direction::Down);

  // black king
  MovableInOneStep[PieceNumber::BKing] |= flag(Direction::LeftUp);
  MovableInOneStep[PieceNumber::BKing] |= flag(Direction::Up);
  MovableInOneStep[PieceNumber::BKing] |= flag(Direction::RightUp);
  MovableInOneStep[PieceNumber::BKing] |= flag(Direction::Left);
  MovableInOneStep[PieceNumber::BKing] |= flag(Direction::Right);
  MovableInOneStep[PieceNumber::BKing] |= flag(Direction::LeftDown);
  MovableInOneStep[PieceNumber::BKing] |= flag(Direction::Down);
  MovableInOneStep[PieceNumber::BKing] |= flag(Direction::RightDown);

  // black golds
  MovableInOneStep[PieceNumber::BTokin] = MovableInOneStep[PieceNumber::BGold];
  MovableInOneStep[PieceNumber::BProLance] = MovableInOneStep[PieceNumber::BGold];
  MovableInOneStep[PieceNumber::BProKnight] = MovableInOneStep[PieceNumber::BGold];
  MovableInOneStep[PieceNumber::BProSilver] = MovableInOneStep[PieceNumber::BGold];

  // black horse
  MovableInOneStep[PieceNumber::BHorse] = MovableInOneStep[PieceNumber::BKing];

  // black dragon
  MovableInOneStep[PieceNumber::BDragon] = MovableInOneStep[PieceNumber::BKing];

  // white pawn
  MovableInOneStep[PieceNumber::WPawn] |= flag(Direction::Down);

  // white lance
  MovableInOneStep[PieceNumber::WLance] |= flag(Direction::Down);

  // white knight
  MovableInOneStep[PieceNumber::WKnight] |= flag(Direction::LeftDownKnight);
  MovableInOneStep[PieceNumber::WKnight] |= flag(Direction::RightDownKnight);

  // white silver
  MovableInOneStep[PieceNumber::WSilver] |= flag(Direction::LeftUp);
  MovableInOneStep[PieceNumber::WSilver] |= flag(Direction::RightUp);
  MovableInOneStep[PieceNumber::WSilver] |= flag(Direction::LeftDown);
  MovableInOneStep[PieceNumber::WSilver] |= flag(Direction::Down);
  MovableInOneStep[PieceNumber::WSilver] |= flag(Direction::RightDown);

  // white gold
  MovableInOneStep[PieceNumber::WGold] |= flag(Direction::Up);
  MovableInOneStep[PieceNumber::WGold] |= flag(Direction::Left);
  MovableInOneStep[PieceNumber::WGold] |= flag(Direction::Right);
  MovableInOneStep[PieceNumber::WGold] |= flag(Direction::LeftDown);
  MovableInOneStep[PieceNumber::WGold] |= flag(Direction::Down);
  MovableInOneStep[PieceNumber::WGold] |= flag(Direction::RightDown);

  // white bishop
  MovableInOneStep[PieceNumber::WBishop] |= flag(Direction::LeftUp);
  MovableInOneStep[PieceNumber::WBishop] |= flag(Direction::RightUp);
  MovableInOneStep[PieceNumber::WBishop] |= flag(Direction::LeftDown);
  MovableInOneStep[PieceNumber::WBishop] |= flag(Direction::RightDown);

  // white rook
  MovableInOneStep[PieceNumber::WRook] |= flag(Direction::Up);
  MovableInOneStep[PieceNumber::WRook] |= flag(Direction::Left);
  MovableInOneStep[PieceNumber::WRook] |= flag(Direction::Right);
  MovableInOneStep[PieceNumber::WRook] |= flag(Direction::Down);

  // white king
  MovableInOneStep[PieceNumber::WKing] |= flag(Direction::LeftUp);
  MovableInOneStep[PieceNumber::WKing] |= flag(Direction::Up);
  MovableInOneStep[PieceNumber::WKing] |= flag(Direction::RightUp);
  MovableInOneStep[PieceNumber::WKing] |= flag(Direction::Left);
  MovableInOneStep[PieceNumber::WKing] |= flag(Direction::Right);
  MovableInOneStep[PieceNumber::WKing] |= flag(Direction::LeftDown);
  MovableInOneStep[PieceNumber::WKing] |= flag(Direction::Down);
  MovableInOneStep[PieceNumber::WKing] |= flag(Direction::RightDown);

  // white golds
  MovableInOneStep[PieceNumber::WTokin] = MovableInOneStep[PieceNumber::WGold];
  MovableInOneStep[PieceNumber::WProLance] = MovableInOneStep[PieceNumber::WGold];
  MovableInOneStep[PieceNumber::WProKnight] = MovableInOneStep[PieceNumber::WGold];
  MovableInOneStep[PieceNumber::WProSilver] = MovableInOneStep[PieceNumber::WGold];

  // white horse
  MovableInOneStep[PieceNumber::WHorse] = MovableInOneStep[PieceNumber::WKing];

  // white dragon
  MovableInOneStep[PieceNumber::WDragon] = MovableInOneStep[PieceNumber::WKing];

  // 2. long step move

  // black lance
  MovableInLongStep[PieceNumber::BLance] = MovableInOneStep[PieceNumber::BLance];

  // black bishop
  MovableInLongStep[PieceNumber::BBishop] = MovableInOneStep[PieceNumber::BBishop];

  // black rook
  MovableInLongStep[PieceNumber::BRook] = MovableInOneStep[PieceNumber::BRook];

  // black horse
  MovableInLongStep[PieceNumber::BHorse] = MovableInOneStep[PieceNumber::BBishop];

  // black dragon
  MovableInLongStep[PieceNumber::BDragon] = MovableInOneStep[PieceNumber::BRook];

  // white lance
  MovableInLongStep[PieceNumber::WLance] = MovableInOneStep[PieceNumber::WLance];

  // white bishop
  MovableInLongStep[PieceNumber::WBishop] = MovableInOneStep[PieceNumber::WBishop];

  // white rook
  MovableInLongStep[PieceNumber::WRook] = MovableInOneStep[PieceNumber::WRook];

  // white horse
  MovableInLongStep[PieceNumber::WHorse] = MovableInOneStep[PieceNumber::WBishop];

  // white dragon
  MovableInLongStep[PieceNumber::WDragon] = MovableInOneStep[PieceNumber::WRook];
}

void MoveTables::initializeBitboards() {
  auto setIfValid = [](Bitboard& bb, const Square& square) {
    if (square.isValid()) {
      bb.set(square);
    }
  };

  SQUARE_EACH(square) {
    auto s = square.raw();

    // black knight
    BlackKnight[s] = Bitboard::zero();
    setIfValid(BlackKnight[s], square.safetyLeftUpKnight());
    setIfValid(BlackKnight[s], square.safetyRightUpKnight());

    // white knight
    WhiteKnight[s] = Bitboard::zero();
    setIfValid(WhiteKnight[s], square.safetyLeftDownKnight());
    setIfValid(WhiteKnight[s], square.safetyRightDownKnight());

    // black silver
    BlackSilver[s] = Bitboard::zero();
    setIfValid(BlackSilver[s], square.safetyLeftUp());
    setIfValid(BlackSilver[s], square.safetyUp());
    setIfValid(BlackSilver[s], square.safetyRightUp());
    setIfValid(BlackSilver[s], square.safetyLeftDown());
    setIfValid(BlackSilver[s], square.safetyRightDown());

    // white silver
    WhiteSilver[s] = Bitboard::zero();
    setIfValid(WhiteSilver[s], square.safetyLeftUp());
    setIfValid(WhiteSilver[s], square.safetyRightUp());
    setIfValid(WhiteSilver[s], square.safetyLeftDown());
    setIfValid(WhiteSilver[s], square.safetyDown());
    setIfValid(WhiteSilver[s], square.safetyRightDown());

    // black gold
    BlackGold[s] = Bitboard::zero();
    setIfValid(BlackGold[s], square.safetyLeftUp());
    setIfValid(BlackGold[s], square.safetyUp());
    setIfValid(BlackGold[s], square.safetyRightUp());
    setIfValid(BlackGold[s], square.safetyLeft());
    setIfValid(BlackGold[s], square.safetyRight());
    setIfValid(BlackGold[s], square.safetyDown());

    // white gold
    WhiteGold[s] = Bitboard::zero();
    setIfValid(WhiteGold[s], square.safetyUp());
    setIfValid(WhiteGold[s], square.safetyLeft());
    setIfValid(WhiteGold[s], square.safetyRight());
    setIfValid(WhiteGold[s], square.safetyLeftDown());
    setIfValid(WhiteGold[s], square.safetyDown());
    setIfValid(WhiteGold[s], square.safetyRightDown());

    // king
    King[s] = Bitboard::zero();
    setIfValid(King[s], square.safetyLeftUp());
    setIfValid(King[s], square.safetyUp());
    setIfValid(King[s], square.safetyRightUp());
    setIfValid(King[s], square.safetyLeft());
    setIfValid(King[s], square.safetyRight());
    setIfValid(King[s], square.safetyLeftDown());
    setIfValid(King[s], square.safetyDown());
    setIfValid(King[s], square.safetyRightDown());

    // cross
    Cross[s] = Bitboard::zero();
    setIfValid(Cross[s], square.safetyUp());
    setIfValid(Cross[s], square.safetyLeft());
    setIfValid(Cross[s], square.safetyRight());
    setIfValid(Cross[s], square.safetyDown());

    // x
    X[s] = Bitboard::zero();
    setIfValid(X[s], square.safetyLeftUp());
    setIfValid(X[s], square.safetyRightUp());
    setIfValid(X[s], square.safetyLeftDown());
    setIfValid(X[s], square.safetyRightDown());

    // neighborhood 3x3
    Neighbor3x3[s] = Bitboard::zero();
    setIfValid(Neighbor3x3[s], square.safetyLeftUp());
    setIfValid(Neighbor3x3[s], square.safetyUp());
    setIfValid(Neighbor3x3[s], square.safetyRightUp());
    setIfValid(Neighbor3x3[s], square.safetyLeft());
    setIfValid(Neighbor3x3[s], square);
    setIfValid(Neighbor3x3[s], square.safetyRight());
    setIfValid(Neighbor3x3[s], square.safetyLeftDown());
    setIfValid(Neighbor3x3[s], square.safetyDown());
    setIfValid(Neighbor3x3[s], square.safetyRightDown());

    // neighborhood 5x5
    Neighbor5x5[s] = Bitboard::zero();
    setIfValid(Neighbor5x5[s], square.safetyUp(2).safetyLeft(2));
    setIfValid(Neighbor5x5[s], square.safetyUp(2).safetyLeft(1));
    setIfValid(Neighbor5x5[s], square.safetyUp(2));
    setIfValid(Neighbor5x5[s], square.safetyUp(2).safetyRight(1));
    setIfValid(Neighbor5x5[s], square.safetyUp(2).safetyRight(2));
    setIfValid(Neighbor5x5[s], square.safetyUp(1).safetyLeft(2));
    setIfValid(Neighbor5x5[s], square.safetyUp(1).safetyLeft(1));
    setIfValid(Neighbor5x5[s], square.safetyUp(1));
    setIfValid(Neighbor5x5[s], square.safetyUp(1).safetyRight(1));
    setIfValid(Neighbor5x5[s], square.safetyUp(1).safetyRight(2));
    setIfValid(Neighbor5x5[s], square.safetyLeft(2));
    setIfValid(Neighbor5x5[s], square.safetyLeft(1));
    setIfValid(Neighbor5x5[s], square);
    setIfValid(Neighbor5x5[s], square.safetyRight(1));
    setIfValid(Neighbor5x5[s], square.safetyRight(2));
    setIfValid(Neighbor5x5[s], square.safetyDown(1).safetyLeft(2));
    setIfValid(Neighbor5x5[s], square.safetyDown(1).safetyLeft(1));
    setIfValid(Neighbor5x5[s], square.safetyDown(1));
    setIfValid(Neighbor5x5[s], square.safetyDown(1).safetyRight(1));
    setIfValid(Neighbor5x5[s], square.safetyDown(1).safetyRight(2));
    setIfValid(Neighbor5x5[s], square.safetyDown(2).safetyLeft(2));
    setIfValid(Neighbor5x5[s], square.safetyDown(2).safetyLeft(1));
    setIfValid(Neighbor5x5[s], square.safetyDown(2));
    setIfValid(Neighbor5x5[s], square.safetyDown(2).safetyRight(1));
    setIfValid(Neighbor5x5[s], square.safetyDown(2).safetyRight(2));

    // vertical
    for (uint32_t pattern = 0x00; pattern < 0x80; pattern++) {
      BlackLance[s][pattern] = Bitboard::zero();
      WhiteLance[s][pattern] = Bitboard::zero();
      Ver[s][pattern] = Bitboard::zero();

      for (Square to = square.safetyUp(); to.isValid(); to = to.safetyUp()) {
        BlackLance[s][pattern].set(to);
        Ver[s][pattern].set(to);
        if (to.safetyUp().isValid() && (pattern & (0x01 << (to.raw() - verLineOffset(to))))) {
          break;
        }
      }

      for (Square to = square.safetyDown(); to.isValid(); to = to.safetyDown()) {
        WhiteLance[s][pattern].set(to);
        Ver[s][pattern].set(to);
        if (to.safetyDown().isValid() && (pattern & (0x01 << (to.raw() - verLineOffset(to))))) {
          break;
        }
      }
    }

    // horizontal
    for (uint32_t pattern = 0x00; pattern < 0x80; pattern++) {
      auto offset = HorLineOffset[s];

      Hor[s][pattern] = Bitboard::zero();

      for (Square to = square.safetyLeft(); to.isValid(); to = to.safetyLeft()) {
        Hor[s][pattern].set(to);
        if (to.safetyLeft().isValid() && (pattern & (0x01 << (to.rotate90().raw() - offset)))) {
          break;
        }
      }

      for (Square to = square.safetyRight(); to.isValid(); to = to.safetyRight()) {
        Hor[s][pattern].set(to);
        if (to.safetyRight().isValid() && (pattern & (0x01 << (to.rotate90().raw() - offset)))) {
          break;
        }
      }
    }

    // right-45 diagonal
    for (uint32_t pattern = 0x00; pattern < 0x80; pattern++) {
      auto offset = DiagRightLineOffset[s];

      DiagRight45[s][pattern] = Bitboard::zero();

      for (Square to = square.safetyRightUp(); to.isValid(); to = to.safetyRightUp()) {
        DiagRight45[s][pattern].set(to);
        if (to.safetyRightUp().isValid() && (pattern & (0x01 << (to.rotateRight45().raw() - offset)))) {
          break;
        }
      }

      for (Square to = square.safetyLeftDown(); to.isValid(); to = to.safetyLeftDown()) {
        DiagRight45[s][pattern].set(to);
        if (to.safetyLeftDown().isValid() && (pattern & (0x01 << (to.rotateRight45().raw() - offset)))) {
          break;
        }
      }
    }

    // left-45 diagonal
    for (uint32_t pattern = 0x00; pattern < 0x80; pattern++) {
      auto offset = DiagLeftLineOffset[s];

      DiagLeft45[s][pattern] = Bitboard::zero();

      for (Square to = square.safetyRightDown(); to.isValid(); to = to.safetyRightDown()) {
        DiagLeft45[s][pattern].set(to);
        if (to.safetyRightDown().isValid() && (pattern & (0x01 << (to.rotateLeft45().raw() - offset)))) {
          break;
        }
      }

      for (Square to = square.safetyLeftUp(); to.isValid(); to = to.safetyLeftUp()) {
        DiagLeft45[s][pattern].set(to);
        if (to.safetyLeftUp().isValid() && (pattern & (0x01 << (to.rotateLeft45().raw() - offset)))) {
          break;
        }
      }
    }
  }
}

const Bitboard& MoveTables::blackLance(const Bitboard& occ, const Square& square) {
  if (isFirstQuadWord(square)) {
    auto offset = verLineOffsetOfFirstQuadWord(square);
    auto pattern = (occ.first() >> offset) & 0x7f;
    return BlackLance[square.raw()][pattern];
  } else {
    auto offset = verLineOffsetOfSecondQuadWord(square);
    auto pattern = (occ.second() >> offset) & 0x7f;
    return BlackLance[square.raw()][pattern];
  }
}

const Bitboard& MoveTables::whiteLance(const Bitboard& occ, const Square& square) {
  if (isFirstQuadWord(square)) {
    auto offset = verLineOffsetOfFirstQuadWord(square);
    auto pattern = (occ.first() >> offset) & 0x7f;
    return WhiteLance[square.raw()][pattern];
  } else {
    auto offset = verLineOffsetOfSecondQuadWord(square);
    auto pattern = (occ.second() >> offset) & 0x7f;
    return WhiteLance[square.raw()][pattern];
  }
}

const Bitboard& MoveTables::ver(const Bitboard& occ, const Square& square) {
  if (isFirstQuadWord(square)) {
    auto offset = verLineOffsetOfFirstQuadWord(square);
    auto pattern = (occ.first() >> offset) & 0x7f;
    return Ver[square.raw()][pattern];
  } else {
    auto offset = verLineOffsetOfSecondQuadWord(square);
    auto pattern = (occ.second() >> offset) & 0x7f;
    return Ver[square.raw()][pattern];
  }
}

const Bitboard& MoveTables::hor(const RotatedBitboard& occ, const Square& square) {
  auto offset = HorLineOffset[square.raw()];
  auto pattern = (occ.raw() >> offset) & 0x7f;
  return Hor[square.raw()][pattern];
}

const Bitboard& MoveTables::diagR45(const RotatedBitboard& occ, const Square& square) {
  auto offset = DiagRightLineOffset[square.raw()];
  auto pattern = (occ.raw() >> offset) & 0x7f;
  return DiagRight45[square.raw()][pattern];
}

const Bitboard& MoveTables::diagL45(const RotatedBitboard& occ, const Square& square) {
  auto offset = DiagLeftLineOffset[square.raw()];
  auto pattern = (occ.raw() >> offset) & 0x7f;
  return DiagLeft45[square.raw()][pattern];
}

AggressableTables::TableType AggressableTables::BlackPawn;
AggressableTables::TableType AggressableTables::WhitePawn;
AggressableTables::TableType AggressableTables::BlackLance;
AggressableTables::TableType AggressableTables::WhiteLance;
AggressableTables::TableType AggressableTables::BlackKnight;
AggressableTables::TableType AggressableTables::WhiteKnight;
AggressableTables::TableType AggressableTables::BlackSilver;
AggressableTables::TableType AggressableTables::WhiteSilver;
AggressableTables::TableType AggressableTables::BlackBishop;
AggressableTables::TableType AggressableTables::WhiteGold;
AggressableTables::TableType AggressableTables::BlackGold;
AggressableTables::TableType AggressableTables::WhiteBishop;
AggressableTables::TableType AggressableTables::Horse;

void AggressableTables::initialize() {
  initialize(BlackPawn  , Piece::blackPawn  ());
  initialize(WhitePawn  , Piece::whitePawn  ());
  initialize(BlackLance , Piece::blackLance ());
  initialize(WhiteLance , Piece::whiteLance ());
  initialize(BlackKnight, Piece::blackKnight());
  initialize(WhiteKnight, Piece::whiteKnight());
  initialize(BlackSilver, Piece::blackSilver());
  initialize(WhiteSilver, Piece::whiteSilver());
  initialize(BlackGold  , Piece::blackGold  ());
  initialize(WhiteGold  , Piece::whiteGold  ());
  initialize(BlackBishop, Piece::blackBishop());
  initialize(WhiteBishop, Piece::whiteBishop());
  initialize(Horse      , Piece::blackHorse ());
}

void AggressableTables::initialize(TableType& table, Piece piece) {
  auto setIfValid = [](Bitboard& bb, const Square& square) {
    if (square.isValid()) {
      bb.set(square);
    }
  };

  SQUARE_EACH(target) {
    auto t = target.raw();

    table[t] = Bitboard::zero();

    SQUARE_EACH(from) {
      if (from == target) {
        continue;
      }

      DIR_EACH(dir) {
        if (!MoveTables::isMovableInOneStep(piece, dir) &&
            !MoveTables::isMovableInLongStep(piece, dir)) {
          continue;
        }

        for (Square to = from.safetyMove(dir); to.isValid(); to = to.safetyMove(dir)) {
          if (to == target) {
            break;
          }

          Direction attackDir = to.dir(target);
          auto distance = to.distance(target);

          if ((distance == 1 &&
               MoveTables::isMovableInOneStep(piece, attackDir)) ||
              MoveTables::isMovableInLongStep(piece, attackDir)) {
            setIfValid(table[t], from);
            break;
          }

          if (piece.isPromotable() &&
              ((piece.isBlack() && (to.isPromotable<Turn::Black>() || from.isPromotable<Turn::Black>())) ||
               (piece.isWhite() && (to.isPromotable<Turn::White>() || from.isPromotable<Turn::White>()))) &&
              ((distance == 1 &&
                MoveTables::isMovableInOneStep(piece.promote(), attackDir)) ||
               MoveTables::isMovableInLongStep(piece.promote(), attackDir))) {
            setIfValid(table[t], from);
            break;
          }

          if (!MoveTables::isMovableInLongStep(piece, dir)) {
            break;
          }
        }
      }
    }
  }
}

} // namespace sunfish
