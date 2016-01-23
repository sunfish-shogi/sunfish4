/* HistoryTest.cpp
 *
 * Kubo Ryosuke
 */

#include "test/Test.hpp"
#include "search/history/History.hpp"

using namespace sunfish;

TEST(HistoryTest, test) {
  {
    History hist;
    Move move(Square::s57(), Square::s55(), false);

    hist.add(Turn::Black, move, 8, 0);
    hist.add(Turn::Black, move, 4, 4);

    auto data = hist.get(Turn::Black, move);
    ASSERT_EQ(12, data.appear);
    ASSERT_EQ( 4, data.good);

    ASSERT_EQ(0xb6d, hist.ratio(Turn::Black, move));
  }

  {
    History hist;
    Move move(PieceType::silver(), Square::s74());

    hist.add(Turn::Black, move, 0x1b85, 0x11c2);
    hist.reduce();

    auto data = hist.get(Turn::Black, move);
    ASSERT_EQ(0x1b, data.appear);
    ASSERT_EQ(0x11, data.good);
  }

  {
    History hist;
    Move move(PieceType::silver(), Square::s74());

    hist.add(Turn::Black, move, 0x4cb09, 0x1b830);
    hist.add(Turn::Black, move, 0x3d012, 0x0e02a);

    auto data = hist.get(Turn::Black, move);
    ASSERT_EQ(0x63596, data.appear);
    ASSERT_EQ(0x1bc42, data.good);
  }
}
