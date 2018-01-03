/* OnlineLearning.cpp
 *
 * Kubo Ryosuke
 */

#include "learn/online/OnlineLearning.hpp"
#include "learn/util/LearningUtil.hpp"
#include "search/eval/FeatureTemplates.hpp"
#include "search/eval/Evaluator.hpp"
#include "search/Searcher.hpp"
#include "core/move/MoveGenerator.hpp"
#include "core/record/CsaReader.hpp"
#include "core/record/SfenParser.hpp"
#include "common/resource/Resource.hpp"
#include "common/string/StringUtil.hpp"
#include "common/file_system/Directory.hpp"
#include <string>
#include <fstream>

namespace {

const char* const OnlineLearnIni = "config/online_learn.ini";

CONSTEXPR_CONST int DefaultDepth = 2;
CONSTEXPR_CONST float DefaultNorm = 1.0e-2f;
CONSTEXPR_CONST float DefaultEta = 10.0;
CONSTEXPR_CONST int DefaultMiniBatchSize = 100;
CONSTEXPR_CONST int DefaultMaxBrothers = 16;

CONSTEXPR_CONST int SearchWindow =  256;

inline float gain() {
  return 7.0f / SearchWindow;
}

inline float sigmoid(float x) {
  if (x <= -SearchWindow) { return 0.0f; }
  if (x >=  SearchWindow) { return 1.0f; }
  return 1.0 / (1.0 + std::exp(x * -gain()));
}

inline float dsigmoid(float x) {
  float s = sigmoid(x);
  return (s - s * s) * gain();
}

inline float loss(float x) {
  return sigmoid(x);
}

inline float gradient(float x) {
  return dsigmoid(x);
}

inline float gnorm(float x) {
  if      (x > 0) { return -1.0f; }
  else if (x < 0) { return  1.0f; }
  else            { return  0.0f; }
}

} // namespace

namespace sunfish {

OnlineLearning::OnlineLearning() :
    evaluator_(std::make_shared<Evaluator>(Evaluator::InitType::Zero)),
    fv_(new Evaluator::FVType()),
    f_(new FeatureVector<float>()),
    av_(new FeatureVector<float>()),
    ag_(new FeatureVector<float>()) {
}

bool OnlineLearning::run() {
  MSG(info) << "####################################################################";
  MSG(info) << "##                        OnlineLearning                          ##";
  MSG(info) << "####################################################################";

  timer_.start();

  readConfigFromIniFile();
  if (!validateConfig()) {
    return false;
  }

  if (!iterateMiniBatch()) {
    return false;
  }

  auto elapsed = timer_.elapsed();
  MSG(info) << "completed";
  MSG(info) << "elapsed: " << elapsed;

  return true;
}

void OnlineLearning::readConfigFromIniFile() {
  auto ini = Resource::ini(OnlineLearnIni);

  config_.trainingData      = getValue(ini, "Learn", "TrainingData");
  config_.numThreads        = StringUtil::toInt(getValue(ini, "Learn", "NumThreads"), std::thread::hardware_concurrency());
  config_.depth             = StringUtil::toInt(getValue(ini, "Learn", "Depth"), DefaultDepth);
  config_.norm              = StringUtil::toFloat(getValue(ini, "Learn", "Norm"), DefaultNorm);
  config_.eta               = StringUtil::toFloat(getValue(ini, "Learn", "Eta"), DefaultEta);
  config_.miniBatchSize     = StringUtil::toInt(getValue(ini, "Learn", "MiniBatchSize"), DefaultMiniBatchSize);
  config_.maxBrothers       = StringUtil::toInt(getValue(ini, "Learn", "MaxBrothers"), DefaultMaxBrothers);

  MSG(info) << "TrainingData : " << config_.trainingData;
  MSG(info) << "NumThreads   : " << config_.numThreads;
  MSG(info) << "Depth        : " << config_.depth;
  MSG(info) << "Norm         : " << config_.norm;
  MSG(info) << "Eta          : " << config_.eta;
  MSG(info) << "MiniBatchSize: " << config_.miniBatchSize;
  MSG(info) << "MaxBrothers  : " << config_.maxBrothers;
  MSG(info) << "";
}

bool OnlineLearning::validateConfig() {
  if (config_.numThreads <= 0) {
    LOG(error) << "NumThreads shall not be less than 1.";
    return false;
  }

  return true;
}

bool OnlineLearning::iterateMiniBatch() {
  std::vector<Thread> threads(config_.numThreads);
  for (auto& th : threads) {
    th.searcher.reset(new Searcher(evaluator_));
  }

  TrainingDataReader reader;
  if (!reader.open(config_.trainingData)) {
    LOG(error) << "failed to open training data file";
    return false;
  }

  for (int mbi = 1; ; mbi++) {
    MSG(info) << "Mini Batch - " << mbi;

    for (int i = 0; i < config_.miniBatchSize; i++) {
      TrainingDataElement td;
      if (!reader.read(td)) {
        if (i == 0) {
          goto label_end;
        } else {
          break;
        }
      }
      queue_.push(std::move(td));
    }

    for (auto& th : threads) {
      th.thread = std::thread([this, &th]() {
        generateGradient(th);
      });
    }

    for (auto& th : threads) {
      if (th.thread.joinable()) {
        th.thread.join();
      }
    }

    float loss = 0.0;
    int numberOfData = 0;
    auto og = std::unique_ptr<OptimizedGradient>(new OptimizedGradient);
    auto gradient = std::unique_ptr<Gradient>(new Gradient);
    memset(reinterpret_cast<void*>(og.get()), 0, sizeof(OptimizedGradient));
    for (auto& th : threads) {
      loss += th.loss;
      numberOfData += th.numberOfData;
      add(*og, th.og);
    }
    expand(*gradient, *og);
    symmetrize(*gradient, [](float& g1, float& g2) {
      g1 = g2 = g1 + g2;
    });

    each(*gradient, *ag_, *f_, *av_, *fv_, [this, mbi](float& g0, float& ag, float& f, float& av, int16_t& v) {
      float g = 0;
      if (g0 != 0.0) {
        ag += g0 * g0;
        g += config_.eta * g0 / sqrtf(ag);
      }
      float gn = gnorm(f);
      if (gn != 0.0) {
        g += config_.norm * gn / mbi;
      }
      f += g;
      av += g * mbi;
      v = int16_t(f);
    });

    optimize(*fv_, evaluator_->ofv());

    each(*f_, *av_, *fv_, [mbi](float& f, float& av, int16_t& v) {
      v = int16_t(f - av / mbi);
    });

    save(*fv_);

    MSG(info) << "Loss: " << (loss / numberOfData);
    MSG(info) << "";
  }

label_end:

  LearningUtil::printFVSummary(fv_.get());
  MSG(info) << "";

  return true;
}

void OnlineLearning::generateGradient(Thread& th) {
  memset(reinterpret_cast<void*>(&th.og), 0, sizeof(th.og));
  th.loss = 0.0;
  th.numberOfData = 0;

  while (true) {
    TrainingDataElement data;
    {
      std::lock_guard<std::mutex> lock(mutex_);
      if (queue_.empty()) {
        return;
      }

      data = queue_.front();
      queue_.pop();
    }

    Position rootPos;
    if (!SfenParser::parsePosition(data.sfen, rootPos)) {
      LOG(error) << "invalid SFEN: " << data.sfen;
      continue;
    }

    int depth = Searcher::Depth1Ply * config_.depth + Searcher::Depth1Ply / 2;
    Score alpha;
    Score beta;

    Position pos0 = rootPos;
    Score score0;
    {
      int newDepth = depth;
      if (pos0.isCheck(data.move)){
        newDepth += Searcher::Depth1Ply;
      }

      Piece captured;
      if (!pos0.doMove(data.move, captured)) {
        LOG(error) << "an illegal move is detected: " << data.move.toString(pos0) << "\n"
                   << pos0.toString();
        return;
      }

      th.searcher->search(pos0,
                          newDepth,
                          -Score::mate(),
                          Score::mate());
      auto& result = th.searcher->getResult();
      score0 = -result.score;

      if (score0 >= Score::mate() ||
          score0 <= -Score::mate()) {
        return;
      }
      alpha = score0 - SearchWindow;
      beta = score0 + SearchWindow;

      for (PV::SizeType pvi = 0; pvi < result.pv.size(); pvi++) {
        Move move = result.pv.getMove(pvi);
        if (!pos0.doMove(move, captured)) {
          LOG(error) << "an illegal move is detected:\n"
                     << pos0.toString()
                     << move.toString(pos0);
          return;
        }
      }
    }

    Moves moves;
    auto cs = rootPos.getCheckState();
    if (!isCheck(cs)) {
      MoveGenerator::generateCaptures(rootPos, moves);
      MoveGenerator::generateQuiets(rootPos, moves);
    } else {
      MoveGenerator::generateEvasions(rootPos, cs, moves);
    }

    th.random.shuffle(moves.begin(), moves.end());

    int count = 0;
    float d0 = 0.0f;
    for (auto& move : moves) {
      if (count >= config_.maxBrothers) {
        break;
      }

      if (move == data.move) {
        continue;
      }

      Position pos = rootPos;

      int newDepth = depth;
      if (pos.isCheck(move)){
        newDepth += Searcher::Depth1Ply;
      }

      Piece captured;
      if (!pos.doMove(move, captured)) {
        continue;
      }
      count++;

      th.searcher->search(pos,
                          newDepth,
                          -beta,
                          -alpha);

      auto& result = th.searcher->getResult();
      Score score = -result.score;

      // fail-low
      if (score <= alpha) {
        continue;
      }

      // fail-high
      if (score >= beta) {
        th.loss += 1.0;
        continue;
      }

      for (PV::SizeType pvi = 0; pvi < result.pv.size(); pvi++) {
        Move move = result.pv.getMove(pvi);
        if (!pos.doMove(move, captured)) {
          LOG(error) << "an illegal move is detected:\n"
                     << pos.toString()
                     << move.toString(pos);
          return;
        }
      }

      auto diff = score - score0;
      float l = loss(diff.raw());
      float d = gradient(diff.raw());

      th.loss += l;

      if (rootPos.getTurn() == Turn::White) {
        d = -d;
      }
      operate<FeatureOperationType::Extract>(th.og, pos, -d);
      d0 += d;
    }
    operate<FeatureOperationType::Extract>(th.og, pos0, d0);
    th.numberOfData++;
  }
}

} // namespace sunfish
