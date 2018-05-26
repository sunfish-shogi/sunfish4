/* SCRDetector.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_SEARCH_SHEK_SCR_HPP__
#define SUNFISH_SEARCH_SHEK_SCR_HPP__

#include "common/Def.hpp"
#include "core/position/Zobrist.hpp"
#include <cstdint>

namespace sunfish {

struct Record;
struct Tree;

enum class SCRState {
  None,
  Draw,
  Win,
  Lose,
};

/**
 * Successive Checks Repetition Detector
 */
class SCRDetector {
public:

  using LengthType = size_t;

  static CONSTEXPR_CONST LengthType MaxLength = 32;

  struct Data {
    Zobrist::Type hash;
    bool check;
  };

  void clear() {
    length_ = 0;
  }

  void registerRecord(const Record& record);

  SCRState detectShort(const Tree& tree) const;

  SCRState detect(const Tree& tree) const;

private:

  LengthType length_;
  Data list_[MaxLength];

};

}

inline std::ostream& operator<<(std::ostream& os, sunfish::SCRState scrState) {
  switch (scrState) {
  case sunfish::SCRState::None:
    os << "None";
    break;
  case sunfish::SCRState::Draw:
    os << "Draw";
    break;
  case sunfish::SCRState::Win:
    os << "Win";
    break;
  case sunfish::SCRState::Lose:
    os << "Lose";
    break;
  }
  return os;
}

#endif // SUNFISH_SEARCH_SHEK_SCR_HPP__
