/* Random.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_COMMON_MATH_RANDOM_HPP__
#define SUNFISH_COMMON_MATH_RANDOM_HPP__

#include "common/Def.hpp"
#include <random>
#include <algorithm>
#include <ctime>
#include <cstdint>

namespace sunfish {

template <class GenType>
class BaseRandom {
private:
  GenType rgen;

public:
  BaseRandom() : rgen(static_cast<unsigned>(time(NULL))) {
  }
  BaseRandom(const BaseRandom&) = delete;
  BaseRandom(BaseRandom&&) = delete;

  uint16_t int16() {
    std::uniform_int_distribution<uint16_t> dst16;
    return dst16(rgen);
  }

  uint16_t int16(uint16_t num) {
    std::uniform_int_distribution<uint16_t> dst16(0, num-1);
    return int16() % num;
  }

  uint32_t int32() {
    std::uniform_int_distribution<uint32_t> dst32;
    return dst32(rgen);
  }

  uint32_t int32(uint32_t num) {
    std::uniform_int_distribution<uint32_t> dst32(0, num-1);
    return int32() % num;
  }

  uint64_t int64() {
    std::uniform_int_distribution<uint64_t> dst64;
    return dst64(rgen);
  }

  uint64_t int64(uint64_t num) {
    std::uniform_int_distribution<uint64_t> dst64(0, num-1);
    return int64() % num;
  }

  unsigned bit() {
    std::uniform_int_distribution<unsigned> dstBit(0, 1);
    return dstBit(rgen);
  }

  template <class Iterator>
  void shuffle(Iterator begin, Iterator end) {
    std::shuffle(begin, end, rgen);
  }
};

using Random = BaseRandom<std::mt19937>;

} // namespace sunfish

#endif //SUNFISH_COMMON_MATH_RANDOM_HPP__
