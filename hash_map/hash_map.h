#ifndef STROUPO_HASH_MAP_H_
#define STROUPO_HASH_MAP_H_

#include <cmath>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

namespace stroupo {

template <typename Key, typename T, typename Hash = std::hash<Key>,
          typename Key_equal = std::equal_to<Key>,
          typename Allocator = std::allocator<std::pair<const Key, T>>>
class hash_map {
  // Internal Member Types
  struct node;
  template <bool Constant>
  class iterator_t;

 public:
  // Non-standard Member Types
  using container = std::vector<node>;
  using real_type = float;
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
  using iterator = iterator_t<false>;
  using const_iterator = iterator_t<true>;

 public:
  // Constructors, Destructors and Assignments
  hash_map();
  hash_map(std::initializer_list<value_type> list);

  // Capacity
  bool empty() const { return load_ == 0; }
  size_type size() const { return load_; }
  size_type capacity() const { return table_.size(); }
  static size_type max_size() noexcept { return container::max_size(); }

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
  const node* first_node() const;
  const node* last_node() const;

 private:
  // Internal Member Variables
  real_type max_load_factor_{0.7};
  size_type load_{0};
  container table_;
};

template <typename Key, typename T, typename Hash, typename Key_equal,
          typename Allocator>
struct hash_map<Key, T, Hash, Key_equal, Allocator>::node {
  node() = default;
  node(const key_type& k, const mapped_type& v)
      : key{k}, value{v}, empty{false} {}
  node(const std::pair<key_type, mapped_type>& v) : node{v.first, v.second} {}

  // Member Variables
  // The order should no be changed.
  // It is used for an reinterpret_cast to value_type.
  key_type key{};
  mapped_type value{};
  bool empty{true};
};

template <typename Key, typename T, typename Hash, typename Key_equal,
          typename Allocator>
template <bool Constant>
class hash_map<Key, T, Hash, Key_equal, Allocator>::iterator_t {
 public:
  // Standard Member Types
  using iterator_category = std::forward_iterator_tag;
  using value_type = hash_map::value_type;
  using difference_type = hash_map::difference_type;
  using reference =
      std::conditional_t<Constant, const value_type&, value_type&>;
  using pointer = std::conditional_t<Constant, const value_type*, value_type*>;
  // Non-standard Member Types
  using node_pointer = std::conditional_t<Constant, const node*, node*>;

  // Constructors, Destructors and Assignments
  // iterator_t() = default; // There should be no default constructor.
  iterator_t(node_pointer n) : node_{n} {}
  iterator_t(const iterator_t& it) = default;
  iterator_t& operator=(const iterator_t& it) = default;
  iterator_t(iterator_t&& it) = default;
  iterator_t& operator=(iterator_t&& it) = default;
  ~iterator_t() = default;

  // Member Functions
  iterator_t& operator++();
  iterator_t operator++(int);
  reference operator*() const { return *reinterpret_cast<pointer>(node_); }
  pointer operator->() const { return reinterpret_cast<pointer>(node_); }
  bool operator==(iterator_t it) const { return node_ == it.node_; }
  bool operator!=(iterator_t it) const { return !(*this == it); }

 private:
  // Internal Member Variables
  node_pointer node_;
};

template <typename Key, typename T, typename Hash, typename Key_equal,
          typename Allocator>
template <bool Constant>
auto hash_map<Key, T, Hash, Key_equal, Allocator>::iterator_t<Constant>::
operator++() -> iterator_t& {
  while ((++node_)->empty)
    ;
  return *this;
}

template <typename Key, typename T, typename Hash, typename Key_equal,
          typename Allocator>
template <bool Constant>
auto hash_map<Key, T, Hash, Key_equal, Allocator>::iterator_t<Constant>::
operator++(int n) -> iterator_t {
  auto ip = *this;
  ++(*this);
  return ip;
}

template <typename Key, typename T, typename Hash, typename Key_equal,
          typename Allocator>
hash_map<Key, T, Hash, Key_equal, Allocator>::hash_map() : table_(2) {
  table_.reserve(3);
  table_[2].empty = false;
}

template <typename Key, typename T, typename Hash, typename Key_equal,
          typename Allocator>
hash_map<Key, T, Hash, Key_equal, Allocator>::hash_map(
    std::initializer_list<value_type> list)
    : hash_map{} {
  reserve(list.size());
  for (const auto& e : list) {
    const auto index = node_index(e.first);
    if (table_[index].empty) ++load_;
    table_[index] = {e.first, e.second};
  }
}

template <typename Key, typename T, typename Hash, typename Key_equal,
          typename Allocator>
auto hash_map<Key, T, Hash, Key_equal, Allocator>::load_factor() const {
  return static_cast<real_type>(load_) / table_.size();
}

template <typename Key, typename T, typename Hash, typename Key_equal,
          typename Allocator>
auto hash_map<Key, T, Hash, Key_equal, Allocator>::first_node() const
    -> const node* {
  auto p = &table_[0];
  while (p->empty) ++p;
  return p;
}

template <typename Key, typename T, typename Hash, typename Key_equal,
          typename Allocator>
auto hash_map<Key, T, Hash, Key_equal, Allocator>::last_node() const
    -> const node* {
  return &table_[table_.size()];
}

template <typename Key, typename T, typename Hash, typename Key_equal,
          typename Allocator>
auto hash_map<Key, T, Hash, Key_equal, Allocator>::begin() noexcept {
  return iterator{const_cast<node*>(first_node())};
}

template <typename Key, typename T, typename Hash, typename Key_equal,
          typename Allocator>
auto hash_map<Key, T, Hash, Key_equal, Allocator>::begin() const noexcept {
  return const_iterator{first_node()};
}

template <typename Key, typename T, typename Hash, typename Key_equal,
          typename Allocator>
auto hash_map<Key, T, Hash, Key_equal, Allocator>::end() noexcept {
  return iterator{const_cast<node*>(last_node())};
}

template <typename Key, typename T, typename Hash, typename Key_equal,
          typename Allocator>
auto hash_map<Key, T, Hash, Key_equal, Allocator>::end() const noexcept {
  return const_iterator{last_node()};
}

template <typename Key, typename T, typename Hash, typename Key_equal,
          typename Allocator>
auto hash_map<Key, T, Hash, Key_equal, Allocator>::node_index(
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
void hash_map<Key, T, Hash, Key_equal, Allocator>::rehash(size_type count) {
  container old_data(count);
  old_data.reserve(count + 1);
  old_data[count].empty = false;
  table_.swap(old_data);
  for (auto e : old_data)
    if (!e.empty) table_[node_index(e.key)] = e;
}

template <typename Key, typename T, typename Hash, typename Key_equal,
          typename Allocator>
void hash_map<Key, T, Hash, Key_equal, Allocator>::reserve(size_type count) {
  rehash(std::ceil(count / max_load_factor()));
}

template <typename Key, typename T, typename Hash, typename Key_equal,
          typename Allocator>
void hash_map<Key, T, Hash, Key_equal, Allocator>::insert(
    const value_type& value) {
  const auto index = node_index(value.first);
  if (table_[index].empty) ++load_;
  table_[index] = {value};
  if (static_cast<real_type>(load_) / table_.size() >= max_load_factor())
    rehash(2 * table_.size());
}

template <typename Key, typename T, typename Hash, typename Key_equal,
          typename Allocator>
auto hash_map<Key, T, Hash, Key_equal, Allocator>::operator[](
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
auto hash_map<Key, T, Hash, Key_equal, Allocator>::at(const key_type& key) const
    -> const mapped_type& {
  const auto index = node_index(key);
  if (table_[index].empty)
    throw std::out_of_range{"The given key was not inserted!"};
  return table_[index].value;
}

template <typename Key, typename T, typename Hash, typename Key_equal,
          typename Allocator>
auto hash_map<Key, T, Hash, Key_equal, Allocator>::at(const key_type& key)
    -> mapped_type& {
  return const_cast<mapped_type&>(const_cast<const hash_map*>(this)->at(key));
}

template <typename Key, typename T, typename Hash, typename Key_equal,
          typename Allocator>
template <typename Iterator>
void hash_map<Key, T, Hash, Key_equal, Allocator>::insert(Iterator first,
                                                          Iterator last) {
  for (auto it = first; it != last; ++it) (*this)[it->first] = it->second;
}

template <typename Key, typename T, typename Hash, typename Key_equal,
          typename Allocator>
template <typename Key_iterator, typename T_iterator>
void hash_map<Key, T, Hash, Key_equal, Allocator>::insert(
    Key_iterator keys_first, Key_iterator keys_last, T_iterator first) {
  auto it = first;
  for (auto key_it = keys_first; key_it != keys_last; ++key_it, ++it)
    (*this)[*key_it] = *it;
}

template <typename Key, typename T, typename Hash, typename Key_equal,
          typename Allocator>
auto hash_map<Key, T, Hash, Key_equal, Allocator>::find(const key_type& key)
    -> iterator {
  const auto index = node_index(key);
  if (table_[index].empty) return end();
  return &table_[index];
}

template <typename Key, typename T, typename Hash, typename Key_equal,
          typename Allocator>
auto hash_map<Key, T, Hash, Key_equal, Allocator>::find(
    const key_type& key) const -> const_iterator {
  const auto index = node_index(key);
  if (table_[index].empty) return end();
  return &table_[index];
}

}  // namespace stroupo

#endif  // STROUPO_HASH_MAP_H_
