#include <doctest/doctest.h>

#include <algorithm>
#include <random>
#include <type_traits>
#include <vector>

#include <hash_map_lib/open_addressing/linear_probing/unordered_map.h>

struct custom_hash {
  constexpr std::size_t operator()(int key) const noexcept { return key; }
};

struct custom_equal_to {
  constexpr bool operator()(int lhs, int rhs) const { return 0 == lhs - rhs; }
};

using key_type = int;
using mapped_type = int;
using hash_map =
    stroupo::hash_map_lib::open_addressing::linear_probing::unordered_map<
        key_type, mapped_type>;
using custom_unordered_map =
    stroupo::hash_map_lib::open_addressing::linear_probing::unordered_map<
        key_type, mapped_type, custom_hash, custom_equal_to>;

template class stroupo::hash_map_lib::open_addressing::linear_probing::
    unordered_map<key_type, mapped_type>;
template class stroupo::hash_map_lib::open_addressing::linear_probing::
    unordered_map<key_type, mapped_type, custom_hash, custom_equal_to>;

// using namespace std;

TEST_CASE("The hash map") {
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

  static_assert(std::is_same_v<custom_unordered_map::hasher, custom_hash>,
                "custom_unordered_map::hasher is not equal to "
                "custom_hash!");
  static_assert(
      std::is_same_v<custom_unordered_map::key_equal, custom_equal_to>,
      "custom_unordered_map::key_equal is not equal to custom_equal_to!");

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

    CHECK(map.size() == 4);

    map.at(2) = 3;
    CHECK(map.at(2) == 3);

    CHECK(map.size() == 4);

    const auto& map_ref = map;
    CHECK(map_ref.at(0) == 0);
    CHECK(map_ref.at(1) == 4);
    CHECK(map_ref.at(2) == 3);
    CHECK(map_ref.at(3) == 5);
    CHECK_THROWS_AS(map_ref.at(4), std::out_of_range);
  }
}

SCENARIO(
    "The hash map can insert and access one or more values through "
    "iterators.") {
  using namespace std;

  static_assert(is_copy_constructible_v<hash_map::iterator>,
                "hash_map::iterator is not copy construbtible!");
  static_assert(is_copy_assignable_v<hash_map::iterator>,
                "hash_map::iterator is not assignable!");
  static_assert(is_destructible_v<hash_map::iterator>,
                "hash_map::iterator is not destructible!");
  static_assert(is_swappable_v<hash_map::iterator>,
                "hash_map::iterator is not swappable!");
  static_assert(is_same_v<iterator_traits<hash_map::iterator>::value_type,
                          hash_map::value_type>);
  // static_assert(is_same_v<iterator_traits<hash_map::iterator>::difference_type,
  //                         hash_map::difference_type>);
  static_assert(is_same_v<iterator_traits<hash_map::iterator>::reference,
                          hash_map::reference>);
  static_assert(is_same_v<iterator_traits<hash_map::iterator>::pointer,
                          hash_map::pointer>);
  static_assert(
      is_same_v<iterator_traits<hash_map::iterator>::iterator_category,
                forward_iterator_tag>);

  GIVEN("a default constructed hash map") {
    hash_map map{};

    GIVEN("some random vector containing values with unique keys") {
      constexpr auto count = 1000;
      mt19937 rng{random_device{}()};

      vector<hash_map::key_type> keys(count);
      iota(begin(keys), end(keys), -count / 2);
      shuffle(begin(keys), end(keys), rng);

      vector<hash_map::mapped_type> values(count);
      generate(begin(values), end(values),
               bind(uniform_int_distribution<int>{-count, count}, ref(rng)));

      vector<hash_map::value_type> data{};
      for (auto i = 0; i < count; ++i) data.push_back({keys[i], values[i]});

      WHEN("the values are inserted") {
        map.insert(begin(data), end(data));

        THEN(
            "for every value one gets a new element in the hash map with the"
            "same key and value") {
          CHECK(map.size() == data.size());
          for (const auto& element : data) {
            CHECK_MESSAGE(map.at(element.first) == element.second,
                          "element = (" << element.first << ", "
                                        << element.second << ")");
          }
        }
      }

      WHEN("keys and their values are inserted separately") {
        map.insert(begin(keys), end(keys), begin(values));

        THEN(
            "for every key-value-pair one gets a new key-value-element in the"
            "hash map.") {
          CHECK(map.size() == keys.size());
          for (auto i = 0; i < keys.size(); ++i) {
            CHECK_MESSAGE(map.at(keys[i]) == values[i],
                          "(keys[i], values[i]) = (" << keys[i] << ", "
                                                     << values[i] << ")");
          }
        }
      }
    }
  }

  GIVEN("a hash map with some initial data") {
    using element_type = pair<hash_map::key_type, hash_map::mapped_type>;
    vector<element_type> data{{1, 1}, {2, 2}, {4, 4}, {5, 5}, {10, 10}};
    hash_map map{};
    map.insert(begin(data), end(data));
    vector<element_type> read{};

    WHEN("one iterates with iterators and helper functions") {
      for (auto it = begin(map); it != end(map); ++it)
        read.push_back({it->first, it->second});
      THEN("every element in the hash map is reached in an undefined order") {
        sort(begin(read), end(read));
        CHECK(read == data);
      }
    }

    WHEN("one iterates with iterators and member functions") {
      for (auto it = map.begin(); it != map.end(); ++it)
        read.push_back({it->first, it->second});
      THEN("every element in the hash map is reached in an undefined order") {
        sort(begin(read), end(read));
        CHECK(read == data);
      }
    }

    WHEN("one iterates with a range-based for loop") {
      for (auto& element : map) read.push_back({element.first, element.second});
      THEN("every element in the hash map is reached in an undefined order") {
        sort(begin(read), end(read));
        CHECK(read == data);
      }
    }

    GIVEN("a constant reference to this hash map") {
      const auto& map_ref = map;

      WHEN("one iterates with iterators and helper functions") {
        for (auto it = begin(map_ref); it != end(map_ref); ++it)
          read.push_back({it->first, it->second});
        THEN("every element in the hash map is reached in an undefined order") {
          sort(begin(read), end(read));
          CHECK(read == data);
        }
      }

      WHEN("one iterates with iterators and member functions") {
        for (auto it = map_ref.begin(); it != map_ref.end(); ++it)
          read.push_back({it->first, it->second});
        THEN("every element in the hash map is reached in an undefined order") {
          sort(begin(read), end(read));
          CHECK(read == data);
        }
      }

      WHEN("one iterates with a range-based for loop") {
        for (const auto& element : map_ref)
          read.push_back({element.first, element.second});
        THEN("every element in the hash map is reached in an undefined order") {
          sort(begin(read), end(read));
          CHECK(read == data);
        }
      }
    }

    SUBCASE(
        "One can access elements through the member function 'find' which "
        "returns an iterator to the element if it exists and the end iterator"
        "if it does not.") {
      auto it = map.find(1);
      CHECK(it->first == 1);
      CHECK(it->second == 1);

      it = map.find(2);
      CHECK(it->first == 2);
      CHECK(it->second == 2);

      it = map.find(3);
      CHECK(it == end(map));
      CHECK(it == map.end());

      it = map.find(4);
      CHECK(it->first == 4);
      CHECK(it->second == 4);

      it = map.find(5);
      CHECK(it->first == 5);
      CHECK(it->second == 5);

      it = map.find(6);
      CHECK(it == end(map));
      CHECK(it == map.end());

      it = map.find(7);
      CHECK(it == end(map));
      CHECK(it == map.end());

      it = map.find(8);
      CHECK(it == end(map));
      CHECK(it == map.end());

      it = map.find(9);
      CHECK(it == end(map));
      CHECK(it == map.end());

      it = map.find(10);
      CHECK(it->first == 10);
      CHECK(it->second == 10);

      GIVEN("a constant reference to this hash map") {
        const auto& map_ref = map;

        SUBCASE(
            "One can access elements through the overloaded member function "
            "'find' which returns a constant iterator to the element if it "
            "exists and the end-iterator if it does not.") {
          auto it = map_ref.find(1);
          CHECK(it->first == 1);
          CHECK(it->second == 1);

          it = map_ref.find(2);
          CHECK(it->first == 2);
          CHECK(it->second == 2);

          it = map_ref.find(3);
          CHECK(it == end(map_ref));
          CHECK(it == map_ref.end());

          it = map_ref.find(4);
          CHECK(it->first == 4);
          CHECK(it->second == 4);

          it = map_ref.find(5);
          CHECK(it->first == 5);
          CHECK(it->second == 5);

          it = map_ref.find(6);
          CHECK(it == end(map_ref));
          CHECK(it == map_ref.end());

          it = map_ref.find(7);
          CHECK(it == end(map_ref));
          CHECK(it == map_ref.end());

          it = map_ref.find(8);
          CHECK(it == end(map_ref));
          CHECK(it == map_ref.end());

          it = map_ref.find(9);
          CHECK(it == end(map_ref));
          CHECK(it == map_ref.end());

          it = map_ref.find(10);
          CHECK(it->first == 10);
          CHECK(it->second == 10);
        }
      }
    }
  }
}

SCENARIO("The hash map can be initialized by initializer lists.") {
  WHEN("an initializer list with unique keys is used") {
    hash_map map{{1, 5}, {-1, 2}, {8, 4}, {5, -4}, {-3, -1}};
    THEN("every key-value-pair can is inserted.") {
      CHECK(size(map) == 5);
      CHECK(map.at(1) == 5);
      CHECK(map.at(-1) == 2);
      CHECK(map.at(8) == 4);
      CHECK(map.at(5) == -4);
      CHECK(map.at(-3) == -1);
    }
  }

  WHEN("an initializer list with non-unique keys is used") {
    hash_map map{{1, 5}, {-1, 2}, {1, 4}, {5, -4}, {5, -1}};
    THEN("the mapped value of non-unique keys is one of the given values.") {
      CHECK(size(map) == 3);
      CHECK((map.at(1) - 5) * (map.at(1) - 4) == 0);
      CHECK(map.at(-1) == 2);
      CHECK((map.at(5) + 4) * (map.at(5) + 1) == 0);
    }
  }
}