/* PieceTest.cpp
 *
 * Kubo Ryosuke
 */

#if !defined(NDEBUG)

#include "test/Test.hpp"
#include "core/base/Piece.hpp"

using namespace sunfish;

TEST(PieceTest, testEmpty) {
  Piece empty = Piece::Empty;
  ASSERT_EQ(true, empty.isEmpty());
  ASSERT_EQ(false, empty.exists());

  Piece pawn = Piece::Pawn;
  ASSERT_EQ(false, pawn.isEmpty());
  ASSERT_EQ(true, pawn.exists());
}

TEST(PieceTest, testHand) {
  ASSERT_EQ(Piece::Pawn, Piece(Piece::BPawn).hand());
  ASSERT_EQ(Piece::Pawn, Piece(Piece::BTokin).hand());
  ASSERT_EQ(Piece::Rook, Piece(Piece::BRook).hand());
  ASSERT_EQ(Piece::Rook, Piece(Piece::BDragon).hand());
  ASSERT_EQ(Piece::Pawn, Piece(Piece::WPawn).hand());
  ASSERT_EQ(Piece::Pawn, Piece(Piece::WTokin).hand());
  ASSERT_EQ(Piece::Rook, Piece(Piece::WRook).hand());
  ASSERT_EQ(Piece::Rook, Piece(Piece::WDragon).hand());
  ASSERT_EQ(Piece::Empty, Piece(Piece::Empty).hand());
}

TEST(PieceTest, testPromotion) {
  ASSERT_EQ(Piece::BTokin, Piece(Piece::BPawn).promote());
  ASSERT_EQ(Piece::BProLance, Piece(Piece::BLance).promote());
  ASSERT_EQ(Piece::BProKnight, Piece(Piece::BKnight).promote());
  ASSERT_EQ(Piece::BProSilver, Piece(Piece::BSilver).promote());
  ASSERT_EQ(Piece::BHorse, Piece(Piece::BBishop).promote());
  ASSERT_EQ(Piece::BDragon, Piece(Piece::BRook).promote());
  ASSERT_EQ(Piece::WTokin, Piece(Piece::WPawn).promote());
  ASSERT_EQ(Piece::WProLance, Piece(Piece::WLance).promote());
  ASSERT_EQ(Piece::WProKnight, Piece(Piece::WKnight).promote());
  ASSERT_EQ(Piece::WProSilver, Piece(Piece::WSilver).promote());
  ASSERT_EQ(Piece::WHorse, Piece(Piece::WBishop).promote());
  ASSERT_EQ(Piece::WDragon, Piece(Piece::WRook).promote());

  ASSERT_EQ(Piece::BPawn, Piece(Piece::BTokin).unpromote());
  ASSERT_EQ(Piece::WRook, Piece(Piece::WDragon).unpromote());
}

TEST(PieceTest, testKind) {
  ASSERT_EQ(Piece::Pawn, Piece(Piece::BPawn).kindOnly());
  ASSERT_EQ(Piece::Tokin, Piece(Piece::BTokin).kindOnly());
  ASSERT_EQ(Piece::Pawn, Piece(Piece::WPawn).kindOnly());
  ASSERT_EQ(Piece::Tokin, Piece(Piece::WTokin).kindOnly());
  ASSERT_EQ(Piece::Empty, Piece(Piece::Empty).kindOnly());
}

TEST(PieceTest, testTurn) {
  ASSERT_EQ(Piece::BPawn, Piece(Piece::BPawn).black());
  ASSERT_EQ(Piece::BDragon, Piece(Piece::BDragon).black());
  ASSERT_EQ(Piece::BPawn, Piece(Piece::WPawn).black());
  ASSERT_EQ(Piece::BDragon, Piece(Piece::WDragon).black());
  ASSERT_EQ(Piece::WPawn, Piece(Piece::BPawn).white());
  ASSERT_EQ(Piece::WDragon, Piece(Piece::BDragon).white());
  ASSERT_EQ(Piece::WPawn, Piece(Piece::WPawn).white());
  ASSERT_EQ(Piece::WDragon, Piece(Piece::WDragon).white());
}

TEST(PieceTest, testStatus) {
  ASSERT_EQ(true, Piece(Piece::BPawn).isUnpromoted());
  ASSERT_EQ(true, Piece(Piece::BKing).isUnpromoted());
  ASSERT_EQ(false, Piece(Piece::BTokin).isUnpromoted());
  ASSERT_EQ(false, Piece(Piece::BDragon).isUnpromoted());
  ASSERT_EQ(true, Piece(Piece::WPawn).isUnpromoted());
  ASSERT_EQ(true, Piece(Piece::WKing).isUnpromoted());
  ASSERT_EQ(false, Piece(Piece::WTokin).isUnpromoted());
  ASSERT_EQ(false, Piece(Piece::WDragon).isUnpromoted());

  ASSERT_EQ(false, Piece(Piece::BPawn).isPromoted());
  ASSERT_EQ(false, Piece(Piece::BKing).isPromoted());
  ASSERT_EQ(true, Piece(Piece::BTokin).isPromoted());
  ASSERT_EQ(true, Piece(Piece::BDragon).isPromoted());
  ASSERT_EQ(false, Piece(Piece::WPawn).isPromoted());
  ASSERT_EQ(false, Piece(Piece::WKing).isPromoted());
  ASSERT_EQ(true, Piece(Piece::WTokin).isPromoted());
  ASSERT_EQ(true, Piece(Piece::WDragon).isPromoted());

  ASSERT_EQ(true, Piece(Piece::BPawn).isBlack());
  ASSERT_EQ(true, Piece(Piece::BDragon).isBlack());
  ASSERT_EQ(false, Piece(Piece::WPawn).isBlack());
  ASSERT_EQ(false, Piece(Piece::WDragon).isBlack());

  ASSERT_EQ(false, Piece(Piece::BPawn).isWhite());
  ASSERT_EQ(false, Piece(Piece::BDragon).isWhite());
  ASSERT_EQ(true, Piece(Piece::WPawn).isWhite());
  ASSERT_EQ(true, Piece(Piece::WDragon).isWhite());
}

TEST(PieceTest, testNext) {
  ASSERT_EQ(Piece::Lance, Piece(Piece::Pawn).next());
  ASSERT_EQ(Piece::Horse, Piece(Piece::ProSilver).next());
  ASSERT_EQ(Piece::BLance, Piece(Piece::BPawn).next());
  ASSERT_EQ(Piece::BHorse, Piece(Piece::BProSilver).next());
  ASSERT_EQ(Piece::WPawn, Piece(Piece::BDragon).next());
  ASSERT_EQ(Piece::WLance, Piece(Piece::WPawn).next());
  ASSERT_EQ(Piece::WHorse, Piece(Piece::WProSilver).next());
  ASSERT_EQ(Piece::End, Piece(Piece::WDragon).next());
}

TEST(PieceTest, testStringify) {
  ASSERT_EQ(std::string("+FU"), Piece(Piece::BPawn).toString());
  ASSERT_EQ(std::string("+RY"), Piece(Piece::BDragon).toString());
  ASSERT_EQ(std::string("-FU"), Piece(Piece::WPawn).toString());
  ASSERT_EQ(std::string("-RY"), Piece(Piece::WDragon).toString());

  ASSERT_EQ(std::string("FU"), Piece(Piece::BPawn).toString(true));
  ASSERT_EQ(std::string("RY"), Piece(Piece::BDragon).toString(true));
  ASSERT_EQ(std::string("FU"), Piece(Piece::WPawn).toString(true));
  ASSERT_EQ(std::string("RY"), Piece(Piece::WDragon).toString(true));

  ASSERT_EQ(Piece(Piece::Pawn), Piece::parse("FU"));
  ASSERT_EQ(Piece(Piece::Dragon), Piece::parse("RY"));

  ASSERT_EQ(Piece(Piece::BPawn), Piece::parse("+FU"));
  ASSERT_EQ(Piece(Piece::BDragon), Piece::parse("+RY"));
  ASSERT_EQ(Piece(Piece::WPawn), Piece::parse("-FU"));
  ASSERT_EQ(Piece(Piece::WDragon), Piece::parse("-RY"));
}

#endif // !defined(NDEBUG)
