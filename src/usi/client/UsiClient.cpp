/* UsiClient.cpp
 *
 * Kubo Ryosuke
 */

#include "usi/client/UsiClient.hpp"
#include "common/util/StringUtil.hpp"
#include "core/record/SfenParser.hpp"
#include "search/eval/Material.hpp"
#include "logger/Logger.hpp"
#include <sstream>
#include <utility>
#include <unordered_map>
#include <functional>
#include <cctype>
#include <cstdint>
#include <cstdlib>

namespace {

const char* ProgramName = "Sunfish4";
const char* Author = "Kubo Ryosuke";

} // namespace

namespace sunfish {

UsiClient::UsiClient() :
    state_(State::None),
    positionIsInitialized_(false) {
  searcher_.setHandler(this);
}

bool UsiClient::start() {
  if (!checkStateIn(State::None)) {
    Loggers::warning << "invalid state: " << toString(state_);
    return false;
  }

  changeState(State::Ready);

  bool usiAccepted = acceptUsiCommand();
  if (!usiAccepted) {
    Loggers::error << "invalid command is received.";
    return false;
  }

  bool ok = runCommandLoop();

  changeState(State::None);

  return ok;
}

bool UsiClient::acceptUsiCommand() {
  auto command = receive();
  if (command != "usi") {
    return false;
  }

  send("id", "name", ProgramName);
  send("id", "author", Author);

  // TODO: send options

  send("usiok");

  return true;
}

bool UsiClient::runCommandLoop() {
#define MAKE_COMMAND_HANDLER(func) ([this](const CommandArguments& args) { return func(args); })
  using Handler = bool(const CommandArguments&);
  using HandlerWrapper = std::function<Handler>;
  std::unordered_map<std::string, HandlerWrapper> handlerMap = {
    { "isready", MAKE_COMMAND_HANDLER(onIsReady) },
    { "setoption", MAKE_COMMAND_HANDLER(onSetOption) },
    { "usinewgame", MAKE_COMMAND_HANDLER(onUsiNewGame) },
    { "position", MAKE_COMMAND_HANDLER(onPosition) },
    { "go", MAKE_COMMAND_HANDLER(onGo) },
    { "stop", MAKE_COMMAND_HANDLER(onStop) },
    { "ponderhit", MAKE_COMMAND_HANDLER(onPonderhit) },
    { "gameover", MAKE_COMMAND_HANDLER(onGameOver) },
  };

  for (;;) {
    auto command = receive();

    auto args = StringUtil::split(command, [](char c) {
      return isspace(c);
    });

    if (args.empty()) {
      Loggers::warning << "empty line is received.";
      continue;
    }

    if (args[0] == "quit") {
      stopSearchIfRunning();
      Loggers::message << "quit";
      return true;
    }

    auto ite = handlerMap.find(args[0]);
    if (ite == handlerMap.end()) {
      Loggers::warning << "unsupported command is received. '" << command << "'";
      continue;
    }

    const auto& handler = ite->second;
    bool ok = handler(args);
    if (!ok) {
      Loggers::warning << "fatal error occurred. '" << command << "'";
    }
  }
}

bool UsiClient::onIsReady(const CommandArguments&) {
  if (!checkStateIn(State::Ready)) {
    Loggers::warning << "invalid state: " << toString(state_);
    return false;
  }

  send("readyok");

  return true;
}

bool UsiClient::onSetOption(const CommandArguments&) {
  // TODO
 
  return true;
}

bool UsiClient::onUsiNewGame(const CommandArguments&) {
  if (!checkStateIn(State::Ready)) {
    Loggers::warning << "invalid state: " << toString(state_);
    return false;
  }

  // TODO
 
  return true;
}

bool UsiClient::onPosition(const CommandArguments& args) {
  if (!checkStateIn(State::Ready)) {
    Loggers::warning << "invalid state: " << toString(state_);
    return false;
  }

  size_t nextIndex;
  if (args.size() >= 2 && args[1] == "startpos") {
    position_.initialize(Position::Handicap::Even);
    nextIndex = 2;

  } else if (args.size() >= 6 && args[1] == "sfen") {
    bool ok = SfenParser::parsePosition(args[2], args[3], args[4], args[5], position_);
    if (!ok) {
      return false;
    }
    nextIndex = 6;

  } else {
    Loggers::error << "illegal arguments";
    return false;
  }

  positionIsInitialized_ = true;

  if (args.size() <= nextIndex) {
    return true;
  }

  if (args[nextIndex] != "moves") {
    Loggers::error << "illegal arguments";
    return false;
  }

  for (auto i = nextIndex + 1; i < args.size(); i++) {
    Move move;
    bool parseOk = SfenParser::parseMove(args[i], position_, move);
    if (!parseOk) {
      Loggers::error << "illegal arguments";
      return false;
    }

    bool moveOk = position_.doMove(move);
    if (!moveOk) {
      Loggers::error << "illegal move";
      return false;
    }
  }

  Loggers::message << position_;
 
  return true;
}

bool UsiClient::onGo(const CommandArguments& args) {
  if (!checkStateIn(State::Ready, State::Ponder)) {
    Loggers::warning << "invalid state: " << toString(state_);
    return false;
  }

  if (!positionIsInitialized_) {
    Loggers::error << "position command has not received.";
    return false;
  }

  if (args[1] == "ponder") {
    // TODO
    return true;
  }

  config_.blackTimeMilliSeconds = 0;
  config_.whiteTimeMilliSeconds = 0;
  config_.byoyomiMilliSeconds = 0;
  config_.infinite = false;

  for (size_t i = 1; i < args.size(); i++) {
    if (args[i] == "btime") {
      config_.blackTimeMilliSeconds = strtol(args[++i].c_str(), nullptr, 10);

    } else if (args[i] == "wtime") {
      config_.whiteTimeMilliSeconds = strtol(args[++i].c_str(), nullptr, 10);

    } else if (args[i] == "byoyomi") {
      config_.byoyomiMilliSeconds = strtol(args[++i].c_str(), nullptr, 10);

    } else if (args[i] == "infinite") {
      config_.infinite = true;

    } else if (args[i] == "mate") {
      Loggers::error << "mate option is not supported";
      return false;
    }
  }

  Loggers::message << "btime     = " << config_.blackTimeMilliSeconds;
  Loggers::message << "wtime     = " << config_.whiteTimeMilliSeconds;
  Loggers::message << "byoyomi   = " << config_.byoyomiMilliSeconds;
  Loggers::message << "inifinite = " << (config_.infinite ? "true" : "false");

  stopSearchIfRunning();

  changeState(State::Search);

  searchThread_.reset(new std::thread([this]() {
    search();
  }));
 
  return true;
}

bool UsiClient::onStop(const CommandArguments&) {
  stopSearchIfRunning();

  if (config_.infinite) {
    sendBestMove();
  }
 
  return true;
}

void UsiClient::search() {
  Loggers::message << "search thread is started. tid=" << std::this_thread::get_id();

  int depth = 5;

  searcher_.idsearch(position_, depth * Searcher::Depth1Ply);

  if (!config_.infinite) {
    sendBestMove();
  }

  changeState(State::Ready);

  Loggers::message << "search thread is stopped. tid=" << std::this_thread::get_id();
}

void UsiClient::onUpdatePV(const PV& pv, int depth, Value value) {
  if (pv.size() == 0) {
    Loggers::warning << "PV is empty: " << __FILE__ << ':' << __LINE__;
    return;
  }

  if (value > -Value::mate() && value < Value::mate()) {
    int valueCentiPawn = value.raw() * 100.0 / material::Pawn;

    send("info",
         "depth", depth,
         "currmove", pv.get(0).toStringSFEN(),
         "score", "cp", valueCentiPawn,
         "pv", pv.toStringSFEN());

  } else {
    int plyToMate;
    if (value >= 0) {
      plyToMate = (Value::infinity() - value).raw();
    } else {
      plyToMate = -(Value::infinity() + value).raw();
    }

    send("info",
         "depth", depth,
         "currmove", pv.get(0).toStringSFEN(),
         "score", "mate", plyToMate,
         "pv", pv.toStringSFEN());
  }
}

void UsiClient::onFailLow(const PV& pv, int depth, Value value) {
  onUpdatePV(pv, depth, value);
  send("info", "string", "fail-low");
}

void UsiClient::onFailHigh(const PV& pv, int depth, Value value) {
  onUpdatePV(pv, depth, value);
  send("info", "string", "fail-high");
}

void UsiClient::stopSearchIfRunning() {
  if (searchThread_.get() && searchThread_->joinable()) {
    Loggers::message << "stopping search thread..";
    searcher_.interrupt();
    searchThread_->join();
  }
}

void UsiClient::sendBestMove() {
  const auto& info = searcher_.getInfo();

  if (!info.move.isEmpty()) {
    send("bestmove", info.move.toStringSFEN());
  } else {
    send("bestmove", "resign");
  }
}

bool UsiClient::onPonderhit(const CommandArguments&) {
  // TODO
 
  return true;
}

bool UsiClient::onGameOver(const CommandArguments&) {
  changeState(State::Ready);
 
  return true;
}

std::string UsiClient::receive() {
  std::string command;
  std::getline(std::cin, command);
  Loggers::receive << command;
  return command;
}

template <class T>
void UsiClient::send(T&& command) {
  std::lock_guard<std::mutex> lock(sendMutex_);

  std::cout << command << std::endl;
  Loggers::send << command;
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

bool UsiClient::checkStateIn(State state) const {
  if (state_ == state) {
    return true;
  }
  return false;
}

template <class... Args>
bool UsiClient::checkStateIn(State state, Args... args) const {
  if (state_ == state) {
    return true;
  }
  return checkStateIn(args...);
}

void UsiClient::changeState(State state) {
  Loggers::message << "change state: " << toString(state_) << " => " << toString(state);
  state_ = state;
}

std::string UsiClient::toString(State state) {
  switch (state) {
  case State::None  : return "None";
  case State::Ready : return "Ready";
  case State::Ponder: return "Ponder";
  case State::Search: return "Search";
  case State::Mate  : return "Mate";
  }
  return "";
}

} // namespace sunfish
