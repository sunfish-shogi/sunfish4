/* MovesTest.cpp
 *
 * Kubo Ryosuke
 */

#if !defined(NDEBUG)

#include "test/Test.hpp"
#include "core/move/Moves.hpp"

using namespace sunfish;

TEST(MovesTest, test) {
  const Move move0(Square::s57(), Square::s56(), false);
  const Move move1(Square::s28(), Square::s24(), false);
  const Move move2(Square::s77(), Square::s22(), false);
  const Move move3(Square::s19(), Square::s18(), false);

  auto createTestData = [=]() {
    Moves moves;
    moves.add(move0);
    moves.add(move1);
    moves.add(move2);
    moves.add(move3);
    return moves;
  };

  {
    Moves moves = createTestData();
    ASSERT_EQ(4, moves.size());
    ASSERT_EQ(move0, moves[0]);
    ASSERT_EQ(move1, moves[1]);
    ASSERT_EQ(move2, moves[2]);
    ASSERT_EQ(move3, moves[3]);
  }

  {
    Moves moves = createTestData();

    moves.remove(1);

    ASSERT_EQ(3, moves.size());
    ASSERT_EQ(move0, moves[0]);
    ASSERT_EQ(move3, moves[1]);
    ASSERT_EQ(move2, moves[2]);
  }

  {
    Moves moves = createTestData();

    auto ite = moves.remove(moves.end()-2);

    ASSERT_EQ(3, moves.size());
    ASSERT_EQ(move0, moves[0]);
    ASSERT_EQ(move1, moves[1]);
    ASSERT_EQ(move3, moves[2]);
    ASSERT_EQ(moves.end()-1, ite);
  }

  {
    Moves moves = createTestData();

    moves.removeAfter(2);

    ASSERT_EQ(2, moves.size());
    ASSERT_EQ(move0, moves[0]);
    ASSERT_EQ(move1, moves[1]);
  }

  {
    Moves moves = createTestData();

    moves.removeAfter(moves.begin()+1);

    ASSERT_EQ(1, moves.size());
    ASSERT_EQ(move0, moves[0]);
  }

  {
    Moves moves = createTestData();

    moves.removeStable(1);

    ASSERT_EQ(3, moves.size());
    ASSERT_EQ(move0, moves[0]);
    ASSERT_EQ(move2, moves[1]);
    ASSERT_EQ(move3, moves[2]);
  }

  {
    Moves moves = createTestData();

    auto ite = moves.removeStable(moves.begin()+1);

    ASSERT_EQ(3, moves.size());
    ASSERT_EQ(move0, moves[0]);
    ASSERT_EQ(move2, moves[1]);
    ASSERT_EQ(move3, moves[2]);
    ASSERT_EQ(moves.begin() + 1, ite);
  }
}

#endif // !defined(NDEBUG)
