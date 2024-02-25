#ifndef ECO_TEST_ARRAY_
#define ECO_TEST_ARRAY_

import std;
import eco;

#include <cassert>

inline void
test_array()
{
  {
    eco::array<int> x;
    test_regular(x);
    assert(x.size() == 0);
    assert(x.capacity() == 0);
  }
  {
    eco::array<int> x{3};
    test_regular(x);
    assert(x.size() == 0);
    assert(x.capacity() == 3);
  }
  {
    int data[3] = {1, 2, 3};
    eco::array x{data};
    test_regular(x);
    assert(x.size() == 3);
    assert(x.capacity() == 3);
    assert(x[0] == 1);
    assert(x[1] == 2);
    assert(x[2] == 3);
  }
  {
    int data[3] = {1, 2, 3};
    eco::array<int> x;
    x = data;
    test_regular(x);
    assert(x.size() == 3);
    assert(x.capacity() == 3);
    assert(x[0] == 1);
    assert(x[1] == 2);
    assert(x[2] == 3);
  }
  {
    int data[3] = {1, 2, 3};
    eco::array<int> x;
    x.push_back(0);
    x = data;
    test_regular(x);
    assert(x.size() == 3);
    assert(x.capacity() == 3);
    assert(x[0] == 1);
    assert(x[1] == 2);
    assert(x[2] == 3);
  }

  {
    eco::array<int> x;
    eco::array<int> y;
    assert(x == y);
    assert(!(x != y));
    assert(!(x < y));
    assert(x >= y);
    assert(!(x > y));
    assert(x <= y);
  }
  {
    eco::array<int> x{1};
    eco::array<int> y;
    assert(x == y);
    assert(!(x != y));
    assert(!(x < y));
    assert(x >= y);
    assert(!(x > y));
    assert(x <= y);
  }
  {
    eco::array<int> x;
    eco::array<int> y{2};
    assert(x == y);
    assert(!(x != y));
    assert(!(x < y));
    assert(x >= y);
    assert(!(x > y));
    assert(x <= y);
  }
  {
    eco::array<int> x{1};
    eco::array<int> y{2};
    assert(x == y);
    assert(!(x != y));
    assert(!(x < y));
    assert(x >= y);
    assert(!(x > y));
    assert(x <= y);
  }
  {
    eco::array<int> x;
    x.push_back(0);
    eco::array<int> y;
    assert(!(x == y));
    assert(x != y);
    assert(!(x < y));
    assert(x >= y);
    assert(x > y);
    assert(!(x <= y));
  }
  {
    eco::array<int> x;
    eco::array<int> y;
    y.push_back(0);
    assert(!(x == y));
    assert(x != y);
    assert(x < y);
    assert(!(x >= y));
    assert(!(x > y));
    assert(x <= y);
  }
  {
    eco::array<int> x;
    x.push_back(0);
    eco::array<int> y;
    y.push_back(0);
    assert(x == y);
    assert(!(x != y));
    assert(!(x < y));
    assert(x >= y);
    assert(!(x > y));
    assert(x <= y);
  }
  {
    eco::array<int> x;
    x.push_back(0);
    eco::array<int> y;
    y.push_back(1);
    assert(!(x == y));
    assert(x != y);
    assert(x < y);
    assert(!(x >= y));
    assert(!(x > y));
    assert(x <= y);
  }
  {
    eco::array<int> x;
    x.push_back(0);
    x.push_back(1);
    eco::array<int> y;
    y.push_back(1);
    assert(!(x == y));
    assert(x != y);
    assert(x < y);
    assert(!(x >= y));
    assert(!(x > y));
    assert(x <= y);
  }
  {
    eco::array<int> x;
    x.push_back(1);
    x.push_back(0);
    eco::array<int> y;
    y.push_back(1);
    assert(!(x == y));
    assert(x != y);
    assert(!(x < y));
    assert(x >= y);
    assert(x > y);
    assert(!(x <= y));
  }
  {
    eco::array<int> x;
    x.push_back(1);
    eco::array<int> y;
    y.push_back(0);
    y.push_back(1);
    assert(!(x == y));
    assert(x != y);
    assert(!(x < y));
    assert(x >= y);
    assert(x > y);
    assert(!(x <= y));
  }
  {
    eco::array<int> x;
    x.push_back(1);
    eco::array<int> y;
    y.push_back(1);
    y.push_back(0);
    assert(!(x == y));
    assert(x != y);
    assert(x < y);
    assert(!(x >= y));
    assert(!(x > y));
    assert(x <= y);
  }

  {
    eco::array<int> x;
    eco::array<int> y;
    x.swap(x);
    assert(x.size() == 0);
    assert(y.size() == 0);
    eco::swap(x, y);
    assert(x.size() == 0);
    assert(y.size() == 0);
  }
  {
    eco::array<int> x;
    eco::array<int> y;
    x.swap(y);
    assert(x.size() == 0);
    assert(y.size() == 0);
    eco::swap(x, y);
    assert(x.size() == 0);
    assert(y.size() == 0);
  }
  {
    int data[3] = {1, 2, 3};
    eco::array x{data};
    eco::array<int> y;
    x.swap(x);
    assert(x.size() == 3);
    assert(x[0] == 1);
    assert(x[1] == 2);
    assert(x[2] == 3);
    assert(y.size() == 0);
    eco::swap(x, y);
    assert(x.size() == 0);
    assert(y.size() == 3);
    assert(y[0] == 1);
    assert(y[1] == 2);
    assert(y[2] == 3);
  }
  {
    int data[3] = {1, 2, 3};
    eco::array x{data};
    eco::array<int> y;
    x.swap(y);
    assert(x.size() == 0);
    assert(y.size() == 3);
    assert(y[0] == 1);
    assert(y[1] == 2);
    assert(y[2] == 3);
    eco::swap(x, y);
    assert(x.size() == 3);
    assert(x[0] == 1);
    assert(x[1] == 2);
    assert(x[2] == 3);
    assert(y.size() == 0);
  }
  {
    int data[3] = {1, 2, 3};
    eco::array<int> x;
    eco::array y{data};
    x.swap(y);
    assert(x.size() == 3);
    assert(x[0] == 1);
    assert(x[1] == 2);
    assert(x[2] == 3);
    assert(y.size() == 0);
    eco::swap(x, y);
    assert(x.size() == 0);
    assert(y.size() == 3);
    assert(y[0] == 1);
    assert(y[1] == 2);
    assert(y[2] == 3);
  }
  {
    int xdata[3] = {1, 2, 3};
    int ydata[3] = {4, 5, 6};
    eco::array x{xdata};
    eco::array y{ydata};
    x.swap(y);
    assert(x.size() == 3);
    assert(x[0] == 4);
    assert(x[1] == 5);
    assert(x[2] == 6);
    assert(y.size() == 3);
    assert(y[0] == 1);
    assert(y[1] == 2);
    assert(y[2] == 3);
    eco::swap(x, y);
    assert(x.size() == 3);
    assert(x[0] == 1);
    assert(x[1] == 2);
    assert(x[2] == 3);
    assert(y.size() == 3);
    assert(y[0] == 4);
    assert(y[1] == 5);
    assert(y[2] == 6);
  }

  {
    eco::array<int> x;
    assert(!x);
  }
  {
    eco::array<int> x{1};
    assert(!x);
  }
  {
    eco::array<int> x;
    x.push_back(0);
    assert(x);
    x.pop_back();
    assert(!x);
  }

  {
    eco::array<int> x;
    assert(x.begin() == x.end());
  }
  {
    int data[3] = {1, 2, 3};
    eco::array x{data};
    assert(x.begin() != x.end());
    assert(x.begin() + x.size() == x.end());
    assert(*(x.begin() + 0) == 1);
    assert(*(x.begin() + 1) == 2);
    assert(*(x.begin() + 2) == 3);
    *(x.begin() + 1) = 0;
    assert(*(x.begin() + 0) == 1);
    assert(*(x.begin() + 1) == 0);
    assert(*(x.begin() + 2) == 3);
  }

  {
    eco::array<int> x;
    assert(x.size() == 0);
    assert(x.capacity() == 0);
  }
  {
    eco::array<int> x{1};
    assert(x.size() == 0);
    assert(x.capacity() == 1);
  }
  {
    eco::array<int> x;
    x.push_back(0);
    assert(x.size() == 1);
    assert(x.capacity() == 1);
    x.push_back(1);
    assert(x.size() == 2);
    assert(x.capacity() == 2);
    x.push_back(2);
    assert(x.size() == 3);
    assert(x.capacity() == 3);
    x.push_back(3);
    assert(x.size() == 4);
    assert(x.capacity() == 4);
    x.push_back(4);
    assert(x.size() == 5);
    assert(x.capacity() == 6);
    x.push_back(5);
    assert(x.size() == 6);
    assert(x.capacity() == 6);
    x.push_back(6);
    assert(x.size() == 7);
    assert(x.capacity() == 9);
    x.push_back(7);
    assert(x.size() == 8);
    assert(x.capacity() == 9);
    x.pop_back();
    assert(x.size() == 7);
    assert(x.capacity() == 9);
    x.pop_back();
    assert(x.size() == 6);
    assert(x.capacity() == 9);
    x.pop_back();
    assert(x.size() == 5);
    assert(x.capacity() == 9);
    x.pop_back();
    assert(x.size() == 4);
    assert(x.capacity() == 9);
    x.pop_back();
    assert(x.size() == 3);
    assert(x.capacity() == 9);
    x.pop_back();
    assert(x.size() == 2);
    assert(x.capacity() == 9);
    x.pop_back();
    assert(x.size() == 1);
    assert(x.capacity() == 9);
    x.pop_back();
    assert(x.size() == 0);
    assert(x.capacity() == 9);
  }

  {
    eco::array<int> x;
    assert(x.max_size() == std::numeric_limits<eco::ssize_t<decltype(x)>>::max() / int(sizeof(int)));
    eco::array<double> y;
    assert(y.max_size() == std::numeric_limits<eco::ssize_t<decltype(x)>>::max() / int(sizeof(double)));
  }

  {
    eco::array<int> x;
    x.push_back(0);
    x.reserve(3);
    assert(x.size() == 1);
    assert(x[0] == 0);
    assert(x.capacity() == 3);
    x.reserve(3);
    assert(x[0] == 0);
    assert(x.capacity() == 3);
    x.reserve(2);
    assert(x.size() == 1);
    assert(x[0] == 0);
    assert(x.capacity() == 3);
  }

  {
    eco::array<int> x;
    x.push_back(0);
    x.shrink_to_fit();
    assert(x.size() == 1);
    assert(x[0] == 0);
    assert(x.capacity() == 1);
    x.reserve(3);
    x.shrink_to_fit();
    assert(x.size() == 1);
    assert(x[0] == 0);
    assert(x.capacity() == 1);
  }

  {
    int data[3] = {1, 2, 3};
    eco::array<int> x;
    eco::array<int> y;
    x.append(y);
    assert(x.size() == 0);
    x.append(data);
    assert(x.size() == 3);
    assert(x[0] == 1);
    assert(x[1] == 2);
    assert(x[2] == 3);
    y = x;
    x.append(y);
    assert(x.size() == 6);
    assert(x[0] == 1);
    assert(x[1] == 2);
    assert(x[2] == 3);
    assert(x[3] == 1);
    assert(x[4] == 2);
    assert(x[5] == 3);
  }

  {
    eco::array<int> x;
    x.insert(x.end(), 1);
    assert(x.size() == 1);
    assert(x[0] == 1);
    x.insert(x.begin(), 2);
    assert(x.size() == 2);
    assert(x[0] == 2);
    assert(x[1] == 1);
    x.insert(x.begin() + 1, 3);
    assert(x.size() == 3);
    assert(x[0] == 2);
    assert(x[1] == 3);
    assert(x[2] == 1);
  }

  {
    int data[3] = {1, 2, 3};
    eco::array<int> x;
    x.insert(x.end(), data);
    assert(x.size() == 3);
    assert(x[0] == 1);
    assert(x[1] == 2);
    assert(x[2] == 3);
    x.insert(x.begin(), data);
    assert(x.size() == 6);
    assert(x[0] == 1);
    assert(x[1] == 2);
    assert(x[2] == 3);
    assert(x[3] == 1);
    assert(x[4] == 2);
    assert(x[5] == 3);
    auto y{x};
    x.insert(x.begin() + 1, y);
    assert(x.size() == 12);
    assert(x[0] == 1);
    assert(x[1] == 1);
    assert(x[2] == 2);
    assert(x[3] == 3);
    assert(x[4] == 1);
    assert(x[5] == 2);
    assert(x[6] == 3);
    assert(x[7] == 2);
    assert(x[8] == 3);
    assert(x[9] == 1);
    assert(x[10] == 2);
    assert(x[11] == 3);
  }

  {
    int data[4] = {1, 2, 3, 4};
    eco::array x{data};
    x.erase(x.begin());
    assert(x.size() == 3);
    assert(x[0] == 2);
    assert(x[1] == 3);
    assert(x[2] == 4);
    x.erase(x.begin() + 1);
    assert(x.size() == 2);
    assert(x[0] == 2);
    assert(x[1] == 4);
    x.erase(x.begin() + 1);
    assert(x.size() == 1);
    assert(x[0] == 2);
    x.erase(x.begin());
    assert(x.size() == 0);
  }

  {
    int data[4] = {1, 2, 3, 4};
    eco::array x{data};
    x.erase(std::ranges::subrange{x.begin() + 1, x.begin() + 1});
    assert(x.size() == 4);
    assert(x[0] == 1);
    assert(x[1] == 2);
    assert(x[2] == 3);
    assert(x[3] == 4);
    x.erase(std::ranges::subrange{x.begin() + 1, x.begin() + 3});
    assert(x.size() == 2);
    assert(x[0] == 1);
    assert(x[1] == 4);
    x.erase(x);
    assert(x.size() == 0);
  }

  {
    eco::array<int> x;
    x.clear();
    assert(x.size() == 0);
    assert(x.capacity() == 0);
  }
  {
    eco::array<int> x{3};
    x.clear();
    assert(x.size() == 0);
    assert(x.capacity() == 3);
  }
  {
    int data[3] = {1, 2, 3};
    eco::array x{data};
    x.clear();
    assert(x.size() == 0);
    assert(x.capacity() == 3);
  }

  {
    eco::array<int> x;
    eco::resize(x, 0, 1);
    assert(x.size() == 0);
    assert(x.capacity() == 0);
    eco::resize(x, 2, 1);
    assert(x.size() == 2);
    assert(x[0] == 1);
    assert(x[1] == 1);
    eco::resize(x, 4, 2);
    assert(x.size() == 4);
    assert(x[0] == 1);
    assert(x[1] == 1);
    assert(x[2] == 2);
    assert(x[3] == 2);
    assert(x.capacity() == 4);
    eco::resize(x, 2, 3);
    assert(x.size() == 2);
    assert(x[0] == 1);
    assert(x[1] == 1);
    eco::resize(x, 0, 0);
    assert(x.size() == 0);
    assert(x.capacity() == 4);
  }
}

#endif
