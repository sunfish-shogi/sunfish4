/* Book.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_BOOK_BOOK_HPP__
#define SUNFISH_BOOK_BOOK_HPP__

#include "common/Def.hpp"
#include "core/move/Move.hpp"
#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>

namespace sunfish {

class Position;

struct BookMove {
  Move     move;  // 32 bits
  uint16_t count; // 16 bits
};

using BookMoves = std::vector<BookMove>;

class Book {
public:

  using BookMap = std::unordered_map<std::string, BookMoves>;

public:

  void insert(const Position& position, Move move) {
    insert(position, move, 1);
  }

  void insert(const Position& position, Move move, int count);

  const BookMoves* get(const Position& position) const;

  void clear() {
    map_.clear();
  }

  bool load();

  bool load(std::istream& is);

  bool save() const;

  bool save(std::ostream& os) const;

private:

  BookMap map_;

};

} // namespace sunfish

#endif // SUNFISH_BOOK_BOOK_HPP__
