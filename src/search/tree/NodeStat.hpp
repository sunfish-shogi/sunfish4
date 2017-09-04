/* NodeStat.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_SEARCH_TREE_NODE_STAT_HPP__
#define SUNFISH_SEARCH_TREE_NODE_STAT_HPP__

#include "common/Def.hpp"
#include <cstdint>

namespace sunfish {

class NodeStat {
public:

  using RawType = uint8_t;

  static CONSTEXPR_CONST RawType NullMoveSearch     = 0x01;
  static CONSTEXPR_CONST RawType RecursiveIDSearch  = 0x02;
  static CONSTEXPR_CONST RawType MateThreat         = 0x04;
  static CONSTEXPR_CONST RawType HashCut            = 0x08;
  static CONSTEXPR_CONST RawType RecaptureExtension = 0x10;
  static CONSTEXPR_CONST RawType MateDetection      = 0x20;
  static CONSTEXPR_CONST RawType Root               = 0x40;

  static CONSTEXPR_CONST RawType Normal = NullMoveSearch
                                        | RecursiveIDSearch
                                        | HashCut
                                        | RecaptureExtension
                                        | MateDetection;

public:

  NodeStat() {
  }

  NodeStat(const NodeStat& src) = default;
  NodeStat(NodeStat&& src) = default;

  static CONSTEXPR NodeStat normal() {
    return NodeStat(Normal);
  }

#define NODESTAT_METHOD__(s) \
  NodeStat& set ## s() { \
    return set(s); \
  } \
  NodeStat& unset ## s() { \
    return unset(s); \
  } \
  bool is ## s() const { \
    return is(s); \
  }

  NODESTAT_METHOD__(NullMoveSearch);
  NODESTAT_METHOD__(RecursiveIDSearch);
  NODESTAT_METHOD__(MateThreat);
  NODESTAT_METHOD__(HashCut);
  NODESTAT_METHOD__(RecaptureExtension);
  NODESTAT_METHOD__(MateDetection);
  NODESTAT_METHOD__(Root);

#undef NODESTAT_METHOD__

private:

  explicit CONSTEXPR NodeStat(RawType stat) : stat_(stat) {
  }

  NodeStat& set(RawType flag) {
    stat_ |= flag;
    return *this;
  }

  NodeStat& unset(RawType flag) {
    stat_ &= ~flag;
    return *this;
  }

  bool is(RawType flag) const {
    return stat_ & flag;
  }

private:

  RawType stat_;

};

} // namespace sunfish

#endif // SUNFISH_SEARCH_TREE_NODE_STAT_HPP__
