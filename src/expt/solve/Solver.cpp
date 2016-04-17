/* Solver.cpp
 *
 * Kubo Ryosuke
 */

#include "expt/solve/Solver.hpp"
#include "common/Def.hpp"
#include "common/file_system/Directory.hpp"
#include "common/file_system/FileUtil.hpp"
#include "common/string/StringUtil.hpp"
#include "core/record/CsaReader.hpp"
#include "search/tree/ErrorCounter.hpp"
#include "logger/Logger.hpp"

#include <fstream>

namespace sunfish {

Solver::Solver() {
  searcher_.setHandler(&searchHandler_);
  config_.muximumDepth = 18;
  config_.muximumTimeSeconds = 3;
}

bool Solver::solve(const char* path) {
  result_.corrected = 0;
  result_.incorrected = 0;
  result_.mate = 0;
  result_.depthSum = 0;
  result_.nodesSum = 0;
  result_.elapsedSum = 0.0;

#if ENABLE_ERR_RATE
  resetErrorCounts();
#endif

  if (FileUtil::isDirectory(path)) {
    // 'path' points to a directory
    Directory directory(path);
    auto files = directory.files("*.csa");
    int n = 0;
    for (const auto& path : files) {
      n++;
      OUT(info) << "------------------------ [" << n << "] ------------------------";
      if (!solveCsaFile(path.c_str())) {
        return false;
      }
    }

  } else if (FileUtil::isFile(path)) {
    // 'path' points to a file
    if (!solveCsaFile(path)) {
      return false;
    }

  } else {
    // a specified path is not available.
    LOG(error) << "not exists: " << path;
    return false;
  }

  OUT(info) << "--------------------- completed ---------------------";

  auto percentage = [](float n, float d) {
    return n / d * 100.0f;
  };
  auto total = result_.corrected + result_.incorrected;
  OUT(info) << "summary:";
  OUT(info) << "  total    : " << total;
  OUT(info) << "  correct  : " << result_.corrected
                               << " (" << percentage(result_.corrected, total) << "%)";
  OUT(info) << "  incorrect: " << result_.incorrected
                               << " (" << percentage(result_.incorrected, total) << "%)";
  OUT(info) << "  nps      : " << static_cast<uint64_t>(result_.nodesSum / result_.elapsedSum);
  OUT(info) << "  depth    : " << (static_cast<float>(result_.depthSum) / Searcher::Depth1Ply / (result_.corrected + result_.incorrected - result_.mate));

#if ENABLE_ERR_RATE
  printErrorRate();
#endif

  return true;
}

bool Solver::solveCsaFile(const char* path) {
  OUT(info) << "[" << path << "]";
  OUT(info) << "";

  std::ifstream file(path);
  if (!file) {
    LOG(error) << "could not open a file: " << path;
    OUT(info) << "";
    return false;
  }

  Record record;
  CsaReader::read(file, record);

  file.close();

  Position position = record.initialPosition;

  for (const auto& move : record.moveList) {
    solve(position, move);

    Piece captured;
    if (!position.doMove(move, captured)) {
      LOG(error) << "an illegal move is detected: " << move.toString(position) << "\n"
                 << position.toString();
      OUT(info) << "";
      return false;
    }
  }

  return true;
}

bool Solver::solve(const Position& position, Move correct) {
  OUT(info) << StringUtil::chomp(position.toString());

  auto config = searcher_.getConfig();
  config.maximumTimeMs = config_.muximumTimeSeconds * 1000;
  config.optimumTimeMs = config_.muximumTimeSeconds * 1000;
  searcher_.setConfig(config);

  int depth = config_.muximumDepth * Searcher::Depth1Ply;
  searcher_.idsearch(position, depth);

#if ENABLE_ERR_RATE
  continueErrorCounts();
#endif

  auto& result = searcher_.getResult();
  auto& info = searcher_.getInfo();
  bool isCorrect = result.move == correct;

  if (isCorrect) {
    result_.corrected++;
  } else {
    result_.incorrected++;
  }

  if (result.move.isNone() || result.score >= Score::mate() || result.score <= -Score::mate()) {
    result_.mate++;
  } else {
    result_.depthSum += result.depth;
    result_.nodesSum += info.nodes + info.quiesNodes;
    result_.elapsedSum += result.elapsed;
  }

  printSearchInfo(OUT(info), info, result.elapsed);
  OUT(info) << "";
  OUT(info) << "answer : " << result.move.toString(position);
  OUT(info) << "correct: " << correct.toString(position);
  OUT(info) << "result : " << (isCorrect ? "correct" : "incorrect");
  OUT(info) << "";

  return true;
}

} // namespace sunfish
