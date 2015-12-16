/* MaterialTest.cpp
 *
 * Kubo Ryosuke
 */

#if !defined(NDEBUG)

#include "test/Test.hpp"
#include "search/eval/Material.hpp"

using namespace sunfish;

TEST(MaterialTest, test) {
  ASSERT(material::Pawn < material::Lance);
  ASSERT(material::Lance < material::Knight);
  ASSERT(material::Knight < material::Silver);
  ASSERT(material::Silver < material::Gold);
  ASSERT(material::Gold < material::Bishop);
  ASSERT(material::Bishop < material::Rook);

  ASSERT(material::Tokin > material::Silver);
  ASSERT(material::ProLance > material::Silver);
  ASSERT(material::ProKnight > material::Silver);
  ASSERT(material::ProSilver > material::Silver);
  ASSERT(material::Tokin < material::Bishop);
  ASSERT(material::ProLance < material::Bishop);
  ASSERT(material::ProKnight < material::Bishop);
  ASSERT(material::ProSilver < material::Bishop);
  ASSERT(material::Horse > material::Bishop);
  ASSERT(material::Dragon > material::Rook);
}

TEST(MaterialTest, testScore) {
  ASSERT_EQ(material::Pawn           , material::score(Piece::blackPawn()));
  ASSERT_EQ(material::Lance          , material::score(Piece::blackLance()));
  ASSERT_EQ(material::Knight         , material::score(Piece::blackKnight()));
  ASSERT_EQ(material::Silver         , material::score(Piece::blackSilver()));
  ASSERT_EQ(material::Gold           , material::score(Piece::blackGold()));
  ASSERT_EQ(material::Bishop         , material::score(Piece::blackBishop()));
  ASSERT_EQ(material::Rook           , material::score(Piece::blackRook()));
  ASSERT_EQ(Score::materialInfinity(), material::score(Piece::blackKing()));
  ASSERT_EQ(material::Tokin          , material::score(Piece::blackTokin()));
  ASSERT_EQ(material::ProLance       , material::score(Piece::blackProLance()));
  ASSERT_EQ(material::Knight         , material::score(Piece::blackKnight()));
  ASSERT_EQ(material::Silver         , material::score(Piece::blackSilver()));
  ASSERT_EQ(material::Horse          , material::score(Piece::blackHorse()));
  ASSERT_EQ(material::Dragon         , material::score(Piece::blackDragon()));

  ASSERT_EQ(material::Pawn           , material::score(Piece::whitePawn()));
  ASSERT_EQ(material::Lance          , material::score(Piece::whiteLance()));
  ASSERT_EQ(material::Knight         , material::score(Piece::whiteKnight()));
  ASSERT_EQ(material::Silver         , material::score(Piece::whiteSilver()));
  ASSERT_EQ(material::Gold           , material::score(Piece::whiteGold()));
  ASSERT_EQ(material::Bishop         , material::score(Piece::whiteBishop()));
  ASSERT_EQ(material::Rook           , material::score(Piece::whiteRook()));
  ASSERT_EQ(Score::materialInfinity(), material::score(Piece::whiteKing()));
  ASSERT_EQ(material::Tokin          , material::score(Piece::whiteTokin()));
  ASSERT_EQ(material::ProLance       , material::score(Piece::whiteProLance()));
  ASSERT_EQ(material::Knight         , material::score(Piece::whiteKnight()));
  ASSERT_EQ(material::Silver         , material::score(Piece::whiteSilver()));
  ASSERT_EQ(material::Horse          , material::score(Piece::whiteHorse()));
  ASSERT_EQ(material::Dragon         , material::score(Piece::whiteDragon()));
}

TEST(MaterialTest, testExchangeScore) {
  ASSERT_EQ(material::Pawn     * 2                , material::exchangeScore(Piece::blackPawn()));
  ASSERT_EQ(material::Lance    * 2                , material::exchangeScore(Piece::blackLance()));
  ASSERT_EQ(material::Knight   * 2                , material::exchangeScore(Piece::blackKnight()));
  ASSERT_EQ(material::Silver   * 2                , material::exchangeScore(Piece::blackSilver()));
  ASSERT_EQ(material::Gold     * 2                , material::exchangeScore(Piece::blackGold()));
  ASSERT_EQ(material::Bishop   * 2                , material::exchangeScore(Piece::blackBishop()));
  ASSERT_EQ(material::Rook     * 2                , material::exchangeScore(Piece::blackRook()));
  ASSERT_EQ(Score::materialInfinity()             , material::exchangeScore(Piece::blackKing()));
  ASSERT_EQ(material::Tokin     + material::Pawn  , material::exchangeScore(Piece::blackTokin()));
  ASSERT_EQ(material::ProLance  + material::Lance , material::exchangeScore(Piece::blackProLance()));
  ASSERT_EQ(material::ProKnight + material::Knight, material::exchangeScore(Piece::blackProKnight()));
  ASSERT_EQ(material::ProSilver + material::Silver, material::exchangeScore(Piece::blackProSilver()));
  ASSERT_EQ(material::Horse     + material::Bishop, material::exchangeScore(Piece::blackHorse()));
  ASSERT_EQ(material::Dragon    + material::Rook  , material::exchangeScore(Piece::blackDragon()));

  ASSERT_EQ(material::Pawn     * 2                , material::exchangeScore(Piece::whitePawn()));
  ASSERT_EQ(material::Lance    * 2                , material::exchangeScore(Piece::whiteLance()));
  ASSERT_EQ(material::Knight   * 2                , material::exchangeScore(Piece::whiteKnight()));
  ASSERT_EQ(material::Silver   * 2                , material::exchangeScore(Piece::whiteSilver()));
  ASSERT_EQ(material::Gold     * 2                , material::exchangeScore(Piece::whiteGold()));
  ASSERT_EQ(material::Bishop   * 2                , material::exchangeScore(Piece::whiteBishop()));
  ASSERT_EQ(material::Rook     * 2                , material::exchangeScore(Piece::whiteRook()));
  ASSERT_EQ(Score::materialInfinity()             , material::exchangeScore(Piece::whiteKing()));
  ASSERT_EQ(material::Tokin     + material::Pawn  , material::exchangeScore(Piece::whiteTokin()));
  ASSERT_EQ(material::ProLance  + material::Lance , material::exchangeScore(Piece::whiteProLance()));
  ASSERT_EQ(material::ProKnight + material::Knight, material::exchangeScore(Piece::whiteProKnight()));
  ASSERT_EQ(material::ProSilver + material::Silver, material::exchangeScore(Piece::whiteProSilver()));
  ASSERT_EQ(material::Horse     + material::Bishop, material::exchangeScore(Piece::whiteHorse()));
  ASSERT_EQ(material::Dragon    + material::Rook  , material::exchangeScore(Piece::whiteDragon()));
}

TEST(MaterialTest, testPromotionScore) {
  ASSERT_EQ(material::Tokin     - material::Pawn  , material::promotionScore(Piece::blackPawn()));
  ASSERT_EQ(material::ProLance  - material::Lance , material::promotionScore(Piece::blackLance()));
  ASSERT_EQ(material::ProKnight - material::Knight, material::promotionScore(Piece::blackKnight()));
  ASSERT_EQ(material::ProSilver - material::Silver, material::promotionScore(Piece::blackSilver()));
  ASSERT_EQ(0                                     , material::promotionScore(Piece::blackGold()));
  ASSERT_EQ(material::Horse     - material::Bishop, material::promotionScore(Piece::blackBishop()));
  ASSERT_EQ(material::Dragon    - material::Rook  , material::promotionScore(Piece::blackRook()));
  ASSERT_EQ(0                                     , material::promotionScore(Piece::blackKing()));
  ASSERT_EQ(0                                     , material::promotionScore(Piece::blackTokin()));
  ASSERT_EQ(0                                     , material::promotionScore(Piece::blackProLance()));
  ASSERT_EQ(0                                     , material::promotionScore(Piece::blackProKnight()));
  ASSERT_EQ(0                                     , material::promotionScore(Piece::blackProSilver()));
  ASSERT_EQ(0                                     , material::promotionScore(Piece::blackHorse()));
  ASSERT_EQ(0                                     , material::promotionScore(Piece::blackDragon()));

  ASSERT_EQ(material::Tokin     - material::Pawn  , material::promotionScore(Piece::whitePawn()));
  ASSERT_EQ(material::ProLance  - material::Lance , material::promotionScore(Piece::whiteLance()));
  ASSERT_EQ(material::ProKnight - material::Knight, material::promotionScore(Piece::whiteKnight()));
  ASSERT_EQ(material::ProSilver - material::Silver, material::promotionScore(Piece::whiteSilver()));
  ASSERT_EQ(0                                     , material::promotionScore(Piece::whiteGold()));
  ASSERT_EQ(material::Horse     - material::Bishop, material::promotionScore(Piece::whiteBishop()));
  ASSERT_EQ(material::Dragon    - material::Rook  , material::promotionScore(Piece::whiteRook()));
  ASSERT_EQ(0                                     , material::promotionScore(Piece::whiteKing()));
  ASSERT_EQ(0                                     , material::promotionScore(Piece::whiteTokin()));
  ASSERT_EQ(0                                     , material::promotionScore(Piece::whiteProLance()));
  ASSERT_EQ(0                                     , material::promotionScore(Piece::whiteProKnight()));
  ASSERT_EQ(0                                     , material::promotionScore(Piece::whiteProSilver()));
  ASSERT_EQ(0                                     , material::promotionScore(Piece::whiteHorse()));
  ASSERT_EQ(0                                     , material::promotionScore(Piece::whiteDragon()));
}

#endif // !defined(NDEBUG)
