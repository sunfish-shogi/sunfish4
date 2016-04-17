/* CsaClient.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_CSA_CLIENT_USICLIENT_HPP__
#define SUNFISH_CSA_CLIENT_USICLIENT_HPP__

#include "search/Searcher.hpp"
#include "core/record/Record.hpp"
#include "csa/client/Socket.hpp"
#include <string>
#include <atomic>
#include <memory>
#include <mutex>

namespace sunfish {

class CsaClient : public LoggingSearchHandler {
public:

  using TimeType = SearchConfig::TimeType;

  struct Config {
    std::string host;
    int port;
    std::string user;
    std::string pass;
    int floodgate;

    int depth;
    int limit;
    int repeat;
    int worker;
    int ponder;
    int hashMem;
    int marginMs;

    int keepalive;
    int keepidle;
    int keepintvl;
    int keepcnt;

    std::string kifuDir;
  };

  struct GameSummary {
    Turn myTurn;
    std::string gameId;
    std::string blackName;
    std::string whiteName;
    Turn firstTurn;
    int maxMoves;
    std::string protocolVersion;
    std::string protocolMode;
    std::string format;
    std::string declaration;
    std::string rematchOnDraw;

    int totalTime;
    int byoyomi;
    int leastTimePerMove;
    std::string timeRoundup;
    int delay;
    int increment;
  };

  CsaClient();

  CsaClient(const CsaClient&) = delete;

  CsaClient(CsaClient&&) = delete;

  bool start();

private:

  void readConfigFromIniFile();

  bool validateConfig();

  void playOnRepeat();

  void play();

  bool login();

  bool logout();

  bool onGameSummary();

  bool onGameSummaryTime();

  bool onGameSummaryPosition();

  bool parseGameSummary(const std::string& str);

  bool agree();

  bool onGameResult();

  bool onMove();

  void search();

  void ponder();

  void waitForSearcherIsStarted();

  void onStart(const Searcher&) override;

  template <class T>
  bool send(const T& str);

  bool receive();

  bool writeRecord();

private:

  Socket socket_;
  std::string lastReceivedString_;

  Config config_;
  GameSummary gameSummary_;
  std::string gameId_;
  Record record_;
  Position position_;
  int blackTime_;
  int whiteTime_;

  Searcher searcher_;
  std::atomic<bool> searcherIsStarted_;
  std::mutex sendMutex_;

};

} // namespace sunfish

#endif // SUNFISH_CSA_CLIENT_USICLIENT_HPP__
