/* MoveTest.cpp
 *
 * Kubo Ryosuke
 */

#if !defined(NDEBUG)

#include "test/Test.hpp"
#include "core/move/Move.hpp"
#include "core/record/CsaReader.hpp"

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
  std::string src = "\
P1 *  *  *  * -OU *  *  *  * \n\
P2 *  *  *  *  *  *  *  *  * \n\
P3 *  *  *  *  *  * +KE *  * \n\
P4 *  *  *  * +FU *  *  *  * \n\
P5 *  * -FU *  * +HI *  *  * \n\
P6 *  *  *  *  *  *  *  *  * \n\
P7 *  * +FU *  *  *  *  *  * \n\
P8 *  *  *  *  *  *  *  *  * \n\
P9 *  *  *  * +OU *  *  *  * \n\
P+\n\
P-\n\
+\n\
";
  std::istringstream iss(src);
  Position pos;
  CsaReader::readPosition(iss, pos);

  {
    Move in(Piece::blackPawn(), Square::s77(), Square::s76(), false);
    uint16_t data = Move::serialize16(in);
    Move out = Move::deserialize16(data, pos);
    ASSERT_EQ(in, out);
  }

  {
    Move in(Piece::blackKnight(), Square::s33(), Square::s41(), true);
    uint16_t data = Move::serialize16(in);
    Move out = Move::deserialize16(data, pos);
    ASSERT_EQ(in, out);
  }

  {
    Move in(Piece::blackRook(), Square::s45(), Square::s75(), false);
    in.setCapturedPiece(Piece::whitePawn());
    uint16_t data = Move::serialize16(in);
    Move out = Move::deserialize16(data, pos);
    ASSERT_EQ(in, out);
  }

  {
    Move in(Piece::blackPawn(), Square::s54());
    uint16_t data = Move::serialize16(in);
    Move out = Move::deserialize16(data, pos);
    ASSERT_EQ(in, out);
  }

  {
    Move in = Move::empty();
    uint16_t data = Move::serialize16(in);
    Move out = Move::deserialize16(data, pos);
    ASSERT_EQ(in, out);
  }
}

#endif // !defined(NDEBUG)
