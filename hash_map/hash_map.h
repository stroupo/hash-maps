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

    key_type key{0};
    mapped_type value{0};
    bool empty{true};
  };

  using container = std::vector<entry>;
  using size_type = typename container::size_type;

  hash_map() : data(2) {}

  // CAPACITY
  constexpr static float max_load_factor() { return 0.7; }
  bool empty() const { return load == (size_type)0; }
  size_type size() const { return load; }
  size_type max_size() const { return container::max_size(); }

  // MODIFIERS
  void insert(const value_type& value) {
    const auto index = node_index(value.first);
    if (data[index].empty) ++load;
    data[index] = {value};  // use implicit conversion
    if (static_cast<float>(load) / data.size() >= max_load_factor())
      resize(2 * data.size());
  }
  // LOOKUP
  mapped_type& operator[](const key_type& key) {
    const auto index = node_index(key);
    if (data[index].empty) // Stop wenn leer
      throw std::runtime_error{"This key was not inserted!"};
    return data[index].value;
  }
  // Vergleich der Abstände
  bool probelength (const size_type& current_index, const key_type& key) const noexcept {
    hasher hash{}
    const int current_length = current_index - (hash(data[current_index].key) % data.size()) // Abstand des gespeicherten Values zu einen ursprünglichen Hash-Wert
    size_type length = current_index - (hash(key) % data.size())
    if (current_length < length)
    return true;
  }

  size_type robin_hood(const key_type& key) const noexcept {
    hasher hash{};
    value_type current_value;
    size_type index = hash(key) % data.size();
    while(!data[index].empty){
      if (probelength(data[index], key)==false && key != data[index].key)){
        index = (index + 1) % data.size();
      }
      else {
        data.[index].swap(current_value);
        return index
        // Problem: Speichern von current_value in nächstem geeigneten
      }
    }

    return index;


  }

  size_type node_index(const key_type& key) const noexcept {
    hasher hash{};
    size_type index = hash(key) % data.size();
    while (!data[index].empty && key != data[index].key)
      index = (index + 1) % data.size();
    return index;
  }

  void resize(size_type new_size) {
    std::vector<entry> old_data(new_size);
    data.swap(old_data);
    for (auto e : old_data) {
      if (e.empty) continue;
      const auto index = node_index(e.key);
      data[index] = e;
    }
  }

  std::vector<entry> data;
  size_type load{0};
};

}  // namespace stroupo

#endif  // STROUPO_HASH_MAP_H_
