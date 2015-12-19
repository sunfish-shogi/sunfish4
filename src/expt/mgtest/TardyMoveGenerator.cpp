/* TardyMoveGenerator.cpp
 *
 * Kubo Ryosuke
 */

#include "expt/mgtest/TardyMoveGenerator.hpp"
#include "logger/Logger.hpp"
#include <unordered_map>
#include <vector>

namespace {

using namespace sunfish;

struct MovableInfo {
  Direction dir;
  bool isLong;
};

const std::vector<MovableInfo> GoldMovableInfo = {
  { Direction::LeftUp, false },
  { Direction::Up, false },
  { Direction::RightUp, false },
  { Direction::Left, false },
  { Direction::Right, false },
  { Direction::Down, false },
};

const std::unordered_map<PieceRawType, std::vector<MovableInfo>> MovableMap = {
  { PieceNumber::Pawn, {
    { Direction::Up, false },
  }},
  { PieceNumber::Lance, {
    { Direction::Up, true },
  }},
  { PieceNumber::Knight, {
    { Direction::LeftUpKnight, false },
    { Direction::RightUpKnight, false },
  }},
  { PieceNumber::Silver, {
    { Direction::LeftUp, false },
    { Direction::Up, false },
    { Direction::RightUp, false },
    { Direction::LeftDown, false },
    { Direction::RightDown, false },
  }},
  { PieceNumber::Gold, GoldMovableInfo },
  { PieceNumber::Bishop, {
    { Direction::LeftUp, true },
    { Direction::RightUp, true },
    { Direction::LeftDown, true },
    { Direction::RightDown, true },
  }},
  { PieceNumber::Rook, {
    { Direction::Left, true },
    { Direction::Right, true },
    { Direction::Up, true },
    { Direction::Down, true },
  }},
  { PieceNumber::King, {
    { Direction::LeftUp, false },
    { Direction::Up, false },
    { Direction::RightUp, false },
    { Direction::Left, false },
    { Direction::Right, false },
    { Direction::LeftDown, false },
    { Direction::Down, false },
    { Direction::RightDown, false },
  }},
  { PieceNumber::Tokin, { GoldMovableInfo }},
  { PieceNumber::ProLance, { GoldMovableInfo }},
  { PieceNumber::ProKnight, { GoldMovableInfo }},
  { PieceNumber::ProSilver, { GoldMovableInfo }},
  { PieceNumber::Horse, {
    { Direction::LeftUp, true },
    { Direction::RightUp, true },
    { Direction::LeftDown, true },
    { Direction::RightDown, true },
    { Direction::Up, false },
    { Direction::Left, false },
    { Direction::Right, false },
    { Direction::Down, false },
  }},
  { PieceNumber::Dragon, {
    { Direction::Left, true },
    { Direction::Right, true },
    { Direction::Up, true },
    { Direction::Down, true },
    { Direction::LeftUp, false },
    { Direction::RightUp, false },
    { Direction::LeftDown, false },
    { Direction::RightDown, false },
  }},
};

template <Turn turn>
void addMove(Moves& moves, Square from, Square to, PieceType pieceType) {
  bool isPromotable = pieceType.isPromotable() &&
      (from.isPromotable<turn>() || to.isPromotable<turn>());

  if (isPromotable) {
    moves.add(Move(from, to, true));

    bool isRawLanceEffective = turn == Turn::Black ?  to.getRank() == 3 : to.getRank() == 7;
    if ((pieceType == PieceType::lance() && isRawLanceEffective) ||
        (pieceType == PieceType::knight() && to.isKnightMovable<turn>()) ||
        pieceType == PieceType::silver()) {
      moves.add(Move(from, to, false));
    }

  } else {
    moves.add(Move(from, to, false));
  }
}

template <Turn turn>
void addMove(Moves& moves, PieceType pieceType, Square to, const Position& position) {
  if ((pieceType == PieceType::pawn() && !to.isPawnMovable<turn>()) ||
      (pieceType == PieceType::lance() && !to.isLanceMovable<turn>()) ||
      (pieceType == PieceType::knight() && !to.isKnightMovable<turn>())) {
    return;
  }

  Move move(pieceType, to);

  if (pieceType == PieceType::pawn()) {
    if ((turn == Turn::Black && position.hasBlackPawnInFile(to.getFile())) ||
        (turn == Turn::White && position.hasWhitePawnInFile(to.getFile()))) {
      return;
    }

    Position p(position);
    Piece captured;
    p.doMove(move, captured);
    if (p.isMate()) {
      return;
    }
  }

  moves.add(move);
}

template <class T>
void generateMoves(const Position& position, Moves& moves, T&& filter) {
  Turn turn = position.getTurn();

  SQUARE_EACH(from) {
    Piece piece = position.getPieceOnBoard(from);

    if ((turn == Turn::Black && !piece.isBlack()) ||
        (turn == Turn::White && !piece.isWhite())) {
      continue;
    }

    PieceType pieceType = piece.type();
    auto ite = MovableMap.find(pieceType.raw());
    if (ite == MovableMap.end()) {
      continue;
    }

    auto& list = ite->second;
    for (auto& info : list) {
      auto dir = turn == Turn::Black ? info.dir : getReversedDir(info.dir);
      for (Square to = from.safetyMove(dir); to.isValid(); to = to.safetyMove(dir)) {
        Piece pieceTo = position.getPieceOnBoard(to);

        // a piece of self turn
        if ((turn == Turn::Black && pieceTo.isBlack()) ||
            (turn == Turn::White && pieceTo.isWhite())) {
          break;
        }

        if (filter(pieceType, to)) {
          if (turn == Turn::Black) {
            addMove<Turn::Black>(moves, from, to, pieceType);
          } else {
            addMove<Turn::White>(moves, from, to, pieceType);
          }
        }

        if (!info.isLong || !pieceTo.isEmpty()) {
          break;
        }
      }
    }
  }

  auto& hand = turn == Turn::Black ? position.getBlackHand() : position.getWhiteHand();

  HAND_EACH(pieceType) {
    if (hand.get(pieceType) == 0) {
      continue;
    }
 
    SQUARE_EACH(to) {
      if (position.getPieceOnBoard(to).isEmpty() &&
          filter(pieceType, to)) {
        if (turn == Turn::Black) {
          addMove<Turn::Black>(moves, pieceType, to, position);
        } else {
          addMove<Turn::White>(moves, pieceType, to, position);
        }
      }
    }
  }
}

} // namespace

namespace sunfish {

void TardyMoveGenerator::generateMoves(const Position& position, Moves& moves) {
  ::generateMoves(position, moves, [](PieceType, Square) { return true; });
}

void TardyMoveGenerator::generateEvasions(const Position& position, CheckState cs, Moves& moves) {
  Turn turn = position.getTurn();
  Square king = turn == Turn::Black ? position.getBlackKingSquare() : position.getWhiteKingSquare();

  if (!isDoubleCheck(cs)) {
    ::generateMoves(position, moves, [king, cs](PieceType pieceType, Square to) {
      if (pieceType == PieceType::king()) {
        return false;
      }

      auto dir = king.dir(cs.from1);

      for (auto s = king.move(dir); ; s = s.move(dir)) {
        if (s == to) { return true; }
        if (s == cs.from1) { break; }
      }

      return false;
    });
  }

  auto& list = MovableMap.find(PieceNumber::King)->second;
  for (auto& info : list) {
    Square to = king.safetyMove(info.dir);
    if (!to.isValid()) {
      continue;
    }

    Piece pieceTo = position.getPieceOnBoard(to);
    if ((turn == Turn::Black && pieceTo.isBlack()) ||
        (turn == Turn::White && pieceTo.isWhite())) {
      continue;
    }

    moves.add(Move(king, to, false));
  }
}

} // namespace sunfish
