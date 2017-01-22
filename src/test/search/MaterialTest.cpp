/* MaterialTest.cpp
 *
 * Kubo Ryosuke
 */

#include "test/Test.hpp"
#include "search/eval/Material.hpp"

using namespace sunfish;

TEST(MaterialTest, test) {
  ASSERT_TRUE(material::pawn() < material::lance());
  ASSERT_TRUE(material::lance() < material::knight());
  ASSERT_TRUE(material::knight() < material::silver());
  ASSERT_TRUE(material::silver() < material::gold());
  ASSERT_TRUE(material::gold() < material::bishop());
  ASSERT_TRUE(material::bishop() < material::rook());

  ASSERT_TRUE(material::tokin() > material::knight());
  ASSERT_TRUE(material::proLance() > material::knight());
  ASSERT_TRUE(material::proKnight() > material::knight());
  ASSERT_TRUE(material::proSilver() > material::knight());
  ASSERT_TRUE(material::tokin() < material::bishop());
  ASSERT_TRUE(material::proLance() < material::bishop());
  ASSERT_TRUE(material::proKnight() < material::bishop());
  ASSERT_TRUE(material::proSilver() < material::bishop());
  ASSERT_TRUE(material::horse() > material::bishop());
  ASSERT_TRUE(material::dragon() > material::rook());
}

TEST(MaterialTest, testScore) {
  ASSERT_EQ(material::pawn()           , material::score(Piece::blackPawn()));
  ASSERT_EQ(material::lance()          , material::score(Piece::blackLance()));
  ASSERT_EQ(material::knight()         , material::score(Piece::blackKnight()));
  ASSERT_EQ(material::silver()         , material::score(Piece::blackSilver()));
  ASSERT_EQ(material::gold()           , material::score(Piece::blackGold()));
  ASSERT_EQ(material::bishop()         , material::score(Piece::blackBishop()));
  ASSERT_EQ(material::rook()           , material::score(Piece::blackRook()));
  ASSERT_EQ(Score::materialInfinity(), material::score(Piece::blackKing()));
  ASSERT_EQ(material::tokin()          , material::score(Piece::blackTokin()));
  ASSERT_EQ(material::proLance()       , material::score(Piece::blackProLance()));
  ASSERT_EQ(material::knight()         , material::score(Piece::blackKnight()));
  ASSERT_EQ(material::silver()         , material::score(Piece::blackSilver()));
  ASSERT_EQ(material::horse()          , material::score(Piece::blackHorse()));
  ASSERT_EQ(material::dragon()         , material::score(Piece::blackDragon()));

  ASSERT_EQ(material::pawn()           , material::score(Piece::whitePawn()));
  ASSERT_EQ(material::lance()          , material::score(Piece::whiteLance()));
  ASSERT_EQ(material::knight()         , material::score(Piece::whiteKnight()));
  ASSERT_EQ(material::silver()         , material::score(Piece::whiteSilver()));
  ASSERT_EQ(material::gold()           , material::score(Piece::whiteGold()));
  ASSERT_EQ(material::bishop()         , material::score(Piece::whiteBishop()));
  ASSERT_EQ(material::rook()           , material::score(Piece::whiteRook()));
  ASSERT_EQ(Score::materialInfinity(), material::score(Piece::whiteKing()));
  ASSERT_EQ(material::tokin()          , material::score(Piece::whiteTokin()));
  ASSERT_EQ(material::proLance()       , material::score(Piece::whiteProLance()));
  ASSERT_EQ(material::knight()         , material::score(Piece::whiteKnight()));
  ASSERT_EQ(material::silver()         , material::score(Piece::whiteSilver()));
  ASSERT_EQ(material::horse()          , material::score(Piece::whiteHorse()));
  ASSERT_EQ(material::dragon()         , material::score(Piece::whiteDragon()));
}

TEST(MaterialTest, testExchangeScore) {
  ASSERT_EQ(material::pawn()     * 2                , material::exchangeScore(Piece::blackPawn()));
  ASSERT_EQ(material::lance()    * 2                , material::exchangeScore(Piece::blackLance()));
  ASSERT_EQ(material::knight()   * 2                , material::exchangeScore(Piece::blackKnight()));
  ASSERT_EQ(material::silver()   * 2                , material::exchangeScore(Piece::blackSilver()));
  ASSERT_EQ(material::gold()     * 2                , material::exchangeScore(Piece::blackGold()));
  ASSERT_EQ(material::bishop()   * 2                , material::exchangeScore(Piece::blackBishop()));
  ASSERT_EQ(material::rook()     * 2                , material::exchangeScore(Piece::blackRook()));
  ASSERT_EQ(Score::materialInfinity()             , material::exchangeScore(Piece::blackKing()));
  ASSERT_EQ(material::tokin()     + material::pawn()  , material::exchangeScore(Piece::blackTokin()));
  ASSERT_EQ(material::proLance()  + material::lance() , material::exchangeScore(Piece::blackProLance()));
  ASSERT_EQ(material::proKnight() + material::knight(), material::exchangeScore(Piece::blackProKnight()));
  ASSERT_EQ(material::proSilver() + material::silver(), material::exchangeScore(Piece::blackProSilver()));
  ASSERT_EQ(material::horse()     + material::bishop(), material::exchangeScore(Piece::blackHorse()));
  ASSERT_EQ(material::dragon()    + material::rook()  , material::exchangeScore(Piece::blackDragon()));

  ASSERT_EQ(material::pawn()     * 2                , material::exchangeScore(Piece::whitePawn()));
  ASSERT_EQ(material::lance()    * 2                , material::exchangeScore(Piece::whiteLance()));
  ASSERT_EQ(material::knight()   * 2                , material::exchangeScore(Piece::whiteKnight()));
  ASSERT_EQ(material::silver()   * 2                , material::exchangeScore(Piece::whiteSilver()));
  ASSERT_EQ(material::gold()     * 2                , material::exchangeScore(Piece::whiteGold()));
  ASSERT_EQ(material::bishop()   * 2                , material::exchangeScore(Piece::whiteBishop()));
  ASSERT_EQ(material::rook()     * 2                , material::exchangeScore(Piece::whiteRook()));
  ASSERT_EQ(Score::materialInfinity()             , material::exchangeScore(Piece::whiteKing()));
  ASSERT_EQ(material::tokin()     + material::pawn()  , material::exchangeScore(Piece::whiteTokin()));
  ASSERT_EQ(material::proLance()  + material::lance() , material::exchangeScore(Piece::whiteProLance()));
  ASSERT_EQ(material::proKnight() + material::knight(), material::exchangeScore(Piece::whiteProKnight()));
  ASSERT_EQ(material::proSilver() + material::silver(), material::exchangeScore(Piece::whiteProSilver()));
  ASSERT_EQ(material::horse()     + material::bishop(), material::exchangeScore(Piece::whiteHorse()));
  ASSERT_EQ(material::dragon()    + material::rook()  , material::exchangeScore(Piece::whiteDragon()));
}

TEST(MaterialTest, testPromotionScore) {
  ASSERT_EQ(material::tokin()     - material::pawn()  , material::promotionScore(Piece::blackPawn()));
  ASSERT_EQ(material::proLance()  - material::lance() , material::promotionScore(Piece::blackLance()));
  ASSERT_EQ(material::proKnight() - material::knight(), material::promotionScore(Piece::blackKnight()));
  ASSERT_EQ(material::proSilver() - material::silver(), material::promotionScore(Piece::blackSilver()));
  ASSERT_EQ(0                                     , material::promotionScore(Piece::blackGold()));
  ASSERT_EQ(material::horse()     - material::bishop(), material::promotionScore(Piece::blackBishop()));
  ASSERT_EQ(material::dragon()    - material::rook()  , material::promotionScore(Piece::blackRook()));
  ASSERT_EQ(0                                     , material::promotionScore(Piece::blackKing()));
  ASSERT_EQ(material::tokin()     - material::pawn()  , material::promotionScore(Piece::blackTokin()));
  ASSERT_EQ(material::proLance()  - material::lance() , material::promotionScore(Piece::blackProLance()));
  ASSERT_EQ(material::proKnight() - material::knight(), material::promotionScore(Piece::blackProKnight()));
  ASSERT_EQ(material::proSilver() - material::silver(), material::promotionScore(Piece::blackProSilver()));
  ASSERT_EQ(material::horse()     - material::bishop(), material::promotionScore(Piece::blackHorse()));
  ASSERT_EQ(material::dragon()    - material::rook()  , material::promotionScore(Piece::blackDragon()));

  ASSERT_EQ(material::tokin()     - material::pawn()  , material::promotionScore(Piece::whitePawn()));
  ASSERT_EQ(material::proLance()  - material::lance() , material::promotionScore(Piece::whiteLance()));
  ASSERT_EQ(material::proKnight() - material::knight(), material::promotionScore(Piece::whiteKnight()));
  ASSERT_EQ(material::proSilver() - material::silver(), material::promotionScore(Piece::whiteSilver()));
  ASSERT_EQ(0                                     , material::promotionScore(Piece::whiteGold()));
  ASSERT_EQ(material::horse()     - material::bishop(), material::promotionScore(Piece::whiteBishop()));
  ASSERT_EQ(material::dragon()    - material::rook()  , material::promotionScore(Piece::whiteRook()));
  ASSERT_EQ(0                                     , material::promotionScore(Piece::whiteKing()));
  ASSERT_EQ(material::tokin()     - material::pawn()  , material::promotionScore(Piece::whiteTokin()));
  ASSERT_EQ(material::proLance()  - material::lance() , material::promotionScore(Piece::whiteProLance()));
  ASSERT_EQ(material::proKnight() - material::knight(), material::promotionScore(Piece::whiteProKnight()));
  ASSERT_EQ(material::proSilver() - material::silver(), material::promotionScore(Piece::whiteProSilver()));
  ASSERT_EQ(material::horse()     - material::bishop(), material::promotionScore(Piece::whiteHorse()));
  ASSERT_EQ(material::dragon()    - material::rook()  , material::promotionScore(Piece::whiteDragon()));
}
