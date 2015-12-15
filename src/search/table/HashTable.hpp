/* HashTable.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_SEACH_TABLE_HASHTABLE_HPP__
#define SUNFISH_SEACH_TABLE_HASHTABLE_HPP__

#include "common/Def.hpp"
#include "common/memory/Memory.hpp"
#include "core/position/Zobrist.hpp"
#include <cstdint>

namespace sunfish {

/**
 * A base class for hash table
 */
template <class E> class HashTable {
public:

  using ElementType = E;
  using SizeType = uint32_t;

  static CONSTEXPR_CONST unsigned DefaultBits = 18;

public:

  HashTable(unsigned bits = DefaultBits) : table_(nullptr), size_(0) {
    clear(bits);
  }
  HashTable(const HashTable&) = delete;
  HashTable(HashTable&&) = delete;

  ~HashTable() {
    delete [] table_;
  }

  HashTable& operator=(const HashTable&) = delete;
  HashTable& operator=(HashTable&&) = delete;

  void clear(unsigned bits = 0) {
    SizeType newSize = 1 << bits;

    if (bits != 0 && size_ != newSize) {
      // resize
      size_ = newSize;
      mask_ = size_ - 1;
      if (table_ != nullptr) {
        delete[] table_;
      }
      table_ = new ElementType[size_];

    } else {
      // clear all elements
      for (SizeType i = 0; i < size_; i++) {
        table_[i].clear(i);
      }
    }
  }

  SizeType getSize() const {
    return size_;
  }

  void prefetch(Zobrist::Type hash) const {
    const ElementType* p = &table_[hash&mask_];
    const char* addr = reinterpret_cast<const char*>(p);
    memory::prefetch<sizeof(ElementType)>(addr);
  }

protected:

  ElementType& getElement(Zobrist::Type hash) {
    return table_[hash&mask_];
  }

  ElementType& getElement(SizeType index) {
    return table_[index];
  }

  const ElementType& getElement(Zobrist::Type hash) const {
    return table_[hash&mask_];
  }

  const ElementType& getElement(SizeType index) const {
    return table_[index];
  }

private:
  ElementType* table_;
  SizeType size_;
  SizeType mask_;

};

} // namespace sunfish

#endif // SUNFISH_SEACH_TABLE_HASHTABLE_HPP__
