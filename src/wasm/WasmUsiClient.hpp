/* WasmUsiClient.hpp
 *
 * Threaded USI adapter for the ShogiHome wasm engine ABI.
 */

#ifndef SUNFISH_WASM_WASMUSICLIENT_HPP__
#define SUNFISH_WASM_WASMUSICLIENT_HPP__

#include "book/Book.hpp"
#include "common/math/Random.hpp"
#include "core/record/Record.hpp"
#include "search/SearchHandler.hpp"
#include "search/Searcher.hpp"
#include <cstdint>
#include <atomic>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

namespace sunfish {

class WasmUsiClient : public SearchHandler {
public:
  WasmUsiClient();
  ~WasmUsiClient();

  void command(const std::string& line);

private:
  using Arguments = std::vector<std::string>;

  void acceptUsi();
  void setOption(const Arguments& args);
  void ready();
  void setPosition(const Arguments& args);
  void go(const Arguments& args);
  void stop(bool emitBestmove);
  void joinSearchThread();
  void emitBestmove();
  void output(const std::string& line);

  void onStart(const Searcher&) override;
  void onUpdatePV(const Searcher&, const PV&, float, int, Score, int) override;
  void onFailLow(const Searcher&, const PV&, float, int, Score) override;
  void onFailHigh(const Searcher&, const PV&, float, int, Score) override;
  void onIterateEnd(const Searcher&, float, int) override;

  std::unique_ptr<Searcher> searcher_;
  Book book_;
  Random random_;
  Record record_;
  bool initialized_;
  bool ready_;
  std::atomic_bool terminated_;
  std::atomic_bool searching_;
  std::atomic_bool infinite_;
  std::atomic_bool resultPending_;
  std::atomic_bool stopRequested_;
  std::atomic_bool suppressBestmove_;
  bool bookLoaded_;
  bool useBook_;
  unsigned hashMB_;
  bool snappy_;
  int marginMs_;
  int maxDepth_;
  int multiPV_;
  int numberOfThreads_;
  std::thread searchThread_;
  std::mutex outputMutex_;
};

} // namespace sunfish

#endif // SUNFISH_WASM_WASMUSICLIENT_HPP__
