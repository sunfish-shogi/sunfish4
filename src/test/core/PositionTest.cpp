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

TEST(PositionTest, testToStringSFEN) {
  {
    Position pos(Position::Handicap::Even);
    ASSERT_EQ("lnsgkgsnl/1r5b1/ppppppppp/9/9/9/PPPPPPPPP/1B5R1/LNSGKGSNL b - 1", pos.toStringSFEN());
  }

  {
    Position pos(Position::Handicap::TwoPieces);
    ASSERT_EQ("lnsgkgsnl/9/ppppppppp/9/9/9/PPPPPPPPP/1B5R1/LNSGKGSNL w - 1", pos.toStringSFEN());
  }
}

TEST(PositionTest, testIsLegalMoveMaybe) {
  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  * -FU * +RY * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  * +KA *  *  *  *  *  * \n"
      "P7 *  *  *  * +FU+OU *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  *  *  *  *  *  * \n"
      "P+00KY00FU00FU\n"
      "P-\n"
      "+\n");
    CheckState checkState = pos.getCheckState();

    // pawn (drop)
    ASSERT_EQ(true , pos.isLegalMoveMaybe(Move(PieceType::pawn(), Square::s42()), checkState));
    ASSERT_EQ(true , pos.isLegalMoveMaybe(Move(PieceType::pawn(), Square::s89()), checkState));
    ASSERT_EQ(false, pos.isLegalMoveMaybe(Move(PieceType::pawn(), Square::s56()), checkState));
    ASSERT_EQ(false, pos.isLegalMoveMaybe(Move(PieceType::pawn(), Square::s76()), checkState));

    // lance (drop)
    ASSERT_EQ(true , pos.isLegalMoveMaybe(Move(PieceType::lance(), Square::s32()), checkState));
    ASSERT_EQ(true , pos.isLegalMoveMaybe(Move(PieceType::lance(), Square::s56()), checkState));
    ASSERT_EQ(true , pos.isLegalMoveMaybe(Move(PieceType::lance(), Square::s99()), checkState));
    ASSERT_EQ(false, pos.isLegalMoveMaybe(Move(PieceType::lance(), Square::s47()), checkState));

    // silver (drop)
    ASSERT_EQ(false, pos.isLegalMoveMaybe(Move(PieceType::silver(), Square::s88()), checkState));

    // pawn
    ASSERT_EQ(true , pos.isLegalMoveMaybe(Move(Square::s57(), Square::s56(), false), checkState));
    ASSERT_EQ(false, pos.isLegalMoveMaybe(Move(Square::s57(), Square::s55(), false), checkState));
    ASSERT_EQ(false, pos.isLegalMoveMaybe(Move(Square::s57(), Square::s58(), false), checkState));

    // bishop
    ASSERT_EQ(true , pos.isLegalMoveMaybe(Move(Square::s76(), Square::s65(), false), checkState));
    ASSERT_EQ(true , pos.isLegalMoveMaybe(Move(Square::s76(), Square::s54(), false), checkState));
    ASSERT_EQ(true , pos.isLegalMoveMaybe(Move(Square::s76(), Square::s43(), true ), checkState));
    ASSERT_EQ(false, pos.isLegalMoveMaybe(Move(Square::s76(), Square::s32(), true ), checkState));
    ASSERT_EQ(false, pos.isLegalMoveMaybe(Move(Square::s76(), Square::s46(), true ), checkState));

    // dragon
    ASSERT_EQ(true , pos.isLegalMoveMaybe(Move(Square::s23(), Square::s12(), false), checkState));
    ASSERT_EQ(false, pos.isLegalMoveMaybe(Move(Square::s23(), Square::s12(), true ), checkState));
    ASSERT_EQ(true , pos.isLegalMoveMaybe(Move(Square::s23(), Square::s29(), false), checkState));
    ASSERT_EQ(true , pos.isLegalMoveMaybe(Move(Square::s23(), Square::s33(), false), checkState));
    ASSERT_EQ(true , pos.isLegalMoveMaybe(Move(Square::s23(), Square::s43(), false), checkState));
    ASSERT_EQ(false, pos.isLegalMoveMaybe(Move(Square::s23(), Square::s53(), false), checkState));
    ASSERT_EQ(false, pos.isLegalMoveMaybe(Move(Square::s23(), Square::s45(), false), checkState));

    // king
    ASSERT_EQ(true , pos.isLegalMoveMaybe(Move(Square::s47(), Square::s56(), false), checkState));
    ASSERT_EQ(false, pos.isLegalMoveMaybe(Move(Square::s47(), Square::s57(), false), checkState));
    ASSERT_EQ(false, pos.isLegalMoveMaybe(Move(Square::s47(), Square::s69(), false), checkState));

    // not exists
    ASSERT_EQ(false, pos.isLegalMoveMaybe(Move(Square::s38(), Square::s37(), false), checkState));
  }

  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  * +OU *  *  * -RY * \n"
      "P7 *  * +FU *  * +FU * +FU * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  *  *  *  *  *  * \n"
      "P+00GI00FU00FU\n"
      "P-\n"
      "+\n");
    CheckState checkState = pos.getCheckState();

    // pawn (drop)
    ASSERT_EQ(false, pos.isLegalMoveMaybe(Move(PieceType::pawn(), Square::s16()), checkState));
    ASSERT_EQ(false, pos.isLegalMoveMaybe(Move(PieceType::pawn(), Square::s26()), checkState));
    ASSERT_EQ(true , pos.isLegalMoveMaybe(Move(PieceType::pawn(), Square::s36()), checkState));
    ASSERT_EQ(false, pos.isLegalMoveMaybe(Move(PieceType::pawn(), Square::s46()), checkState));
    ASSERT_EQ(true , pos.isLegalMoveMaybe(Move(PieceType::pawn(), Square::s56()), checkState));
    ASSERT_EQ(false, pos.isLegalMoveMaybe(Move(PieceType::pawn(), Square::s66()), checkState));
    ASSERT_EQ(false, pos.isLegalMoveMaybe(Move(PieceType::pawn(), Square::s76()), checkState));
    ASSERT_EQ(false, pos.isLegalMoveMaybe(Move(PieceType::pawn(), Square::s44()), checkState));

    // silver (drop)
    ASSERT_EQ(false, pos.isLegalMoveMaybe(Move(PieceType::silver(), Square::s26()), checkState));
    ASSERT_EQ(true , pos.isLegalMoveMaybe(Move(PieceType::silver(), Square::s36()), checkState));
    ASSERT_EQ(true , pos.isLegalMoveMaybe(Move(PieceType::silver(), Square::s46()), checkState));
    ASSERT_EQ(true , pos.isLegalMoveMaybe(Move(PieceType::silver(), Square::s56()), checkState));
    ASSERT_EQ(false, pos.isLegalMoveMaybe(Move(PieceType::silver(), Square::s66()), checkState));

    // pawn
    ASSERT_EQ(true , pos.isLegalMoveMaybe(Move(Square::s27(), Square::s26(), false), checkState));
    ASSERT_EQ(true , pos.isLegalMoveMaybe(Move(Square::s47(), Square::s46(), false), checkState));
    ASSERT_EQ(false, pos.isLegalMoveMaybe(Move(Square::s77(), Square::s76(), false), checkState));

    // king
    ASSERT_EQ(true , pos.isLegalMoveMaybe(Move(Square::s66(), Square::s56(), false), checkState));
    ASSERT_EQ(true , pos.isLegalMoveMaybe(Move(Square::s66(), Square::s75(), false), checkState));
    ASSERT_EQ(false, pos.isLegalMoveMaybe(Move(Square::s66(), Square::s64(), false), checkState));
  }

  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  *  *  *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  * -FU-OU *  *  * \n"
      "P4 *  * -KA *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  *  * +FU * -RY * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+\n"
      "P-00KY00FU00FU\n"
      "-\n");
    CheckState checkState = pos.getCheckState();

    // pawn (drop)
    ASSERT_EQ(true , pos.isLegalMoveMaybe(Move(PieceType::pawn(), Square::s48()), checkState));
    ASSERT_EQ(true , pos.isLegalMoveMaybe(Move(PieceType::pawn(), Square::s81()), checkState));
    ASSERT_EQ(false, pos.isLegalMoveMaybe(Move(PieceType::pawn(), Square::s54()), checkState));
    ASSERT_EQ(false, pos.isLegalMoveMaybe(Move(PieceType::pawn(), Square::s74()), checkState));

    // lance (drop)
    ASSERT_EQ(true , pos.isLegalMoveMaybe(Move(PieceType::lance(), Square::s38()), checkState));
    ASSERT_EQ(true , pos.isLegalMoveMaybe(Move(PieceType::lance(), Square::s54()), checkState));
    ASSERT_EQ(true , pos.isLegalMoveMaybe(Move(PieceType::lance(), Square::s91()), checkState));
    ASSERT_EQ(false, pos.isLegalMoveMaybe(Move(PieceType::lance(), Square::s43()), checkState));

    // silver (drop)
    ASSERT_EQ(false, pos.isLegalMoveMaybe(Move(PieceType::silver(), Square::s82()), checkState));

    // pawn
    ASSERT_EQ(true , pos.isLegalMoveMaybe(Move(Square::s53(), Square::s54(), false), checkState));
    ASSERT_EQ(false, pos.isLegalMoveMaybe(Move(Square::s53(), Square::s55(), false), checkState));
    ASSERT_EQ(false, pos.isLegalMoveMaybe(Move(Square::s53(), Square::s52(), false), checkState));

    // bishop
    ASSERT_EQ(true , pos.isLegalMoveMaybe(Move(Square::s74(), Square::s65(), false), checkState));
    ASSERT_EQ(true , pos.isLegalMoveMaybe(Move(Square::s74(), Square::s56(), false), checkState));
    ASSERT_EQ(true , pos.isLegalMoveMaybe(Move(Square::s74(), Square::s47(), true ), checkState));
    ASSERT_EQ(false, pos.isLegalMoveMaybe(Move(Square::s74(), Square::s38(), true ), checkState));
    ASSERT_EQ(false, pos.isLegalMoveMaybe(Move(Square::s74(), Square::s44(), true ), checkState));

    // dragon
    ASSERT_EQ(true , pos.isLegalMoveMaybe(Move(Square::s27(), Square::s18(), false), checkState));
    ASSERT_EQ(false, pos.isLegalMoveMaybe(Move(Square::s27(), Square::s18(), true ), checkState));
    ASSERT_EQ(true , pos.isLegalMoveMaybe(Move(Square::s27(), Square::s21(), false), checkState));
    ASSERT_EQ(true , pos.isLegalMoveMaybe(Move(Square::s27(), Square::s37(), false), checkState));
    ASSERT_EQ(true , pos.isLegalMoveMaybe(Move(Square::s27(), Square::s47(), false), checkState));
    ASSERT_EQ(false, pos.isLegalMoveMaybe(Move(Square::s27(), Square::s57(), false), checkState));
    ASSERT_EQ(false, pos.isLegalMoveMaybe(Move(Square::s27(), Square::s45(), false), checkState));

    // king
    ASSERT_EQ(true , pos.isLegalMoveMaybe(Move(Square::s43(), Square::s54(), false), checkState));
    ASSERT_EQ(false, pos.isLegalMoveMaybe(Move(Square::s43(), Square::s53(), false), checkState));
    ASSERT_EQ(false, pos.isLegalMoveMaybe(Move(Square::s43(), Square::s61(), false), checkState));

    // not exists
    ASSERT_EQ(false, pos.isLegalMoveMaybe(Move(Square::s32(), Square::s33(), false), checkState));
  }

  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  *  *  *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  * -FU *  * -FU * -FU * \n"
      "P4 *  *  * -OU *  *  * +RY * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  *  *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+\n"
      "P-00GI00FU00FU\n"
      "-\n");
    CheckState checkState = pos.getCheckState();

    // pawn (drop)
    ASSERT_EQ(false, pos.isLegalMoveMaybe(Move(PieceType::pawn(), Square::s14()), checkState));
    ASSERT_EQ(false, pos.isLegalMoveMaybe(Move(PieceType::pawn(), Square::s24()), checkState));
    ASSERT_EQ(true , pos.isLegalMoveMaybe(Move(PieceType::pawn(), Square::s34()), checkState));
    ASSERT_EQ(false, pos.isLegalMoveMaybe(Move(PieceType::pawn(), Square::s44()), checkState));
    ASSERT_EQ(true , pos.isLegalMoveMaybe(Move(PieceType::pawn(), Square::s54()), checkState));
    ASSERT_EQ(false, pos.isLegalMoveMaybe(Move(PieceType::pawn(), Square::s64()), checkState));
    ASSERT_EQ(false, pos.isLegalMoveMaybe(Move(PieceType::pawn(), Square::s74()), checkState));
    ASSERT_EQ(false, pos.isLegalMoveMaybe(Move(PieceType::pawn(), Square::s46()), checkState));

    // silver (drop)
    ASSERT_EQ(false, pos.isLegalMoveMaybe(Move(PieceType::silver(), Square::s24()), checkState));
    ASSERT_EQ(true , pos.isLegalMoveMaybe(Move(PieceType::silver(), Square::s34()), checkState));
    ASSERT_EQ(true , pos.isLegalMoveMaybe(Move(PieceType::silver(), Square::s44()), checkState));
    ASSERT_EQ(true , pos.isLegalMoveMaybe(Move(PieceType::silver(), Square::s54()), checkState));
    ASSERT_EQ(false, pos.isLegalMoveMaybe(Move(PieceType::silver(), Square::s64()), checkState));

    // pawn
    ASSERT_EQ(true , pos.isLegalMoveMaybe(Move(Square::s23(), Square::s24(), false), checkState));
    ASSERT_EQ(true , pos.isLegalMoveMaybe(Move(Square::s43(), Square::s44(), false), checkState));
    ASSERT_EQ(false, pos.isLegalMoveMaybe(Move(Square::s73(), Square::s74(), false), checkState));

    // king
    ASSERT_EQ(true , pos.isLegalMoveMaybe(Move(Square::s64(), Square::s54(), false), checkState));
    ASSERT_EQ(true , pos.isLegalMoveMaybe(Move(Square::s64(), Square::s75(), false), checkState));
    ASSERT_EQ(false, pos.isLegalMoveMaybe(Move(Square::s64(), Square::s66(), false), checkState));
  }
}

TEST(PositionTest, testDoMove) {
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

    Move move(Square::s77(), Square::s76(), false);

    Piece captured;
    bool ok = pos.doMove(move, captured);
    ASSERT_EQ(true , ok);
    ASSERT_EQ(Piece::empty(), captured);
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

    Move move(Square::s33(), Square::s34(), false);

    Piece captured;
    bool ok = pos.doMove(move, captured);
    ASSERT_EQ(true , ok);
    ASSERT_EQ(Piece::empty(), captured);
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

    Move move(Square::s88(), Square::s22(), true);

    Piece captured;
    bool ok = pos.doMove(move, captured);
    ASSERT_EQ(true , ok);
    ASSERT_EQ(Piece::whiteBishop(), captured);
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

    Move move(Square::s31(), Square::s22(), false);

    Piece captured;
    bool ok = pos.doMove(move, captured);
    ASSERT_EQ(true , ok);
    ASSERT_EQ(Piece::blackHorse(), captured);
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

    Move move(PieceType::bishop(), Square::s45());

    Piece captured;
    bool ok = pos.doMove(move, captured);
    ASSERT_EQ(true , ok);
    ASSERT_EQ(Piece::empty(), captured);
    assertEq(expectPos, pos);
  }

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
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7+FU+FU+FU+FU+FU+FU+FU+FU+FU\n"
      "P8 * +KA * +OU *  *  * +HI * \n"
      "P9+KY+KE+GI+KI * +KI+GI+KE+KY\n"
      "P+\n"
      "P-\n"
      "-\n");

    Move move(Square::s59(), Square::s68(), false);

    Piece captured;
    bool ok = pos.doMove(move, captured);
    ASSERT_EQ(true , ok);
    ASSERT_EQ(Piece::empty(), captured);
    assertEq(expectPos, pos);
  }

  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1-KY-KE-GI-KI-OU-KI-GI-KE-KY\n"
      "P2 * -HI *  *  *  *  * -KA * \n"
      "P3-FU-FU-FU-FU-FU-FU-FU-FU-FU\n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7+FU+FU+FU+FU+FU+FU+FU+FU+FU\n"
      "P8 * +KA * +OU *  *  * +HI * \n"
      "P9+KY+KE+GI+KI * +KI+GI+KE+KY\n"
      "P+\n"
      "P-\n"
      "-\n");

    Position expectPos = PositionUtil::createPositionFromCsaString(
      "P1-KY-KE-GI-KI * -KI-GI-KE-KY\n"
      "P2 * -HI *  * -OU *  * -KA * \n"
      "P3-FU-FU-FU-FU-FU-FU-FU-FU-FU\n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7+FU+FU+FU+FU+FU+FU+FU+FU+FU\n"
      "P8 * +KA * +OU *  *  * +HI * \n"
      "P9+KY+KE+GI+KI * +KI+GI+KE+KY\n"
      "P+\n"
      "P-\n"
      "+\n");

    Move move(Square::s51(), Square::s52(), false);

    Piece captured;
    bool ok = pos.doMove(move, captured);
    ASSERT_EQ(true , ok);
    ASSERT_EQ(Piece::empty(), captured);
    assertEq(expectPos, pos);
  }

  {
    const char* posStr =
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  * -UM *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  * +OU *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  *  *  *  *  *  * \n"
      "P+\n"
      "P-\n"
      "+\n";
    Position pos = PositionUtil::createPositionFromCsaString(posStr);

    Move king57(Square::s67(), Square::s57(), false);
    Move king58(Square::s67(), Square::s58(), false);
    Move king68(Square::s67(), Square::s68(), false);
    Move king76(Square::s67(), Square::s76(), false);
    Move king78(Square::s67(), Square::s78(), false);
    Move king56(Square::s67(), Square::s56(), false);
    Move king66(Square::s67(), Square::s66(), false);
    Move king77(Square::s67(), Square::s77(), false);

    Piece captured;

    bool ok = pos.doMove(king57, captured);
    ASSERT_EQ(true , ok);
    pos.undoMove(king57, captured);
    ASSERT_EQ(posStr, pos.toString());

    ok = pos.doMove(king58, captured);
    ASSERT_EQ(true , ok);
    pos.undoMove(king58, captured);
    ASSERT_EQ(posStr, pos.toString());

    ok = pos.doMove(king68, captured);
    ASSERT_EQ(true , ok);
    pos.undoMove(king68, captured);
    ASSERT_EQ(posStr, pos.toString());

    ok = pos.doMove(king76, captured);
    ASSERT_EQ(true , ok);
    pos.undoMove(king76, captured);
    ASSERT_EQ(posStr, pos.toString());

    ok = pos.doMove(king78, captured);
    ASSERT_EQ(true , ok);
    pos.undoMove(king78, captured);
    ASSERT_EQ(posStr, pos.toString());

    ok = pos.doMove(king56, captured);
    ASSERT_EQ(false, ok);
    ASSERT_EQ(posStr, pos.toString());

    ok = pos.doMove(king66, captured);
    ASSERT_EQ(false, ok);
    ASSERT_EQ(posStr, pos.toString());

    ok = pos.doMove(king77, captured);
    ASSERT_EQ(false, ok);
    ASSERT_EQ(posStr, pos.toString());
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

    Move move(Square::s77(), Square::s76(), false);

    pos.undoMove(move, Piece::empty());

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

    Move move(Square::s33(), Square::s34(), false);

    pos.undoMove(move, Piece::empty());

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

    Move move(Square::s88(), Square::s22(), true);

    pos.undoMove(move, Piece::whiteBishop());

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

    Move move(Square::s31(), Square::s22(), false);

    pos.undoMove(move, Piece::blackHorse());

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

    Move move(PieceType::bishop(), Square::s45());

    pos.undoMove(move, Piece::empty());

    assertEq(expectPos, pos);
  }
}

TEST(PositionTest, testDoNullMove) {
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
      "+\n");

    pos.doNullMove();

    assertEq(expectPos, pos);
  }
}

TEST(PositionTest, testUndoNullMove) {
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

    pos.undoNullMove();

    assertEq(expectPos, pos);
  }
}

TEST(PositionTest, testIsCapture) {
  Position pos = PositionUtil::createPositionFromCsaString(
    "P1-KY-KE-GI-KI * -KI * -KE-KY\n"
    "P2 *  * -OU *  * -GI * -HI * \n"
    "P3-FU-FU-FU-FU-FU * -KA * -FU\n"
    "P4 *  *  *  *  * -FU-FU-FU * \n"
    "P5 *  *  *  *  * +FU *  *  * \n"
    "P6 *  * +FU *  *  * +FU *  * \n"
    "P7+FU+FU+KA+FU+FU *  * +FU+FU\n"
    "P8 *  *  * +OU * +HI+GI *  * \n"
    "P9+KY+KE+GI+KI * +KI * +KE+KY\n"
    "P+00KA\n"
    "P-\n"
    "+\n");
  ASSERT_EQ(true , pos.isCapture(Move(Square::s77(), Square::s44(), false)));
  ASSERT_EQ(false, pos.isCapture(Move(Square::s77(), Square::s55(), false)));
}

TEST(PositionTest, testInCheck) {
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

    ASSERT_EQ(true , pos.inCheck());
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

    ASSERT_EQ(false, pos.inCheck());
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

    ASSERT_EQ(true , pos.inCheck());
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

    ASSERT_EQ(true , pos.inCheck());
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

    ASSERT_EQ(true , pos.inCheck());
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

    ASSERT_EQ(true , pos.inCheck());
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

    ASSERT_EQ(false, pos.inCheck());
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

    ASSERT_EQ(true , pos.inCheck());
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

    ASSERT_EQ(true , pos.inCheck());
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

    ASSERT_EQ(false, pos.inCheck());
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

    ASSERT_EQ(false, pos.inCheck());
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

    ASSERT_EQ(true , pos.inCheck());
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

    ASSERT_EQ(false, pos.inCheck());
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

    ASSERT_EQ(true , pos.inCheck());
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

    ASSERT_EQ(false, pos.inCheck());
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

    ASSERT_EQ(true , pos.inCheck());
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

    ASSERT_EQ(false, pos.inCheck());
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

    ASSERT_EQ(true , pos.inCheck());
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

    ASSERT_EQ(false, pos.inCheck());
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
    // checked by rook
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  * +UM *  * \n"
      "P5 *  *  * -FU *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  * -GI+OU *  *  *  *  * \n"
      "P8 *  *  * -HI *  *  *  *  * \n"
      "P9 *  *  *  *  *  *  *  *  * \n"
      "P+\n"
      "P-\n"
      "+\n");

    auto state = pos.getCheckState();
    ASSERT_EQ(Square::s68(), state.from1);
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
    // checked by lance
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  *  *  *  *  *  * \n"
      "P2 *  *  * -OU *  *  *  *  * \n"
      "P3 *  *  * +KY+FU *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 * +UM *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  *  *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+\n"
      "P-\n"
      "-\n");

    auto state = pos.getCheckState();
    ASSERT_EQ(Square::s63(), state.from1);
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

    ASSERT_EQ(true , pos.isMate());
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

    ASSERT_EQ(true , pos.isMate());
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

    ASSERT_EQ(true , pos.isMate());
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

    ASSERT_EQ(true , pos.isMate());
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

    ASSERT_EQ(true , pos.isMate());
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

    ASSERT_EQ(true , pos.isMate());
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

    ASSERT_EQ(true , pos.isMate());
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

    ASSERT_EQ(true , pos.isMate());
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

    ASSERT_EQ(true , pos.isMate());
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

    ASSERT_EQ(true , pos.isMate());
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

    ASSERT_EQ(true , pos.isMateWithPawnDrop());
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

    ASSERT_EQ(true , pos.isMateWithPawnDrop());
  }

  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  *  * -OU *  *  * \n"
      "P2 *  *  * +RY *  * -FU *  * \n"
      "P3 *  *  *  * +KA *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
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
      "P1 *  *  *  *  * -OU-FU *  * \n"
      "P2 *  *  * +RY *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
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
      "P1 *  *  *  *  * -OU-FU *  * \n"
      "P2 *  *  * +RY *  *  *  *  * \n"
      "P3 *  *  *  *  *  * +FU *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  *  *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+\n"
      "P-\n"
      "+\n");

    ASSERT_EQ(true , pos.isMateWithPawnDrop());
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

    ASSERT_EQ(true , pos.isMateWithPawnDrop());
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

    ASSERT_EQ(true , pos.isMateWithPawnDrop());
  }

  {
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  * -KA *  *  *  * \n"
      "P8 *  *  * -RY *  * +FU *  * \n"
      "P9 *  *  *  *  * +OU *  *  * \n"
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
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  *  *  *  *  *  * \n"
      "P8 *  *  * -RY *  *  *  *  * \n"
      "P9 *  *  *  *  * +OU+FU *  * \n"
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
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  *  *  * -FU *  * \n"
      "P8 *  *  * -RY *  *  *  *  * \n"
      "P9 *  *  *  *  * +OU+FU *  * \n"
      "P+\n"
      "P-\n"
      "-\n");

    ASSERT_EQ(true , pos.isMateWithPawnDrop());
  }
}

TEST(PositionTest, testZobrist) {
  Position pos(Position::Handicap::Even);
  Piece captured;

  ASSERT_TRUE(0x00LLU != pos.getBoardHash());
  ASSERT_EQ(0x00LLU, pos.getHandHash());
  ASSERT_EQ(0x01LLU, pos.getTurnHash());
  ASSERT_EQ(0x01LLU, pos.getHash() & 0x01LLU);

  pos.doMove(Move(Square::s76(), Square::s75(), false), captured);

  ASSERT_TRUE(0x00LLU != pos.getBoardHash());
  ASSERT_EQ(0x00LLU, pos.getHandHash());
  ASSERT_EQ(0x00LLU, pos.getTurnHash());
  ASSERT_EQ(0x00LLU, pos.getHash() & 0x01LLU);

  pos.doMove(Move(Square::s33(), Square::s34(), false), captured);

  ASSERT_TRUE(0x00LLU != pos.getBoardHash());
  ASSERT_EQ(0x00LLU, pos.getHandHash());
  ASSERT_EQ(0x01LLU, pos.getTurnHash());
  ASSERT_EQ(0x01LLU, pos.getHash() & 0x01LLU);

  pos.doMove(Move(Square::s27(), Square::s26(), false), captured);

  ASSERT_TRUE(0x00LLU != pos.getBoardHash());
  ASSERT_EQ(0x00LLU, pos.getHandHash());
  ASSERT_EQ(0x00LLU, pos.getTurnHash());
  ASSERT_EQ(0x00LLU, pos.getHash() & 0x01LLU);

  pos.doMove(Move(Square::s22(), Square::s88(), true), captured);

  ASSERT_TRUE(0x00LLU != pos.getBoardHash());
  ASSERT_TRUE(0x00LLU != pos.getHandHash());
  ASSERT_EQ(0x01LLU, pos.getTurnHash());
  ASSERT_EQ(0x01LLU, pos.getHash() & 0x01LLU);

  pos.doMove(Move(Square::s79(), Square::s88(), false), captured);

  ASSERT_TRUE(0x00LLU != pos.getBoardHash());
  ASSERT_TRUE(0x00LLU != pos.getHandHash());
  ASSERT_EQ(0x00LLU, pos.getTurnHash());
  ASSERT_EQ(0x00LLU, pos.getHash() & 0x01LLU);

  pos.doNullMove();

  ASSERT_TRUE(0x00LLU != pos.getBoardHash());
  ASSERT_TRUE(0x00LLU != pos.getHandHash());
  ASSERT_EQ(0x01LLU, pos.getTurnHash());
  ASSERT_EQ(0x01LLU, pos.getHash() & 0x01LLU);

  pos.doNullMove();

  ASSERT_TRUE(0x00LLU != pos.getBoardHash());
  ASSERT_TRUE(0x00LLU != pos.getHandHash());
  ASSERT_EQ(0x00LLU, pos.getTurnHash());
  ASSERT_EQ(0x00LLU, pos.getHash() & 0x01LLU);
}

#endif // !defined(NDEBUG)
