/* PV.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_SEARCH_TREE_PV_HPP__
#define SUNFISH_SEARCH_TREE_PV_HPP__

#include "common/Def.hpp"
#include "core/move/Moves.hpp"
#include <cstdint>
#include <cstring>
#include <sstream>

namespace sunfish {

class PV {
public:

  using SizeType = uint32_t;

  static CONSTEXPR_CONST SizeType StackSize = 64;

  PV() : num_(0) {
  }

  PV(const PV&) = default;
  PV(PV&&) = default;

  PV& operator=(const PV&) = default;
  PV& operator=(PV&&) = default;

  void clear() {
    num_ = 0;
  }

  SizeType size() const {
    return num_;
  }

  void set(const Move& move, const PV& pv) {
    moves_[0] = move;
    // static_cast is required on Clang.
    // See https://trello.com/c/iJqg1GqN
    num_ = std::min(pv.num_ + 1, static_cast<SizeType>(StackSize));
    memcpy(&moves_[1], pv.moves_, sizeof(moves_[0]) * (num_ - 1));
  }

  const Move& get(SizeType depth) const {
    return moves_[depth];
  }

  std::string toString() const {
    std::ostringstream oss;
    for (SizeType i = 0; i < num_; i++) {
      if (i != 0) {
        oss << ' ';
      }
      oss << moves_[i].toString();
    }
    return oss.str();
  }

  std::string toStringSFEN() const {
    std::ostringstream oss;
    for (SizeType i = 0; i < num_; i++) {
      if (i != 0) {
        oss << ' ';
      }
      oss << moves_[i].toStringSFEN();
    }
    return oss.str();
  }

  friend std::ostream& operator<<(std::ostream& os, const sunfish::PV& pv) {
    os << pv.toString();
    return os;
  }

private:

  SizeType num_;
  Move moves_[StackSize];

};

} // namespace sunfish

#endif // SUNFISH_SEARCH_TREE_PV_HPP__
