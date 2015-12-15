/* UsiClient.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_USI_CLIENT_USICLIENT_HPP__
#define SUNFISH_USI_CLIENT_USICLIENT_HPP__

#include "core/position/Position.hpp"
#include "search/Searcher.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <mutex>
#include <cstdint>

namespace sunfish {

class UsiClient : public SearchHandler{
private:

  using CommandArguments = std::vector<std::string>;

  enum class State : uint8_t {
    None,
    Ready,
    Ponder,
    Search,
    Mate,
  };

  enum class CommandState : uint8_t {
    Ok,
    Eof,
    Error,
  };

  struct Command {
    CommandState state;
    std::string value;
  };

public:

  UsiClient();

  bool start();

private:

  bool acceptUsiCommand();

  bool runCommandLoop();

  bool onIsReady(const CommandArguments&);

  bool onSetOption(const CommandArguments&);

  bool onUsiNewGame(const CommandArguments&);

  bool onPosition(const CommandArguments&);

  bool onGo(const CommandArguments&);

  bool onStop(const CommandArguments&);

  void search();

  void waitForSearcherIsStarted();

  void onStart() override;

  void onUpdatePV(const PV& pv, float elapsed, int depth, Score score) override;

  void onFailLow(const PV& pv, float elapsed, int depth, Score score) override;

  void onFailHigh(const PV& pv, float elapsed, int depth, Score score) override;

  void stopSearchIfRunning();

  void sendBestMove();

  void outputSearchInfo();

  bool onPonderhit(const CommandArguments&);

  bool onGameOver(const CommandArguments&);

  Command receive();

  template <class T>
  void send(T&& command);

  template <class T, class... Args>
  void send(T&& command, Args&&... options);

  template <class T>
  void joinOptions(std::ostream& os, T&& arg);

  template <class T, class... Args>
  void joinOptions(std::ostream& os, T&& arg, Args&&... args);

  bool checkStateIn(State state) const;

  template <class... Args>
  bool checkStateIn(State state, Args... args) const;

  void changeState(State state);

  static std::string toString(State state);

  std::atomic<State> state_;

  bool positionIsInitialized_;
  Position position_;

  uint32_t blackTimeMilliSeconds_;
  uint32_t whiteTimeMilliSeconds_;
  uint32_t byoyomiMilliSeconds_;
  bool isInfinite_;

  Searcher searcher_;
  std::atomic<bool> searcherIsStarted_;

  std::unique_ptr<std::thread> searchThread_;
  std::mutex sendMutex_;

};

} // namespace sunfish

#endif // SUNFISH_USI_CLIENT_USICLIENT_HPP__
