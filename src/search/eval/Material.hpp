/* Material.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_SEARCH_EVAL_MATERIAL__
#define SUNFISH_SEARCH_EVAL_MATERIAL__

#include "common/Def.hpp"
#include "search/eval/Score.hpp"
#include "core/base/Piece.hpp"

#define MATERIAL_SCORE_PAWN       115
#define MATERIAL_SCORE_LANCE      246
#define MATERIAL_SCORE_KNIGHT     273
#define MATERIAL_SCORE_SILVER     414
#define MATERIAL_SCORE_GOLD       504
#define MATERIAL_SCORE_BISHOP     625
#define MATERIAL_SCORE_ROOK       695
#define MATERIAL_SCORE_TOKIN      535
#define MATERIAL_SCORE_PRO_LANCE  424
#define MATERIAL_SCORE_PRO_KNIGHT 469
#define MATERIAL_SCORE_PRO_SILVER 429
#define MATERIAL_SCORE_HORSE      768
#define MATERIAL_SCORE_DRAGON     903

#define MATERIAL_SCORE_PAWN_EX       (MATERIAL_SCORE_PAWN * 2)
#define MATERIAL_SCORE_LANCE_EX      (MATERIAL_SCORE_LANCE * 2)
#define MATERIAL_SCORE_KNIGHT_EX     (MATERIAL_SCORE_KNIGHT * 2)
#define MATERIAL_SCORE_SILVER_EX     (MATERIAL_SCORE_SILVER * 2)
#define MATERIAL_SCORE_GOLD_EX       (MATERIAL_SCORE_GOLD * 2)
#define MATERIAL_SCORE_BISHOP_EX     (MATERIAL_SCORE_BISHOP * 2)
#define MATERIAL_SCORE_ROOK_EX       (MATERIAL_SCORE_ROOK * 2)
#define MATERIAL_SCORE_TOKIN_EX      (MATERIAL_SCORE_TOKIN + MATERIAL_SCORE_PAWN)
#define MATERIAL_SCORE_PRO_LANCE_EX  (MATERIAL_SCORE_PRO_LANCE + MATERIAL_SCORE_LANCE)
#define MATERIAL_SCORE_PRO_KNIGHT_EX (MATERIAL_SCORE_PRO_KNIGHT + MATERIAL_SCORE_KNIGHT)
#define MATERIAL_SCORE_PRO_SILVER_EX (MATERIAL_SCORE_PRO_SILVER + MATERIAL_SCORE_SILVER)
#define MATERIAL_SCORE_HORSE_EX      (MATERIAL_SCORE_HORSE + MATERIAL_SCORE_BISHOP)
#define MATERIAL_SCORE_DRAGON_EX     (MATERIAL_SCORE_DRAGON + MATERIAL_SCORE_ROOK)

namespace sunfish {

namespace material {

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

#if LEARNING
inline Score pawn()      { return scores[PieceNumber::Pawn]; }
inline Score lance()     { return scores[PieceNumber::Lance]; }
inline Score knight()    { return scores[PieceNumber::Knight]; }
inline Score silver()    { return scores[PieceNumber::Silver]; }
inline Score gold()      { return scores[PieceNumber::Gold]; }
inline Score bishop()    { return scores[PieceNumber::Bishop]; }
inline Score rook()      { return scores[PieceNumber::Rook]; }
inline Score tokin()     { return scores[PieceNumber::Tokin]; }
inline Score proLance()  { return scores[PieceNumber::ProLance]; }
inline Score proKnight() { return scores[PieceNumber::ProKnight]; }
inline Score proSilver() { return scores[PieceNumber::ProSilver]; }
inline Score horse()     { return scores[PieceNumber::Horse]; }
inline Score dragon()    { return scores[PieceNumber::Dragon]; }

inline Score pawnEx()      { return exchangeScores[PieceNumber::Pawn]; }
inline Score lanceEx()     { return exchangeScores[PieceNumber::Lance]; }
inline Score knightEx()    { return exchangeScores[PieceNumber::Knight]; }
inline Score silverEx()    { return exchangeScores[PieceNumber::Silver]; }
inline Score goldEx()      { return exchangeScores[PieceNumber::Gold]; }
inline Score bishopEx()    { return exchangeScores[PieceNumber::Bishop]; }
inline Score rookEx()      { return exchangeScores[PieceNumber::Rook]; }
inline Score tokinEx()     { return exchangeScores[PieceNumber::Tokin]; }
inline Score proLanceEx()  { return exchangeScores[PieceNumber::ProLance]; }
inline Score proKnightEx() { return exchangeScores[PieceNumber::ProKnight]; }
inline Score proSilverEx() { return exchangeScores[PieceNumber::ProSilver]; }
inline Score horseEx()     { return exchangeScores[PieceNumber::Horse]; }
inline Score dragonEx()    { return exchangeScores[PieceNumber::Dragon]; }
#else // LEARNING
inline Score pawn()      { return MATERIAL_SCORE_PAWN; }
inline Score lance()     { return MATERIAL_SCORE_LANCE; }
inline Score knight()    { return MATERIAL_SCORE_KNIGHT; }
inline Score silver()    { return MATERIAL_SCORE_SILVER; }
inline Score gold()      { return MATERIAL_SCORE_GOLD; }
inline Score bishop()    { return MATERIAL_SCORE_BISHOP; }
inline Score rook()      { return MATERIAL_SCORE_ROOK; }
inline Score tokin()     { return MATERIAL_SCORE_TOKIN; }
inline Score proLance()  { return MATERIAL_SCORE_PRO_LANCE; }
inline Score proKnight() { return MATERIAL_SCORE_PRO_KNIGHT; }
inline Score proSilver() { return MATERIAL_SCORE_PRO_SILVER; }
inline Score horse()     { return MATERIAL_SCORE_HORSE; }
inline Score dragon()    { return MATERIAL_SCORE_DRAGON; }

inline Score pawnEx()      { return MATERIAL_SCORE_PAWN_EX; }
inline Score lanceEx()     { return MATERIAL_SCORE_LANCE_EX; }
inline Score knightEx()    { return MATERIAL_SCORE_KNIGHT_EX; }
inline Score silverEx()    { return MATERIAL_SCORE_SILVER_EX; }
inline Score goldEx()      { return MATERIAL_SCORE_GOLD_EX; }
inline Score bishopEx()    { return MATERIAL_SCORE_BISHOP_EX; }
inline Score rookEx()      { return MATERIAL_SCORE_ROOK_EX; }
inline Score tokinEx()     { return MATERIAL_SCORE_TOKIN_EX; }
inline Score proLanceEx()  { return MATERIAL_SCORE_PRO_LANCE_EX; }
inline Score proKnightEx() { return MATERIAL_SCORE_PRO_KNIGHT_EX; }
inline Score proSilverEx() { return MATERIAL_SCORE_PRO_SILVER_EX; }
inline Score horseEx()     { return MATERIAL_SCORE_HORSE_EX; }
inline Score dragonEx()    { return MATERIAL_SCORE_DRAGON_EX; }
#endif // LEARNING

} // namespace material

} // namespace sunfish

#endif // SUNFISH_SEARCH_EVAL_MATERIAL__
