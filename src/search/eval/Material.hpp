/* Material.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_SEARCH_EVAL_MATERIAL__
#define SUNFISH_SEARCH_EVAL_MATERIAL__

#include "common/Def.hpp"
#include "search/eval/Score.hpp"
#include "core/base/Piece.hpp"

#define MATERIAL_SCORE_PAWN       87
#define MATERIAL_SCORE_LANCE      232
#define MATERIAL_SCORE_KNIGHT     257
#define MATERIAL_SCORE_SILVER     369
#define MATERIAL_SCORE_GOLD       444
#define MATERIAL_SCORE_BISHOP     569
#define MATERIAL_SCORE_ROOK       642
#define MATERIAL_SCORE_TOKIN      534
#define MATERIAL_SCORE_PRO_LANCE  489
#define MATERIAL_SCORE_PRO_KNIGHT 510
#define MATERIAL_SCORE_PRO_SILVER 495
#define MATERIAL_SCORE_HORSE      827
#define MATERIAL_SCORE_DRAGON     945

namespace sunfish {

namespace material {

CONSTEXPR_CONST int16_t Pawn       = MATERIAL_SCORE_PAWN;
CONSTEXPR_CONST int16_t Lance      = MATERIAL_SCORE_LANCE;
CONSTEXPR_CONST int16_t Knight     = MATERIAL_SCORE_KNIGHT;
CONSTEXPR_CONST int16_t Silver     = MATERIAL_SCORE_SILVER;
CONSTEXPR_CONST int16_t Gold       = MATERIAL_SCORE_GOLD;
CONSTEXPR_CONST int16_t Bishop     = MATERIAL_SCORE_BISHOP;
CONSTEXPR_CONST int16_t Rook       = MATERIAL_SCORE_ROOK;
CONSTEXPR_CONST int16_t Tokin      = MATERIAL_SCORE_TOKIN;
CONSTEXPR_CONST int16_t ProLance   = MATERIAL_SCORE_PRO_LANCE;
CONSTEXPR_CONST int16_t ProKnight  = MATERIAL_SCORE_PRO_KNIGHT;
CONSTEXPR_CONST int16_t ProSilver  = MATERIAL_SCORE_PRO_SILVER;
CONSTEXPR_CONST int16_t Horse      = MATERIAL_SCORE_HORSE;
CONSTEXPR_CONST int16_t Dragon     = MATERIAL_SCORE_DRAGON;

CONSTEXPR_CONST int16_t PawnEx       = Pawn * 2;
CONSTEXPR_CONST int16_t LanceEx      = Lance * 2;
CONSTEXPR_CONST int16_t KnightEx     = Knight * 2;
CONSTEXPR_CONST int16_t SilverEx     = Silver * 2;
CONSTEXPR_CONST int16_t GoldEx       = Gold * 2;
CONSTEXPR_CONST int16_t BishopEx     = Bishop * 2;
CONSTEXPR_CONST int16_t RookEx       = Rook * 2;
CONSTEXPR_CONST int16_t TokinEx      = Tokin + Pawn;
CONSTEXPR_CONST int16_t ProLanceEx   = ProLance + Lance;
CONSTEXPR_CONST int16_t ProKnightEx  = ProKnight + Knight;
CONSTEXPR_CONST int16_t ProSilverEx  = ProSilver + Silver;
CONSTEXPR_CONST int16_t HorseEx      = Horse + Bishop;
CONSTEXPR_CONST int16_t DragonEx     = Dragon + Rook;

extern Score scores[PieceNumber::Num];
extern Score exchangeScores[PieceNumber::Num];
extern Score promotionScores[PieceNumber::Num];

inline Score score(const Piece& piece) {
  return scores[piece.raw()];
}

inline Score exchangeScore(const Piece& piece) {
  return exchangeScores[piece.raw()];
}

inline Score promotionScore(const Piece& piece) {
  return promotionScores[piece.raw()];
}

} // namespace material

} // namespace sunfish

#endif // SUNFISH_SEARCH_EVAL_MATERIAL__
