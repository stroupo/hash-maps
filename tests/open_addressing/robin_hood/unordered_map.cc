#include <doctest/doctest.h>

#include <type_traits>

#include <hash_map_lib/open_addressing/robin_hood/unordered_map.h>

// using namespace std;

using key_type = int;
using mapped_type = int;
using unordered_map =
    stroupo::hash_map_lib::open_addressing::robin_hood::unordered_map<
        key_type, mapped_type>;

TEST_CASE("The hash map") {
  static_assert(std::is_same_v<unordered_map::key_type, key_type>,
                "unordered_map::key_type is not equal to key_type!");
  static_assert(std::is_same_v<unordered_map::mapped_type, mapped_type>,
                "unordered_map::mapped_type is not equal to mapped_type!");
  static_assert(std::is_same_v<unordered_map::hasher, std::hash<key_type>>,
                "unordered_map::hasher is not equal to std::hash<key_type>!");
  static_assert(
      std::is_same_v<unordered_map::key_equal, std::equal_to<key_type>>,
      "unordered_map::key_equal is not equal to std::equal_to<key_type>!");
  static_assert(std::is_same_v<unordered_map::value_type,
                               std::pair<const key_type, mapped_type>>,
                "unordered_map::value_type is not equal "
                "to std::pair<const key_type, mapped_type>!");
  static_assert(std::is_same_v<unordered_map::allocator_type,
                               std::allocator<unordered_map::value_type>>,
                "unordered_map::allocator_type is not equal to "
                "std::allocator<unordered_map::value_type>!");
}