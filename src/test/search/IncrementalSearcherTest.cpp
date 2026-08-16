/* IncrementalSearcherTest.cpp */

#include "test/Test.hpp"
#include "search/Searcher.hpp"
#include "core/position/Position.hpp"
#include <memory>

using namespace sunfish;

namespace {

std::unique_ptr<Searcher> createSearcher() {
  std::unique_ptr<Searcher> searcher(new Searcher());
  auto config = searcher->getConfig();
  config.numberOfThreads = 1;
  config.optimumTimeMs = 1000;
  config.maximumTimeMs = 1000;
  searcher->setConfig(config);
  return searcher;
}

} // namespace

TEST(IncrementalSearcherTest, completesOneIteration) {
  Position position;
  position.initialize(Position::Handicap::Even);
  auto searcher = createSearcher();

  ASSERT_TRUE(searcher->startIDSearch(position, Searcher::Depth1Ply));
  ASSERT_FALSE(searcher->pollIDSearch());
  ASSERT_FALSE(searcher->getResult().move.isNone());
}

TEST(IncrementalSearcherTest, preservesLegalFallbackOnStop) {
  Position position;
  position.initialize(Position::Handicap::Even);
  auto searcher = createSearcher();

  ASSERT_TRUE(searcher->startIDSearch(position, Searcher::DepthInfinity));
  searcher->stopIDSearch();
  ASSERT_FALSE(searcher->isIDSearchActive());
  ASSERT_FALSE(searcher->getResult().move.isNone());
}
