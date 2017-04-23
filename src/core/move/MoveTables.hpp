/* MoveTables.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_CORE_MOVE_MOVETABLES_HPP__
#define SUNFISH_CORE_MOVE_MOVETABLES_HPP__

#include "core/base/Piece.hpp"
#include "core/position/Bitboard.hpp"
#include <array>

namespace sunfish {

class MoveTables {
private:

  using MovableInOneStepType = std::array<uint16_t, PieceNumber::Num>;
  using MovableInLongStepType = std::array<uint16_t, PieceNumber::Num>;
  using OneStepTableType = std::array<Bitboard, NUMBER_OF_SQUARES>;
  using VerTableType = std::array<std::array<Bitboard, 0x80>, NUMBER_OF_SQUARES>;
  using HorTableType = std::array<std::array<Bitboard, 0x80>, NUMBER_OF_SQUARES>;
  using DiagTableType = std::array<std::array<Bitboard, 0x80>, NUMBER_OF_SQUARES>;

public:

  MoveTables() = delete;

  static void initialize();

  static bool isMovableInOneStep(const Piece& piece, Direction dir) {
    return MovableInOneStep[piece.raw()] & (static_cast<uint16_t>(0x01) << static_cast<int32_t>(dir));
  }

  static bool isMovableInLongStep(const Piece& piece, Direction dir) {
    return MovableInLongStep[piece.raw()] & (static_cast<uint16_t>(0x01) << static_cast<int32_t>(dir));
  }

  static const Bitboard& blackKnight(const Square& square) {
    return BlackKnight[square.raw()];
  }
  static const Bitboard& whiteKnight(const Square& square) {
    return WhiteKnight[square.raw()];
  }
  static const Bitboard& blackSilver(const Square& square) {
    return BlackSilver[square.raw()];
  }
  static const Bitboard& whiteSilver(const Square& square) {
    return WhiteSilver[square.raw()];
  }
  static const Bitboard& blackGold(const Square& square) {
    return BlackGold[square.raw()];
  }
  static const Bitboard& whiteGold(const Square& square) {
    return WhiteGold[square.raw()];
  }
  static const Bitboard& king(const Square& square) {
    return King[square.raw()];
  }
  static const Bitboard& cross(const Square& square) {
    return Cross[square.raw()];
  }
  static const Bitboard& x(const Square& square) {
    return X[square.raw()];
  }
  static const Bitboard& neighbor3x3(const Square& square) {
    return Neighbor3x3[square.raw()];
  }
  static const Bitboard& neighbor5x5(const Square& square) {
    return Neighbor5x5[square.raw()];
  }
  static const Bitboard& blackLance(const Bitboard& occ, const Square& square);
  static const Bitboard& whiteLance(const Bitboard& occ, const Square& square);
  static const Bitboard& ver(const Bitboard& occ, const Square& square);
  static const Bitboard& hor(const RotatedBitboard& occ, const Square& square);
  static const Bitboard& diagR45(const RotatedBitboard& occ, const Square& square);
  static const Bitboard& diagL45(const RotatedBitboard& occ, const Square& square);

private:

  static void initializeDirectionTable();
  static void initializeBitboards();

  static MovableInOneStepType MovableInOneStep;
  static MovableInLongStepType MovableInLongStep;
  static OneStepTableType BlackKnight;
  static OneStepTableType WhiteKnight;
  static OneStepTableType BlackSilver;
  static OneStepTableType WhiteSilver;
  static OneStepTableType BlackGold;
  static OneStepTableType WhiteGold;
  static OneStepTableType King;
  static OneStepTableType Cross;
  static OneStepTableType X;
  static OneStepTableType Neighbor3x3;
  static OneStepTableType Neighbor5x5;
  static VerTableType BlackLance;
  static VerTableType WhiteLance;
  static VerTableType Ver;
  static HorTableType Hor;
  static DiagTableType DiagRight45;
  static DiagTableType DiagLeft45;

};

class AggressableTables {
public:

  using TableType = std::array<Bitboard, NUMBER_OF_SQUARES>;

  AggressableTables() = delete;

  static void initialize();

  static const Bitboard& blackPawn(const Square& square) {
    return BlackPawn[square.raw()];
  }
  static const Bitboard& whitePawn(const Square& square) {
    return WhitePawn[square.raw()];
  }
  static const Bitboard& blackLance(const Square& square) {
    return BlackLance[square.raw()];
  }
  static const Bitboard& whiteLance(const Square& square) {
    return WhiteLance[square.raw()];
  }
  static const Bitboard& blackKnight(const Square& square) {
    return BlackKnight[square.raw()];
  }
  static const Bitboard& whiteKnight(const Square& square) {
    return WhiteKnight[square.raw()];
  }
  static const Bitboard& blackSilver(const Square& square) {
    return BlackSilver[square.raw()];
  }
  static const Bitboard& whiteSilver(const Square& square) {
    return WhiteSilver[square.raw()];
  }
  static const Bitboard& blackGold(const Square& square) {
    return BlackGold[square.raw()];
  }
  static const Bitboard& whiteGold(const Square& square) {
    return WhiteGold[square.raw()];
  }
  static const Bitboard& blackBishop(const Square& square) {
    return BlackBishop[square.raw()];
  }
  static const Bitboard& whiteBishop(const Square& square) {
    return WhiteBishop[square.raw()];
  }
  static const Bitboard& horse(const Square& square) {
    return Horse[square.raw()];
  }

private:

  static void initialize(TableType& table, Piece piece);

  static TableType BlackPawn;
  static TableType WhitePawn;
  static TableType BlackLance;
  static TableType WhiteLance;
  static TableType BlackKnight;
  static TableType WhiteKnight;
  static TableType BlackSilver;
  static TableType WhiteSilver;
  static TableType BlackGold;
  static TableType WhiteGold;
  static TableType BlackBishop;
  static TableType WhiteBishop;
  static TableType Horse;

};

} // namespace sunfish

#endif // SUNFISH_CORE_MOVE_MOVETABLES_HPP__
