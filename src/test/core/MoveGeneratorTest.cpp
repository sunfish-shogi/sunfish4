/* MoveGeneratorTest.cpp
 *
 * Kubo Ryosuke
 */

#if !defined(NDEBUG)

#include "test/Test.hpp"
#include "core/move/MoveGenerator.hpp"
#include "core/util/PositionUtil.hpp"

using namespace sunfish;

namespace {

void sortMoves(Moves& moves) {
  std::sort(moves.begin(), moves.end(), [](const Move& lhs, const Move& rhs) {
    if (lhs.isDrop() != rhs.isDrop()) {
      return rhs.isDrop();
    }

    if (lhs.piece() != rhs.piece()) {
      return lhs.piece().raw() < rhs.piece().raw();
    }

    if (lhs.from().getFile() != rhs.from().getFile()) {
      return lhs.from().getFile() < rhs.from().getFile();
    }

    if (lhs.from().getRank() != rhs.from().getRank()) {
      return lhs.from().getRank() < rhs.from().getRank();
    }

    if (lhs.to().getFile() != rhs.to().getFile()) {
      return lhs.to().getFile() < rhs.to().getFile();
    }

    if (lhs.to().getRank() != rhs.to().getRank()) {
      return lhs.to().getRank() < rhs.to().getRank();
    }

    if (lhs.isPromotion() != rhs.isPromotion()) {
      return lhs.isPromotion();
    }

    return true;
  });
}

#if 0
void debugPrint(const Moves& moves) {
  for (const auto& move : moves) {
    Loggers::message << ' ' << move.toString();
  }
}
#endif

}

TEST(MoveGeneratorTest, test) {
  {
    // black pawn
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  * +FU\n"
      "P3 *  *  *  *  *  *  * +FU * \n"
      "P4 *  *  *  *  *  * +FU *  * \n"
      "P5 *  *  *  *  * +FU *  *  * \n"
      "P6 *  *  * -KE+FU-KE *  *  * \n"
      "P7 *  *  * +FU *  *  *  *  * \n"
      "P8 *  * +FU *  *  *  *  *  * \n"
      "P9 * +FU *  * +OU *  *  *  * \n"
      "P+\n"
      "P-\n"
      "+\n");

    Moves caps;
    MoveGenerator::generateCapturingMoves(pos, caps);
    sortMoves(caps);
    ASSERT_EQ(4, caps.size());
    ASSERT_EQ(Move(Piece::blackPawn(), Square::s12(), Square::s11(), true), caps[0]);
    ASSERT_EQ(Move(Piece::blackPawn(), Square::s23(), Square::s22(), true), caps[1]);
    ASSERT_EQ(Move(Piece::blackPawn(), Square::s34(), Square::s33(), true), caps[2]);
    ASSERT_EQ(Move(Piece::blackPawn(), Square::s67(), Square::s66(), false), caps[3]);

    Moves nocaps;
    MoveGenerator::generateNotCapturingMoves(pos, nocaps);
    sortMoves(nocaps);
    ASSERT_EQ(9, nocaps.size());
    ASSERT_EQ(Move(Piece::blackPawn(), Square::s45(), Square::s44(), false), nocaps[0]);
    ASSERT_EQ(Move(Piece::blackPawn(), Square::s56(), Square::s55(), false), nocaps[1]);
    ASSERT_EQ(Move(Piece::blackPawn(), Square::s78(), Square::s77(), false), nocaps[2]);
    ASSERT_EQ(Move(Piece::blackPawn(), Square::s89(), Square::s88(), false), nocaps[3]);
    ASSERT_EQ(Move(Piece::blackKing(), Square::s59(), Square::s48(), false), nocaps[4]);
    ASSERT_EQ(Move(Piece::blackKing(), Square::s59(), Square::s49(), false), nocaps[5]);
    ASSERT_EQ(Move(Piece::blackKing(), Square::s59(), Square::s58(), false), nocaps[6]);
    ASSERT_EQ(Move(Piece::blackKing(), Square::s59(), Square::s68(), false), nocaps[7]);
    ASSERT_EQ(Move(Piece::blackKing(), Square::s59(), Square::s69(), false), nocaps[8]);
  }

  {
    // white pawn
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 * -FU *  * -OU *  *  *  * \n"
      "P2 *  * -FU *  *  *  *  *  * \n"
      "P3 *  *  * -FU *  *  *  *  * \n"
      "P4 *  *  * +KE-FU+KE *  *  * \n"
      "P5 *  *  *  *  * -FU *  *  * \n"
      "P6 *  *  *  *  *  * -FU *  * \n"
      "P7 *  *  *  *  *  *  * -FU * \n"
      "P8 *  *  *  *  *  *  *  * -FU\n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+\n"
      "P-\n"
      "-\n");

    Moves caps;
    MoveGenerator::generateCapturingMoves(pos, caps);
    sortMoves(caps);
    ASSERT_EQ(4, caps.size());
    ASSERT_EQ(Move(Piece::whitePawn(), Square::s18(), Square::s19(), true), caps[0]);
    ASSERT_EQ(Move(Piece::whitePawn(), Square::s27(), Square::s28(), true), caps[1]);
    ASSERT_EQ(Move(Piece::whitePawn(), Square::s36(), Square::s37(), true), caps[2]);
    ASSERT_EQ(Move(Piece::whitePawn(), Square::s63(), Square::s64(), false), caps[3]);

    Moves nocaps;
    MoveGenerator::generateNotCapturingMoves(pos, nocaps);
    sortMoves(nocaps);
    ASSERT_EQ(9, nocaps.size());
    ASSERT_EQ(Move(Piece::whitePawn(), Square::s45(), Square::s46(), false), nocaps[0]);
    ASSERT_EQ(Move(Piece::whitePawn(), Square::s54(), Square::s55(), false), nocaps[1]);
    ASSERT_EQ(Move(Piece::whitePawn(), Square::s72(), Square::s73(), false), nocaps[2]);
    ASSERT_EQ(Move(Piece::whitePawn(), Square::s81(), Square::s82(), false), nocaps[3]);
    ASSERT_EQ(Move(Piece::whiteKing(), Square::s51(), Square::s41(), false), nocaps[4]);
    ASSERT_EQ(Move(Piece::whiteKing(), Square::s51(), Square::s42(), false), nocaps[5]);
    ASSERT_EQ(Move(Piece::whiteKing(), Square::s51(), Square::s52(), false), nocaps[6]);
    ASSERT_EQ(Move(Piece::whiteKing(), Square::s51(), Square::s61(), false), nocaps[7]);
    ASSERT_EQ(Move(Piece::whiteKing(), Square::s51(), Square::s62(), false), nocaps[8]);
  }

  {
    // black knight
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  * -FU *  *  *  *  * \n"
      "P4 *  *  *  * -FU * +KE *  * \n"
      "P5 *  *  *  * +KE *  *  *  * \n"
      "P6 *  *  *  *  * +KE *  *  * \n"
      "P7 *  *  *  *  *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+\n"
      "P-\n"
      "+\n");

    Moves caps;
    MoveGenerator::generateCapturingMoves(pos, caps);
    sortMoves(caps);
    ASSERT_EQ(6, caps.size());
    ASSERT_EQ(Move(Piece::blackKnight(), Square::s34(), Square::s22(), true),  caps[0]);
    ASSERT_EQ(Move(Piece::blackKnight(), Square::s34(), Square::s42(), true),  caps[1]);
    ASSERT_EQ(Move(Piece::blackKnight(), Square::s46(), Square::s54(), false), caps[2]);
    ASSERT_EQ(Move(Piece::blackKnight(), Square::s55(), Square::s43(), true),  caps[3]);
    ASSERT_EQ(Move(Piece::blackKnight(), Square::s55(), Square::s63(), true),  caps[4]);
    ASSERT_EQ(Move(Piece::blackKnight(), Square::s55(), Square::s63(), false), caps[5]);

    Moves nocaps;
    MoveGenerator::generateNotCapturingMoves(pos, nocaps);
    sortMoves(nocaps);
    ASSERT_EQ(6, nocaps.size());
    ASSERT_EQ(Move(Piece::blackKnight(), Square::s55(), Square::s43(), false), nocaps[0]);
    ASSERT_EQ(Move(Piece::blackKing(), Square::s59(), Square::s48(), false),   nocaps[1]);
    ASSERT_EQ(Move(Piece::blackKing(), Square::s59(), Square::s49(), false),   nocaps[2]);
    ASSERT_EQ(Move(Piece::blackKing(), Square::s59(), Square::s58(), false),   nocaps[3]);
    ASSERT_EQ(Move(Piece::blackKing(), Square::s59(), Square::s68(), false),   nocaps[4]);
    ASSERT_EQ(Move(Piece::blackKing(), Square::s59(), Square::s69(), false),   nocaps[5]);
  }

  {
    // white knight
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  * -KE *  *  * \n"
      "P5 *  *  *  * -KE *  *  *  * \n"
      "P6 *  *  *  * +FU * -KE *  * \n"
      "P7 *  *  * +FU *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+\n"
      "P-\n"
      "-\n");

    Moves caps;
    MoveGenerator::generateCapturingMoves(pos, caps);
    sortMoves(caps);
    ASSERT_EQ(6, caps.size());
    ASSERT_EQ(Move(Piece::whiteKnight(), Square::s36(), Square::s28(), true),  caps[0]);
    ASSERT_EQ(Move(Piece::whiteKnight(), Square::s36(), Square::s48(), true),  caps[1]);
    ASSERT_EQ(Move(Piece::whiteKnight(), Square::s44(), Square::s56(), false), caps[2]);
    ASSERT_EQ(Move(Piece::whiteKnight(), Square::s55(), Square::s47(), true),  caps[3]);
    ASSERT_EQ(Move(Piece::whiteKnight(), Square::s55(), Square::s67(), true),  caps[4]);
    ASSERT_EQ(Move(Piece::whiteKnight(), Square::s55(), Square::s67(), false), caps[5]);

    Moves nocaps;
    MoveGenerator::generateNotCapturingMoves(pos, nocaps);
    sortMoves(nocaps);
    ASSERT_EQ(6, nocaps.size());
    ASSERT_EQ(Move(Piece::whiteKnight(), Square::s55(), Square::s47(), false), nocaps[0]);
    ASSERT_EQ(Move(Piece::whiteKing(),   Square::s51(), Square::s41(), false), nocaps[1]);
    ASSERT_EQ(Move(Piece::whiteKing(),   Square::s51(), Square::s42(), false), nocaps[2]);
    ASSERT_EQ(Move(Piece::whiteKing(),   Square::s51(), Square::s52(), false), nocaps[3]);
    ASSERT_EQ(Move(Piece::whiteKing(),   Square::s51(), Square::s61(), false), nocaps[4]);
    ASSERT_EQ(Move(Piece::whiteKing(),   Square::s51(), Square::s62(), false), nocaps[5]);
  }

  {
    // black moves
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1+TO *  *  * -OU *  *  *  * \n"
      "P2 *  * +GI *  *  *  * +KI-FU\n"
      "P3+UM-FU * +HI *  * +GI *  * \n"
      "P4 *  *  *  *  * -FU *  *  * \n"
      "P5 *  * -KI *  *  * +KA * +KY\n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  * +FU+KE *  *  *  * \n"
      "P8 * +KY *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+\n"
      "P-\n"
      "+\n");

    Moves caps;
    MoveGenerator::generateCapturingMoves(pos, caps);
    sortMoves(caps);
    ASSERT_EQ(28, caps.size());
    ASSERT_EQ(Move(Piece::blackLance(),  Square::s15(), Square::s12(), true),  caps[0]);
    ASSERT_EQ(Move(Piece::blackLance(),  Square::s15(), Square::s13(), true),  caps[1]);
    ASSERT_EQ(Move(Piece::blackLance(),  Square::s88(), Square::s83(), true),  caps[2]);
    ASSERT_EQ(Move(Piece::blackLance(),  Square::s88(), Square::s83(), false), caps[3]);
    ASSERT_EQ(Move(Piece::blackSilver(), Square::s33(), Square::s24(), true),  caps[4]);
    ASSERT_EQ(Move(Piece::blackSilver(), Square::s33(), Square::s32(), true),  caps[5]);
    ASSERT_EQ(Move(Piece::blackSilver(), Square::s33(), Square::s42(), true),  caps[6]);
    ASSERT_EQ(Move(Piece::blackSilver(), Square::s33(), Square::s44(), true),  caps[7]);
    ASSERT_EQ(Move(Piece::blackSilver(), Square::s33(), Square::s44(), false), caps[8]);
    ASSERT_EQ(Move(Piece::blackSilver(), Square::s72(), Square::s61(), true),  caps[9]);
    ASSERT_EQ(Move(Piece::blackSilver(), Square::s72(), Square::s71(), true),  caps[10]);
    ASSERT_EQ(Move(Piece::blackSilver(), Square::s72(), Square::s81(), true),  caps[11]);
    ASSERT_EQ(Move(Piece::blackSilver(), Square::s72(), Square::s83(), true),  caps[12]);
    ASSERT_EQ(Move(Piece::blackSilver(), Square::s72(), Square::s83(), false), caps[13]);
    ASSERT_EQ(Move(Piece::blackGold(),   Square::s22(), Square::s12(), false), caps[14]);
    ASSERT_EQ(Move(Piece::blackBishop(), Square::s35(), Square::s13(), true),  caps[15]);
    ASSERT_EQ(Move(Piece::blackBishop(), Square::s35(), Square::s44(), false), caps[16]);
    ASSERT_EQ(Move(Piece::blackRook(),   Square::s63(), Square::s43(), true),  caps[17]);
    ASSERT_EQ(Move(Piece::blackRook(),   Square::s63(), Square::s53(), true),  caps[18]);
    ASSERT_EQ(Move(Piece::blackRook(),   Square::s63(), Square::s61(), true),  caps[19]);
    ASSERT_EQ(Move(Piece::blackRook(),   Square::s63(), Square::s62(), true),  caps[20]);
    ASSERT_EQ(Move(Piece::blackRook(),   Square::s63(), Square::s64(), true),  caps[21]);
    ASSERT_EQ(Move(Piece::blackRook(),   Square::s63(), Square::s65(), true),  caps[22]);
    ASSERT_EQ(Move(Piece::blackRook(),   Square::s63(), Square::s66(), true),  caps[23]);
    ASSERT_EQ(Move(Piece::blackRook(),   Square::s63(), Square::s73(), true),  caps[24]);
    ASSERT_EQ(Move(Piece::blackRook(),   Square::s63(), Square::s83(), true),  caps[25]);
    ASSERT_EQ(Move(Piece::blackHorse(),  Square::s93(), Square::s75(), false), caps[26]);
    ASSERT_EQ(Move(Piece::blackHorse(),  Square::s93(), Square::s83(), false), caps[27]);

    Moves nocaps;
    MoveGenerator::generateNotCapturingMoves(pos, nocaps);
    sortMoves(nocaps);
    ASSERT_EQ(36, nocaps.size());
    ASSERT_EQ(Move(Piece::blackPawn(),   Square::s67(), Square::s66(), false), nocaps[0]);
    ASSERT_EQ(Move(Piece::blackLance(),  Square::s15(), Square::s13(), false), nocaps[1]);
    ASSERT_EQ(Move(Piece::blackLance(),  Square::s15(), Square::s14(), false), nocaps[2]);
    ASSERT_EQ(Move(Piece::blackLance(),  Square::s88(), Square::s84(), false), nocaps[3]);
    ASSERT_EQ(Move(Piece::blackLance(),  Square::s88(), Square::s85(), false), nocaps[4]);
    ASSERT_EQ(Move(Piece::blackLance(),  Square::s88(), Square::s86(), false), nocaps[5]);
    ASSERT_EQ(Move(Piece::blackLance(),  Square::s88(), Square::s87(), false), nocaps[6]);
    ASSERT_EQ(Move(Piece::blackKnight(), Square::s57(), Square::s45(), false), nocaps[7]);
    ASSERT_EQ(Move(Piece::blackKnight(), Square::s57(), Square::s65(), false), nocaps[8]);
    ASSERT_EQ(Move(Piece::blackSilver(), Square::s33(), Square::s24(), false), nocaps[9]);
    ASSERT_EQ(Move(Piece::blackSilver(), Square::s33(), Square::s32(), false), nocaps[10]);
    ASSERT_EQ(Move(Piece::blackSilver(), Square::s33(), Square::s42(), false), nocaps[11]);
    ASSERT_EQ(Move(Piece::blackSilver(), Square::s72(), Square::s61(), false), nocaps[12]);
    ASSERT_EQ(Move(Piece::blackSilver(), Square::s72(), Square::s71(), false), nocaps[13]);
    ASSERT_EQ(Move(Piece::blackSilver(), Square::s72(), Square::s81(), false), nocaps[14]);
    ASSERT_EQ(Move(Piece::blackGold(),   Square::s22(), Square::s11(), false), nocaps[15]);
    ASSERT_EQ(Move(Piece::blackGold(),   Square::s22(), Square::s21(), false), nocaps[16]);
    ASSERT_EQ(Move(Piece::blackGold(),   Square::s22(), Square::s23(), false), nocaps[17]);
    ASSERT_EQ(Move(Piece::blackGold(),   Square::s22(), Square::s31(), false), nocaps[18]);
    ASSERT_EQ(Move(Piece::blackGold(),   Square::s22(), Square::s32(), false), nocaps[19]);
    ASSERT_EQ(Move(Piece::blackBishop(), Square::s35(), Square::s17(), false), nocaps[20]);
    ASSERT_EQ(Move(Piece::blackBishop(), Square::s35(), Square::s24(), false), nocaps[21]);
    ASSERT_EQ(Move(Piece::blackBishop(), Square::s35(), Square::s26(), false), nocaps[22]);
    ASSERT_EQ(Move(Piece::blackBishop(), Square::s35(), Square::s46(), false), nocaps[23]);
    ASSERT_EQ(Move(Piece::blackKing(),   Square::s59(), Square::s48(), false), nocaps[24]);
    ASSERT_EQ(Move(Piece::blackKing(),   Square::s59(), Square::s49(), false), nocaps[25]);
    ASSERT_EQ(Move(Piece::blackKing(),   Square::s59(), Square::s58(), false), nocaps[26]);
    ASSERT_EQ(Move(Piece::blackKing(),   Square::s59(), Square::s68(), false), nocaps[27]);
    ASSERT_EQ(Move(Piece::blackKing(),   Square::s59(), Square::s69(), false), nocaps[28]);
    ASSERT_EQ(Move(Piece::blackTokin(),  Square::s91(), Square::s81(), false), nocaps[29]);
    ASSERT_EQ(Move(Piece::blackTokin(),  Square::s91(), Square::s92(), false), nocaps[30]);
    ASSERT_EQ(Move(Piece::blackHorse(),  Square::s93(), Square::s71(), false), nocaps[31]);
    ASSERT_EQ(Move(Piece::blackHorse(),  Square::s93(), Square::s82(), false), nocaps[32]);
    ASSERT_EQ(Move(Piece::blackHorse(),  Square::s93(), Square::s84(), false), nocaps[33]);
    ASSERT_EQ(Move(Piece::blackHorse(),  Square::s93(), Square::s92(), false), nocaps[34]);
    ASSERT_EQ(Move(Piece::blackHorse(),  Square::s93(), Square::s94(), false), nocaps[35]);
  }

  {
    // white moves
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 * -KY *  *  *  *  *  *  * \n"
      "P3 *  *  * -FU-KE *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  * +KI *  *  * -KA * -KY\n"
      "P6 *  *  *  *  * +FU *  *  * \n"
      "P7-UM+FU * -HI *  * -GI *  * \n"
      "P8 *  * -GI *  *  *  * -KI+FU\n"
      "P9-TO *  *  * +OU *  *  *  * \n"
      "P+\n"
      "P-\n"
      "-\n");

    Moves caps;
    MoveGenerator::generateCapturingMoves(pos, caps);
    sortMoves(caps);
    ASSERT_EQ(28, caps.size());
    ASSERT_EQ(Move(Piece::whiteLance(),  Square::s15(), Square::s17(), true),  caps[0]);
    ASSERT_EQ(Move(Piece::whiteLance(),  Square::s15(), Square::s18(), true),  caps[1]);
    ASSERT_EQ(Move(Piece::whiteLance(),  Square::s82(), Square::s87(), true),  caps[2]);
    ASSERT_EQ(Move(Piece::whiteLance(),  Square::s82(), Square::s87(), false), caps[3]);
    ASSERT_EQ(Move(Piece::whiteSilver(), Square::s37(), Square::s26(), true),  caps[4]);
    ASSERT_EQ(Move(Piece::whiteSilver(), Square::s37(), Square::s38(), true),  caps[5]);
    ASSERT_EQ(Move(Piece::whiteSilver(), Square::s37(), Square::s46(), true),  caps[6]);
    ASSERT_EQ(Move(Piece::whiteSilver(), Square::s37(), Square::s46(), false), caps[7]);
    ASSERT_EQ(Move(Piece::whiteSilver(), Square::s37(), Square::s48(), true),  caps[8]);
    ASSERT_EQ(Move(Piece::whiteSilver(), Square::s78(), Square::s69(), true),  caps[9]);
    ASSERT_EQ(Move(Piece::whiteSilver(), Square::s78(), Square::s79(), true),  caps[10]);
    ASSERT_EQ(Move(Piece::whiteSilver(), Square::s78(), Square::s87(), true),  caps[11]);
    ASSERT_EQ(Move(Piece::whiteSilver(), Square::s78(), Square::s87(), false), caps[12]);
    ASSERT_EQ(Move(Piece::whiteSilver(), Square::s78(), Square::s89(), true),  caps[13]);
    ASSERT_EQ(Move(Piece::whiteGold(),   Square::s28(), Square::s18(), false), caps[14]);
    ASSERT_EQ(Move(Piece::whiteBishop(), Square::s35(), Square::s17(), true),  caps[15]);
    ASSERT_EQ(Move(Piece::whiteBishop(), Square::s35(), Square::s46(), false), caps[16]);
    ASSERT_EQ(Move(Piece::whiteRook(),   Square::s67(), Square::s47(), true),  caps[17]);
    ASSERT_EQ(Move(Piece::whiteRook(),   Square::s67(), Square::s57(), true),  caps[18]);
    ASSERT_EQ(Move(Piece::whiteRook(),   Square::s67(), Square::s64(), true),  caps[19]);
    ASSERT_EQ(Move(Piece::whiteRook(),   Square::s67(), Square::s65(), true),  caps[20]);
    ASSERT_EQ(Move(Piece::whiteRook(),   Square::s67(), Square::s66(), true),  caps[21]);
    ASSERT_EQ(Move(Piece::whiteRook(),   Square::s67(), Square::s68(), true),  caps[22]);
    ASSERT_EQ(Move(Piece::whiteRook(),   Square::s67(), Square::s69(), true),  caps[23]);
    ASSERT_EQ(Move(Piece::whiteRook(),   Square::s67(), Square::s77(), true),  caps[24]);
    ASSERT_EQ(Move(Piece::whiteRook(),   Square::s67(), Square::s87(), true),  caps[25]);
    ASSERT_EQ(Move(Piece::whiteHorse(),  Square::s97(), Square::s75(), false), caps[26]);
    ASSERT_EQ(Move(Piece::whiteHorse(),  Square::s97(), Square::s87(), false), caps[27]);

    Moves nocaps;
    MoveGenerator::generateNotCapturingMoves(pos, nocaps);
    sortMoves(nocaps);
    ASSERT_EQ(36, nocaps.size());
    ASSERT_EQ(Move(Piece::whitePawn(),   Square::s63(), Square::s64(), false), nocaps[0]);
    ASSERT_EQ(Move(Piece::whiteLance(),  Square::s15(), Square::s16(), false), nocaps[1]);
    ASSERT_EQ(Move(Piece::whiteLance(),  Square::s15(), Square::s17(), false), nocaps[2]);
    ASSERT_EQ(Move(Piece::whiteLance(),  Square::s82(), Square::s83(), false), nocaps[3]);
    ASSERT_EQ(Move(Piece::whiteLance(),  Square::s82(), Square::s84(), false), nocaps[4]);
    ASSERT_EQ(Move(Piece::whiteLance(),  Square::s82(), Square::s85(), false), nocaps[5]);
    ASSERT_EQ(Move(Piece::whiteLance(),  Square::s82(), Square::s86(), false), nocaps[6]);
    ASSERT_EQ(Move(Piece::whiteKnight(), Square::s53(), Square::s45(), false), nocaps[7]);
    ASSERT_EQ(Move(Piece::whiteKnight(), Square::s53(), Square::s65(), false), nocaps[8]);
    ASSERT_EQ(Move(Piece::whiteSilver(), Square::s37(), Square::s26(), false), nocaps[9]);
    ASSERT_EQ(Move(Piece::whiteSilver(), Square::s37(), Square::s38(), false), nocaps[10]);
    ASSERT_EQ(Move(Piece::whiteSilver(), Square::s37(), Square::s48(), false), nocaps[11]);
    ASSERT_EQ(Move(Piece::whiteSilver(), Square::s78(), Square::s69(), false), nocaps[12]);
    ASSERT_EQ(Move(Piece::whiteSilver(), Square::s78(), Square::s79(), false), nocaps[13]);
    ASSERT_EQ(Move(Piece::whiteSilver(), Square::s78(), Square::s89(), false), nocaps[14]);
    ASSERT_EQ(Move(Piece::whiteGold(),   Square::s28(), Square::s19(), false), nocaps[15]);
    ASSERT_EQ(Move(Piece::whiteGold(),   Square::s28(), Square::s27(), false), nocaps[16]);
    ASSERT_EQ(Move(Piece::whiteGold(),   Square::s28(), Square::s29(), false), nocaps[17]);
    ASSERT_EQ(Move(Piece::whiteGold(),   Square::s28(), Square::s38(), false), nocaps[18]);
    ASSERT_EQ(Move(Piece::whiteGold(),   Square::s28(), Square::s39(), false), nocaps[19]);
    ASSERT_EQ(Move(Piece::whiteBishop(), Square::s35(), Square::s13(), false), nocaps[20]);
    ASSERT_EQ(Move(Piece::whiteBishop(), Square::s35(), Square::s24(), false), nocaps[21]);
    ASSERT_EQ(Move(Piece::whiteBishop(), Square::s35(), Square::s26(), false), nocaps[22]);
    ASSERT_EQ(Move(Piece::whiteBishop(), Square::s35(), Square::s44(), false), nocaps[23]);
    ASSERT_EQ(Move(Piece::whiteKing(),   Square::s51(), Square::s41(), false), nocaps[24]);
    ASSERT_EQ(Move(Piece::whiteKing(),   Square::s51(), Square::s42(), false), nocaps[25]);
    ASSERT_EQ(Move(Piece::whiteKing(),   Square::s51(), Square::s52(), false), nocaps[26]);
    ASSERT_EQ(Move(Piece::whiteKing(),   Square::s51(), Square::s61(), false), nocaps[27]);
    ASSERT_EQ(Move(Piece::whiteKing(),   Square::s51(), Square::s62(), false), nocaps[28]);
    ASSERT_EQ(Move(Piece::whiteTokin(),  Square::s99(), Square::s89(), false), nocaps[29]);
    ASSERT_EQ(Move(Piece::whiteTokin(),  Square::s99(), Square::s98(), false), nocaps[30]);
    ASSERT_EQ(Move(Piece::whiteHorse(),  Square::s97(), Square::s79(), false), nocaps[31]);
    ASSERT_EQ(Move(Piece::whiteHorse(),  Square::s97(), Square::s86(), false), nocaps[32]);
    ASSERT_EQ(Move(Piece::whiteHorse(),  Square::s97(), Square::s88(), false), nocaps[33]);
    ASSERT_EQ(Move(Piece::whiteHorse(),  Square::s97(), Square::s96(), false), nocaps[34]);
    ASSERT_EQ(Move(Piece::whiteHorse(),  Square::s97(), Square::s98(), false), nocaps[35]);
  }
}

#endif // !defined(NDEBUG)
