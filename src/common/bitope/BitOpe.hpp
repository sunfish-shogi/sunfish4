/* BitOpe.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_COMMON_BITOPE_BITOPE_HPP__
#define SUNFISH_COMMON_BITOPE_BITOPE_HPP__

#include "common/Def.hpp"
#include <cstdint>

namespace sunfish {

/**
 * Returns the number of 1-bits in the specified value.
 */
inline int popcount(uint32_t val) {
#if defined(UNIX)
  return __builtin_popcount(val);
#else
	val = val - ((val >> 1) & 0x55555555U);
	val = (val & 0x33333333U) + ((val >> 2) & 0x33333333U);
	val = (val + (val >> 4)) & 0x0F0F0F0FU;
	val = val + (val >> 8);
	val = val + (val >> 16);
	return val & 0x0000003FU;
#endif
}

/**
 * Returns the number of 1-bits in the specified value.
 */
inline int popcount(uint64_t val) {
#if defined(UNIX)
  return __builtin_popcountll(val);
#else
	val = val - ((val >> 1) & 0x5555555555555555llu);
	val = (val & 0x3333333333333333llu) + ((val >> 2) & 0x3333333333333333llu);
	val = (val + (val >> 4)) & 0x0F0F0F0F0F0F0F0Fllu;
	val = val + (val >> 8);
	val = val + (val >> 16);
	val = val + (val >> 32);
	return static_cast<int>(val & 0x000000000000007Fllu);
#endif
}

} // namespace sunfish

#endif // SUNFISH_COMMON_BITOPE_BITOPE_HPP__
