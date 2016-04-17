/* BookGeneratorTest.cpp
 *
 * Kubo Ryosuke
 */

#include "test/Test.hpp"
#include "book/BookGenerator.hpp"
#include "core/position/Position.hpp"
#include "core/record/SfenParser.hpp"

using namespace sunfish;

TEST(BookGeneratorTest, test) {
  {
    BookGenerator bookGenerator("kifu/test/book_gen");
    bookGenerator.setLimit(20);

    bool ok = bookGenerator.generate();
    ASSERT_TRUE(ok);

    Book book = bookGenerator.getBook();

    std::string sfen = "lnsgkgsnl/1r5b1/ppppppppp/9/9/9/PPPPPPPPP/1B5R1/LNSGKGSNL b - 1";
    Position pos;
    SfenParser::parsePosition(sfen, pos);

    auto pbm = book.get(pos);
    ASSERT_TRUE(pbm != nullptr);
    ASSERT_EQ(1, pbm->size());
    ASSERT_EQ(Move(Square::s77(), Square::s76(), false), pbm->at(0).move);
    ASSERT_EQ(3, pbm->at(0).count);

    sfen = "lnsgkgsnl/1r5b1/p1ppppppp/1p7/9/2P6/PP1PPPPPP/1B5R1/LNSGKGSNL b - 1";
    SfenParser::parsePosition(sfen, pos);

    pbm = book.get(pos);
    ASSERT_TRUE(pbm != nullptr);
    ASSERT_EQ(3, pbm->size());

    sfen = "ln1g1gsnl/1r3k3/p1ppsp1p1/4p1p1p/1p7/2P1P3P/PPSP1PPP1/4R2K1/LN1G1GSNL w Bb 1";
    SfenParser::parsePosition(sfen, pos);

    pbm = book.get(pos);
    ASSERT_TRUE(pbm != nullptr);
    ASSERT_EQ(1, pbm->size());

    sfen = "ln1g1gsnl/1r3k3/p1pp1p1p1/3sp1p1p/1p7/2P1P3P/PPSP1PPP1/4R2K1/LN1G1GSNL b Bb 1";
    SfenParser::parsePosition(sfen, pos);

    pbm = book.get(pos);
    ASSERT_TRUE(pbm == nullptr);
  }

  {
    BookGenerator bookGenerator("kifu/test/book_gen/kifu001.csa");
    bookGenerator.setLimit(20);

    bool ok = bookGenerator.generate();
    ASSERT_TRUE(ok);

    Book book = bookGenerator.getBook();

    std::string sfen = "lnsgkgsnl/1r5b1/ppppppppp/9/9/9/PPPPPPPPP/1B5R1/LNSGKGSNL b - 1";
    Position pos;
    SfenParser::parsePosition(sfen, pos);

    auto pbm = book.get(pos);
    ASSERT_TRUE(pbm != nullptr);
    ASSERT_EQ(1, pbm->size());
    ASSERT_EQ(Move(Square::s77(), Square::s76(), false), pbm->at(0).move);
    ASSERT_EQ(1, pbm->at(0).count);

    sfen = "lnsgkgsnl/1r5b1/p1ppppppp/1p7/9/2P6/PP1PPPPPP/1B5R1/LNSGKGSNL b - 1";
    SfenParser::parsePosition(sfen, pos);

    pbm = book.get(pos);
    ASSERT_TRUE(pbm != nullptr);
    ASSERT_EQ(1, pbm->size());
    ASSERT_EQ(Move(Square::s79(), Square::s68(), false), pbm->at(0).move);
    ASSERT_EQ(1, pbm->at(0).count);
  }
}
