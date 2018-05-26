/* SCRDetectorTest.cpp
 *
 * Kubo Ryosuke
 */

#include "test/Test.hpp"
#include "core/util/PositionUtil.hpp"
#include "search/tree/Tree.hpp"
#include "search/shek/SCRDetector.hpp"

using namespace sunfish;

namespace {

const char* PosStrBlackNoCheck =
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

const char* PosStrBlackCheck =
    "P1-KY-KE-GI-KI-OU-KI-GI-KE-KY\n"
    "P2 * -HI *  *  *  *  * -KA * \n"
    "P3-FU-FU-FU-FU * -FU+UM-FU-FU\n"
    "P4 *  *  *  * -FU * -FU *  * \n"
    "P5 *  *  *  *  *  *  *  *  * \n"
    "P6 *  * +FU * +FU *  *  *  * \n"
    "P7+FU+FU * +FU * +FU+FU+FU+FU\n"
    "P8 *  *  *  *  *  *  * +HI * \n"
    "P9+KY+KE+GI+KI+OU+KI+GI+KE+KY\n"
    "P+\n"
    "P-\n"
    "-\n";

const char* PosStrBlackCheck2 =
    "P1-KY-KE-GI-KI-OU-KI-GI-KE-KY\n"
    "P2 * -HI *  *  *  *  *  *  * \n"
    "P3-FU-FU-FU-FU-FU-FU * -FU-FU\n"
    "P4 *  *  *  *  *  * -FU *  * \n"
    "P5 *  *  *  *  *  *  *  *  * \n"
    "P6 *  * +FU-UM+FU *  *  *  * \n"
    "P7+FU+FU * +FU * +FU+FU+FU+FU\n"
    "P8 *  *  *  *  *  *  * +HI * \n"
    "P9+KY+KE+GI+KI+OU+KI+GI+KE+KY\n"
    "P+\n"
    "P-00KA\n"
    "-\n";

const char* PosStrWhiteNoCheck =
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

const char* PosStrWhiteCheck =
    "P1-KY-KE-GI-KI-OU-KI-GI-KE-KY\n"
    "P2 * -HI *  *  *  *  *  *  * \n"
    "P3-FU-FU-FU-FU-FU-FU * -FU-FU\n"
    "P4 *  *  *  *  *  * -FU *  * \n"
    "P5 *  *  *  *  *  *  *  *  * \n"
    "P6 *  * +FU * +FU *  *  *  * \n"
    "P7+FU+FU-UM+FU * +FU+FU+FU+FU\n"
    "P8 *  *  *  *  *  *  * +HI * \n"
    "P9+KY+KE+GI+KI+OU+KI+GI+KE+KY\n"
    "P+\n"
    "P-00KA\n"
    "+\n";

const char* PosStrWhiteCheck2 =
    "P1-KY-KE-GI-KI-OU-KI-GI-KE-KY\n"
    "P2 * -HI *  *  *  *  * -KA * \n"
    "P3-FU-FU-FU-FU * -FU * -FU-FU\n"
    "P4 *  *  *  * -FU+UM-FU *  * \n"
    "P5 *  *  *  *  *  *  *  *  * \n"
    "P6 *  * +FU * +FU *  *  *  * \n"
    "P7+FU+FU * +FU * +FU+FU+FU+FU\n"
    "P8 *  *  *  *  *  *  * +HI * \n"
    "P9+KY+KE+GI+KI+OU+KI+GI+KE+KY\n"
    "P+\n"
    "P-\n"
    "+\n";

};

TEST(SCRDetectorTest, testBlackShort) {
  {
    SCRDetector scr;
    scr.clear();

    Tree tree;
    tree.position = PositionUtil::createPositionFromCsaString(PosStrBlackNoCheck);
    tree.ply = 1;
    tree.nodes[0].checkState = { Square::invalid(), Square::invalid() };
    tree.nodes[0].hash = 0LLU;

    ASSERT_EQ(SCRState::None, scr.detectShort(tree));
  }

  {
    SCRDetector scr;
    scr.clear();

    Tree tree;
    tree.position = PositionUtil::createPositionFromCsaString(PosStrBlackNoCheck);
    tree.ply = 2;
    tree.nodes[0].checkState = { Square::invalid(), Square::invalid() };
    tree.nodes[0].hash = tree.position.getHash();
    tree.nodes[1].checkState = { Square::invalid(), Square::invalid() };
    tree.nodes[1].hash = 0LLU;

    ASSERT_EQ(SCRState::Draw, scr.detectShort(tree));
  }

  {
    SCRDetector scr;
    scr.clear();

    Tree tree;
    tree.position = PositionUtil::createPositionFromCsaString(PosStrBlackCheck);
    tree.ply = 2;
    tree.nodes[0].checkState = { Square::s73(), Square::invalid() };
    tree.nodes[0].hash = tree.position.getHash();
    tree.nodes[1].checkState = { Square::invalid(), Square::invalid() };
    tree.nodes[1].hash = 0LLU;

    ASSERT_EQ(SCRState::Win, scr.detectShort(tree));
  }

  {
    SCRDetector scr;
    scr.clear();

    Tree tree;
    tree.position = PositionUtil::createPositionFromCsaString(PosStrBlackNoCheck);
    tree.ply = 2;
    tree.nodes[0].checkState = { Square::invalid(), Square::invalid() };
    tree.nodes[0].hash = tree.position.getHash();
    tree.nodes[1].checkState = { Square::s73(), Square::invalid() };
    tree.nodes[1].hash = 0LLU;

    ASSERT_EQ(SCRState::Lose, scr.detectShort(tree));
  }
}

TEST(SCRDetectorTest, testWhiteShort) {
  {
    SCRDetector scr;
    scr.clear();

    Tree tree;
    tree.position = PositionUtil::createPositionFromCsaString(PosStrWhiteNoCheck);
    tree.ply = 1;
    tree.nodes[0].checkState = { Square::invalid(), Square::invalid() };
    tree.nodes[0].hash = 0LLU;

    ASSERT_EQ(SCRState::None, scr.detectShort(tree));
  }

  {
    SCRDetector scr;
    scr.clear();

    Tree tree;
    tree.position = PositionUtil::createPositionFromCsaString(PosStrWhiteNoCheck);
    tree.ply = 2;
    tree.nodes[0].checkState = { Square::invalid(), Square::invalid() };
    tree.nodes[0].hash = tree.position.getHash();
    tree.nodes[1].checkState = { Square::invalid(), Square::invalid() };
    tree.nodes[1].hash = 0LLU;

    ASSERT_EQ(SCRState::Draw, scr.detectShort(tree));
  }

  {
    SCRDetector scr;
    scr.clear();

    Tree tree;
    tree.position = PositionUtil::createPositionFromCsaString(PosStrWhiteCheck);
    tree.ply = 2;
    tree.nodes[0].checkState = { Square::s73(), Square::invalid() };
    tree.nodes[0].hash = tree.position.getHash();
    tree.nodes[1].checkState = { Square::invalid(), Square::invalid() };
    tree.nodes[1].hash = 0LLU;

    ASSERT_EQ(SCRState::Win, scr.detectShort(tree));
  }

  {
    SCRDetector scr;
    scr.clear();

    Tree tree;
    tree.position = PositionUtil::createPositionFromCsaString(PosStrWhiteNoCheck);
    tree.ply = 2;
    tree.nodes[0].checkState = { Square::invalid(), Square::invalid() };
    tree.nodes[0].hash = tree.position.getHash();
    tree.nodes[1].checkState = { Square::s73(), Square::invalid() };
    tree.nodes[1].hash = 0LLU;

    ASSERT_EQ(SCRState::Lose, scr.detectShort(tree));
  }
}

TEST(SCRDetectorTest, testBlack) {
  {
    SCRDetector scr;
    scr.clear();

    Tree tree;
    tree.position = PositionUtil::createPositionFromCsaString(PosStrBlackNoCheck);
    tree.ply = 1;
    tree.nodes[0].checkState = { Square::invalid(), Square::invalid() };
    tree.nodes[0].hash = 0LLU;

    ASSERT_EQ(SCRState::None, scr.detect(tree));
  }

  {
    SCRDetector scr;
    scr.clear();

    Tree tree;
    tree.position = PositionUtil::createPositionFromCsaString(PosStrBlackNoCheck);
    tree.ply = 6;
    tree.nodes[0].checkState = { Square::invalid(), Square::invalid() };
    tree.nodes[0].hash = tree.position.getHash();
    tree.nodes[1].checkState = { Square::invalid(), Square::invalid() };
    tree.nodes[1].hash = 0LLU;
    tree.nodes[2].checkState = { Square::invalid(), Square::invalid() };
    tree.nodes[2].hash = tree.position.getHash();
    tree.nodes[3].checkState = { Square::invalid(), Square::invalid() };
    tree.nodes[3].hash = 0LLU;
    tree.nodes[4].checkState = { Square::invalid(), Square::invalid() };
    tree.nodes[4].hash = tree.position.getHash();
    tree.nodes[5].checkState = { Square::invalid(), Square::invalid() };
    tree.nodes[5].hash = 0LLU;

    ASSERT_EQ(SCRState::Draw, scr.detect(tree));
  }

  {
    SCRDetector scr;
    scr.clear();

    Tree tree;
    tree.position = PositionUtil::createPositionFromCsaString(PosStrBlackCheck);
    tree.ply = 6;
    tree.nodes[0].checkState = { Square::s73(), Square::invalid() };
    tree.nodes[0].hash = tree.position.getHash();
    tree.nodes[1].checkState = { Square::invalid(), Square::invalid() };
    tree.nodes[1].hash = 0LLU;
    tree.nodes[2].checkState = { Square::s73(), Square::invalid() };
    tree.nodes[2].hash = tree.position.getHash();
    tree.nodes[3].checkState = { Square::invalid(), Square::invalid() };
    tree.nodes[3].hash = 0LLU;
    tree.nodes[4].checkState = { Square::s73(), Square::invalid() };
    tree.nodes[4].hash = tree.position.getHash();
    tree.nodes[5].checkState = { Square::invalid(), Square::invalid() };
    tree.nodes[5].hash = 0LLU;

    ASSERT_EQ(SCRState::Win, scr.detect(tree));
  }

  {
    SCRDetector scr;
    scr.clear();

    Tree tree;
    tree.position = PositionUtil::createPositionFromCsaString(PosStrBlackCheck);
    tree.ply = 6;
    tree.nodes[0].checkState = { Square::invalid(), Square::invalid() };
    tree.nodes[0].hash = tree.position.getHash();
    tree.nodes[1].checkState = { Square::s58(), Square::invalid() };
    tree.nodes[1].hash = 0LLU;
    tree.nodes[2].checkState = { Square::invalid(), Square::invalid() };
    tree.nodes[2].hash = tree.position.getHash();
    tree.nodes[3].checkState = { Square::s58(), Square::invalid() };
    tree.nodes[3].hash = 0LLU;
    tree.nodes[4].checkState = { Square::invalid(), Square::invalid() };
    tree.nodes[4].hash = tree.position.getHash();
    tree.nodes[5].checkState = { Square::s58(), Square::invalid() };
    tree.nodes[5].hash = 0LLU;

    ASSERT_EQ(SCRState::Lose, scr.detect(tree));
  }

  {
    SCRDetector scr;
    scr.clear();

    Tree tree;
    tree.position = PositionUtil::createPositionFromCsaString(PosStrBlackCheck);
    tree.ply = 8;
    tree.nodes[0].checkState = { Square::s73(), Square::invalid() };
    tree.nodes[0].hash = tree.position.getHash();
    tree.nodes[1].checkState = { Square::invalid(), Square::invalid() };
    tree.nodes[1].hash = 0LLU;
    tree.nodes[2].checkState = { Square::s73(), Square::invalid() };
    tree.nodes[2].hash = tree.position.getHash();
    tree.nodes[3].checkState = { Square::invalid(), Square::invalid() };
    tree.nodes[3].hash = 0LLU;
    tree.nodes[4].checkState = { Square::invalid(), Square::invalid() };
    tree.nodes[4].hash = 0LLU;
    tree.nodes[5].checkState = { Square::invalid(), Square::invalid() };
    tree.nodes[5].hash = 0LLU;
    tree.nodes[6].checkState = { Square::s73(), Square::invalid() };
    tree.nodes[6].hash = tree.position.getHash();
    tree.nodes[7].checkState = { Square::invalid(), Square::invalid() };
    tree.nodes[7].hash = 0LLU;

    ASSERT_EQ(SCRState::Draw, scr.detect(tree));
  }

  {
    Record record;
    record.initialPosition = PositionUtil::createPositionFromCsaString(PosStrBlackNoCheck);
    record.moveList.push_back(Move(Square::s51(), Square::s62(), false));
    record.moveList.push_back(Move(Square::s59(), Square::s68(), false));
    record.moveList.push_back(Move(Square::s62(), Square::s51(), false));
    record.moveList.push_back(Move(Square::s68(), Square::s59(), false));
    record.moveList.push_back(Move(Square::s51(), Square::s62(), false));
    record.moveList.push_back(Move(Square::s59(), Square::s68(), false));
    record.moveList.push_back(Move(Square::s62(), Square::s51(), false));
    record.moveList.push_back(Move(Square::s68(), Square::s59(), false));

    SCRDetector scr;
    scr.registerRecord(record);

    Tree tree;
    tree.position = PositionUtil::createPositionFromCsaString(PosStrBlackNoCheck);
    tree.ply = 2;
    tree.nodes[0].checkState = { Square::invalid(), Square::invalid() };
    tree.nodes[0].hash = tree.position.getHash();
    tree.nodes[1].checkState = { Square::invalid(), Square::invalid() };
    tree.nodes[1].hash = 0LLU;

    ASSERT_EQ(SCRState::Draw, scr.detect(tree));
  }

  {
    Record record;
    record.initialPosition = PositionUtil::createPositionFromCsaString(PosStrBlackCheck);
    record.moveList.push_back(Move(Square::s51(), Square::s62(), false));
    record.moveList.push_back(Move(Square::s33(), Square::s44(), false));
    record.moveList.push_back(Move(Square::s62(), Square::s51(), false));
    record.moveList.push_back(Move(Square::s44(), Square::s33(), false));
    record.moveList.push_back(Move(Square::s51(), Square::s62(), false));
    record.moveList.push_back(Move(Square::s33(), Square::s44(), false));
    record.moveList.push_back(Move(Square::s62(), Square::s51(), false));
    record.moveList.push_back(Move(Square::s44(), Square::s33(), false));

    SCRDetector scr;
    scr.registerRecord(record);

    Tree tree;
    tree.position = PositionUtil::createPositionFromCsaString(PosStrBlackCheck);
    tree.ply = 2;
    tree.nodes[0].checkState = { Square::s33(), Square::invalid() };
    tree.nodes[0].hash = tree.position.getHash();
    tree.nodes[1].checkState = { Square::invalid(), Square::invalid() };
    tree.nodes[1].hash = 0LLU;

    ASSERT_EQ(SCRState::Win, scr.detect(tree));
  }

  {
    Record record;
    record.initialPosition = PositionUtil::createPositionFromCsaString(PosStrBlackCheck2);
    record.moveList.push_back(Move(Square::s66(), Square::s77(), false));
    record.moveList.push_back(Move(Square::s59(), Square::s48(), false));
    record.moveList.push_back(Move(Square::s77(), Square::s66(), false));
    record.moveList.push_back(Move(Square::s48(), Square::s59(), false));
    record.moveList.push_back(Move(Square::s66(), Square::s77(), false));
    record.moveList.push_back(Move(Square::s59(), Square::s48(), false));
    record.moveList.push_back(Move(Square::s77(), Square::s66(), false));
    record.moveList.push_back(Move(Square::s48(), Square::s59(), false));

    SCRDetector scr;
    scr.registerRecord(record);

    Tree tree;
    tree.position = PositionUtil::createPositionFromCsaString(PosStrBlackCheck2);
    tree.ply = 2;
    tree.nodes[0].checkState = { Square::invalid(), Square::invalid() };
    tree.nodes[0].hash = 0LLU;
    tree.nodes[1].checkState = { Square::s77(), Square::invalid() };
    tree.nodes[1].hash = tree.position.getHash();

    ASSERT_EQ(SCRState::Lose, scr.detect(tree));
  }
}

TEST(SCRDetectorTest, testWhite) {
  {
    SCRDetector scr;
    scr.clear();

    Tree tree;
    tree.position = PositionUtil::createPositionFromCsaString(PosStrWhiteNoCheck);
    tree.ply = 1;
    tree.nodes[0].checkState = { Square::invalid(), Square::invalid() };
    tree.nodes[0].hash = 0LLU;

    ASSERT_EQ(SCRState::None, scr.detect(tree));
  }

  {
    SCRDetector scr;
    scr.clear();

    Tree tree;
    tree.position = PositionUtil::createPositionFromCsaString(PosStrWhiteNoCheck);
    tree.ply = 6;
    tree.nodes[0].checkState = { Square::invalid(), Square::invalid() };
    tree.nodes[0].hash = tree.position.getHash();
    tree.nodes[1].checkState = { Square::invalid(), Square::invalid() };
    tree.nodes[1].hash = 0LLU;
    tree.nodes[2].checkState = { Square::invalid(), Square::invalid() };
    tree.nodes[2].hash = tree.position.getHash();
    tree.nodes[3].checkState = { Square::invalid(), Square::invalid() };
    tree.nodes[3].hash = 0LLU;
    tree.nodes[4].checkState = { Square::invalid(), Square::invalid() };
    tree.nodes[4].hash = tree.position.getHash();
    tree.nodes[5].checkState = { Square::invalid(), Square::invalid() };
    tree.nodes[5].hash = 0LLU;

    ASSERT_EQ(SCRState::Draw, scr.detect(tree));
  }

  {
    SCRDetector scr;
    scr.clear();

    Tree tree;
    tree.position = PositionUtil::createPositionFromCsaString(PosStrWhiteCheck);
    tree.ply = 6;
    tree.nodes[0].checkState = { Square::s73(), Square::invalid() };
    tree.nodes[0].hash = tree.position.getHash();
    tree.nodes[1].checkState = { Square::invalid(), Square::invalid() };
    tree.nodes[1].hash = 0LLU;
    tree.nodes[2].checkState = { Square::s73(), Square::invalid() };
    tree.nodes[2].hash = tree.position.getHash();
    tree.nodes[3].checkState = { Square::invalid(), Square::invalid() };
    tree.nodes[3].hash = 0LLU;
    tree.nodes[4].checkState = { Square::s73(), Square::invalid() };
    tree.nodes[4].hash = tree.position.getHash();
    tree.nodes[5].checkState = { Square::invalid(), Square::invalid() };
    tree.nodes[5].hash = 0LLU;

    ASSERT_EQ(SCRState::Win, scr.detect(tree));
  }

  {
    SCRDetector scr;
    scr.clear();

    Tree tree;
    tree.position = PositionUtil::createPositionFromCsaString(PosStrWhiteCheck);
    tree.ply = 6;
    tree.nodes[0].checkState = { Square::invalid(), Square::invalid() };
    tree.nodes[0].hash = tree.position.getHash();
    tree.nodes[1].checkState = { Square::s58(), Square::invalid() };
    tree.nodes[1].hash = 0LLU;
    tree.nodes[2].checkState = { Square::invalid(), Square::invalid() };
    tree.nodes[2].hash = tree.position.getHash();
    tree.nodes[3].checkState = { Square::s58(), Square::invalid() };
    tree.nodes[3].hash = 0LLU;
    tree.nodes[4].checkState = { Square::invalid(), Square::invalid() };
    tree.nodes[4].hash = tree.position.getHash();
    tree.nodes[5].checkState = { Square::s58(), Square::invalid() };
    tree.nodes[5].hash = 0LLU;

    ASSERT_EQ(SCRState::Lose, scr.detect(tree));
  }

  {
    SCRDetector scr;
    scr.clear();

    Tree tree;
    tree.position = PositionUtil::createPositionFromCsaString(PosStrWhiteCheck);
    tree.ply = 8;
    tree.nodes[0].checkState = { Square::s73(), Square::invalid() };
    tree.nodes[0].hash = tree.position.getHash();
    tree.nodes[1].checkState = { Square::invalid(), Square::invalid() };
    tree.nodes[1].hash = 0LLU;
    tree.nodes[2].checkState = { Square::s73(), Square::invalid() };
    tree.nodes[2].hash = tree.position.getHash();
    tree.nodes[3].checkState = { Square::invalid(), Square::invalid() };
    tree.nodes[3].hash = 0LLU;
    tree.nodes[4].checkState = { Square::invalid(), Square::invalid() };
    tree.nodes[4].hash = 0LLU;
    tree.nodes[5].checkState = { Square::invalid(), Square::invalid() };
    tree.nodes[5].hash = 0LLU;
    tree.nodes[6].checkState = { Square::s73(), Square::invalid() };
    tree.nodes[6].hash = tree.position.getHash();
    tree.nodes[7].checkState = { Square::invalid(), Square::invalid() };
    tree.nodes[7].hash = 0LLU;

    ASSERT_EQ(SCRState::Draw, scr.detect(tree));
  }

  {
    Record record;
    record.initialPosition = PositionUtil::createPositionFromCsaString(PosStrWhiteCheck);
    record.moveList.push_back(Move(Square::s59(), Square::s48(), false));
    record.moveList.push_back(Move(Square::s77(), Square::s66(), false));
    record.moveList.push_back(Move(Square::s48(), Square::s59(), false));
    record.moveList.push_back(Move(Square::s66(), Square::s77(), false));
    record.moveList.push_back(Move(Square::s59(), Square::s48(), false));
    record.moveList.push_back(Move(Square::s77(), Square::s66(), false));
    record.moveList.push_back(Move(Square::s48(), Square::s59(), false));
    record.moveList.push_back(Move(Square::s66(), Square::s77(), false));

    SCRDetector scr;
    scr.registerRecord(record);

    Tree tree;
    tree.position = PositionUtil::createPositionFromCsaString(PosStrWhiteCheck);
    tree.ply = 2;
    tree.nodes[0].checkState = { Square::s77(), Square::invalid() };
    tree.nodes[0].hash = tree.position.getHash();
    tree.nodes[1].checkState = { Square::invalid(), Square::invalid() };
    tree.nodes[1].hash = 0LLU;

    ASSERT_EQ(SCRState::Win, scr.detect(tree));
  }

  {
    Record record;
    record.initialPosition = PositionUtil::createPositionFromCsaString(PosStrWhiteCheck2);
    record.moveList.push_back(Move(Square::s44(), Square::s33(), false));
    record.moveList.push_back(Move(Square::s51(), Square::s62(), false));
    record.moveList.push_back(Move(Square::s33(), Square::s44(), false));
    record.moveList.push_back(Move(Square::s62(), Square::s51(), false));
    record.moveList.push_back(Move(Square::s44(), Square::s33(), false));
    record.moveList.push_back(Move(Square::s51(), Square::s62(), false));
    record.moveList.push_back(Move(Square::s33(), Square::s44(), false));
    record.moveList.push_back(Move(Square::s62(), Square::s51(), false));

    SCRDetector scr;
    scr.registerRecord(record);

    Tree tree;
    tree.position = PositionUtil::createPositionFromCsaString(PosStrWhiteCheck2);
    tree.ply = 2;
    tree.nodes[0].checkState = { Square::invalid(), Square::invalid() };
    tree.nodes[0].hash = 0LLU;
    tree.nodes[1].checkState = { Square::s33(), Square::invalid() };
    tree.nodes[1].hash = tree.position.getHash();

    ASSERT_EQ(SCRState::Lose, scr.detect(tree));
  }
}
