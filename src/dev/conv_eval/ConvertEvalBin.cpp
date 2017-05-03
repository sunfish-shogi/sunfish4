/* ConvertEvalBin.cpp
 *
 * Kubo Ryosuke
 */

#include "dev/conv_eval/ConvertEvalBin.hpp"
#include "search/eval/Evaluator.hpp"
#include "logger/Logger.hpp"
#include <fstream>
#include <cstring>

namespace {

void write(std::ofstream& file, int16_t* p, size_t s) {
  for (size_t i = 0; i < s; i++) {
    file.write(reinterpret_cast<const char*>(&p[i]), sizeof(int16_t));
  }
}

} // namespace

namespace sunfish {

bool ConvertEvalBin ::convert() {
  const char* const srcPath = "eval.bin";
  const char* const dstPath = "eval.bin.new";

  auto fv = std::unique_ptr<Evaluator::OFVType>(new Evaluator::OFVType);

  load(srcPath, *fv.get());

  std::ofstream file(dstPath, std::ios::out | std::ios::binary);
  if (!file) {
    LOG(warning) << "failed to open: " << dstPath;
    return false;
  }

  const char* ver = "2018.05.29.0-light";
  uint8_t len = strlen(ver);
  file.write(reinterpret_cast<const char*>(&len), sizeof(len));
  file.write(reinterpret_cast<const char*>(ver), len);

#define WRITE(F) write(file, reinterpret_cast<int16_t*>(&fv->F), sizeof(fv->F) / sizeof(int16_t))
  WRITE(kingHand);

  WRITE(kingPiece);

  //WRITE(kingPieceNeighborX);

  //WRITE(kingPieceNeighborY);

  //WRITE(kingPieceNeighborXY);

  //WRITE(kingPieceNeighborXY2);

  WRITE(kingNeighborHand);

  //WRITE(kingNeighborPiece);

  WRITE(kingKingHand);
  //WRITE(kingKingPiece);

  WRITE(kingBRookUp);
  WRITE(kingWRookUp);
  WRITE(kingBRookDown);
  WRITE(kingWRookDown);
  WRITE(kingBRookLeft);
  WRITE(kingWRookLeft);
  WRITE(kingBRookRight);
  WRITE(kingWRookRight);
  WRITE(kingBBishopLeftUp45);
  WRITE(kingWBishopLeftUp45);
  WRITE(kingBBishopRightDown45);
  WRITE(kingWBishopRightDown45);
  WRITE(kingBBishopRightUp45);
  WRITE(kingWBishopRightUp45);
  WRITE(kingBBishopLeftDown45);
  WRITE(kingWBishopLeftDown45);
  WRITE(kingBLance);
  WRITE(kingWLance);

  WRITE(kingAllyEffect9);
  WRITE(kingEnemyEffect9);
  WRITE(kingAllyEffect25);
  WRITE(kingEnemyEffect25);
  WRITE(kingEffect9Diff);
  WRITE(kingEffect25Diff);

  file.close();

  return true;
}

} // namespace sunfish
