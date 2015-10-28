/* Memory.h
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_COMMON_MEMORY_MEMORY_HPP__
#define SUNFISH_COMMON_MEMORY_MEMORY_HPP__

#include "common/Def.hpp"
#include <cstddef>

namespace sunfish {

namespace memory {

template <size_t size, int rw = 0, int locality = 1>
inline void prefetch(const char* addr) {
  CONSTEXPR_CONST size_t CacheLineSize = 64;
#if defined(UNIX)
  for (size_t i = 0; i < size; i += CacheLineSize) {
    __builtin_prefetch(addr + i, rw, locality);
  }
#endif
}

} // memory

} // sunfish

#endif // SUNFISH_COMMON_MEMORY_MEMORY_HPP__
