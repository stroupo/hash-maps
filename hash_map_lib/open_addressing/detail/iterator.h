#ifndef STROUPO_HASH_MAP_LIB_OPEN_ADDRESSING_DETAIL_ITERATOR_H_
#define STROUPO_HASH_MAP_LIB_OPEN_ADDRESSING_DETAIL_ITERATOR_H_

#include <functional>
#include <iterator>
#include <type_traits>
#include <utility>

#include "node.h"

namespace stroupo::hash_map_lib::open_addressing {
namespace detail {

template <typename Key, typename T, bool Constant>
class iterator {
 public:
  // Standard Member Types
  using iterator_category = std::forward_iterator_tag;
  using value_type = std::pair<const Key, T>;
  using difference_type = long;
  using reference =
      std::conditional_t<Constant, const value_type&, value_type&>;
  using pointer = std::conditional_t<Constant, const value_type*, value_type*>;
  // Non-standard Member Types
  using node_type = node<Key, T>;
  using node_pointer =
      std::conditional_t<Constant, const node_type*, node_type*>;

  // Constructors, Destructors and Assignments
  iterator() = delete;  // There should be no default constructor.
  iterator(node_pointer n) : node_{n} {}
  iterator(const iterator& it) = default;
  iterator& operator=(const iterator& it) = default;
  iterator(iterator&& it) = default;
  iterator& operator=(iterator&& it) = default;
  ~iterator() = default;

  // Member Functions
  iterator& operator++();
  iterator operator++(int);
  reference operator*() const { return *reinterpret_cast<pointer>(node_); }
  pointer operator->() const { return reinterpret_cast<pointer>(node_); }
  bool operator==(iterator it) const { return node_ == it.node_; }
  bool operator!=(iterator it) const { return !(*this == it); }

 private:
  // Internal Member Variables
  node_pointer node_;
};

template <typename Key, typename T, bool Constant>
auto iterator<Key, T, Constant>::operator++() -> iterator& {
  while ((++node_)->empty)
    ;
  return *this;
}

template <typename Key, typename T, bool Constant>
auto iterator<Key, T, Constant>::operator++(int n) -> iterator {
  auto ip = *this;
  ++(*this);
  return ip;
}

}  // namespace detail
}  // namespace stroupo::hash_map_lib::open_addressing

#endif  // STROUPO_HASH_MAP_LIB_OPEN_ADDRESSING_DETAIL_ITERATOR_H_