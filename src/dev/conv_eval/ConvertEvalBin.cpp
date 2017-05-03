/* ConvertEvalBin.cpp
 *
 * Kubo Ryosuke
 */

#include "dev/conv_eval/ConvertEvalBin.hpp"
#include "search/eval/Evaluator.hpp"
#include "logger/Logger.hpp"
#include <fstream>
#include <cstring>

namespace sunfish {

bool ConvertEvalBin ::convert() {
  const char* const srcPath = "eval.bin";
  const char* const dstPath = "eval.bin.new";

  auto fv = std::unique_ptr<Evaluator::FVType>(new Evaluator::FVType);

  load(srcPath, *fv.get());

  std::ofstream file(dstPath, std::ios::out | std::ios::binary);
  if (!file) {
    LOG(warning) << "failed to open: " << dstPath;
    return false;
  }

  const char* ver = "2017.05.23.0";
  uint8_t len = strlen(ver);
  file.write(reinterpret_cast<const char*>(&len), sizeof(len));
  file.write(reinterpret_cast<const char*>(ver), len);

#define WRITE(F) file.write(reinterpret_cast<const char*>(&fv->F), sizeof(fv->F))
  WRITE(kingHand);

  WRITE(kingPieceR);
  WRITE(kingPieceXR);
  WRITE(kingPieceYR);
  WRITE(kingPiece);

  WRITE(kingNeighborHand);

  WRITE(kingNeighborPieceR);
  WRITE(kingNeighborPieceXR);
  WRITE(kingNeighborPieceYR);
  WRITE(kingNeighborPiece);

  WRITE(kingKingHand);
  WRITE(kingKingPiece);

  WRITE(bRookVer);
  WRITE(kingBRookVerR);
  WRITE(kingBRookVerXR);
  WRITE(kingBRookVerYR);
  WRITE(kingBRookVer);

  WRITE(wRookVer);
  WRITE(kingWRookVerR);
  WRITE(kingWRookVerXR);
  WRITE(kingWRookVerYR);
  WRITE(kingWRookVer);

  WRITE(bRookHor);
  WRITE(kingBRookHorR);
  WRITE(kingBRookHorXR);
  WRITE(kingBRookHorYR);
  WRITE(kingBRookHor);

  WRITE(wRookHor);
  WRITE(kingWRookHorR);
  WRITE(kingWRookHorXR);
  WRITE(kingWRookHorYR);
  WRITE(kingWRookHor);

  WRITE(bBishopDiagL45);
  WRITE(kingBBishopDiagL45R);
  WRITE(kingBBishopDiagL45XR);
  WRITE(kingBBishopDiagL45YR);
  WRITE(kingBBishopDiagL45);

  WRITE(wBishopDiagL45);
  WRITE(kingWBishopDiagL45R);
  WRITE(kingWBishopDiagL45XR);
  WRITE(kingWBishopDiagL45YR);
  WRITE(kingWBishopDiagL45);

  WRITE(bBishopDiagR45);
  WRITE(kingBBishopDiagR45R);
  WRITE(kingBBishopDiagR45XR);
  WRITE(kingBBishopDiagR45YR);
  WRITE(kingBBishopDiagR45);

  WRITE(wBishopDiagR45);
  WRITE(kingWBishopDiagR45R);
  WRITE(kingWBishopDiagR45XR);
  WRITE(kingWBishopDiagR45YR);
  WRITE(kingWBishopDiagR45);

  WRITE(bLance);
  WRITE(kingBLanceR);
  WRITE(kingBLanceXR);
  WRITE(kingBLanceYR);
  WRITE(kingBLance);

  WRITE(wLance);
  WRITE(kingWLanceR);
  WRITE(kingWLanceXR);
  WRITE(kingWLanceYR);
  WRITE(kingWLance);

  int16_t x;
  for (int i = 0; i < 81 * 10 * 2; i++) {
    file.write(reinterpret_cast<const char*>(&x), sizeof(int16_t));
  }
  for (int i = 0; i < 81 * 26 * 2; i++) {
    file.write(reinterpret_cast<const char*>(&x), sizeof(int16_t));
  }

  file.close();

  return true;
}

} // namespace sunfish
