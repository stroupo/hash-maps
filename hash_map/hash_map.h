#ifndef STROUPO_HASH_MAP_H_
#define STROUPO_HASH_MAP_H_

#include <cmath>
#include <functional>
#include <iterator>
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
  // using value_type = std::pair<Key, T>;
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
  using difference_type = std::ptrdiff_t;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = typename std::allocator_traits<allocator_type>::pointer;
  using const_pointer =
      typename std::allocator_traits<allocator_type>::const_pointer;
  using real_type = float;
  // using const_iterator = ;

  struct iterator {
    using value_type = hash_map::value_type;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using pointer = value_type*;
    using iterator_category = std::forward_iterator_tag;

    iterator& operator++() {
      while ((++pointer_)->empty) {
      }
      return *this;
    }

    // iterator& operator++(int x) {}

    auto operator*() { return *reinterpret_cast<value_type*>(pointer_); }
    auto operator-> () { return reinterpret_cast<value_type*>(pointer_); }

    bool operator==(iterator it) const { return pointer_ == it.pointer_; }
    bool operator!=(iterator it) const { return !(*this == it); }

    entry* pointer_;
  };

  hash_map() : data(2) {
    data.reserve(3);
    data[2].empty = false;
  }

  real_type max_load_factor() const { return max_load_factor_; }
  void max_load_factor(real_type ml) { max_load_factor_ = ml; }
  real_type load_factor() const {
    return static_cast<real_type>(load_) / data.size();
  }
  bool empty() const { return load_ == 0; }
  size_type size() const { return load_; }
  size_type capacity() const { return data.size(); }
  static size_type max_size() noexcept { return container::max_size(); }

  auto begin() {
    return (data[0].empty) ? (++iterator{&data[0]}) : (iterator{&data[0]});
  }
  auto end() { return iterator{&data[data.size()]}; }

  void insert(const value_type& value);
  template <typename Iterator>
  void insert(Iterator first, Iterator last);
  template <typename Key_iterator, typename T_iterator>
  void insert(Key_iterator keys_first, Key_iterator keys_last,
              T_iterator first);

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
};  // namespace stroupo

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
  old_data.reserve(count + 1);
  old_data[count].empty = false;
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
    if (load_ >= data.size() * max_load_factor()) {
      rehash(2 * data.size());
      // index was invalidated through rehash
      return data[node_index(key)].value;
    }
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

}  // namespace stroupo

#endif  // STROUPO_HASH_MAP_H_