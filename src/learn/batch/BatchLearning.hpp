/* BatchLearning.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_LEARN_BATCH_BATCHLEARNING_HPP__
#define SUNFISH_LEARN_BATCH_BATCHLEARNING_HPP__

#include "common/time/Timer.hpp"
#include "common/math/Random.hpp"
#include "core/move/Move.hpp"
#include "learn/batch/Gradient.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <memory>

namespace sunfish {

class Position;
class Evaluator;
struct Gradient;

class BatchLearning {
public:

  struct Config {
    std::string kifuDir;
    int iteration;
    int numThreads;
    int depth;
    float norm;
  };

  BatchLearning();

  bool run();

private:

  void readConfigFromIniFile();

  bool validateConfig();

  bool iterate();

  bool generateTrainingData();

  bool generateTrainingData(std::ostream& os,
                            const std::string& path,
                            int& failLoss,
                            int& numberOfData);

  bool generateTrainingData(std::ostream& os,
                            Position& pos,
                            Move bestMove,
                            int& failLoss,
                            int& numberOfData);

  bool generateGradient();

  void generateGradient(std::istream& is,
                        Gradient& g,
                        float& lsum);

  void generateGradient(const Position& rootPos,
                        const std::vector<std::vector<Move>>& trainingData,
                        Gradient& g,
                        float& lsum);

  void updateParameters();

  void printSummary();

private:

  Config config_;
  Timer timer_;
  Random random_;

  int failLoss_;
  float loss_;
  int numberOfData_;

  std::shared_ptr<Evaluator> evaluator_;
  std::unique_ptr<Gradient> gradient_;

};

} // namespace sunfish

#endif // SUNFISH_LEARN_BATCH_BATCHLEARNING_HPP__
