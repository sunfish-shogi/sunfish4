
#include "core/move/Move.hpp"
#include "core/position/Position.hpp"
#include <sstream>

namespace sunfish {

std::string Move::toString() const {
  if (isNone()) {
    return "none";
  }

  std::ostringstream oss;

  if (isDrop()) {
    oss << to().toString();
    oss << droppingPieceType().toString();
  } else {
    oss << from().toString();
    oss << to().toString();
  }

  if (isPromotion()) {
    oss << '+';
  }

  return oss.str();
}

std::string Move::toString(const Position& position) const {
  if (isNone()) {
    return "none";
  }

  std::ostringstream oss;

  Turn turn = position.getTurn();
  PieceType pieceType = isDrop()
    ? droppingPieceType()
    : position.getPieceOnBoard(from()).type();

  oss << (turn == Turn::Black ? '+' : '-');

  if (isDrop()) {
    oss << "00";
  } else {
    oss << from().toString();
  }

  oss << to().toString();

  if (isPromotion()) {
    oss << pieceType.promote().toString();
  } else {
    oss << pieceType.toString();
  }

  return oss.str();
}

std::string Move::toStringSFEN() const {
  if (isNone()) {
    return "none";
  }

  std::ostringstream oss;

  if (isDrop()) {
    PieceType pieceType = droppingPieceType();
    oss << pieceType.black().toStringSFEN();
    oss << '*';
    oss << to().toStringSFEN();

  } else {
    oss << from().toStringSFEN();
    oss << to().toStringSFEN();
    if (isPromotion()) {
      oss << '+';
    }

  }

  return oss.str();
}

} // namespace sunfish
