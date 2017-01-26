/* ShekTable.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_SEACH_SHEK_SHEKELEMENT_HPP__
#define SUNFISH_SEACH_SHEK_SHEKELEMENT_HPP__

#include "common/Def.hpp"
#include "core/position/Zobrist.hpp"
#include "search/shek/HandSet.hpp"
#include "logger/Logger.hpp"

namespace sunfish {

class ShekElement {
public:

  using DataType = uint32_t;

  static CONSTEXPR_CONST DataType MaxCount  = 0x00000007;
  static CONSTEXPR_CONST DataType CountMask = 0x00000007;
  static CONSTEXPR_CONST DataType TurnMask  = 0x00000008;
  static CONSTEXPR_CONST DataType HashMask  = 0xfffffff0;
  static CONSTEXPR_CONST int      HashShift = 32;

  ShekElement() : data_(0LLU) {
  }

  ShekState check(const HandSet& handSet,
                 Turn turn) const {
    ShekState shekState = handSet.compareTo(handSet_);

    Turn turn0 = data_ & TurnMask ? Turn::Black : Turn::White;
    switch (shekState) {
    case ShekState::Equal:
      return turn0 != turn ? ShekState::Superior :
             count() >= 3  ? ShekState::Equal4 :
                             ShekState::Equal;
    case ShekState::Superior:
      return turn  == Turn::Black ? ShekState::Superior :
             turn0 == Turn::Black ? ShekState::None :
                                    ShekState::Inferior;
    case ShekState::Inferior:
      return turn  == Turn::White ? ShekState::Superior :
             turn0 == Turn::White ? ShekState::None :
                                    ShekState::Inferior;
    default:
      return ShekState::None;
    }
  }

  void setAndRetain(Zobrist::Type hash,
                    const HandSet& handSet,
                    Turn turn) {
    handSet_ = handSet;
    data_ = (hash >> HashShift) & HashMask;
    data_ |= turn == Turn::Black ? TurnMask : 0LLU;
    data_ |= 1LLU;
  }

  void retain() {
    ASSERT((data_ & CountMask) != MaxCount);
    data_++;
  }

  void release() {
    if (!isVacant()) {
      data_--;
    }
  }

  bool isVacant() const {
    return (data_ & CountMask) == 0LLU;
  }

  bool checkHash(Zobrist::Type hash) const {
    return !isVacant() && ((static_cast<DataType>(hash >> HashShift) ^ data_) & HashMask) == 0LLU;
  }

  const HandSet& handSet() const {
    return handSet_;
  }

private:

  DataType count() const {
    return data_ & CountMask;
  }

  HandSet handSet_;
  DataType data_;

};

} // namespace sunfish

#endif // SUNFISH_SEACH_SHEK_SHEKELEMENT_HPP__
