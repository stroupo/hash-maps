#include <doctest/doctest.h>

#include <iostream>

#include <iterator>
#include <type_traits>

using namespace std;

template <typename T, bool Constant>
struct range_iterator {
  range_iterator(T n) : n_{n} {}

  range_iterator& operator++() { return (++n_, *this); }
  T operator*() const { return n_; }
  bool operator==(range_iterator it) const { return n_ == it.n_; }
  bool operator!=(range_iterator it) const { return !(*this == it); }

  T n_;
};

struct range {
  using value_type = int;
  using iterator = range_iterator<int, false>;
  using const_iterator = range_iterator<int, true>;

  range(int first, int last) : first_{first}, last_{last} {}

  iterator begin() { return first_; }
  iterator end() { return last_; }

  int first_;
  int last_;
};

template <int First, int Last>
struct static_range {
  // using value_type = int;
  using iterator = range_iterator<int, false>;
  using const_iterator = range_iterator<int, true>;

  iterator begin() { return First; }
  iterator end() { return Last; }
};

TEST_CASE("Ranges") {
  for (auto x : range{1, 10}) cout << x << "\t";
  cout << endl;
  for (auto x : static_range<1, 10>{}) cout << x << "\t";
  cout << endl;
}