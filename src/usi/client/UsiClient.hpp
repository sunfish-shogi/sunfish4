/* UsiClient.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_USI_CLIENT_USICLIENT_HPP__
#define SUNFISH_USI_CLIENT_USICLIENT_HPP__

#include "common/math/Random.hpp"
#include "core/position/Position.hpp"
#include "core/record/Record.hpp"
#include "book/Book.hpp"
#include "search/Searcher.hpp"
#include <atomic>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <mutex>
#include <thread>
#include <queue>
#include <utility>
#include <cstdint>

namespace sunfish {

class UsiClient : public SearchHandler {
private:

  using CommandArguments = std::vector<std::string>;

  using TimeType = SearchConfig::TimeType;

  struct Options {
    std::atomic_bool ponder;
    std::atomic_uint hash;
    std::atomic_bool useBook;
    std::atomic_bool snappy;
    std::atomic_int marginMs;
    std::atomic_int numberOfThreads;
    std::atomic_int maxDepth;
    std::atomic_int multiPV;
  };

  enum class CommandState : uint8_t {
    Ok,
    Broken,
  };

public:

  UsiClient();

  void start();

private:

  void ready();
  void receiveNewGame();
  void game();
  void receiveGo();

  void runSearch(const CommandArguments& args);
  void search();

  void runPonder(const CommandArguments& args);
  void ponder();

  void waitForSearcherIsStarted();
  void waitForStopCommand();

  void onStart(const Searcher&) override;
  void onUpdatePV(const Searcher& searcher, const PV& pv, float elapsed, int depth, Score score, bool failLow, bool failHigh, int multiPV);
  void onUpdatePV(const Searcher& searcher, const PV& pv, float elapsed, int depth, Score score, int multiPV) override;
  void onFailLow(const Searcher& searcher, const PV& pv, float elapsed, int depth, Score score) override;
  void onFailHigh(const Searcher& searcher, const PV& pv, float elapsed, int depth, Score score) override;
  void onIterateEnd(const Searcher& searcher, float elapsed, int depth) override;

  std::string receive();

  std::pair<CommandState, std::string> receiveWithBreak();

  void receiver();
  void acceptUsi();
  void setOption(const CommandArguments&);

  void breakReceive();

  template <class T>
  void send(T&& command);

  template <class T, class... Args>
  void send(T&& command, Args&&... options);

  template <class T>
  void joinOptions(std::ostream& os, T&& arg);

  template <class T, class... Args>
  void joinOptions(std::ostream& os, T&& arg, Args&&... args);

  Options options_;

  std::queue<std::string> deferredCommands_;
  std::queue<std::string> commandQueue_;

  std::string lastPositionCommand_;
  std::string lastGoCommand_;

  Record record_;

  TimeType blackTimeMs_;
  TimeType whiteTimeMs_;
  TimeType byoyomiMs_;
  TimeType blackIncMs_;
  TimeType whiteIncMs_;
  bool isInfinite_;
  bool inPonder_;

  std::unique_ptr<Searcher> searcher_;
  std::atomic<bool> searcherIsStarted_;
  std::atomic<bool> stopCommandReceived_;
  std::atomic<bool> breakReceiver_;

  Book book_;
  bool isBookLoaded;

  Random random_;

  std::mutex sendMutex_;
  std::mutex receiveMutex_;
  std::thread receiver_;

};

} // namespace sunfish

#endif // SUNFISH_USI_CLIENT_USICLIENT_HPP__
