/* WasmUsiClient.hpp
 *
 * Single-threaded USI adapter for the ShogiHome wasm engine ABI.
 */

#ifndef SUNFISH_WASM_WASMUSICLIENT_HPP__
#define SUNFISH_WASM_WASMUSICLIENT_HPP__

#include "book/Book.hpp"
#include "common/math/Random.hpp"
#include "core/record/Record.hpp"
#include "search/SearchHandler.hpp"
#include "search/Searcher.hpp"
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace sunfish {

class WasmUsiClient : public SearchHandler {
public:
  WasmUsiClient();

  void command(const std::string& line);
  void poll();

private:
  using Arguments = std::vector<std::string>;

  void acceptUsi();
  void setOption(const Arguments& args);
  void ready();
  void setPosition(const Arguments& args);
  void go(const Arguments& args);
  void stop(bool emitBestmove);
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
  bool terminated_;
  bool searching_;
  bool infinite_;
  bool resultPending_;
  bool bookLoaded_;
  bool useBook_;
  unsigned hashMB_;
  bool snappy_;
  int marginMs_;
  int maxDepth_;
  int multiPV_;
};

} // namespace sunfish

#endif // SUNFISH_WASM_WASMUSICLIENT_HPP__
