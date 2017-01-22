/* MoveGeneratorBM.cpp
 *
 * Kubo Ryosuke
 */

#include "benchmark/Benchmark.hpp"
#include "core/move/MoveGenerator.hpp"
#include "core/util/PositionUtil.hpp"

using namespace sunfish;

namespace {

auto DATA_A =
  "'-- DATA_A ------------------\n"
  "P1-KY-KE-GI-KI-OU-KI-GI-KE-KY\n"
  "P2 * -HI *  *  *  *  * -KA * \n"
  "P3-FU-FU-FU-FU-FU-FU-FU-FU-FU\n"
  "P4 *  *  *  *  *  *  *  *  * \n"
  "P5 *  *  *  *  *  *  *  *  * \n"
  "P6 *  *  *  *  *  *  *  *  * \n"
  "P7+FU+FU+FU+FU+FU+FU+FU+FU+FU\n"
  "P8 * +KA *  *  *  *  * +HI * \n"
  "P9+KY+KE+GI+KI+OU+KI+GI+KE+KY\n"
  "P+\n"
  "P-\n"
  "+\n";

auto DATA_B =
  "'-- DATA_B ------------------\n"
  "P1-KY-KE * -KI *  *  *  *  * \n"
  "P2 * -OU-GI-KY-GI *  *  *  * \n"
  "P3 * -FU-KE *  *  *  * -FU-FU\n"
  "P4+FU * -FU *  * -UM-FU *  * \n"
  "P5 *  *  *  *  * +FU * +FU * \n"
  "P6 *  * +FU * -FU * +FU+GI * \n"
  "P7 *  *  * +FU *  *  *  * +FU\n"
  "P8 *  *  * +KI *  *  * +HI * \n"
  "P9 *  *  * +OU *  *  * +KE+KY\n"
  "P+00KI00GI00KY00FU00FU\n"
  "P-00HI00KA00KI00KE00FU00FU00FU\n"
  "+\n";

auto DATA_X =
  "'-- DATA_X ------------------\n"
  "P1-KY * -GI * -KY *  * -KE-KY\n"
  "P2 * -HI *  *  * -OU-KI *  * \n"
  "P3-FU * -KE-FU-FU-FU * -FU-FU\n"
  "P4 * +FU *  * +FU * -GI *  * \n"
  "P5-KE *  *  *  *  *  *  *  * \n"
  "P6 *  * +KI *  *  *  * +FU * \n"
  "P7+FU-FU * +FU+KA+FU+FU-GI+FU\n"
  "P8 * -KI+OU+KI *  * -FU *  * \n"
  "P9+KY+KE+GI *  * -RY *  * -UM\n"
  "P-00FU00FU\n"
  "+\n";

auto DATA_Y =
  "'-- DATA_Y ------------------\n"
  "P1+HI *  *  *  * -OU * -KE-KY\n"
  "P2 *  *  *  *  *  * -KI *  * \n"
  "P3 *  *  * -FU * -KI-GI-FU-FU\n"
  "P4 * -FU *  *  *  * -KY *  * \n"
  "P5 *  *  *  * +FU *  *  * +FU\n"
  "P6 *  * +KI+FU * -FU *  *  * \n"
  "P7-NY+FU+OU *  * +FU+GI+FU * \n"
  "P8 *  *  * +GI *  *  * +HI * \n"
  "P9 *  *  *  *  *  *  * +KE+KY\n"
  "P+00GI00KE00FU00FU00FU00FU\n"
  "P-00KA00KA00KI00KE00FU00FU00FU\n"
  "-\n";
}

BENCHMARK(GenerateCaptures, [](BenchmarkController& bc, bmstr_t data) {
  Position pos = PositionUtil::createPositionFromCsaString(data);

  bc.start();
  while(bc.cont()) {
    Moves moves;
    MoveGenerator::generateCaptures(pos, moves);
  }
})->args(BMSTR(DATA_A))
  ->args(BMSTR(DATA_B));

BENCHMARK(GenerateQuiets, [](BenchmarkController& bc, bmstr_t data) {
  Position pos = PositionUtil::createPositionFromCsaString(data);

  bc.start();
  while(bc.cont()) {
    Moves moves;
    MoveGenerator::generateQuiets(pos, moves);
  }
})->args(BMSTR(DATA_A))
  ->args(BMSTR(DATA_B));

BENCHMARK(GenerateEvasions, [](BenchmarkController& bc, bmstr_t data) {
  Position pos = PositionUtil::createPositionFromCsaString(data);
  CheckState cs = pos.getCheckState();

  bc.start();
  while(bc.cont()) {
    Moves moves;
    MoveGenerator::generateEvasions(pos, cs, moves);
  }
})->args(BMSTR(DATA_X))
  ->args(BMSTR(DATA_Y));
