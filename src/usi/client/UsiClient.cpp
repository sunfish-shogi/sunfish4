/* UsiClient.cpp
 *
 * Kubo Ryosuke
 */

#include "usi/client/UsiClient.hpp"
#include "common/string/StringUtil.hpp"
#include "common/resource/Resource.hpp"
#include "common/thread/ScopedThread.hpp"
#include "book/BookUtil.hpp"
#include "core/record/SfenParser.hpp"
#include "search/eval/Material.hpp"
#include "logger/Logger.hpp"
#include <iomanip>
#include <sstream>
#include <utility>
#include <unordered_map>
#include <functional>
#include <chrono>
#include <cctype>
#include <cstdint>
#include <cstdlib>

namespace {

using namespace sunfish;

namespace resources {

const char* ProgramName = "res/strings/usi_name";
const char* Author = "res/strings/usi_author";

} // namespace resources

} // namespace

namespace sunfish {

UsiClient::UsiClient() : breakReceiver_(false), isBookLoaded(false) {
  receiver_ = std::thread([this]() {
    receiver();
  });

  options_.ponder = true;
  options_.hash = 0;
  options_.useBook = true;
  options_.snappy = true;
  options_.marginMs = 500;
  options_.numberOfThreads = 1;
  options_.maxDepth = Searcher::DepthInfinity;
}

void UsiClient::start() {
  for (;;) {
    // >isready
    // <readyok
    ready();

    // >usinewgame
    receiveNewGame();

    game();
  }
}

void UsiClient::ready() {
  for (;;) {
    auto command = receive();

    if (command == "isready") {
      if (!searcher_) {
        auto dataSourceType = Evaluator::sharedEvaluator()->dataSourceType();
        if (dataSourceType != Evaluator::DataSourceType::EvalBin) {
          LOG(error) << "Invalid data source type: " << dataSourceType;
          LOG(error) << "Failed to read eval.bin.";
          exit(1);
        }
        searcher_.reset(new Searcher(Evaluator::sharedEvaluator()));
        searcher_->setHandler(this);
      } else {
        searcher_->clean();
      }

      if (options_.hash != 0) {
        searcher_->ttResizeMB(options_.hash);
      }

      if (!isBookLoaded) {
        book_.load();
        isBookLoaded = true;
      }

      send("readyok");
      return;
    }

    auto args = StringUtil::split(command, [](char c) {
      return isspace(c);
    });

    LOG(error) << "unknown command: " << command;
    exit(0);
  }
}

void UsiClient::receiveNewGame() {
  auto command = receive();

  if (command == "usinewgame") {
    return;
  }

  LOG(error) << "unknown command: " << command;
  exit(0);
}

void UsiClient::game() {
  for (;;) {
    auto command = receive();

    auto args = StringUtil::split(command, [](char c) {
      return isspace(c);
    });

    // >position
    // >go
    if (args[0] == "position") {
      if (!SfenParser::parseUsiCommand(args.begin(),
                                       args.end(),
                                       record_)) {
        LOG(error) << "an error is occured in SfenParser";
        exit(0);
      }
      lastPositionCommand_ = command;

      receiveGo();

      continue;
    }

    // >gameover
    if (args[0] == "gameover") {
      return;
    }

    LOG(error) << "unknown command: " << command;
    exit(0);
  }
}

void UsiClient::receiveGo() {
  auto command = receive();

  auto args = StringUtil::split(command, [](char c) {
    return isspace(c);
  });

  if (args[0] != "go") {
    LOG(error) << "invalid command: " << command;
    exit(0);
  }
  lastGoCommand_ = command;

  // > go ponder
  if (args[1] == "ponder") {
    runPonder(args);
    return;
  }
 
  // > go mate
  if (args[1] == "mate") {
    LOG(warning) << "mate option is not supported";
    send("checkmate" "nomate");
    return;
  }

  runSearch(args);
}

void UsiClient::runSearch(const CommandArguments& args) {
  blackTimeMs_ = 0;
  whiteTimeMs_ = 0;
  byoyomiMs_ = 0;
  blackIncMs_ = 0;
  whiteIncMs_ = 0;
  isInfinite_ = false;

  for (size_t i = 1; i < args.size(); i++) {
    if (args[i] == "btime") {
      blackTimeMs_ = strtol(args[++i].c_str(), nullptr, 10);

    } else if (args[i] == "wtime") {
      whiteTimeMs_ = strtol(args[++i].c_str(), nullptr, 10);

    } else if (args[i] == "byoyomi") {
      byoyomiMs_ = strtol(args[++i].c_str(), nullptr, 10);

    } else if (args[i] == "binc") {
      blackIncMs_ = strtol(args[++i].c_str(), nullptr, 10);

    } else if (args[i] == "winc") {
      whiteIncMs_ = strtol(args[++i].c_str(), nullptr, 10);

    } else if (args[i] == "infinite") {
      isInfinite_ = true;
    }
  }

  MSG(info) << "btime    : " << blackTimeMs_;
  MSG(info) << "wtime    : " << whiteTimeMs_;
  MSG(info) << "byoyomi  : " << byoyomiMs_;
  MSG(info) << "binc     : " << blackIncMs_;
  MSG(info) << "winc     : " << whiteIncMs_;
  MSG(info) << "inifinite: " << (isInfinite_ ? "true" : "false");

  // check opening book
  if (options_.useBook) {
    auto pos = generatePosition(record_, -1);
    Move bookMove = BookUtil::select(book_, pos, random_);
    if (!bookMove.isNone()) {
      MSG(info) << "opening book hit";
      auto bookMoves = book_.get(pos);
      send("info", "string", BookUtil::stringify(pos, *bookMoves));
      send("bestmove", bookMove.toStringSFEN());
      return;
    }
  }

  searcherIsStarted_ = false;
  stopCommandReceived_ = false;
  inPonder_ = false;

  ScopedThread searchThread;
  searchThread.start([this]() {
    search();
  }, [this]() {
    searcher_->interrupt();
    stopCommandReceived_ = true;
  });
  waitForSearcherIsStarted();

  auto command = receiveWithBreak();
  if (command.first == CommandState::Broken) {
    return;
  }

  auto args2 = StringUtil::split(command.second, [](char c) {
    return isspace(c);
  });

  if (args2[0] != "stop") {
    deferredCommands_.push(command.second);
  }
}

void UsiClient::search() {
  MSG(info) << "search thread is started. tid=" << std::this_thread::get_id();

  auto pos = generatePosition(record_, -1);
  auto config = searcher_->getConfig();

  if (isInfinite_) {
    config.maximumTimeMs = SearchConfig::InfinityTime;
    config.optimumTimeMs = SearchConfig::InfinityTime;

  } else {
    bool isBlack = pos.getTurn() == Turn::Black;
    TimeType remainingTimeMs = isBlack ?  blackTimeMs_ : whiteTimeMs_;
    TimeType incrementMs = isBlack ?  blackIncMs_ : whiteIncMs_;
    config.maximumTimeMs = remainingTimeMs + byoyomiMs_ - options_.marginMs;
    config.optimumTimeMs = std::max(remainingTimeMs / 50,
                           std::min(remainingTimeMs, byoyomiMs_ + incrementMs))
                         + byoyomiMs_;

    if (options_.snappy) {
      config.optimumTimeMs /= 3;
    }

    if (!options_.snappy && remainingTimeMs == 0 && incrementMs == 0) {
      config.optimumTimeMs = SearchConfig::InfinityTime;
    }
  }

  config.numberOfThreads = options_.numberOfThreads;
  config.multiPV = options_.multiPV;

  searcher_->setConfig(config);

  searcher_->idsearch(pos, options_.maxDepth * Searcher::Depth1Ply, &record_);

  if (isInfinite_) {
    waitForStopCommand();
  }

  const auto& result = searcher_->getResult();
  const auto& info = searcher_->getInfo();
  bool canPonder = !result.move.isNone() &&
                   result.pv.size() >= 2;

  // send the result of search
  if (canPonder) {
    send("bestmove", result.move.toStringSFEN(),
           "ponder", result.pv.getMove(1).toStringSFEN());
  } else if (!result.move.isNone()) {
    send("bestmove", result.move.toStringSFEN());
  } else {
    send("bestmove", "resign");
  }

  // print the result of search
  printSearchInfo(MSG(info), info, result.elapsed);

  // notify to receiver
  breakReceive();

  MSG(info) << "search thread is stopped. tid=" << std::this_thread::get_id();
}

void UsiClient::runPonder(const CommandArguments&) {
  searcherIsStarted_ = false;
  inPonder_ = true;

  ScopedThread searchThread;
  searchThread.start([this]() {
    ponder();
  }, [this]() {
    searcher_->interrupt();
  });
  waitForSearcherIsStarted();

  auto command = receive();

  auto args = StringUtil::split(command, [](char c) {
    return isspace(c);
  });

  if (args[0] == "stop") {
    send("bestmove", "resign");
    return;
  }

  if (args[0] == "ponderhit") {
    std::string ponderSection(" ponder");
    lastGoCommand_.replace(lastGoCommand_.find(ponderSection),
                           ponderSection.length(),
                           "");
    deferredCommands_.push(lastPositionCommand_);
    deferredCommands_.push(lastGoCommand_);
    return;
  }

  deferredCommands_.push(command);
}

void UsiClient::ponder() {
  MSG(info) << "ponder thread is started. tid=" << std::this_thread::get_id();

  record_.moveList.pop_back();
  auto pos = generatePosition(record_, -1);
  auto config = searcher_->getConfig();

  config.maximumTimeMs = SearchConfig::InfinityTime;
  config.optimumTimeMs = SearchConfig::InfinityTime;
  config.numberOfThreads = options_.numberOfThreads;
  config.multiPV = options_.multiPV;

  searcher_->setConfig(config);

  searcher_->idsearch(pos, options_.maxDepth * Searcher::Depth1Ply, &record_);

  MSG(info) << "ponder thread is stopped. tid=" << std::this_thread::get_id();
}

void UsiClient::waitForSearcherIsStarted() {
  while (true) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    if (searcherIsStarted_) { break; }
  }
}

void UsiClient::waitForStopCommand() {
  while (true) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    if (stopCommandReceived_) { break; }
  }
}

void UsiClient::onStart(const Searcher&) {
  searcherIsStarted_ = true;
}

void UsiClient::onUpdatePV(const Searcher& searcher, const PV& pv, float elapsed, int depth, Score score, bool failLow, bool failHigh, int multiPV) {
  auto& info = searcher.getInfo();

  auto timeMs = static_cast<uint32_t>(elapsed * 1e3);
  auto realDepth = depth / Searcher::Depth1Ply;
  auto totalNodes = info.nodes + info.quiesNodes;
  auto nps = static_cast<uint32_t>(totalNodes / elapsed);
  auto hashfull = static_cast<int>(searcher_->ttUsageRates() * 1000);

  const char* scoreKey;
  int scoreValue;
  if (score > -Score::mate() && score < Score::mate()) {
    scoreKey = "cp";
    scoreValue = score.raw() * 100.0 / material::pawn().raw();
  } else {
    scoreKey = "mate";
    if (score >= 0) {
      scoreValue = (Score::infinity() - score).raw();
    } else {
      scoreValue = -(Score::infinity() + score).raw();
    }
  }

  MSG(info) << std::setw(2) << realDepth << ": "
            << std::setw(10) << (info.nodes + info.quiesNodes) << ": "
            << std::setw(7) << timeMs << ' '
            << pv.toString() << ": "
            << score;

  if (!inPonder_) {
    if (failLow) { send("info", "string", "fail-low"); }
    if (failHigh) { send("info", "string", "fail-high"); }
    send("info",
         "time", timeMs,
         "depth", realDepth,
         "seldepth", pv.size(),
         "nodes", totalNodes,
         "nps", nps,
         "currmove", pv.getMove(0).toStringSFEN(),
         "score", scoreKey, scoreValue,
         "pv", pv.toStringSFEN(),
         "multipv", multiPV,
         "hashfull", hashfull);
  }
  if (failLow) { MSG(info) << "fail-low"; }
  if (failHigh) { MSG(info) << "fail-high"; }
}

void UsiClient::onUpdatePV(const Searcher& searcher, const PV& pv, float elapsed, int depth, Score score, int multiPV) {
  onUpdatePV(searcher, pv, elapsed, depth, score, false, false, multiPV);
}

void UsiClient::onFailLow(const Searcher& searcher, const PV& pv, float elapsed, int depth, Score score) {
  onUpdatePV(searcher, pv, elapsed, depth, score, true, false, 1);
}

void UsiClient::onFailHigh(const Searcher& searcher, const PV& pv, float elapsed, int depth, Score score) {
  onUpdatePV(searcher, pv, elapsed, depth, score, false, true, 1);
}

void UsiClient::onIterateEnd(const Searcher& searcher, float elapsed, int depth) {
}

std::string UsiClient::receive() {
  for (;;) {
    auto command = receiveWithBreak();
    if (command.first != CommandState::Broken) {
      return command.second;
    }
  }
}

std::pair<UsiClient::CommandState, std::string> UsiClient::receiveWithBreak() {
  if (!deferredCommands_.empty()) {
    auto command = deferredCommands_.front();
    deferredCommands_.pop();
    return { CommandState::Ok, command };
  }

  while (!breakReceiver_) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    {
      std::lock_guard<std::mutex> lock(receiveMutex_);
      if (!commandQueue_.empty()) {
        auto cs = commandQueue_.front();
        commandQueue_.pop();
        return { CommandState::Ok, cs };
      }
    }
  }
  breakReceiver_ = false;

  return { CommandState::Broken, "" };
}

void UsiClient::receiver() {
  bool usi = false;

  for (;;) {
    std::string command;
    std::getline(std::cin, command);

    if (std::cin.eof()) {
      LOG(warning) << "reached to EOF.";
      exit(0);
    } else if (!std::cin.good()) {
      LOG(error) << "an error is occured in STDIN.";
      exit(0);
    } else if (command.empty()) {
      LOG(error) << "received an empty string.";
      exit(0);
    }

    MSG(receive) << command;

    // > usi
    // < usiok
    if (!usi) {
      if (command == "usi") {
        acceptUsi();
        usi = true;
        continue;
      } else {
        LOG(error) << "invalid command: " << command;
        exit(0);
      }
    }

    // > quit
    if (command == "quit") {
      MSG(info) << "quit";
      exit(0);
    }

    auto args = StringUtil::split(command, [](char c) {
      return isspace(c);
    });

    if (args[0] == "setoption") {
      setOption(args);
      continue;
    }

    {
      std::lock_guard<std::mutex> lock(receiveMutex_);
      commandQueue_.push({
        command
      });
    }
  }
}

void UsiClient::acceptUsi() {
  auto name = Resource::string(resources::ProgramName, "Sunfish4");
  auto author = Resource::string(resources::Author, "Kubo Ryosuke");
  send("id", "name", name);
  send("id", "author", author);

  send("option", "name", "UseBook", "type", "check", "default", "true");
  send("option", "name", "Snappy", "type", "check", "default", "true");
  send("option", "name", "MarginMs", "type", "spin", "default", "500", "min", "0", "max", "2000");
  send("option", "name", "Threads", "type", "spin", "default", "1", "min", "1", "max", "32");
  send("option", "name", "MaxDepth", "type", "spin", "default", "64", "min", "1", "max", "64");
  send("option", "name", "MultiPV", "type", "spin", "default", "1", "min", "1", "max", "10");

  send("usiok");
}

void UsiClient::setOption(const CommandArguments& args) {
  const auto& name = args[2];
  const auto& value = args[4];

  if (name == "USI_Ponder") {
    options_.ponder = value == "true";
  } else if (name == "USI_Hash") {
    options_.hash = std::stoi(value);
  } else if (name == "UseBook") {
    options_.useBook = value == "true";
  } else if (name == "Snappy") {
    options_.snappy = value == "true";
  } else if (name == "MarginMs") {
    options_.marginMs = StringUtil::toInt(value, options_.marginMs);
  } else if (name == "Threads") {
    options_.numberOfThreads = StringUtil::toInt(value, options_.numberOfThreads);
  } else if (name == "MaxDepth") {
    options_.maxDepth = StringUtil::toInt(value, options_.maxDepth);
  } else if (name == "MultiPV") {
    options_.multiPV = StringUtil::toInt(value, options_.multiPV);
  } else {
    LOG(warning) << "unknown option: " << name;
  }
}

void UsiClient::breakReceive() {
  breakReceiver_ = true;
}

template <class T>
void UsiClient::send(T&& command) {
  std::lock_guard<std::mutex> lock(sendMutex_);

  std::cout << command << std::endl;
  std::cout.flush();
  MSG(send) << command;
}

template <class T, class... Args>
void UsiClient::send(T&& command, Args&&... options) {
  std::ostringstream oss;
  oss << command;
  joinOptions(oss, std::forward<Args>(options)...);
  send(oss.str());
}

template <class T>
void UsiClient::joinOptions(std::ostream& os, T&& arg) {
  os << ' ' << arg;
}

template <class T, class... Args>
void UsiClient::joinOptions(std::ostream& os, T&& arg, Args&&... args) {
  os << ' ' << arg;
  joinOptions(os, std::forward<Args>(args)...);
}

} // namespace sunfish
