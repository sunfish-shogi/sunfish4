/* FeatureVectorTest.cpp
 *
 * Kubo Ryosuke
 */

#include "test/Test.hpp"
#include "search/eval/FeatureVector.hpp"
#include "core/position/Position.hpp"
#include "core/util/PositionUtil.hpp"

using namespace sunfish;

TEST(FeatureVectorTest, testGetEvalPieceIndex) {
  ASSERT_EQ(EvalPieceIndex::BPawn  , getEvalPieceIndex(PieceNumber::BPawn     ));
  ASSERT_EQ(EvalPieceIndex::BLance , getEvalPieceIndex(PieceNumber::BLance    ));
  ASSERT_EQ(EvalPieceIndex::BKnight, getEvalPieceIndex(PieceNumber::BKnight   ));
  ASSERT_EQ(EvalPieceIndex::BSilver, getEvalPieceIndex(PieceNumber::BSilver   ));
  ASSERT_EQ(EvalPieceIndex::BGold  , getEvalPieceIndex(PieceNumber::BGold     ));
  ASSERT_EQ(EvalPieceIndex::BBishop, getEvalPieceIndex(PieceNumber::BBishop   ));
  ASSERT_EQ(EvalPieceIndex::BRook  , getEvalPieceIndex(PieceNumber::BRook     ));
  ASSERT_EQ(EvalPieceIndex::BGold  , getEvalPieceIndex(PieceNumber::BTokin    ));
  ASSERT_EQ(EvalPieceIndex::BGold  , getEvalPieceIndex(PieceNumber::BProLance ));
  ASSERT_EQ(EvalPieceIndex::BGold  , getEvalPieceIndex(PieceNumber::BProKnight));
  ASSERT_EQ(EvalPieceIndex::BGold  , getEvalPieceIndex(PieceNumber::BProSilver));
  ASSERT_EQ(EvalPieceIndex::BHorse , getEvalPieceIndex(PieceNumber::BHorse    ));
  ASSERT_EQ(EvalPieceIndex::BDragon, getEvalPieceIndex(PieceNumber::BDragon   ));
  ASSERT_EQ(EvalPieceIndex::WPawn  , getEvalPieceIndex(PieceNumber::WPawn     ));
  ASSERT_EQ(EvalPieceIndex::WLance , getEvalPieceIndex(PieceNumber::WLance    ));
  ASSERT_EQ(EvalPieceIndex::WKnight, getEvalPieceIndex(PieceNumber::WKnight   ));
  ASSERT_EQ(EvalPieceIndex::WSilver, getEvalPieceIndex(PieceNumber::WSilver   ));
  ASSERT_EQ(EvalPieceIndex::WGold  , getEvalPieceIndex(PieceNumber::WGold     ));
  ASSERT_EQ(EvalPieceIndex::WBishop, getEvalPieceIndex(PieceNumber::WBishop   ));
  ASSERT_EQ(EvalPieceIndex::WRook  , getEvalPieceIndex(PieceNumber::WRook     ));
  ASSERT_EQ(EvalPieceIndex::WGold  , getEvalPieceIndex(PieceNumber::WTokin    ));
  ASSERT_EQ(EvalPieceIndex::WGold  , getEvalPieceIndex(PieceNumber::WProLance ));
  ASSERT_EQ(EvalPieceIndex::WGold  , getEvalPieceIndex(PieceNumber::WProKnight));
  ASSERT_EQ(EvalPieceIndex::WGold  , getEvalPieceIndex(PieceNumber::WProSilver));
  ASSERT_EQ(EvalPieceIndex::WHorse , getEvalPieceIndex(PieceNumber::WHorse    ));
  ASSERT_EQ(EvalPieceIndex::WDragon, getEvalPieceIndex(PieceNumber::WDragon   ));
}

TEST(FeatureVectorTest, testGetEvalPieceTypeIndex) {
  ASSERT_EQ(EvalPieceTypeIndex::Pawn  , getEvalPieceTypeIndex(PieceNumber::Pawn     ));
  ASSERT_EQ(EvalPieceTypeIndex::Lance , getEvalPieceTypeIndex(PieceNumber::Lance    ));
  ASSERT_EQ(EvalPieceTypeIndex::Knight, getEvalPieceTypeIndex(PieceNumber::Knight   ));
  ASSERT_EQ(EvalPieceTypeIndex::Silver, getEvalPieceTypeIndex(PieceNumber::Silver   ));
  ASSERT_EQ(EvalPieceTypeIndex::Gold  , getEvalPieceTypeIndex(PieceNumber::Gold     ));
  ASSERT_EQ(EvalPieceTypeIndex::Bishop, getEvalPieceTypeIndex(PieceNumber::Bishop   ));
  ASSERT_EQ(EvalPieceTypeIndex::Rook  , getEvalPieceTypeIndex(PieceNumber::Rook     ));
  ASSERT_EQ(EvalPieceTypeIndex::Gold  , getEvalPieceTypeIndex(PieceNumber::Tokin    ));
  ASSERT_EQ(EvalPieceTypeIndex::Gold  , getEvalPieceTypeIndex(PieceNumber::ProLance ));
  ASSERT_EQ(EvalPieceTypeIndex::Gold  , getEvalPieceTypeIndex(PieceNumber::ProKnight));
  ASSERT_EQ(EvalPieceTypeIndex::Gold  , getEvalPieceTypeIndex(PieceNumber::ProSilver));
  ASSERT_EQ(EvalPieceTypeIndex::Horse , getEvalPieceTypeIndex(PieceNumber::Horse    ));
  ASSERT_EQ(EvalPieceTypeIndex::Dragon, getEvalPieceTypeIndex(PieceNumber::Dragon   ));
}

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
