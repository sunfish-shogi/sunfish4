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

template <size_t Mod, size_t CS, size_t S>
struct Padding {
  enum { Size = Padding<CS % (S + 1), CS, (S + 1)>::Size + 1 };
};

template <size_t CS, size_t S>
struct Padding<0, CS, S> {
  enum { Size = 0 };
};

/**
 * A base class for hash table
 */
template <class E> class HashTable {
public:

  using SizeType = uint32_t;

  static CONSTEXPR_CONST uintptr_t CacheLineSize = 64;
  static CONSTEXPR_CONST unsigned DefaultWidth = 18;

  struct Element : E {
    uint8_t padding[Padding<CacheLineSize % sizeof(E), CacheLineSize, sizeof(E)>::Size];
  };

  static_assert(CacheLineSize % sizeof(Element) == 0, "invalid element size");

public:

  HashTable(unsigned width = DefaultWidth) :
      table_(nullptr),
      size_(0) {
    resize(width);
  }
  HashTable(const HashTable&) = delete;
  HashTable(HashTable&&) = delete;

  ~HashTable() {
    delete [] p_;
  }

  HashTable& operator=(const HashTable&) = delete;
  HashTable& operator=(HashTable&&) = delete;

  void clear() {
    for (SizeType i = 0; i < size_; i++) {
      table_[i] = Element();
    }
  }

  void resize(unsigned width) {
    SizeType newSize = 1 << width;
    if (newSize == size_) {
      return;
    }

    size_ = newSize;
    mask_ = size_ - 1;
    if (table_ != nullptr) {
      delete[] p_;
    }
    p_ = new uint8_t[size_ * sizeof(Element) + CacheLineSize - 1];
    table_ = reinterpret_cast<Element*>(p_ + CacheLineSize - 1 - (reinterpret_cast<uintptr_t>(p_) - 1) % CacheLineSize);
    clear();
  }

  void resizeMB(unsigned mebiBytes) {
    unsigned width = 8;
    for (; width < 32; width++) {
      SizeType size = 1 << width;
      size_t sb = sizeof(Element) * size;
      if (sb > mebiBytes * 1024 * 1024) {
        break;
      }
    }
    resize(width - 1);
  }

  SizeType getSize() const {
    return size_;
  }

  void prefetch(Zobrist::Type hash) const {
    const Element* p = &table_[hash & mask_];
    const char* addr = reinterpret_cast<const char*>(p);
    memory::prefetch<sizeof(Element)>(addr);
  }

protected:

  Element& getElement(Zobrist::Type hash) {
    return table_[hash & mask_];
  }

  Element& getElement(SizeType index) {
    return table_[index];
  }

  const Element& getElement(Zobrist::Type hash) const {
    return table_[hash & mask_];
  }

  const Element& getElement(SizeType index) const {
    return table_[index];
  }

private:
  uint8_t* p_;
  Element* table_;
  SizeType size_;
  SizeType mask_;

};

} // namespace sunfish

#endif // SUNFISH_SEACH_TABLE_HASHTABLE_HPP__
