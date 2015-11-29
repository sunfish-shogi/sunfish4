/* PieceTest.cpp
 *
 * Kubo Ryosuke
 */

#if !defined(NDEBUG)

#include "test/Test.hpp"
#include "core/base/Piece.hpp"
#include <sstream>

using namespace sunfish;

TEST(PieceTest, testEmpty) {
  {
    PieceType empty = PieceType::empty();
    ASSERT_EQ(true, empty.isEmpty());
  }

  {
    PieceType pawn = PieceType::pawn();
    ASSERT_EQ(false, pawn.isEmpty());
  }

  {
    Piece empty = Piece::empty();
    ASSERT_EQ(true, empty.isEmpty());
  }

  {
    Piece blackPawn = Piece::blackPawn();
    ASSERT_EQ(false, blackPawn.isEmpty());
  }
}

TEST(PieceTest, testHand) {
  ASSERT_EQ(PieceType::pawn(), PieceType::pawn().hand());
  ASSERT_EQ(PieceType::pawn(), PieceType::tokin().hand());
  ASSERT_EQ(PieceType::rook(), PieceType::rook().hand());
  ASSERT_EQ(PieceType::rook(), PieceType::dragon().hand());
  ASSERT_EQ(PieceType::empty(), PieceType::empty().hand());

  ASSERT_EQ(PieceType::pawn(), Piece::blackPawn().hand());
  ASSERT_EQ(PieceType::pawn(), Piece::blackTokin().hand());
  ASSERT_EQ(PieceType::rook(), Piece::blackRook().hand());
  ASSERT_EQ(PieceType::rook(), Piece::blackDragon().hand());
  ASSERT_EQ(PieceType::pawn(), Piece::whitePawn().hand());
  ASSERT_EQ(PieceType::pawn(), Piece::whiteTokin().hand());
  ASSERT_EQ(PieceType::rook(), Piece::whiteRook().hand());
  ASSERT_EQ(PieceType::rook(), Piece::whiteDragon().hand());
  ASSERT_EQ(PieceType::empty(), Piece::empty().hand());
}

TEST(PieceTest, testPromotion) {
  ASSERT_EQ(PieceType::tokin(), PieceType::pawn().promote());
  ASSERT_EQ(PieceType::proLance(), PieceType::lance().promote());
  ASSERT_EQ(PieceType::proKnight(), PieceType::knight().promote());
  ASSERT_EQ(PieceType::proSilver(), PieceType::silver().promote());
  ASSERT_EQ(PieceType::horse(), PieceType::bishop().promote());
  ASSERT_EQ(PieceType::dragon(), PieceType::rook().promote());

  ASSERT_EQ(PieceType::pawn(), PieceType::tokin().unpromote());

  ASSERT_EQ(Piece::blackTokin(), Piece::blackPawn().promote());
  ASSERT_EQ(Piece::blackProLance(), Piece::blackLance().promote());
  ASSERT_EQ(Piece::blackProKnight(), Piece::blackKnight().promote());
  ASSERT_EQ(Piece::blackProSilver(), Piece::blackSilver().promote());
  ASSERT_EQ(Piece::blackHorse(), Piece::blackBishop().promote());
  ASSERT_EQ(Piece::blackDragon(), Piece::blackRook().promote());
  ASSERT_EQ(Piece::whiteTokin(), Piece::whitePawn().promote());
  ASSERT_EQ(Piece::whiteProLance(), Piece::whiteLance().promote());
  ASSERT_EQ(Piece::whiteProKnight(), Piece::whiteKnight().promote());
  ASSERT_EQ(Piece::whiteProSilver(), Piece::whiteSilver().promote());
  ASSERT_EQ(Piece::whiteHorse(), Piece::whiteBishop().promote());
  ASSERT_EQ(Piece::whiteDragon(), Piece::whiteRook().promote());

  ASSERT_EQ(Piece::blackPawn(), Piece::blackTokin().unpromote());
  ASSERT_EQ(Piece::whiteRook(), Piece::whiteDragon().unpromote());
}

TEST(PieceTest, testType) {
  ASSERT_EQ(PieceType::pawn(), Piece::blackPawn().type());
  ASSERT_EQ(PieceType::tokin(), Piece::blackTokin().type());
  ASSERT_EQ(PieceType::pawn(), Piece::whitePawn().type());
  ASSERT_EQ(PieceType::tokin(), Piece::whiteTokin().type());
  ASSERT_EQ(PieceType::empty(), Piece::empty().type());
}

TEST(PieceTest, testTurn) {
  ASSERT_EQ(Piece::blackPawn(), Piece::blackPawn().black());
  ASSERT_EQ(Piece::blackDragon(), Piece::blackDragon().black());
  ASSERT_EQ(Piece::blackPawn(), Piece::whitePawn().black());
  ASSERT_EQ(Piece::blackDragon(), Piece::whiteDragon().black());
  ASSERT_EQ(Piece::whitePawn(), Piece::blackPawn().white());
  ASSERT_EQ(Piece::whiteDragon(), Piece::blackDragon().white());
  ASSERT_EQ(Piece::whitePawn(), Piece::whitePawn().white());
  ASSERT_EQ(Piece::whiteDragon(), Piece::whiteDragon().white());
}

TEST(PieceTest, testStatus) {
  ASSERT_EQ(true, PieceType::pawn().isUnpromoted());
  ASSERT_EQ(true, PieceType::king().isUnpromoted());
  ASSERT_EQ(false, PieceType::tokin().isUnpromoted());
  ASSERT_EQ(false, PieceType::dragon().isUnpromoted());

  ASSERT_EQ(false, PieceType::pawn().isPromoted());
  ASSERT_EQ(false, PieceType::king().isPromoted());
  ASSERT_EQ(true, PieceType::tokin().isPromoted());
  ASSERT_EQ(true, PieceType::dragon().isPromoted());

  ASSERT_EQ(true, Piece::blackPawn().isUnpromoted());
  ASSERT_EQ(true, Piece::blackKing().isUnpromoted());
  ASSERT_EQ(false, Piece::blackTokin().isUnpromoted());
  ASSERT_EQ(false, Piece::blackDragon().isUnpromoted());
  ASSERT_EQ(true, Piece::whitePawn().isUnpromoted());
  ASSERT_EQ(true, Piece::whiteKing().isUnpromoted());
  ASSERT_EQ(false, Piece::whiteTokin().isUnpromoted());
  ASSERT_EQ(false, Piece::whiteDragon().isUnpromoted());

  ASSERT_EQ(false, Piece::blackPawn().isPromoted());
  ASSERT_EQ(false, Piece::blackKing().isPromoted());
  ASSERT_EQ(true, Piece::blackTokin().isPromoted());
  ASSERT_EQ(true, Piece::blackDragon().isPromoted());
  ASSERT_EQ(false, Piece::whitePawn().isPromoted());
  ASSERT_EQ(false, Piece::whiteKing().isPromoted());
  ASSERT_EQ(true, Piece::whiteTokin().isPromoted());
  ASSERT_EQ(true, Piece::whiteDragon().isPromoted());

  ASSERT_EQ(true, Piece::blackPawn().isBlack());
  ASSERT_EQ(true, Piece::blackDragon().isBlack());
  ASSERT_EQ(false, Piece::whitePawn().isBlack());
  ASSERT_EQ(false, Piece::whiteDragon().isBlack());

  ASSERT_EQ(false, Piece::blackPawn().isWhite());
  ASSERT_EQ(false, Piece::blackDragon().isWhite());
  ASSERT_EQ(true, Piece::whitePawn().isWhite());
  ASSERT_EQ(true, Piece::whiteDragon().isWhite());
}

TEST(PieceTest, testNext) {
  ASSERT_EQ(PieceType::lance(), PieceType::pawn().next());
  ASSERT_EQ(PieceType::horse(), PieceType::proSilver().next());

  ASSERT_EQ(Piece::blackLance(), Piece::blackPawn().next());
  ASSERT_EQ(Piece::blackHorse(), Piece::blackProSilver().next());
  ASSERT_EQ(Piece::whitePawn(), Piece::blackDragon().next());
  ASSERT_EQ(Piece::whiteLance(), Piece::whitePawn().next());
  ASSERT_EQ(Piece::whiteHorse(), Piece::whiteProSilver().next());

  ASSERT_EQ(PieceNumber::End, Piece::whiteDragon().next().raw());
}

TEST(PieceTest, testStringify) {
  ASSERT_EQ(std::string("+FU"), Piece::blackPawn().toString());
  ASSERT_EQ(std::string("+RY"), Piece::blackDragon().toString());
  ASSERT_EQ(std::string("-FU"), Piece::whitePawn().toString());
  ASSERT_EQ(std::string("-RY"), Piece::whiteDragon().toString());

  ASSERT_EQ(std::string("FU"), PieceType::pawn().toString());
  ASSERT_EQ(std::string("RY"), PieceType::dragon().toString());

  ASSERT_EQ(PieceType::pawn(), PieceType::parse("FU"));
  ASSERT_EQ(PieceType::dragon(), PieceType::parse("RY"));

  ASSERT_EQ(Piece::blackPawn(), Piece::parse("+FU"));
  ASSERT_EQ(Piece::blackDragon(), Piece::parse("+RY"));
  ASSERT_EQ(Piece::whitePawn(), Piece::parse("-FU"));
  ASSERT_EQ(Piece::whiteDragon(), Piece::parse("-RY"));
}

TEST(PieceTest, testConversion) {
  ASSERT_EQ(Piece::blackPawn(), PieceType::pawn().black());
  ASSERT_EQ(Piece::whitePawn(), PieceType::pawn().white());

  ASSERT_EQ(PieceType::pawn(), Piece::whitePawn().type());
  ASSERT_EQ(PieceType::dragon(), Piece::blackDragon().type());
}

TEST(PieceTest, testToString) {
  {
    ASSERT_EQ("FU", PieceType::pawn().toString());
    ASSERT_EQ("RY", PieceType::dragon().toString());
    ASSERT_EQ("102", PieceType(102).toString());
  }

  {
    ASSERT_EQ("+FU", Piece::blackPawn().toString());
    ASSERT_EQ("-RY", Piece::whiteDragon().toString());
    ASSERT_EQ("102", Piece(102).toString());
  }

  {
    std::ostringstream oss;
    oss << PieceType::dragon();
    ASSERT_EQ("RY", oss.str());
  }

  {
    std::ostringstream oss;
    oss << Piece::whiteSilver();
    ASSERT_EQ("-GI", oss.str());
  }
}

TEST(PieceTest, testParse) {
  ASSERT_EQ(PieceType::pawn(), PieceType::parse("FU"));
  ASSERT_EQ(PieceType::dragon(), PieceType::parse("RY"));
  ASSERT_EQ(PieceType::empty(), PieceType::parse("hoge"));

  ASSERT_EQ(Piece::blackPawn(), Piece::parse("+FU"));
  ASSERT_EQ(Piece::whiteDragon(), Piece::parse("-RY"));
  ASSERT_EQ(Piece::empty(), Piece::parse("hoge"));
}

TEST(PieceTest, testToStringSFEN) {
  {
    ASSERT_EQ("P", Piece::blackPawn().toStringSFEN());
    ASSERT_EQ("+r", Piece::whiteDragon().toStringSFEN());
    ASSERT_EQ("102", Piece(102).toStringSFEN());
  }
}

TEST(PieceTest, testParseSFEN) {
  ASSERT_EQ(Piece::blackPawn(), Piece::parseSFEN("P"));
  ASSERT_EQ(Piece::whiteDragon(), Piece::parseSFEN("+r"));
  ASSERT_EQ(Piece::empty(), Piece::parseSFEN("hoge"));
}

#endif // !defined(NDEBUG)
