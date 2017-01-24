/* MoveGeneratorTest.cpp
 *
 * Kubo Ryosuke
 */

#include "test/Test.hpp"
#include "core/move/MoveGenerator.hpp"
#include "core/util/PositionUtil.hpp"
#include <sstream>

using namespace sunfish;

namespace {

#if 0
void debugPrint(const Moves& moves) {
  std::ostringstream oss;
  oss << "debugPrint(Moves):";
  for (const auto& move : moves) {
    oss << ' ' << move.toString();
  }
  OUT(debug) << oss.str();
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
    MoveGenerator::generateCaptures(pos, caps);
    sortMovesForDebug(caps, pos);
    ASSERT_EQ(4, caps.size());
    ASSERT_EQ(Move(Square::s12(), Square::s11(), true),  caps[0]);
    ASSERT_EQ(Move(Square::s23(), Square::s22(), true),  caps[1]);
    ASSERT_EQ(Move(Square::s34(), Square::s33(), true),  caps[2]);
    ASSERT_EQ(Move(Square::s67(), Square::s66(), false), caps[3]);

    Moves nocaps;
    MoveGenerator::generateQuiets(pos, nocaps);
    sortMovesForDebug(nocaps, pos);
    ASSERT_EQ(9, nocaps.size());
    ASSERT_EQ(Move(Square::s45(), Square::s44(), false), nocaps[0]);
    ASSERT_EQ(Move(Square::s56(), Square::s55(), false), nocaps[1]);
    ASSERT_EQ(Move(Square::s78(), Square::s77(), false), nocaps[2]);
    ASSERT_EQ(Move(Square::s89(), Square::s88(), false), nocaps[3]);
    ASSERT_EQ(Move(Square::s59(), Square::s48(), false), nocaps[4]);
    ASSERT_EQ(Move(Square::s59(), Square::s49(), false), nocaps[5]);
    ASSERT_EQ(Move(Square::s59(), Square::s58(), false), nocaps[6]);
    ASSERT_EQ(Move(Square::s59(), Square::s68(), false), nocaps[7]);
    ASSERT_EQ(Move(Square::s59(), Square::s69(), false), nocaps[8]);
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
    MoveGenerator::generateCaptures(pos, caps);
    sortMovesForDebug(caps, pos);
    ASSERT_EQ(4, caps.size());
    ASSERT_EQ(Move(Square::s18(), Square::s19(), true),  caps[0]);
    ASSERT_EQ(Move(Square::s27(), Square::s28(), true),  caps[1]);
    ASSERT_EQ(Move(Square::s36(), Square::s37(), true),  caps[2]);
    ASSERT_EQ(Move(Square::s63(), Square::s64(), false), caps[3]);

    Moves nocaps;
    MoveGenerator::generateQuiets(pos, nocaps);
    sortMovesForDebug(nocaps, pos);
    ASSERT_EQ(9, nocaps.size());
    ASSERT_EQ(Move(Square::s45(), Square::s46(), false), nocaps[0]);
    ASSERT_EQ(Move(Square::s54(), Square::s55(), false), nocaps[1]);
    ASSERT_EQ(Move(Square::s72(), Square::s73(), false), nocaps[2]);
    ASSERT_EQ(Move(Square::s81(), Square::s82(), false), nocaps[3]);
    ASSERT_EQ(Move(Square::s51(), Square::s41(), false), nocaps[4]);
    ASSERT_EQ(Move(Square::s51(), Square::s42(), false), nocaps[5]);
    ASSERT_EQ(Move(Square::s51(), Square::s52(), false), nocaps[6]);
    ASSERT_EQ(Move(Square::s51(), Square::s61(), false), nocaps[7]);
    ASSERT_EQ(Move(Square::s51(), Square::s62(), false), nocaps[8]);
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
    MoveGenerator::generateCaptures(pos, caps);
    sortMovesForDebug(caps, pos);
    ASSERT_EQ(6, caps.size());
    ASSERT_EQ(Move(Square::s34(), Square::s22(), true),  caps[0]);
    ASSERT_EQ(Move(Square::s34(), Square::s42(), true),  caps[1]);
    ASSERT_EQ(Move(Square::s46(), Square::s54(), false), caps[2]);
    ASSERT_EQ(Move(Square::s55(), Square::s43(), true),  caps[3]);
    ASSERT_EQ(Move(Square::s55(), Square::s63(), true),  caps[4]);
    ASSERT_EQ(Move(Square::s55(), Square::s63(), false), caps[5]);

    Moves nocaps;
    MoveGenerator::generateQuiets(pos, nocaps);
    sortMovesForDebug(nocaps, pos);
    ASSERT_EQ(6, nocaps.size());
    ASSERT_EQ(Move(Square::s55(), Square::s43(), false), nocaps[0]);
    ASSERT_EQ(Move(Square::s59(), Square::s48(), false), nocaps[1]);
    ASSERT_EQ(Move(Square::s59(), Square::s49(), false), nocaps[2]);
    ASSERT_EQ(Move(Square::s59(), Square::s58(), false), nocaps[3]);
    ASSERT_EQ(Move(Square::s59(), Square::s68(), false), nocaps[4]);
    ASSERT_EQ(Move(Square::s59(), Square::s69(), false), nocaps[5]);
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
    MoveGenerator::generateCaptures(pos, caps);
    sortMovesForDebug(caps, pos);
    ASSERT_EQ(6, caps.size());
    ASSERT_EQ(Move(Square::s36(), Square::s28(), true),  caps[0]);
    ASSERT_EQ(Move(Square::s36(), Square::s48(), true),  caps[1]);
    ASSERT_EQ(Move(Square::s44(), Square::s56(), false), caps[2]);
    ASSERT_EQ(Move(Square::s55(), Square::s47(), true),  caps[3]);
    ASSERT_EQ(Move(Square::s55(), Square::s67(), true),  caps[4]);
    ASSERT_EQ(Move(Square::s55(), Square::s67(), false), caps[5]);

    Moves nocaps;
    MoveGenerator::generateQuiets(pos, nocaps);
    sortMovesForDebug(nocaps, pos);
    ASSERT_EQ(6, nocaps.size());
    ASSERT_EQ(Move(Square::s55(), Square::s47(), false), nocaps[0]);
    ASSERT_EQ(Move(Square::s51(), Square::s41(), false), nocaps[1]);
    ASSERT_EQ(Move(Square::s51(), Square::s42(), false), nocaps[2]);
    ASSERT_EQ(Move(Square::s51(), Square::s52(), false), nocaps[3]);
    ASSERT_EQ(Move(Square::s51(), Square::s61(), false), nocaps[4]);
    ASSERT_EQ(Move(Square::s51(), Square::s62(), false), nocaps[5]);
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
    MoveGenerator::generateCaptures(pos, caps);
    sortMovesForDebug(caps, pos);
    ASSERT_EQ(22, caps.size());
    ASSERT_EQ(Move(Square::s15(), Square::s12(), true),  caps[0]);
    ASSERT_EQ(Move(Square::s15(), Square::s13(), true),  caps[1]);
    ASSERT_EQ(Move(Square::s88(), Square::s83(), true),  caps[2]);
    ASSERT_EQ(Move(Square::s88(), Square::s83(), false), caps[3]);
    ASSERT_EQ(Move(Square::s33(), Square::s44(), true),  caps[4]);
    ASSERT_EQ(Move(Square::s33(), Square::s44(), false), caps[5]);
    ASSERT_EQ(Move(Square::s72(), Square::s83(), true),  caps[6]);
    ASSERT_EQ(Move(Square::s72(), Square::s83(), false), caps[7]);
    ASSERT_EQ(Move(Square::s22(), Square::s12(), false), caps[8]);
    ASSERT_EQ(Move(Square::s35(), Square::s13(), true),  caps[9]);
    ASSERT_EQ(Move(Square::s35(), Square::s44(), false), caps[10]);
    ASSERT_EQ(Move(Square::s63(), Square::s43(), true),  caps[11]);
    ASSERT_EQ(Move(Square::s63(), Square::s53(), true),  caps[12]);
    ASSERT_EQ(Move(Square::s63(), Square::s61(), true),  caps[13]);
    ASSERT_EQ(Move(Square::s63(), Square::s62(), true),  caps[14]);
    ASSERT_EQ(Move(Square::s63(), Square::s64(), true),  caps[15]);
    ASSERT_EQ(Move(Square::s63(), Square::s65(), true),  caps[16]);
    ASSERT_EQ(Move(Square::s63(), Square::s66(), true),  caps[17]);
    ASSERT_EQ(Move(Square::s63(), Square::s73(), true),  caps[18]);
    ASSERT_EQ(Move(Square::s63(), Square::s83(), true),  caps[19]);
    ASSERT_EQ(Move(Square::s93(), Square::s75(), false), caps[20]);
    ASSERT_EQ(Move(Square::s93(), Square::s83(), false), caps[21]);

    Moves nocaps;
    MoveGenerator::generateQuiets(pos, nocaps);
    sortMovesForDebug(nocaps, pos);
    ASSERT_EQ(42, nocaps.size());
    ASSERT_EQ(Move(Square::s67(), Square::s66(), false), nocaps[0]);
    ASSERT_EQ(Move(Square::s15(), Square::s13(), false), nocaps[1]);
    ASSERT_EQ(Move(Square::s15(), Square::s14(), false), nocaps[2]);
    ASSERT_EQ(Move(Square::s88(), Square::s84(), false), nocaps[3]);
    ASSERT_EQ(Move(Square::s88(), Square::s85(), false), nocaps[4]);
    ASSERT_EQ(Move(Square::s88(), Square::s86(), false), nocaps[5]);
    ASSERT_EQ(Move(Square::s88(), Square::s87(), false), nocaps[6]);
    ASSERT_EQ(Move(Square::s57(), Square::s45(), false), nocaps[7]);
    ASSERT_EQ(Move(Square::s57(), Square::s65(), false), nocaps[8]);
    ASSERT_EQ(Move(Square::s33(), Square::s24(), true),  nocaps[9]);
    ASSERT_EQ(Move(Square::s33(), Square::s24(), false), nocaps[10]);
    ASSERT_EQ(Move(Square::s33(), Square::s32(), true),  nocaps[11]);
    ASSERT_EQ(Move(Square::s33(), Square::s32(), false), nocaps[12]);
    ASSERT_EQ(Move(Square::s33(), Square::s42(), true),  nocaps[13]);
    ASSERT_EQ(Move(Square::s33(), Square::s42(), false), nocaps[14]);
    ASSERT_EQ(Move(Square::s72(), Square::s61(), true),  nocaps[15]);
    ASSERT_EQ(Move(Square::s72(), Square::s61(), false), nocaps[16]);
    ASSERT_EQ(Move(Square::s72(), Square::s71(), true),  nocaps[17]);
    ASSERT_EQ(Move(Square::s72(), Square::s71(), false), nocaps[18]);
    ASSERT_EQ(Move(Square::s72(), Square::s81(), true),  nocaps[19]);
    ASSERT_EQ(Move(Square::s72(), Square::s81(), false), nocaps[20]);
    ASSERT_EQ(Move(Square::s22(), Square::s11(), false), nocaps[21]);
    ASSERT_EQ(Move(Square::s22(), Square::s21(), false), nocaps[22]);
    ASSERT_EQ(Move(Square::s22(), Square::s23(), false), nocaps[23]);
    ASSERT_EQ(Move(Square::s22(), Square::s31(), false), nocaps[24]);
    ASSERT_EQ(Move(Square::s22(), Square::s32(), false), nocaps[25]);
    ASSERT_EQ(Move(Square::s35(), Square::s17(), false), nocaps[26]);
    ASSERT_EQ(Move(Square::s35(), Square::s24(), false), nocaps[27]);
    ASSERT_EQ(Move(Square::s35(), Square::s26(), false), nocaps[28]);
    ASSERT_EQ(Move(Square::s35(), Square::s46(), false), nocaps[29]);
    ASSERT_EQ(Move(Square::s59(), Square::s48(), false), nocaps[30]);
    ASSERT_EQ(Move(Square::s59(), Square::s49(), false), nocaps[31]);
    ASSERT_EQ(Move(Square::s59(), Square::s58(), false), nocaps[32]);
    ASSERT_EQ(Move(Square::s59(), Square::s68(), false), nocaps[33]);
    ASSERT_EQ(Move(Square::s59(), Square::s69(), false), nocaps[34]);
    ASSERT_EQ(Move(Square::s91(), Square::s81(), false), nocaps[35]);
    ASSERT_EQ(Move(Square::s91(), Square::s92(), false), nocaps[36]);
    ASSERT_EQ(Move(Square::s93(), Square::s71(), false), nocaps[37]);
    ASSERT_EQ(Move(Square::s93(), Square::s82(), false), nocaps[38]);
    ASSERT_EQ(Move(Square::s93(), Square::s84(), false), nocaps[39]);
    ASSERT_EQ(Move(Square::s93(), Square::s92(), false), nocaps[40]);
    ASSERT_EQ(Move(Square::s93(), Square::s94(), false), nocaps[41]);
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
    MoveGenerator::generateCaptures(pos, caps);
    sortMovesForDebug(caps, pos);
    ASSERT_EQ(22, caps.size());
    ASSERT_EQ(Move(Square::s15(), Square::s17(), true),  caps[0]);
    ASSERT_EQ(Move(Square::s15(), Square::s18(), true),  caps[1]);
    ASSERT_EQ(Move(Square::s82(), Square::s87(), true),  caps[2]);
    ASSERT_EQ(Move(Square::s82(), Square::s87(), false), caps[3]);
    ASSERT_EQ(Move(Square::s37(), Square::s46(), true),  caps[4]);
    ASSERT_EQ(Move(Square::s37(), Square::s46(), false), caps[5]);
    ASSERT_EQ(Move(Square::s78(), Square::s87(), true),  caps[6]);
    ASSERT_EQ(Move(Square::s78(), Square::s87(), false), caps[7]);
    ASSERT_EQ(Move(Square::s28(), Square::s18(), false), caps[8]);
    ASSERT_EQ(Move(Square::s35(), Square::s17(), true),  caps[9]);
    ASSERT_EQ(Move(Square::s35(), Square::s46(), false), caps[10]);
    ASSERT_EQ(Move(Square::s67(), Square::s47(), true),  caps[11]);
    ASSERT_EQ(Move(Square::s67(), Square::s57(), true),  caps[12]);
    ASSERT_EQ(Move(Square::s67(), Square::s64(), true),  caps[13]);
    ASSERT_EQ(Move(Square::s67(), Square::s65(), true),  caps[14]);
    ASSERT_EQ(Move(Square::s67(), Square::s66(), true),  caps[15]);
    ASSERT_EQ(Move(Square::s67(), Square::s68(), true),  caps[16]);
    ASSERT_EQ(Move(Square::s67(), Square::s69(), true),  caps[17]);
    ASSERT_EQ(Move(Square::s67(), Square::s77(), true),  caps[18]);
    ASSERT_EQ(Move(Square::s67(), Square::s87(), true),  caps[19]);
    ASSERT_EQ(Move(Square::s97(), Square::s75(), false), caps[20]);
    ASSERT_EQ(Move(Square::s97(), Square::s87(), false), caps[21]);

    Moves nocaps;
    MoveGenerator::generateQuiets(pos, nocaps);
    sortMovesForDebug(nocaps, pos);
    ASSERT_EQ(42, nocaps.size());
    ASSERT_EQ(Move(Square::s63(), Square::s64(), false), nocaps[0]);
    ASSERT_EQ(Move(Square::s15(), Square::s16(), false), nocaps[1]);
    ASSERT_EQ(Move(Square::s15(), Square::s17(), false), nocaps[2]);
    ASSERT_EQ(Move(Square::s82(), Square::s83(), false), nocaps[3]);
    ASSERT_EQ(Move(Square::s82(), Square::s84(), false), nocaps[4]);
    ASSERT_EQ(Move(Square::s82(), Square::s85(), false), nocaps[5]);
    ASSERT_EQ(Move(Square::s82(), Square::s86(), false), nocaps[6]);
    ASSERT_EQ(Move(Square::s53(), Square::s45(), false), nocaps[7]);
    ASSERT_EQ(Move(Square::s53(), Square::s65(), false), nocaps[8]);
    ASSERT_EQ(Move(Square::s37(), Square::s26(), true),  nocaps[9]);
    ASSERT_EQ(Move(Square::s37(), Square::s26(), false), nocaps[10]);
    ASSERT_EQ(Move(Square::s37(), Square::s38(), true),  nocaps[11]);
    ASSERT_EQ(Move(Square::s37(), Square::s38(), false), nocaps[12]);
    ASSERT_EQ(Move(Square::s37(), Square::s48(), true),  nocaps[13]);
    ASSERT_EQ(Move(Square::s37(), Square::s48(), false), nocaps[14]);
    ASSERT_EQ(Move(Square::s78(), Square::s69(), true),  nocaps[15]);
    ASSERT_EQ(Move(Square::s78(), Square::s69(), false), nocaps[16]);
    ASSERT_EQ(Move(Square::s78(), Square::s79(), true),  nocaps[17]);
    ASSERT_EQ(Move(Square::s78(), Square::s79(), false), nocaps[18]);
    ASSERT_EQ(Move(Square::s78(), Square::s89(), true),  nocaps[19]);
    ASSERT_EQ(Move(Square::s78(), Square::s89(), false), nocaps[20]);
    ASSERT_EQ(Move(Square::s28(), Square::s19(), false), nocaps[21]);
    ASSERT_EQ(Move(Square::s28(), Square::s27(), false), nocaps[22]);
    ASSERT_EQ(Move(Square::s28(), Square::s29(), false), nocaps[23]);
    ASSERT_EQ(Move(Square::s28(), Square::s38(), false), nocaps[24]);
    ASSERT_EQ(Move(Square::s28(), Square::s39(), false), nocaps[25]);
    ASSERT_EQ(Move(Square::s35(), Square::s13(), false), nocaps[26]);
    ASSERT_EQ(Move(Square::s35(), Square::s24(), false), nocaps[27]);
    ASSERT_EQ(Move(Square::s35(), Square::s26(), false), nocaps[28]);
    ASSERT_EQ(Move(Square::s35(), Square::s44(), false), nocaps[29]);
    ASSERT_EQ(Move(Square::s51(), Square::s41(), false), nocaps[30]);
    ASSERT_EQ(Move(Square::s51(), Square::s42(), false), nocaps[31]);
    ASSERT_EQ(Move(Square::s51(), Square::s52(), false), nocaps[32]);
    ASSERT_EQ(Move(Square::s51(), Square::s61(), false), nocaps[33]);
    ASSERT_EQ(Move(Square::s51(), Square::s62(), false), nocaps[34]);
    ASSERT_EQ(Move(Square::s99(), Square::s89(), false), nocaps[35]);
    ASSERT_EQ(Move(Square::s99(), Square::s98(), false), nocaps[36]);
    ASSERT_EQ(Move(Square::s97(), Square::s79(), false), nocaps[37]);
    ASSERT_EQ(Move(Square::s97(), Square::s86(), false), nocaps[38]);
    ASSERT_EQ(Move(Square::s97(), Square::s88(), false), nocaps[39]);
    ASSERT_EQ(Move(Square::s97(), Square::s96(), false), nocaps[40]);
    ASSERT_EQ(Move(Square::s97(), Square::s98(), false), nocaps[41]);
  }

  {
    // black drop
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1-KY *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  * -KI *  * \n"
      "P3 *  *  *  * -FU *  *  *  * \n"
      "P4 * +FU * +FU *  *  * +FU * \n"
      "P5 *  *  *  *  *  *  * -KY * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  * -FU *  *  *  *  * \n"
      "P8 * -RY *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+00KI00KE00FU\n"
      "P-\n"
      "+\n");

    Moves caps;
    MoveGenerator::generateCaptures(pos, caps);
    sortMovesForDebug(caps, pos);
    ASSERT_EQ(3, caps.size());

    Moves nocaps;
    MoveGenerator::generateQuiets(pos, nocaps);
    sortMovesForDebug(nocaps, pos);
    ASSERT_EQ(175, nocaps.size());
    ASSERT_EQ(Move(Square::s59(), Square::s48(), false), nocaps[0]);
    ASSERT_EQ(Move(PieceType::pawn(), Square::s12()),    nocaps[5]);
    ASSERT_EQ(Move(PieceType::pawn(), Square::s19()),    nocaps[12]);
    ASSERT_EQ(Move(PieceType::pawn(), Square::s33()),    nocaps[13]);
    ASSERT_EQ(Move(PieceType::knight(), Square::s13()),  nocaps[50]);
    ASSERT_EQ(Move(PieceType::gold(), Square::s11()),    nocaps[105]);
  }

  {
    // white drop
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 * +RY *  *  *  *  *  *  * \n"
      "P3 *  *  * +FU *  *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  * +KY * \n"
      "P6 * -FU * -FU *  *  * -FU * \n"
      "P7 *  *  *  * +FU *  *  *  * \n"
      "P8 *  *  *  *  *  * +KI *  * \n"
      "P9+KY *  *  * +OU *  *  *  * \n"
      "P+\n"
      "P-00KI00KE00FU\n"
      "-\n");

    Moves caps;
    MoveGenerator::generateCaptures(pos, caps);
    sortMovesForDebug(caps, pos);
    ASSERT_EQ(3, caps.size());

    Moves nocaps;
    MoveGenerator::generateQuiets(pos, nocaps);
    sortMovesForDebug(nocaps, pos);
    ASSERT_EQ(175, nocaps.size());
    ASSERT_EQ(Move(Square::s51(), Square::s41(), false), nocaps[0]);
    ASSERT_EQ(Move(PieceType::pawn(), Square::s11()),    nocaps[5]);
    ASSERT_EQ(Move(PieceType::pawn(), Square::s18()),    nocaps[12]);
    ASSERT_EQ(Move(PieceType::pawn(), Square::s31()),    nocaps[13]);
    ASSERT_EQ(Move(PieceType::knight(), Square::s11()),  nocaps[50]);
    ASSERT_EQ(Move(PieceType::knight(), Square::s17()),  nocaps[56]);
    ASSERT_EQ(Move(PieceType::knight(), Square::s21()),  nocaps[57]);
    ASSERT_EQ(Move(PieceType::gold(), Square::s11()),    nocaps[105]);
  }
}

TEST(MoveGeneratorTest, testEvasions) {
  {
    // black
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  *  *  *  *  *  * \n"
      "P8 *  *  * -KI *  *  *  *  * \n"
      "P9 *  *  * +GI+OU *  *  *  * \n"
      "P+00FU00FU00GI00KA00HI\n"
      "P-\n"
      "+\n");

    Moves evasions;
    MoveGenerator::generateEvasions(pos, pos.getCheckState(), evasions);
    sortMovesForDebug(evasions, pos);
    ASSERT_EQ(5, evasions.size());
    ASSERT_EQ(Move(Square::s69(), Square::s68(), false), evasions[0]);
    ASSERT_EQ(Move(Square::s59(), Square::s48(), false), evasions[1]);
    ASSERT_EQ(Move(Square::s59(), Square::s49(), false), evasions[2]);
    ASSERT_EQ(Move(Square::s59(), Square::s58(), false), evasions[3]);
    ASSERT_EQ(Move(Square::s59(), Square::s68(), false), evasions[4]);
  }

  {
    // black
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  * +RY * -KA *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  * -HI * \n"
      "P7 * +OU+FU *  *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 * -UM *  *  *  *  *  *  * \n"
      "P+00FU00FU00KI\n"
      "P-\n"
      "+\n");

    Moves evasions;
    MoveGenerator::generateEvasions(pos, pos.getCheckState(), evasions);
    sortMovesForDebug(evasions, pos);
    ASSERT_EQ(14, evasions.size());
    ASSERT_EQ(Move(Square::s77(), Square::s76(), false), evasions[0]);
    ASSERT_EQ(Move(Square::s87(), Square::s76(), false), evasions[1]);
    ASSERT_EQ(Move(Square::s87(), Square::s78(), false), evasions[2]);
    ASSERT_EQ(Move(Square::s87(), Square::s86(), false), evasions[3]);
    ASSERT_EQ(Move(Square::s87(), Square::s88(), false), evasions[4]);
    ASSERT_EQ(Move(Square::s87(), Square::s96(), false), evasions[5]);
    ASSERT_EQ(Move(Square::s87(), Square::s97(), false), evasions[6]);
    ASSERT_EQ(Move(Square::s87(), Square::s98(), false), evasions[7]);
    ASSERT_EQ(Move(Square::s74(), Square::s54(), false), evasions[8]);
    ASSERT_EQ(Move(Square::s74(), Square::s65(), false), evasions[9]);
    ASSERT_EQ(Move(Square::s74(), Square::s76(), false), evasions[10]);
    ASSERT_EQ(Move(PieceType::pawn(),   Square::s65()),  evasions[11]);
    ASSERT_EQ(Move(PieceType::gold(),   Square::s65()),  evasions[12]);
    ASSERT_EQ(Move(PieceType::gold(),   Square::s76()),  evasions[13]);
  }

  {
    // white
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -GI-OU *  *  * \n"
      "P2 *  *  *  *  * +HI *  *  * \n"
      "P3 *  *  *  *  *  *  *  *  * \n"
      "P4 *  *  * +KA *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  *  *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+\n"
      "P-00FU00FU00GI00KA00HI\n"
      "-\n");

    Moves evasions;
    MoveGenerator::generateEvasions(pos, pos.getCheckState(), evasions);
    sortMovesForDebug(evasions, pos);
    ASSERT_EQ(5, evasions.size());
    ASSERT_EQ(Move(Square::s51(), Square::s42(), false), evasions[0]);
    ASSERT_EQ(Move(Square::s41(), Square::s31(), false), evasions[1]);
    ASSERT_EQ(Move(Square::s41(), Square::s32(), false), evasions[2]);
    ASSERT_EQ(Move(Square::s41(), Square::s42(), false), evasions[3]);
    ASSERT_EQ(Move(Square::s41(), Square::s52(), false), evasions[4]);
  }

  {
    // white
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 * +UM *  *  *  *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3 * -OU-FU *  *  *  *  *  * \n"
      "P4 *  *  *  *  *  *  * +HI * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  * -RY * +KA *  *  *  * \n"
      "P7 *  *  *  *  *  *  *  *  * \n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+\n"
      "P-00FU00FU00KI\n"
      "-\n");

    Moves evasions;
    MoveGenerator::generateEvasions(pos, pos.getCheckState(), evasions);
    sortMovesForDebug(evasions, pos);
    ASSERT_EQ(14, evasions.size());
    ASSERT_EQ(Move(Square::s73(), Square::s74(), false), evasions[0]);
    ASSERT_EQ(Move(Square::s83(), Square::s72(), false), evasions[1]);
    ASSERT_EQ(Move(Square::s83(), Square::s74(), false), evasions[2]);
    ASSERT_EQ(Move(Square::s83(), Square::s82(), false), evasions[3]);
    ASSERT_EQ(Move(Square::s83(), Square::s84(), false), evasions[4]);
    ASSERT_EQ(Move(Square::s83(), Square::s92(), false), evasions[5]);
    ASSERT_EQ(Move(Square::s83(), Square::s93(), false), evasions[6]);
    ASSERT_EQ(Move(Square::s83(), Square::s94(), false), evasions[7]);
    ASSERT_EQ(Move(Square::s76(), Square::s56(), false), evasions[8]);
    ASSERT_EQ(Move(Square::s76(), Square::s65(), false), evasions[9]);
    ASSERT_EQ(Move(Square::s76(), Square::s74(), false), evasions[10]);
    ASSERT_EQ(Move(PieceType::pawn(),   Square::s65()),  evasions[11]);
    ASSERT_EQ(Move(PieceType::gold(),   Square::s65()),  evasions[12]);
    ASSERT_EQ(Move(PieceType::gold(),   Square::s74()),  evasions[13]);
  }
}

TEST(MoveGeneratorTest, testMateWithPawnDrop) {
  {
    // black
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  * -KY-OU-KE *  *  * \n"
      "P2 *  *  * -KY *  *  *  *  * \n"
      "P3 *  *  *  *  * +KI *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6-FU-FU-FU-FU * -FU-FU-FU-FU\n"
      "P7+FU+FU+FU+FU * +FU+FU+FU+FU\n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+00FU00FU00FU\n"
      "P-\n"
      "+\n");

    Moves nocaps;
    MoveGenerator::generateQuiets(pos, nocaps);
    sortMovesForDebug(nocaps, pos);
    ASSERT_EQ(17, nocaps.size());
    ASSERT_EQ(Move(Square::s43(), Square::s32(), false), nocaps[0]);
    ASSERT_EQ(Move(Square::s43(), Square::s33(), false), nocaps[1]);
    ASSERT_EQ(Move(Square::s43(), Square::s42(), false), nocaps[2]);
    ASSERT_EQ(Move(Square::s43(), Square::s44(), false), nocaps[3]);
    ASSERT_EQ(Move(Square::s43(), Square::s52(), false), nocaps[4]);
    ASSERT_EQ(Move(Square::s43(), Square::s53(), false), nocaps[5]);
    ASSERT_EQ(Move(Square::s59(), Square::s48(), false), nocaps[6]);
    ASSERT_EQ(Move(Square::s59(), Square::s49(), false), nocaps[7]);
    ASSERT_EQ(Move(Square::s59(), Square::s58(), false), nocaps[8]);
    ASSERT_EQ(Move(Square::s59(), Square::s68(), false), nocaps[9]);
    ASSERT_EQ(Move(Square::s59(), Square::s69(), false), nocaps[10]);
    ASSERT_EQ(Move(PieceType::pawn(), Square::s53()),    nocaps[11]);
    ASSERT_EQ(Move(PieceType::pawn(), Square::s54()),    nocaps[12]);
    ASSERT_EQ(Move(PieceType::pawn(), Square::s55()),    nocaps[13]);
    ASSERT_EQ(Move(PieceType::pawn(), Square::s56()),    nocaps[14]);
    ASSERT_EQ(Move(PieceType::pawn(), Square::s57()),    nocaps[15]);
    ASSERT_EQ(Move(PieceType::pawn(), Square::s58()),    nocaps[16]);
  }

  {
    // white
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3-FU-FU-FU-FU * -FU-FU-FU-FU\n"
      "P4+FU+FU+FU+FU * +FU+FU+FU+FU\n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  *  * -KI *  *  * \n"
      "P8 *  *  * +KY *  *  *  *  * \n"
      "P9 *  *  * +KY+OU+KE *  *  * \n"
      "P+\n"
      "P-00FU00FU00FU\n"
      "-\n");

    Moves nocaps;
    MoveGenerator::generateQuiets(pos, nocaps);
    sortMovesForDebug(nocaps, pos);
    ASSERT_EQ(17, nocaps.size());
    ASSERT_EQ(Move(Square::s47(), Square::s37(), false), nocaps[0]);
    ASSERT_EQ(Move(Square::s47(), Square::s38(), false), nocaps[1]);
    ASSERT_EQ(Move(Square::s47(), Square::s46(), false), nocaps[2]);
    ASSERT_EQ(Move(Square::s47(), Square::s48(), false), nocaps[3]);
    ASSERT_EQ(Move(Square::s47(), Square::s57(), false), nocaps[4]);
    ASSERT_EQ(Move(Square::s47(), Square::s58(), false), nocaps[5]);
    ASSERT_EQ(Move(Square::s51(), Square::s41(), false), nocaps[6]);
    ASSERT_EQ(Move(Square::s51(), Square::s42(), false), nocaps[7]);
    ASSERT_EQ(Move(Square::s51(), Square::s52(), false), nocaps[8]);
    ASSERT_EQ(Move(Square::s51(), Square::s61(), false), nocaps[9]);
    ASSERT_EQ(Move(Square::s51(), Square::s62(), false), nocaps[10]);
    ASSERT_EQ(Move(PieceType::pawn(), Square::s52()),    nocaps[11]);
    ASSERT_EQ(Move(PieceType::pawn(), Square::s53()),    nocaps[12]);
    ASSERT_EQ(Move(PieceType::pawn(), Square::s54()),    nocaps[13]);
    ASSERT_EQ(Move(PieceType::pawn(), Square::s55()),    nocaps[14]);
    ASSERT_EQ(Move(PieceType::pawn(), Square::s56()),    nocaps[15]);
    ASSERT_EQ(Move(PieceType::pawn(), Square::s57()),    nocaps[16]);
  }

  {
    // black
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  * -KY-OU-KE *  *  * \n"
      "P2 *  *  * -KY *  *  *  *  * \n"
      "P3 *  *  * -GI * +KI *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6-FU-FU-FU-FU * -FU-FU-FU-FU\n"
      "P7+FU+FU+FU+FU * +FU+FU+FU+FU\n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+00FU00FU00FU\n"
      "P-\n"
      "+\n");

    Moves nocaps;
    MoveGenerator::generateQuiets(pos, nocaps);
    sortMovesForDebug(nocaps, pos);
    ASSERT_EQ(18, nocaps.size());
  }

  {
    // white
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3-FU-FU-FU-FU * -FU-FU-FU-FU\n"
      "P4+FU+FU+FU+FU * +FU+FU+FU+FU\n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  * +GI * -KI *  *  * \n"
      "P8 *  *  * +KY *  *  *  *  * \n"
      "P9 *  *  * +KY+OU+KE *  *  * \n"
      "P+\n"
      "P-00FU00FU00FU\n"
      "-\n");

    Moves nocaps;
    MoveGenerator::generateQuiets(pos, nocaps);
    sortMovesForDebug(nocaps, pos);
    ASSERT_EQ(18, nocaps.size());
  }

  {
    // black
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  * -KY-OU-KE *  *  * \n"
      "P2 *  *  * -KY *  *  *  *  * \n"
      "P3 *  *  *  *  * +KI *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 * -KA *  *  *  *  *  *  * \n"
      "P6-FU-FU-FU-FU * -FU-FU-FU-FU\n"
      "P7+FU+FU+FU+FU * +FU+FU+FU+FU\n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+00FU00FU00FU\n"
      "P-\n"
      "+\n");

    Moves nocaps;
    MoveGenerator::generateQuiets(pos, nocaps);
    sortMovesForDebug(nocaps, pos);
    ASSERT_EQ(18, nocaps.size());
  }

  {
    // white
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3-FU-FU-FU-FU * -FU-FU-FU-FU\n"
      "P4+FU+FU+FU+FU * +FU+FU+FU+FU\n"
      "P5 * +KA *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  *  * -KI *  *  * \n"
      "P8 *  *  * +KY *  *  *  *  * \n"
      "P9 *  *  * +KY+OU+KE *  *  * \n"
      "P+\n"
      "P-00FU00FU00FU\n"
      "-\n");

    Moves nocaps;
    MoveGenerator::generateQuiets(pos, nocaps);
    sortMovesForDebug(nocaps, pos);
    ASSERT_EQ(18, nocaps.size());
  }

  {
    // black
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  * -KI-OU-KE *  *  * \n"
      "P2 *  *  * -KY *  *  *  *  * \n"
      "P3 *  *  *  *  * +KI *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6-FU-FU-FU-FU * -FU-FU-FU-FU\n"
      "P7+FU+FU+FU+FU * +FU+FU+FU+FU\n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+00FU00FU00FU\n"
      "P-\n"
      "+\n");

    Moves nocaps;
    MoveGenerator::generateQuiets(pos, nocaps);
    sortMovesForDebug(nocaps, pos);
    ASSERT_EQ(18, nocaps.size());
  }

  {
    // white
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3-FU-FU-FU-FU * -FU-FU-FU-FU\n"
      "P4+FU+FU+FU+FU * +FU+FU+FU+FU\n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  *  * -KI *  *  * \n"
      "P8 *  *  * +KY *  *  *  *  * \n"
      "P9 *  *  * +KI+OU+KE *  *  * \n"
      "P+\n"
      "P-00FU00FU00FU\n"
      "-\n");

    Moves nocaps;
    MoveGenerator::generateQuiets(pos, nocaps);
    sortMovesForDebug(nocaps, pos);
    ASSERT_EQ(18, nocaps.size());
  }

  {
    // black
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1+RY *  * -KI-OU-KE *  *  * \n"
      "P2 *  *  * -KY *  *  *  *  * \n"
      "P3 *  *  *  *  * +KI *  *  * \n"
      "P4 *  *  *  *  *  *  *  *  * \n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6-FU-FU-FU-FU * -FU-FU-FU-FU\n"
      "P7+FU+FU+FU+FU * +FU+FU+FU+FU\n"
      "P8 *  *  *  *  *  *  *  *  * \n"
      "P9 *  *  *  * +OU *  *  *  * \n"
      "P+00FU00FU00FU\n"
      "P-\n"
      "+\n");

    Moves nocaps;
    MoveGenerator::generateQuiets(pos, nocaps);
    sortMovesForDebug(nocaps, pos);
    ASSERT_EQ(24, nocaps.size());
  }

  {
    // white
    Position pos = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 *  *  *  *  *  *  *  *  * \n"
      "P3-FU-FU-FU-FU * -FU-FU-FU-FU\n"
      "P4+FU+FU+FU+FU * +FU+FU+FU+FU\n"
      "P5 *  *  *  *  *  *  *  *  * \n"
      "P6 *  *  *  *  *  *  *  *  * \n"
      "P7 *  *  *  *  * -KI *  *  * \n"
      "P8 *  *  * +KY *  *  *  *  * \n"
      "P9-RY *  * +KI+OU+KE *  *  * \n"
      "P+\n"
      "P-00FU00FU00FU\n"
      "-\n");

    Moves nocaps;
    MoveGenerator::generateQuiets(pos, nocaps);
    sortMovesForDebug(nocaps, pos);
    ASSERT_EQ(24, nocaps.size());
  }
}
