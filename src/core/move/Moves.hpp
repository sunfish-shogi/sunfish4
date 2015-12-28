/* Moves.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_CORE_MOVE_MOVES_HPP__
#define SUNFISH_CORE_MOVE_MOVES_HPP__

#include "common/Def.hpp"
#include "core/move/Move.hpp"
#include "core/position/Position.hpp"
#include <array>
#include <sstream>
#include <cstdint>

#define MAX_NUMBER_OF_MOVES 900

namespace sunfish {

class Position;

template <uint32_t capacity>
class MoveArray {
public:

  using InternalContainerType = std::array<Move, capacity>;
  using value_type = Move;
  using size_type = uint32_t;
  using difference_type = typename InternalContainerType::difference_type;
  using reference = typename InternalContainerType::reference;
  using const_reference = typename InternalContainerType::const_reference;
  using pointer = typename InternalContainerType::pointer;
  using const_pointer = typename InternalContainerType::const_pointer;
  using iterator = typename InternalContainerType::iterator;
  using const_iterator = typename InternalContainerType::const_iterator;

  /**
   * Default constructor
   */
  MoveArray() : size_(0) {
  }

  /**
   * Clear the contents.
   */
  void clear() {
    size_ = 0;
  }

  /**
   * Return the number of moves.
   */
  size_type size() const {
    return size_;
  }

  /**
   * Add a move to the end.
   */
  void add(const value_type& move) {
    moves_[size_++] = move;
  }

  /**
   * Remove specified move.
   */
  void remove(size_type index) {
    moves_[index] = moves_[--size_];
  }

  /**
   * Remove specified move.
   */
  iterator remove(iterator ite) {
    (*ite) = moves_[--size_];
    return ite;
  }

  /**
   * Remove specified move.
   */
  void removeStable(size_type index) {
    removeStable(begin()+index);
  }

  /**
   * Remove specified move.
   */
  iterator removeStable(iterator ite) {
    for (auto itmp = ite+1; itmp != end(); itmp++) {
      *(itmp-1) = *(itmp);
    }
    size_--;
    return ite;
  }

  /**
   * Remove specified move and moves after it.
   */
  void removeAfter(size_type index) {
    size_ = index;
  }

  /**
   * Remove specified move and moves after it.
   */
  void removeAfter(iterator ite) {
    size_ = (size_type)(ite - moves_.begin());
  }

  /**
   * Access specified move.
   */
  value_type& get(size_type index) {
    return moves_[index];
  }

  /**
   * Access specified move.
   */
  const value_type& get(size_type index) const {
    return moves_[index];
  }

  /**
   * Access specified move.
   */
  value_type& operator[](size_type index) {
    return moves_[index];
  }

  /**
   * Access specified move.
   */
  const value_type& operator[](size_type index) const {
    return moves_[index];
  }

  /**
   * Return an iterator to the begining.
   */
  iterator begin() {
    return moves_.begin();
  }

  /**
   * Return an iterator to the begining.
   */
  const_iterator begin() const {
    return moves_.cbegin();
  }

  /**
   * Return an iterator to the begining.
   */
  const_iterator cbegin() const {
    return moves_.cbegin();
  }

  /**
   * Return an iterator to the end.
   */
  iterator end() {
    return moves_.begin() + size_;
  }

  /**
   * Return an iterator to the end.
   */
  const_iterator end() const {
    return moves_.cbegin() + size_;
  }

  /**
   * Return an iterator to the end.
   */
  const_iterator cend() const {
    return moves_.cbegin() + size_;
  }

  std::string toString() const {
    std::ostringstream oss;
    for (auto ite = cbegin(); ite != cend(); ite++) {
      if (ite != cbegin()) {
        oss << ' ';
      }
      oss << ite->toString();
    }
    return oss.str();
  }

  std::string toString(Position& position) const {
    std::ostringstream oss;
    for (auto ite = cbegin(); ite != cend(); ite++) {
      if (ite != cbegin()) {
        oss << ' ';
      }
      oss << ite->toString(position);
    }
    return oss.str();
  }

private:

  size_type size_;
  InternalContainerType moves_;

};

using Moves = MoveArray<MAX_NUMBER_OF_MOVES>;

inline void remove(Moves& moves,
                   Moves::iterator begin,
                   const Move& move) {
  for (auto ite = begin; ite != moves.end();) {
    if (*ite == move) {
      ite = moves.remove(ite);
    } else {
      ite++;
    }
  }
}

template <class T>
inline void remove(Moves& moves,
                   Moves::iterator begin,
                   T&& f) {
  for (auto ite = begin; ite != moves.end();) {
    if (f(*ite)) {
      ite = moves.remove(ite);
    } else {
      ite++;
    }
  }
}

void sortMovesForDebug(Moves& moves, const Position& position);

} // namespace sunfish

#endif // SUNFISH_CORE_MOVE_MOVES_HPP__
