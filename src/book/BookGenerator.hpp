/* BookGenerator.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_BOOK_BOOKGENERATOR_HPP__
#define SUNFISH_BOOK_BOOKGENERATOR_HPP__

#include "book/Book.hpp"
#include <utility>

namespace sunfish {

class BookGenerator {
public:

  template <class T>
  BookGenerator(T&& path) :
    path_(std::forward<T>(path)),
    limit_(0),
    handicap_(false) {
  }

  void setLimit(unsigned limit) {
    limit_ = limit;
  }

  void setHandicap(bool enable) {
    handicap_ = enable;
  }

  bool generate();

  const Book& getBook() const {
    return book_;
  }

private:

  bool generate(const std::string& path);

  std::string path_;
  Book book_;
  unsigned limit_;
  bool handicap_;

};

} // namespace sunfish

#endif // SUNFISH_BOOK_BOOKGENERATOR_HPP__
