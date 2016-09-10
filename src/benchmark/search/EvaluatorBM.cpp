/* EvaluatorBM.cpp
 *
 * Kubo Ryosuke
 */

#include "benchmark/Benchmark.hpp"
#include "core/record/CsaReader.hpp"
#include "core/util/PositionUtil.hpp"
#include "search/eval/Evaluator.hpp"
#include "logger/Logger.hpp"
#include <memory>

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
auto DATA_A_M1 = "+7776FU";
auto DATA_A_M2 = "+2726FU";

auto DATA_B =
  "'-- DATA_B ------------------\n"
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
auto DATA_B_M1 = "+7988GI";
auto DATA_B_M2 = "+7877OU";

auto DATA_C =
  "'-- DATA_C ------------------\n"
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
auto DATA_C_M1 = "-0051FU";
auto DATA_C_M2 = "-4152OU";

} // namespace

BENCHMARK(CalculateMaterialScore, [](BenchmarkController& bc, bmstr_t data) {
  Position pos = PositionUtil::createPositionFromCsaString(data);
  std::unique_ptr<Evaluator> eval(new Evaluator(Evaluator::InitType::Zero));

  bc.start();
  while(bc.cont()) {
    eval->calculateMaterialScore(pos);
  }
})->args(BMSTR(DATA_A))
  ->args(BMSTR(DATA_B))
  ->args(BMSTR(DATA_C));

BENCHMARK(CalculateMaterialScoreDiff, [](BenchmarkController& bc, bmstr_t data1, bmstr_t data2) {
  Position pos = PositionUtil::createPositionFromCsaString(data1);
  Move move;
  if (!CsaReader::readMove(data2, pos, move)) {
    LOG(error) << "invalid move: " << data2;
    exit(1);
  }
  Piece captured = pos.getPieceOnBoard(move.to());
  std::unique_ptr<Evaluator> eval(new Evaluator(Evaluator::InitType::Zero));

  bc.start();
  while(bc.cont()) {
    eval->calculateMaterialScoreDiff(Score::zero(),
                                     pos,
                                     move,
                                     captured);
  }
})->args(BMSTR(DATA_A), BMSTR(DATA_A_M1))
  ->args(BMSTR(DATA_A), BMSTR(DATA_A_M2))
  ->args(BMSTR(DATA_B), BMSTR(DATA_B_M1))
  ->args(BMSTR(DATA_B), BMSTR(DATA_B_M2))
  ->args(BMSTR(DATA_C), BMSTR(DATA_C_M1))
  ->args(BMSTR(DATA_C), BMSTR(DATA_C_M2));

BENCHMARK(CalculateTotalScore, [](BenchmarkController& bc, bmstr_t data) {
  Position pos = PositionUtil::createPositionFromCsaString(data);
  std::unique_ptr<Evaluator> eval(new Evaluator(Evaluator::InitType::Zero));

  bc.start();
  while(bc.cont()) {
    eval->calculatePositionalScore(pos);
  }
})->args(BMSTR(DATA_A))
  ->args(BMSTR(DATA_B))
  ->args(BMSTR(DATA_C));
