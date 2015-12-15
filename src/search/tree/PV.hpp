/* PV.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_SEARCH_TREE_PV_HPP__
#define SUNFISH_SEARCH_TREE_PV_HPP__

#include "common/Def.hpp"
#include "core/move/Moves.hpp"
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <sstream>

namespace sunfish {

class PV {
public:

  using SizeType = uint32_t;
  using MoveType = uint16_t;
  using DepthType = int16_t;

  static CONSTEXPR_CONST SizeType MaxSize = 64;

  struct Element {
    MoveType move;
    DepthType depth;
  };

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

  void set(const Move& move, int depth, const PV& pv) {
    depth = std::max(depth, 0);

    elements_[0].move = move.serialize16();
    elements_[0].depth = static_cast<DepthType>(depth);

    // static_cast is required on Clang.
    // See https://trello.com/c/iJqg1GqN
    num_ = std::min(pv.num_ + 1, static_cast<SizeType>(MaxSize));
    memcpy(&elements_[1], pv.elements_, sizeof(elements_[0]) * (num_ - 1));
  }

  Move getMove(SizeType depth) const {
    return Move::deserialize(elements_[depth].move);
  }

  int getDepth(SizeType depth) const {
    return static_cast<int>(elements_[depth].depth);
  }

  std::string toString() const {
    std::ostringstream oss;
    for (SizeType i = 0; i < num_; i++) {
      if (i != 0) {
        oss << ' ';
      }
      Move move = Move::deserialize(elements_[i].move);
      oss << move.toString();
    }
    return oss.str();
  }

  std::string toStringSFEN() const {
    std::ostringstream oss;
    for (SizeType i = 0; i < num_; i++) {
      if (i != 0) {
        oss << ' ';
      }
      Move move = Move::deserialize(elements_[i].move);
      oss << move.toStringSFEN();
    }
    return oss.str();
  }

  friend std::ostream& operator<<(std::ostream& os, const sunfish::PV& pv) {
    os << pv.toString();
    return os;
  }

private:

  SizeType num_;
  Element elements_[MaxSize];

};

} // namespace sunfish

#endif // SUNFISH_SEARCH_TREE_PV_HPP__
