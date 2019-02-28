#ifndef STROUPO_HASH_MAP_LIB_OPEN_ADDRESSING_DETAIL_NODE_H_
#define STROUPO_HASH_MAP_LIB_OPEN_ADDRESSING_DETAIL_NODE_H_

#include <utility>

namespace stroupo::hash_map_lib::open_addressing {
namespace detail {

template <typename Key, typename T>
struct node {
  using key_type = Key;
  using mapped_type = T;

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

}  // namespace detail
}  // namespace stroupo::hash_map_lib::open_addressing

#endif  // STROUPO_HASH_MAP_LIB_OPEN_ADDRESSING_DETAIL_NODE_H_