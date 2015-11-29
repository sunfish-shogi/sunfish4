
#include "core/move/Move.hpp"
#include "core/position/Position.hpp"
#include <sstream>

namespace sunfish {

uint16_t Move::serialize16(const Move& obj) {
  if (obj.isEmpty()) {
    return S16Empty;
  } else if (obj.isDrop()) {
    assert(!(obj.piece().isPromoted()));
    uint32_t masked = obj.move_ & (ToMask | PieceMask);
    uint32_t shifted = (masked >> S16HandOffset);
    assert(shifted < 0x0800);
    uint16_t data = (uint16_t)(shifted | S16Hand);
    assert(((uint32_t)data & ~S16Hand) < 0x0800);
    return data;
  } else {
    uint32_t masked = obj.move_ & (FromMask | ToMask | PromoteMask);
    assert(masked < S16Hand);
    uint16_t data = (uint16_t)masked;
    assert(!(data & S16Hand));
    return data;
  }
}

Move Move::deserialize16(uint16_t value, const Position& position) {
  if (value == S16Empty) {
    return empty();
  } else if (value & S16Hand) {
    Move obj;
    uint32_t masked = (uint32_t)value & ~S16Hand;
    assert(masked < 0x0800);
    obj.move_ = masked << S16HandOffset;
    assert(!obj.piece().isPromoted());
    return obj;
  } else {
    Move obj;
    obj.move_ = value;
    Piece piece = position.getPieceOnBoard(obj.from());
    obj.move_ = (obj.move_ & ~PieceMask) | static_cast<RawType>(piece.type().raw()) << PieceOffset;
    return obj;
  }
}

std::string Move::toString() const {
  std::ostringstream oss;

  oss << (piece().isBlack() ? '+' : '-');

  if (isDrop()) {
    oss << "00";
  } else {
    oss << from().toString();
  }

  oss << to().toString();

  if (isPromotion()) {
    oss << piece().promote().type().toString();
  } else {
    oss << piece().type().toString();
  }

  return oss.str();
}

std::string Move::toStringSFEN() const {
  std::ostringstream oss;

  if (isDrop()) {
    oss << piece().type().black().toStringSFEN();
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
