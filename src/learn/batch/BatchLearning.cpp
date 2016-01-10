/* BatchLearning.cpp
 *
 * Kubo Ryosuke
 */

#include "learn/batch/BatchLearning.hpp"
#include "search/eval/Evaluator.hpp"
#include "search/eval/FeatureTemplates.hpp"
#include "search/Searcher.hpp"
#include "core/move/MoveGenerator.hpp"
#include "core/record/CsaReader.hpp"
#include "common/file_system/Directory.hpp"
#include "common/resource/Resource.hpp"
#include "common/string/TablePrinter.hpp"
#include "common/string/StringUtil.hpp"
#include "logger/Logger.hpp"
#include <thread>
#include <atomic>
#include <sstream>
#include <fstream>
#include <cmath>

namespace {

const char* const BatchLearnIni = "config/batch_learn.ini";
CONSTEXPR_CONST int DefaultIteration = 32;
CONSTEXPR_CONST int DefaultDepth = 2;
CONSTEXPR_CONST float DefaultNorm = 1.0e-2f;

CONSTEXPR_CONST int MaximumUpdateCount = 32;
CONSTEXPR_CONST int MinimumUpdateCount = 8;

CONSTEXPR_CONST int16_t Int16Max = 32767;
CONSTEXPR_CONST int16_t Int16Min = -32768;

std::string trainingDataPath(unsigned tn) {
  std::ostringstream oss;
  oss << "out/training" << tn << ".dat";
  return oss.str();
}

CONSTEXPR_CONST float SearchWindow =  256;

inline float gain() {
  return 7.0f / SearchWindow;
}

inline float sigmoid(float x) {
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

inline float norm(int16_t x, float n) {
  if      (x > 0) { return -n; }
  else if (x < 0) { return n; }
  else            { return 0.0f; }
}

} // namespace

namespace sunfish {

BatchLearning::BatchLearning() :
    evaluator_(std::make_shared<Evaluator>(Evaluator::InitType::Zero)),
    gradient_(new Gradient) {
}

bool BatchLearning::run() {
  OUT(info) << "####################################################################";
  OUT(info) << "##                         BatchLearning                          ##";
  OUT(info) << "####################################################################";

  timer_.start();

  readConfigFromIniFile();
  if (!validateConfig()) {
    return false;
  }

  bool ok = iterate();
  if (!ok) {
    return false;
  }

  auto elapsed = timer_.elapsed();
  OUT(info) << "completed";
  OUT(info) << "elapsed: " << elapsed;

  return true;
}

void BatchLearning::readConfigFromIniFile() {
  auto ini = Resource::ini(BatchLearnIni);

  config_.kifuDir = getValue(ini, "Learn", "KifuDir");
  config_.iteration = StringUtil::toInt(getValue(ini, "Learn", "Iteration"),
                                        DefaultIteration);
  config_.numThreads = StringUtil::toInt(getValue(ini, "Learn", "NumThreads"),
                                         std::thread::hardware_concurrency());
  config_.depth = StringUtil::toInt(getValue(ini, "Learn", "Depth"),
                                    DefaultDepth);
  config_.norm = StringUtil::toFloat(getValue(ini, "Learn", "Norm"),
                                     DefaultNorm);

  OUT(info) << "KifuDir   : " << config_.kifuDir;
  OUT(info) << "Iteration : " << config_.iteration;
  OUT(info) << "NumThreads: " << config_.numThreads;
  OUT(info) << "Depth     : " << config_.depth;
  OUT(info) << "Norm      : " << config_.norm;
}

bool BatchLearning::validateConfig() {
  if (config_.numThreads <= 0) {
    LOG(error) << "NumThreads shall not be less than 1.";
    return false;
  }

  return true;
}

bool BatchLearning::iterate() {
  int updateCount = MaximumUpdateCount;

  for (int i = 0; i < config_.iteration; i++) {
    OUT(info) << "";
    OUT(info) << "ITERATION - " << i;

    OUT(info) << "Training-Data-Generation Phase";

    bool ok = generateTrainingData();
    if (!ok) {
      return false;
    }

    OUT(info) << "Parameter-Adjustment Phase";

    float lossFirst = 0.0f;
    float lossLast = 0.0f;
    for (int uc = 0; uc < updateCount; uc++) {
      ok = generateGradient();
      if (!ok) {
        return false;
      }

      updateParameters();

      if (uc == 0) {
        lossFirst = loss_ / numberOfData_;
      }
      
      if (uc == updateCount - 1) {
        lossLast = loss_ / numberOfData_;
      }
    }

    OUT(info) << "Write-To-File Phase";

    evaluator_->writeEvalBin();

    OUT(info) << "";
    OUT(info) << "loss = " << lossFirst << " - " << lossLast;

    printSummary();

    updateCount = std::max(updateCount * 2 / 3, MinimumUpdateCount);
  }
  return true;
}

bool BatchLearning::generateTrainingData() {
  Directory directory(config_.kifuDir.c_str());
  auto files = directory.files("*.csa");

  if (files.size() == 0) {
    LOG(error) << ".csa files not found";
    return false;
  }

  struct ThreadInfo {
    std::thread thread;
    std::vector<std::string> files;
    std::ofstream os;
    int failLoss;
    int numberOfData;
  };
  std::vector<ThreadInfo> threads(config_.numThreads);

  for (unsigned tn = 0; tn < threads.size(); tn++) {
    auto& ti = threads[tn];

    auto path = trainingDataPath(tn);
    ti.os.open(path, std::ios::out | std::ios::binary);
    if (!ti.os) {
      LOG(error) << "could not open a file: " << path;
      return false;
    }

    ti.failLoss = 0;
    ti.numberOfData = 0;
  }

  int tn = 0;
  for (const auto& path : files) {
    threads[tn].files.push_back(path);
    tn = (tn + 1) % config_.numThreads;
  }

  std::atomic<int> succ(0);
  std::atomic<int> fail(0);
  for (unsigned tn = 0; tn < threads.size(); tn++) {
    auto& ti = threads[tn];
    ti.thread = std::thread([this, &ti, &succ, &fail]() {
      for (const auto& path : ti.files) {
        bool ok = generateTrainingData(ti.os,
                                       path,
                                       ti.failLoss,
                                       ti.numberOfData);
        if (ok) {
          succ++;
        } else {
          fail++;
        }
      }
    });
  }

  for (auto& ti : threads) {
    if (ti.thread.joinable()) {
      ti.thread.join();
    }
  }

  failLoss_ = 0;
  numberOfData_ = 0;
  for (auto& ti : threads) {
    failLoss_ += ti.failLoss;
    numberOfData_ += ti.numberOfData;
  }

  OUT(info) << "success: " << succ;
  OUT(info) << "fail   : " << fail;

  return true;
}

bool BatchLearning::generateTrainingData(std::ostream& os,
                                         const std::string& path,
                                         int& failLoss,
                                         int& numberOfData) {
  std::ifstream file(path);
  if (!file) {
    LOG(error) << "could not open a file: " << path;
    return false;
  }
  Record record;
  CsaReader::read(file, record);
  file.close();

  Position pos = record.initialPosition;
  for (const auto& move : record.moveList) {
    if (!generateTrainingData(os,
                              pos,
                              move,
                              failLoss,
                              numberOfData)) {
      return false;
    }

    Piece captured;
    if (!pos.doMove(move, captured)) {
      LOG(error) << "an illegal move is detected: " << move.toString(pos) << "\n"
                 << pos.toString();
      return false;
    }
  }
  return true;
}

bool BatchLearning::generateTrainingData(std::ostream& os,
                                         Position& pos,
                                         Move bestMove,
                                         int& failLoss,
                                         int& numberOfData) {
  Searcher searcher(evaluator_);
  int depth = config_.depth * Searcher::Depth1Ply;
  Score alpha;
  Score beta;
  struct Data {
    Move move;
    PV pv;
  };
  std::vector<Data> results;

  {
    Piece captured;
    if (!pos.doMove(bestMove, captured)) {
      LOG(error) << "an illegal move is detected: " << bestMove.toString(pos) << "\n"
                 << pos.toString();
      return false;
    }

    searcher.search(pos,
                    depth,
                    -Score::mate(),
                    Score::mate());
    auto& result = searcher.getResult();

    pos.undoMove(bestMove, captured);

    if (result.score >= Score::mate() ||
        result.score <= -Score::mate()) {
      return true;
    }
    alpha = -result.score - SearchWindow;
    beta = -result.score + SearchWindow;
    results.push_back({ bestMove, result.pv });
  }

  Moves moves;
  auto cs = pos.getCheckState();
  if (!isCheck(cs)) {
    MoveGenerator::generateCapturingMoves(pos, moves);
    MoveGenerator::generateNotCapturingMoves(pos, moves);
  } else {
    MoveGenerator::generateEvasions(pos, cs, moves);
  }

  for (auto& move : moves) {
    if (move == bestMove) {
      continue;
    }

    Piece captured;
    if (!pos.doMove(move, captured)) {
      continue;
    }
    searcher.search(pos,
                    depth,
                    -beta,
                    -alpha);
    pos.undoMove(move, captured);

    auto& result = searcher.getResult();

    // fail-low
    if (-result.score <= alpha) {
      continue;
    }

    // fail-high
    if (-result.score >= beta) {
      failLoss++;
      continue;
    }

    results.push_back({ move, result.pv });
  }

  numberOfData++;

  if (results.size() == 1) {
    return true;
  }

  auto mp = pos.getMutablePosition();
  os.write(reinterpret_cast<char*>(&mp), sizeof(MutablePosition));

  for (const auto& result : results) {
    uint8_t length = result.pv.size() + 1;
    os.write(reinterpret_cast<char*>(&length), sizeof(uint8_t));
    uint16_t firstMove = result.move.serialize16();
    os.write(reinterpret_cast<char*>(&firstMove), sizeof(uint16_t));
    for (unsigned i = 0; i < result.pv.size(); i++) {
      uint16_t move = result.pv.getMove(i).serialize16();
      os.write(reinterpret_cast<char*>(&move), sizeof(uint16_t));
    }
  }
  uint8_t end = 0;
  os.write(reinterpret_cast<char*>(&end), sizeof(uint8_t));

  return true;
}

bool BatchLearning::generateGradient() {
  struct ThreadInfo {
    std::thread thread;
    std::ifstream is;
    Gradient gradient;
    float loss;
  };
  std::vector<ThreadInfo> threads(config_.numThreads);

  for (unsigned tn = 0; tn < threads.size(); tn++) {
    auto& ti = threads[tn];

    auto path = trainingDataPath(tn);
    ti.is.open(path, std::ios::in | std::ios::binary);
    if (!ti.is) {
      LOG(error) << "could not open a file: " << path;
      return false;
    }

    memset(reinterpret_cast<void*>(&ti.gradient), 0, sizeof(ti.gradient));
    ti.loss = 0.0f;
  }

  for (unsigned tn = 0; tn < threads.size(); tn++) {
    auto& ti = threads[tn];
    ti.thread = std::thread([this, &ti]() {
      generateGradient(ti.is, ti.gradient, ti.loss);
    });
  }

  for (auto& ti : threads) {
    if (ti.thread.joinable()) {
      ti.thread.join();
    }
  }

  loss_ = failLoss_;
  memset(reinterpret_cast<void*>(gradient_.get()), 0, sizeof(Gradient));
  for (auto& ti : threads) {
    loss_ += ti.loss;
    add(gradient_->g, ti.gradient.g);
    add(gradient_->c, ti.gradient.c);
  }
  rcumulate(gradient_->g, gradient_->c);
  symmetrize(gradient_->g, [](float& g1, float& g2) {
    g1 = g2 = g1 + g2;
  });

  return true;
}

void BatchLearning::generateGradient(std::istream& is,
                                     Gradient& g,
                                     float& lsum) {
  for (;;) {
    MutablePosition mp;
    is.read(reinterpret_cast<char*>(&mp), sizeof(MutablePosition));

    if (is.eof()) {
      return;
    }

    std::vector<std::vector<Move>> trainingData;
    for (;;) {
      uint8_t length;
      is.read(reinterpret_cast<char*>(&length), sizeof(uint8_t));
      if (length == 0) { break; }

      std::vector<Move> pv(length);
      for (unsigned i = 0; i < length; i++) {
        uint16_t move;
        is.read(reinterpret_cast<char*>(&move), sizeof(uint16_t));
        pv[i] = Move::deserialize(move);
      }
      trainingData.push_back(std::move(pv));
    }

    generateGradient(Position(mp), trainingData, g, lsum);
  }
}

void BatchLearning::generateGradient(const Position& rootPos,
                                     const std::vector<std::vector<Move>>& trainingData,
                                     Gradient& g,
                                     float& lsum) {
  Position pos0 = rootPos;
  Score score0;
  {
    const auto& pv = trainingData[0];
    for (auto& move : pv) {
      Piece captured;
      if (!pos0.doMove(move, captured)) {
        LOG(error) << "an illegal move is detected:\n"
                   << pos0.toString()
                   << move.toString(pos0);
        return;
      }
    }
    Score materialScore = evaluator_->calculateMaterialScore(pos0);
    score0 = evaluator_->calculateTotalScore(materialScore,
                                             pos0);
  }

  for (unsigned i = 1; i < trainingData.size(); i++) {
    Position pos = rootPos;
    const auto& pv = trainingData[i];
    for (auto& move : pv) {
      Piece captured;
      if (!pos.doMove(move, captured)) {
        LOG(error) << "an illegal move is detected:\n"
                   << pos.toString()
                   << move.toString(pos);
        return;
      }
    }

    Score materialScore = evaluator_->calculateMaterialScore(pos);
    auto score = evaluator_->calculateTotalScore(materialScore,
                                                 pos);
    if (rootPos.getTurn() == Turn::White) {
      score0 = -score0;
      score = -score;
    }

    auto diff = score - score0;
    float l = loss(diff.raw());
    float d = gradient(diff.raw());

    lsum += l;

    if (rootPos.getTurn() == Turn::White) {
      d = -d;
    }
    operate<FeatureOperationType::Extract>(g.g, g.c, pos0, d);
    operate<FeatureOperationType::Extract>(g.g, g.c, pos, -d);
  }
}

void BatchLearning::updateParameters() {
  each(evaluator_->fv(), gradient_->g, [this](int16_t& e, float& g) {
    float n = norm(e, config_.norm);
    int16_t step = random_.bit() + random_.bit();
    if      (g + n > 0.0f && e <= Int16Max - step) { e += step; }
    else if (g + n < 0.0f && e >= Int16Min + step) { e -= step; }
    else if (g + n != 0.0f) { LOG(warning) << "A parameter is out of bounce."; }
  });

  symmetrize(evaluator_->fv(), [](int16_t& e1, int16_t& e2) {
    e1 = e2;
  });

  evaluator_->onChanged();
}

void BatchLearning::printSummary() {
  auto summary = summarize(evaluator_->fv());
  TablePrinter tp;

  tp.row() << "name"
           << "num"
           << "zero"
           << "non-zero"
           << "max"
           << "min"
           << "max-abs"
           << "ave-abs"
           << "";
  for (const auto& s : summary) {
    tp.row() << s.name
             << s.num
             << s.zero
             << s.nonZero
             << s.max
             << s.min
             << s.maxAbs
             << s.aveAbs;
  }

  OUT(info) << "";
  OUT(info) << StringUtil::chomp(tp.stringify());
}

} // namespace sunfish
