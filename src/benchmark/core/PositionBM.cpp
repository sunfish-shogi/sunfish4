/* PositionBM.cpp
 *
 * Kubo Ryosuke
 */

#include "benchmark/Benchmark.hpp"
#include "core/util/PositionUtil.hpp"

using namespace sunfish;

namespace {

auto DATA_MATE_A =
  "'-- DATA_A ------------------\n"
  "P1 *  *  *  * -OU *  *  *  * \n"
  "P2 *  *  *  *  *  *  *  *  * \n"
  "P3 *  *  *  *  *  *  *  *  * \n"
  "P4 *  *  *  *  *  *  *  *  * \n"
  "P5 *  *  *  *  *  *  *  *  * \n"
  "P6 *  *  *  *  *  *  *  *  * \n"
  "P7 *  *  * -FU-KI *  *  *  * \n"
  "P8 *  *  *  *  *  *  *  *  * \n"
  "P9 *  *  * +OU *  *  *  *  * \n"
  "P+\n"
  "P-\n"
  "+\n";

auto DATA_MATE_B =
  "'-- DATA_B ------------------\n"
  "P1 *  *  *  * -OU *  *  *  * \n"
  "P2 *  *  *  *  *  *  *  *  * \n"
  "P3 *  *  *  *  *  *  *  *  * \n"
  "P4 *  *  *  *  *  *  *  *  * \n"
  "P5 *  *  *  *  *  *  *  *  * \n"
  "P6 *  *  *  *  *  *  *  *  * \n"
  "P7 *  *  * -FU *  *  *  *  * \n"
  "P8 *  *  * -KI *  *  *  *  * \n"
  "P9 *  *  * +OU *  *  *  *  * \n"
  "P+\n"
  "P-\n"
  "+\n";

}

BENCHMARK(IsMate, [](BenchmarkController& bc, bmstr_t data) {
  Position pos = PositionUtil::createPositionFromCsaString(data);

  bc.start();
  while(bc.cont()) {
    pos.isMate();
  }
})
->args(BMSTR(DATA_MATE_A))
->args(BMSTR(DATA_MATE_B));
