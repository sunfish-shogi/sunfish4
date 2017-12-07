/* Gradient.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_LEARN_BATCH_GRADIENT_HPP__
#define SUNFISH_LEARN_BATCH_GRADIENT_HPP__

#include "search/eval/FeatureVector.hpp"
#include <type_traits>

namespace sunfish {

class Position;

using Gradient = FeatureVector<float>;
using OptimizedGradient = OptimizedFeatureVector<float>;

using MaterialGradient = float[16];

inline
void madd(MaterialGradient dst, const MaterialGradient src) {
  for (size_t i = 0; i < std::extent<MaterialGradient>::value; i++) {
    dst[i] += src[i];
  }
}

void extractMaterial(MaterialGradient mg, const Position& pos, float d);

} // namespace sunfish

#endif // SUNFISH_LEARN_BATCH_GRADIENT_HPP__
