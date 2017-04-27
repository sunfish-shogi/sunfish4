/* BatchLearning.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_LEARN_BATCH_BATCHLEARNING_HPP__
#define SUNFISH_LEARN_BATCH_BATCHLEARNING_HPP__

#include "common/time/Timer.hpp"
#include "common/math/Random.hpp"
#include "core/move/Move.hpp"
#include "search/eval/Evaluator.hpp"
#include "learn/batch/Gradient.hpp"
#include <thread>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <memory>

namespace sunfish {

class Position;
class Searcher;
class RecordQueue;

class BatchLearning {
public:

  struct Config {
    std::string kifuDir;
    int iteration;
    int restart;
    int restartIteration;
    int numThreads;
    int depth;
    float norm;
  };

private:

  struct GenTrDataThread {
    std::thread thread;
    RecordQueue* recordQueue;
    std::ofstream os;
    std::unique_ptr<Searcher> searcher;
    int failLoss;
    int numberOfData;
  };

  struct GenGradThread {
    std::thread thread;
    std::ifstream is;
    OptimizedGradient og;
    MaterialGradient mg;
    float loss;
  };

public:

  BatchLearning();

  bool run();

private:

  void readConfigFromIniFile();

  bool validateConfig();

  bool iterate();

  bool generateTrainingData();

  void generateTrainingData(GenTrDataThread& th);

  void generateTrainingData(GenTrDataThread& th,
                            const std::string& path);

  void generateTrainingData(GenTrDataThread& th,
                            Position& pos,
                            Move bestMove);

  bool generateGradient();

  void generateGradient(GenGradThread& th);

  void generateGradient(GenGradThread& th,
                        const Position& rootPos,
                        const std::vector<std::vector<Move>>& trainingData);

  void updateParameters();

  void printParametersSummary();

private:

  Config config_;
  Timer timer_;
  Random random_;

  int failLoss_;
  float loss_;
  int numberOfData_;

  std::shared_ptr<Evaluator> evaluator_;
  std::unique_ptr<Evaluator::FVType> fv_;
  std::unique_ptr<Gradient> gradient_;
  MaterialGradient mgradient_;

};

} // namespace sunfish

#endif // SUNFISH_LEARN_BATCH_BATCHLEARNING_HPP__
