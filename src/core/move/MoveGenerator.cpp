/* MoveGenerator.cpp
 *
 * Kubo Ryosuke
 */

#include "core/move/MoveGenerator.hpp"
#include "core/move/MoveTables.hpp"

namespace sunfish {

template <Turn turn, MoveGenerator::GenerationType type>
void MoveGenerator::generateMovesOnBoard(const Position& pos, Moves& moves) {
  auto notSelfOcc = turn == Turn::Black ? ~pos.getBOccupiedBitboard() : ~pos.getWOccupiedBitboard();
  auto cap = turn == Turn::Black ? pos.getWOccupiedBitboard() : pos.getBOccupiedBitboard();
  auto prom = turn == Turn::Black ? Bitboard::blackPromotable() : Bitboard::whitePromotable();

  auto notCap = cap.andNot(notSelfOcc);
  auto capOrProm = (cap | prom) & notSelfOcc;
  auto notCapProm = (cap | prom).andNot(notSelfOcc);

  // pawn
  {
    auto fbb = turn == Turn::Black ? pos.getBPawnBitboard() : pos.getWPawnBitboard();
    if (type == GenerationType::Capturing) {
      auto mask = turn == Turn::Black ? capOrProm << 1 : capOrProm >> 1;
      fbb &= mask;
    } else {
      auto mask = turn == Turn::Black ? notCapProm << 1 : notCapProm >> 1;
      fbb &= mask;
    }

    BB_EACH(from, fbb) {
      if (turn == Turn::Black) {
        Square to = from.up();
        moves.add(Move(Piece::blackPawn(), from, to, to.isPromotable<turn>()));
      } else {
        Square to = from.down();
        moves.add(Move(Piece::whitePawn(), from, to, to.isPromotable<turn>()));
      }
    }
  }

  // silver
  {
    auto fbb = turn == Turn::Black ? pos.getBSilverBitboard() : pos.getWSilverBitboard();
    BB_EACH(from, fbb) {
      auto tbb = turn == Turn::Black
        ? MoveTables::blackSilver(from)
        : MoveTables::whiteSilver(from);
      if (type == GenerationType::Capturing) {
        tbb &= from.isPromotable<turn>() ? notSelfOcc : capOrProm;
      } else {
        tbb &= notCap;
      }

      BB_EACH(to, tbb) {
        auto piece = turn == Turn::Black ? Piece::blackSilver() : Piece::whiteSilver();
        if (from.isPromotable<turn>() || to.isPromotable<turn>()) {
          if (type == GenerationType::Capturing) {
            moves.add(Move(piece, from, to, true));
            if (cap.check(to)) {
              moves.add(Move(piece, from, to, false));
            }
          } else {
            moves.add(Move(piece, from, to, false));
          }

        } else {
          moves.add(Move(piece, from, to, false));
        }
      }
    }
  }

  // gold, tokin, promoted-lance, promoted-knight, promoted-siver
  {
    auto fbb = turn == Turn::Black ? pos.getBGoldBitboard() : pos.getWGoldBitboard();
    BB_EACH(from, fbb) {
      auto tbb = turn == Turn::Black
        ? MoveTables::blackGold(from)
        : MoveTables::whiteGold(from);
      tbb &= type == GenerationType::Capturing ? cap : notCap;

      auto piece = pos.getPieceOnBoard(from);
      BB_EACH(to, tbb) {
        moves.add(Move(piece, from, to, false));
      }
    }
  }

  // king
  {
    auto from = turn == Turn::Black ? pos.getBlackKingSquare() : pos.getWhiteKingSquare();
    auto tbb = MoveTables::king(from);
    tbb &= type == GenerationType::Capturing ? cap : notCap;

    BB_EACH(to, tbb) {
      auto piece = turn == Turn::Black ? Piece::blackKing() : Piece::whiteKing();
      moves.add(Move(piece, from, to, false));
    }
  }

  // bishop
  {
    auto fbb = turn == Turn::Black ? pos.getBBishopBitboard() : pos.getWBishopBitboard();
    if (type != GenerationType::Capturing) {
      fbb &= turn == Turn::Black ? Bitboard::blackNotPromotable() : Bitboard::whiteNotPromotable();
    }
    BB_EACH(from, fbb) {
      auto tbb =
        MoveTables::diagR45(pos.getRight45RotatedBitboard(), from) |
        MoveTables::diagL45(pos.getLeft45RotatedBitboard(), from);
      if (type == GenerationType::Capturing) {
        tbb &= from.isPromotable<turn>() ? notSelfOcc : capOrProm;
      } else {
        tbb &= notCapProm;
      }

      BB_EACH(to, tbb) {
        auto piece = turn == Turn::Black ? Piece::blackBishop() : Piece::whiteBishop();
        if (type == GenerationType::Capturing) {
          auto promotable = from.isPromotable<turn>() || to.isPromotable<turn>();
          moves.add(Move(piece, from, to, promotable));
        } else {
          moves.add(Move(piece, from, to, false));
        }
      }
    }
  }

  // horse
  {
    auto fbb = turn == Turn::Black ? pos.getBHorseBitboard() : pos.getWHorseBitboard();
    BB_EACH(from, fbb) {
      auto tbb =
        MoveTables::diagR45(pos.getRight45RotatedBitboard(), from) |
        MoveTables::diagL45(pos.getLeft45RotatedBitboard(), from) |
        MoveTables::king(from);
      tbb &= type == GenerationType::Capturing ? cap : notCap;

      BB_EACH(to, tbb) {
        auto piece = turn == Turn::Black ? Piece::blackHorse() : Piece::whiteHorse();
        moves.add(Move(piece, from, to, false));
      }
    }
  }

  auto occ = pos.getBOccupiedBitboard() | pos.getWOccupiedBitboard();

  // rook
  {
    auto fbb = turn == Turn::Black ? pos.getBRookBitboard() : pos.getWRookBitboard();
    if (type != GenerationType::Capturing) {
      fbb &= turn == Turn::Black ? Bitboard::blackNotPromotable() : Bitboard::whiteNotPromotable();
    }
    BB_EACH(from, fbb) {
      auto tbb =
        MoveTables::ver(occ, from) |
        MoveTables::hor(pos.get90RotatedBitboard(), from);
      if (type == GenerationType::Capturing) {
        tbb &= from.isPromotable<turn>() ? notSelfOcc : capOrProm;
      } else {
        tbb &= notCapProm;
      }

      BB_EACH(to, tbb) {
        auto piece = turn == Turn::Black ? Piece::blackRook() : Piece::whiteRook();
        if (type == GenerationType::Capturing) {
          auto promotable = from.isPromotable<turn>() || to.isPromotable<turn>();
          moves.add(Move(piece, from, to, promotable));
        } else {
          moves.add(Move(piece, from, to, false));
        }
      }
    }
  }

  // dragon
  {
    auto fbb = turn == Turn::Black ? pos.getBDragonBitboard() : pos.getWDragonBitboard();
    BB_EACH(from, fbb) {
      auto tbb =
        MoveTables::ver(occ, from) |
        MoveTables::hor(pos.get90RotatedBitboard(), from) |
        MoveTables::king(from);
      tbb &= type == GenerationType::Capturing ? cap : notCap;

      BB_EACH(to, tbb) {
        auto piece = turn == Turn::Black ? Piece::blackDragon() : Piece::whiteDragon();
        moves.add(Move(piece, from, to, false));
      }
    }
  }

  auto prom2 = turn == Turn::Black
    ? Bitboard::blackPromotable2()
    : Bitboard::whitePromotable2();
  auto notCapProm2 =  (cap | prom2).andNot(notSelfOcc);

  // lance
  {
    auto fbb = turn == Turn::Black ? pos.getBLanceBitboard() : pos.getWLanceBitboard();
    BB_EACH(from, fbb) {
      auto tbb = turn == Turn::Black
        ? MoveTables::blackLance(occ, from)
        : MoveTables::whiteLance(occ, from);
      tbb &= type == GenerationType::Capturing ? capOrProm : notCapProm2;

      BB_EACH(to, tbb) {
        auto piece = turn == Turn::Black ? Piece::blackLance() : Piece::whiteLance();
        if (to.isPromotable<turn>()) {
          if (type == GenerationType::Capturing) {
            moves.add(Move(piece, from, to, true));
            if (to.getRank() == (turn == Turn::Black ? 3 : 7) && cap.check(to)) {
              moves.add(Move(piece, from, to, false));
            }
          } else {
            moves.add(Move(piece, from, to, false));
          }

        } else {
          moves.add(Move(piece, from, to, false));
        }
      }
    }
  }

  // knight
  {
    auto fbb = turn == Turn::Black ? pos.getBKnightBitboard() : pos.getWKnightBitboard();
    BB_EACH(from, fbb) {
      auto tbb = turn == Turn::Black
        ? MoveTables::blackKnight(from)
        : MoveTables::whiteKnight(from);
      tbb &= type == GenerationType::Capturing ? capOrProm : notCapProm2;

      BB_EACH(to, tbb) {
        auto piece = turn == Turn::Black ? Piece::blackKnight() : Piece::whiteKnight();
        if (to.isPromotable<turn>()) {
          if (type == GenerationType::Capturing) {
            moves.add(Move(piece, from, to, true));
            if (to.getRank() == (turn == Turn::Black ? 3 : 7) && cap.check(to)) {
              moves.add(Move(piece, from, to, false));
            }
          } else {
            moves.add(Move(piece, from, to, false));
          }

        } else {
          moves.add(Move(piece, from, to, false));
        }
      }
    }
  }
}
template void MoveGenerator::generateMovesOnBoard<Turn::Black, MoveGenerator::GenerationType::Capturing>(const Position&, Moves&);
template void MoveGenerator::generateMovesOnBoard<Turn::White, MoveGenerator::GenerationType::Capturing>(const Position&, Moves&);
template void MoveGenerator::generateMovesOnBoard<Turn::Black, MoveGenerator::GenerationType::NotCapturing>(const Position&, Moves&);
template void MoveGenerator::generateMovesOnBoard<Turn::White, MoveGenerator::GenerationType::NotCapturing>(const Position&, Moves&);

template <Turn turn>
void MoveGenerator::generateDrops(const Position&, Moves&) {
  // TODO
}
template void MoveGenerator::generateDrops<Turn::Black>(const Position&, Moves&);
template void MoveGenerator::generateDrops<Turn::White>(const Position&, Moves&);

template <Turn turn>
void MoveGenerator::generateEvasions(const Position&, Moves&) {
  // TODO
}
template void MoveGenerator::generateEvasions<Turn::Black>(const Position&, Moves&);
template void MoveGenerator::generateEvasions<Turn::White>(const Position&, Moves&);

} // namespace sunfish
