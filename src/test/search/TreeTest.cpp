/* TreeTest.cpp
 *
 * Kubo Ryosuke
 */

#include "test/Test.hpp"
#include "search/tree/Tree.hpp"
#include "search/tt/TT.hpp"
#include "core/util/PositionUtil.hpp"

using namespace sunfish;

namespace {

Evaluator g_eval(Evaluator::InitType::Zero);

} // namespace

TEST(TreeTest, testInitializeTree) {
  auto position = PositionUtil::createPositionFromCsaString(
    "P1 *  *  *  * -OU *  *  *  * \n"
    "P2 *  *  *  *  *  *  *  *  * \n"
    "P3 *  *  *  *  *  *  *  *  * \n"
    "P4 *  *  *  *  *  *  *  *  * \n"
    "P5 *  *  *  *  *  *  *  *  * \n"
    "P6 *  *  *  *  *  * +FU *  * \n"
    "P7 *  *  *  *  *  *  *  *  * \n"
    "P8 * -RY *  *  *  *  *  *  * \n"
    "P9 *  *  *  * +OU *  *  *  * \n"
    "P+\n"
    "P-\n"
    "+\n");
  Tree tree;
  tree.ply = 123;
  tree.info.nodes = 123;
  tree.nodes[0].score = 123;
  tree.nodes[0].killerMove1 = Move(PieceType::lance(), Square::s38());
  tree.nodes[0].killerMove2 = Move(PieceType::lance(), Square::s38());

  initializeTree(tree, position, g_eval, nullptr);
  ASSERT_EQ(Piece::blackPawn(), tree.position.getPieceOnBoard(Square::s36()));
  ASSERT_EQ(Piece::whiteDragon(), tree.position.getPieceOnBoard(Square::s88()));
  ASSERT_EQ(0, tree.ply);
  ASSERT_EQ(0, tree.info.nodes);
  ASSERT_EQ(g_eval.calculateMaterialScore(position), tree.nodes[0].materialScore);
  ASSERT_EQ(Score::invalid(), tree.nodes[0].score);
  ASSERT_EQ(Move::none(), tree.nodes[0].killerMove1);
  ASSERT_EQ(Move::none(), tree.nodes[0].killerMove2);
}

TEST(TreeTest, testVisit) {
  Tree tree;
  tree.ply = 0;
  tree.nodes[0].isHistorical = true;
  tree.nodes[0].ttMove = Move(Square::s77(), Square::s76(), false);
  tree.nodes[0].quietsSearched.add(Move(Square::s77(), Square::s76(), false));
  tree.nodes[0].pv.set(Move(Square::s77(), Square::s76(), false), 0, PV());
  tree.nodes[1].killerMove1 = Move(Square::s77(), Square::s76(), false);
  tree.nodes[1].killerMove2 = Move(Square::s77(), Square::s76(), false);
  tree.nodes[1].excludedMove = Move(Square::s77(), Square::s76(), false);
  visit(tree, NodeStat::normal().unsetRoot());
  ASSERT_FALSE(tree.nodes[0].isHistorical);
  ASSERT_EQ(Move::none(), tree.nodes[0].ttMove);
  ASSERT_EQ(0, tree.nodes[0].quietsSearched.size());
  ASSERT_EQ(0, tree.nodes[0].pv.size());
  ASSERT_EQ(Move::none(), tree.nodes[1].killerMove1);
  ASSERT_EQ(Move::none(), tree.nodes[1].killerMove2);
  ASSERT_EQ(Move::none(), tree.nodes[1].excludedMove);

  tree.nodes[0].pv.set(Move(Square::s77(), Square::s76(), false), 0, PV());
  visit(tree, NodeStat::normal().setRoot());
  ASSERT_EQ(1, tree.nodes[0].pv.size());
}

TEST(TreeTest, testRevisit) {
  Tree tree;
  tree.ply = 0;
  tree.nodes[0].isHistorical = true;
  tree.nodes[0].ttMove = Move(Square::s77(), Square::s76(), false);
  tree.nodes[0].quietsSearched.add(Move(Square::s77(), Square::s76(), false));
  tree.nodes[0].pv.set(Move(Square::s77(), Square::s76(), false), 0, PV());
  tree.nodes[1].killerMove1 = Move(Square::s77(), Square::s76(), false);
  tree.nodes[1].killerMove2 = Move(Square::s77(), Square::s76(), false);
  tree.nodes[1].excludedMove = Move(Square::s77(), Square::s76(), false);
  revisit(tree, NodeStat::normal().unsetRoot());
  ASSERT_FALSE(tree.nodes[0].isHistorical);
  ASSERT_EQ(Move(Square::s77(), Square::s76(), false), tree.nodes[0].ttMove);
  ASSERT_EQ(0, tree.nodes[0].quietsSearched.size());
  ASSERT_EQ(0, tree.nodes[0].pv.size());
  ASSERT_EQ(Move::none(), tree.nodes[1].killerMove1);
  ASSERT_EQ(Move::none(), tree.nodes[1].killerMove2);
  ASSERT_EQ(Move::none(), tree.nodes[1].excludedMove);

  tree.nodes[0].pv.set(Move(Square::s77(), Square::s76(), false), 0, PV());
  revisit(tree, NodeStat::normal().setRoot());
  ASSERT_EQ(1, tree.nodes[0].pv.size());
}

TEST(TreeTest, testHasKiller1) {
  Tree tree;
  tree.ply = 3;
  tree.nodes[3].killerMove1 = Move::none();
  ASSERT_FALSE(hasKiller1(tree));
  tree.nodes[3].killerMove1 = Move(Square::s76(), Square::s77(), false);
  ASSERT_TRUE(hasKiller1(tree));
}

TEST(TreeTest, testHasKiller2) {
  Tree tree;
  tree.ply = 3;
  tree.nodes[3].killerMove2 = Move::none();
  ASSERT_FALSE(hasKiller2(tree));
  tree.nodes[3].killerMove2 = Move(Square::s76(), Square::s77(), false);
  ASSERT_TRUE(hasKiller2(tree));
}

TEST(TreeTest, testIsKiller1Good) {
  Tree tree;
  tree.ply = 3;
  tree.nodes[3].killerCount1 = -1;
  ASSERT_FALSE(isKiller1Good(tree));
  tree.nodes[3].killerCount1 = 0;
  ASSERT_TRUE(isKiller1Good(tree));
}

TEST(TreeTest, testIsKiller2Good) {
  Tree tree;
  tree.ply = 3;
  tree.nodes[3].killerCount2 = -1;
  ASSERT_FALSE(isKiller2Good(tree));
  tree.nodes[3].killerCount2 = 0;
  ASSERT_TRUE(isKiller2Good(tree));
}

TEST(TreeTest, testIsKiller1Legal) {
  // TODO
}

TEST(TreeTest, testIsKiller2Legal) {
  // TODO
}

TEST(TreeTest, testIsPriorMove) {
  Move move1 = Move(Square::s24(), Square::s23(), true);
  Move move2 = Move(Square::s65(), Square::s73(), true);
  Move move3 = Move(Square::s79(), Square::s88(), false);
  Move move4 = Move(Square::s57(), Square::s66(), false);

  Tree tree;
  tree.ply = 3;
  tree.nodes[3].ttMove = move1;
  tree.nodes[3].killerMove1 = move2;
  tree.nodes[3].killerCount1 = -1;
  tree.nodes[3].killerMove2 = move3;
  tree.nodes[3].killerCount2 = -1;

  // TTMove
  ASSERT_TRUE(isPriorMove(tree, move1));

  // Killer1
  ASSERT_FALSE(isPriorMove(tree, move2));
  tree.nodes[3].killerCount1 = 1;
  ASSERT_TRUE(isPriorMove(tree, move2));

  // Killer2
  ASSERT_FALSE(isPriorMove(tree, move3));
  tree.nodes[3].killerCount2 = 1;
  ASSERT_TRUE(isPriorMove(tree, move3));

  // Not match
  ASSERT_FALSE(isPriorMove(tree, move4));
}

TEST(TreeTest, testAddKiller) {
  Move move1(Square::s37(), Square::s73(), true);
  Move move2(Square::s78(), Square::s73(), true);
  Move move3(PieceType::gold(), Square::s62());

  Tree tree;
  tree.ply = 4;
  visit(tree, NodeStat::normal());
  tree.ply = 5;

  addKiller(tree, move1);
  ASSERT_EQ(move1, tree.nodes[5].killerMove1);
  ASSERT_EQ(    0, tree.nodes[5].killerCount1);
  ASSERT_EQ(Move::none(), tree.nodes[5].killerMove2);

  addKiller(tree, move2);
  ASSERT_EQ(move2, tree.nodes[5].killerMove1);
  ASSERT_EQ(    0, tree.nodes[5].killerCount1);
  ASSERT_EQ(move1, tree.nodes[5].killerMove2);
  ASSERT_EQ(   -1, tree.nodes[5].killerCount2);

  addKiller(tree, move3);
  ASSERT_EQ(move3, tree.nodes[5].killerMove1);
  ASSERT_EQ(    0, tree.nodes[5].killerCount1);
  ASSERT_EQ(move2, tree.nodes[5].killerMove2);
  ASSERT_EQ(   -1, tree.nodes[5].killerCount2);

  addKiller(tree, move2);
  ASSERT_EQ(move2, tree.nodes[5].killerMove1);
  ASSERT_EQ(    1, tree.nodes[5].killerCount1);
  ASSERT_EQ(move3, tree.nodes[5].killerMove2);
  ASSERT_EQ(   -1, tree.nodes[5].killerCount2);

  addKiller(tree, move2);
  ASSERT_EQ(move2, tree.nodes[5].killerMove1);
  ASSERT_EQ(    3, tree.nodes[5].killerCount1);
  ASSERT_EQ(move3, tree.nodes[5].killerMove2);
  ASSERT_EQ(   -2, tree.nodes[5].killerCount2);
}

TEST(TreeTest, testDoMove) {
  Tree tree;
  TT tt;

  auto pos1 = Position(Position::Handicap::Even);
  auto pos2 = PositionUtil::createPositionFromCsaString(
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
  auto pos3 = PositionUtil::createPositionFromCsaString(
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
    "-\n");
  initializeTree(tree, pos1, g_eval, nullptr);
  tree.nodes[0].score = Score::zero();

  auto move = Move(Square::s77(), Square::s76(), false);
  doMove<true>(tree, move, g_eval, tt);
  ASSERT_EQ(1, tree.ply);
  ASSERT_EQ(move, tree.nodes[0].move);
  ASSERT_EQ(Score::invalid(), tree.nodes[1].score);
  ASSERT_EQ(pos2.toString(), tree.position.toString());

  undoMove<true>(tree);
  ASSERT_EQ(0, tree.ply);
  ASSERT_EQ(pos1.toString(), tree.position.toString());

  doNullMove(tree, tt);
  ASSERT_EQ(1, tree.ply);
  ASSERT_EQ(Move::none(), tree.nodes[0].move);
  ASSERT_EQ(Score::zero(), tree.nodes[1].score);
  ASSERT_EQ(pos3.toString(), tree.position.toString());

  undoNullMove(tree);
  ASSERT_EQ(0, tree.ply);
  ASSERT_EQ(pos1.toString(), tree.position.toString());
}

TEST(TreeTest, testCalculateStandPat) {
  auto pos1 = PositionUtil::createPositionFromCsaString(
    "P1-KY-KE-GI-KI-OU-KI-GI-KE-KY\n"
    "P2 * -HI *  *  *  *  * -KA * \n"
    "P3-FU * -FU-FU-FU-FU-FU-FU-FU\n"
    "P4 * -FU *  *  *  *  *  *  * \n"
    "P5 *  *  *  *  *  *  *  *  * \n"
    "P6 *  * +FU *  *  *  *  *  * \n"
    "P7+FU+FU * +FU+FU+FU+FU+FU+FU\n"
    "P8 * +KA *  *  *  *  * +HI * \n"
    "P9+KY+KE+GI+KI+OU+KI+GI+KE+KY\n"
    "P+\n"
    "P-\n"
    "+\n");
  auto pos2 = PositionUtil::createPositionFromCsaString(
    "P1-KY-KE-GI-KI-OU-KI-GI-KE-KY\n"
    "P2 * -HI *  *  *  *  * -KA * \n"
    "P3-FU * -FU-FU-FU-FU-FU-FU-FU\n"
    "P4 * -FU *  *  *  *  *  *  * \n"
    "P5 *  *  *  *  *  *  *  *  * \n"
    "P6 *  * +FU * +FU *  *  *  * \n"
    "P7+FU+FU * +FU * +FU+FU+FU+FU\n"
    "P8 * +KA *  *  *  *  * +HI * \n"
    "P9+KY+KE+GI+KI+OU+KI+GI+KE+KY\n"
    "P+\n"
    "P-\n"
    "-\n");

  Tree tree;
  tree.position = pos1;
  tree.ply = 0;
  tree.nodes[0].materialScore = 123;
  tree.nodes[0].score = Score::invalid();
  ASSERT_EQ(g_eval.calculateTotalScore(123, pos1), calculateStandPat(tree, g_eval));

  ASSERT_EQ(g_eval.calculateTotalScore(123, pos1), tree.nodes[0].score);
  ASSERT_EQ(g_eval.calculateTotalScore(123, pos1), calculateStandPat(tree, g_eval));

  tree.nodes[0].score = 234;
  ASSERT_EQ(234, calculateStandPat(tree, g_eval));

  tree.position = pos2;
  tree.ply = 0;
  tree.nodes[0].materialScore = 123;
  tree.nodes[0].score = Score::invalid();
  ASSERT_EQ(-g_eval.calculateTotalScore(123, pos2), calculateStandPat(tree, g_eval));

  ASSERT_EQ(g_eval.calculateTotalScore(123, pos2), tree.nodes[0].score);
  ASSERT_EQ(-g_eval.calculateTotalScore(123, pos2), calculateStandPat(tree, g_eval));

  tree.nodes[0].score = 234;
  ASSERT_EQ(-234, calculateStandPat(tree, g_eval));
}

TEST(TreeTest, testEstimateScore) {
  // TODO
}

TEST(TreeTest, testIsRecapture) {
  Move move2255 = Move(Square::s22(), Square::s55(), false);
  Move move5855 = Move(Square::s58(), Square::s55(), false);
  Move move5344 = Move(Square::s53(), Square::s44(), false);
  Move move8382 = Move(Square::s83(), Square::s82(), false);
  Move move8382p = Move(Square::s83(), Square::s82(), true);
  Move move7282 = Move(Square::s72(), Square::s82(), false);

  {
    Tree tree;
    tree.ply = 0;
    ASSERT_FALSE(isRecapture(tree, move2255));
  }

  {
    Tree tree;
    tree.ply = 3;
    tree.nodes[2].move = move5855;
    tree.nodes[2].captured = Piece::blackLance();
    ASSERT_TRUE(isRecapture(tree, move2255));
    ASSERT_FALSE(isRecapture(tree, move5344));
  }

  {
    Tree tree;
    tree.ply = 3;
    tree.nodes[2].move = move8382;
    tree.nodes[2].captured = Piece::empty();
    tree.position = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 * +FU-KI *  *  *  *  *  * \n"
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
    ASSERT_FALSE(isRecapture(tree, move7282));

    tree.nodes[2].move = move8382p;
    tree.position = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 * +TO-KI *  *  *  *  *  * \n"
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
    ASSERT_TRUE(isRecapture(tree, move7282));

    tree.nodes[2].move = move8382p;
    tree.position = PositionUtil::createPositionFromCsaString(
      "P1 *  *  *  * -OU *  *  *  * \n"
      "P2 * +NG-KI *  *  *  *  *  * \n"
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
    ASSERT_FALSE(isRecapture(tree, move7282));
  }
}

TEST(TreeTest, testTargetPiece) {
  // TODO
}
