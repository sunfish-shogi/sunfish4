/* WasmUsiClient.cpp */

#include "wasm/WasmUsiClient.hpp"
#include "book/BookUtil.hpp"
#include "common/string/StringUtil.hpp"
#include "core/record/SfenParser.hpp"
#include "core/util/CoreUtil.hpp"
#include "search/eval/Material.hpp"
#include "search/util/SearchUtil.hpp"
#include <algorithm>
#include <cerrno>
#include <climits>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <utility>

namespace {

bool parseInteger(const std::string& value, long min, long max, long& result) {
  if (value.empty()) {
    return false;
  }
  char* end = nullptr;
  errno = 0;
  long parsed = std::strtol(value.c_str(), &end, 10);
  if (errno == ERANGE || end == value.c_str() || *end != '\0' ||
      parsed < min || parsed > max) {
    return false;
  }
  result = parsed;
  return true;
}

std::vector<std::string> split(const std::string& line) {
  std::istringstream stream(line);
  std::vector<std::string> result;
  std::string value;
  while (stream >> value) {
    result.push_back(value);
  }
  return result;
}

} // namespace

namespace sunfish {

WasmUsiClient::WasmUsiClient() :
  initialized_(false),
  ready_(false),
  terminated_(false),
  searching_(false),
  infinite_(false),
  resultPending_(false),
  bookLoaded_(false),
  useBook_(true),
  hashMB_(32),
  snappy_(true),
  marginMs_(500),
  maxDepth_(64),
  multiPV_(1) {
}

void WasmUsiClient::command(const std::string& line) {
  if (terminated_) {
    return;
  }
  auto args = split(line);
  if (args.empty()) {
    return;
  }

  const auto& name = args[0];
  if (name == "quit") {
    stop(false);
    terminated_ = true;
  } else if (name == "usi") {
    acceptUsi();
  } else if (name == "setoption") {
    setOption(args);
  } else if (name == "isready") {
    ready();
  } else if (name == "usinewgame") {
    if (searcher_) {
      searcher_->clean();
    }
  } else if (name == "position") {
    setPosition(args);
  } else if (name == "go") {
    go(args);
  } else if (name == "stop") {
    stop(true);
  } else if (name == "gameover") {
    stop(false);
  } else if (name == "ponderhit") {
    // Ponder is intentionally not advertised by the single-threaded build.
  }
}

void WasmUsiClient::acceptUsi() {
  output("id name Sunfish4");
  output("id author Kubo, Ryosuke");
  output("option name UseBook type check default true");
  output("option name Snappy type check default true");
  output("option name MarginMs type spin default 500 min 0 max 2000");
  output("option name MaxDepth type spin default 64 min 1 max 64");
  output("option name MultiPV type spin default 1 min 1 max 10");
  output("usiok");
}

void WasmUsiClient::setOption(const Arguments& args) {
  if (args.size() < 5 || args[1] != "name" || args[3] != "value") {
    return;
  }
  long value = 0;
  if (args[2] == "USI_Hash" && parseInteger(args[4], 1, 256, value)) {
    hashMB_ = static_cast<unsigned>(value);
  } else if (args[2] == "UseBook") {
    useBook_ = args[4] == "true";
  } else if (args[2] == "Snappy") {
    snappy_ = args[4] == "true";
  } else if (args[2] == "MarginMs" && parseInteger(args[4], 0, 2000, value)) {
    marginMs_ = static_cast<int>(value);
  } else if (args[2] == "MaxDepth" && parseInteger(args[4], 1, 64, value)) {
    maxDepth_ = static_cast<int>(value);
  } else if (args[2] == "MultiPV" && parseInteger(args[4], 1, 10, value)) {
    multiPV_ = static_cast<int>(value);
  }
}

void WasmUsiClient::ready() {
  if (!initialized_) {
    CoreUtil::initialize();
    SearchUtil::initialize();
    initialized_ = true;
  }
  if (!searcher_) {
    searcher_.reset(new Searcher());
    searcher_->setHandler(this);
  } else {
    searcher_->clean();
  }
  searcher_->ttResizeMB(hashMB_);
  if (!bookLoaded_) {
    book_.load();
    bookLoaded_ = true;
  }
  ready_ = true;
  output("readyok");
}

void WasmUsiClient::setPosition(const Arguments& args) {
  if (searching_ || resultPending_) {
    stop(false);
  }
  Record record;
  if (!SfenParser::parseUsiCommand(args.begin(), args.end(), record)) {
    output("info string invalid position command");
    return;
  }
  Position position = record.initialPosition;
  for (const auto& move : record.moveList) {
    Piece captured;
    if (!position.validateMove(move, position.getCheckState()) ||
        !position.doMove(move, captured)) {
      output("info string invalid position command");
      return;
    }
  }
  record_ = std::move(record);
}

void WasmUsiClient::go(const Arguments& args) {
  if (!ready_ || !searcher_ || searching_ || resultPending_) {
    return;
  }
  if (args.size() >= 2 && args[1] == "mate") {
    output("checkmate notimplemented");
    return;
  }

  SearchConfig config = searcher_->getConfig();
  config.numberOfThreads = 1;
  config.multiPV = multiPV_;
  config.maximumTimeMs = SearchConfig::InfinityTime;
  config.optimumTimeMs = SearchConfig::InfinityTime;
  int depth = maxDepth_;
  long blackTime = 0;
  long whiteTime = 0;
  long byoyomi = 0;
  long blackInc = 0;
  long whiteInc = 0;
  bool infinite = false;
  bool hasClock = false;

  for (size_t i = 1; i < args.size(); ++i) {
    long value = 0;
    if (args[i] == "infinite") {
      infinite = true;
    } else if (i + 1 < args.size() && args[i] == "depth" &&
               parseInteger(args[i + 1], 1, 64, value)) {
      depth = std::min(depth, static_cast<int>(value));
      ++i;
    } else if (i + 1 < args.size() && args[i] == "movetime" &&
               parseInteger(args[i + 1], 1, INT_MAX, value)) {
      config.maximumTimeMs = static_cast<SearchConfig::TimeType>(value);
      config.optimumTimeMs = config.maximumTimeMs;
      ++i;
    } else if (i + 1 < args.size() && parseInteger(args[i + 1], 0, INT_MAX, value)) {
      if (args[i] == "btime") blackTime = value;
      else if (args[i] == "wtime") whiteTime = value;
      else if (args[i] == "byoyomi") byoyomi = value;
      else if (args[i] == "binc") blackInc = value;
      else if (args[i] == "winc") whiteInc = value;
      else continue;
      hasClock = true;
      ++i;
    }
  }

  auto position = generatePosition(record_, -1);
  if (useBook_ && !infinite) {
    Move bookMove = BookUtil::select(book_, position, random_);
    if (!bookMove.isNone()) {
      const auto* bookMoves = book_.get(position);
      output(std::string("info string ") +
          BookUtil::stringify(position, *bookMoves));
      output(std::string("bestmove ") + bookMove.toStringSFEN());
      return;
    }
  }
  if (!infinite && hasClock &&
      config.maximumTimeMs == SearchConfig::InfinityTime) {
    bool black = position.getTurn() == Turn::Black;
    long remaining = black ? blackTime : whiteTime;
    long increment = black ? blackInc : whiteInc;
    long maximum = std::max(1L, remaining + byoyomi - marginMs_);
    long optimum = std::max(remaining / 50, std::min(remaining, byoyomi + increment)) + byoyomi;
    if (snappy_) optimum /= 3;
    config.maximumTimeMs = static_cast<SearchConfig::TimeType>(maximum);
    config.optimumTimeMs = static_cast<SearchConfig::TimeType>(std::max(1L, optimum));
  }

  searcher_->setConfig(config);
  infinite_ = infinite;
  resultPending_ = false;
  searching_ = searcher_->startIDSearch(position,
      depth * Searcher::Depth1Ply, &record_);
  if (!searching_) {
    if (infinite_) {
      resultPending_ = true;
    } else {
      emitBestmove();
    }
  }
}

void WasmUsiClient::poll() {
  if (terminated_ || !searching_) {
    return;
  }
  searching_ = searcher_->pollIDSearch();
  if (!searching_) {
    if (infinite_) {
      resultPending_ = true;
    } else {
      emitBestmove();
    }
  }
}

void WasmUsiClient::stop(bool emit) {
  if (!searching_ && !resultPending_) {
    return;
  }
  if (searching_) {
    searcher_->stopIDSearch();
  }
  searching_ = false;
  if (emit) {
    emitBestmove();
  }
  infinite_ = false;
  resultPending_ = false;
}

void WasmUsiClient::emitBestmove() {
  const auto& result = searcher_->getResult();
  if (result.move.isNone()) {
    output("bestmove resign");
  } else {
    output(std::string("bestmove ") + result.move.toStringSFEN());
  }
}

void WasmUsiClient::output(const std::string& line) {
  if (!terminated_) {
    std::cout << line << std::endl;
  }
}

void WasmUsiClient::onStart(const Searcher&) {}

void WasmUsiClient::onUpdatePV(const Searcher& searcher, const PV& pv,
                               float elapsed, int depth, Score score, int multiPV) {
  if (pv.size() == 0) {
    return;
  }
  const auto& info = searcher.getInfo();
  uint64_t nodes = info.nodes + info.quiesNodes;
  uint32_t timeMs = static_cast<uint32_t>(elapsed * 1000);
  uint64_t nps = elapsed > 0 ? static_cast<uint64_t>(nodes / elapsed) : 0;
  const char* scoreType;
  int scoreValue;
  if (score > -Score::mate() && score < Score::mate()) {
    scoreType = "cp";
    scoreValue = score.raw() * 100 / material::pawn().raw();
  } else {
    scoreType = "mate";
    scoreValue = score >= 0
        ? (Score::infinity() - score).raw()
        : -(Score::infinity() + score).raw();
  }
  std::ostringstream line;
  line << "info time " << timeMs
       << " depth " << depth / Searcher::Depth1Ply
       << " seldepth " << pv.size()
       << " nodes " << nodes
       << " nps " << nps
       << " score " << scoreType << ' ' << scoreValue
       << " multipv " << multiPV
       << " pv " << pv.toStringSFEN();
  output(line.str());
}

void WasmUsiClient::onFailLow(const Searcher& searcher, const PV& pv,
                              float elapsed, int depth, Score score) {
  onUpdatePV(searcher, pv, elapsed, depth, score, 1);
}

void WasmUsiClient::onFailHigh(const Searcher& searcher, const PV& pv,
                               float elapsed, int depth, Score score) {
  onUpdatePV(searcher, pv, elapsed, depth, score, 1);
}

void WasmUsiClient::onIterateEnd(const Searcher&, float, int) {}

} // namespace sunfish
