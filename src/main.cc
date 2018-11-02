#include <iostream>
#include <stdexcept>
#include <vector>

struct hash_map {
  struct entry {
    entry() = default;
    entry(int in_key, int in_value)
        : key{in_key}, value{in_value}, empty{false} {}

    int key{0};
    int value{0};
    bool empty{true};
  };

  hash_map() : data(2), load{0} {}

  void resize(int new_size) {
    std::vector<entry> new_data(new_size);
    for (auto e : data) {
      if (e.empty) continue;
      int index = hash(e.key) % new_data.size();
      while (!new_data[index].empty && e.key != new_data[index].key) {
        index = (index + 1) % new_data.size();
      }
      new_data[index] = e;
    }
    data.swap(new_data);
  }

  void insert(int key, int value) {
    int index = hash(key) % data.size();
    while (!data[index].empty && key != data[index].key) {
      index = (index + 1) % data.size();
    }
    if (data[index].empty) ++load;
    data[index] = {key, value};

    if (static_cast<float>(load) / data.size() >= 0.7) resize(2 * data.size());
  }

  int search(int key) {
    int index = hash(key) % data.size();
    while (!data[index].empty && key != data[index].key) {
      index = (index + 1) % data.size();
    }

    if (data[index].empty) throw std::runtime_error{"no key!"};

    return data[index].value;
  }

  int hash(int x) { return x; }

  std::vector<entry> data;
  int load;
};

int main(int argc, char** argv) {
  hash_map map;
  // map.data.resize(10);

  map.insert(1, 1);
  map.insert(2, 5);
  map.insert(1, 2);
  map.insert(4, 9);
  map.insert(3, 0);
  map.insert(12, 10);

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

  map.resize(15);

  for (int i = 0; i < map.data.size(); ++i) {
    std::cout << i << ":\t";
    if (!map.data[i].empty) {
      std::cout << map.data[i].key << "\t" << map.data[i].value;
    } else {
      std::cout << "----";
    }
    std::cout << std::endl;
  }

  std::cout << "map.search(1) = " << map.search(1) << std::endl;
  std::cout << "map.search(2) = " << map.search(2) << std::endl;
  std::cout << "map.search(3) = " << map.search(3) << std::endl;
  std::cout << "map.search(4) = " << map.search(4) << std::endl;
  // std::cout << "map.search(13) = " << map.search(13) << std::endl;
}