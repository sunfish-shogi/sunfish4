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
#include <sstream>
#include <iomanip>

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

  static std::string stringify(const Position& position, const BookMoves& bookMoves) {
    std::ostringstream oss;
    float sum = 0.0;
    for (auto& m : bookMoves) {
      sum += m.count;
    }
    for (auto& m : bookMoves) {
      float rate = m.count / sum * 100;
      oss << m.move.toString(position) << "(" << std::setprecision(3) << rate << "%) ";
    }
    return oss.str();
  }

};

} // namespace sunfish

#endif // SUNFISH_BOOK_BOOKMOVESELECTOR_HPP__
