/* KifuWriterTest.cpp
 *
 * Kubo Ryosuke
 */

#include "test/Test.hpp"
#include "core/record/KifuWriter.hpp"
#include <sstream>

using namespace sunfish;

TEST(KifuWriterTest, testWrite) {
  {
    Record record = {
      Position(Position::Handicap::Even),
      {
        Move(Square::s77(), Square::s76(), false),
        Move(Square::s33(), Square::s34(), false),
        Move(Square::s88(), Square::s22(), true),
        Move(Square::s31(), Square::s22(), false),
        Move(PieceType::bishop(), Square::s45()),
        Move(Square::s71(), Square::s62(), false),
      },
      ""
    };
    RecordInfo ri;
    ri.title = "WCSC2016";
    ri.site = "Waseda";
    ri.opening = "Kakugawari";
    ri.startTime = "2019/01/02";
    ri.endTime = "2019/01/03";
    ri.blackName = "Sunfish";
    ri.whiteName = "Firefly";
    ri.timeLimitHours= 9;
    ri.timeLimitMinutes = 30;
    ri.timeLimitReadoff = 60;

    const char* expect =
      "棋戦：WCSC2016\n"
      "場所：Waseda\n"
      "戦型：Kakugawari\n"
      "開始日時：2019/01/02\n"
      "終了日時：2019/01/03\n"
      "先手：Sunfish\n"
      "後手：Firefly\n"
      "持ち時間：09:30+60\n"
      "後手の持駒：なし\n"
      "  ９ ８ ７ ６ ５ ４ ３ ２ １\n"
      "+---------------------------+\n"
      "P1v香v桂v銀v金v玉v金v銀v桂v香\n"
      "P2 ・v飛 ・ ・ ・ ・ ・v角 ・\n"
      "P3v歩v歩v歩v歩v歩v歩v歩v歩v歩\n"
      "P4 ・ ・ ・ ・ ・ ・ ・ ・ ・\n"
      "P5 ・ ・ ・ ・ ・ ・ ・ ・ ・\n"
      "P6 ・ ・ ・ ・ ・ ・ ・ ・ ・\n"
      "P7 歩 歩 歩 歩 歩 歩 歩 歩 歩\n"
      "P8 ・ 角 ・ ・ ・ ・ ・ 飛 ・\n"
      "P9 香 桂 銀 金 玉 金 銀 桂 香\n"
      "+---------------------------+\n"
      "先手の持駒：なし\n"
      "1 ７六歩(77)\n"
      "2 ３四歩(33)\n"
      "3 ２二角成(88)\n"
      "4 ２二銀(31)\n"
      "5 ４五角打\n"
      "6 ６二銀(71)\n";

    std::ostringstream oss;
    bool ok = KifuWriter::write(oss, record, &ri);

    ASSERT_TRUE(ok);
    ASSERT_EQ(expect, oss.str());
  }

  {
    Record record = {
      Position(Position::Handicap::Even),
      {
        Move(Square::s77(), Square::s76(), false),
        Move(Square::s33(), Square::s34(), false),
        Move(Square::s88(), Square::s22(), true),
        Move(Square::s31(), Square::s22(), false),
        Move(PieceType::bishop(), Square::s45()),
        Move(Square::s71(), Square::s62(), false),
      },
      "%TORYO"
    };
    RecordInfo ri;
    ri.title = "WCSC2016";
    ri.site = "Waseda";
    ri.opening = "Kakugawari";
    ri.startTime = "2019/01/02";
    ri.endTime = "2019/01/03";
    ri.blackName = "Sunfish";
    ri.whiteName = "Firefly";
    ri.timeLimitHours= 9;
    ri.timeLimitMinutes = 30;
    ri.timeLimitReadoff = 60;

    const char* expect =
      "棋戦：WCSC2016\n"
      "場所：Waseda\n"
      "戦型：Kakugawari\n"
      "開始日時：2019/01/02\n"
      "終了日時：2019/01/03\n"
      "先手：Sunfish\n"
      "後手：Firefly\n"
      "持ち時間：09:30+60\n"
      "後手の持駒：なし\n"
      "  ９ ８ ７ ６ ５ ４ ３ ２ １\n"
      "+---------------------------+\n"
      "P1v香v桂v銀v金v玉v金v銀v桂v香\n"
      "P2 ・v飛 ・ ・ ・ ・ ・v角 ・\n"
      "P3v歩v歩v歩v歩v歩v歩v歩v歩v歩\n"
      "P4 ・ ・ ・ ・ ・ ・ ・ ・ ・\n"
      "P5 ・ ・ ・ ・ ・ ・ ・ ・ ・\n"
      "P6 ・ ・ ・ ・ ・ ・ ・ ・ ・\n"
      "P7 歩 歩 歩 歩 歩 歩 歩 歩 歩\n"
      "P8 ・ 角 ・ ・ ・ ・ ・ 飛 ・\n"
      "P9 香 桂 銀 金 玉 金 銀 桂 香\n"
      "+---------------------------+\n"
      "先手の持駒：なし\n"
      "1 ７六歩(77)\n"
      "2 ３四歩(33)\n"
      "3 ２二角成(88)\n"
      "4 ２二銀(31)\n"
      "5 ４五角打\n"
      "6 ６二銀(71)\n"
      "7 投了\n";

    std::ostringstream oss;
    bool ok = KifuWriter::write(oss, record, &ri);

    ASSERT_TRUE(ok);
    ASSERT_EQ(expect, oss.str());
  }
}
