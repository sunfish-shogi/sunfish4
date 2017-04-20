/* Random.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_COMMON_MATH_RANDOM_HPP__
#define SUNFISH_COMMON_MATH_RANDOM_HPP__

#include "common/Def.hpp"
#include <random>
#include <algorithm>
#include <cstdint>

namespace sunfish {

template <class GenType>
class BaseRandom {
public:
  BaseRandom() = delete;

  static uint16_t uint16() {
    std::uniform_int_distribution<uint16_t> dst16;
    return dst16(gen());
  }

  static uint16_t uint16(uint16_t num) {
    std::uniform_int_distribution<uint16_t> dst16(0, num-1);
    return dst16(gen());
  }

  static uint32_t uint32() {
    std::uniform_int_distribution<uint32_t> dst32;
    return dst32(gen());
  }

  static uint32_t uint32(uint32_t num) {
    std::uniform_int_distribution<uint32_t> dst32(0, num-1);
    return dst32(gen());
  }

  static uint64_t uint64() {
    std::uniform_int_distribution<uint64_t> dst64;
    return dst64(gen());
  }

  static uint64_t uint64(uint64_t num) {
    std::uniform_int_distribution<uint64_t> dst64(0, num-1);
    return dst64(gen());
  }

  static unsigned bit() {
    std::uniform_int_distribution<unsigned> dstBit(0, 1);
    return dstBit(gen());
  }

  template <class T>
  static unsigned nonuniform(unsigned num, T&& weightFunc) {
    uint64_t total = 0.0f;
    for (unsigned i = 0; i < num; i++) {
      total += weightFunc(i);
    }

    uint64_t r = uint64(total);

    for (unsigned i = 0; i < num - 1; i++) {
      uint64_t w = weightFunc(i);
      if (r < w) {
        return i;
      }
      r -= w;
    }
    return num - 1;
  }

  template <class Iterator>
  static void shuffle(Iterator begin, Iterator end) {
    std::shuffle(begin, end, gen());
  }

private:
  static GenType& gen();

};

using Random = BaseRandom<std::mt19937>;

} // namespace sunfish

#endif //SUNFISH_COMMON_MATH_RANDOM_HPP__
