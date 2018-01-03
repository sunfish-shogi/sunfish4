/* OnlineLearning.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_LEARN_ONLINE_ONLINELEARNING_HPP__
#define SUNFISH_LEARN_ONLINE_ONLINELEARNING_HPP__

#include "common/time/Timer.hpp"
#include "common/math/Random.hpp"
#include "search/eval/Evaluator.hpp"
#include "learn/gradient/Gradient.hpp"
#include "learn/training_data/TrainingData.hpp"
#include <string>
#include <thread>
#include <memory>
#include <thread>
#include <queue>
#include <mutex>

namespace sunfish {

class Searcher;

class OnlineLearning {
public:

  struct Config {
    std::string trainingData;
    int numThreads;
    int depth;
    float norm;
    float eta;
    int miniBatchSize;
    int maxBrothers;
  };

private:

  using TrainingDataQueue = std::queue<TrainingDataElement>;

  struct Thread {
    std::thread thread;
    std::unique_ptr<Searcher> searcher;
    OptimizedGradient og;
    float loss;
    int numberOfData;
    Random random;
  };

  void readConfigFromIniFile();

  bool validateConfig();

  bool iterateMiniBatch();

  void generateGradient(Thread& th);

public:

  OnlineLearning();

  bool run();

private:

  Config config_;
  Timer timer_;
  Random random_;

  TrainingDataQueue queue_;
  std::mutex mutex_;

  std::shared_ptr<Evaluator> evaluator_;
  std::unique_ptr<Evaluator::FVType> fv_;
  std::unique_ptr<FeatureVector<float>> f_;
  std::unique_ptr<FeatureVector<float>> av_; // Difference of Averaged Perceptron
  std::unique_ptr<FeatureVector<float>> ag_; // Ada Gard

};

} // namespace sunfish

#endif // SUNFISH_LEARN_ONLINE_ONLINELEARNING_HPP__
