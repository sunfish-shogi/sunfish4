/* PositionTest.cpp
 *
 * Kubo Ryosuke
 */

#if !defined(NDEBUG)

#include "test/Test.hpp"
#include "core/position/Position.hpp"
#include "core/util/PositionUtil.hpp"

using namespace sunfish;

namespace {

void assertEq(const Position& expect, const Position& exact) {
  ASSERT_EQ(expect.getBPawnBitboard(), exact.getBPawnBitboard());
  ASSERT_EQ(expect.getBLanceBitboard(), exact.getBLanceBitboard());
  ASSERT_EQ(expect.getBKnightBitboard(), exact.getBKnightBitboard());
  ASSERT_EQ(expect.getBSilverBitboard(), exact.getBSilverBitboard());
  ASSERT_EQ(expect.getBGoldBitboard(), exact.getBGoldBitboard());
  ASSERT_EQ(expect.getBBishopBitboard(), exact.getBBishopBitboard());
  ASSERT_EQ(expect.getBRookBitboard(), exact.getBRookBitboard());
  ASSERT_EQ(expect.getBHorseBitboard(), exact.getBHorseBitboard());
  ASSERT_EQ(expect.getBDragonBitboard(), exact.getBDragonBitboard());

  ASSERT_EQ(expect.getWPawnBitboard(), exact.getWPawnBitboard());
  ASSERT_EQ(expect.getWLanceBitboard(), exact.getWLanceBitboard());
  ASSERT_EQ(expect.getWKnightBitboard(), exact.getWKnightBitboard());
  ASSERT_EQ(expect.getWSilverBitboard(), exact.getWSilverBitboard());
  ASSERT_EQ(expect.getWGoldBitboard(), exact.getWGoldBitboard());
  ASSERT_EQ(expect.getWBishopBitboard(), exact.getWBishopBitboard());
  ASSERT_EQ(expect.getWRookBitboard(), exact.getWRookBitboard());
  ASSERT_EQ(expect.getWHorseBitboard(), exact.getWHorseBitboard());
  ASSERT_EQ(expect.getWDragonBitboard(), exact.getWDragonBitboard());

  ASSERT_EQ(expect.getBOccupiedBitboard(), exact.getBOccupiedBitboard());
  ASSERT_EQ(expect.getWOccupiedBitboard(), exact.getWOccupiedBitboard());

  ASSERT_EQ(expect.get90RotatedBitboard(), exact.get90RotatedBitboard());
  ASSERT_EQ(expect.getRight45RotatedBitboard().raw() >> 1, exact.getRight45RotatedBitboard().raw() >> 1);
  ASSERT_EQ(expect.getLeft45RotatedBitboard().raw() >> 1, exact.getLeft45RotatedBitboard().raw() >> 1);

  ASSERT_EQ(expect.getBlackKingSquare(), exact.getBlackKingSquare());
  ASSERT_EQ(expect.getWhiteKingSquare(), exact.getWhiteKingSquare());

  SQUARE_EACH(square) {
    ASSERT_EQ(expect.getPieceOnBoard(square), exact.getPieceOnBoard(square));
  }

  HAND_EACH(pieceType) {
    ASSERT_EQ(expect.getBlackHandPieceCount(pieceType), exact.getBlackHandPieceCount(pieceType));
    ASSERT_EQ(expect.getWhiteHandPieceCount(pieceType), exact.getWhiteHandPieceCount(pieceType));
  }

  ASSERT_EQ(expect.getTurn(), exact.getTurn());

  ASSERT_EQ(expect.getBoardHash(), exact.getBoardHash());
  ASSERT_EQ(expect.getHandHash(), exact.getHandHash());
}

}

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

    ASSERT_EQ(Piece::empty(), pos.getPieceOnBoard(Square::s11()));
    ASSERT_EQ(Piece::empty(), pos.getPieceOnBoard(Square::s67()));

    ASSERT_EQ(0, pos.getBlackHandPieceCount(PieceType::pawn()));
    ASSERT_EQ(0, pos.getBlackHandPieceCount(PieceType::bishop()));

    ASSERT_EQ(Turn::Black, pos.getTurn());
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

    ASSERT_EQ(Piece::whiteLance(), pos.getPieceOnBoard(Square::s11()));
    ASSERT_EQ(Piece::whiteBishop(), pos.getPieceOnBoard(Square::s22()));
    ASSERT_EQ(Piece::whiteRook(), pos.getPieceOnBoard(Square::s82()));
    ASSERT_EQ(Piece::blackPawn(), pos.getPieceOnBoard(Square::s67()));
    ASSERT_EQ(Piece::blackBishop(), pos.getPieceOnBoard(Square::s88()));
    ASSERT_EQ(Piece::blackRook(), pos.getPieceOnBoard(Square::s28()));

    ASSERT_EQ(0, pos.getBlackHandPieceCount(PieceType::pawn()));
    ASSERT_EQ(0, pos.getBlackHandPieceCount(PieceType::bishop()));

    ASSERT_EQ(Turn::Black, pos.getTurn());
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

    ASSERT_EQ(Piece::whiteLance(), pos.getPieceOnBoard(Square::s11()));
    ASSERT_EQ(Piece::empty(), pos.getPieceOnBoard(Square::s22()));
    ASSERT_EQ(Piece::empty(), pos.getPieceOnBoard(Square::s82()));
    ASSERT_EQ(Piece::blackPawn(), pos.getPieceOnBoard(Square::s67()));
    ASSERT_EQ(Piece::blackBishop(), pos.getPieceOnBoard(Square::s88()));
    ASSERT_EQ(Piece::blackRook(), pos.getPieceOnBoard(Square::s28()));

    ASSERT_EQ(0, pos.getBlackHandPieceCount(PieceType::pawn()));
    ASSERT_EQ(0, pos.getBlackHandPieceCount(PieceType::bishop()));

    ASSERT_EQ(Turn::White, pos.getTurn());
  }
}

TEST(PositionTest, testMutablePosition) {
  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1-KY-KE-GI-KI-OU-KI-GI-KE-KY\n"
      "P2 * -HI *  *  *  *  * -KA * \n"
      "P3-FU-FU-FU-FU-FU-FU-FU-FU-FU\n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7+FU+FU+FU+FU+FU+FU+FU+FU+FU\n"
      "P8 * +KA *  *  *  *  * +HI * \n"
      "P9+KY+KE+GI+KI+OU+KI+GI+KE+KY\n"
      "P+\n"
      "P-\n"
      "+\n");
    MutablePosition mp = pos.getMutablePosition();
    ASSERT_EQ(
      "P1-KY-KE-GI-KI-OU-KI-GI-KE-KY\n"
      "P2 * -HI *  *  *  *  * -KA * \n"
      "P3-FU-FU-FU-FU-FU-FU-FU-FU-FU\n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7+FU+FU+FU+FU+FU+FU+FU+FU+FU\n"
      "P8 * +KA *  *  *  *  * +HI * \n"
      "P9+KY+KE+GI+KI+OU+KI+GI+KE+KY\n"
      "P+\n"
      "P-\n"
      "+\n", Position(mp).toString());
  }
}

TEST(PositionTest, testToString) {
  Position pos(Position::Handicap::Even);

  ASSERT_EQ(
    "P1-KY-KE-GI-KI-OU-KI-GI-KE-KY\n"
    "P2 * -HI *  *  *  *  * -KA * \n"
    "P3-FU-FU-FU-FU-FU-FU-FU-FU-FU\n"
    "P4 *  *  *  *  *  *  *  *  * \n"
    "P5 *  *  *  *  *  *  *  *  * \n"
    "P6 *  *  *  *  *  *  *  *  * \n"
    "P7+FU+FU+FU+FU+FU+FU+FU+FU+FU\n"
    "P8 * +KA *  *  *  *  * +HI * \n"
    "P9+KY+KE+GI+KI+OU+KI+GI+KE+KY\n"
    "P+\n"
    "P-\n"
    "+\n", pos.toString());
}

TEST(PositionTest, testMakeMove) {
  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1-KY-KE-GI-KI-OU-KI-GI-KE-KY\n"
      "P2 * -HI *  *  *  *  * -KA * \n"
      "P3-FU-FU-FU-FU-FU-FU-FU-FU-FU\n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7+FU+FU+FU+FU+FU+FU+FU+FU+FU\n"
      "P8 * +KA *  *  *  *  * +HI * \n"
      "P9+KY+KE+GI+KI+OU+KI+GI+KE+KY\n"
      "P+\n"
      "P-\n"
      "+\n");

    Position expectPos = PositionUtil::createPositionFromCsaString(
      "P1-KY-KE-GI-KI-OU-KI-GI-KE-KY\n"
      "P2 * -HI *  *  *  *  * -KA * \n"
      "P3-FU-FU-FU-FU-FU-FU-FU-FU-FU\n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  * +FU *  *  *  *  *  * \n"
      "P7+FU+FU * +FU+FU+FU+FU+FU+FU\n"
      "P8 * +KA *  *  *  *  * +HI * \n"
      "P9+KY+KE+GI+KI+OU+KI+GI+KE+KY\n"
      "P+\n"
      "P-\n"
      "-\n");

    Move move(Piece::blackPawn(), Square::s77(), Square::s76(), false);

    pos.doMove(move);

    ASSERT_EQ(Piece::empty(), move.capturedPiece());

    assertEq(expectPos, pos);
  }

  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1-KY-KE-GI-KI-OU-KI-GI-KE-KY\n"
      "P2 * -HI *  *  *  *  * -KA * \n"
      "P3-FU-FU-FU-FU-FU-FU-FU-FU-FU\n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  * +FU *  *  *  *  *  * \n"
      "P7+FU+FU * +FU+FU+FU+FU+FU+FU\n"
      "P8 * +KA *  *  *  *  * +HI * \n"
      "P9+KY+KE+GI+KI+OU+KI+GI+KE+KY\n"
      "P+\n"
      "P-\n"
      "-\n");

    Position expectPos = PositionUtil::createPositionFromCsaString(
      "P1-KY-KE-GI-KI-OU-KI-GI-KE-KY\n"
      "P2 * -HI *  *  *  *  * -KA * \n"
      "P3-FU-FU-FU-FU-FU-FU * -FU-FU\n"
      "P4 *  *  *  *  *  * -FU *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  * +FU *  *  *  *  *  * \n"
      "P7+FU+FU * +FU+FU+FU+FU+FU+FU\n"
      "P8 * +KA *  *  *  *  * +HI * \n"
      "P9+KY+KE+GI+KI+OU+KI+GI+KE+KY\n"
      "P+\n"
      "P-\n"
      "+\n");

    Move move(Piece::whitePawn(), Square::s33(), Square::s34(), false);

    pos.doMove(move);

    ASSERT_EQ(Piece::empty(), move.capturedPiece());

    assertEq(expectPos, pos);
  }

  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1-KY-KE-GI-KI-OU-KI-GI-KE-KY\n"
      "P2 * -HI *  *  *  *  * -KA * \n"
      "P3-FU-FU-FU-FU-FU-FU * -FU-FU\n"
      "P4 *  *  *  *  *  * -FU *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  * +FU *  *  *  *  *  * \n"
      "P7+FU+FU * +FU+FU+FU+FU+FU+FU\n"
      "P8 * +KA *  *  *  *  * +HI * \n"
      "P9+KY+KE+GI+KI+OU+KI+GI+KE+KY\n"
      "P+\n"
      "P-\n"
      "+\n");

    Position expectPos = PositionUtil::createPositionFromCsaString(
      "P1-KY-KE-GI-KI-OU-KI-GI-KE-KY\n"
      "P2 * -HI *  *  *  *  * +UM * \n"
      "P3-FU-FU-FU-FU-FU-FU * -FU-FU\n"
      "P4 *  *  *  *  *  * -FU *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  * +FU *  *  *  *  *  * \n"
      "P7+FU+FU * +FU+FU+FU+FU+FU+FU\n"
      "P8 *  *  *  *  *  *  * +HI * \n"
      "P9+KY+KE+GI+KI+OU+KI+GI+KE+KY\n"
      "P+00KA\n"
      "P-\n"
      "-\n");

    Move move(Piece::blackBishop(), Square::s88(), Square::s22(), true);

    pos.doMove(move);

    ASSERT_EQ(Piece::whiteBishop(), move.capturedPiece());

    assertEq(expectPos, pos);
  }

  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1-KY-KE-GI-KI-OU-KI-GI-KE-KY\n"
      "P2 * -HI *  *  *  *  * +UM * \n"
      "P3-FU-FU-FU-FU-FU-FU * -FU-FU\n"
      "P4 *  *  *  *  *  * -FU *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  * +FU *  *  *  *  *  * \n"
      "P7+FU+FU * +FU+FU+FU+FU+FU+FU\n"
      "P8 *  *  *  *  *  *  * +HI * \n"
      "P9+KY+KE+GI+KI+OU+KI+GI+KE+KY\n"
      "P+00KA\n"
      "P-\n"
      "-\n");

    Position expectPos = PositionUtil::createPositionFromCsaString(
      "P1-KY-KE-GI-KI-OU-KI * -KE-KY\n"
      "P2 * -HI *  *  *  *  * -GI * \n"
      "P3-FU-FU-FU-FU-FU-FU * -FU-FU\n"
      "P4 *  *  *  *  *  * -FU *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  * +FU *  *  *  *  *  * \n"
      "P7+FU+FU * +FU+FU+FU+FU+FU+FU\n"
      "P8 *  *  *  *  *  *  * +HI * \n"
      "P9+KY+KE+GI+KI+OU+KI+GI+KE+KY\n"
      "P+00KA\n"
      "P-00KA\n"
      "+\n");

    Move move(Piece::whiteSilver(), Square::s31(), Square::s22(), false);

    pos.doMove(move);

    ASSERT_EQ(Piece::blackHorse(), move.capturedPiece());

    assertEq(expectPos, pos);
  }

  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1-KY-KE-GI-KI-OU-KI * -KE-KY\n"
      "P2 * -HI *  *  *  *  * -GI * \n"
      "P3-FU-FU-FU-FU-FU-FU * -FU-FU\n"
      "P4 *  *  *  *  *  * -FU *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  * +FU *  *  *  *  *  * \n"
      "P7+FU+FU * +FU+FU+FU+FU+FU+FU\n"
      "P8 *  *  *  *  *  *  * +HI * \n"
      "P9+KY+KE+GI+KI+OU+KI+GI+KE+KY\n"
      "P+00KA\n"
      "P-00KA\n"
      "+\n");

    Position expectPos = PositionUtil::createPositionFromCsaString(
      "P1-KY-KE-GI-KI-OU-KI * -KE-KY\n"
      "P2 * -HI *  *  *  *  * -GI * \n"
      "P3-FU-FU-FU-FU-FU-FU * -FU-FU\n"
      "P4 *  *  *  *  *  * -FU *  * \n"
      "P5 *  *  *  *  * +KA *  *  * \n"
      "P6 *  * +FU *  *  *  *  *  * \n"
      "P7+FU+FU * +FU+FU+FU+FU+FU+FU\n"
      "P8 *  *  *  *  *  *  * +HI * \n"
      "P9+KY+KE+GI+KI+OU+KI+GI+KE+KY\n"
      "P+\n"
      "P-00KA\n"
      "-\n");

    Move move(Piece::blackBishop(), Square::s45());

    pos.doMove(move);

    ASSERT_EQ(Piece::empty(), move.capturedPiece());

    assertEq(expectPos, pos);
  }
}

TEST(PositionTest, testUndoMove) {
  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1-KY-KE-GI-KI-OU-KI-GI-KE-KY\n"
      "P2 * -HI *  *  *  *  * -KA * \n"
      "P3-FU-FU-FU-FU-FU-FU-FU-FU-FU\n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  * +FU *  *  *  *  *  * \n"
      "P7+FU+FU * +FU+FU+FU+FU+FU+FU\n"
      "P8 * +KA *  *  *  *  * +HI * \n"
      "P9+KY+KE+GI+KI+OU+KI+GI+KE+KY\n"
      "P+\n"
      "P-\n"
      "-\n");

    Position expectPos = PositionUtil::createPositionFromCsaString(
      "P1-KY-KE-GI-KI-OU-KI-GI-KE-KY\n"
      "P2 * -HI *  *  *  *  * -KA * \n"
      "P3-FU-FU-FU-FU-FU-FU-FU-FU-FU\n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7+FU+FU+FU+FU+FU+FU+FU+FU+FU\n"
      "P8 * +KA *  *  *  *  * +HI * \n"
      "P9+KY+KE+GI+KI+OU+KI+GI+KE+KY\n"
      "P+\n"
      "P-\n"
      "+\n");

    Move move(Piece::blackPawn(), Square::s77(), Square::s76(), false);

    pos.undoMove(move);

    ASSERT_EQ(Piece::empty(), move.capturedPiece());

    assertEq(expectPos, pos);
  }

  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1-KY-KE-GI-KI-OU-KI-GI-KE-KY\n"
      "P2 * -HI *  *  *  *  * -KA * \n"
      "P3-FU-FU-FU-FU-FU-FU * -FU-FU\n"
      "P4 *  *  *  *  *  * -FU *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  * +FU *  *  *  *  *  * \n"
      "P7+FU+FU * +FU+FU+FU+FU+FU+FU\n"
      "P8 * +KA *  *  *  *  * +HI * \n"
      "P9+KY+KE+GI+KI+OU+KI+GI+KE+KY\n"
      "P+\n"
      "P-\n"
      "+\n");

    Position expectPos = PositionUtil::createPositionFromCsaString(
      "P1-KY-KE-GI-KI-OU-KI-GI-KE-KY\n"
      "P2 * -HI *  *  *  *  * -KA * \n"
      "P3-FU-FU-FU-FU-FU-FU-FU-FU-FU\n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  * +FU *  *  *  *  *  * \n"
      "P7+FU+FU * +FU+FU+FU+FU+FU+FU\n"
      "P8 * +KA *  *  *  *  * +HI * \n"
      "P9+KY+KE+GI+KI+OU+KI+GI+KE+KY\n"
      "P+\n"
      "P-\n"
      "-\n");

    Move move(Piece::whitePawn(), Square::s33(), Square::s34(), false);

    pos.undoMove(move);

    ASSERT_EQ(Piece::empty(), move.capturedPiece());

    assertEq(expectPos, pos);
  }

  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1-KY-KE-GI-KI-OU-KI-GI-KE-KY\n"
      "P2 * -HI *  *  *  *  * +UM * \n"
      "P3-FU-FU-FU-FU-FU-FU * -FU-FU\n"
      "P4 *  *  *  *  *  * -FU *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  * +FU *  *  *  *  *  * \n"
      "P7+FU+FU * +FU+FU+FU+FU+FU+FU\n"
      "P8 *  *  *  *  *  *  * +HI * \n"
      "P9+KY+KE+GI+KI+OU+KI+GI+KE+KY\n"
      "P+00KA\n"
      "P-\n"
      "-\n");

    Position expectPos = PositionUtil::createPositionFromCsaString(
      "P1-KY-KE-GI-KI-OU-KI-GI-KE-KY\n"
      "P2 * -HI *  *  *  *  * -KA * \n"
      "P3-FU-FU-FU-FU-FU-FU * -FU-FU\n"
      "P4 *  *  *  *  *  * -FU *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  * +FU *  *  *  *  *  * \n"
      "P7+FU+FU * +FU+FU+FU+FU+FU+FU\n"
      "P8 * +KA *  *  *  *  * +HI * \n"
      "P9+KY+KE+GI+KI+OU+KI+GI+KE+KY\n"
      "P+\n"
      "P-\n"
      "+\n");

    Move move(Piece::blackBishop(), Square::s88(), Square::s22(), true);
    move.setCapturedPiece(Piece::whiteBishop());

    pos.undoMove(move);

    ASSERT_EQ(Piece::whiteBishop(), move.capturedPiece());

    assertEq(expectPos, pos);
  }

  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1-KY-KE-GI-KI-OU-KI * -KE-KY\n"
      "P2 * -HI *  *  *  *  * -GI * \n"
      "P3-FU-FU-FU-FU-FU-FU * -FU-FU\n"
      "P4 *  *  *  *  *  * -FU *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  * +FU *  *  *  *  *  * \n"
      "P7+FU+FU * +FU+FU+FU+FU+FU+FU\n"
      "P8 *  *  *  *  *  *  * +HI * \n"
      "P9+KY+KE+GI+KI+OU+KI+GI+KE+KY\n"
      "P+00KA\n"
      "P-00KA\n"
      "+\n");

    Position expectPos = PositionUtil::createPositionFromCsaString(
      "P1-KY-KE-GI-KI-OU-KI-GI-KE-KY\n"
      "P2 * -HI *  *  *  *  * +UM * \n"
      "P3-FU-FU-FU-FU-FU-FU * -FU-FU\n"
      "P4 *  *  *  *  *  * -FU *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  * +FU *  *  *  *  *  * \n"
      "P7+FU+FU * +FU+FU+FU+FU+FU+FU\n"
      "P8 *  *  *  *  *  *  * +HI * \n"
      "P9+KY+KE+GI+KI+OU+KI+GI+KE+KY\n"
      "P+00KA\n"
      "P-\n"
      "-\n");

    Move move(Piece::whiteSilver(), Square::s31(), Square::s22(), false);
    move.setCapturedPiece(Piece::blackHorse());

    pos.undoMove(move);

    ASSERT_EQ(Piece::blackHorse(), move.capturedPiece());

    assertEq(expectPos, pos);
  }

  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1-KY-KE-GI-KI-OU-KI * -KE-KY\n"
      "P2 * -HI *  *  *  *  * -GI * \n"
      "P3-FU-FU-FU-FU-FU-FU * -FU-FU\n"
      "P4 *  *  *  *  *  * -FU *  * \n"
      "P5 *  *  *  *  * +KA *  *  * \n"
      "P6 *  * +FU *  *  *  *  *  * \n"
      "P7+FU+FU * +FU+FU+FU+FU+FU+FU\n"
      "P8 *  *  *  *  *  *  * +HI * \n"
      "P9+KY+KE+GI+KI+OU+KI+GI+KE+KY\n"
      "P+\n"
      "P-00KA\n"
      "-\n");

    Position expectPos = PositionUtil::createPositionFromCsaString(
      "P1-KY-KE-GI-KI-OU-KI * -KE-KY\n"
      "P2 * -HI *  *  *  *  * -GI * \n"
      "P3-FU-FU-FU-FU-FU-FU * -FU-FU\n"
      "P4 *  *  *  *  *  * -FU *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  * +FU *  *  *  *  *  * \n"
      "P7+FU+FU * +FU+FU+FU+FU+FU+FU\n"
      "P8 *  *  *  *  *  *  * +HI * \n"
      "P9+KY+KE+GI+KI+OU+KI+GI+KE+KY\n"
      "P+00KA\n"
      "P-00KA\n"
      "+\n");

    Move move(Piece::blackBishop(), Square::s45());

    pos.undoMove(move);

    ASSERT_EQ(Piece::empty(), move.capturedPiece());

    assertEq(expectPos, pos);
  }
}

TEST(PositionTest, testIsChecking) {
  {
    // checked by pawn
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  * +UM *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  * -FU *  *  *  *  * \n"
      "P7 *  *  * +OU *  *  *  *  * \n"
      "P8 *  *  * +FU *  *  *  *  * \n"
      "P9 *  *  *  *  *  *  *  *  * \n"
      "P+\n"
      "P-\n"
      "+\n");

    ASSERT_EQ(true, pos.isChecking());
  }

  {
    // not checked
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  * -KE * +UM *  * \n"
      "P5 *  * -GI-FU *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  * +OU *  *  *  *  * \n"
      "P8 *  *  * +FU *  *  *  *  * \n"
      "P9 *  *  *  *  *  *  *  *  * \n"
      "P+\n"
      "P-\n"
      "+\n");

    ASSERT_EQ(false, pos.isChecking());
  }

  {
    // checked by knight
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  * +UM *  * \n"
      "P5 *  *  * -FU-KE *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  * +OU *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  *  *  *  *  *  * \n"
      "P+\n"
      "P-\n"
      "+\n");

    ASSERT_EQ(true, pos.isChecking());
  }

  {
    // checked by silver
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  * +UM *  * \n"
      "P5 *  *  * -FU *  *  *  *  * \n"
      "P6 *  * -GI *  *  *  *  *  * \n"
      "P7 *  *  * +OU *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  *  *  *  *  *  * \n"
      "P+\n"
      "P-\n"
      "+\n");

    ASSERT_EQ(true, pos.isChecking());
  }

  {
    // checked by silver
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  * +UM *  * \n"
      "P5 *  *  * -FU *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  * +OU *  *  *  *  * \n"
      "P8 *  * -GI *  *  *  *  *  * \n"
      "P9 *  *  *  *  *  *  *  *  * \n"
      "P+\n"
      "P-\n"
      "+\n");

    ASSERT_EQ(true, pos.isChecking());
  }

  {
    // checked by bishop
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  * +UM *  * \n"
      "P5 *  *  * -FU *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  * -GI+OU *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 * -KA *  *  *  *  *  *  * \n"
      "P+\n"
      "P-\n"
      "+\n");

    ASSERT_EQ(true, pos.isChecking());
  }

  {
    // not checked
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  * -KY-OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  * +UM *  * \n"
      "P5 *  *  * -FU *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  * -GI+OU *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  * -KA *  *  *  *  *  * \n"
      "P+\n"
      "P-\n"
      "+\n");

    ASSERT_EQ(false, pos.isChecking());
  }

  {
    // checked by dragon
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  * +UM *  * \n"
      "P5 *  *  * -FU *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  * -GI+OU *  *  * -RY * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  *  *  *  *  *  * \n"
      "P+\n"
      "P-\n"
      "+\n");

    ASSERT_EQ(true, pos.isChecking());
  }

  {
    // checked by lance
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  * -KY *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  * +UM *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  * -GI+OU *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  *  *  *  *  *  * \n"
      "P+\n"
      "P-\n"
      "+\n");

    ASSERT_EQ(true, pos.isChecking());
  }

  {
    // not checked
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  * +UM *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  * -GI+OU *  *  *  *  * \n"
      "P8 *  *  * -KY *  *  *  *  * \n"
      "P9 *  *  *  *  *  *  *  *  * \n"
      "P+\n"
      "P-\n"
      "+\n");

    ASSERT_EQ(false, pos.isChecking());
  }

  {
    // not checked
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  * +UM-KA * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  * -KE *  *  *  *  * \n"
      "P7 *  * -GI *  *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  * +OU *  *  *  *  * \n"
      "P+\n"
      "P-\n"
      "+\n");

    ASSERT_EQ(false, pos.isChecking());
  }

  {
    // checked
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  * +UM * -KA\n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  * -KE *  *  *  *  * \n"
      "P7 *  * -GI *  *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  * +OU *  *  *  *  * \n"
      "P+\n"
      "P-\n"
      "+\n");

    ASSERT_EQ(true, pos.isChecking());
  }

  {
    // not checked
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  *  *  *  *  *  * \n"
      "P2 *  *  * -OU *  *  *  *  * \n"
      "P3 *  *  *  * +FU *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  * +KE *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  * +KY *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+\n"
      "P-\n"
      "-\n");

    ASSERT_EQ(false, pos.isChecking());
  }

  {
    // checked by tokin
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  *  *  *  *  *  * \n"
      "P2 *  *  * -OU+TO *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  * +KE *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  * +KY *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+\n"
      "P-\n"
      "-\n");

    ASSERT_EQ(true, pos.isChecking());
  }

  {
    // not checked
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * +TO *  *  *  * \n"
      "P2 *  *  * -OU *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  * +KE *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  * +KY *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+\n"
      "P-\n"
      "-\n");

    ASSERT_EQ(false, pos.isChecking());
  }

  {
    // checked by horse
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  *  *  *  *  *  * \n"
      "P2 *  *  * -OU *  *  *  *  * \n"
      "P3 *  *  *  * +FU *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5+UM *  * +KE *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  * +KY *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+\n"
      "P-\n"
      "-\n");

    ASSERT_EQ(true, pos.isChecking());
  }

  {
    // not checked
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  *  *  *  *  *  * \n"
      "P2 *  *  * -OU *  *  *  *  * \n"
      "P3 *  *  *  * +FU *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 * +UM * +KE *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  * +KY *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+\n"
      "P-\n"
      "-\n");

    ASSERT_EQ(false, pos.isChecking());
  }

  {
    // checked by lance
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  *  *  *  *  *  * \n"
      "P2 *  *  * -OU *  *  *  *  * \n"
      "P3 *  *  *  * +FU *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 * +UM *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  * +KY *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+\n"
      "P-\n"
      "-\n");

    ASSERT_EQ(true, pos.isChecking());
  }

  {
    // not checked
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  *  *  *  *  *  * \n"
      "P2 *  *  * +KY *  *  *  *  * \n"
      "P3 *  *  *  * +FU *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5+UM *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  * -OU *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+\n"
      "P-\n"
      "-\n");

    ASSERT_EQ(false, pos.isChecking());
  }
}

TEST(PositionTest, testGetCheckState) {
  {
    // checked by pawn
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  * +UM *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  * -FU *  *  *  *  * \n"
      "P7 *  *  * +OU *  *  *  *  * \n"
      "P8 *  *  * +FU *  *  *  *  * \n"
      "P9 *  *  *  *  *  *  *  *  * \n"
      "P+\n"
      "P-\n"
      "+\n");

    auto state = pos.getCheckState();
    ASSERT_EQ(Square::s66(), state.from1);
    ASSERT_EQ(Square::invalid(), state.from2);
  }

  {
    // not checked
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  * -KE * +UM *  * \n"
      "P5 *  * -GI-FU *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  * +OU *  *  *  *  * \n"
      "P8 *  *  * +FU *  *  *  *  * \n"
      "P9 *  *  *  *  *  *  *  *  * \n"
      "P+\n"
      "P-\n"
      "+\n");

    auto state = pos.getCheckState();
    ASSERT_EQ(Square::invalid(), state.from1);
    ASSERT_EQ(Square::invalid(), state.from2);
  }

  {
    // checked by knight
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  * +UM *  * \n"
      "P5 *  *  * -FU-KE *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  * +OU *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  *  *  *  *  *  * \n"
      "P+\n"
      "P-\n"
      "+\n");

    auto state = pos.getCheckState();
    ASSERT_EQ(Square::s55(), state.from1);
    ASSERT_EQ(Square::invalid(), state.from2);
  }

  {
    // checked by silver
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  * +UM *  * \n"
      "P5 *  *  * -FU *  *  *  *  * \n"
      "P6 *  * -GI *  *  *  *  *  * \n"
      "P7 *  *  * +OU *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  *  *  *  *  *  * \n"
      "P+\n"
      "P-\n"
      "+\n");

    auto state = pos.getCheckState();
    ASSERT_EQ(Square::s76(), state.from1);
    ASSERT_EQ(Square::invalid(), state.from2);
  }

  {
    // checked by silver
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  * +UM *  * \n"
      "P5 *  *  * -FU *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  * +OU *  *  *  *  * \n"
      "P8 *  * -GI *  *  *  *  *  * \n"
      "P9 *  *  *  *  *  *  *  *  * \n"
      "P+\n"
      "P-\n"
      "+\n");

    auto state = pos.getCheckState();
    ASSERT_EQ(Square::s78(), state.from1);
    ASSERT_EQ(Square::invalid(), state.from2);
  }

  {
    // checked by bishop
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  * +UM *  * \n"
      "P5 *  *  * -FU *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  * -GI+OU *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 * -KA *  *  *  *  *  *  * \n"
      "P+\n"
      "P-\n"
      "+\n");

    auto state = pos.getCheckState();
    ASSERT_EQ(Square::s89(), state.from1);
    ASSERT_EQ(Square::invalid(), state.from2);
  }

  {
    // checked by bishop and promoted silver
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  * +UM *  * \n"
      "P5 *  *  * -FU *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  * +OU-NG *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  *  * -KA *  *  * \n"
      "P+\n"
      "P-\n"
      "+\n");

    auto state = pos.getCheckState();
    ASSERT_EQ(Square::s57(), state.from1);
    ASSERT_EQ(Square::s49(), state.from2);
  }

  {
    // checked by dragon
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  * +UM *  * \n"
      "P5 *  *  * -FU *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  * -GI+OU *  *  * -RY * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  *  *  *  *  *  * \n"
      "P+\n"
      "P-\n"
      "+\n");

    auto state = pos.getCheckState();
    ASSERT_EQ(Square::s27(), state.from1);
    ASSERT_EQ(Square::invalid(), state.from2);
  }

  {
    // checked by lance
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  * -KY *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  * +UM *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  * -GI+OU *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  *  *  *  *  *  * \n"
      "P+\n"
      "P-\n"
      "+\n");

    auto state = pos.getCheckState();
    ASSERT_EQ(Square::s62(), state.from1);
    ASSERT_EQ(Square::invalid(), state.from2);
  }

  {
    // checked by lance and knight
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  * -KY *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  * +UM *  * \n"
      "P5 *  * -KE *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  * -GI+OU *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  *  *  *  *  *  * \n"
      "P+\n"
      "P-\n"
      "+\n");

    auto state = pos.getCheckState();
    ASSERT_EQ(Square::s75(), state.from1);
    ASSERT_EQ(Square::s62(), state.from2);
  }

  {
    // checked by rook and horse
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  * +UM *  * \n"
      "P5 *  *  *  *  * -UM *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  * -GI+OU *  *  *  * -HI\n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  *  *  *  *  *  * \n"
      "P+\n"
      "P-\n"
      "+\n");

    auto state = pos.getCheckState();
    ASSERT_EQ(Square::s17(), state.from1);
    ASSERT_EQ(Square::s45(), state.from2);
  }

  {
    // not checked
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  * +UM-KA * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  * -KE *  *  *  *  * \n"
      "P7 *  * -GI *  *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  * +OU *  *  *  *  * \n"
      "P+\n"
      "P-\n"
      "+\n");

    auto state = pos.getCheckState();
    ASSERT_EQ(Square::invalid(), state.from1);
    ASSERT_EQ(Square::invalid(), state.from2);
  }

  {
    // checked
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  * +UM * -KA\n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  * -KE *  *  *  *  * \n"
      "P7 *  * -GI *  *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  * +OU *  *  *  *  * \n"
      "P+\n"
      "P-\n"
      "+\n");

    auto state = pos.getCheckState();
    ASSERT_EQ(Square::s14(), state.from1);
    ASSERT_EQ(Square::invalid(), state.from2);
  }

  {
    // not checked
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  *  *  *  *  *  * \n"
      "P2 *  *  * -OU *  *  *  *  * \n"
      "P3 *  *  *  * +FU *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  * +KE *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  * +KY *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+\n"
      "P-\n"
      "-\n");

    auto state = pos.getCheckState();
    ASSERT_EQ(Square::invalid(), state.from1);
    ASSERT_EQ(Square::invalid(), state.from2);
  }

  {
    // checked by tokin
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  *  *  *  *  *  * \n"
      "P2 *  *  * -OU+TO *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  * +KE *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  * +KY *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+\n"
      "P-\n"
      "-\n");

    auto state = pos.getCheckState();
    ASSERT_EQ(Square::s52(), state.from1);
    ASSERT_EQ(Square::invalid(), state.from2);
  }

  {
    // checked by tokin and bishop
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  *  *  *  *  *  * \n"
      "P2 *  *  * -OU+TO *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  * +KE *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  * +KY *  *  *  * +KA\n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+\n"
      "P-\n"
      "-\n");

    auto state = pos.getCheckState();
    ASSERT_EQ(Square::s52(), state.from1);
    ASSERT_EQ(Square::s17(), state.from2);
  }

  {
    // checked by horse
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  *  *  *  *  *  * \n"
      "P2 *  *  * -OU *  *  *  *  * \n"
      "P3 *  *  *  * +FU *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5+UM *  * +KE *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  * +KY *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+\n"
      "P-\n"
      "-\n");

    auto state = pos.getCheckState();
    ASSERT_EQ(Square::s95(), state.from1);
    ASSERT_EQ(Square::invalid(), state.from2);
  }

  {
    // checked by lance
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  *  *  *  *  *  * \n"
      "P2 *  *  * -OU *  *  *  *  * \n"
      "P3 *  *  *  * +FU *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 * +UM *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  * +KY *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+\n"
      "P-\n"
      "-\n");

    auto state = pos.getCheckState();
    ASSERT_EQ(Square::s67(), state.from1);
    ASSERT_EQ(Square::invalid(), state.from2);
  }

  {
    // checked by lance and gold
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  *  *  *  *  *  * \n"
      "P2 *  *  * -OU *  *  *  *  * \n"
      "P3 *  * +KI * +FU *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 * +UM *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  * +KY *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+\n"
      "P-\n"
      "-\n");

    auto state = pos.getCheckState();
    ASSERT_EQ(Square::s73(), state.from1);
    ASSERT_EQ(Square::s67(), state.from2);
  }

  {
    // checked by dragon
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  *  *  *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  * +RY *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  *  * -OU *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  * +KI+OU+KI *  *  * \n"
      "P+\n"
      "P-\n"
      "-\n");

    auto state = pos.getCheckState();
    ASSERT_EQ(Square::s44(), state.from1);
    ASSERT_EQ(Square::invalid(), state.from2);
  }

  {
    // checked by bishop and dragon
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  *  *  *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 * +KA *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  * +RY *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  *  * -OU *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  * +KI+OU+KI *  *  * \n"
      "P+\n"
      "P-\n"
      "-\n");

    auto state = pos.getCheckState();
    ASSERT_EQ(Square::s44(), state.from1);
    ASSERT_EQ(Square::s83(), state.from2);
  }
}

TEST(PositionTest, testIsMate) {
  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  * -FU-KI *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  * +OU *  *  *  *  * \n"
      "P+\n"
      "P-\n"
      "+\n");

    ASSERT_EQ(false, pos.isMate());
  }

  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  * -FU *  *  *  *  * \n"
      "P8 *  *  * -KI *  *  *  *  * \n"
      "P9 *  *  * +OU *  *  *  *  * \n"
      "P+\n"
      "P-\n"
      "+\n");

    ASSERT_EQ(true, pos.isMate());
  }

  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  * -FU *  *  *  *  * \n"
      "P7 *  *  *  *  *  *  *  *  * \n"
      "P8 *  *  * -KI *  *  *  *  * \n"
      "P9 *  *  * +OU *  *  *  *  * \n"
      "P+\n"
      "P-\n"
      "+\n");

    ASSERT_EQ(false, pos.isMate());
  }

  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  * -FU *  *  *  *  * \n"
      "P8 *  *  * -KI *  *  *  *  * \n"
      "P9 *  *  * +OU+GI *  *  *  * \n"
      "P+\n"
      "P-\n"
      "+\n");

    ASSERT_EQ(false, pos.isMate());
  }

  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  * -KE * \n"
      "P6 *  *  *  * -RY *  *  *  * \n"
      "P7 *  *  *  *  *  * +OU+FU * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  *  *  * -UM *  * \n"
      "P+\n"
      "P-\n"
      "+\n");

    ASSERT_EQ(true, pos.isMate());
  }

  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  * -KE * \n"
      "P6 *  *  *  * -RY *  *  *  * \n"
      "P7 *  *  *  *  *  * +OU *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  *  *  * -UM *  * \n"
      "P+\n"
      "P-\n"
      "+\n");

    ASSERT_EQ(false, pos.isMate());
  }

  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU * -KY *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  * -RY *  *  *  * \n"
      "P7 *  *  *  *  *  * +OU+FU * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  *  *  * -UM *  * \n"
      "P+\n"
      "P-\n"
      "+\n");

    ASSERT_EQ(true, pos.isMate());
  }

  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU * -KY *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  * -RY *  *  *  * \n"
      "P7 *  *  *  *  *  * +OU+FU * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  *  *  * -UM *  * \n"
      "P+00FU\n"
      "P-\n"
      "+\n");

    ASSERT_EQ(false, pos.isMate());
  }

  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU * -KY *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  * -RY *  *  *  * \n"
      "P7 *  *  *  *  *  * +OU+FU * \n"
      "P8 *  *  *  *  *  * +FU *  * \n"
      "P9 *  *  *  *  *  * -UM *  * \n"
      "P+00FU\n"
      "P-\n"
      "+\n");

    ASSERT_EQ(true, pos.isMate());
  }

  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU * -KY *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  * -RY *  *  *  * \n"
      "P7 *  *  *  *  *  * +OU+FU * \n"
      "P8 *  *  *  *  *  * +FU *  * \n"
      "P9 *  *  *  *  *  * -UM *  * \n"
      "P+00FU00KE\n"
      "P-\n"
      "+\n");

    ASSERT_EQ(false, pos.isMate());
  }

  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU * -KY *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  * -KA\n"
      "P6 *  *  *  * -RY *  *  *  * \n"
      "P7 *  *  *  *  *  * +OU+FU * \n"
      "P8 *  *  *  *  *  * +FU *  * \n"
      "P9 *  *  *  *  *  * -UM *  * \n"
      "P+00FU00KE\n"
      "P-\n"
      "+\n");

    ASSERT_EQ(true, pos.isMate());
  }

  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  * -OU *  *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  * +FU+KI *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  *  *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+\n"
      "P-\n"
      "-\n");

    ASSERT_EQ(false, pos.isMate());
  }

  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  * -OU *  *  *  *  * \n"
      "P2 *  *  * +KI *  *  *  *  * \n"
      "P3 *  *  * +FU *  *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  *  *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+\n"
      "P-\n"
      "-\n");

    ASSERT_EQ(true, pos.isMate());
  }

  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  * -OU *  *  *  *  * \n"
      "P2 *  *  * +KI *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  * +FU *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  *  *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+\n"
      "P-\n"
      "-\n");

    ASSERT_EQ(false, pos.isMate());
  }

  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  * -OU-GI *  *  *  * \n"
      "P2 *  *  * +KI *  *  *  *  * \n"
      "P3 *  *  * +FU *  *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  *  *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+\n"
      "P-\n"
      "-\n");

    ASSERT_EQ(false, pos.isMate());
  }

  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  *  *  * +UM *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  * -OU-FU * \n"
      "P4 *  *  *  * +RY *  *  *  * \n"
      "P5 *  *  *  *  *  *  * +KE * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  *  *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+\n"
      "P-\n"
      "-\n");

    ASSERT_EQ(true, pos.isMate());
  }

  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  *  *  * +UM *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  * -OU *  * \n"
      "P4 *  *  *  * +RY *  *  *  * \n"
      "P5 *  *  *  *  *  *  * +KE * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  *  *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+\n"
      "P-\n"
      "-\n");

    ASSERT_EQ(false, pos.isMate());
  }

  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  *  *  * +UM *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  * -OU-FU * \n"
      "P4 *  *  *  * +RY *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  *  *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU * +KY *  * \n"
      "P+\n"
      "P-\n"
      "-\n");

    ASSERT_EQ(true, pos.isMate());
  }

  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  *  *  * +UM *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  * -OU-FU * \n"
      "P4 *  *  *  * +RY *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  *  *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU * +KY *  * \n"
      "P+\n"
      "P-00FU\n"
      "-\n");

    ASSERT_EQ(false, pos.isMate());
  }

  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  *  *  * +UM *  * \n"
      "P2 *  *  *  *  *  * -FU *  * \n"
      "P3 *  *  *  *  *  * -OU-FU * \n"
      "P4 *  *  *  * +RY *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  *  *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU * +KY *  * \n"
      "P+\n"
      "P-00FU\n"
      "-\n");

    ASSERT_EQ(true, pos.isMate());
  }

  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  *  *  * +UM *  * \n"
      "P2 *  *  *  *  *  * -FU *  * \n"
      "P3 *  *  *  *  *  * -OU-FU * \n"
      "P4 *  *  *  * +RY *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  *  *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU * +KY *  * \n"
      "P+\n"
      "P-00FU00KE\n"
      "-\n");

    ASSERT_EQ(false, pos.isMate());
  }

  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  *  *  * +UM *  * \n"
      "P2 *  *  *  *  *  * -FU *  * \n"
      "P3 *  *  *  *  *  * -OU-FU * \n"
      "P4 *  *  *  * +RY *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  * +KA\n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  *  *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU * +KY *  * \n"
      "P+\n"
      "P-00FU00KE\n"
      "-\n");

    ASSERT_EQ(true, pos.isMate());
  }
}

TEST(PositionTest, testIsMateWithPawnDrop) {
  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  *  *  *  *  * -OU\n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  * +RY * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  *  *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+\n"
      "P-\n"
      "+\n");

    ASSERT_EQ(true, pos.isMateWithPawnDrop());
  }

  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  *  *  *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  * -OU\n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  *  * +RY * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  *  *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+\n"
      "P-\n"
      "+\n");

    ASSERT_EQ(false, pos.isMateWithPawnDrop());
  }

  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  *  *  *  *  *  * \n"
      "P2 *  *  *  *  *  *  * -KE-KY\n"
      "P3 *  *  *  *  *  *  *  * -OU\n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  * +RY * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  *  *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+\n"
      "P-\n"
      "+\n");

    ASSERT_EQ(false, pos.isMateWithPawnDrop());
  }

  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  *  *  * +UM *  * \n"
      "P2 *  *  *  *  *  *  * -KE-KY\n"
      "P3 *  *  *  *  *  *  *  * -OU\n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  * +RY * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  *  *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+\n"
      "P-\n"
      "+\n");

    ASSERT_EQ(true, pos.isMateWithPawnDrop());
  }

  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  *  *  *  * -RY * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  *  *  *  *  * +OU\n"
      "P+\n"
      "P-\n"
      "-\n");

    ASSERT_EQ(true, pos.isMateWithPawnDrop());
  }

  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  * -RY * \n"
      "P7 *  *  *  *  *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  * +OU\n"
      "P9 *  *  *  *  *  *  *  *  * \n"
      "P+\n"
      "P-\n"
      "-\n");

    ASSERT_EQ(false, pos.isMateWithPawnDrop());
  }

  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  * -RY * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  *  *  *  *  * +OU\n"
      "P8 *  *  *  *  *  *  * +KE+KY\n"
      "P9 *  *  *  *  *  *  *  *  * \n"
      "P+\n"
      "P-\n"
      "-\n");

    ASSERT_EQ(false, pos.isMateWithPawnDrop());
  }

  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  * -RY * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  *  *  *  *  * +OU\n"
      "P8 *  *  *  *  *  *  * +KE+KY\n"
      "P9 *  *  *  *  *  * -UM *  * \n"
      "P+\n"
      "P-\n"
      "-\n");

    ASSERT_EQ(true, pos.isMateWithPawnDrop());
  }
}

#endif // !defined(NDEBUG)
