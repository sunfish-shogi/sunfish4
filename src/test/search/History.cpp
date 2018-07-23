/* HistoryTest.cpp
 *
 * Kubo Ryosuke
 */

#include "test/Test.hpp"
#include "search/history/History.hpp"

using namespace sunfish;

TEST(HistoryTest, testFromToHistory) {
  FromToHistory fromToHistory;

  fromToHistory.update(Turn::Black, Square::s68(), Square::s46(), 1);
  fromToHistory.update(Turn::White, Square::s52(), Square::s58(), 2);
  fromToHistory.update(Turn::Black, Square::s15(), Square::s16(), 100);
  fromToHistory.update(Turn::White, Square::s15(), Square::s16(), 200);

  ASSERT_EQ(  32, fromToHistory.get(Turn::Black, Square::s68(), Square::s46()));
  ASSERT_EQ(  64, fromToHistory.get(Turn::White, Square::s52(), Square::s58()));
  ASSERT_EQ(   0, fromToHistory.get(Turn::Black, Square::s52(), Square::s58()));
  ASSERT_EQ(3200, fromToHistory.get(Turn::Black, Square::s15(), Square::s16()));
  ASSERT_EQ(6400, fromToHistory.get(Turn::White, Square::s15(), Square::s16()));

  fromToHistory.reduce();

  ASSERT_EQ(16, fromToHistory.get(Turn::Black, Square::s68(), Square::s46()));
  ASSERT_EQ(32, fromToHistory.get(Turn::White, Square::s52(), Square::s58()));
  ASSERT_EQ( 0, fromToHistory.get(Turn::Black, Square::s52(), Square::s58()));

  fromToHistory.clear();

  ASSERT_EQ(0, fromToHistory.get(Turn::Black, Square::s68(), Square::s46()));
  ASSERT_EQ(0, fromToHistory.get(Turn::White, Square::s52(), Square::s58()));
  ASSERT_EQ(0, fromToHistory.get(Turn::Black, Square::s52(), Square::s58()));
}

TEST(HistoryTest, testPieceToHistory) {
  PieceToHistory pieceToHistory;

  pieceToHistory.update(Turn::Black, PieceType::silver(), Square::s49(), 3);
  pieceToHistory.update(Turn::White, PieceType::bishop(), Square::s82(), 4);

  ASSERT_EQ( 96, pieceToHistory.get(Turn::Black, PieceType::silver(), Square::s49()));
  ASSERT_EQ(128, pieceToHistory.get(Turn::White, PieceType::bishop(), Square::s82()));
  ASSERT_EQ(  0, pieceToHistory.get(Turn::Black, PieceType::bishop(), Square::s82()));

  pieceToHistory.reduce();

  ASSERT_EQ(48, pieceToHistory.get(Turn::Black, PieceType::silver(), Square::s49()));
  ASSERT_EQ(64, pieceToHistory.get(Turn::White, PieceType::bishop(), Square::s82()));
  ASSERT_EQ( 0, pieceToHistory.get(Turn::Black, PieceType::bishop(), Square::s82()));

  pieceToHistory.clear();

  ASSERT_EQ(0, pieceToHistory.get(Turn::Black, PieceType::silver(), Square::s49()));
  ASSERT_EQ(0, pieceToHistory.get(Turn::White, PieceType::bishop(), Square::s82()));
  ASSERT_EQ(0, pieceToHistory.get(Turn::Black, PieceType::bishop(), Square::s82()));
}
