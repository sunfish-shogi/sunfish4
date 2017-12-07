/* LearningUtil.cpp
 *
 * Kubo Ryosuke
 */

#include "learn/util/LearningUtil.hpp"
#include "search/eval/Material.hpp"
#include "search/eval/FeatureTemplates.hpp"
#include "common/string/TablePrinter.hpp"

namespace sunfish {

void LearningUtil::printFVSummary(Evaluator::FVType* fv) {
  auto summary = summarize(*fv);
  TablePrinter tp;

  tp.row() << "name"
           << "num"
           << "zero"
           << "non-zero"
           << "non-zero(%)"
           << "max"
           << "min"
           << "max-abs"
           << "ave-abs"
           << "";
  for (const auto& s : summary) {
    float nonZeroPer = (float)s.nonZero / s.num * 100.0f;
    tp.row() << s.name
             << s.num
             << s.zero
             << s.nonZero
             << nonZeroPer
             << s.max
             << s.min
             << s.maxAbs
             << s.aveAbs;
  }

  MSG(info) << "Summary:\n" << StringUtil::chomp(tp.stringify());
}

void LearningUtil::printMaterial() {
  std::ostringstream oss;
  PIECE_TYPE_EACH(pieceType) {
    oss << pieceType << ": " << material::scores[pieceType.raw()] << "\n";
  }
  MSG(info) << "Material:\n" << StringUtil::chomp(oss.str());
}

} // namespace sunfish
