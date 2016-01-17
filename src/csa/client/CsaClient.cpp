/* CsaClient.cpp
 *
 * Kubo Ryosuke
 */

#include "csa/client/CsaClient.hpp"
#include "core/record/CsaReader.hpp"
#include "core/record/CsaWriter.hpp"
#include "common/resource/Resource.hpp"
#include "common/string/StringUtil.hpp"
#include "common/string/Wildcard.hpp"
#include "logger/Logger.hpp"
#include <functional>
#include <fstream>
#include <chrono>
#include <utility>
#include <thread>
#include <sstream>

namespace {

using namespace sunfish;

const char* const CsaIni = "config/csa.ini";

CONSTEXPR_CONST int DefaultPort      = 4081;
CONSTEXPR_CONST int DefaultFloodgate = 0;

CONSTEXPR_CONST int DefaultDepth  = 48;
CONSTEXPR_CONST int DefaultLimit  = 28;
CONSTEXPR_CONST int DefaultRepeat = 1000;
CONSTEXPR_CONST int DefaultPonder = 1;
CONSTEXPR_CONST int DefaultHashMem = 64;

CONSTEXPR_CONST int DefaultKeepAlive = 0;
CONSTEXPR_CONST int DefaultKeepIdle  = 120;
CONSTEXPR_CONST int DefaultKeepIntvl = 60;
CONSTEXPR_CONST int DefaultKeepCnt   = 10;

const Wildcard LoginOk("LOGIN:* OK");
const Wildcard Start("START:*");
const Wildcard BeginGameSummary("BEGIN Game_Summary");
const Wildcard EndGameSummary("END Game_Summary");
const Wildcard BeginTime("BEGIN Time");
const Wildcard EndTime("END Time");
const Wildcard BeginPosition("BEGIN Position");
const Wildcard EndPosition("END Position");
const Wildcard GameResult("#*");
const Wildcard SpecialMove("%*");
const Wildcard Win("#WIN");
const Wildcard Lose("#LOSE");
const Wildcard Draw("#DRAW");
const Wildcard Chudan("#CHUDAN");
const Wildcard Sennichite("#SENNICHITE");
const Wildcard OutSennichite("#OUTE_SENNICHITE");
const Wildcard IllegalMove("#ILLEGAL_MOVE");
const Wildcard TimeUp("#TIME_UP");
const Wildcard Resign("#RESIGN");
const Wildcard Jishogi("#JISHOGI");
const Wildcard MaxMoves("#MAX_MOVES");
const Wildcard Censored("#CENSORED");

class ScopedThread {
public:

  ScopedThread() {
  }

  ~ScopedThread() {
    if (thread_.joinable()) {
      if (stop_) {
        stop_();
      }
      thread_.join();
    }
  }

  template <class T, class U>
  void start(T&& proc,
             U&& stop) {
    thread_ = std::thread(proc);
    stop_ = std::forward<U>(stop);
  }

private:

  std::thread thread_;
  std::function<void()> stop_;

};

} // namespace

namespace sunfish {

CsaClient::CsaClient() {
  searcher_.setHandler(this);
}

bool CsaClient::start() {
  OUT(info) << "####################################################################";
  OUT(info) << "##                           CSA Client                           ##";
  OUT(info) << "####################################################################";

  readConfigFromIniFile();
  if (!validateConfig()) {
    return false;
  }

  searcher_.ttResizeMB(config_.hashMem);

  playOnRepeat();

  return true;
}

void CsaClient::readConfigFromIniFile() {
  auto ini = Resource::ini(CsaIni);

  config_.host      = getValue(ini, "Server", "Host");
  config_.port      = StringUtil::toInt(getValue(ini, "Server", "Port"), DefaultPort);
  config_.user      = getValue(ini, "Server", "User");
  config_.pass      = getValue(ini, "Server", "Pass");
  config_.floodgate = StringUtil::toInt(getValue(ini, "Server", "Floodgate"), DefaultFloodgate);

  config_.depth   = StringUtil::toInt(getValue(ini, "Search", "Depth"), DefaultDepth);
  config_.limit   = StringUtil::toInt(getValue(ini, "Search", "Limit"), DefaultLimit);
  config_.repeat  = StringUtil::toInt(getValue(ini, "Search", "Repeat"), DefaultRepeat);
  config_.worker  = StringUtil::toInt(getValue(ini, "Search", "Worker"), std::thread::hardware_concurrency());
  config_.ponder  = StringUtil::toInt(getValue(ini, "Search", "Ponder"), DefaultPonder);
  config_.hashMem = StringUtil::toInt(getValue(ini, "Search", "HashMem"), DefaultHashMem);

  config_.keepalive = StringUtil::toInt(getValue(ini, "KeepAlive", "KeepAlive"), DefaultKeepAlive);
  config_.keepidle  = StringUtil::toInt(getValue(ini, "KeepIdle", "KeepIdle"), DefaultKeepIdle);
  config_.keepintvl = StringUtil::toInt(getValue(ini, "KeepIntvl", "KeepIntvl"), DefaultKeepIntvl);
  config_.keepcnt   = StringUtil::toInt(getValue(ini, "KeepCnt", "KeepCnt"), DefaultKeepCnt);

  config_.kifuDir = getValue(ini, "File", "KifuDir");

  OUT(info) << "Configurations";
  OUT(info) << "  Server";
  OUT(info) << "    host     : " << config_.host;
  OUT(info) << "    port     : " << config_.port;
  OUT(info) << "    user     : " << config_.user;
  OUT(info) << "    pass     : " << config_.pass;
  OUT(info) << "    floodgate: " << config_.floodgate;
  OUT(info) << "  Search";
  OUT(info) << "    depth    : " << config_.depth;
  OUT(info) << "    limit    : " << config_.limit;
  OUT(info) << "    repeat   : " << config_.repeat;
  OUT(info) << "    worker   : " << config_.worker;
  OUT(info) << "    ponder   : " << config_.ponder;
  OUT(info) << "    hashMem  : " << config_.hashMem;
  OUT(info) << "  KeepAlive";
  OUT(info) << "    keepalive: " << config_.keepalive;
  OUT(info) << "    keepidle : " << config_.keepidle ;
  OUT(info) << "    keepintvl: " << config_.keepintvl;
  OUT(info) << "    keepcnt  : " << config_.keepcnt;
  OUT(info) << "  File";
  OUT(info) << "    kifuDir  : " << config_.kifuDir;
  OUT(info) << "";
}

bool CsaClient::validateConfig() {
  if (config_.worker <= 0) {
    LOG(error) << CsaIni << ": Search.Worker shall not be less than 1.";
    return false;
  }

  return true;
}

bool CsaClient::playOnRepeat() {
  for (int cnt = 0; cnt < config_.repeat; cnt++) {
    if (!play()) {
      return false;
    }
  }
  return true;
}

bool CsaClient::play() {
  // connect to the CSA server
  Socket::AutoDisconnector ad(socket_);
  socket_.setHost(config_.host);
  socket_.setPort(config_.port);
  socket_.setKeepalive(config_.keepalive,
                       config_.keepidle,
                       config_.keepintvl,
                       config_.keepcnt);
  bool connected = socket_.connect();
  if (!connected) {
    LOG(error) << "connection failed";
    return false;
  }

  bool loginOk = login();
  if (!loginOk) {
    LOG(error) << "login failed";
    return false;
  }

  if (!onGameSummary()) {
    LOG(error) << "an error occured in onGameSummary";
    return false;
  }

  if (!agree()) {
    LOG(error) << "an error occured in agree";
    return false;
  }

  blackTime_ = gameSummary_.totalTime;
  whiteTime_ = gameSummary_.totalTime;

  for (;;) {
    OUT(info) << "Time";
    OUT(info) << "  Black: " << blackTime_;
    OUT(info) << "  White: " << blackTime_;
    OUT(info) << "";

    ScopedThread searchThread;
    bool isMyTurn = gameSummary_.myTurn == position_.getTurn();
    if (isMyTurn) {
      // search
      searcherIsStarted_ = false;
      searchThread.start([this]() {
        search();
      }, [this]() {
        searcher_.interrupt();
      });
      waitForSearcherIsStarted();

    } else if (config_.ponder) {
      // ponder
      searcherIsStarted_ = false;
      searchThread.start([this]() {
        ponder();
      }, [this]() {
        searcher_.interrupt();
      });
      waitForSearcherIsStarted();
    }

    if (!receive()) {
      LOG(error) << " failed to receive.";
      return false;
    }

    if (SpecialMove.match(lastReceivedString_)) {
      record_.specialMove = lastReceivedString_;
      if (!receive()) {
        LOG(error) << " failed to receive.";
        return false;
      }
    }

    if (GameResult.match(lastReceivedString_)) {
      if (onGameResult()) {
        break;
      }
      return false;
    }

    if (!onMove()) {
      return false;
    }

    writeRecord();
  }

  writeRecord();

  bool logoutOk = logout();
  if (!logoutOk) {
    LOG(error) << "logout failed";
    return false;
  }

  return true;
}

bool CsaClient::login() {
  std::ostringstream os;
  os << "LOGIN " << config_.user << ' ' << config_.pass;
  return send(os.str()) &&
         receive() &&
         LoginOk.match(lastReceivedString_);
}

bool CsaClient::logout() {
  // Originary, a client program should wait "LOGOUT:completed" command.
  // But shogi-server(floodgate) is not send that.
  return send("LOGOUT");
}

bool CsaClient::onGameSummary() {
  gameSummary_.myTurn = Turn::Black;
  gameSummary_.gameId = "";
  gameSummary_.blackName = "";
  gameSummary_.whiteName = "";
  gameSummary_.firstTurn = Turn::Black;
  gameSummary_.maxMoves = 0;
  gameSummary_.protocolVersion = "";
  gameSummary_.protocolMode = "";
  gameSummary_.format = "";
  gameSummary_.declaration = "";
  gameSummary_.rematchOnDraw = "";

  gameSummary_.totalTime = 0;
  gameSummary_.byoyomi = 0;
  gameSummary_.leastTimePerMove = 0;
  gameSummary_.timeRoundup = "";
  gameSummary_.delay = 0;
  gameSummary_.increment = 0;

  bool beginOk = receive() &&
                 BeginGameSummary.match(lastReceivedString_);
  if (!beginOk) {
    return false;
  }

  for (;;) {
    if (!receive()) {
      return false;
    }

    if (EndGameSummary.match(lastReceivedString_)) {
      break;
    }

    if (BeginTime.match(lastReceivedString_)) {
      if (!onGameSummaryTime()) {
        return false;
      }
      continue;
    }

    if (BeginPosition.match(lastReceivedString_)) {
      if (!onGameSummaryPosition()) {
        return false;
      }
      continue;
    }

    if (!parseGameSummary(lastReceivedString_)) {
      return false;
    }
  }

  OUT(info) << "GameSummary";
  OUT(info) << "  MyTurn            : " << gameSummary_.myTurn;
  OUT(info) << "  GameId            : " << gameSummary_.gameId;
  OUT(info) << "  BlackName         : " << gameSummary_.blackName;
  OUT(info) << "  WhiteName         : " << gameSummary_.whiteName;
  OUT(info) << "  FirstTurn         : " << gameSummary_.firstTurn;
  OUT(info) << "  MaxMoves          : " << gameSummary_.maxMoves;
  OUT(info) << "  ProtocolVersion   : " << gameSummary_.protocolVersion;
  OUT(info) << "  ProtocolMode      : " << gameSummary_.protocolMode;
  OUT(info) << "  Format            : " << gameSummary_.format;
  OUT(info) << "  Declaration       : " << gameSummary_.declaration;
  OUT(info) << "  RematchOnDraw     : " << gameSummary_.rematchOnDraw;
  OUT(info) << "  Time";
  OUT(info) << "    TotalTime       : " << gameSummary_.totalTime;
  OUT(info) << "    Byoyomi         : " << gameSummary_.byoyomi;
  OUT(info) << "    LeastTimePerMove: " << gameSummary_.leastTimePerMove;
  OUT(info) << "    TimeRoundup     : " << gameSummary_.timeRoundup;
  OUT(info) << "    Delay           : " << gameSummary_.delay;
  OUT(info) << "    Increment       : " << gameSummary_.increment;
  OUT(info) << "";
  OUT(info) << position_.toString();

  return true;
}

bool CsaClient::onGameSummaryTime() {
  for (;;) {
    if (!receive()) {
      return false;
    }

    if (EndTime.match(lastReceivedString_)) {
      return true;
    }

    auto pair = StringUtil::splitOnce(lastReceivedString_, ':');
    auto& key = pair.first;
    auto& value = pair.second;
    
    if (key == "Total_Time") {
      gameSummary_.totalTime = std::stoi(value);
      continue;
    }
    
    if (key == "Byoyomi") {
      gameSummary_.byoyomi = std::stoi(value);
      continue;
    }

    if (key == "Time_Unit") {
      if (value != "1sec") {
        LOG(error) << "unsupported: " << value;
        return false;
      }
      continue;
    }

    if (key == "Least_Time_Per_Move") {
      gameSummary_.leastTimePerMove = std::stoi(value);
      continue;
    }

    if (key == "Time_Roundup") {
      gameSummary_.timeRoundup = value;
      continue;
    }

    if (key == "Delay") {
      gameSummary_.delay = std::stoi(value);
      continue;
    }

    if (key == "Increment") {
      gameSummary_.increment = std::stoi(value);
      continue;
    }
    
    Loggers::error << "unknown: " << key;
    return false;
  }
}

bool CsaClient::onGameSummaryPosition() {
  std::ostringstream oss;

  for (;;) {
    if (!receive()) {
      return false;
    }

    if (EndPosition.match(lastReceivedString_)) {
      break;
    }

    oss << lastReceivedString_ << '\n';
  }

  std::istringstream iss(oss.str());
  if (!CsaReader::read(iss, record_)) {
    return false;
  }
  position_ = generatePosition(record_, -1);

  return true;
}

bool CsaClient::parseGameSummary(const std::string& str) {
  auto pair = StringUtil::splitOnce(str, ':');
  auto& key = pair.first;
  auto& value = pair.second;

  if (key == "Your_Turn") {
    if      (value == "+") { gameSummary_.myTurn = Turn::Black; return true; }
    else if (value == "-") { gameSummary_.myTurn = Turn::White; return true; }
    else { Loggers::error << "invalid value: " << value; return false; }
  }
  
  if (key == "Game_ID") {
    gameSummary_.gameId = value;
    return true;
  }
  
  if (key == "Name+") {
    gameSummary_.blackName = value;
    return true;
  }
  
  if (key == "Name-") {
    gameSummary_.whiteName = value;
    return true;
  }

  if (key == "To_Move") {
    if      (value == "+") { gameSummary_.firstTurn = Turn::Black; return true; }
    else if (value == "-") { gameSummary_.firstTurn = Turn::White; return true; }
    else { Loggers::error << "invalid value: " << value; return false; }
  }

  if (key == "Max_Moves") {
    gameSummary_.maxMoves = std::stoi(value);
    return true;
  }
  
  if (key == "Protocol_Version") {
    gameSummary_.protocolVersion = value;
    return true;
  }

  if (key == "Protocol_Mode") {
    gameSummary_.protocolMode = value;
    return true;
  }

  if (key == "Format") {
    gameSummary_.format = value;
    return true;
  }

  if (key == "Declaration") {
    gameSummary_.declaration = value;
    return true;
  }

  if (key == "Rematch_On_Draw") {
    gameSummary_.rematchOnDraw = value;
    return true;
  }

  Loggers::warning << "unknown: " << key;
  return  false;
}

bool CsaClient::agree() {
  if (send("AGREE") &&
      receive() &&
      Start.match(lastReceivedString_)) {
    gameId_ = lastReceivedString_.substr(7);
    OUT(info) << "Game ID: " << gameId_;
    return true;
  }

  return false;
}

bool CsaClient::onGameResult() {
  bool nextCommand = false;

  if (Sennichite.match(lastReceivedString_)) {
    nextCommand = true;
  }

  if (OutSennichite.match(lastReceivedString_)) {
    nextCommand = true;
  }

  if (IllegalMove.match(lastReceivedString_)) {
    nextCommand = true;
  }

  if (TimeUp.match(lastReceivedString_)) {
    nextCommand = true;
  }

  if (Resign.match(lastReceivedString_)) {
    nextCommand = true;
  }

  if (Jishogi.match(lastReceivedString_)) {
    nextCommand = true;
  }

  if (MaxMoves.match(lastReceivedString_)) {
    nextCommand = true;
  }

  if (Chudan.match(lastReceivedString_)) {
    return true;
  }

  if (nextCommand) {
    if (!receive()) {
      return false;
    }
  }

  if (Win.match(lastReceivedString_)) {
    return true;
  }

  if (Lose.match(lastReceivedString_)) {
    return true;
  }

  if (Draw.match(lastReceivedString_)) {
    return true;
  }

  if (Censored.match(lastReceivedString_)) {
    return true;
  }

  LOG(error) << "unknown command: " << lastReceivedString_;
  return false;
}

bool CsaClient::onMove() {
  auto pair = StringUtil::splitOnce(lastReceivedString_, ',');
  const auto& mstr = pair.first;
  const auto& tstr = pair.second;
  Turn turn = position_.getTurn();

  Move move;
  bool ok = CsaReader::readMove(mstr.c_str(), position_, move);
  if (!ok) {
    LOG(error) << "invalid move format: " << mstr;
    return false;
  }

  Piece captured;
  if (!position_.doMove(move, captured)) {
    LOG(error) << "illegal move: " << move.toString(position_);
    return false;
  }
  record_.moveList.push_back(move);

  OUT(info) << "Move: " << record_.moveList.size() << ": " << move.toString(position_);
  OUT(info) << "";
  OUT(info) << position_;

  if (!tstr.empty() && tstr[0] == 'T') {
    int reduced = std::stoi(tstr.c_str() + 1);
    if (turn == Turn::Black) {
      blackTime_ = std::max(blackTime_ - reduced, 0)
                 + gameSummary_.increment;
    } else {
      whiteTime_ = std::max(whiteTime_ - reduced, 0)
                 + gameSummary_.increment;
    }
  }

  return true;
}

void CsaClient::search() {
  Turn turn = position_.getTurn();
  auto config = searcher_.getConfig();

  TimeType maximumMilliSeconds = turn == Turn::Black ?
                                 blackTime_ * 1000 :
                                 whiteTime_ * 1000;
  maximumMilliSeconds += gameSummary_.byoyomi;
  config.maximumMilliSeconds = std::min(8000u, maximumMilliSeconds);
  config.optimumMilliSeconds = std::min(8000u, maximumMilliSeconds / 5); // TODO;

  searcher_.setConfig(config);

  searcher_.idsearch(position_,
                     Searcher::DepthInfinity,
                     &record_);
  auto& result = searcher_.getResult();

  if (result.move.isEmpty()) {
    send("%TORYO");
    return;
  }

  std::ostringstream oss;
  oss << result.move.toString(position_);

  if (config_.floodgate) {
    auto score = position_.getTurn() == Turn::Black
               ? result.score.raw()
               : -result.score.raw();
    oss << ",\'*" << score;
    auto pos = position_;
    for (unsigned i = 0; i < result.pv.size(); i++) {
      Move move = result.pv.getMove(i);
      oss << ' ' << move.toString(pos);
      Piece captured;
      if (!pos.doMove(move, captured)) {
        LOG(warning) << "an illegal move is contained in PV.";
        break;
      }
    }
  }

  send(oss.str());
}

void CsaClient::ponder() {
  auto config = searcher_.getConfig();

  config.maximumMilliSeconds = SearchConfig::InfinityTime;
  config.optimumMilliSeconds = SearchConfig::InfinityTime;

  searcher_.setConfig(config);

  searcher_.idsearch(position_,
                     Searcher::DepthInfinity,
                     &record_);
}

void CsaClient::waitForSearcherIsStarted() {
  while (true) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    if (searcherIsStarted_) { break; }
  }
}

void CsaClient::onStart(const Searcher&) {
  searcherIsStarted_ = true;
}

template <class T>
bool CsaClient::send(const T& str) {
  std::lock_guard<std::mutex> lock(sendMutex_);

  std::ostringstream os;
  os << str << '\n';
  if (!socket_.sendString(os.str())) {
    return false;
  }

  OUT(send) << str;

  return true;
}

bool CsaClient::receive() {
  if (!socket_.receiveString(lastReceivedString_)) {
    return false;
  }

  // chomp
  if (lastReceivedString_.length() != 0 &&
      lastReceivedString_.back() == '\n') {
    lastReceivedString_.resize(lastReceivedString_.length() - 1);
  }

  OUT(receive) << lastReceivedString_;

  return true;
}

bool CsaClient::writeRecord() {
  std::ostringstream path;
  path << config_.kifuDir;
  if (!config_.kifuDir.empty() &&
      config_.kifuDir.back() != '/') {
    path << '/';
  }
  path << gameId_ << ".csa";

  std::ofstream fout(path.str(), std::ios::out);
  if (!fout) {
    return false;
  }

  RecordInfo info;
  info.title = gameId_;
  info.blackName = gameSummary_.blackName;
  info.whiteName = gameSummary_.whiteName;
  info.timeLimitHours = 0;
  info.timeLimitMinutes = gameSummary_.totalTime / 60;
  info.timeLimitReadoff = gameSummary_.byoyomi;
  CsaWriter::write(fout, record_, &info);

  fout.close();
  return false;
}

} // namespace sunfish
