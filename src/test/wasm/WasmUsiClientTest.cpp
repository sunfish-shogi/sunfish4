/* WasmUsiClientTest.cpp */

#include "test/Test.hpp"
#include "wasm/WasmUsiClient.hpp"
#include <chrono>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

using namespace sunfish;

namespace {

class OutputCapture {
public:
  OutputCapture() : original_(std::cout.rdbuf(output_.rdbuf())) {}

  ~OutputCapture() {
    std::cout.rdbuf(original_);
  }

  std::string str() const {
    return output_.str();
  }

private:
  std::ostringstream output_;
  std::streambuf* original_;
};

std::unique_ptr<WasmUsiClient> createClient() {
  std::unique_ptr<WasmUsiClient> client(new WasmUsiClient());
  client->command("setoption name UseBook value false");
  client->command("isready");
  return client;
}

} // namespace

TEST(WasmUsiClientTest, infiniteWaitsForStopAtMaximumDepth) {
  OutputCapture output;
  auto client = createClient();
  client->command("setoption name MaxDepth value 1");
  client->command("position startpos");
  client->command("go infinite");
  client->poll();

  ASSERT_TRUE(output.str().find("bestmove") == std::string::npos);

  client->command("stop");
  ASSERT_TRUE(output.str().find("bestmove") != std::string::npos);
}

TEST(WasmUsiClientTest, infinitePollIsBounded) {
  OutputCapture output;
  auto client = createClient();
  client->command("position startpos");
  client->command("go infinite");
  for (int i = 0; i < 20; ++i) {
    auto start = std::chrono::steady_clock::now();
    client->poll();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - start).count();
    ASSERT_TRUE(elapsed < 1000);
  }
  ASSERT_TRUE(output.str().find("bestmove") == std::string::npos);
  client->command("stop");
}

TEST(WasmUsiClientTest, depthSearchIsUntimed) {
  OutputCapture output;
  auto client = createClient();
  client->command("position startpos");
  client->command("go depth 3");
  for (int i = 0; i < 4; ++i) {
    client->poll();
  }

  ASSERT_TRUE(output.str().find("info time") != std::string::npos);
  ASSERT_TRUE(output.str().find(" depth 3 ") != std::string::npos);
  ASSERT_TRUE(output.str().find("bestmove") != std::string::npos);
}

TEST(WasmUsiClientTest, rejectsIllegalMoveHistory) {
  OutputCapture output;
  auto client = createClient();
  client->command("position startpos");
  client->command("position startpos moves 7g7f 7g7f");

  ASSERT_TRUE(output.str().find("info string invalid position command") !=
      std::string::npos);

  client->command("go depth 1");
  client->poll();
  ASSERT_TRUE(output.str().find("bestmove") != std::string::npos);
}

TEST(WasmUsiClientTest, rejectsInvalidPromotions) {
  OutputCapture output;
  auto client = createClient();
  client->command("position startpos moves 7g7f+");
  ASSERT_TRUE(output.str().find("info string invalid position command") !=
      std::string::npos);
}

TEST(WasmUsiClientTest, rejectsPawnDropMate) {
  OutputCapture output;
  auto client = createClient();
  client->command(
      "position sfen 8k/9/7+R1/9/9/9/9/9/4K4 b P 1 moves P*1b");
  ASSERT_TRUE(output.str().find("info string invalid position command") !=
      std::string::npos);
}

TEST(WasmUsiClientTest, rejectsPositionsWithoutKings) {
  OutputCapture output;
  auto client = createClient();
  client->command("position sfen 9/9/9/9/9/9/9/9/9 b - 1");
  ASSERT_TRUE(output.str().find("info string invalid position command") !=
      std::string::npos);
}

TEST(WasmUsiClientTest, reportsMateScore) {
  OutputCapture output;
  auto client = createClient();
  client->command("position sfen 4k4/9/4P4/9/9/9/9/9/4K4 b G 1");
  client->command("go depth 1");
  client->poll();

  ASSERT_TRUE(output.str().find(" score mate ") != std::string::npos);
}
