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
#include "learn/gradient/Gradient.hpp"
#include "learn/training_data/TrainingData.hpp"
#include <thread>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <mutex>

namespace sunfish {

class Position;
class Searcher;

class BatchLearning {
public:

  struct Config {
    std::string trainingData;
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
    std::ofstream os;
    std::unique_ptr<Searcher> searcher;
    int failLoss;
    int numberOfData;
  };

  struct GenGradThread {
    std::thread thread;
    std::ifstream is;
#if !MATERIAL_LEARNING_ONLY
    OptimizedGradient og;
#endif // !MATERIAL_LEARNING_ONLY
#if MATERIAL_LEARNING
    MaterialGradient mg;
#endif // MATERIAL_LEARNING
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
                            Position& pos,
                            Move bestMove);

  bool generateGradient();

  void generateGradient(GenGradThread& th);

  void generateGradient(GenGradThread& th,
                        const Position& rootPos,
                        const std::vector<std::vector<Move>>& trainingData);

  void updateParameters();

private:

  Config config_;
  Timer timer_;
  Random random_;

  int failLoss_;
  float loss_;
  int numberOfData_;

  std::unique_ptr<TrainingDataReader> reader_;
  std::mutex readerMutex_;

  std::shared_ptr<Evaluator> evaluator_;
#if !MATERIAL_LEARNING_ONLY
  std::unique_ptr<Evaluator::FVType> fv_;
  std::unique_ptr<Gradient> gradient_;
#endif // !MATERIAL_LEARNING_ONLY
#if MATERIAL_LEARNING
  MaterialGradient mgradient_;
#endif // MATERIAL_LEARNING

};

} // namespace sunfish

#endif // SUNFISH_LEARN_BATCH_BATCHLEARNING_HPP__
