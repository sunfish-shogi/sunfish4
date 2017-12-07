/* LearningUtil.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_LEARN_UTIL_LEARNINGUTIL_HPP__
#define SUNFISH_LEARN_UTIL_LEARNINGUTIL_HPP__

#include "search/eval/Evaluator.hpp"

namespace sunfish {

class LearningUtil {
public:

  static void printFVSummary(Evaluator::FVType* fv);

  static void printMaterial();

private:

  LearningUtil();

};

} // namespace sunfish

#endif // SUNFISH_LEARN_UTIL_LEARNINGUTIL_HPP__
