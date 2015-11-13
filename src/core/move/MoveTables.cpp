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
MoveTables::OneStepTableType MoveTables::King;
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
  }

  // black pawn
  MovableInOneStep[PieceNumber::BPawn] |= static_cast<uint16_t>(0x01) << static_cast<int32_t>(Direction::Up);

  // black lance
  MovableInOneStep[PieceNumber::BLance] |= static_cast<uint16_t>(0x01) << static_cast<int32_t>(Direction::Up);

  // black knight
  MovableInOneStep[PieceNumber::BKnight] |= static_cast<uint16_t>(0x01) << static_cast<int32_t>(Direction::LeftUpKnight);
  MovableInOneStep[PieceNumber::BKnight] |= static_cast<uint16_t>(0x01) << static_cast<int32_t>(Direction::RightUpKnight);

  // black silver
  MovableInOneStep[PieceNumber::BSilver] |= static_cast<uint16_t>(0x01) << static_cast<int32_t>(Direction::LeftUp);
  MovableInOneStep[PieceNumber::BSilver] |= static_cast<uint16_t>(0x01) << static_cast<int32_t>(Direction::Up);
  MovableInOneStep[PieceNumber::BSilver] |= static_cast<uint16_t>(0x01) << static_cast<int32_t>(Direction::RightUp);
  MovableInOneStep[PieceNumber::BSilver] |= static_cast<uint16_t>(0x01) << static_cast<int32_t>(Direction::LeftDown);
  MovableInOneStep[PieceNumber::BSilver] |= static_cast<uint16_t>(0x01) << static_cast<int32_t>(Direction::RightDown);

  // black gold
  MovableInOneStep[PieceNumber::BGold] |= static_cast<uint16_t>(0x01) << static_cast<int32_t>(Direction::LeftUp);
  MovableInOneStep[PieceNumber::BGold] |= static_cast<uint16_t>(0x01) << static_cast<int32_t>(Direction::Up);
  MovableInOneStep[PieceNumber::BGold] |= static_cast<uint16_t>(0x01) << static_cast<int32_t>(Direction::RightUp);
  MovableInOneStep[PieceNumber::BGold] |= static_cast<uint16_t>(0x01) << static_cast<int32_t>(Direction::Left);
  MovableInOneStep[PieceNumber::BGold] |= static_cast<uint16_t>(0x01) << static_cast<int32_t>(Direction::Right);
  MovableInOneStep[PieceNumber::BGold] |= static_cast<uint16_t>(0x01) << static_cast<int32_t>(Direction::Down);

  // black bishop
  MovableInOneStep[PieceNumber::BBishop] |= static_cast<uint16_t>(0x01) << static_cast<int32_t>(Direction::LeftUp);
  MovableInOneStep[PieceNumber::BBishop] |= static_cast<uint16_t>(0x01) << static_cast<int32_t>(Direction::RightUp);
  MovableInOneStep[PieceNumber::BBishop] |= static_cast<uint16_t>(0x01) << static_cast<int32_t>(Direction::LeftDown);
  MovableInOneStep[PieceNumber::BBishop] |= static_cast<uint16_t>(0x01) << static_cast<int32_t>(Direction::RightDown);

  // black king
  MovableInOneStep[PieceNumber::BKing] |= static_cast<uint16_t>(0x01) << static_cast<int32_t>(Direction::LeftUp);
  MovableInOneStep[PieceNumber::BKing] |= static_cast<uint16_t>(0x01) << static_cast<int32_t>(Direction::Up);
  MovableInOneStep[PieceNumber::BKing] |= static_cast<uint16_t>(0x01) << static_cast<int32_t>(Direction::RightUp);
  MovableInOneStep[PieceNumber::BKing] |= static_cast<uint16_t>(0x01) << static_cast<int32_t>(Direction::Left);
  MovableInOneStep[PieceNumber::BKing] |= static_cast<uint16_t>(0x01) << static_cast<int32_t>(Direction::Right);
  MovableInOneStep[PieceNumber::BKing] |= static_cast<uint16_t>(0x01) << static_cast<int32_t>(Direction::LeftDown);
  MovableInOneStep[PieceNumber::BKing] |= static_cast<uint16_t>(0x01) << static_cast<int32_t>(Direction::Down);
  MovableInOneStep[PieceNumber::BKing] |= static_cast<uint16_t>(0x01) << static_cast<int32_t>(Direction::RightDown);

  // black rook
  MovableInOneStep[PieceNumber::BRook] |= static_cast<uint16_t>(0x01) << static_cast<int32_t>(Direction::Up);
  MovableInOneStep[PieceNumber::BRook] |= static_cast<uint16_t>(0x01) << static_cast<int32_t>(Direction::Left);
  MovableInOneStep[PieceNumber::BRook] |= static_cast<uint16_t>(0x01) << static_cast<int32_t>(Direction::Right);
  MovableInOneStep[PieceNumber::BRook] |= static_cast<uint16_t>(0x01) << static_cast<int32_t>(Direction::Down);

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
  MovableInOneStep[PieceNumber::WPawn] |= static_cast<uint16_t>(0x01) << static_cast<int32_t>(Direction::Down);

  // white lance
  MovableInOneStep[PieceNumber::WLance] |= static_cast<uint16_t>(0x01) << static_cast<int32_t>(Direction::Down);

  // white knight
  MovableInOneStep[PieceNumber::WKnight] |= static_cast<uint16_t>(0x01) << static_cast<int32_t>(Direction::LeftDownKnight);
  MovableInOneStep[PieceNumber::WKnight] |= static_cast<uint16_t>(0x01) << static_cast<int32_t>(Direction::RightDownKnight);

  // white silver
  MovableInOneStep[PieceNumber::WSilver] |= static_cast<uint16_t>(0x01) << static_cast<int32_t>(Direction::LeftUp);
  MovableInOneStep[PieceNumber::WSilver] |= static_cast<uint16_t>(0x01) << static_cast<int32_t>(Direction::RightUp);
  MovableInOneStep[PieceNumber::WSilver] |= static_cast<uint16_t>(0x01) << static_cast<int32_t>(Direction::LeftDown);
  MovableInOneStep[PieceNumber::WSilver] |= static_cast<uint16_t>(0x01) << static_cast<int32_t>(Direction::Down);
  MovableInOneStep[PieceNumber::WSilver] |= static_cast<uint16_t>(0x01) << static_cast<int32_t>(Direction::RightDown);

  // white gold
  MovableInOneStep[PieceNumber::WGold] |= static_cast<uint16_t>(0x01) << static_cast<int32_t>(Direction::Up);
  MovableInOneStep[PieceNumber::WGold] |= static_cast<uint16_t>(0x01) << static_cast<int32_t>(Direction::Left);
  MovableInOneStep[PieceNumber::WGold] |= static_cast<uint16_t>(0x01) << static_cast<int32_t>(Direction::Right);
  MovableInOneStep[PieceNumber::WGold] |= static_cast<uint16_t>(0x01) << static_cast<int32_t>(Direction::LeftDown);
  MovableInOneStep[PieceNumber::WGold] |= static_cast<uint16_t>(0x01) << static_cast<int32_t>(Direction::Down);
  MovableInOneStep[PieceNumber::WGold] |= static_cast<uint16_t>(0x01) << static_cast<int32_t>(Direction::RightDown);

  // white bishop
  MovableInOneStep[PieceNumber::WBishop] |= static_cast<uint16_t>(0x01) << static_cast<int32_t>(Direction::LeftUp);
  MovableInOneStep[PieceNumber::WBishop] |= static_cast<uint16_t>(0x01) << static_cast<int32_t>(Direction::RightUp);
  MovableInOneStep[PieceNumber::WBishop] |= static_cast<uint16_t>(0x01) << static_cast<int32_t>(Direction::LeftDown);
  MovableInOneStep[PieceNumber::WBishop] |= static_cast<uint16_t>(0x01) << static_cast<int32_t>(Direction::RightDown);

  // white king
  MovableInOneStep[PieceNumber::WKing] |= static_cast<uint16_t>(0x01) << static_cast<int32_t>(Direction::LeftUp);
  MovableInOneStep[PieceNumber::WKing] |= static_cast<uint16_t>(0x01) << static_cast<int32_t>(Direction::Up);
  MovableInOneStep[PieceNumber::WKing] |= static_cast<uint16_t>(0x01) << static_cast<int32_t>(Direction::RightUp);
  MovableInOneStep[PieceNumber::WKing] |= static_cast<uint16_t>(0x01) << static_cast<int32_t>(Direction::Left);
  MovableInOneStep[PieceNumber::WKing] |= static_cast<uint16_t>(0x01) << static_cast<int32_t>(Direction::Right);
  MovableInOneStep[PieceNumber::WKing] |= static_cast<uint16_t>(0x01) << static_cast<int32_t>(Direction::LeftDown);
  MovableInOneStep[PieceNumber::WKing] |= static_cast<uint16_t>(0x01) << static_cast<int32_t>(Direction::Down);
  MovableInOneStep[PieceNumber::WKing] |= static_cast<uint16_t>(0x01) << static_cast<int32_t>(Direction::RightDown);

  // white rook
  MovableInOneStep[PieceNumber::WRook] |= static_cast<uint16_t>(0x01) << static_cast<int32_t>(Direction::Up);
  MovableInOneStep[PieceNumber::WRook] |= static_cast<uint16_t>(0x01) << static_cast<int32_t>(Direction::Left);
  MovableInOneStep[PieceNumber::WRook] |= static_cast<uint16_t>(0x01) << static_cast<int32_t>(Direction::Right);
  MovableInOneStep[PieceNumber::WRook] |= static_cast<uint16_t>(0x01) << static_cast<int32_t>(Direction::Down);

  // white golds
  MovableInOneStep[PieceNumber::WTokin] = MovableInOneStep[PieceNumber::WGold];
  MovableInOneStep[PieceNumber::WProLance] = MovableInOneStep[PieceNumber::WGold];
  MovableInOneStep[PieceNumber::WProKnight] = MovableInOneStep[PieceNumber::WGold];
  MovableInOneStep[PieceNumber::WProSilver] = MovableInOneStep[PieceNumber::WGold];

  // white horse
  MovableInOneStep[PieceNumber::WHorse] = MovableInOneStep[PieceNumber::WKing];

  // white dragon
  MovableInOneStep[PieceNumber::WDragon] = MovableInOneStep[PieceNumber::WKing];
}

void MoveTables::initializeBitboards() {
  auto setIfValid = [](Bitboard& bb, const Square& square) {
    if (square.isValid()) {
      bb.set(square);
    }
  };

  SQUARE_EACH(square) {
    auto s = square.raw();

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

    // vertical
    for (uint32_t pattern = 0x00; pattern < 0x80; pattern++) {
      Ver[s][pattern] = Bitboard::zero();

      for (Square to = square.safetyUp(); to.isValid(); to = to.safetyUp()) {
        Ver[s][pattern].set(to);
        if (to.safetyUp().isValid() && (pattern & (0x01 << (to.raw() - verLineOffset(to.raw()))))) {
          break;
        }
      }

      for (Square to = square.safetyDown(); to.isValid(); to = to.safetyDown()) {
        Ver[s][pattern].set(to);
        if (to.safetyDown().isValid() && (pattern & (0x01 << (to.raw() - verLineOffset(to.raw()))))) {
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

} // namespace sunfish
