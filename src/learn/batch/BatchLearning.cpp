/* BatchLearning.cpp
 *
 * Kubo Ryosuke
 */

#include "learn/batch/BatchLearning.hpp"
#include "learn/util/LearningUtil.hpp"
#include "search/eval/FeatureTemplates.hpp"
#include "search/eval/Material.hpp"
#include "search/Searcher.hpp"
#include "core/move/MoveGenerator.hpp"
#include "core/record/CsaReader.hpp"
#include "core/record/SfenParser.hpp"
#include "common/file_system/Directory.hpp"
#include "common/resource/Resource.hpp"
#include "common/string/StringUtil.hpp"
#include "logger/Logger.hpp"
#include <atomic>
#include <sstream>
#include <utility>
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

BatchLearning::BatchLearning() :
    evaluator_(std::make_shared<Evaluator>(Evaluator::InitType::Zero))
#if !MATERIAL_LEARNING_ONLY
    ,fv_(new Evaluator::FVType())
    ,gradient_(new Gradient())
#endif // !MATERIAL_LEARNING_ONLY
  {
}

bool BatchLearning::run() {
  MSG(info) << "####################################################################";
  MSG(info) << "##                         BatchLearning                          ##";
  MSG(info) << "####################################################################";

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
  MSG(info) << "completed";
  MSG(info) << "elapsed: " << elapsed;

  return true;
}

void BatchLearning::readConfigFromIniFile() {
  auto ini = Resource::ini(BatchLearnIni);

  config_.trainingData      = getValue(ini, "Learn", "TrainingData");
  config_.iteration         = StringUtil::toInt(getValue(ini, "Learn", "Iteration"), DefaultIteration);
  config_.restart           = StringUtil::toInt(getValue(ini, "Learn", "Restart"), 0);
  config_.restartIteration  = StringUtil::toInt(getValue(ini, "Learn", "RestartIteration"), 0);
  config_.numThreads        = StringUtil::toInt(getValue(ini, "Learn", "NumThreads"), std::thread::hardware_concurrency());
  config_.depth             = StringUtil::toInt(getValue(ini, "Learn", "Depth"), DefaultDepth);
  config_.norm              = StringUtil::toFloat(getValue(ini, "Learn", "Norm"), DefaultNorm);

  MSG(info) << "TrainingData    : " << config_.trainingData;
  MSG(info) << "Iteration       : " << config_.iteration;
  MSG(info) << "Restart         : " << config_.restart;
  MSG(info) << "RestartIteration: " << config_.restartIteration;
  MSG(info) << "NumThreads      : " << config_.numThreads;
  MSG(info) << "Depth           : " << config_.depth;
  MSG(info) << "Norm            : " << config_.norm;
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
#if !MATERIAL_LEARNING_ONLY
    if (!load(*fv_)) {
      return false;
    }
    optimize(*fv_, evaluator_->ofv());
    evaluator_->onChanged(Evaluator::DataSourceType::Custom);
#else // MATERIAL_LEARNING_ONLY
    LOG(error) << "Restart option can not be used in MATERIAL_LEARNING_ONLY mode.";
    return false;
#endif
  }

  for (int i = 0; i < config_.iteration; i++) {
    if (!config_.restart || i >= config_.restartIteration) {
      MSG(info) << "";
      MSG(info) << "ITERATION - " << i;

      MSG(info) << "generating training data..";

      bool ok = generateTrainingData();
      if (!ok) {
        return false;
      }

      MSG(info) << "adjusting parameters..";

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

      MSG(info) << "writing to file..";

#if !MATERIAL_LEARNING_ONLY
      save(*fv_);
#endif // !MATERIAL_LEARNING_ONLY

      MSG(info) << "";
      MSG(info) << "loss = " << lossFirst << " - " << lossLast;
#if !MATERIAL_LEARNING_ONLY
      MSG(info) << "";
      LearningUtil::printFVSummary(fv_.get());
#endif // !MATERIAL_LEARNING_ONLY
#if !NO_MATERIAL_LEARNING
      MSG(info) << "";
      LearningUtil::printMaterial();
#endif // !NO_MATERIAL_LEARNING
    }

    updateCount = std::max(updateCount * 2 / 3, MinimumUpdateCount);
  }
  return true;
}

bool BatchLearning::generateTrainingData() {
  reader_.reset(new TrainingDataReader());
  if (!reader_->open(config_.trainingData)) {
    LOG(error) << "failed to read training data";
    return false;
  }

  std::vector<GenTrDataThread> threads(config_.numThreads);

  for (unsigned tn = 0; tn < threads.size(); tn++) {
    auto& th = threads[tn];

    auto path = trainingDataPath(tn);
    th.os.open(path, std::ios::out | std::ios::binary);
    if (!th.os) {
      LOG(error) << "could not open a file: " << path;
      return false;
    }

    th.searcher.reset(new Searcher(evaluator_));
    th.failLoss = 0;
    th.numberOfData = 0;
  }

  for (auto& th : threads) {
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
    TrainingDataElement td;
    {
      std::lock_guard<std::mutex> lock(readerMutex_);
      if (!reader_->read(td)) {
        break;
      }
    }

    Position pos;
    if (!SfenParser::parsePosition(td.sfen, pos)) {
      LOG(error) << "invalid SFEN: " << td.sfen;
      continue;
    }

    generateTrainingData(th, pos, td.move);
  }
}

void BatchLearning::generateTrainingData(GenTrDataThread& th,
                                         Position& pos,
                                         Move bestMove) {
  int depth = Searcher::Depth1Ply * config_.depth + Searcher::Depth1Ply / 2;
  Score alpha;
  Score beta;
  Score bestScore;
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
    bestScore = -result.score;
    results.push_back({ bestMove, result.pv });
  }

  Moves moves;
  auto cs = pos.getCheckState();
  if (!isCheck(cs)) {
    MoveGenerator::generateCaptures(pos, moves);
    MoveGenerator::generateQuiets(pos, moves);
  } else {
    MoveGenerator::generateEvasions(pos, cs, moves);
  }

  for (auto& move : moves) {
    if (move == bestMove) {
      continue;
    }

    for (int newDepth = Searcher::Depth1Ply * 3 / 2; newDepth <= depth; newDepth += Searcher::Depth1Ply) {
      if (pos.isCheck(move)){
        newDepth += Searcher::Depth1Ply;
      }

      Piece captured;
      if (!pos.doMove(move, captured)) {
        break;
      }
      th.searcher->search(pos,
                          newDepth,
                          -beta,
                          -alpha);
      pos.undoMove(move, captured);

      auto& result = th.searcher->getResult();

      // fail-low
      if (-result.score <= alpha) {
        break;
      }

      if (-result.score > bestScore && newDepth + Searcher::Depth1Ply <= depth) {
        continue;
      }

      // fail-high
      if (-result.score >= beta) {
        th.failLoss++;
        break;
      }

      results.push_back({ move, result.pv });
      break;
    }
  }

  th.numberOfData++;

  if (results.size() == 1) {
    return;
  }

  auto sfen = pos.toStringSFEN();
  char sfenSize = static_cast<char>(sfen.size());
  th.os.write(&sfenSize, sizeof(sfenSize));
  th.os.write(sfen.c_str(), sfenSize);

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

#if !MATERIAL_LEARNING_ONLY
    memset(reinterpret_cast<void*>(&th.og), 0, sizeof(th.og));
#endif // !MATERIAL_LEARNING_ONLY
#if !NO_MATERIAL_LEARNING
    memset(reinterpret_cast<void*>(&th.mg), 0, sizeof(th.mg));
#endif // !NO_MATERIAL_LEARNING
    th.loss = 0.0f;
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

  loss_ = failLoss_;
  auto og = std::unique_ptr<OptimizedGradient>(new OptimizedGradient);
  memset(reinterpret_cast<void*>(og.get()), 0, sizeof(OptimizedGradient));
#if !NO_MATERIAL_LEARNING
  memset(reinterpret_cast<void*>(mgradient_), 0, sizeof(MaterialGradient));
#endif // !NO_MATERIAL_LEARNING
  for (auto& th : threads) {
    loss_ += th.loss;
#if !MATERIAL_LEARNING_ONLY
    add(*og, th.og);
#endif // !MATERIAL_LEARNING_ONLY
#if !NO_MATERIAL_LEARNING
    madd(mgradient_, th.mg);
#endif // !NO_MATERIAL_LEARNING
  }
#if !MATERIAL_LEARNING_ONLY
  expand(*gradient_, *og);
  symmetrize(*gradient_, [](float& g1, float& g2) {
    g1 = g2 = g1 + g2;
  });
#endif // !MATERIAL_LEARNING_ONLY

  return true;
}

void BatchLearning::generateGradient(GenGradThread& th) {
  for (;;) {
    char sfenSize;
    th.is.read(&sfenSize, sizeof(sfenSize));

    if (th.is.eof()) {
      return;
    }

    char sfen[sfenSize+1];
    memset(sfen, 0, sizeof(sfen));
    th.is.read(sfen, sfenSize);

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

    Position pos;
    if (!SfenParser::parsePosition(sfen, pos)) {
      LOG(error) << "invalid SFEN: " << static_cast<const char*>(sfen);
      continue;
    }

    generateGradient(th, pos, trainingData);
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
#if !MATERIAL_LEARNING_ONLY
    operate<FeatureOperationType::Extract>(th.og, pos, -d);
#endif // !MATERIAL_LEARNING_ONLY
#if !NO_MATERIAL_LEARNING
    extractMaterial(th.mg, pos, -d);
#endif // !NO_MATERIAL_LEARNING
    d0 += d;
  }
#if !MATERIAL_LEARNING_ONLY
  operate<FeatureOperationType::Extract>(th.og, pos0, d0);
#endif // !MATERIAL_LEARNING_ONLY
#if !NO_MATERIAL_LEARNING
  extractMaterial(th.mg, pos0, d0);
#endif // !NO_MATERIAL_LEARNING
}

void BatchLearning::updateParameters() {
#if !MATERIAL_LEARNING_ONLY
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
#endif // !MATERIAL_LEARNING_ONLY

#if !NO_MATERIAL_LEARNING
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
    if (piece.type() == PieceType::king()) {
      continue;
    }
    material::exchangeScores[piece.raw()]
      = material::scores[piece.raw()] + material::scores[piece.unpromote().raw()];
    material::promotionScores[piece.raw()]
      = material::scores[piece.promote().raw()] - material::scores[piece.unpromote().raw()];
  }
#endif // !NO_MATERIAL_LEARNING

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
  MSG(info) << tp.stringify();
#endif // DEBUG_PRINT

  evaluator_->onChanged(Evaluator::DataSourceType::Custom);
}

} // namespace sunfish
