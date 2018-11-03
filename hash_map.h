#ifndef STROUPO_HASH_MAP_H_
#define STROUPO_HASH_MAP_H_

#include <functional>
#include <stdexcept>
#include <utility>
#include <vector>

namespace stroupo {

template <typename Key, typename T, typename Hash = std::hash<Key>>
class hash_map {
 public:
  using key_type = Key;
  using mapped_type = T;
  using value_type = std::pair<const Key, T>;
  using hasher = Hash;

  struct entry {
    entry() = default;
    entry(const key_type& k, const mapped_type& v)
        : key{k}, value{v}, empty{false} {}
    entry(const std::pair<key_type, mapped_type>& v)
        : entry{v.first, v.second} {}

    int key{0};
    int value{0};
    bool empty{true};
  };

  hash_map() : data(2), load{0} {}

  constexpr static float max_load_factor() { return 0.7; }

  void resize(std::size_t new_size) {
    std::vector<entry> new_data(new_size);
    hasher hash{};
    for (auto e : data) {
      if (e.empty) continue;
      std::size_t index = hash(e.key) % new_data.size();
      while (!new_data[index].empty && e.key != new_data[index].key)
        index = (index + 1) % new_data.size();
      new_data[index] = e;
    }
    data.swap(new_data);
  }

  void insert(const value_type& value) {
    hasher hash{};
    std::size_t index = hash(value.first) % data.size();
    while (!data[index].empty && value.first != data[index].key)
      index = (index + 1) % data.size();
    if (data[index].empty) ++load;
    data[index] = {value};  // use implicit conversion
    if (static_cast<float>(load) / data.size() >= max_load_factor())
      resize(2 * data.size());
  }

  mapped_type& operator[](const key_type& key) {
    hasher hash{};
    std::size_t index = hash(key) % data.size();
    while (!data[index].empty && key != data[index].key)
      index = (index + 1) % data.size();
    if (data[index].empty) throw std::runtime_error{"no key!"};
    return data[index].value;
  }

  std::vector<entry> data;
  std::size_t load;
};

}  // namespace stroupo

#endif  // STROUPO_HASH_MAP_H_