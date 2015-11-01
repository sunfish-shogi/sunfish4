/* MoveTest.cpp
 *
 * Kubo Ryosuke
 */

#if !defined(NDEBUG)

#include "test/Test.hpp"
#include "core/move/Move.hpp"

using namespace sunfish;

TEST(MoveTest, test) {
  {
    Move move(Piece::blackPawn(), Square::s57(), Square::s56(), false);
    ASSERT_EQ(Square::s57(), move.from().raw());
    ASSERT_EQ(Square::s56(), move.to().raw());
    ASSERT_EQ(false, move.isPromotion());
    ASSERT_EQ(Piece::blackPawn(), move.piece());
    ASSERT_EQ(false, move.isDrop());
  }

  {
    Move move(Piece::whitePawn(), Square::s53(), Square::s54(), false);
    ASSERT_EQ(Square::s53(), move.from().raw());
    ASSERT_EQ(Square::s54(), move.to().raw());
    ASSERT_EQ(false, move.isPromotion());
    ASSERT_EQ(Piece::whitePawn(), move.piece());
    ASSERT_EQ(false, move.isDrop());
  }

  {
    Move move(Piece::blackPawn(), Square::s54(), Square::s53(), true);
    ASSERT_EQ(Square::s54(), move.from().raw());
    ASSERT_EQ(Square::s53(), move.to().raw());
    ASSERT_EQ(true, move.isPromotion());
    ASSERT_EQ(Piece::blackPawn(), move.piece());
    ASSERT_EQ(false, move.isDrop());
  }

  {
    Move move(Piece::blackPawn(), Square::s55());
    ASSERT_EQ(Square::s55(), move.to().raw());
    ASSERT_EQ(false, move.isPromotion());
    ASSERT_EQ(Piece::blackPawn(), move.piece());
    ASSERT_EQ(true, move.isDrop());
  }
}

TEST(MoveTest, testSerialization) {
  {
    Move in(Piece::blackPawn(), Square::s77(), Square::s76(), false);
    uint32_t data = Move::serialize(in);
    Move out = Move::deserialize(data);
    ASSERT_EQ(in, out);
  }

  {
    Move in(Piece::whiteKnight(), Square::s33(), Square::s41(), true);
    uint32_t data = Move::serialize(in);
    Move out = Move::deserialize(data);
    ASSERT_EQ(in, out);
  }

  {
    Move in(Piece::blackRook(), Square::s45(), Square::s75(), false);
    in.setCapturedPiece(Piece::whitePawn());
    uint32_t data = Move::serialize(in);
    Move out = Move::deserialize(data);
    ASSERT_EQ(in, out);
  }

  {
    Move in(Piece::whitePawn(), Square::s54());
    uint32_t data = Move::serialize(in);
    Move out = Move::deserialize(data);
    ASSERT_EQ(in, out);
  }
}

TEST(MoveTest, testSerialization16) {
  // TODO
}

#endif // !defined(NDEBUG)
