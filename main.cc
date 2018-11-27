#include <bitset>
#include <iomanip>
#include <iostream>
#include <random>

#include <hash_map/hash_map.h>

struct custom_hash {
  std::size_t operator()(int key) const noexcept { return key; }
};

struct bit_reversal_hash {
  std::size_t operator()(int key) const {
    // we assume int == std::uint32_t
    key = ((0b10101010101010101010101010101010 & key) >> 1) |
          ((0b01010101010101010101010101010101 & key) << 1);
    key = ((0b11001100110011001100110011001100 & key) >> 2) |
          ((0b00110011001100110011001100110011 & key) << 2);
    key = ((0b11110000111100001111000011110000 & key) >> 4) |
          ((0b00001111000011110000111100001111 & key) << 4);
    key = ((0b11111111000000001111111100000000 & key) >> 8) |
          ((0b00000000111111110000000011111111 & key) << 8);
    key = ((0b11111111111111110000000000000000 & key) >> 16) |
          ((0b00000000000000001111111111111111 & key) << 16);
    return key;
  }
};

int main(int argc, char** argv) {
  using stroupo::hash_map;
  hash_map<int, int, bit_reversal_hash> map;

  bit_reversal_hash hash{};
  std::mt19937 rng{std::random_device{}()};
  for (int i = 0; i < 10; ++i) {
    const auto number = rng();
    std::cout << std::bitset<32>(number) << "\t"
              << std::bitset<32>(hash(number)) << std::endl;
  }

  map.insert({1, 1});
  map.insert({2, 5});
  map.insert({1, 2});
  map.insert({4, 9});
  map.insert({3, 0});
  map.insert({12, 10});

  for (int i = 0; i < map.data.size(); ++i) {
    std::cout << i << ":\t";
    if (!map.data[i].empty) {
      std::cout << map.data[i].key << "\t" << map.data[i].value;
    } else {
      std::cout << "----";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;

  map.reserve(15);

  for (int i = 0; i < map.data.size(); ++i) {
    std::cout << i << ":\t";
    if (!map.data[i].empty) {
      std::cout << map.data[i].key << "\t" << map.data[i].value;
    } else {
      std::cout << "----";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;

  std::cout << "map[1] = " << map[1] << std::endl;
  std::cout << "map[2] = " << map[2] << std::endl;
  std::cout << "map[3] = " << map[3] << std::endl;
  std::cout << "map[4] = " << map[4] << std::endl;
  // std::cout << "map[1]) = " << map[1]) << std::endl;

  std::cout << "sizeof(hash_map<int,int,bit_reversal_hash>) = "
            << sizeof(hash_map<int, int, bit_reversal_hash>) << std::endl
            << "sizeof(bit_reversal_hash) = " << sizeof(bit_reversal_hash)
            << std::endl;
}