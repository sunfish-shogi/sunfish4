/* TTTest.cpp
 *
 * Kubo Ryosuke
 */

#if !defined(NDEBUG)

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

}

using namespace sunfish;

TEST(TTTest, test) {
  TT tt;
  TTElement tte;

  Position pos1 = PositionUtil::createPositionFromCsaString(posStr1);
  Position pos2 = PositionUtil::createPositionFromCsaString(posStr2);
  Position pos3 = PositionUtil::createPositionFromCsaString(posStr3);

  tt.store(pos1.getHash(),
           Score(-123), // alpha
           Score(456), // beta
           Score(77), // score
           5, // depth
           3, // ply
           Move(Square::s77(), Square::s76(), false));

  tt.storePV(pos2.getHash(),
             7, // depth
             Move(Square::s33(), Square::s34(), false));

  ASSERT_EQ(true , tt.get(pos1.getHash(), tte));
  ASSERT_EQ(true , tt.get(pos2.getHash(), tte));
  ASSERT_EQ(false, tt.get(pos3.getHash(), tte));
}

TEST(TTTest, testElement) {
  TT tt;
  TTElement tte;

  Position pos1 = PositionUtil::createPositionFromCsaString(posStr1);
  Position pos2 = PositionUtil::createPositionFromCsaString(posStr2);
  Position pos3 = PositionUtil::createPositionFromCsaString(posStr3);
  Position pos4 = PositionUtil::createPositionFromCsaString(posStr4);

  tt.store(pos1.getHash(),
           Score(-123), // alpha
           Score(456), // beta
           Score(77), // score
           5, // depth
           3, // ply
           Move(Square::s77(), Square::s76(), false));

  tt.store(pos2.getHash(),
           Score(-123), // alpha
           Score(456), // beta
           Score(517), // score
           -2, // depth
           3, // ply
           Move(Square::s33(), Square::s34(), false));

  tt.store(pos3.getHash(),
           Score(-123), // alpha
           Score(456), // beta
           Score(-298), // score
           0, // depth
           3, // ply
           Move(Square::s27(), Square::s26(), false));

  tt.storePV(pos4.getHash(),
             7, // depth
             Move(Square::s83(), Square::s84(), false));

  bool success = tt.get(pos1.getHash(), tte);
  ASSERT_EQ(true, success);
  ASSERT_EQ(Score(77), tte.score(8));
  ASSERT_EQ(TTScoreType::Exact, tte.scoreType());
  ASSERT_EQ(5, tte.depth());
  ASSERT_EQ(Move(Square::s77(), Square::s76(), false), tte.move());

  success = tt.get(pos2.getHash(), tte);
  ASSERT_EQ(true, success);
  ASSERT_EQ(Score(517), tte.score(8));
  ASSERT_EQ(TTScoreType::Lower, tte.scoreType());
  ASSERT_EQ(0, tte.depth());
  ASSERT_EQ(Move(Square::s33(), Square::s34(), false), tte.move());

  success = tt.get(pos3.getHash(), tte);
  ASSERT_EQ(true, success);
  ASSERT_EQ(Score(-298), tte.score(8));
  ASSERT_EQ(TTScoreType::Upper, tte.scoreType());
  ASSERT_EQ(0, tte.depth());
  ASSERT_EQ(Move(Square::s27(), Square::s26(), false), tte.move());

  success = tt.get(pos4.getHash(), tte);
  ASSERT_EQ(true, success);
  ASSERT_EQ(TTScoreType::None, tte.scoreType());
  ASSERT_EQ(7, tte.depth());
  ASSERT_EQ(Move(Square::s83(), Square::s84(), false), tte.move());
}

TEST(TTTest, testUpdate) {
  TT tt;
  TTElement tte;
  TTStatus tts;

  Position pos1 = PositionUtil::createPositionFromCsaString(posStr1);

  // first
  tts = tt.store(pos1.getHash(),
                 Score(-123), // alpha
                 Score(456), // beta
                 Score(77), // score
                 5, // depth
                 3, // ply
                 Move(Square::s77(), Square::s76(), false));
  ASSERT_TRUE(TTStatus::New == tts);

  // shallow
  tts = tt.store(pos1.getHash(),
                 Score(-123), // alpha
                 Score(456), // beta
                 Score(77), // score
                 3, // depth
                 3, // ply
                 Move(Square::s77(), Square::s76(), false));
  ASSERT_TRUE(TTStatus::Reject == tts);

  // same depth
  tts = tt.store(pos1.getHash(),
                 Score(-123), // alpha
                 Score(456), // beta
                 Score(77), // score
                 5, // depth
                 3, // ply
                 Move(Square::s77(), Square::s76(), false));
  ASSERT_TRUE(TTStatus::Update == tts);

  // deep
  tts = tt.store(pos1.getHash(),
                 Score(-123), // alpha
                 Score(456), // beta
                 Score(77), // score
                 7, // depth
                 3, // ply
                 Move(Square::s77(), Square::s76(), false));
  ASSERT_TRUE(TTStatus::Update == tts);
}

#endif // !defined(NDEBUG)
