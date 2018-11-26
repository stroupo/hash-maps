#include <doctest/doctest.h>

#include <type_traits>

#include <hash_map/hash_map.h>

struct custom_hash {
  std::size_t operator()(int key) const noexcept { return key; }
};

TEST_CASE("The hash map") {
  using key_type = int;
  using mapped_type = int;
  using hash_map = stroupo::hash_map<key_type, mapped_type>;
  using custom_hash_map = stroupo::hash_map<key_type, mapped_type, custom_hash>;

  static_assert(std::is_same_v<hash_map::key_type, key_type>,
                "hash_map::key_type is not equal to key_type!");
  static_assert(std::is_same_v<hash_map::mapped_type, mapped_type>,
                "hash_map::mapped_type is not equal to mapped_type!");
  static_assert(std::is_same_v<hash_map::hasher, std::hash<key_type>>,
                "hash_map::hasher is not equal to std::hash<key_type>!");
  static_assert(
      std::is_same_v<hash_map::value_type,
                     std::pair<const key_type, mapped_type>>,
      "hash_map::value_type is not equal to std::pair<const key_type, "
      "mapped_type>!");
  static_assert(std::is_same_v<custom_hash_map::hasher, custom_hash>,
                "custom_hash_map::hasher is not equal to "
                "custom_hash>!");

  SUBCASE("can be default constructed.") {
    hash_map map{};
    CHECK(map.size() == 0);
  }
}