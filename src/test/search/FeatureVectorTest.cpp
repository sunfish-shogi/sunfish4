/* FeatureVectorTest.cpp
 *
 * Kubo Ryosuke
 */

#include "test/Test.hpp"
#include "search/eval/FeatureVector.hpp"
#include "core/position/Position.hpp"
#include "core/util/PositionUtil.hpp"

using namespace sunfish;

TEST(FeatureVectorTest, testGetHSymNeighbor3x3) {
  ASSERT_EQ(getNeighbor3x3(Square::s55(), Square::s44()), getHSymNeighbor3x3(getNeighbor3x3(Square::s55(), Square::s64())));
  ASSERT_EQ(getNeighbor3x3(Square::s55(), Square::s64()), getHSymNeighbor3x3(getNeighbor3x3(Square::s55(), Square::s44())));
}

TEST(FeatureVectorTest, testGetNeighbor3x3) {
  ASSERT_EQ(Neighbor3x3::N00, getNeighbor3x3(Square::s55(), Square::s64()));
  ASSERT_EQ(Neighbor3x3::N01, getNeighbor3x3(Square::s55(), Square::s65()));
  ASSERT_EQ(Neighbor3x3::N22, getNeighbor3x3(Square::s55(), Square::s46()));

  ASSERT_EQ(Neighbor3x3::N22, getNeighbor3x3R(Square::s55(), Square::s64()));
  ASSERT_EQ(Neighbor3x3::N21, getNeighbor3x3R(Square::s55(), Square::s65()));
  ASSERT_EQ(Neighbor3x3::N00, getNeighbor3x3R(Square::s55(), Square::s46()));
}
