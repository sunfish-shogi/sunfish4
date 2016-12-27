/* BatchLearning.cpp
 *
 * Kubo Ryosuke
 */

#include "learn/batch/BatchLearning.hpp"
#include "search/eval/FeatureTemplates.hpp"
#include "search/eval/Material.hpp"
#include "search/Searcher.hpp"
#include "core/move/MoveGenerator.hpp"
#include "core/record/CsaReader.hpp"
#include "common/file_system/Directory.hpp"
#include "common/resource/Resource.hpp"
#include "common/string/TablePrinter.hpp"
#include "common/string/StringUtil.hpp"
#include "logger/Logger.hpp"
#include <atomic>
#include <sstream>
#include <utility>
#include <mutex>
#include <cmath>

#define DEBUG_PRINT 0

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

inline float norm(int16_t x, float n) {
  if      (x > 0) { return -n; }
  else if (x < 0) { return n; }
  else            { return 0.0f; }
}

} // namespace

namespace sunfish {

class RecordQueue {
public:

  using QueueType = Directory::Files;

  template <class T>
  RecordQueue(T&& files) : files_(std::forward<T>(files)) {
    iterator_ = files_.begin();
  }

  RecordQueue() = delete;
  RecordQueue(const RecordQueue&) = delete;
  RecordQueue(RecordQueue&&) = delete;

  bool pop(const std::string** path) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (iterator_ == files_.end()) {
      return false;
    }
    *path = &(*iterator_);
    iterator_++;
    return true;
  }

private:

  QueueType files_;
  QueueType::iterator iterator_;
  std::mutex mutex_;

};

BatchLearning::BatchLearning() :
    evaluator_(std::make_shared<Evaluator>(Evaluator::InitType::Zero)),
    fv_(new Evaluator::FVType),
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

  config_.kifuDir           = getValue(ini, "Learn", "KifuDir");
  config_.iteration         = StringUtil::toInt(getValue(ini, "Learn", "Iteration"), DefaultIteration);
  config_.restart           = StringUtil::toInt(getValue(ini, "Learn", "Restart"), 0);
  config_.restartIteration  = StringUtil::toInt(getValue(ini, "Learn", "RestartIteration"), 0);
  config_.numThreads        = StringUtil::toInt(getValue(ini, "Learn", "NumThreads"), std::thread::hardware_concurrency());
  config_.depth             = StringUtil::toInt(getValue(ini, "Learn", "Depth"), DefaultDepth);
  config_.norm              = StringUtil::toFloat(getValue(ini, "Learn", "Norm"), DefaultNorm);

  OUT(info) << "KifuDir         : " << config_.kifuDir;
  OUT(info) << "Iteration       : " << config_.iteration;
  OUT(info) << "Restart         : " << config_.restart;
  OUT(info) << "RestartIteration: " << config_.restartIteration;
  OUT(info) << "NumThreads      : " << config_.numThreads;
  OUT(info) << "Depth           : " << config_.depth;
  OUT(info) << "Norm            : " << config_.norm;
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

  if (config_.restart) {
    load(*fv_);
    optimize(*fv_, evaluator_->ofv());
    evaluator_->onChanged(Evaluator::DataSourceType::Custom);
  }

  for (int i = 0; i < config_.iteration; i++) {
    if (!config_.restart || i >= config_.restartIteration) {
      OUT(info) << "";
      OUT(info) << "ITERATION - " << i;

      OUT(info) << "generating training data..";

      bool ok = generateTrainingData();
      if (!ok) {
        return false;
      }

      OUT(info) << "adjusting parameters..";

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

      OUT(info) << "writing to file..";

      save(*fv_);

      OUT(info) << "";
      OUT(info) << "loss = " << lossFirst << " - " << lossLast;

      printParametersSummary();
    }

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

  RecordQueue recordQueue(std::move(files));
  std::vector<GenTrDataThread> threads(config_.numThreads);

  for (unsigned tn = 0; tn < threads.size(); tn++) {
    auto& th = threads[tn];

    auto path = trainingDataPath(tn);
    th.os.open(path, std::ios::out | std::ios::binary);
    if (!th.os) {
      LOG(error) << "could not open a file: " << path;
      return false;
    }

    th.recordQueue = &recordQueue;
    th.searcher.reset(new Searcher(evaluator_));
    th.failLoss = 0;
    th.numberOfData = 0;
  }

  for (unsigned tn = 0; tn < threads.size(); tn++) {
    auto& th = threads[tn];
    th.thread = std::thread([this, &th]() {
      generateTrainingData(th);
    });
  }

  for (auto& th : threads) {
    if (th.thread.joinable()) {
      th.thread.join();
    }
  }

  failLoss_ = 0;
  numberOfData_ = 0;
  for (auto& th : threads) {
    failLoss_ += th.failLoss;
    numberOfData_ += th.numberOfData;
  }

  return true;
}

void BatchLearning::generateTrainingData(GenTrDataThread& th) {
  for (;;) {
    const std::string* path;
    if (!th.recordQueue->pop(&path)) {
      break;
    }
    generateTrainingData(th, *path);
  }
}

void BatchLearning::generateTrainingData(GenTrDataThread& th,
                                         const std::string& path) {
  std::ifstream file(path);
  if (!file) {
    LOG(error) << "could not open a file: " << path;
    return;
  }
  Record record;
  CsaReader::read(file, record);
  file.close();

  th.searcher->clean();

  Position pos = record.initialPosition;
  for (const auto& move : record.moveList) {
    generateTrainingData(th, pos, move);

    Piece captured;
    if (!pos.doMove(move, captured)) {
      LOG(error) << "an illegal move is detected: " << move.toString(pos) << "\n"
                 << pos.toString();
      return;
    }
  }
}

void BatchLearning::generateTrainingData(GenTrDataThread& th,
                                         Position& pos,
                                         Move bestMove) {
  int depth = Searcher::Depth1Ply * config_.depth + Searcher::Depth1Ply / 2;
  Score alpha;
  Score beta;
  struct Data {
    Move move;
    PV pv;
  };
  std::vector<Data> results;

  {
    int newDepth = depth;
    if (pos.isCheck(bestMove)){
      newDepth += Searcher::Depth1Ply;
    }

    Piece captured;
    if (!pos.doMove(bestMove, captured)) {
      LOG(error) << "an illegal move is detected: " << bestMove.toString(pos) << "\n"
                 << pos.toString();
      return;
    }

    th.searcher->search(pos,
                        newDepth,
                        -Score::mate(),
                        Score::mate());
    auto& result = th.searcher->getResult();

    pos.undoMove(bestMove, captured);

    if (result.score >= Score::mate() ||
        result.score <= -Score::mate()) {
      return;
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

    int newDepth = depth;
    if (pos.isCheck(move)){
      newDepth += Searcher::Depth1Ply;
    }

    Piece captured;
    if (!pos.doMove(move, captured)) {
      continue;
    }
    th.searcher->search(pos,
                        newDepth,
                        -beta,
                        -alpha);
    pos.undoMove(move, captured);

    auto& result = th.searcher->getResult();

    // fail-low
    if (-result.score <= alpha) {
      continue;
    }

    // fail-high
    if (-result.score >= beta) {
      th.failLoss++;
      continue;
    }

    results.push_back({ move, result.pv });
  }

  th.numberOfData++;

  if (results.size() == 1) {
    return;
  }

  auto mp = pos.getMutablePosition();
  th.os.write(reinterpret_cast<char*>(&mp), sizeof(MutablePosition));

  for (const auto& result : results) {
    uint8_t length = result.pv.size() + 1;
    th.os.write(reinterpret_cast<char*>(&length), sizeof(uint8_t));
    uint16_t firstMove = result.move.serialize16();
    th.os.write(reinterpret_cast<char*>(&firstMove), sizeof(uint16_t));
    for (unsigned i = 0; i < result.pv.size(); i++) {
      uint16_t move = result.pv.getMove(i).serialize16();
      th.os.write(reinterpret_cast<char*>(&move), sizeof(uint16_t));
    }
  }
  uint8_t end = 0;
  th.os.write(reinterpret_cast<char*>(&end), sizeof(uint8_t));
}

bool BatchLearning::generateGradient() {
  std::vector<GenGradThread> threads(config_.numThreads);

  for (unsigned tn = 0; tn < threads.size(); tn++) {
    auto& th = threads[tn];

    auto path = trainingDataPath(tn);
    th.is.open(path, std::ios::in | std::ios::binary);
    if (!th.is) {
      LOG(error) << "could not open a file: " << path;
      return false;
    }

    memset(reinterpret_cast<void*>(&th.og), 0, sizeof(th.og));
    memset(reinterpret_cast<void*>(&th.mg), 0, sizeof(th.mg));
    th.loss = 0.0f;
  }

  for (unsigned tn = 0; tn < threads.size(); tn++) {
    auto& th = threads[tn];
    th.thread = std::thread([this, &th]() {
      generateGradient(th);
    });
  }

  for (auto& th : threads) {
    if (th.thread.joinable()) {
      th.thread.join();
    }
  }

  loss_ = failLoss_;
  auto og = std::unique_ptr<OptimizedGradient>(new OptimizedGradient);
  memset(reinterpret_cast<void*>(og.get()), 0, sizeof(OptimizedGradient));
  memset(reinterpret_cast<void*>(mgradient_), 0, sizeof(MaterialGradient));
  for (auto& th : threads) {
    loss_ += th.loss;
    add(*og, th.og);
    madd(mgradient_, th.mg);
  }
  expand(*gradient_, *og);
  symmetrize(*gradient_, [](float& g1, float& g2) {
    g1 = g2 = g1 + g2;
  });

  return true;
}

void BatchLearning::generateGradient(GenGradThread& th) {
  for (;;) {
    MutablePosition mp;
    th.is.read(reinterpret_cast<char*>(&mp), sizeof(MutablePosition));

    if (th.is.eof()) {
      return;
    }

    std::vector<std::vector<Move>> trainingData;
    for (;;) {
      uint8_t length;
      th.is.read(reinterpret_cast<char*>(&length), sizeof(uint8_t));
      if (length == 0) { break; }

      std::vector<Move> pv(length);
      for (unsigned i = 0; i < length; i++) {
        uint16_t move;
        th.is.read(reinterpret_cast<char*>(&move), sizeof(uint16_t));
        pv[i] = Move::deserialize(move);
      }
      trainingData.push_back(std::move(pv));
    }

    generateGradient(th, Position(mp), trainingData);
  }
}

void BatchLearning::generateGradient(GenGradThread& th,
                                     const Position& rootPos,
                                     const std::vector<std::vector<Move>>& trainingData) {
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
    if (rootPos.getTurn() == Turn::White) {
      score0 = -score0;
    }
  }

  float d0 = 0.0f;
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
      score = -score;
    }

    auto diff = score - score0;
    float l = loss(diff.raw());
    float d = gradient(diff.raw());

    th.loss += l;

    if (rootPos.getTurn() == Turn::White) {
      d = -d;
    }
    operate<FeatureOperationType::Extract>(th.og, pos, -d);
    extractMaterial(th.mg, pos, -d);
    d0 += d;
  }
  operate<FeatureOperationType::Extract>(th.og, pos0, d0);
  extractMaterial(th.mg, pos0, d0);
}

void BatchLearning::updateParameters() {
  each(*fv_, *gradient_, [this](int16_t& e, float& g) {
    float n = norm(e, config_.norm);
    int16_t step = random_.bit() + random_.bit();
    if      (g + n > 0.0f && e <= Int16Max - step) { e += step; }
    else if (g + n < 0.0f && e >= Int16Min + step) { e -= step; }
    else if (g + n != 0.0f) { LOG(warning) << "A parameter is out of bounce."; }
  });

  symmetrize(*fv_, [](int16_t& e1, int16_t& e2) {
    e1 = e2;
  });

  optimize(*fv_, evaluator_->ofv());

  std::array<float*, 13> m = {
    &mgradient_[PieceNumber::Pawn],
    &mgradient_[PieceNumber::Lance],
    &mgradient_[PieceNumber::Knight],
    &mgradient_[PieceNumber::Silver],
    &mgradient_[PieceNumber::Gold],
    &mgradient_[PieceNumber::Bishop],
    &mgradient_[PieceNumber::Rook],
    &mgradient_[PieceNumber::Tokin],
    &mgradient_[PieceNumber::ProLance],
    &mgradient_[PieceNumber::ProKnight],
    &mgradient_[PieceNumber::ProSilver],
    &mgradient_[PieceNumber::Horse],
    &mgradient_[PieceNumber::Dragon],
  };
  std::sort(m.begin(), m.end(), [](float* lhs, float* rhs) {
    return *lhs < *rhs;
  });
  random_.shuffle(m.begin(), m.begin() + 6);
  random_.shuffle(m.begin() + 6, m.end());

#if DEBUG_PRINT
  Score mprev[PieceNumber::Num];
  MaterialGradient mgprev;
  memcpy(mprev, material::scores, sizeof(mprev));
  memcpy(mgprev, mgradient_, sizeof(mgprev));
#endif // DEBUG_PRINT

  *m[ 0] = *m[ 1]          = -2.0f;
  *m[ 2] = *m[ 3] = *m[ 4] = -1.0f;
  *m[ 5] = *m[ 6] = *m[ 7] =  0.0f;
  *m[ 8] = *m[ 9] = *m[10] =  1.0f;
  *m[11] = *m[12]          =  2.0f;
  PIECE_TYPE_EACH(pieceType) {
    material::scores[pieceType.raw()] += mgradient_[pieceType.raw()];
    material::scores[pieceType.white().raw()] = material::scores[pieceType.raw()];
  }
  PIECE_EACH(piece) {
    material::exchangeScores[piece.raw()]
      = material::scores[piece.raw()] + material::scores[piece.unpromote().raw()];
    material::promotionScores[piece.raw()]
      = material::scores[piece.promote().raw()] - material::scores[piece.unpromote().raw()];
  }

#if DEBUG_PRINT
  TablePrinter tp;
  tp.row() << "name" << "grad" << "g" << "prev" << "curr" << "(w)" << "exch" << "(w)" << "prom" << "(w)";
  PIECE_TYPE_EACH(pieceType) {
    tp.row() << pieceType
             << mgprev[pieceType.raw()]
             << mgradient_[pieceType.raw()]
             << mprev[pieceType.raw()]
             << material::scores[pieceType.raw()]
             << material::scores[pieceType.white().raw()]
             << material::exchangeScores[pieceType.raw()]
             << material::exchangeScores[pieceType.white().raw()]
             << material::promotionScores[pieceType.raw()]
             << material::promotionScores[pieceType.white().raw()];
  }
  OUT(info) << tp.stringify();
#endif // DEBUG_PRINT

  evaluator_->onChanged(Evaluator::DataSourceType::Custom);
}

void BatchLearning::printParametersSummary() {
  auto summary = summarize(*fv_);
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

  OUT(info) << "";
  OUT(info) << "Summary:\n" << StringUtil::chomp(tp.stringify());

  std::ostringstream oss;
  PIECE_TYPE_EACH(pieceType) {
    oss << pieceType << ": " << material::scores[pieceType.raw()] << "\n";
  }
  OUT(info) << "";
  OUT(info) << "Material:\n" << StringUtil::chomp(oss.str());
}

} // namespace sunfish
