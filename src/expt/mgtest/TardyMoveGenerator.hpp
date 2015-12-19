/* TardyMoveGenerator.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_EXPT_MGTEST_TARDYMOVEGENERATOR_HPP__
#define SUNFISH_EXPT_MGTEST_TARDYMOVEGENERATOR_HPP__

#include "core/position/Position.hpp"
#include "core/move/Moves.hpp"

namespace sunfish {

class TardyMoveGenerator {
public:

  static void generateMoves(const Position& position, Moves& moves);

  static void generateEvasions(const Position& position, CheckState cs, Moves& moves);

  TardyMoveGenerator() = delete;

};

} // namespace sunfish

#endif // SUNFISH_EXPT_MGTEST_TARDYMOVEGENERATOR_HPP__
