/* MovesTest.cpp
 *
 * Kubo Ryosuke
 */

#if !defined(NDEBUG)

#include "test/Test.hpp"
#include "core/move/Moves.hpp"

using namespace sunfish;

TEST(MovesTest, test) {
  {
    Moves moves;
    moves.add(Move(Piece::blackPawn(), Square::s57(), Square::s56(), false));
    moves.add(Move(Piece::blackRook(), Square::s28(), Square::s24(), false));
    moves.add(Move(Piece::blackBishop(), Square::s77(), Square::s22(), false));
    moves.add(Move(Piece::blackLance(), Square::s19(), Square::s18(), false));

    ASSERT_EQ(4, moves.size());
    ASSERT_EQ(Piece::blackPawn(), moves[0].piece());
    ASSERT_EQ(Piece::blackRook(), moves[1].piece());
    ASSERT_EQ(Piece::blackBishop(), moves[2].piece());
    ASSERT_EQ(Piece::blackLance(), moves[3].piece());
  }

  {
    Moves moves;
    moves.add(Move(Piece::blackPawn(), Square::s57(), Square::s56(), false));
    moves.add(Move(Piece::blackRook(), Square::s28(), Square::s24(), false));
    moves.add(Move(Piece::blackBishop(), Square::s77(), Square::s22(), false));
    moves.add(Move(Piece::blackLance(), Square::s19(), Square::s18(), false));

    moves.remove(1);

    ASSERT_EQ(3, moves.size());
    ASSERT_EQ(Piece::blackPawn(), moves[0].piece());
    ASSERT_EQ(Piece::blackLance(), moves[1].piece());
    ASSERT_EQ(Piece::blackBishop(), moves[2].piece());
  }

  {
    Moves moves;
    moves.add(Move(Piece::blackPawn(), Square::s57(), Square::s56(), false));
    moves.add(Move(Piece::blackRook(), Square::s28(), Square::s24(), false));
    moves.add(Move(Piece::blackBishop(), Square::s77(), Square::s22(), false));
    moves.add(Move(Piece::blackLance(), Square::s19(), Square::s18(), false));

    moves.remove(moves.end()-1);

    ASSERT_EQ(3, moves.size());
    ASSERT_EQ(Piece::blackPawn(), moves[0].piece());
    ASSERT_EQ(Piece::blackRook(), moves[1].piece());
    ASSERT_EQ(Piece::blackBishop(), moves[2].piece());
  }

  {
    Moves moves;
    moves.add(Move(Piece::blackPawn(), Square::s57(), Square::s56(), false));
    moves.add(Move(Piece::blackRook(), Square::s28(), Square::s24(), false));
    moves.add(Move(Piece::blackBishop(), Square::s77(), Square::s22(), false));
    moves.add(Move(Piece::blackLance(), Square::s19(), Square::s18(), false));

    moves.removeAfter(2);

    ASSERT_EQ(2, moves.size());
    ASSERT_EQ(Piece::blackPawn(), moves[0].piece());
    ASSERT_EQ(Piece::blackRook(), moves[1].piece());
  }

  {
    Moves moves;
    moves.add(Move(Piece::blackPawn(), Square::s57(), Square::s56(), false));
    moves.add(Move(Piece::blackRook(), Square::s28(), Square::s24(), false));
    moves.add(Move(Piece::blackBishop(), Square::s77(), Square::s22(), false));
    moves.add(Move(Piece::blackLance(), Square::s19(), Square::s18(), false));

    moves.removeAfter(moves.begin()+1);

    ASSERT_EQ(1, moves.size());
    ASSERT_EQ(Piece::blackPawn(), moves[0].piece());
  }

  {
    Moves moves;
    moves.add(Move(Piece::blackPawn(), Square::s57(), Square::s56(), false));
    moves.add(Move(Piece::blackRook(), Square::s28(), Square::s24(), false));
    moves.add(Move(Piece::blackBishop(), Square::s77(), Square::s22(), false));
    moves.add(Move(Piece::blackLance(), Square::s19(), Square::s18(), false));

    moves.removeStable(1);

    ASSERT_EQ(3, moves.size());
    ASSERT_EQ(Piece::blackPawn(), moves[0].piece());
    ASSERT_EQ(Piece::blackBishop(), moves[1].piece());
    ASSERT_EQ(Piece::blackLance(), moves[2].piece());
  }

  {
    Moves moves;
    moves.add(Move(Piece::blackPawn(), Square::s57(), Square::s56(), false));
    moves.add(Move(Piece::blackRook(), Square::s28(), Square::s24(), false));
    moves.add(Move(Piece::blackBishop(), Square::s77(), Square::s22(), false));
    moves.add(Move(Piece::blackLance(), Square::s19(), Square::s18(), false));

    moves.removeStable(moves.begin()+1);

    ASSERT_EQ(3, moves.size());
    ASSERT_EQ(Piece::blackPawn(), moves[0].piece());
    ASSERT_EQ(Piece::blackBishop(), moves[1].piece());
    ASSERT_EQ(Piece::blackLance(), moves[2].piece());
  }
}

#endif // !defined(NDEBUG)
