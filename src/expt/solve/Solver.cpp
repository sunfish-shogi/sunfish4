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
#include "logger/Logger.hpp"

#include <fstream>

namespace {

CONSTEXPR_CONST int SearchDepth = 15;

} // namespace

namespace sunfish {

Solver::Solver() {
  searcher_.setHandler(&searchHandler_);
  result_.corrected = 0;
  result_.incorrected = 0;
  result_.skipped = 0;
}

bool Solver::solve(const char* path) {
  if (FileUtil::isDirectory(path)) {
    // 'path' point to a directory
    Directory directory(path);
    auto files = directory.files("*.csa");

    for (const auto& path : files) {
      if (!solveCsaFile(path.c_str())) {
        return false;
      }
    }

  } else if (FileUtil::isFile(path)) {
    // 'path' point to a file
    if (!solveCsaFile(path)) {
      return false;
    }

  } else {
    LOG(error) << "not exists: " << path;
    return false;
  }

  auto percentage = [](float n, float d) {
    return n / d * 100.0f;
  };

  auto total = result_.corrected + result_.incorrected + result_.skipped;

  OUT(info) << "";
  OUT(info) << "summary:";
  OUT(info) << "  total    : " << total;
  OUT(info) << "  correct  : " << result_.corrected
                               << " (" << percentage(result_.corrected, total) << "%)";
  OUT(info) << "  incorrect: " << result_.incorrected
                               << " (" << percentage(result_.incorrected, total) << "%)";
  OUT(info) << "  skipped  : " << result_.skipped
                               << " (" << percentage(result_.skipped, total) << "%)";

  return true;
}

bool Solver::solveCsaFile(const char* path) {
  OUT(info) << "open a record file: " << path;

  std::ifstream file(path);
  if (!file) {
    LOG(error) << "could not open a file: " << path;
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
      LOG(error) << "invalid move: " << move.toString(position) << "\n"
                 << "on:\n" << position.toString();
      return false;
    }
  }

  return true;
}

bool Solver::solve(const Position& position, Move correct) {
  OUT(info) << "";
  OUT(info) << StringUtil::chomp(position.toString());

  auto config = searcher_.getConfig();
  config.maximumMilliSeconds = 3 * 1000;
  config.optimumMilliSeconds = 1 * 1000;
  searcher_.setConfig(config);

  bool ok = searcher_.idsearch(position, SearchDepth * Searcher::Depth1Ply);

  if (!ok) {
    OUT(info) << "skipped.";
    result_.skipped++;
    return true;
  }

  auto& result = searcher_.getResult();
  OUT(info) << "answer : " << result.move.toString(position);
  OUT(info) << "correct: " << correct.toString(position);
  if (result.move == correct) {
    OUT(info) << "result : correct";
    result_.corrected++;
  } else {
    OUT(info) << "result : incorrect";
    result_.incorrected++;
  }

  return true;
}

} // namespace sunfish
