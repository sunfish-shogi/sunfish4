/* Random.cpp
 *
 * Kubo Ryosuke
 */

#include "common/math/Random.hpp"
#include <ctime>

namespace sunfish {

template <class GenType>
GenType& BaseRandom<GenType>::gen() {
  thread_local GenType g(static_cast<unsigned>(time(NULL)));
  return g;
}

template std::mt19937& BaseRandom<std::mt19937>::gen();

} // namespace sunfish
