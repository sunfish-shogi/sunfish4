/* TTTest.cpp
 *
 * Kubo Ryosuke
 */

#include "test/Test.hpp"
#include "search/tt/TT.hpp"
#include "core/position/Position.hpp"
#include "core/util/PositionUtil.hpp"

namespace {

using namespace sunfish;

const char* posStr1 =
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

const char* posStr2 =
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

const char* posStr3 =
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

const char* posStr4 =
  "P1-KY-KE-GI-KI-OU-KI-GI-KE-KY\n"
  "P2 * -HI *  *  *  *  * -KA * \n"
  "P3-FU-FU-FU-FU-FU-FU * -FU-FU\n"
  "P4 *  *  *  *  *  * -FU *  * \n"
  "P5 *  *  *  *  *  *  *  *  * \n"
  "P6 *  * +FU *  *  *  * +FU * \n"
  "P7+FU+FU * +FU+FU+FU+FU * +FU\n"
  "P8 * +KA *  *  *  *  * +HI * \n"
  "P9+KY+KE+GI+KI+OU+KI+GI+KE+KY\n"
  "P+\n"
  "P-\n"
  "-\n";

const char* posStr5 =
  "P1-KY-KE-GI-KI-OU-KI-GI-KE-KY\n"
  "P2 * -HI *  *  *  *  * -KA * \n"
  "P3-FU * -FU-FU-FU-FU * -FU-FU\n"
  "P4 * -FU *  *  *  * -FU *  * \n"
  "P5 *  *  *  *  *  *  *  *  * \n"
  "P6 *  * +FU *  *  *  * +FU * \n"
  "P7+FU+FU * +FU+FU+FU+FU * +FU\n"
  "P8 * +KA *  *  *  *  * +HI * \n"
  "P9+KY+KE+GI+KI+OU+KI+GI+KE+KY\n"
  "P+\n"
  "P-\n"
  "+\n";

}

using namespace sunfish;

TEST(TTTest, test) {
  TT tt;
  TTElement tte;

  Position pos1 = PositionUtil::createPositionFromCsaString(posStr1);
  Position pos2 = PositionUtil::createPositionFromCsaString(posStr2);

  tt.store(/* hash  */ pos1.getHash(),
           /* alpha */ Score(-123),
           /* beta  */ Score(456),
           /* score */ Score(77),
           /* depth */ 5,
           /* ply   */ 3,
           /* move  */ Move(Square::s77(), Square::s76(), false),
           /* mate  */ false);

  ASSERT_EQ(true , tt.get(pos1.getHash(), tte));
  ASSERT_EQ(false, tt.get(pos2.getHash(), tte));
}

TEST(TTTest, testInitialization) {
  TTElement tte;
  ASSERT_FALSE(tte.isLive());

  Position pos = PositionUtil::createPositionFromCsaString(posStr1);
  tte.update(/* hash  */ pos.getHash(),
             /* alpha */ Score(-123),
             /* beta  */ Score(456),
             /* score */ Score(77),
             /* depth */ 5,
             /* ply   */ 3,
             /* move  */ Move(Square::s77(), Square::s76(), false),
             /* mate  */ false);
  ASSERT_TRUE(tte.isLive());
}

TEST(TTTest, testElement) {
  TT tt;
  TTElement tte;

  Position pos1 = PositionUtil::createPositionFromCsaString(posStr1);
  Position pos2 = PositionUtil::createPositionFromCsaString(posStr2);
  Position pos3 = PositionUtil::createPositionFromCsaString(posStr3);
  Position pos4 = PositionUtil::createPositionFromCsaString(posStr4);
  Position pos5 = PositionUtil::createPositionFromCsaString(posStr5);

  tt.store(/* hash  */ pos1.getHash(),
           /* alpha */ Score(-123),
           /* beta  */ Score(456),
           /* score */ Score(77),
           /* depth */ 5,
           /* ply   */ 3,
           /* move  */ Move(Square::s77(), Square::s76(), false),
           /* mate  */ false);

  tt.store(/* hash  */ pos2.getHash(),
           /* alpha */ Score(-123),
           /* beta  */ Score(456),
           /* score */ Score(517),
           /* depth */ -2,
           /* ply   */ 3,
           /* move  */ Move(Square::s33(), Square::s34(), false),
           /* mate  */ false);

  tt.store(/* hash  */ pos3.getHash(),
           /* alpha */ Score(-123),
           /* beta  */ Score(456),
           /* score */ Score(-298),
           /* depth */ 0,
           /* ply   */ 3,
           /* move  */ Move(Square::s27(), Square::s26(), false),
           /* mate  */ true);

  tt.store(/* hash  */ pos4.getHash(),
           /* alpha */ Score(-123),
           /* beta  */ Score(456),
           /* score */ Score::infinity() - 7,
           /* depth */ 5,
           /* ply   */ 3,
           /* move  */ Move(Square::s26(), Square::s25(), false),
           /* mate  */ false);

  tt.store(/* hash  */ pos5.getHash(),
           /* alpha */ Score(-123),
           /* beta  */ Score(456),
           /* score */ -Score::infinity() + 5,
           /* depth */ 5,
           /* ply   */ 4,
           /* move  */ Move(Square::s84(), Square::s85(), false),
           /* mate  */ false);

  bool success = tt.get(pos1.getHash(), tte);
  ASSERT_EQ(true, success);
  ASSERT_EQ(Score(77), tte.score(8));
  ASSERT_EQ(TTScoreType::Exact, tte.scoreType());
  ASSERT_EQ(5, tte.depth());
  ASSERT_EQ(Move(Square::s77(), Square::s76(), false), tte.move());
  ASSERT_EQ(false, tte.isMateThreat());

  success = tt.get(pos2.getHash(), tte);
  ASSERT_EQ(true, success);
  ASSERT_EQ(Score(517), tte.score(8));
  ASSERT_EQ(TTScoreType::Lower, tte.scoreType());
  ASSERT_EQ(0, tte.depth());
  ASSERT_EQ(Move(Square::s33(), Square::s34(), false), tte.move());
  ASSERT_EQ(false, tte.isMateThreat());

  success = tt.get(pos3.getHash(), tte);
  ASSERT_EQ(true, success);
  ASSERT_EQ(Score(-298), tte.score(8));
  ASSERT_EQ(TTScoreType::Upper, tte.scoreType());
  ASSERT_EQ(0, tte.depth());
  ASSERT_EQ(Move(Square::s27(), Square::s26(), false), tte.move());
  ASSERT_EQ(true, tte.isMateThreat());

  success = tt.get(pos4.getHash(), tte);
  ASSERT_EQ(true, success);
  ASSERT_EQ(Score::infinity() - 8, tte.score(4));
  ASSERT_EQ(TTScoreType::Lower, tte.scoreType());

  success = tt.get(pos5.getHash(), tte);
  ASSERT_EQ(true, success);
  ASSERT_EQ(-Score::infinity() + 6, tte.score(5));
  ASSERT_EQ(TTScoreType::Upper, tte.scoreType());
}

TEST(TTTest, testUpdate) {
  TT tt;
  TTElement tte;
  TTStatus tts;

  Position pos1 = PositionUtil::createPositionFromCsaString(posStr1);

  // first
  tts = tt.store(/* hash  */ pos1.getHash(),
                 /* alpha */ Score(-123),
                 /* beta  */ Score(456),
                 /* score */ Score(77),
                 /* depth */ 5,
                 /* ply   */ 3,
                 /* move  */ Move(Square::s77(), Square::s76(), false),
                 /* mate  */ false);
  ASSERT_TRUE(TTStatus::New == tts);

  // shallow
  tts = tt.store(/* hash  */ pos1.getHash(),
                 /* alpha */ Score(-123),
                 /* beta  */ Score(456),
                 /* score */ Score(77),
                 /* depth */ 3,
                 /* ply   */ 3,
                 /* move  */ Move(Square::s77(), Square::s76(), false),
                 /* mate  */ false);
  ASSERT_TRUE(TTStatus::Reject == tts);

  // same depth
  tts = tt.store(/* hash  */ pos1.getHash(),
                 /* alpha */ Score(-123),
                 /* beta  */ Score(456),
                 /* score */ Score(77),
                 /* depth */ 5,
                 /* ply   */ 3,
                 /* move  */ Move(Square::s77(), Square::s76(), false),
                 /* mate  */ false);
  ASSERT_TRUE(TTStatus::Update == tts);

  // deep
  tts = tt.store(/* hash  */ pos1.getHash(),
                 /* alpha */ Score(-123),
                 /* beta  */ Score(456),
                 /* score */ Score(77),
                 /* depth */ 7,
                 /* ply   */ 3,
                 /* move  */ Move(Square::s77(), Square::s76(), false),
                 /* mate  */ false);
  ASSERT_TRUE(TTStatus::Update == tts);
}
