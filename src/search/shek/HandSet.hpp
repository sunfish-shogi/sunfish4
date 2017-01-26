/* HandSet.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_SEACH_SHEK_HANDSET_HPP__
#define SUNFISH_SEACH_SHEK_HANDSET_HPP__

#include "common/Def.hpp"
#include "common/string/StringUtil.hpp"
#include "core/position/Hand.hpp"
#include "search/shek/ShekState.hpp"
#include <string>
#include <cstdint>

namespace sunfish {

class HandSet {
public:

  using Type = uint32_t;

  HandSet() = default;

  explicit HandSet(const Hand& hand) {
    set_ = 0LLU;
    push(hand.get(PieceType::pawn())  , 0);
    push(hand.get(PieceType::lance()) , 4);
    push(hand.get(PieceType::knight()), 4);
    push(hand.get(PieceType::silver()), 4);
    push(hand.get(PieceType::gold())  , 4);
    push(hand.get(PieceType::bishop()), 2);
    push(hand.get(PieceType::rook())  , 2);
  }

  bool operator==(const HandSet& rhs) const {
    return set_ == rhs.set_;
  }

  ShekState compareTo(const HandSet& rhs) const {
    if (set_ == rhs.set_) {
      return ShekState::Equal;
    }

    bool sup = set_ & (~rhs.set_);
    bool inf = (~set_) & rhs.set_;

    if (sup && !inf) {
      return ShekState::Superior;
    } else if (!sup && inf) {
      return ShekState::Inferior;
    } else {
      return ShekState::None;
    }
  }

  std::string toString() const {
    return StringUtil::stringify(set_);
  }

  friend std::ostream& operator<<(std::ostream& os, const HandSet& handSet) {
    os << handSet.toString();
    return os;
  }

private:

  void push(Hand::Type count, int shift) {
    set_ <<= shift;
    set_ |= (1LLU << count) - 1;
  }

  Type set_;
};

} // namespace sunfish

#endif // SUNFISH_SEACH_SHEK_HANDSET_HPP__
