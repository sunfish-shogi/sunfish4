/* Book.cpp
 *
 * Kubo Ryosuke
 */

#include "book/Book.hpp"
#include "core/position/Position.hpp"
#include "core/record/SfenParser.hpp"
#include "common/string/StringUtil.hpp"
#include <fstream>
#include <cstring>

namespace {

const char* const BookBin = "book.bin";

} // namespace

namespace sunfish {

void Book::insert(const Position& position, Move move, int count) {
  std::string sfen = position.toStringSFEN();
  auto ite = map_.find(sfen);
  if (ite == map_.end()) {
    map_[sfen].push_back({ move, static_cast<uint16_t>(count) });
    return;
  }

  auto& bookMoves = ite->second;

  for (auto& bookMove : bookMoves) {
    if (bookMove.move == move) {
      bookMove.count += static_cast<uint16_t>(count);
      return;
    }
  }

  bookMoves.push_back({ move, static_cast<uint16_t>(count) });
}

void Book::sort() {
  for (auto& pair : map_) {
    std::sort(pair.second.begin(), pair.second.end(), [](BookMove lhs, BookMove rhs) {
      return lhs.count >= rhs.count;
    });
  }
}

const BookMoves* Book::get(const Position& position) const {
  std::string sfen = position.toStringSFEN();
  auto ite = map_.find(sfen);
  return ite != map_.end() ? &ite->second : nullptr;
}

bool Book::load() {
  std::ifstream file(BookBin);
  if (!file) {
    LOG(error) << "could not open a file: " << BookBin;
    return false;
  }

  bool ok = load(file);

  file.close();

  return ok;
}

bool Book::load(std::istream& is) {
  std::string buffer;
  Position pos;
  bool isPosInitialized = false;

  while (true) {
    getline(is, buffer);

    if (is.eof()) {
      return true;
    }

    if (is.fail()) {
      LOG(error) << "file I/O error";
      return false;
    }

    if (strncmp(buffer.c_str(), "sfen ", 5) == 0) {
      bool ok = SfenParser::parsePosition(buffer.c_str() + 5, pos);
      if (!ok) {
        return false;
      }

      isPosInitialized = true;

    } else {
      if (!isPosInitialized) {
        LOG(error) << "position is required before moves";
        return false;
      }

      auto columns = StringUtil::split(buffer, ' ');
      if (columns.size() < 2) {
        LOG(error) << "invalid format";
        return false;
      }

      Move move;
      bool ok = SfenParser::parseMove(columns[0], move);
      if (!ok) {
        return false;
      }

      if (move.isNone()) {
        LOG(error) << "first column is required: " << buffer;
        return false;
      }

      int count = std::stoi(columns[1]);

      insert(pos, move, count);

    }
  }
}

bool Book::save() const {
  std::ofstream file(BookBin);
  if (!file) {
    LOG(error) << "could not open a file: " << BookBin;
    return false;
  }

  bool ok = save(file);

  file.close();

  return ok;
}

bool Book::save(std::ostream& os) const {
  for (const auto& pair : map_) {
    auto& sfen = pair.first;
    auto& bookMoves = pair.second;

    os << "sfen " << sfen << '\n';
    for (auto& bookMove : bookMoves) {
      os << bookMove.move.toStringSFEN() << ' ' << bookMove.count << '\n';
    }
  }

  return true;
}

} // namespace sunfish
