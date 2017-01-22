/* Mate.cpp
 *
 * Kubo Ryosuke
 */

#include "search/mate/Mate.hpp"
#include "core/move/MoveTables.hpp"

namespace sunfish {

template <Turn turn>
bool Mate::mate1Ply(const Position& position) {
  // TODO: discovery check

  auto occ = position.getBOccupiedBitboard() | position.getWOccupiedBitboard();
  auto notSelfOcc = ~(turn == Turn::Black ? position.getBOccupiedBitboard() : position.getWOccupiedBitboard());
  auto king = turn == Turn::Black ? position.getWhiteKingSquare()
                                  : position.getBlackKingSquare();

  auto bbtGold = notSelfOcc & (turn == Turn::Black ? MoveTables::whiteGold(king)
                                                   : MoveTables::blackGold(king));

  // gold
  {
    // drop
    int handCount = turn == Turn::Black ? position.getBlackHandPieceCount(PieceType::gold())
                                        : position.getWhiteHandPieceCount(PieceType::gold());
    if (handCount) {
      auto bb = bbtGold & ~occ;
      BB_EACH(to, bb) {
        if (isMate(position, Move(PieceType::gold(), to))) {
          return true;
        }
      }
    }

    // position
    auto bb = (turn == Turn::Black ? position.getBGoldBitboard()
                                   : position.getWGoldBitboard());
    bb &= (turn == Turn::Black ? AggressableTables::blackGold(king)
                               : AggressableTables::whiteGold(king));
    BB_EACH(from, bb) {
      auto bb2 = bbtGold & (turn == Turn::Black ? MoveTables::blackGold(from)
                                                : MoveTables::whiteGold(from));
      BB_EACH(to, bb2) {
        if (isMate(position, Move(from, to, false))) {
          return true;
        }
      }
    }
  }

  auto bbtProGold = bbtGold & (turn == Turn::Black ? Bitboard::blackPromotable()
                                                   : Bitboard::whitePromotable());

  // silver
  {
    auto bbt = turn == Turn::Black ? MoveTables::whiteSilver(king)
                                   : MoveTables::blackSilver(king);
    bbt &= notSelfOcc;

    // drop
    int handCount = turn == Turn::Black ? position.getBlackHandPieceCount(PieceType::silver())
                                        : position.getWhiteHandPieceCount(PieceType::silver());
    if (handCount) {
      auto bb = bbt & ~occ;
      BB_EACH(to, bb) {
        if (isMate(position, Move(PieceType::silver(), to))) {
          return true;
        }
      }
    }

    // position
    auto bb = turn == Turn::Black ? position.getBSilverBitboard()
                                  : position.getWSilverBitboard();
    bb &= turn == Turn::Black ? AggressableTables::blackSilver(king)
                              : AggressableTables::whiteSilver(king);
    BB_EACH(from, bb) {
      auto bbe = turn == Turn::Black ? MoveTables::blackSilver(from)
                                     : MoveTables::whiteSilver(from);
      auto bb2 = bbt & bbe;
      BB_EACH(to, bb2) {
        if (isMate(position, Move(from, to, false))) {
          return true;
        }
      }

      bb2 = bbe & (from.isPromotable<turn>() ? bbtGold : bbtProGold);
      BB_EACH(to, bb2) {
        if (isMate(position, Move(from, to, true))) {
          return true;
        }
      }
    }
  }

  // knight
  {
    // drop
    auto to1 = turn == Turn::Black ? king.safetyLeftDownKnight()
                                   : king.safetyLeftUpKnight();
    auto to2 = turn == Turn::Black ? king.safetyRightDownKnight()
                                   : king.safetyRightUpKnight();
    int handCount = turn == Turn::Black ? position.getBlackHandPieceCount(PieceType::knight())
                                        : position.getWhiteHandPieceCount(PieceType::knight());
    if (handCount) {
      if (to1.isValid() && !occ.check(to1)) {
        if (isMate(position, Move(PieceType::knight(), to1))) {
          return true;
        }
      }
      if (to2.isValid() && !occ.check(to2)) {
        if (isMate(position, Move(PieceType::knight(), to2))) {
          return true;
        }
      }
    }

    // position
    auto bb = turn == Turn::Black ? position.getBKnightBitboard()
                                  : position.getWKnightBitboard();
    bb &= turn == Turn::Black ? AggressableTables::blackKnight(king)
                              : AggressableTables::whiteKnight(king);
    auto bbt = Bitboard::mask(to1) | Bitboard::mask(to2);
    bbt &= notSelfOcc;
    BB_EACH(from, bb) {
      auto bbe = turn == Turn::Black ? MoveTables::blackKnight(from)
                                     : MoveTables::whiteKnight(from);
      auto bb2 = bbt & bbe;
      BB_EACH(to, bb2) {
        if (isMate(position, Move(from, to, false))) {
          return true;
        }
      }

      bb2 = bbtProGold & bbe;
      BB_EACH(to, bb2) {
        if (isMate(position, Move(from, to, true))) {
          return true;
        }
      }
    }
  }

  // pawn
  {
    auto to = turn == Turn::Black ? king.safetyDown() : king.safetyUp();
    if (to.isValid()) {
      auto bb = turn == Turn::Black ? position.getBPawnBitboard()
                                    : position.getWPawnBitboard();
      bb &= turn == Turn::Black ? AggressableTables::blackPawn(king)
                                : AggressableTables::whitePawn(king);
      if (turn == Turn::Black) {
        bb.rightShift64(1);
      } else {
        bb.leftShift64(1);
      }
      bb &= Bitboard::mask(to) | (turn == Turn::Black ? Bitboard::blackPromotable()
                                                      : Bitboard::whitePromotable());
      bb &= notSelfOcc;
      BB_EACH(to, bb) {
        auto from = turn == Turn::Black ? to.down() : to.up();
        if (isMate(position, Move(from, to, to.isPromotable<turn>()))) {
          return true;
        }
      }
    }
  }

  // lance
  {
    // drop
    int handCount = turn == Turn::Black ? position.getBlackHandPieceCount(PieceType::lance())
                                        : position.getWhiteHandPieceCount(PieceType::lance());
    if (handCount) {
      auto to = turn == Turn::Black ? king.safetyDown() : king.safetyUp();
      if (to.isValid() &&
          position.getPieceOnBoard(to).isEmpty() &&
          isMate(position, Move(PieceType::lance(), to))) {
        return true;
      }
    }

    // position
    auto bbt = turn == Turn::Black ? MoveTables::whiteLance(occ, king)
                                   : MoveTables::blackLance(occ, king);
    bbt &= notSelfOcc;
    auto bb = turn == Turn::Black ? position.getBLanceBitboard()
                                  : position.getWLanceBitboard();
    bb &= turn == Turn::Black ? AggressableTables::blackLance(king)
                              : AggressableTables::whiteLance(king);
    BB_EACH(from, bb) {
      auto bbe = turn == Turn::Black ? MoveTables::blackLance(occ, from)
                                     : MoveTables::whiteLance(occ, from);
      auto bb2 = bbt & bbe;
      BB_EACH(to, bb2) {
        if (isMate(position, Move(from, to, false))) {
          return true;
        }
      }

      bb2 = bbtProGold & bbe;
      BB_EACH(to, bb2) {
        if (isMate(position, Move(from, to, true))) {
          return true;
        }
      }
    }
  }

  auto occr45 = position.getRight45RotatedBitboard();
  auto occl45 = position.getLeft45RotatedBitboard();

  // bishop
  {
    // drop
    int handCount = turn == Turn::Black ? position.getBlackHandPieceCount(PieceType::bishop())
                                        : position.getWhiteHandPieceCount(PieceType::bishop());
    if (handCount) {
#define GEN_CHECK_DROP_BISHOP(dir) { \
  auto to = king.safety ## dir(); \
  if (to.isValid() && \
      position.getPieceOnBoard(to).isEmpty() && \
      isMate(position, Move(PieceType::bishop(), to))) { \
    return true; \
  } \
}
      GEN_CHECK_DROP_BISHOP(LeftUp);
      GEN_CHECK_DROP_BISHOP(LeftDown);
      GEN_CHECK_DROP_BISHOP(RightUp);
      GEN_CHECK_DROP_BISHOP(RightDown);
    }

    // position
    auto bb = turn == Turn::Black ? position.getBBishopBitboard()
                                  : position.getWBishopBitboard();
    bb &= turn == Turn::Black ? AggressableTables::blackBishop(king)
                              : AggressableTables::whiteBishop(king);
    auto bbt = MoveTables::diagR45(occr45, king)
             | MoveTables::diagL45(occl45, king);
    bbt &= notSelfOcc;
    auto bbtCross = notSelfOcc & MoveTables::cross(king);
    auto bbtProCross = bbtCross & (turn == Turn::Black ? Bitboard::blackPromotable()
                                                       : Bitboard::whitePromotable());
    BB_EACH(from, bb) {
      auto bbe = MoveTables::diagR45(occr45, from)
               | MoveTables::diagL45(occl45, from);
      auto bb2 = bbt & bbe;
      BB_EACH(to, bb2) {
        if (isMate(position, Move(from, to, to.isPromotable<turn>()))) {
          return true;
        }
      }

      bb2 = bbe & (from.isPromotable<turn>() ? bbtCross : bbtProCross);
      BB_EACH(to, bb2) {
        if (isMate(position, Move(from, to, true))) {
          return true;
        }
      }
    }
  }

  // horse
  {
    auto bbt = MoveTables::diagR45(occr45, king)
             | MoveTables::diagL45(occl45, king)
             | MoveTables::king(king);
    bbt &= notSelfOcc;
    auto bb = turn == Turn::Black ? position.getBHorseBitboard()
                                  : position.getWHorseBitboard();
    bb &= AggressableTables::horse(king);
    BB_EACH(from, bb) {
      auto bbe = MoveTables::diagR45(occr45, from)
               | MoveTables::diagL45(occl45, from)
               | MoveTables::king(from);
      auto bb2 = bbt & bbe;
      BB_EACH(to, bb2) {
        if (isMate(position, Move(from, to, false))) {
          return true;
        }
      }
    }
  }

  auto occh = position.get90RotatedBitboard();

  // rook
  {
    // drop
    int handCount = turn == Turn::Black ? position.getBlackHandPieceCount(PieceType::rook())
                                        : position.getWhiteHandPieceCount(PieceType::rook());
    if (handCount) {
#define GEN_CHECK_DROP_ROOK(dir) { \
  auto to = king.safety ## dir(); \
  if (to.isValid() && \
      position.getPieceOnBoard(to).isEmpty() && \
      isMate(position, Move(PieceType::rook(), to))) { \
    return true; \
  } \
}
      GEN_CHECK_DROP_ROOK(Left);
      GEN_CHECK_DROP_ROOK(Right);
      GEN_CHECK_DROP_ROOK(Up);
      GEN_CHECK_DROP_ROOK(Down);
    }

    // position
    auto bbt = MoveTables::ver(occ, king)
             | MoveTables::hor(occh, king);
    bbt &= notSelfOcc;
    auto bb = turn == Turn::Black ? position.getBRookBitboard()
                                  : position.getWRookBitboard();
    auto bbtX = notSelfOcc & MoveTables::x(king);
    auto bbtProX = bbtX & (turn == Turn::Black ? Bitboard::blackPromotable()
                                               : Bitboard::whitePromotable());
    BB_EACH(from, bb) {
      auto bbe = MoveTables::ver(occ, from)
               | MoveTables::hor(occh, from);
      auto bb2 = bbt & bbe;
      BB_EACH(to, bb2) {
        if (isMate(position, Move(from, to, to.isPromotable<turn>()))) {
          return true;
        }
      }

      bb2 = bbe & (from.isPromotable<turn>() ? bbtX : bbtProX);
      BB_EACH(to, bb2) {
        if (isMate(position, Move(from, to, true))) {
          return true;
        }
      }
    }
  }

  // dragon
  {
    auto bbt = MoveTables::ver(occ, king)
             | MoveTables::hor(occh, king)
             | MoveTables::king(king);
    bbt &= notSelfOcc;
    auto bb = turn == Turn::Black ? position.getBDragonBitboard()
                                  : position.getWDragonBitboard();
    BB_EACH(from, bb) {
      auto bbe = MoveTables::ver(occ, from)
               | MoveTables::hor(occh, from)
               | MoveTables::king(from);
      auto bb2 = bbt & bbe;
      BB_EACH(to, bb2) {
        if (isMate(position, Move(from, to, false))) {
          return true;
        }
      }
    }
  }

  return false;
}
template bool Mate::mate1Ply<Turn::Black>(const Position& position);
template bool Mate::mate1Ply<Turn::White>(const Position& position);

} // namespace sunfish
