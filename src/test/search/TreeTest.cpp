/* TreeTest.cpp
 *
 * Kubo Ryosuke
 */

#include "test/Test.hpp"
#include "search/tree/Tree.hpp"

using namespace sunfish;

TEST(TreeTest, testSortKiller) {
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
