/* BookUtil.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_BOOK_BOOKMOVESELECTOR_HPP__
#define SUNFISH_BOOK_BOOKMOVESELECTOR_HPP__

#include "common/math/Random.hpp"
#include "core/move/Move.hpp"
#include "core/position/Position.hpp"
#include "book/Book.hpp"

namespace sunfish {

class BookUtil {
public:

  BookUtil() = delete;

  static Move select(const Book& book, const Position& position, Random& random) {
    auto bookMoves = book.get(position);
    if (bookMoves == nullptr || bookMoves->size() == 0) {
      return Move::none();
    }

    unsigned idx = random.nonuniform(bookMoves->size(), [bookMoves](unsigned i) {
      return bookMoves->at(i).count;
    });
    return bookMoves->at(idx).move;
  }

};

} // namespace sunfish

#endif // SUNFISH_BOOK_BOOKMOVESELECTOR_HPP__
