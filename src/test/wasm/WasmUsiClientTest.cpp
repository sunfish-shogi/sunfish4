/* WasmUsiClientTest.cpp */

#include "test/Test.hpp"
#include "wasm/WasmUsiClient.hpp"
#include <chrono>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <thread>

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

unsigned countOccurrences(const std::string& text, const std::string& pattern) {
  unsigned count = 0;
  std::string::size_type position = 0;
  while ((position = text.find(pattern, position)) != std::string::npos) {
    ++count;
    position += pattern.size();
  }
  return count;
}

} // namespace

TEST(WasmUsiClientTest, infiniteWaitsForStopAtMaximumDepth) {
  OutputCapture output;
  auto client = createClient();
  client->command("setoption name MaxDepth value 1");
  client->command("position startpos");
  client->command("go infinite");
  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  client->command("stop");
  ASSERT_EQ(1u, countOccurrences(output.str(), "bestmove"));
}

TEST(WasmUsiClientTest, advertisesThreads) {
  OutputCapture output;
  WasmUsiClient client;
  client.command("usi");
  ASSERT_TRUE(output.str().find(
      "option name Threads type spin default 1 min 1 max 4") !=
      std::string::npos);
}

TEST(WasmUsiClientTest, depthSearchIsUntimed) {
  OutputCapture output;
  auto client = createClient();
  client->command("position startpos");
  client->command("go depth 3");
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  client->command("stop");

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
  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  client->command("stop");
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
  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  client->command("stop");

  ASSERT_TRUE(output.str().find(" score mate ") != std::string::npos);
}
