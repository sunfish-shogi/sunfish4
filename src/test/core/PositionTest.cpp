/* PositionTest.cpp
 *
 * Kubo Ryosuke
 */

#if !defined(NDEBUG)

#include "test/Test.hpp"
#include "core/position/Position.hpp"
#include "core/record/CsaReader.hpp"

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

TEST(PositionTest, testToString) {
  {
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

  // TODO: hand, promoted piece, white turn
}

TEST(PositionTest, testMakeMove) {
  {
    std::string src =
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
      "+\n";
    std::istringstream iss(src);
    Position pos;
    CsaReader::readPosition(iss, pos);

    Move move(Piece::blackPawn(), Square::s77(), Square::s76(), false);

    pos.makeMove(move);

    ASSERT_EQ(Piece::empty(), pos.getPieceOnBoard(Square::s77()));
    ASSERT_EQ(false, pos.getBPawnBitboard().check(Square::s77()));
    ASSERT_EQ(false, pos.getBOccupiedBitboard().check(Square::s77()));
    ASSERT_EQ(false, pos.getWOccupiedBitboard().check(Square::s77()));
    ASSERT_EQ(false, pos.get90RotatedBitboard().check(Square::s77().rotate90()));
    ASSERT_EQ(false, pos.getRight45RotatedBitboard().check(Square::s77().rotateRight45()));
    ASSERT_EQ(false, pos.getLeft45RotatedBitboard().check(Square::s77().rotateLeft45()));

    ASSERT_EQ(Piece::blackPawn(), pos.getPieceOnBoard(Square::s76()));
    ASSERT_EQ(true, pos.getBPawnBitboard().check(Square::s76()));
    ASSERT_EQ(true, pos.getBOccupiedBitboard().check(Square::s76()));
    ASSERT_EQ(false, pos.getWOccupiedBitboard().check(Square::s76()));
    ASSERT_EQ(true, pos.get90RotatedBitboard().check(Square::s76().rotate90()));
    ASSERT_EQ(true, pos.getRight45RotatedBitboard().check(Square::s76().rotateRight45()));
    ASSERT_EQ(true, pos.getLeft45RotatedBitboard().check(Square::s76().rotateLeft45()));

    ASSERT_EQ(Piece::empty(), move.capturedPiece());

    ASSERT_EQ(
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
      "-\n", pos.toString());
  }

  {
    std::string src =
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
      "-\n";
    std::istringstream iss(src);
    Position pos;
    CsaReader::readPosition(iss, pos);

    Move move(Piece::whitePawn(), Square::s33(), Square::s34(), false);

    pos.makeMove(move);

    ASSERT_EQ(Piece::empty(), pos.getPieceOnBoard(Square::s33()));
    ASSERT_EQ(false, pos.getWPawnBitboard().check(Square::s33()));
    ASSERT_EQ(false, pos.getBOccupiedBitboard().check(Square::s33()));
    ASSERT_EQ(false, pos.getWOccupiedBitboard().check(Square::s33()));
    ASSERT_EQ(false, pos.get90RotatedBitboard().check(Square::s33().rotate90()));
    ASSERT_EQ(false, pos.getRight45RotatedBitboard().check(Square::s33().rotateRight45()));
    ASSERT_EQ(false, pos.getLeft45RotatedBitboard().check(Square::s33().rotateLeft45()));

    ASSERT_EQ(Piece::whitePawn(), pos.getPieceOnBoard(Square::s34()));
    ASSERT_EQ(true, pos.getWPawnBitboard().check(Square::s34()));
    ASSERT_EQ(false, pos.getBOccupiedBitboard().check(Square::s34()));
    ASSERT_EQ(true, pos.getWOccupiedBitboard().check(Square::s34()));
    ASSERT_EQ(true, pos.get90RotatedBitboard().check(Square::s34().rotate90()));
    ASSERT_EQ(true, pos.getRight45RotatedBitboard().check(Square::s34().rotateRight45()));
    ASSERT_EQ(true, pos.getLeft45RotatedBitboard().check(Square::s34().rotateLeft45()));

    ASSERT_EQ(Piece::empty(), move.capturedPiece());

    ASSERT_EQ(
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
      "+\n", pos.toString());
  }

  {
    std::string src =
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
      "+\n";
    std::istringstream iss(src);
    Position pos;
    CsaReader::readPosition(iss, pos);

    Move move(Piece::blackBishop(), Square::s88(), Square::s22(), true);

    pos.makeMove(move);

    ASSERT_EQ(Piece::empty(), pos.getPieceOnBoard(Square::s88()));
    ASSERT_EQ(false, pos.getBBishopBitboard().check(Square::s88()));
    ASSERT_EQ(false, pos.getBHorseBitboard().check(Square::s88()));
    ASSERT_EQ(false, pos.getBOccupiedBitboard().check(Square::s88()));
    ASSERT_EQ(false, pos.getWOccupiedBitboard().check(Square::s88()));
    ASSERT_EQ(false, pos.get90RotatedBitboard().check(Square::s88().rotate90()));
    ASSERT_EQ(false, pos.getRight45RotatedBitboard().check(Square::s88().rotateRight45()));
    ASSERT_EQ(false, pos.getLeft45RotatedBitboard().check(Square::s88().rotateLeft45()));

    ASSERT_EQ(Piece::blackHorse(), pos.getPieceOnBoard(Square::s22()));
    ASSERT_EQ(false, pos.getBBishopBitboard().check(Square::s22()));
    ASSERT_EQ(true, pos.getBHorseBitboard().check(Square::s22()));
    ASSERT_EQ(true, pos.getBOccupiedBitboard().check(Square::s22()));
    ASSERT_EQ(false, pos.getWOccupiedBitboard().check(Square::s22()));
    ASSERT_EQ(true, pos.get90RotatedBitboard().check(Square::s22().rotate90()));
    ASSERT_EQ(true, pos.getRight45RotatedBitboard().check(Square::s22().rotateRight45()));
    ASSERT_EQ(true, pos.getLeft45RotatedBitboard().check(Square::s22().rotateLeft45()));

    ASSERT_EQ(Piece::whiteBishop(), move.capturedPiece());

    ASSERT_EQ(
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
      "-\n", pos.toString());
  }

  {
    std::string src =
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
      "-\n";
    std::istringstream iss(src);
    Position pos;
    CsaReader::readPosition(iss, pos);

    Move move(Piece::whiteSilver(), Square::s31(), Square::s22(), false);

    pos.makeMove(move);

    ASSERT_EQ(Piece::empty(), pos.getPieceOnBoard(Square::s31()));
    ASSERT_EQ(false, pos.getWSilverBitboard().check(Square::s31()));
    ASSERT_EQ(false, pos.getBOccupiedBitboard().check(Square::s31()));
    ASSERT_EQ(false, pos.getWOccupiedBitboard().check(Square::s31()));
    ASSERT_EQ(false, pos.get90RotatedBitboard().check(Square::s31().rotate90()));
    ASSERT_EQ(false, pos.getRight45RotatedBitboard().check(Square::s31().rotateRight45()));
    ASSERT_EQ(false, pos.getLeft45RotatedBitboard().check(Square::s31().rotateLeft45()));

    ASSERT_EQ(Piece::whiteSilver(), pos.getPieceOnBoard(Square::s22()));
    ASSERT_EQ(false, pos.getBHorseBitboard().check(Square::s22()));
    ASSERT_EQ(true, pos.getWSilverBitboard().check(Square::s22()));
    ASSERT_EQ(false, pos.getBOccupiedBitboard().check(Square::s22()));
    ASSERT_EQ(true, pos.getWOccupiedBitboard().check(Square::s22()));
    ASSERT_EQ(true, pos.get90RotatedBitboard().check(Square::s22().rotate90()));
    ASSERT_EQ(true, pos.getRight45RotatedBitboard().check(Square::s22().rotateRight45()));
    ASSERT_EQ(true, pos.getLeft45RotatedBitboard().check(Square::s22().rotateLeft45()));

    ASSERT_EQ(Piece::blackHorse(), move.capturedPiece());

    ASSERT_EQ(
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
      "+\n", pos.toString());
  }

  {
    std::string src =
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
      "+\n";
    std::istringstream iss(src);
    Position pos;
    CsaReader::readPosition(iss, pos);

    Move move(Piece::blackBishop(), Square::s45());

    pos.makeMove(move);

    ASSERT_EQ(Piece::blackBishop(), pos.getPieceOnBoard(Square::s45()));
    ASSERT_EQ(true, pos.getBBishopBitboard().check(Square::s45()));
    ASSERT_EQ(true, pos.getBOccupiedBitboard().check(Square::s45()));
    ASSERT_EQ(false, pos.getWOccupiedBitboard().check(Square::s45()));
    ASSERT_EQ(true, pos.get90RotatedBitboard().check(Square::s45().rotate90()));
    ASSERT_EQ(true, pos.getRight45RotatedBitboard().check(Square::s45().rotateRight45()));
    ASSERT_EQ(true, pos.getLeft45RotatedBitboard().check(Square::s45().rotateLeft45()));

    ASSERT_EQ(Piece::empty(), move.capturedPiece());

    ASSERT_EQ(
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
      "-\n", pos.toString());
  }
}

#endif // !defined(NDEBUG)
