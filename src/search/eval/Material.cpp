/* Material.cpp
 *
 * Kubo Ryosuke
 */

#include "search/eval/Material.hpp"

namespace sunfish {

namespace material {

Score scores[PieceNumber::Num] = {
  /*  0 */ material::Pawn,
  /*  1 */ material::Lance,
  /*  2 */ material::Knight,
  /*  3 */ material::Silver,
  /*  4 */ material::Gold,
  /*  5 */ material::Bishop,
  /*  6 */ material::Rook,
  /*  7 */ Score::materialInfinity(),
  /*  8 */ material::Tokin,
  /*  9 */ material::ProLance,
  /* 10 */ material::ProKnight,
  /* 11 */ material::ProSilver,
  /* 12 */ 0,
  /* 13 */ material::Horse,
  /* 14 */ material::Dragon,
  /* 15 */ 0,
  /* 16 */ material::Pawn,
  /* 17 */ material::Lance,
  /* 18 */ material::Knight,
  /* 19 */ material::Silver,
  /* 20 */ material::Gold,
  /* 21 */ material::Bishop,
  /* 22 */ material::Rook,
  /* 23 */ Score::materialInfinity(),
  /* 24 */ material::Tokin,
  /* 25 */ material::ProLance,
  /* 26 */ material::ProKnight,
  /* 27 */ material::ProSilver,
  /* 28 */ 0,
  /* 29 */ material::Horse,
  /* 30 */ material::Dragon,
};

Score exchangeScores[PieceNumber::Num] = {
  /*  0 */ material::PawnEx,
  /*  1 */ material::LanceEx,
  /*  2 */ material::KnightEx,
  /*  3 */ material::SilverEx,
  /*  4 */ material::GoldEx,
  /*  5 */ material::BishopEx,
  /*  6 */ material::RookEx,
  /*  7 */ Score::materialInfinity(),
  /*  8 */ material::TokinEx,
  /*  9 */ material::ProLanceEx,
  /* 10 */ material::ProKnightEx,
  /* 11 */ material::ProSilverEx,
  /* 12 */ 0,
  /* 13 */ material::HorseEx,
  /* 14 */ material::DragonEx,
  /* 15 */ 0,
  /* 16 */ material::PawnEx,
  /* 17 */ material::LanceEx,
  /* 18 */ material::KnightEx,
  /* 19 */ material::SilverEx,
  /* 20 */ material::GoldEx,
  /* 21 */ material::BishopEx,
  /* 22 */ material::RookEx,
  /* 23 */ Score::materialInfinity(),
  /* 24 */ material::TokinEx,
  /* 25 */ material::ProLanceEx,
  /* 26 */ material::ProKnightEx,
  /* 27 */ material::ProSilverEx,
  /* 28 */ 0,
  /* 29 */ material::HorseEx,
  /* 30 */ material::DragonEx,
};

Score promotionScores[PieceNumber::Num] = {
  /*  0 */ material::Tokin - material::Pawn,
  /*  1 */ material::ProLance - material::Lance,
  /*  2 */ material::ProKnight - material::Knight,
  /*  3 */ material::ProSilver - material::Silver,
  /*  4 */ 0,
  /*  5 */ material::Horse - material::Bishop,
  /*  6 */ material::Dragon - material::Rook,
  /*  7 */ 0,
  /*  8 */ 0,
  /*  9 */ 0,
  /* 10 */ 0,
  /* 11 */ 0,
  /* 12 */ 0,
  /* 13 */ 0,
  /* 14 */ 0,
  /* 15 */ 0,
  /* 16 */ material::Tokin - material::Pawn,
  /* 17 */ material::ProLance - material::Lance,
  /* 18 */ material::ProKnight - material::Knight,
  /* 19 */ material::ProSilver - material::Silver,
  /* 20 */ 0,
  /* 21 */ material::Horse - material::Bishop,
  /* 22 */ material::Dragon - material::Rook,
  /* 23 */ 0,
  /* 24 */ 0,
  /* 25 */ 0,
  /* 26 */ 0,
  /* 27 */ 0,
  /* 28 */ 0,
  /* 29 */ 0,
  /* 30 */ 0,
};

} // namespace material

} // namespace sunfish
