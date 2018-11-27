#include <doctest/doctest.h>

#include <type_traits>

#include <hash_map/hash_map.h>

struct custom_hash {
  constexpr std::size_t operator()(int key) const noexcept { return key; }
};

struct custom_equal_to {
  constexpr bool operator()(int lhs, int rhs) const { return 0 == lhs - rhs; }
};

TEST_CASE("The hash map") {
  using key_type = int;
  using mapped_type = int;
  using hash_map = stroupo::hash_map<key_type, mapped_type>;
  using custom_hash_map =
      stroupo::hash_map<key_type, mapped_type, custom_hash, custom_equal_to>;

  static_assert(std::is_same_v<hash_map::key_type, key_type>,
                "hash_map::key_type is not equal to key_type!");
  static_assert(std::is_same_v<hash_map::mapped_type, mapped_type>,
                "hash_map::mapped_type is not equal to mapped_type!");
  static_assert(std::is_same_v<hash_map::hasher, std::hash<key_type>>,
                "hash_map::hasher is not equal to std::hash<key_type>!");
  static_assert(std::is_same_v<hash_map::key_equal, std::equal_to<key_type>>,
                "hash_map::key_equal is not equal to std::equal_to<key_type>!");
  static_assert(std::is_same_v<hash_map::value_type,
                               std::pair<const key_type, mapped_type>>,
                "hash_map::value_type is not equal "
                "to std::pair<const key_type, mapped_type>!");
  static_assert(std::is_same_v<hash_map::allocator_type,
                               std::allocator<hash_map::value_type>>,
                "hash_map::allocator_type is not equal to "
                "std::allocator<hash_map::value_type>!");

  static_assert(std::is_same_v<custom_hash_map::hasher, custom_hash>,
                "custom_hash_map::hasher is not equal to "
                "custom_hash!");
  static_assert(std::is_same_v<custom_hash_map::key_equal, custom_equal_to>,
                "custom_hash_map::key_equal is not equal to custom_equal_to!");

  SUBCASE("can insert and access values through the subscript operator '[]'.") {
    hash_map map{};
    CHECK(map.size() == 0);

    map[0] = 1;
    CHECK(map.size() == 1);
    CHECK(map[0] == 1);
    CHECK(map.size() == 1);

    map[1] = 3;
    CHECK(map.size() == 2);
    CHECK(map[1] == 3);
    CHECK(map.size() == 2);
  }

  SUBCASE("will rehash the values if the load factor is too big.") {
    hash_map map{};

    map.max_load_factor(0.7);
    CHECK(map.max_load_factor() == hash_map::real_type{0.7});

    CHECK(map.size() == 0);
    CHECK(map.load_factor() == 0);

    map[0] = 0;
    CHECK(map.size() == 1);
    CHECK(map.load_factor() < map.max_load_factor());

    map[1] = 4;
    CHECK(map.size() == 2);
    CHECK(map.load_factor() < map.max_load_factor());

    map[2] = 1;
    CHECK(map.size() == 3);
    CHECK(map.load_factor() < map.max_load_factor());

    map[3] = 5;
    CHECK(map.size() == 4);
    CHECK(map.load_factor() < map.max_load_factor());
  }

  SUBCASE(
      "can access const and non-const values through the member function 'at' "
      "which throws an exception if the given key was not already inserted.") {
    hash_map map{};
    map[0] = 0;
    map[1] = 4;
    map[2] = 1;
    map[3] = 5;

    CHECK(map.size() == 4);

    CHECK(map.at(0) == 0);
    CHECK(map.at(1) == 4);
    CHECK(map.at(2) == 1);
    CHECK(map.at(3) == 5);
    CHECK_THROWS_AS(map.at(4), std::out_of_range);

    map.at(2) = 3;
    CHECK(map.at(2) == 3);

    const auto& map_ref = map;
    CHECK(map_ref.at(0) == 0);
    CHECK(map_ref.at(1) == 4);
    CHECK(map_ref.at(2) == 3);
    CHECK(map_ref.at(3) == 5);
    CHECK_THROWS_AS(map_ref.at(4), std::out_of_range);
  }
}