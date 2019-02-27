/*
TODO:
    - add persistency
    - include load factor
    - Move up directory
    - get memory footprint of all the operations
    - add realistic use-cases
    - add erase existing int
*/

#include <hash_map_lib/hash_map.h>
#include <algorithm>
#include <boost/filesystem.hpp>
#include <boost/unordered_map.hpp>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

typedef double Time;
typedef std::string KeyType;
typedef std::vector<Time> Timings;
typedef std::vector<std::pair<int, std::vector<Time>>> TimingResults;
boost::filesystem::path full_path(boost::filesystem::current_path());
std::string img_path = full_path.string();

struct Range {
  Range(int start, int stop, int step) : start(start), stop(stop), step(step){};
  Range(int stop) : start(0), stop(stop), step(1){};
  int start;
  int stop;
  int step;
};

template <typename HashMap>
auto mf_sequential_insertion(HashMap &hm,
                             std::vector<typename HashMap::key_type> &keyvec) {
  return [&hm, &keyvec]() {
    for (auto key : keyvec) {
      hm.insert({key, 0});
    }
  };
}
template <typename HashMap>
auto mf_sequential_lookups(HashMap &hm,
                           std::vector<typename HashMap::key_type> &keyvec) {
  return [&hm, &keyvec]() {
    for (auto key : keyvec) {
      hm.find(key);
    }
  };
}

template <typename Function>
Time measure(Function function, int repetitions = 1) {
  auto start = std::chrono::high_resolution_clock::now();
  function();
  auto end = std::chrono::high_resolution_clock::now();
  auto diff = end - start;
  return std::chrono::duration<Time>(diff).count();
}
struct MakeUniqueString {
  MakeUniqueString(int length = 32, int count = 0)
      : length(length), count(count) {}
  int length;
  int count;
  std::string str{"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"};
  // std::random_device rd;
  std::mt19937 rng{std::random_device{}()};
  std::string operator()() {
    std::shuffle(str.begin(), str.end(), rng);
    return std::to_string(count++).append(str).substr(0, 32);
  }
};
template <typename T>
std::vector<T> make_unique_vector(int size);

template <>
std::vector<int> make_unique_vector<int>(int size) {
  std::vector<int> vec(size);
  std::iota(vec.begin(), vec.end(), 0);
  return vec;
}
template <>
std::vector<std::string> make_unique_vector<std::string>(int size) {
  std::vector<std::string> vec(size);
  MakeUniqueString mus;
  std::generate(vec.begin(), vec.end(), [&]() { return mus(); });
  return vec;
}
template <typename T>
std::vector<T> make_vector(int size, float non_unique = 0.0f) {
  std::vector<T> vec = make_unique_vector<T>(size);

  std::random_device rd;
  std::mt19937 rng(rd());
  std::uniform_int_distribution<int> uni(0, vec.size() - 1);

  if (non_unique > 0.0f) {
    int mults = (int)std::floor(non_unique * size);
    // std::for_each(vec.begin(), vec.end(),
    // [&vec, &uni, &rng](T& x){x = vec[uni(rng)];});
    std::transform(vec.begin(), vec.begin() + mults, vec.begin(),
                   [&vec, &uni, &rng](T i) { return vec[uni(rng)]; });
  }
  std::shuffle(vec.begin(), vec.end(), rng);
  return vec;
}
template <typename T>
std::vector<T> range(T start, T stop, T step) {
  std::vector<T> vec;
  T i = start;
  while ((step > 0) ? (i < stop) : (i > stop)) {
    vec.push_back(i);
    i += step;
  }
  return vec;
}
template <typename T>
std::vector<T> range(Range &r) {
  std::vector<T> vec;
  T i = r.start;
  while ((r.step > 0) ? (i < r.stop) : (i > r.stop)) {
    vec.push_back(i);
    i += r.step;
  }
  return vec;
}

template <typename T, class... HashMaps>
Timings time_sequential_insert_h(int size, float non_unique, HashMaps... hms) {
  std::vector<T> vec = make_vector<T>(size, non_unique);
  Timings tms{measure(mf_sequential_insertion(hms, vec))...};
  return tms;
}
template <typename T, class... HashMaps>
Timings time_sequential_lookups_h(int size, float non_unique,
                                  std::vector<T> &vec, HashMaps... hms) {
  Timings tms{measure(mf_sequential_lookups(hms, vec))...};
  return tms;
}
template <typename KeyType>
TimingResults time_sequential_insert(Range &r, float unique,
                                     bool verbose = true) {
  std::vector<int> sizes = range<int>(r);
  TimingResults timing_results;
  for (int size : sizes) {
    // std::unordered_map<KeyType, int, std::hash<KeyType>> stl_hm;
    stroupo::hash_map_lib::hash_map<KeyType, int> stroupo_hm;
    std::unordered_map<KeyType, int> stl_hm;
    boost::unordered_map<KeyType, int> boost_hm;
    // boost::unordered_map<KeyType, int, std::hash<KeyType>> boost_hm;
    Timings timings = time_sequential_insert_h<KeyType>(size, unique, boost_hm,
                                                        stl_hm, stroupo_hm);
    if (verbose) {
      std::cout << size;
      for (auto t : timings) std::cout << "\t" << t;
      std::cout << "\n";
    }
    timing_results.push_back({size, timings});
  }
  return timing_results;
}
template <typename KeyType>
TimingResults time_sequential_lookups(Range &r, float unique,
                                      bool verbose = true) {
  std::vector<int> sizes = range<int>(r);
  TimingResults timing_results;
  for (int size : sizes) {
    std::vector<KeyType> keys = make_vector<KeyType>(size, unique);
    std::unordered_map<KeyType, int> stl_hm;
    boost::unordered_map<KeyType, int> boost_hm;
    for (const KeyType &k : keys) {
      boost_hm.insert({k, 0});
      stl_hm.insert({k, 0});
    }
    Timings timings = time_sequential_lookups_h<KeyType>(size, unique, keys,
                                                         boost_hm, stl_hm);
    if (verbose) {
      std::cout << size;
      for (auto t : timings) std::cout << "\t" << t;
      std::cout << "\n";
    }
    timing_results.push_back({size, timings});
  }
  return timing_results;
}

std::string toPylist(TimingResults &trs) {
  std::string str = "[";
  for (auto tr : trs) {
    str.append("[");
    str.append(std::to_string(tr.first));
    for (auto r : tr.second) {
      str.append(",");
      str.append(std::to_string(r));
    }
    str.append("],\n");
  }
  str.append("]");
  return str;
}
std::string trToPython(TimingResults &trs, std::string title,
                       std::string filename, std::vector<std::string> names,
                       std::string xlabel = "input size",
                       std::string ylabel = "time / s") {
  std::string code;
  code.append("import matplotlib.pyplot as plt\n");
  code.append("xys=" + toPylist(trs) + "\n");
  code.append("title='" + title + "'\n");
  code.append("filename='" + filename + "'\n");
  code.append("names=[");
  for (auto name : names) code.append("'" + name + "',");
  code.append("]\n");
  code.append("xlabel='" + xlabel + "'\n");
  code.append("ylabel='" + ylabel + "'\n");
  code.append("xys = list(map(list, zip(*xys)))\n");
  code.append("xs = xys[0]\n");
  code.append("for ys, name in zip(xys[1:], names):\n");
  code.append("\tplt.plot(xs, ys, label=name)\n");
  code.append("plt.xlabel(xlabel)\n");
  code.append("plt.ylabel(ylabel)\n");
  code.append("plt.title(title)\n");
  code.append("plt.legend()\n");
  code.append("plt.savefig(filename)\n");
  code = "\"" + code + "\"";
  return code;
}

template <typename KeyType>
void benchmark_all_sequential_insertions(Range &r, std::string keytype,
                                         float unique, std::string filename) {
  TimingResults trs = time_sequential_insert<KeyType>(r, unique);
  MakeUniqueString mus;
  std::string code =
      trToPython(trs,
                 "Sequential Inserts - Unique: " + std::to_string(unique) +
                     "  - " + keytype,
                 img_path + "/" + filename, {"BOOST", "STL", "STROUPO"});
  std::system(("python -c " + code).c_str());
}
template <typename KeyType>
void benchmark_all_sequential_lookups(Range &r, std::string keytype,
                                      float unique, std::string filename) {
  TimingResults trs = time_sequential_lookups<KeyType>(r, unique);
  MakeUniqueString mus;
  std::string code =
      trToPython(trs,
                 "Sequential Lookups - Unique: " + std::to_string(unique) +
                     "  - " + keytype,
                 img_path + "/" + filename, {"BOOST", "STL", "STROUPO"});
  std::system(("python -c " + code).c_str());
}

int main() {
  Range r{60'000, 200'000, 20'000};
  benchmark_all_sequential_insertions<std::string>(r, "str", 0.0f,
                                                   "inserts-unique-str");
  benchmark_all_sequential_insertions<std::string>(r, "str", 0.5f,
                                                   "inserts-non-unique-str");
  benchmark_all_sequential_insertions<int>(r, "int", 0.0f,
                                           "inserts-unique-int");
  benchmark_all_sequential_insertions<int>(r, "int", 0.5f,
                                           "inserts-non-unique-int");
  benchmark_all_sequential_lookups<std::string>(r, "str", 0.0f,
                                                "lookups-unique-str");
  benchmark_all_sequential_lookups<std::string>(r, "str", 0.5f,
                                                "lookups-non-unique-str");
  benchmark_all_sequential_lookups<int>(r, "int", 0.0f, "lookups-unique-int");
  benchmark_all_sequential_lookups<int>(r, "int", 0.5f,
                                        "lookups-non-unique-int");
}