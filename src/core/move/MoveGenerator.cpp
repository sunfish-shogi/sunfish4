/* MoveGenerator.cpp
 *
 * Kubo Ryosuke
 */

#include "core/move/MoveGenerator.hpp"
#include "core/move/MoveTables.hpp"

namespace sunfish {

template <Turn turn, MoveGenerator::GenerationType type, bool exceptKing>
void MoveGenerator::generateMovesOnBoard(const Position& pos, Moves& moves, const Bitboard& mask) {
  auto occ = pos.getBOccupiedBitboard() | pos.getWOccupiedBitboard();

  auto notSelfOcc = turn == Turn::Black ? ~pos.getBOccupiedBitboard() : ~pos.getWOccupiedBitboard();
  auto cap = turn == Turn::Black ? pos.getWOccupiedBitboard() : pos.getBOccupiedBitboard();
  auto prom = turn == Turn::Black ? Bitboard::blackPromotable() : Bitboard::whitePromotable();

  auto notCap = cap.andNot(notSelfOcc);
  auto capOrProm = (cap | prom) & notSelfOcc;
  auto notCapProm = (cap | prom).andNot(notSelfOcc);

  // pawn
  {
    auto fbb = turn == Turn::Black ? pos.getBPawnBitboard() : pos.getWPawnBitboard();
    if (type == GenerationType::Capture) {
      fbb &= turn == Turn::Black ? capOrProm << 1 : capOrProm >> 1;
    } else if (type == GenerationType::Quiet) {
      fbb &= turn == Turn::Black ? notCapProm << 1 : notCapProm >> 1;;
    } else {
      fbb &= turn == Turn::Black ? mask << 1 : mask >> 1;;
    }

    BB_EACH(from, fbb) {
      Square to = turn == Turn::Black ? from.up() : from.down();
      moves.add(Move(from, to, to.isPromotable<turn>()));
    }
  }

  // silver
  {
    auto fbb = turn == Turn::Black ? pos.getBSilverBitboard() : pos.getWSilverBitboard();
    BB_EACH(from, fbb) {
      auto tbb = turn == Turn::Black
        ? MoveTables::blackSilver(from)
        : MoveTables::whiteSilver(from);
      if (type == GenerationType::Capture) {
        tbb &= cap;
      } else if (type == GenerationType::Quiet) {
        tbb &= notCap;
      } else {
        tbb &= mask;
      }

      BB_EACH(to, tbb) {
        if (from.isPromotable<turn>() || to.isPromotable<turn>()) {
          moves.add(Move(from, to, true));
          moves.add(Move(from, to, false));
        } else {
          moves.add(Move(from, to, false));
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
      if (type == GenerationType::Capture) {
        tbb &= cap;
      } else if (type == GenerationType::Quiet) {
        tbb &= notCap;
      } else {
        tbb &= mask;
      }

      BB_EACH(to, tbb) {
        moves.add(Move(from, to, false));
      }
    }
  }

  // king
  if (!exceptKing) {
    auto from = turn == Turn::Black ? pos.getBlackKingSquare() : pos.getWhiteKingSquare();
    auto tbb = MoveTables::king(from);
    if (type == GenerationType::Capture) {
      tbb &= cap;
    } else if (type == GenerationType::Quiet) {
      tbb &= notCap;
    } else {
      tbb &= mask;
    }

    BB_EACH(to, tbb) {
      moves.add(Move(from, to, false));
    }
  }

  // bishop
  {
    auto fbb = turn == Turn::Black ? pos.getBBishopBitboard() : pos.getWBishopBitboard();
    if (type == GenerationType::Quiet) {
      fbb &= turn == Turn::Black ? Bitboard::blackNotPromotable() : Bitboard::whiteNotPromotable();
    }
    BB_EACH(from, fbb) {
      auto tbb =
        MoveTables::diagR45(pos.getRight45RotatedBitboard(), from) |
        MoveTables::diagL45(pos.getLeft45RotatedBitboard(), from);
      if (type == GenerationType::Capture) {
        tbb &= from.isPromotable<turn>() ? notSelfOcc : capOrProm;
      } else if (type == GenerationType::Quiet) {
        tbb &= notCapProm;
      } else {
        tbb &= mask;
      }

      BB_EACH(to, tbb) {
        if (type == GenerationType::Capture || type == GenerationType::All) {
          auto promotable = from.isPromotable<turn>() || to.isPromotable<turn>();
          moves.add(Move(from, to, promotable));
        } else {
          moves.add(Move(from, to, false));
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
      if (type == GenerationType::Capture) {
        tbb &= cap;
      } else if (type == GenerationType::Quiet) {
        tbb &= notCap;
      } else {
        tbb &= mask;
      }

      BB_EACH(to, tbb) {
        moves.add(Move(from, to, false));
      }
    }
  }

  // rook
  {
    auto fbb = turn == Turn::Black ? pos.getBRookBitboard() : pos.getWRookBitboard();
    if (type == GenerationType::Quiet) {
      fbb &= turn == Turn::Black ? Bitboard::blackNotPromotable() : Bitboard::whiteNotPromotable();
    }
    BB_EACH(from, fbb) {
      auto tbb =
        MoveTables::ver(occ, from) |
        MoveTables::hor(pos.get90RotatedBitboard(), from);
      if (type == GenerationType::Capture) {
        tbb &= from.isPromotable<turn>() ? notSelfOcc : capOrProm;
      } else if (type == GenerationType::Quiet) {
        tbb &= notCapProm;
      } else {
        tbb &= mask;
      }

      BB_EACH(to, tbb) {
        if (type == GenerationType::Capture || type == GenerationType::All) {
          auto promotable = from.isPromotable<turn>() || to.isPromotable<turn>();
          moves.add(Move(from, to, promotable));
        } else {
          moves.add(Move(from, to, false));
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
      if (type == GenerationType::Capture) {
        tbb &= cap;
      } else if (type == GenerationType::Quiet) {
        tbb &= notCap;
      } else {
        tbb &= mask;
      }

      BB_EACH(to, tbb) {
        moves.add(Move(from, to, false));
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
      if (type == GenerationType::Capture) {
        tbb &= capOrProm;
      } else if (type == GenerationType::Quiet) {
        tbb &= notCapProm2;
      } else {
        tbb &= mask;
      }

      BB_EACH(to, tbb) {
        if (to.isPromotable<turn>()) {
          if (type == GenerationType::Capture) {
            moves.add(Move(from, to, true));
            if (to.getRank() == (turn == Turn::Black ? 3 : 7) && cap.check(to)) {
              moves.add(Move(from, to, false));
            }
          } else if (type == GenerationType::Quiet) {
            moves.add(Move(from, to, false));
          } else {
            moves.add(Move(from, to, true));
            if (to.getRank() == (turn == Turn::Black ? 3 : 7)) {
              moves.add(Move(from, to, false));
            }
          }

        } else {
          moves.add(Move(from, to, false));
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
      if (type == GenerationType::Capture) {
        tbb &= capOrProm;
      } else if (type == GenerationType::Quiet) {
        tbb &= notCapProm2;
      } else {
        tbb &= mask;
      }

      BB_EACH(to, tbb) {
        if (to.isPromotable<turn>()) {
          if (type == GenerationType::Capture) {
            moves.add(Move(from, to, true));
            if (to.getRank() == (turn == Turn::Black ? 3 : 7) && cap.check(to)) {
              moves.add(Move(from, to, false));
            }
          } else if (type == GenerationType::Quiet) {
            moves.add(Move(from, to, false));
          } else {
            moves.add(Move(from, to, true));
            if (to.getRank() == (turn == Turn::Black ? 3 : 7)) {
              moves.add(Move(from, to, false));
            }
          }

        } else {
          moves.add(Move(from, to, false));
        }
      }
    }
  }
}
template void MoveGenerator::generateMovesOnBoard<Turn::Black, MoveGenerator::GenerationType::Capture, false>(const Position&, Moves&, const Bitboard&);
template void MoveGenerator::generateMovesOnBoard<Turn::White, MoveGenerator::GenerationType::Capture, false>(const Position&, Moves&, const Bitboard&);
template void MoveGenerator::generateMovesOnBoard<Turn::Black, MoveGenerator::GenerationType::Quiet, false>(const Position&, Moves&, const Bitboard&);
template void MoveGenerator::generateMovesOnBoard<Turn::White, MoveGenerator::GenerationType::Quiet, false>(const Position&, Moves&, const Bitboard&);

template <Turn turn>
void MoveGenerator::generateDrops(const Position& pos, Moves& moves, const Bitboard& mask) {
  PieceType pieces[6];
  int kn = 0;
  int ln = 0;
  int pn = 0;

  const auto& hand = turn == Turn::Black ? pos.getBlackHand() : pos.getWhiteHand();
  if (hand.get(PieceType::knight()) != 0) { pieces[pn++] = PieceType::knight(); ln = kn = pn; }
  if (hand.get(PieceType::lance()) != 0)  { pieces[pn++] = PieceType::lance(); ln = pn; }
  if (hand.get(PieceType::silver()) != 0) { pieces[pn++] = PieceType::silver(); }
  if (hand.get(PieceType::gold()) != 0)   { pieces[pn++] = PieceType::gold(); }
  if (hand.get(PieceType::bishop()) != 0) { pieces[pn++] = PieceType::bishop(); }
  if (hand.get(PieceType::rook()) != 0)   { pieces[pn++] = PieceType::rook(); }

  auto occ = pos.getBOccupiedBitboard() | pos.getWOccupiedBitboard();
  auto noocc = occ.andNot(mask);

  if (hand.get(PieceType::pawn()) != 0) {
    Bitboard rank2to9;
    if (turn == Turn::Black) {
      rank2to9 = Bitboard::rank2to9() & noocc;
    } else {
      rank2to9 = Bitboard::rank1to8() & noocc;
    }

    const auto& pawn = turn == Turn::Black ? pos.getBPawnBitboard() : pos.getWPawnBitboard();
    if (pawn.checkFile(1)) { rank2to9 = Bitboard::file1().andNot(rank2to9); }
    if (pawn.checkFile(2)) { rank2to9 = Bitboard::file2().andNot(rank2to9); }
    if (pawn.checkFile(3)) { rank2to9 = Bitboard::file3().andNot(rank2to9); }
    if (pawn.checkFile(4)) { rank2to9 = Bitboard::file4().andNot(rank2to9); }
    if (pawn.checkFile(5)) { rank2to9 = Bitboard::file5().andNot(rank2to9); }
    if (pawn.checkFile(6)) { rank2to9 = Bitboard::file6().andNot(rank2to9); }
    if (pawn.checkFile(7)) { rank2to9 = Bitboard::file7().andNot(rank2to9); }
    if (pawn.checkFile(8)) { rank2to9 = Bitboard::file8().andNot(rank2to9); }
    if (pawn.checkFile(9)) { rank2to9 = Bitboard::file9().andNot(rank2to9); }

    if (pos.isMateWithPawnDrop()) {
      auto enemyKing = turn == Turn::Black ? pos.getWhiteKingSquare() : pos.getBlackKingSquare();
      auto square = turn == Turn::Black ? enemyKing.down() : enemyKing.up();
      rank2to9 = Bitboard::mask(square).andNot(rank2to9);
    }

    BB_EACH(to, rank2to9) {
      moves.add(Move(PieceType::pawn(), to));
    }
  }

  if (pn == 0) {
    return;
  }

  Bitboard rank1;
  Bitboard rank2;
  Bitboard rank3to9;
  if (turn == Turn::Black) {
    rank1 = Bitboard::rank1() & noocc;
    rank2 = Bitboard::rank2() & noocc;
    rank3to9 = Bitboard::rank3to9() & noocc;
  } else {
    rank1 = Bitboard::rank9() & noocc;
    rank2 = Bitboard::rank8() & noocc;
    rank3to9 = Bitboard::rank1to7() & noocc;
  }

  BB_EACH(to, rank3to9) {
    for (int i = 0; i < pn; i++) {
      moves.add(Move(pieces[i], to));
    }
  }

  BB_EACH(to, rank2) {
    for (int i = kn; i < pn; i++) {
      moves.add(Move(pieces[i], to));
    }
  }

  BB_EACH(to, rank1) {
    for (int i = ln; i < pn; i++) {
      moves.add(Move(pieces[i], to));
    }
  }
}
template void MoveGenerator::generateDrops<Turn::Black>(const Position&, Moves&, const Bitboard&);
template void MoveGenerator::generateDrops<Turn::White>(const Position&, Moves&, const Bitboard&);

template <Turn turn>
void MoveGenerator::generateEvasions(const Position& pos, CheckState checkState, Moves& moves) {
  ASSERT(isCheck(checkState));

  auto kingSquare = turn == Turn::Black ? pos.getBlackKingSquare() : pos.getWhiteKingSquare();

  if (!checkState.from2.isValid()) {
    auto targetSquare = checkState.from1;
    ASSERT(turn != Turn::Black || pos.getPieceOnBoard(targetSquare).isWhite());
    ASSERT(turn != Turn::White || pos.getPieceOnBoard(targetSquare).isBlack());

    const auto& tbb = Bitboard::mask(kingSquare, targetSquare);

    if (targetSquare.distance(kingSquare) != 1) {
      generateDrops<turn>(pos, moves, tbb);
    }

    generateMovesOnBoard<turn, GenerationType::All, true>(pos, moves, tbb);
  }

  auto tbb = MoveTables::king(kingSquare);
  if (turn == Turn::Black) {
    tbb = pos.getBOccupiedBitboard().andNot(tbb);
  } else {
    tbb = pos.getWOccupiedBitboard().andNot(tbb);
  }

  BB_EACH(to, tbb) {
    moves.add(Move(kingSquare, to, false));
  }
}
template void MoveGenerator::generateEvasions<Turn::Black>(const Position&, CheckState, Moves&);
template void MoveGenerator::generateEvasions<Turn::White>(const Position&, CheckState, Moves&);

} // namespace sunfish
