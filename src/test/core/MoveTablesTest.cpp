/* MoveTablesTest.cpp
 *
 * Kubo Ryosuke
 */

#include "test/Test.hpp"
#include "core/move/MoveTables.hpp"

using namespace sunfish;

TEST(MoveTablesTest, testIsMovehableInOneStep) {
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackPawn(), Direction::LeftUp));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackPawn(), Direction::Up));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackPawn(), Direction::RightUp));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackPawn(), Direction::Left));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackPawn(), Direction::Right));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackPawn(), Direction::LeftDown));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackPawn(), Direction::Down));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackPawn(), Direction::RightDown));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackPawn(), Direction::LeftUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackPawn(), Direction::RightUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackPawn(), Direction::LeftDownKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackPawn(), Direction::RightDownKnight));

  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackLance(), Direction::LeftUp));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackLance(), Direction::Up));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackLance(), Direction::RightUp));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackLance(), Direction::Left));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackLance(), Direction::Right));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackLance(), Direction::LeftDown));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackLance(), Direction::Down));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackLance(), Direction::RightDown));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackLance(), Direction::LeftUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackLance(), Direction::RightUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackLance(), Direction::LeftDownKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackLance(), Direction::RightDownKnight));

  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackKnight(), Direction::LeftUp));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackKnight(), Direction::Up));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackKnight(), Direction::RightUp));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackKnight(), Direction::Left));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackKnight(), Direction::Right));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackKnight(), Direction::LeftDown));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackKnight(), Direction::Down));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackKnight(), Direction::RightDown));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackKnight(), Direction::LeftUpKnight));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackKnight(), Direction::RightUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackKnight(), Direction::LeftDownKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackKnight(), Direction::RightDownKnight));

  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackSilver(), Direction::LeftUp));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackSilver(), Direction::Up));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackSilver(), Direction::RightUp));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackSilver(), Direction::Left));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackSilver(), Direction::Right));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackSilver(), Direction::LeftDown));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackSilver(), Direction::Down));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackSilver(), Direction::RightDown));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackSilver(), Direction::LeftUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackSilver(), Direction::RightUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackSilver(), Direction::LeftDownKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackSilver(), Direction::RightDownKnight));

  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackGold(), Direction::LeftUp));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackGold(), Direction::Up));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackGold(), Direction::RightUp));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackGold(), Direction::Left));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackGold(), Direction::Right));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackGold(), Direction::LeftDown));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackGold(), Direction::Down));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackGold(), Direction::RightDown));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackGold(), Direction::LeftUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackGold(), Direction::RightUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackGold(), Direction::LeftDownKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackGold(), Direction::RightDownKnight));

  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackBishop(), Direction::LeftUp));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackBishop(), Direction::Up));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackBishop(), Direction::RightUp));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackBishop(), Direction::Left));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackBishop(), Direction::Right));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackBishop(), Direction::LeftDown));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackBishop(), Direction::Down));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackBishop(), Direction::RightDown));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackBishop(), Direction::LeftUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackBishop(), Direction::RightUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackBishop(), Direction::LeftDownKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackBishop(), Direction::RightDownKnight));

  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackRook(), Direction::LeftUp));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackRook(), Direction::Up));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackRook(), Direction::RightUp));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackRook(), Direction::Left));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackRook(), Direction::Right));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackRook(), Direction::LeftDown));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackRook(), Direction::Down));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackRook(), Direction::RightDown));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackRook(), Direction::LeftUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackRook(), Direction::RightUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackRook(), Direction::LeftDownKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackRook(), Direction::RightDownKnight));

  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackKing(), Direction::LeftUp));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackKing(), Direction::Up));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackKing(), Direction::RightUp));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackKing(), Direction::Left));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackKing(), Direction::Right));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackKing(), Direction::LeftDown));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackKing(), Direction::Down));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackKing(), Direction::RightDown));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackKing(), Direction::LeftUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackKing(), Direction::RightUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackKing(), Direction::LeftDownKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackKing(), Direction::RightDownKnight));

  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackTokin(), Direction::LeftUp));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackTokin(), Direction::Up));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackTokin(), Direction::RightUp));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackTokin(), Direction::Left));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackTokin(), Direction::Right));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackTokin(), Direction::LeftDown));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackTokin(), Direction::Down));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackTokin(), Direction::RightDown));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackTokin(), Direction::LeftUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackTokin(), Direction::RightUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackTokin(), Direction::LeftDownKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackTokin(), Direction::RightDownKnight));

  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackProLance(), Direction::LeftUp));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackProLance(), Direction::Up));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackProLance(), Direction::RightUp));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackProLance(), Direction::Left));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackProLance(), Direction::Right));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackProLance(), Direction::LeftDown));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackProLance(), Direction::Down));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackProLance(), Direction::RightDown));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackProLance(), Direction::LeftUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackProLance(), Direction::RightUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackProLance(), Direction::LeftDownKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackProLance(), Direction::RightDownKnight));

  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackProKnight(), Direction::LeftUp));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackProKnight(), Direction::Up));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackProKnight(), Direction::RightUp));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackProKnight(), Direction::Left));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackProKnight(), Direction::Right));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackProKnight(), Direction::LeftDown));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackProKnight(), Direction::Down));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackProKnight(), Direction::RightDown));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackProKnight(), Direction::LeftUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackProKnight(), Direction::RightUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackProKnight(), Direction::LeftDownKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackProKnight(), Direction::RightDownKnight));

  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackProSilver(), Direction::LeftUp));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackProSilver(), Direction::Up));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackProSilver(), Direction::RightUp));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackProSilver(), Direction::Left));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackProSilver(), Direction::Right));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackProSilver(), Direction::LeftDown));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackProSilver(), Direction::Down));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackProSilver(), Direction::RightDown));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackProSilver(), Direction::LeftUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackProSilver(), Direction::RightUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackProSilver(), Direction::LeftDownKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackProSilver(), Direction::RightDownKnight));

  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackHorse(), Direction::LeftUp));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackHorse(), Direction::Up));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackHorse(), Direction::RightUp));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackHorse(), Direction::Left));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackHorse(), Direction::Right));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackHorse(), Direction::LeftDown));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackHorse(), Direction::Down));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackHorse(), Direction::RightDown));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackHorse(), Direction::LeftUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackHorse(), Direction::RightUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackHorse(), Direction::LeftDownKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackHorse(), Direction::RightDownKnight));

  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackDragon(), Direction::LeftUp));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackDragon(), Direction::Up));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackDragon(), Direction::RightUp));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackDragon(), Direction::Left));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackDragon(), Direction::Right));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackDragon(), Direction::LeftDown));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackDragon(), Direction::Down));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::blackDragon(), Direction::RightDown));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackDragon(), Direction::LeftUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackDragon(), Direction::RightUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackDragon(), Direction::LeftDownKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::blackDragon(), Direction::RightDownKnight));

  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whitePawn(), Direction::LeftUp));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whitePawn(), Direction::Up));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whitePawn(), Direction::RightUp));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whitePawn(), Direction::Left));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whitePawn(), Direction::Right));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whitePawn(), Direction::LeftDown));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whitePawn(), Direction::Down));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whitePawn(), Direction::RightDown));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whitePawn(), Direction::LeftUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whitePawn(), Direction::RightUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whitePawn(), Direction::LeftDownKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whitePawn(), Direction::RightDownKnight));

  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteLance(), Direction::LeftUp));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteLance(), Direction::Up));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteLance(), Direction::RightUp));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteLance(), Direction::Left));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteLance(), Direction::Right));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteLance(), Direction::LeftDown));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteLance(), Direction::Down));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteLance(), Direction::RightDown));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteLance(), Direction::LeftUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteLance(), Direction::RightUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteLance(), Direction::LeftDownKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteLance(), Direction::RightDownKnight));

  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteKnight(), Direction::LeftUp));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteKnight(), Direction::Up));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteKnight(), Direction::RightUp));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteKnight(), Direction::Left));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteKnight(), Direction::Right));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteKnight(), Direction::LeftDown));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteKnight(), Direction::Down));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteKnight(), Direction::RightDown));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteKnight(), Direction::LeftUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteKnight(), Direction::RightUpKnight));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteKnight(), Direction::LeftDownKnight));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteKnight(), Direction::RightDownKnight));

  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteSilver(), Direction::LeftUp));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteSilver(), Direction::Up));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteSilver(), Direction::RightUp));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteSilver(), Direction::Left));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteSilver(), Direction::Right));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteSilver(), Direction::LeftDown));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteSilver(), Direction::Down));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteSilver(), Direction::RightDown));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteSilver(), Direction::LeftUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteSilver(), Direction::RightUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteSilver(), Direction::LeftDownKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteSilver(), Direction::RightDownKnight));

  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteGold(), Direction::LeftUp));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteGold(), Direction::Up));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteGold(), Direction::RightUp));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteGold(), Direction::Left));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteGold(), Direction::Right));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteGold(), Direction::LeftDown));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteGold(), Direction::Down));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteGold(), Direction::RightDown));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteGold(), Direction::LeftUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteGold(), Direction::RightUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteGold(), Direction::LeftDownKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteGold(), Direction::RightDownKnight));

  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteBishop(), Direction::LeftUp));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteBishop(), Direction::Up));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteBishop(), Direction::RightUp));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteBishop(), Direction::Left));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteBishop(), Direction::Right));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteBishop(), Direction::LeftDown));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteBishop(), Direction::Down));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteBishop(), Direction::RightDown));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteBishop(), Direction::LeftUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteBishop(), Direction::RightUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteBishop(), Direction::LeftDownKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteBishop(), Direction::RightDownKnight));

  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteRook(), Direction::LeftUp));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteRook(), Direction::Up));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteRook(), Direction::RightUp));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteRook(), Direction::Left));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteRook(), Direction::Right));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteRook(), Direction::LeftDown));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteRook(), Direction::Down));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteRook(), Direction::RightDown));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteRook(), Direction::LeftUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteRook(), Direction::RightUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteRook(), Direction::LeftDownKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteRook(), Direction::RightDownKnight));

  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteKing(), Direction::LeftUp));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteKing(), Direction::Up));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteKing(), Direction::RightUp));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteKing(), Direction::Left));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteKing(), Direction::Right));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteKing(), Direction::LeftDown));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteKing(), Direction::Down));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteKing(), Direction::RightDown));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteKing(), Direction::LeftUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteKing(), Direction::RightUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteKing(), Direction::LeftDownKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteKing(), Direction::RightDownKnight));

  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteTokin(), Direction::LeftUp));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteTokin(), Direction::Up));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteTokin(), Direction::RightUp));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteTokin(), Direction::Left));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteTokin(), Direction::Right));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteTokin(), Direction::LeftDown));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteTokin(), Direction::Down));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteTokin(), Direction::RightDown));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteTokin(), Direction::LeftUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteTokin(), Direction::RightUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteTokin(), Direction::LeftDownKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteTokin(), Direction::RightDownKnight));

  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteProLance(), Direction::LeftUp));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteProLance(), Direction::Up));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteProLance(), Direction::RightUp));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteProLance(), Direction::Left));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteProLance(), Direction::Right));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteProLance(), Direction::LeftDown));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteProLance(), Direction::Down));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteProLance(), Direction::RightDown));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteProLance(), Direction::LeftUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteProLance(), Direction::RightUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteProLance(), Direction::LeftDownKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteProLance(), Direction::RightDownKnight));

  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteProKnight(), Direction::LeftUp));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteProKnight(), Direction::Up));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteProKnight(), Direction::RightUp));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteProKnight(), Direction::Left));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteProKnight(), Direction::Right));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteProKnight(), Direction::LeftDown));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteProKnight(), Direction::Down));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteProKnight(), Direction::RightDown));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteProKnight(), Direction::LeftUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteProKnight(), Direction::RightUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteProKnight(), Direction::LeftDownKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteProKnight(), Direction::RightDownKnight));

  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteProSilver(), Direction::LeftUp));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteProSilver(), Direction::Up));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteProSilver(), Direction::RightUp));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteProSilver(), Direction::Left));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteProSilver(), Direction::Right));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteProSilver(), Direction::LeftDown));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteProSilver(), Direction::Down));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteProSilver(), Direction::RightDown));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteProSilver(), Direction::LeftUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteProSilver(), Direction::RightUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteProSilver(), Direction::LeftDownKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteProSilver(), Direction::RightDownKnight));

  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteHorse(), Direction::LeftUp));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteHorse(), Direction::Up));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteHorse(), Direction::RightUp));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteHorse(), Direction::Left));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteHorse(), Direction::Right));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteHorse(), Direction::LeftDown));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteHorse(), Direction::Down));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteHorse(), Direction::RightDown));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteHorse(), Direction::LeftUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteHorse(), Direction::RightUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteHorse(), Direction::LeftDownKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteHorse(), Direction::RightDownKnight));

  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteDragon(), Direction::LeftUp));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteDragon(), Direction::Up));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteDragon(), Direction::RightUp));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteDragon(), Direction::Left));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteDragon(), Direction::Right));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteDragon(), Direction::LeftDown));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteDragon(), Direction::Down));
  ASSERT_EQ(true , MoveTables::isMovableInOneStep(Piece::whiteDragon(), Direction::RightDown));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteDragon(), Direction::LeftUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteDragon(), Direction::RightUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteDragon(), Direction::LeftDownKnight));
  ASSERT_EQ(false, MoveTables::isMovableInOneStep(Piece::whiteDragon(), Direction::RightDownKnight));
}

TEST(MoveTablesTest, testIsMovehableInLongStep) {
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackPawn(), Direction::LeftUp));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackPawn(), Direction::Up));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackPawn(), Direction::RightUp));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackPawn(), Direction::Left));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackPawn(), Direction::Right));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackPawn(), Direction::LeftDown));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackPawn(), Direction::Down));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackPawn(), Direction::RightDown));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackPawn(), Direction::LeftUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackPawn(), Direction::RightUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackPawn(), Direction::LeftDownKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackPawn(), Direction::RightDownKnight));

  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackLance(), Direction::LeftUp));
  ASSERT_EQ(true , MoveTables::isMovableInLongStep(Piece::blackLance(), Direction::Up));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackLance(), Direction::RightUp));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackLance(), Direction::Left));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackLance(), Direction::Right));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackLance(), Direction::LeftDown));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackLance(), Direction::Down));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackLance(), Direction::RightDown));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackLance(), Direction::LeftUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackLance(), Direction::RightUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackLance(), Direction::LeftDownKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackLance(), Direction::RightDownKnight));

  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackKnight(), Direction::LeftUp));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackKnight(), Direction::Up));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackKnight(), Direction::RightUp));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackKnight(), Direction::Left));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackKnight(), Direction::Right));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackKnight(), Direction::LeftDown));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackKnight(), Direction::Down));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackKnight(), Direction::RightDown));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackKnight(), Direction::LeftUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackKnight(), Direction::RightUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackKnight(), Direction::LeftDownKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackKnight(), Direction::RightDownKnight));

  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackSilver(), Direction::LeftUp));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackSilver(), Direction::Up));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackSilver(), Direction::RightUp));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackSilver(), Direction::Left));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackSilver(), Direction::Right));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackSilver(), Direction::LeftDown));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackSilver(), Direction::Down));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackSilver(), Direction::RightDown));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackSilver(), Direction::LeftUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackSilver(), Direction::RightUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackSilver(), Direction::LeftDownKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackSilver(), Direction::RightDownKnight));

  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackGold(), Direction::LeftUp));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackGold(), Direction::Up));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackGold(), Direction::RightUp));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackGold(), Direction::Left));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackGold(), Direction::Right));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackGold(), Direction::LeftDown));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackGold(), Direction::Down));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackGold(), Direction::RightDown));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackGold(), Direction::LeftUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackGold(), Direction::RightUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackGold(), Direction::LeftDownKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackGold(), Direction::RightDownKnight));

  ASSERT_EQ(true , MoveTables::isMovableInLongStep(Piece::blackBishop(), Direction::LeftUp));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackBishop(), Direction::Up));
  ASSERT_EQ(true , MoveTables::isMovableInLongStep(Piece::blackBishop(), Direction::RightUp));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackBishop(), Direction::Left));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackBishop(), Direction::Right));
  ASSERT_EQ(true , MoveTables::isMovableInLongStep(Piece::blackBishop(), Direction::LeftDown));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackBishop(), Direction::Down));
  ASSERT_EQ(true , MoveTables::isMovableInLongStep(Piece::blackBishop(), Direction::RightDown));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackBishop(), Direction::LeftUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackBishop(), Direction::RightUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackBishop(), Direction::LeftDownKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackBishop(), Direction::RightDownKnight));

  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackRook(), Direction::LeftUp));
  ASSERT_EQ(true , MoveTables::isMovableInLongStep(Piece::blackRook(), Direction::Up));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackRook(), Direction::RightUp));
  ASSERT_EQ(true , MoveTables::isMovableInLongStep(Piece::blackRook(), Direction::Left));
  ASSERT_EQ(true , MoveTables::isMovableInLongStep(Piece::blackRook(), Direction::Right));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackRook(), Direction::LeftDown));
  ASSERT_EQ(true , MoveTables::isMovableInLongStep(Piece::blackRook(), Direction::Down));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackRook(), Direction::RightDown));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackRook(), Direction::LeftUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackRook(), Direction::RightUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackRook(), Direction::LeftDownKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackRook(), Direction::RightDownKnight));

  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackKing(), Direction::LeftUp));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackKing(), Direction::Up));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackKing(), Direction::RightUp));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackKing(), Direction::Left));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackKing(), Direction::Right));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackKing(), Direction::LeftDown));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackKing(), Direction::Down));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackKing(), Direction::RightDown));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackKing(), Direction::LeftUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackKing(), Direction::RightUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackKing(), Direction::LeftDownKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackKing(), Direction::RightDownKnight));

  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackTokin(), Direction::LeftUp));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackTokin(), Direction::Up));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackTokin(), Direction::RightUp));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackTokin(), Direction::Left));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackTokin(), Direction::Right));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackTokin(), Direction::LeftDown));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackTokin(), Direction::Down));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackTokin(), Direction::RightDown));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackTokin(), Direction::LeftUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackTokin(), Direction::RightUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackTokin(), Direction::LeftDownKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackTokin(), Direction::RightDownKnight));

  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackProLance(), Direction::LeftUp));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackProLance(), Direction::Up));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackProLance(), Direction::RightUp));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackProLance(), Direction::Left));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackProLance(), Direction::Right));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackProLance(), Direction::LeftDown));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackProLance(), Direction::Down));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackProLance(), Direction::RightDown));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackProLance(), Direction::LeftUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackProLance(), Direction::RightUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackProLance(), Direction::LeftDownKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackProLance(), Direction::RightDownKnight));

  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackProKnight(), Direction::LeftUp));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackProKnight(), Direction::Up));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackProKnight(), Direction::RightUp));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackProKnight(), Direction::Left));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackProKnight(), Direction::Right));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackProKnight(), Direction::LeftDown));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackProKnight(), Direction::Down));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackProKnight(), Direction::RightDown));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackProKnight(), Direction::LeftUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackProKnight(), Direction::RightUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackProKnight(), Direction::LeftDownKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackProKnight(), Direction::RightDownKnight));

  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackProSilver(), Direction::LeftUp));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackProSilver(), Direction::Up));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackProSilver(), Direction::RightUp));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackProSilver(), Direction::Left));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackProSilver(), Direction::Right));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackProSilver(), Direction::LeftDown));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackProSilver(), Direction::Down));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackProSilver(), Direction::RightDown));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackProSilver(), Direction::LeftUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackProSilver(), Direction::RightUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackProSilver(), Direction::LeftDownKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackProSilver(), Direction::RightDownKnight));

  ASSERT_EQ(true , MoveTables::isMovableInLongStep(Piece::blackHorse(), Direction::LeftUp));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackHorse(), Direction::Up));
  ASSERT_EQ(true , MoveTables::isMovableInLongStep(Piece::blackHorse(), Direction::RightUp));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackHorse(), Direction::Left));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackHorse(), Direction::Right));
  ASSERT_EQ(true , MoveTables::isMovableInLongStep(Piece::blackHorse(), Direction::LeftDown));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackHorse(), Direction::Down));
  ASSERT_EQ(true , MoveTables::isMovableInLongStep(Piece::blackHorse(), Direction::RightDown));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackHorse(), Direction::LeftUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackHorse(), Direction::RightUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackHorse(), Direction::LeftDownKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackHorse(), Direction::RightDownKnight));

  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackDragon(), Direction::LeftUp));
  ASSERT_EQ(true , MoveTables::isMovableInLongStep(Piece::blackDragon(), Direction::Up));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackDragon(), Direction::RightUp));
  ASSERT_EQ(true , MoveTables::isMovableInLongStep(Piece::blackDragon(), Direction::Left));
  ASSERT_EQ(true , MoveTables::isMovableInLongStep(Piece::blackDragon(), Direction::Right));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackDragon(), Direction::LeftDown));
  ASSERT_EQ(true , MoveTables::isMovableInLongStep(Piece::blackDragon(), Direction::Down));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackDragon(), Direction::RightDown));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackDragon(), Direction::LeftUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackDragon(), Direction::RightUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackDragon(), Direction::LeftDownKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::blackDragon(), Direction::RightDownKnight));

  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whitePawn(), Direction::LeftUp));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whitePawn(), Direction::Up));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whitePawn(), Direction::RightUp));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whitePawn(), Direction::Left));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whitePawn(), Direction::Right));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whitePawn(), Direction::LeftDown));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whitePawn(), Direction::Down));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whitePawn(), Direction::RightDown));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whitePawn(), Direction::LeftUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whitePawn(), Direction::RightUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whitePawn(), Direction::LeftDownKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whitePawn(), Direction::RightDownKnight));

  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteLance(), Direction::LeftUp));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteLance(), Direction::Up));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteLance(), Direction::RightUp));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteLance(), Direction::Left));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteLance(), Direction::Right));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteLance(), Direction::LeftDown));
  ASSERT_EQ(true , MoveTables::isMovableInLongStep(Piece::whiteLance(), Direction::Down));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteLance(), Direction::RightDown));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteLance(), Direction::LeftUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteLance(), Direction::RightUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteLance(), Direction::LeftDownKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteLance(), Direction::RightDownKnight));

  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteKnight(), Direction::LeftUp));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteKnight(), Direction::Up));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteKnight(), Direction::RightUp));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteKnight(), Direction::Left));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteKnight(), Direction::Right));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteKnight(), Direction::LeftDown));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteKnight(), Direction::Down));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteKnight(), Direction::RightDown));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteKnight(), Direction::LeftUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteKnight(), Direction::RightUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteKnight(), Direction::LeftDownKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteKnight(), Direction::RightDownKnight));

  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteSilver(), Direction::LeftUp));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteSilver(), Direction::Up));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteSilver(), Direction::RightUp));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteSilver(), Direction::Left));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteSilver(), Direction::Right));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteSilver(), Direction::LeftDown));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteSilver(), Direction::Down));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteSilver(), Direction::RightDown));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteSilver(), Direction::LeftUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteSilver(), Direction::RightUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteSilver(), Direction::LeftDownKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteSilver(), Direction::RightDownKnight));

  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteGold(), Direction::LeftUp));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteGold(), Direction::Up));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteGold(), Direction::RightUp));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteGold(), Direction::Left));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteGold(), Direction::Right));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteGold(), Direction::LeftDown));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteGold(), Direction::Down));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteGold(), Direction::RightDown));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteGold(), Direction::LeftUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteGold(), Direction::RightUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteGold(), Direction::LeftDownKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteGold(), Direction::RightDownKnight));

  ASSERT_EQ(true , MoveTables::isMovableInLongStep(Piece::whiteBishop(), Direction::LeftUp));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteBishop(), Direction::Up));
  ASSERT_EQ(true , MoveTables::isMovableInLongStep(Piece::whiteBishop(), Direction::RightUp));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteBishop(), Direction::Left));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteBishop(), Direction::Right));
  ASSERT_EQ(true , MoveTables::isMovableInLongStep(Piece::whiteBishop(), Direction::LeftDown));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteBishop(), Direction::Down));
  ASSERT_EQ(true , MoveTables::isMovableInLongStep(Piece::whiteBishop(), Direction::RightDown));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteBishop(), Direction::LeftUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteBishop(), Direction::RightUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteBishop(), Direction::LeftDownKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteBishop(), Direction::RightDownKnight));

  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteRook(), Direction::LeftUp));
  ASSERT_EQ(true , MoveTables::isMovableInLongStep(Piece::whiteRook(), Direction::Up));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteRook(), Direction::RightUp));
  ASSERT_EQ(true , MoveTables::isMovableInLongStep(Piece::whiteRook(), Direction::Left));
  ASSERT_EQ(true , MoveTables::isMovableInLongStep(Piece::whiteRook(), Direction::Right));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteRook(), Direction::LeftDown));
  ASSERT_EQ(true , MoveTables::isMovableInLongStep(Piece::whiteRook(), Direction::Down));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteRook(), Direction::RightDown));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteRook(), Direction::LeftUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteRook(), Direction::RightUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteRook(), Direction::LeftDownKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteRook(), Direction::RightDownKnight));

  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteKing(), Direction::LeftUp));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteKing(), Direction::Up));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteKing(), Direction::RightUp));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteKing(), Direction::Left));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteKing(), Direction::Right));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteKing(), Direction::LeftDown));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteKing(), Direction::Down));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteKing(), Direction::RightDown));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteKing(), Direction::LeftUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteKing(), Direction::RightUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteKing(), Direction::LeftDownKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteKing(), Direction::RightDownKnight));

  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteTokin(), Direction::LeftUp));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteTokin(), Direction::Up));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteTokin(), Direction::RightUp));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteTokin(), Direction::Left));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteTokin(), Direction::Right));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteTokin(), Direction::LeftDown));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteTokin(), Direction::Down));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteTokin(), Direction::RightDown));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteTokin(), Direction::LeftUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteTokin(), Direction::RightUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteTokin(), Direction::LeftDownKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteTokin(), Direction::RightDownKnight));

  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteProLance(), Direction::LeftUp));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteProLance(), Direction::Up));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteProLance(), Direction::RightUp));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteProLance(), Direction::Left));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteProLance(), Direction::Right));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteProLance(), Direction::LeftDown));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteProLance(), Direction::Down));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteProLance(), Direction::RightDown));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteProLance(), Direction::LeftUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteProLance(), Direction::RightUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteProLance(), Direction::LeftDownKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteProLance(), Direction::RightDownKnight));

  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteProKnight(), Direction::LeftUp));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteProKnight(), Direction::Up));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteProKnight(), Direction::RightUp));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteProKnight(), Direction::Left));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteProKnight(), Direction::Right));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteProKnight(), Direction::LeftDown));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteProKnight(), Direction::Down));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteProKnight(), Direction::RightDown));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteProKnight(), Direction::LeftUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteProKnight(), Direction::RightUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteProKnight(), Direction::LeftDownKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteProKnight(), Direction::RightDownKnight));

  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteProSilver(), Direction::LeftUp));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteProSilver(), Direction::Up));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteProSilver(), Direction::RightUp));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteProSilver(), Direction::Left));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteProSilver(), Direction::Right));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteProSilver(), Direction::LeftDown));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteProSilver(), Direction::Down));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteProSilver(), Direction::RightDown));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteProSilver(), Direction::LeftUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteProSilver(), Direction::RightUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteProSilver(), Direction::LeftDownKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteProSilver(), Direction::RightDownKnight));

  ASSERT_EQ(true , MoveTables::isMovableInLongStep(Piece::whiteHorse(), Direction::LeftUp));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteHorse(), Direction::Up));
  ASSERT_EQ(true , MoveTables::isMovableInLongStep(Piece::whiteHorse(), Direction::RightUp));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteHorse(), Direction::Left));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteHorse(), Direction::Right));
  ASSERT_EQ(true , MoveTables::isMovableInLongStep(Piece::whiteHorse(), Direction::LeftDown));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteHorse(), Direction::Down));
  ASSERT_EQ(true , MoveTables::isMovableInLongStep(Piece::whiteHorse(), Direction::RightDown));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteHorse(), Direction::LeftUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteHorse(), Direction::RightUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteHorse(), Direction::LeftDownKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteHorse(), Direction::RightDownKnight));

  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteDragon(), Direction::LeftUp));
  ASSERT_EQ(true , MoveTables::isMovableInLongStep(Piece::whiteDragon(), Direction::Up));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteDragon(), Direction::RightUp));
  ASSERT_EQ(true , MoveTables::isMovableInLongStep(Piece::whiteDragon(), Direction::Left));
  ASSERT_EQ(true , MoveTables::isMovableInLongStep(Piece::whiteDragon(), Direction::Right));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteDragon(), Direction::LeftDown));
  ASSERT_EQ(true , MoveTables::isMovableInLongStep(Piece::whiteDragon(), Direction::Down));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteDragon(), Direction::RightDown));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteDragon(), Direction::LeftUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteDragon(), Direction::RightUpKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteDragon(), Direction::LeftDownKnight));
  ASSERT_EQ(false, MoveTables::isMovableInLongStep(Piece::whiteDragon(), Direction::RightDownKnight));
}

TEST(MoveTablesTest, testOneStep) {
  {
    const auto& bb = MoveTables::blackSilver(Square::s67());
    ASSERT_EQ(
      "000000000\n"
      "000000000\n"
      "000000000\n"
      "000000000\n"
      "000000000\n"
      "001110000\n"
      "000000000\n"
      "001010000\n"
      "000000000\n",
      bb.toString());
  }

  {
    const auto& bb = MoveTables::whiteSilver(Square::s67());
    ASSERT_EQ(
      "000000000\n"
      "000000000\n"
      "000000000\n"
      "000000000\n"
      "000000000\n"
      "001010000\n"
      "000000000\n"
      "001110000\n"
      "000000000\n",
      bb.toString());
  }

  {
    const auto& bb = MoveTables::blackGold(Square::s67());
    ASSERT_EQ(
      "000000000\n"
      "000000000\n"
      "000000000\n"
      "000000000\n"
      "000000000\n"
      "001110000\n"
      "001010000\n"
      "000100000\n"
      "000000000\n",
      bb.toString());
  }

  {
    const auto& bb = MoveTables::whiteGold(Square::s67());
    ASSERT_EQ(
      "000000000\n"
      "000000000\n"
      "000000000\n"
      "000000000\n"
      "000000000\n"
      "000100000\n"
      "001010000\n"
      "001110000\n"
      "000000000\n",
      bb.toString());
  }

  {
    const auto& bb = MoveTables::king(Square::s67());
    ASSERT_EQ(
      "000000000\n"
      "000000000\n"
      "000000000\n"
      "000000000\n"
      "000000000\n"
      "001110000\n"
      "001010000\n"
      "001110000\n"
      "000000000\n",
      bb.toString());
  }

  {
    const auto& bb = MoveTables::cross(Square::s67());
    ASSERT_EQ(
      "000000000\n"
      "000000000\n"
      "000000000\n"
      "000000000\n"
      "000000000\n"
      "000100000\n"
      "001010000\n"
      "000100000\n"
      "000000000\n",
      bb.toString());
  }

  {
    const auto& bb = MoveTables::x(Square::s67());
    ASSERT_EQ(
      "000000000\n"
      "000000000\n"
      "000000000\n"
      "000000000\n"
      "000000000\n"
      "001010000\n"
      "000000000\n"
      "001010000\n"
      "000000000\n",
      bb.toString());
  }

  {
    const auto& bb = MoveTables::neighbor3x3(Square::s67());
    ASSERT_EQ(
      "000000000\n"
      "000000000\n"
      "000000000\n"
      "000000000\n"
      "000000000\n"
      "001110000\n"
      "001110000\n"
      "001110000\n"
      "000000000\n",
      bb.toString());
  }

  {
    const auto& bb = MoveTables::neighbor5x5(Square::s67());
    ASSERT_EQ(
      "000000000\n"
      "000000000\n"
      "000000000\n"
      "000000000\n"
      "011111000\n"
      "011111000\n"
      "011111000\n"
      "011111000\n"
      "011111000\n",
      bb.toString());
  }
}

TEST(MoveTablesTest, testVer) {
  {
    auto occ = Bitboard::zero();
    occ.set(Square::s43());
    occ.set(Square::s48());
    const auto& bb = MoveTables::blackLance(occ, Square::s47());
    ASSERT_EQ(
      "000000000\n"
      "000000000\n"
      "000001000\n"
      "000001000\n"
      "000001000\n"
      "000001000\n"
      "000000000\n"
      "000000000\n"
      "000000000\n",
      bb.toString());
  }

  {
    auto occ = Bitboard::zero();
    occ.set(Square::s43());
    occ.set(Square::s48());
    const auto& bb = MoveTables::whiteLance(occ, Square::s47());
    ASSERT_EQ(
      "000000000\n"
      "000000000\n"
      "000000000\n"
      "000000000\n"
      "000000000\n"
      "000000000\n"
      "000000000\n"
      "000001000\n"
      "000000000\n",
      bb.toString());
  }

  {
    auto occ = Bitboard::zero();
    const auto& bb = MoveTables::ver(occ, Square::s47());
    ASSERT_EQ(
      "000001000\n"
      "000001000\n"
      "000001000\n"
      "000001000\n"
      "000001000\n"
      "000001000\n"
      "000000000\n"
      "000001000\n"
      "000001000\n",
      bb.toString());
  }

  {
    auto occ = Bitboard::zero();
    occ.set(Square::s43());
    occ.set(Square::s48());
    const auto& bb = MoveTables::ver(occ, Square::s47());
    ASSERT_EQ(
      "000000000\n"
      "000000000\n"
      "000001000\n"
      "000001000\n"
      "000001000\n"
      "000001000\n"
      "000000000\n"
      "000001000\n"
      "000000000\n",
      bb.toString());
  }

  {
    auto occ = Bitboard::zero();
    occ.set(Square::s92());
    const auto& bb = MoveTables::ver(occ, Square::s94());
    ASSERT_EQ(
      "000000000\n"
      "100000000\n"
      "100000000\n"
      "000000000\n"
      "100000000\n"
      "100000000\n"
      "100000000\n"
      "100000000\n"
      "100000000\n",
      bb.toString());
  }
}

TEST(MoveTablesTest, testHor) {
  {
    auto occ = RotatedBitboard::zero();
    occ.set(Square::s63().rotate90());
    const auto& bb = MoveTables::hor(occ, Square::s13());
    ASSERT_EQ(
      "000000000\n"
      "000000000\n"
      "000111110\n"
      "000000000\n"
      "000000000\n"
      "000000000\n"
      "000000000\n"
      "000000000\n"
      "000000000\n",
      bb.toString());
  }

  {
    auto occ = RotatedBitboard::zero();
    occ.set(Square::s29().rotate90());
    const auto& bb = MoveTables::hor(occ, Square::s79());
    ASSERT_EQ(
      "000000000\n"
      "000000000\n"
      "000000000\n"
      "000000000\n"
      "000000000\n"
      "000000000\n"
      "000000000\n"
      "000000000\n"
      "110111110\n",
      bb.toString());
  }
}

TEST(MoveTablesTest, testDiagRight) {
  {
    auto occ = RotatedBitboard::zero();
    occ.set(Square::s76().rotateRight45());
    const auto& bb = MoveTables::diagR45(occ, Square::s43());
    ASSERT_EQ(
      "000000010\n"
      "000000100\n"
      "000000000\n"
      "000010000\n"
      "000100000\n"
      "001000000\n"
      "000000000\n"
      "000000000\n"
      "000000000\n",
      bb.toString());
  }

  {
    auto occ = RotatedBitboard::zero();
    occ.set(Square::s26().rotateRight45());
    const auto& bb = MoveTables::diagR45(occ, Square::s37());
    ASSERT_EQ(
      "000000000\n"
      "000000000\n"
      "000000000\n"
      "000000000\n"
      "000000000\n"
      "000000010\n"
      "000000000\n"
      "000001000\n"
      "000010000\n",
      bb.toString());
  }
}

TEST(MoveTablesTest, testDiagLeft) {
  {
    auto occ = RotatedBitboard::zero();
    occ.set(Square::s72().rotateLeft45());
    occ.set(Square::s27().rotateLeft45());
    const auto& bb = MoveTables::diagL45(occ, Square::s36());
    ASSERT_EQ(
      "000000000\n"
      "001000000\n"
      "000100000\n"
      "000010000\n"
      "000001000\n"
      "000000000\n"
      "000000010\n"
      "000000000\n"
      "000000000\n",
      bb.toString());
  }

  {
    auto occ = RotatedBitboard::zero();
    const auto& bb = MoveTables::diagL45(occ, Square::s93());
    ASSERT_EQ(
      "000000000\n"
      "000000000\n"
      "000000000\n"
      "010000000\n"
      "001000000\n"
      "000100000\n"
      "000010000\n"
      "000001000\n"
      "000000100\n",
      bb.toString());
  }
}

TEST(AggressableTablesTest, test) {
  // black pawn
  ASSERT_EQ(
    "000000000\n"
    "000000000\n"
    "000000000\n"
    "000000000\n"
    "000000000\n"
    "000000100\n"
    "000000000\n"
    "000000000\n"
    "000000000\n",
    AggressableTables::blackPawn(Square::s34()).toString());

  ASSERT_EQ(
    "000000000\n"
    "000000000\n"
    "001010000\n"
    "001110000\n"
    "000000000\n"
    "000000000\n"
    "000000000\n"
    "000000000\n"
    "000000000\n",
    AggressableTables::blackPawn(Square::s62()).toString());

  // white pawn
  ASSERT_EQ(
    "000000000\n"
    "000000000\n"
    "000000000\n"
    "000010000\n"
    "000000000\n"
    "000000000\n"
    "000000000\n"
    "000000000\n"
    "000000000\n",
    AggressableTables::whitePawn(Square::s56()).toString());

  ASSERT_EQ(
    "000000000\n"
    "000000000\n"
    "000000000\n"
    "000000000\n"
    "000000000\n"
    "000000111\n"
    "000000101\n"
    "000000000\n"
    "000000000\n",
    AggressableTables::whitePawn(Square::s28()).toString());

  // black lance
  ASSERT_EQ(
    "000000000\n"
    "000000000\n"
    "000000000\n"
    "000000000\n"
    "000000000\n"
    "000000100\n"
    "000000100\n"
    "000000100\n"
    "000000100\n",
    AggressableTables::blackLance(Square::s34()).toString());

  ASSERT_EQ(
    "000000000\n"
    "000000000\n"
    "001010000\n"
    "001110000\n"
    "001110000\n"
    "001110000\n"
    "001110000\n"
    "001110000\n"
    "001110000\n",
    AggressableTables::blackLance(Square::s62()).toString());

  // white lance
  ASSERT_EQ(
    "000010000\n"
    "000010000\n"
    "000010000\n"
    "000010000\n"
    "000000000\n"
    "000000000\n"
    "000000000\n"
    "000000000\n"
    "000000000\n",
    AggressableTables::whiteLance(Square::s56()).toString());

  ASSERT_EQ(
    "000000111\n"
    "000000111\n"
    "000000111\n"
    "000000111\n"
    "000000111\n"
    "000000111\n"
    "000000101\n"
    "000000000\n"
    "000000000\n",
    AggressableTables::whiteLance(Square::s28()).toString());

  // black knight
  ASSERT_EQ(
    "000000000\n"
    "000000000\n"
    "000000000\n"
    "000000000\n"
    "000001010\n"
    "000000000\n"
    "000000000\n"
    "000010101\n"
    "000000000\n",
    AggressableTables::blackKnight(Square::s34()).toString());

  ASSERT_EQ(
    "000000000\n"
    "000000000\n"
    "001010000\n"
    "010101000\n"
    "011111000\n"
    "010101000\n"
    "000000000\n"
    "000000000\n"
    "000000000\n",
    AggressableTables::blackKnight(Square::s62()).toString());

  // white knight
  ASSERT_EQ(
    "000000000\n"
    "001010100\n"
    "000000000\n"
    "000000000\n"
    "000101000\n"
    "000000000\n"
    "000000000\n"
    "000000000\n"
    "000000000\n",
    AggressableTables::whiteKnight(Square::s56()).toString());

  ASSERT_EQ(
    "000000000\n"
    "000000000\n"
    "000000000\n"
    "000001010\n"
    "000001111\n"
    "000001010\n"
    "000000101\n"
    "000000000\n"
    "000000000\n",
    AggressableTables::whiteKnight(Square::s28()).toString());

  // black silver
  ASSERT_EQ(
    "000000000\n"
    "000011111\n"
    "000010101\n"
    "000011011\n"
    "000000000\n"
    "000011111\n"
    "000000000\n"
    "000000000\n"
    "000000000\n",
    AggressableTables::blackSilver(Square::s34()).toString());

  ASSERT_EQ(
    "010101000\n"
    "011011000\n"
    "011111000\n"
    "011111000\n"
    "000000000\n"
    "000000000\n"
    "000000000\n"
    "000000000\n"
    "000000000\n",
    AggressableTables::blackSilver(Square::s62()).toString());

  // white silver
  ASSERT_EQ(
    "000000000\n"
    "000000000\n"
    "000000000\n"
    "001111100\n"
    "000000000\n"
    "001101100\n"
    "001010100\n"
    "001111100\n"
    "000000000\n",
    AggressableTables::whiteSilver(Square::s56()).toString());

  ASSERT_EQ(
    "000000000\n"
    "000000000\n"
    "000000000\n"
    "000000000\n"
    "000000000\n"
    "000001111\n"
    "000001111\n"
    "000001101\n"
    "000001010\n",
    AggressableTables::whiteSilver(Square::s28()).toString());

  // black gold
  ASSERT_EQ(
    "000000000\n"
    "000000100\n"
    "000001010\n"
    "000011011\n"
    "000011111\n"
    "000011111\n"
    "000000000\n"
    "000000000\n"
    "000000000\n",
    AggressableTables::blackGold(Square::s34()).toString());

  // white gold
  ASSERT_EQ(
    "000000000\n"
    "000000000\n"
    "000000000\n"
    "001111100\n"
    "001111100\n"
    "001101100\n"
    "000101000\n"
    "000010000\n"
    "000000000\n",
    AggressableTables::whiteGold(Square::s56()).toString());

  // black bishop
  ASSERT_EQ(
    "011111011\n"
    "101111111\n"
    "010111111\n"
    "101011011\n"
    "010111011\n"
    "101110101\n"
    "011101010\n"
    "111010101\n"
    "110101010\n",
    AggressableTables::blackBishop(Square::s34()).toString());

  ASSERT_EQ(
    "110101101\n"
    "011011010\n"
    "111111101\n"
    "111111110\n"
    "111011111\n"
    "110101111\n"
    "101010111\n"
    "010101011\n"
    "101010101\n",
    AggressableTables::blackBishop(Square::s62()).toString());

  // white bishop
  ASSERT_EQ(
    "010101010\n"
    "101010101\n"
    "110101011\n"
    "111010111\n"
    "011101110\n"
    "101101101\n"
    "011111110\n"
    "111111111\n"
    "111101111\n",
    AggressableTables::whiteBishop(Square::s56()).toString());

  ASSERT_EQ(
    "111010101\n"
    "111101010\n"
    "111110101\n"
    "011111010\n"
    "101111101\n"
    "010111111\n"
    "101011110\n"
    "010101101\n"
    "101011010\n",
    AggressableTables::whiteBishop(Square::s28()).toString());

  // horse
  ASSERT_EQ(
    "011111111\n"
    "101111111\n"
    "010110110\n"
    "101111111\n"
    "011111111\n"
    "111110111\n"
    "111101011\n"
    "111010101\n"
    "110101010\n",
    AggressableTables::horse(Square::s43()).toString());
}
