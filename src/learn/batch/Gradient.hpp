/* Gradient.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_LEARN_BATCH_GRADIENT_HPP__
#define SUNFISH_LEARN_BATCH_GRADIENT_HPP__

#include "search/eval/FeatureVector.hpp"

namespace sunfish {

struct Gradient : public FeatureVector<float> {
};

} // namespace sunfish

#endif // SUNFISH_LEARN_BATCH_GRADIENT_HPP__
