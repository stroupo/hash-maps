#ifndef STROUPO_HASH_MAP_H_
#define STROUPO_HASH_MAP_H_

#include <cmath>
#include <functional>
#include <stdexcept>
#include <utility>
#include <vector>

namespace stroupo {

template <typename Key, typename T, typename Hash = std::hash<Key>,
          typename Key_equal = std::equal_to<Key>,
          typename Allocator = std::allocator<std::pair<const Key, T>>>
class hash_map {
 public:
  using key_type = Key;
  using mapped_type = T;
  using value_type = std::pair<const Key, T>;
  using hasher = Hash;
  using key_equal = Key_equal;
  using allocator_type = Allocator;

  struct entry {
    entry() = default;
    entry(const key_type& k, const mapped_type& v)
        : key{k}, value{v}, empty{false} {}
    entry(const std::pair<key_type, mapped_type>& v)
        : entry{v.first, v.second} {}

    key_type key{};
    mapped_type value{};
    bool empty{true};
  };

  using container = std::vector<entry>;
  using size_type = typename container::size_type;
  using real_type = float;

  hash_map() : data(2) {}

  real_type max_load_factor() const { return max_load_factor_; }
  void max_load_factor(real_type ml) { max_load_factor_ = ml; }
  real_type load_factor() const {
    return static_cast<real_type>(load_) / data.size();
  }
  bool empty() const { return load_ == 0; }
  size_type size() const { return load_; }
  size_type capacity() const { return data.size(); }
  static size_type max_size() noexcept { return container::max_size(); }

  void insert(const value_type& value);
  mapped_type& operator[](const key_type& key);
  const mapped_type& at(const key_type& key) const;
  mapped_type& at(const key_type& key) {
    return const_cast<mapped_type&>(const_cast<const hash_map*>(this)->at(key));
  }
  auto node_index(const key_type& key) const;

  void rehash(size_type count);
  void reserve(size_type count);

  container data;
  size_type load_{0};
  real_type max_load_factor_{0.7};
};

template <typename Key, typename T, typename Hash, typename Key_equal,
          typename Allocator>
auto hash_map<Key, T, Hash, Key_equal, Allocator>::node_index(
    const key_type& key) const {
  hasher hash{};
  size_type index = hash(key) % data.size();
  while (!data[index].empty && key != data[index].key)
    index = (index + 1) % data.size();
  return index;
}

template <typename Key, typename T, typename Hash, typename Key_equal,
          typename Allocator>
void hash_map<Key, T, Hash, Key_equal, Allocator>::rehash(size_type count) {
  container old_data(count);
  data.swap(old_data);
  for (auto e : old_data)
    if (!e.empty) data[node_index(e.key)] = e;
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
  if (data[index].empty) ++load_;
  data[index] = {value};
  if (static_cast<real_type>(load_) / data.size() >= max_load_factor())
    rehash(2 * data.size());
}

template <typename Key, typename T, typename Hash, typename Key_equal,
          typename Allocator>
auto hash_map<Key, T, Hash, Key_equal, Allocator>::operator[](
    const key_type& key) -> mapped_type& {
  const auto index = node_index(key);
  if (data[index].empty) {
    ++load_;
    data[index] = {key, mapped_type{}};
    if (load_ >= data.size() * max_load_factor()) rehash(2 * data.size());
  }
  return data[index].value;
}

template <typename Key, typename T, typename Hash, typename Key_equal,
          typename Allocator>
auto hash_map<Key, T, Hash, Key_equal, Allocator>::at(const key_type& key) const
    -> const mapped_type& {
  const auto index = node_index(key);
  if (data[index].empty)
    throw std::out_of_range{"The given key was not inserted!"};
  return data[index].value;
}

}  // namespace stroupo

#endif  // STROUPO_HASH_MAP_H_