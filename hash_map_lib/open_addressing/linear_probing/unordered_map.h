#ifndef STROUPO_HASH_MAP_LIB_OPEN_ADDRESSING_LINEAR_PROBING_UNORDERED_MAP_H_
#define STROUPO_HASH_MAP_LIB_OPEN_ADDRESSING_LINEAR_PROBING_UNORDERED_MAP_H_

#include <memory>
#include <utility>
#include <vector>

#include "../detail/iterator.h"
#include "../detail/node.h"
// #include "../detail/unordered_map_base.h"

namespace stroupo::hash_map_lib::open_addressing {
namespace linear_probing {

template <typename Key, typename T, typename Hash = std::hash<Key>,
          typename Key_equal = std::equal_to<Key>,
          typename Allocator = std::allocator<std::pair<const Key, T>>>
class unordered_map {
 public:
  // Non-standard Member Types
  using real_type = float;
  using node_type = detail::node<Key, T>;
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

 public:
  // Constructors, Destructors and Assignments
  unordered_map();
  unordered_map(std::initializer_list<value_type> list);

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

  // Modifiers
  void insert(const value_type& value);
  template <typename Iterator>
  void insert(Iterator first, Iterator last);
  template <typename Key_iterator, typename T_iterator>
  void insert(Key_iterator keys_first, Key_iterator keys_last,
              T_iterator first);
  auto erase(const key_type& key);

  // Lookup
  mapped_type& operator[](const key_type& key);
  mapped_type& at(const key_type& key);
  const mapped_type& at(const key_type& key) const;
  iterator find(const key_type& key);
  const_iterator find(const key_type& key) const;

  // Hash Policy
  auto load_factor() const;
  auto max_load_factor() const { return max_load_factor_; }
  void max_load_factor(real_type ml) { max_load_factor_ = ml; }
  void rehash(size_type count);
  void reserve(size_type count);

 private:
  // Internal Member Functions
  auto node_index(const key_type& key) const;
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
unordered_map<Key, T, Hash, Key_equal, Allocator>::unordered_map() : table_(2) {
  table_.reserve(3);
  table_[2].empty = false;
}

template <typename Key, typename T, typename Hash, typename Key_equal,
          typename Allocator>
unordered_map<Key, T, Hash, Key_equal, Allocator>::unordered_map(
    std::initializer_list<value_type> list)
    : unordered_map{} {
  reserve(list.size());
  for (const auto& e : list) {
    const auto index = node_index(e.first);
    if (table_[index].empty) ++load_;
    table_[index] = {e.first, e.second};
  }
}

template <typename Key, typename T, typename Hash, typename Key_equal,
          typename Allocator>
auto unordered_map<Key, T, Hash, Key_equal, Allocator>::load_factor() const {
  return static_cast<real_type>(load_) / table_.size();
}

template <typename Key, typename T, typename Hash, typename Key_equal,
          typename Allocator>
auto unordered_map<Key, T, Hash, Key_equal, Allocator>::first_node() const
    -> const node_type* {
  auto p = &table_[0];
  while (p->empty) ++p;
  return p;
}

template <typename Key, typename T, typename Hash, typename Key_equal,
          typename Allocator>
auto unordered_map<Key, T, Hash, Key_equal, Allocator>::last_node() const
    -> const node_type* {
  return &table_[table_.size()];
}

template <typename Key, typename T, typename Hash, typename Key_equal,
          typename Allocator>
auto unordered_map<Key, T, Hash, Key_equal, Allocator>::begin() noexcept {
  return iterator{const_cast<node_type*>(first_node())};
}

template <typename Key, typename T, typename Hash, typename Key_equal,
          typename Allocator>
auto unordered_map<Key, T, Hash, Key_equal, Allocator>::begin() const noexcept {
  return const_iterator{first_node()};
}

template <typename Key, typename T, typename Hash, typename Key_equal,
          typename Allocator>
auto unordered_map<Key, T, Hash, Key_equal, Allocator>::end() noexcept {
  return iterator{const_cast<node_type*>(last_node())};
}

template <typename Key, typename T, typename Hash, typename Key_equal,
          typename Allocator>
auto unordered_map<Key, T, Hash, Key_equal, Allocator>::end() const noexcept {
  return const_iterator{last_node()};
}

template <typename Key, typename T, typename Hash, typename Key_equal,
          typename Allocator>
auto unordered_map<Key, T, Hash, Key_equal, Allocator>::node_index(
    const key_type& key) const {
  hasher hash{};
  key_equal equal{};
  size_type index = hash(key) % table_.size();
  while (!table_[index].empty && !equal(key, table_[index].key))
    index = (index + 1) % table_.size();
  return index;
}

template <typename Key, typename T, typename Hash, typename Key_equal,
          typename Allocator>
void unordered_map<Key, T, Hash, Key_equal, Allocator>::rehash(
    size_type count) {
  container old_data(count);
  old_data.reserve(count + 1);
  old_data[count].empty = false;
  table_.swap(old_data);
  for (auto e : old_data)
    if (!e.empty) table_[node_index(e.key)] = e;
}

template <typename Key, typename T, typename Hash, typename Key_equal,
          typename Allocator>
void unordered_map<Key, T, Hash, Key_equal, Allocator>::reserve(
    size_type count) {
  rehash(std::ceil(count / max_load_factor()));
}

template <typename Key, typename T, typename Hash, typename Key_equal,
          typename Allocator>
void unordered_map<Key, T, Hash, Key_equal, Allocator>::insert(
    const value_type& value) {
  const auto index = node_index(value.first);
  if (table_[index].empty) ++load_;
  table_[index] = {value};
  if (static_cast<real_type>(load_) / table_.size() >= max_load_factor())
    rehash(2 * table_.size());
}

template <typename Key, typename T, typename Hash, typename Key_equal,
          typename Allocator>
auto unordered_map<Key, T, Hash, Key_equal, Allocator>::operator[](
    const key_type& key) -> mapped_type& {
  const auto index = node_index(key);
  if (table_[index].empty) {
    ++load_;
    table_[index] = {key, mapped_type{}};
    if (load_ >= table_.size() * max_load_factor()) {
      rehash(2 * table_.size());
      // index was invalidated through rehash
      return table_[node_index(key)].value;
    }
  }
  return table_[index].value;
}

template <typename Key, typename T, typename Hash, typename Key_equal,
          typename Allocator>
auto unordered_map<Key, T, Hash, Key_equal, Allocator>::at(
    const key_type& key) const -> const mapped_type& {
  const auto index = node_index(key);
  if (table_[index].empty)
    throw std::out_of_range{"The given key was not inserted!"};
  return table_[index].value;
}

template <typename Key, typename T, typename Hash, typename Key_equal,
          typename Allocator>
auto unordered_map<Key, T, Hash, Key_equal, Allocator>::at(const key_type& key)
    -> mapped_type& {
  return const_cast<mapped_type&>(
      const_cast<const unordered_map*>(this)->at(key));
}

template <typename Key, typename T, typename Hash, typename Key_equal,
          typename Allocator>
template <typename Iterator>
void unordered_map<Key, T, Hash, Key_equal, Allocator>::insert(Iterator first,
                                                               Iterator last) {
  for (auto it = first; it != last; ++it) (*this)[it->first] = it->second;
}

template <typename Key, typename T, typename Hash, typename Key_equal,
          typename Allocator>
template <typename Key_iterator, typename T_iterator>
void unordered_map<Key, T, Hash, Key_equal, Allocator>::insert(
    Key_iterator keys_first, Key_iterator keys_last, T_iterator first) {
  auto it = first;
  for (auto key_it = keys_first; key_it != keys_last; ++key_it, ++it)
    (*this)[*key_it] = *it;
}

template <typename Key, typename T, typename Hash, typename Key_equal,
          typename Allocator>
auto unordered_map<Key, T, Hash, Key_equal, Allocator>::find(
    const key_type& key) -> iterator {
  const auto index = node_index(key);
  if (table_[index].empty) return end();
  return &table_[index];
}

template <typename Key, typename T, typename Hash, typename Key_equal,
          typename Allocator>
auto unordered_map<Key, T, Hash, Key_equal, Allocator>::erase(
    const key_type& key) {
  const auto index = node_index(key);
  if (table_[index].empty) return 0;
  table_.erase(table_.begin() + index);
  return 1;
}

template <typename Key, typename T, typename Hash, typename Key_equal,
          typename Allocator>
auto unordered_map<Key, T, Hash, Key_equal, Allocator>::find(
    const key_type& key) const -> const_iterator {
  const auto index = node_index(key);
  if (table_[index].empty) return end();
  return &table_[index];
}

}  // namespace linear_probing
}  // namespace stroupo::hash_map_lib::open_addressing

#endif  // STROUPO_HASH_MAP_LIB_OPEN_ADDRESSING_LINEAR_PROBING_UNORDERED_MAP_H_