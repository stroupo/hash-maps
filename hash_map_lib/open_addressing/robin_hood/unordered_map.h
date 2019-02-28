#ifndef STROUPO_HASH_MAP_LIB_OPEN_ADDRESSING_ROBIN_HOOD_UNORDERED_MAP_H_
#define STROUPO_HASH_MAP_LIB_OPEN_ADDRESSING_ROBIN_HOOD_UNORDERED_MAP_H_

#include "../detail/unordered_map_base.h"

namespace stroupo::hash_map_lib::open_addressing {
namespace robin_hood {

template <typename Key, typename T, typename Hash = std::hash<Key>,
          typename Key_equal = std::equal_to<Key>,
          typename Allocator = std::allocator<std::pair<const Key, T>>>
class unordered_map
    : public detail::unordered_map_base<Key, T, Hash, Key_equal, Allocator> {
 public:
};

}  // namespace robin_hood
}  // namespace stroupo::hash_map_lib::open_addressing

#endif  // STROUPO_HASH_MAP_LIB_OPEN_ADDRESSING_ROBIN_HOOD_UNORDERED_MAP_H_