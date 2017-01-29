/* Memory.h
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_COMMON_MEMORY_MEMORY_HPP__
#define SUNFISH_COMMON_MEMORY_MEMORY_HPP__

#include "common/Def.hpp"

namespace sunfish {

namespace memory {

inline void prefetch(const char* addr) {
#if defined(__INTEL_COMPILER)
  __asm__("")
#endif

#if defined(__INTEL_COMPILER) || defined(_MSC_VER)
  _mm_prefetch((char*)addr, _MM_HINT_T0)
#else
  __builtin_prefetch(addr);
#endif
}

} // memory

} // sunfish

#endif // SUNFISH_COMMON_MEMORY_MEMORY_HPP__
