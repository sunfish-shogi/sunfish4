/* EvalCache.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_SEARCH_EVAL_EVALCACHE_HPP__
#define SUNFISH_SEARCH_EVAL_EVALCACHE_HPP__

#include "common/Def.hpp"
#include "core/position/Zobrist.hpp"
#include "search/eval/Score.hpp"
#include "search/table/HashTable.hpp"
#include <cstdint>

namespace sunfish {

class EvalCacheElement {
public:

  using DataType = uint64_t;

  static CONSTEXPR_CONST DataType HashMask  = 0xffffffffffff0000;

  // XXX
  EvalCacheElement() : data_(0llu) {
  }

  void set(Zobrist::Type hash,
           const Score& score) {
    data_ = (hash & HashMask) | static_cast<uint16_t>(score.raw());
  }

  bool check(Zobrist::Type hash) {
    return ((data_ ^ hash) & HashMask) == 0llu;
  }

  Score score() {
    uint16_t u16score = static_cast<uint16_t>(data_);
    return Score(static_cast<int16_t>(u16score));
  }

private:

  DataType data_;

};

class EvalCache : public HashTable<EvalCacheElement> {
public:

  static CONSTEXPR_CONST unsigned Width = 18;

  static_assert(Width >= sizeof(Score) * 8, "too small table size");

  void entry(Zobrist::Type hash, const Score& score) {
    auto e = getElement(hash);
    e.set(hash, score);
    getElement(hash) = e;
  }

  bool check(Zobrist::Type hash, Score& score) {
    auto e = getElement(hash);
    if (e.check(hash)) {
      score = e.score();
      return true;
    }
    return false;
  }

};

} // namespace sunfish

#endif // SUNFISH_SEARCH_EVAL_EVALCACHE_HPP__
