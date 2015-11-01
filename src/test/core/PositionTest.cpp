/* PositionTest.cpp
 *
 * Kubo Ryosuke
 */

#if !defined(NDEBUG)

#include "test/Test.hpp"
#include "core/position/Position.hpp"

using namespace sunfish;

TEST(PositionTest, testInitialization) {
  {
    Position pos;

    ASSERT_EQ(0x00, pos.getBPawnBitboard().first());
    ASSERT_EQ(0x00, pos.getBPawnBitboard().second());
    ASSERT_EQ(0x00, pos.getWPawnBitboard().first());
    ASSERT_EQ(0x00, pos.getWPawnBitboard().second());

    ASSERT_EQ(0x00, pos.get90RotatedBitboard().raw());
    ASSERT_EQ(0x00, pos.getRight45RotatedBitboard().raw());
    ASSERT_EQ(0x00, pos.getLeft45RotatedBitboard().raw());

    ASSERT_EQ(Piece::Empty, pos.getPieceOnBoard(Square::S11));
    ASSERT_EQ(Piece::Empty, pos.getPieceOnBoard(Square::S67));

    ASSERT_EQ(0, pos.getBlackHandPieceCount(Piece::Pawn));
    ASSERT_EQ(0, pos.getBlackHandPieceCount(Piece::Bishop));

    ASSERT_EQ(true, pos.isBlackTurn());
    ASSERT_EQ(false, pos.isWhiteTurn());
  }

  {
    Position pos(Position::Handicap::Even);

    ASSERT_EQ(0x0000040201008040, pos.getBPawnBitboard().first());
    ASSERT_EQ(0x0000000201008040, pos.getBPawnBitboard().second());
    ASSERT_EQ(0x0000004020100804, pos.getWPawnBitboard().first());
    ASSERT_EQ(0x0000000020100804, pos.getWPawnBitboard().second());

    ASSERT_EQ(0xff07f800003fc1ff, pos.get90RotatedBitboard().raw());
    ASSERT_EQ(0x0003221458d14227, pos.getRight45RotatedBitboard().raw());
    ASSERT_EQ(0x0003221458d14227, pos.getLeft45RotatedBitboard().raw());

    ASSERT_EQ(Piece::WLance, pos.getPieceOnBoard(Square::S11));
    ASSERT_EQ(Piece::WBishop, pos.getPieceOnBoard(Square::S22));
    ASSERT_EQ(Piece::WRook, pos.getPieceOnBoard(Square::S82));
    ASSERT_EQ(Piece::BPawn, pos.getPieceOnBoard(Square::S67));
    ASSERT_EQ(Piece::BBishop, pos.getPieceOnBoard(Square::S88));
    ASSERT_EQ(Piece::BRook, pos.getPieceOnBoard(Square::S28));

    ASSERT_EQ(0, pos.getBlackHandPieceCount(Piece::Pawn));
    ASSERT_EQ(0, pos.getBlackHandPieceCount(Piece::Bishop));

    ASSERT_EQ(true, pos.isBlackTurn());
    ASSERT_EQ(false, pos.isWhiteTurn());
  }

  {
    Position pos(Position::Handicap::TwoPieces);

    ASSERT_EQ(0x0000040201008040, pos.getBPawnBitboard().first());
    ASSERT_EQ(0x0000000201008040, pos.getBPawnBitboard().second());
    ASSERT_EQ(0x0000004020100804, pos.getWPawnBitboard().first());
    ASSERT_EQ(0x0000000020100804, pos.getWPawnBitboard().second());

    ASSERT_EQ(0xff07f800003f80ff, pos.get90RotatedBitboard().raw());
    ASSERT_EQ(0x0003221448d14225, pos.getRight45RotatedBitboard().raw());
    ASSERT_EQ(0x0001221458914227, pos.getLeft45RotatedBitboard().raw());

    ASSERT_EQ(Piece::WLance, pos.getPieceOnBoard(Square::S11));
    ASSERT_EQ(Piece::Empty, pos.getPieceOnBoard(Square::S22));
    ASSERT_EQ(Piece::Empty, pos.getPieceOnBoard(Square::S82));
    ASSERT_EQ(Piece::BPawn, pos.getPieceOnBoard(Square::S67));
    ASSERT_EQ(Piece::BBishop, pos.getPieceOnBoard(Square::S88));
    ASSERT_EQ(Piece::BRook, pos.getPieceOnBoard(Square::S28));

    ASSERT_EQ(0, pos.getBlackHandPieceCount(Piece::Pawn));
    ASSERT_EQ(0, pos.getBlackHandPieceCount(Piece::Bishop));

    ASSERT_EQ(false, pos.isBlackTurn());
    ASSERT_EQ(true, pos.isWhiteTurn());
  }
}

#endif // !defined(NDEBUG)
