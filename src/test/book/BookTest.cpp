/* BookTest.cpp
 *
 * Kubo Ryosuke
 */

#include "test/Test.hpp"
#include "book/Book.hpp"
#include "core/position/Position.hpp"
#include "core/record/SfenParser.hpp"
#include <sstream>

using namespace sunfish;

TEST(BookTest, test) {
  Book book;

  std::string sfen = "lnsgkgsnl/1r5b1/ppppppppp/9/9/9/PPPPPPPPP/1B5R1/LNSGKGSNL b - 1";
  Position pos1;
  SfenParser::parsePosition(sfen, pos1);

  book.insert(pos1, Move(Square::s77(), Square::s76(), false));
  book.insert(pos1, Move(Square::s77(), Square::s76(), false));
  book.insert(pos1, Move(Square::s27(), Square::s26(), false));
  book.insert(pos1, Move(Square::s57(), Square::s56(), false));

  sfen = "ln1gkgsnl/1r1s3b1/p1pp1p1pp/1p2p1p2/9/2PPP4/PP3PPPP/1B1S3R1/LN1GKGSNL b - 1";
  Position pos2;
  SfenParser::parsePosition(sfen, pos2);

  book.insert(pos2, Move(Square::s39(), Square::s48(), false), 5);
  book.insert(pos2, Move(Square::s39(), Square::s48(), false), 2);
  book.insert(pos2, Move(Square::s28(), Square::s58(), false), 3);

  sfen = "ln1gkg1nl/1r1s2sb1/p1pp1p1pp/1p2p1p2/9/2PPP4/PP3PPPP/1B1SGS1R1/LN1GK2NL w - 1";
  Position pos3;
  SfenParser::parsePosition(sfen, pos3);

  auto pbm = book.get(pos1);
  ASSERT_TRUE(pbm != nullptr);
  ASSERT_EQ(3, pbm->size());
  ASSERT_EQ(Move(Square::s77(), Square::s76(), false), pbm->at(0).move);
  ASSERT_EQ(2, pbm->at(0).count);
  ASSERT_EQ(Move(Square::s27(), Square::s26(), false), pbm->at(1).move);
  ASSERT_EQ(1, pbm->at(1).count);
  ASSERT_EQ(Move(Square::s57(), Square::s56(), false), pbm->at(2).move);
  ASSERT_EQ(1, pbm->at(2).count);

  pbm = book.get(pos2);
  ASSERT_TRUE(pbm != nullptr);
  ASSERT_EQ(2, pbm->size());
  ASSERT_EQ(Move(Square::s39(), Square::s48(), false), pbm->at(0).move);
  ASSERT_EQ(7, pbm->at(0).count);
  ASSERT_EQ(Move(Square::s28(), Square::s58(), false), pbm->at(1).move);
  ASSERT_EQ(3, pbm->at(1).count);

  pbm = book.get(pos3);
  ASSERT_TRUE(pbm == nullptr);

  std::ostringstream oss;
  book.save(oss);

  std::istringstream iss(oss.str());
  Book book2;
  book2.load(iss);

  pbm = book2.get(pos1);
  ASSERT_TRUE(pbm != nullptr);
  ASSERT_EQ(3, pbm->size());
  ASSERT_EQ(Move(Square::s77(), Square::s76(), false), pbm->at(0).move);
  ASSERT_EQ(2, pbm->at(0).count);
  ASSERT_EQ(Move(Square::s27(), Square::s26(), false), pbm->at(1).move);
  ASSERT_EQ(1, pbm->at(1).count);
  ASSERT_EQ(Move(Square::s57(), Square::s56(), false), pbm->at(2).move);
  ASSERT_EQ(1, pbm->at(2).count);

  pbm = book2.get(pos2);
  ASSERT_TRUE(pbm != nullptr);
  ASSERT_EQ(2, pbm->size());
  ASSERT_EQ(Move(Square::s39(), Square::s48(), false), pbm->at(0).move);
  ASSERT_EQ(7, pbm->at(0).count);
  ASSERT_EQ(Move(Square::s28(), Square::s58(), false), pbm->at(1).move);
  ASSERT_EQ(3, pbm->at(1).count);
}

