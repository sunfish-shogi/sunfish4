/* Sse.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_COMMON_SSE_HPP__
#define SUNFISH_COMMON_SSE_HPP__

#if !defined(USE_SSE2) && __SSE2__
# define USE_SSE2 1
#endif

#if USE_SSE2
# include <emmintrin.h>
#endif

#endif // SUNFISH_COMMON_SSE_HPP__
