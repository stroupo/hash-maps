#ifndef STROUPO_HASH_MAP_LIB_OPEN_ADDRESSING_DETAIL_UNORDERED_MAP_BASE_H_
#define STROUPO_HASH_MAP_LIB_OPEN_ADDRESSING_DETAIL_UNORDERED_MAP_BASE_H_

#include "iterator.h"
#include "node.h"

namespace stroupo::hash_map_lib::open_addressing {
namespace detail {

template <typename Key, typename T, typename Hash = std::hash<Key>,
          typename Key_equal = std::equal_to<Key>,
          typename Allocator = std::allocator<std::pair<const Key, T>>>
class unordered_map_base {
 public:
  // Non-standard Member Types
  using real_type = float;
  using node_type = node<Key, T>;
  using container = std::vector<node_type>;
  // Standard Member Types
  using key_type = Key;
  using mapped_type = T;
  using value_type = std::pair<const Key, T>;
  using size_type = typename container::size_type;
  using difference_type = typename container::difference_type;
  using hasher = Hash;
  using key_equal = Key_equal;
  using allocator_type = Allocator;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = typename std::allocator_traits<allocator_type>::pointer;
  using const_pointer =
      typename std::allocator_traits<allocator_type>::const_pointer;
  using iterator = detail::iterator<key_type, mapped_type, false>;
  using const_iterator = detail::iterator<key_type, mapped_type, true>;

  // Capacity
  bool empty() const { return load_ == 0; }
  size_type size() const { return load_; }
  size_type capacity() const { return table_.size(); }
  // static size_type max_size() noexcept { return container::max_size(); }

  // Iterators
  auto begin() noexcept;
  auto begin() const noexcept;
  auto end() noexcept;
  auto end() const noexcept;

  // Hash Policy
  auto load_factor() const;
  auto max_load_factor() const { return max_load_factor_; }
  void max_load_factor(real_type ml) { max_load_factor_ = ml; }

 private:
  // Internal Member Functions
  const node_type* first_node() const;
  const node_type* last_node() const;

 private:
  // Internal Member Variables
  real_type max_load_factor_{0.5};
  size_type load_{0};
  container table_;
};

template <typename Key, typename T, typename Hash, typename Key_equal,
          typename Allocator>
auto unordered_map_base<Key, T, Hash, Key_equal, Allocator>::load_factor()
    const {
  return static_cast<real_type>(load_) / table_.size();
}

template <typename Key, typename T, typename Hash, typename Key_equal,
          typename Allocator>
auto unordered_map_base<Key, T, Hash, Key_equal, Allocator>::first_node() const
    -> const node_type* {
  auto p = &table_[0];
  while (p->empty) ++p;
  return p;
}

template <typename Key, typename T, typename Hash, typename Key_equal,
          typename Allocator>
auto unordered_map_base<Key, T, Hash, Key_equal, Allocator>::last_node() const
    -> const node_type* {
  return &table_[table_.size()];
}

template <typename Key, typename T, typename Hash, typename Key_equal,
          typename Allocator>
auto unordered_map_base<Key, T, Hash, Key_equal, Allocator>::begin() noexcept {
  return iterator{const_cast<node_type*>(first_node())};
}

template <typename Key, typename T, typename Hash, typename Key_equal,
          typename Allocator>
auto unordered_map_base<Key, T, Hash, Key_equal, Allocator>::begin() const
    noexcept {
  return const_iterator{first_node()};
}

template <typename Key, typename T, typename Hash, typename Key_equal,
          typename Allocator>
auto unordered_map_base<Key, T, Hash, Key_equal, Allocator>::end() noexcept {
  return iterator{const_cast<node_type*>(last_node())};
}

template <typename Key, typename T, typename Hash, typename Key_equal,
          typename Allocator>
auto unordered_map_base<Key, T, Hash, Key_equal, Allocator>::end() const
    noexcept {
  return const_iterator{last_node()};
}

}  // namespace detail
}  // namespace stroupo::hash_map_lib::open_addressing

#endif  // STROUPO_HASH_MAP_LIB_OPEN_ADDRESSING_DETAIL_UNORDERED_MAP_BASE_H_