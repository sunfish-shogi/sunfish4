/* TablePrinter.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_COMMON_STRING_TABLEPRINTER_HPP__
#define SUNFISH_COMMON_STRING_TABLEPRINTER_HPP__

#include <string>
#include <sstream>
#include <iomanip>
#include <list>
#include <vector>
#include <algorithm>

namespace sunfish {

class TablePrinter {
public:

  using Columns = std::list<std::string>;
  using Rows = std::list<Columns>;

  class Raw {
  public:

    Raw(TablePrinter& printer) : printer_(printer) {
    }

    template<class T>
    Raw& operator<<(T t) {
      std::ostringstream oss;
      oss << t;
      (*printer_.table_.rbegin()).push_back(oss.str());
      return *this;
    }

  private:

    TablePrinter& printer_;

  };

  TablePrinter(const char* separator = " ",
               const char* prefix = "",
               const char* suffix = "") :
      prefix_(prefix),
      separator_(separator),
      suffix_(suffix) {
  }

  Raw row() {
    table_.push_back(std::list<std::string>());
    return Raw(*this);
  }

  std::string stringify() const {
    auto widthList = generateMaxLengthList();
    std::ostringstream oss;
    oss << std::left;
    for (auto& raw : table_) {
      int index = 0;
      oss << prefix_;
      for (auto& column : raw) {
        if (index != 0) {
          oss << separator_;
        }
        oss << std::setw(widthList[index]) << column;
        index++;
      }
      oss << suffix_ << '\n';
    }
    return oss.str();
  }

private:

  std::vector<std::string::size_type> generateMaxLengthList() const {
    std::vector<std::string::size_type> list(maxSize());
    for (auto& raw : table_) {
      int index = 0;
      for (auto& column : raw) {
        list[index] = std::max(list[index], column.length());
        index++;
      }
    }
    return list;
  }

  size_t maxSize() const {
    size_t maxSize = 0;
    for (auto& raw : table_) {
      maxSize = std::max(maxSize, raw.size());
    }
    return maxSize;
  }

private:

  Rows table_;
  std::string prefix_;
  std::string separator_;
  std::string suffix_;

};

} // namespace sunfish

#endif // SUNFISH_COMMON_STRING_TABLEPRINTER_HPP__
