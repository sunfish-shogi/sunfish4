/* Bitboard.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_CORE_BASE_BITBOARD_HPP__
#define SUNFISH_CORE_BASE_BITBOARD_HPP__

#include "common/Def.hpp"
#include "core/base/Square.hpp"
#include "core/position/Bitset128.hpp"
#include <cstdint>

#define BB_FILES_1ST 5
#define BB_FILES_2ND 4

#define BB_SQUARES_1ST (BB_FILES_1ST*SQUARE_RANKS)
#define BB_SQUARES_2ND (BB_FILES_2ND*SQUARE_RANKS)

namespace sunfish {

class Bitboard : public Bitset128<Bitboard, BB_SQUARES_1ST, BB_SQUARES_2ND> {
public:

  using Bitset128::Bitset128;

};

} // namespace sunfish

#endif // SUNFISH_CORE_BASE_BITBOARD_HPP__
