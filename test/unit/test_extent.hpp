#ifndef ECO_TEST_EXTENT_
#define ECO_TEST_EXTENT_

import std;
import eco;

#include <cassert>

inline void
test_extent()
{
  {
    eco::extent<int> x;
    test_regular(x);
  }
  {
    eco::extent<int> x;
    x.push_back(0);
    x.push_back(1);
    x.push_back(2);
    test_regular(x);
  }

  {
    eco::extent<int> x;
    assert(!x);
  }
  {
    eco::extent<int> x;
    x.push_back(0);
    assert(x);
  }
  {
    eco::extent<int> x;
    x.push_back(0);
    x.pop_back();
    assert(!x);
  }

  {
    eco::extent<int> x;
    assert(x.begin() == nullptr);
    assert(x.begin() == x.end());
  }
  {
    eco::extent<int> x;
    x.push_back(0);
    assert(x.begin() != nullptr);
    assert(x.begin() != x.end());
    x.pop_back();
    assert(x.begin() != nullptr);
    assert(x.begin() == x.end());
  }

  {
    eco::extent<int> x;
    assert(x.capacity() == 0);
    assert(x.unused_capacity() == 0);
    assert(x.size() == 0);
  }
  {
    eco::extent<int> x{1};
    assert(x.capacity() == 1);
    assert(x.unused_capacity() == 1);
    assert(x.size() == 0);
  }
  {
    eco::extent<int> x;
    x.push_back(0);
    assert(x.capacity() == 1);
    assert(x.unused_capacity() == 0);
    assert(x.size() == 1);
    x.push_back(1);
    assert(x.capacity() == 2);
    assert(x.unused_capacity() == 0);
    assert(x.size() == 2);
    x.push_back(2);
    assert(x.capacity() == 3);
    assert(x.unused_capacity() == 0);
    assert(x.size() == 3);
    x.push_back(3);
    assert(x.capacity() == 4);
    assert(x.unused_capacity() == 0);
    assert(x.size() == 4);
    x.push_back(4);
    assert(x.capacity() == 6);
    assert(x.unused_capacity() == 1);
    assert(x.size() == 5);
    x.push_back(5);
    assert(x.capacity() == 6);
    assert(x.unused_capacity() == 0);
    assert(x.size() == 6);
    x.push_back(6);
    assert(x.capacity() == 9);
    assert(x.unused_capacity() == 2);
    assert(x.size() == 7);
    x.push_back(7);
    assert(x.capacity() == 9);
    assert(x.unused_capacity() == 1);
    assert(x.size() == 8);
  }

  {
    eco::extent<int, eco::ssize_t<eco::memory_view>, int> x;
    assert(x.metadata() == nullptr);
  }
  {
    eco::extent<int, eco::ssize_t<eco::memory_view>, int> x{1};
    assert(x.metadata() != nullptr);
    *x.metadata() = 1;
    assert(*x.metadata() == 1);
  }

  {
    eco::extent<int> x;
    eco::extent y{x};
    assert(y.capacity() == x.size());
    assert(y.unused_capacity() == 0);
    assert(y.size() == x.size());
  }
  {
    eco::extent<int, eco::ssize_t<eco::memory_view>, int> x;
    eco::extent y{x};
    assert(y.capacity() == x.size());
    assert(y.unused_capacity() == 0);
    assert(y.size() == x.size());
    assert(x.metadata() == y.metadata());
  }
  {
    eco::extent<int, eco::ssize_t<eco::memory_view>, int> x{1};
    *x.metadata() = 1;
    eco::extent y{x};
    assert(y.capacity() == x.size());
    assert(y.unused_capacity() == 0);
    assert(y.size() == x.size());
    assert(*x.metadata() == 1);
    assert(y.metadata() != nullptr);
    assert(x.metadata() != y.metadata());
    assert(*x.metadata() == *y.metadata());
  }
  {
    eco::extent<int, eco::ssize_t<eco::memory_view>, int> x{2};
    *x.metadata() = 1;
    x.push_back(0);
    eco::extent y{x};
    assert(y.capacity() == x.size());
    assert(y.unused_capacity() == 0);
    assert(y.size() == x.size());
    assert(*x.metadata() == 1);
    assert(y.metadata() != nullptr);
    assert(x.metadata() != y.metadata());
    assert(*x.metadata() == *y.metadata());
  }

  {
    eco::extent<int> x;
    eco::extent<int> y;
    y = x;
    assert(y.capacity() == x.size());
    assert(y.unused_capacity() == 0);
    assert(y.size() == x.size());
  }
  {
    eco::extent<int, eco::ssize_t<eco::memory_view>, int> x;
    eco::extent<int, eco::ssize_t<eco::memory_view>, int> y;
    y = x;
    assert(y.capacity() == x.size());
    assert(y.unused_capacity() == 0);
    assert(y.size() == x.size());
    assert(x.metadata() == y.metadata());
  }
  {
    eco::extent<int, eco::ssize_t<eco::memory_view>, int> x{1};
    *x.metadata() = 1;
    eco::extent<int, eco::ssize_t<eco::memory_view>, int> y;
    y = x;
    assert(y.capacity() == x.size());
    assert(y.unused_capacity() == 0);
    assert(y.size() == x.size());
    assert(*x.metadata() == 1);
    assert(y.metadata() != nullptr);
    assert(x.metadata() != y.metadata());
    assert(*x.metadata() == *y.metadata());
  }
  {
    eco::extent<int, eco::ssize_t<eco::memory_view>, int> x{2};
    *x.metadata() = 1;
    x.push_back(0);
    eco::extent<int, eco::ssize_t<eco::memory_view>, int> y;
    y = x;
    assert(y.capacity() == x.size());
    assert(y.unused_capacity() == 0);
    assert(y.size() == x.size());
    assert(*x.metadata() == 1);
    assert(y.metadata() != nullptr);
    assert(x.metadata() != y.metadata());
    assert(*x.metadata() == *y.metadata());
  }
  {
    eco::extent<int> x;
    eco::extent<int> y;
    y.push_back(0);
    y = x;
    assert(y.capacity() == 1);
    assert(y.unused_capacity() == 1);
    assert(y.size() == x.size());
  }
  {
    eco::extent<int, eco::ssize_t<eco::memory_view>, int> x;
    eco::extent<int, eco::ssize_t<eco::memory_view>, int> y;
    y.push_back(0);
    y = x;
    assert(y.capacity() == 1);
    assert(y.unused_capacity() == 1);
    assert(y.size() == x.size());
    assert(x.metadata() != y.metadata());
  }
  {
    eco::extent<int, eco::ssize_t<eco::memory_view>, int> x{1};
    *x.metadata() = 1;
    eco::extent<int, eco::ssize_t<eco::memory_view>, int> y;
    y.push_back(0);
    y = x;
    assert(y.capacity() == 1);
    assert(y.unused_capacity() == 1);
    assert(y.size() == x.size());
    assert(*x.metadata() == 1);
    assert(y.metadata() != nullptr);
    assert(x.metadata() != y.metadata());
    assert(*x.metadata() == *y.metadata());
  }
  {
    eco::extent<int, eco::ssize_t<eco::memory_view>, int> x{2};
    *x.metadata() = 1;
    x.push_back(0);
    eco::extent<int, eco::ssize_t<eco::memory_view>, int> y;
    y.push_back(0);
    y = x;
    assert(y.capacity() == x.size());
    assert(y.unused_capacity() == 0);
    assert(y.size() == x.size());
    assert(*x.metadata() == 1);
    assert(y.metadata() != nullptr);
    assert(x.metadata() != y.metadata());
    assert(*x.metadata() == *y.metadata());
  }

  {
    eco::extent<int> x;
    auto y{std::move(x)};
    assert(x.begin() == nullptr);
    assert(y.begin() == nullptr);
  }
  {
    eco::extent<int, eco::ssize_t<eco::memory_view>, int> x;
    auto y{std::move(x)};
    assert(x.begin() == nullptr);
    assert(y.begin() == nullptr);
    assert(y.metadata() == nullptr);
  }
  {
    eco::extent<int, eco::ssize_t<eco::memory_view>, int> x{1};
    *x.metadata() = 1;
    auto y{std::move(x)};
    assert(x.begin() == nullptr);
    assert(y.capacity() == 1);
    assert(y.unused_capacity() == 1);
    assert(y.size() == 0);
    assert(y.metadata() != nullptr);
    assert(*y.metadata() == 1);
  }
  {
    eco::extent<int, eco::ssize_t<eco::memory_view>, int> x{2};
    *x.metadata() = 1;
    x.push_back(0);
    auto y{std::move(x)};
    assert(x.begin() == nullptr);
    assert(y.capacity() == 2);
    assert(y.unused_capacity() == 1);
    assert(y.size() == 1);
    assert(y.metadata() != nullptr);
    assert(*y.metadata() == 1);
  }

  {
    eco::extent<int> x;
    eco::extent<int> y;
    assert(x == y);
    assert(!(x != y));
    assert(!(x < y));
    assert(x >= y);
    assert(!(x > y));
    assert(x <= y);
  }
  {
    eco::extent<int> x{1};
    eco::extent<int> y;
    assert(x == y);
    assert(!(x != y));
    assert(!(x < y));
    assert(x >= y);
    assert(!(x > y));
    assert(x <= y);
  }
  {
    eco::extent<int> x;
    eco::extent<int> y{2};
    assert(x == y);
    assert(!(x != y));
    assert(!(x < y));
    assert(x >= y);
    assert(!(x > y));
    assert(x <= y);
  }
  {
    eco::extent<int> x{1};
    eco::extent<int> y{2};
    assert(x == y);
    assert(!(x != y));
    assert(!(x < y));
    assert(x >= y);
    assert(!(x > y));
    assert(x <= y);
  }
  {
    eco::extent<int> x;
    x.push_back(0);
    eco::extent<int> y;
    assert(!(x == y));
    assert(x != y);
    assert(!(x < y));
    assert(x >= y);
    assert(x > y);
    assert(!(x <= y));
  }
  {
    eco::extent<int> x;
    eco::extent<int> y;
    y.push_back(0);
    assert(!(x == y));
    assert(x != y);
    assert(x < y);
    assert(!(x >= y));
    assert(!(x > y));
    assert(x <= y);
  }
  {
    eco::extent<int> x;
    x.push_back(0);
    eco::extent<int> y;
    y.push_back(0);
    assert(x == y);
    assert(!(x != y));
    assert(!(x < y));
    assert(x >= y);
    assert(!(x > y));
    assert(x <= y);
  }
  {
    eco::extent<int> x;
    x.push_back(0);
    eco::extent<int> y;
    y.push_back(1);
    assert(!(x == y));
    assert(x != y);
    assert(x < y);
    assert(!(x >= y));
    assert(!(x > y));
    assert(x <= y);
  }
  {
    eco::extent<int> x;
    x.push_back(0);
    x.push_back(1);
    eco::extent<int> y;
    y.push_back(1);
    assert(!(x == y));
    assert(x != y);
    assert(x < y);
    assert(!(x >= y));
    assert(!(x > y));
    assert(x <= y);
  }
  {
    eco::extent<int> x;
    x.push_back(1);
    x.push_back(0);
    eco::extent<int> y;
    y.push_back(1);
    assert(!(x == y));
    assert(x != y);
    assert(!(x < y));
    assert(x >= y);
    assert(x > y);
    assert(!(x <= y));
  }
  {
    eco::extent<int> x;
    x.push_back(1);
    eco::extent<int> y;
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
    eco::extent<int> x;
    x.push_back(1);
    eco::extent<int> y;
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
    eco::extent<int, eco::ssize_t<eco::memory_view>, int> x{1};
    *x.metadata() = 0;
    x.push_back(0);
    eco::extent<int, eco::ssize_t<eco::memory_view>, int> y{1};
    *y.metadata() = 1;
    y.push_back(0);
    assert(x == y);
    assert(!(x != y));
    assert(!(x < y));
    assert(x >= y);
    assert(!(x > y));
    assert(x <= y);
  }

  {
    eco::extent<int> x;
    x.set_unused_capacity(1);
    assert(x.capacity() == 1);
    assert(x.unused_capacity() == 1);
    assert(x.size() == 0);
  }
  {
    eco::extent<int> x{2};
    x.push_back(0);
    x.push_back(1);
    x.push_back(2);
    x.set_unused_capacity(8);
    assert(x.capacity() == 11);
    assert(x.unused_capacity() == 8);
    assert(x.size() == 3);
    assert(*(x.begin() + 0) == 0);
    assert(*(x.begin() + 1) == 1);
    assert(*(x.begin() + 2) == 2);
  }
  {
    eco::extent<int> x{2};
    x.push_back(0);
    x.push_back(1);
    x.push_back(2);
    x.push_back(3);
    x.push_back(4);
    x.set_unused_capacity(0);
    assert(x.capacity() == 5);
    assert(x.unused_capacity() == 0);
    assert(x.size() == 5);
    assert(*(x.begin() + 0) == 0);
    assert(*(x.begin() + 1) == 1);
    assert(*(x.begin() + 2) == 2);
    assert(*(x.begin() + 3) == 3);
    assert(*(x.begin() + 4) == 4);
  }
  {
    eco::extent<int, eco::ssize_t<eco::memory_view>, int> x{1};
    *x.metadata() = 1;
    x.set_unused_capacity(1);
    assert(x.metadata() != nullptr);
    assert(*x.metadata() == 1);
  }

  {
    eco::extent<int> x;
    auto writer = [](int* pos){ std::iota(pos, pos + 3, 10); };
    x.insert_space(3, writer);
    assert(x.capacity() == 3);
    assert(x.unused_capacity() == 0);
    assert(x.size() == 3);
    assert(*(x.begin() + 0) == 10);
    assert(*(x.begin() + 1) == 11);
    assert(*(x.begin() + 2) == 12);
  }
  {
    eco::extent<int> x;
    auto writer = [](int* pos){ std::iota(pos, pos + 3, 10); };
    x.push_back(0);
    x.push_back(1);
    x.insert_space(3, writer);
    assert(x.capacity() == 5);
    assert(x.unused_capacity() == 0);
    assert(x.size() == 5);
    assert(*(x.begin() + 0) == 0);
    assert(*(x.begin() + 1) == 1);
    assert(*(x.begin() + 2) == 10);
    assert(*(x.begin() + 3) == 11);
    assert(*(x.begin() + 4) == 12);
  }

  {
    eco::extent<int> x;
    auto writer = [](int* pos){ std::iota(pos, pos + 3, 10); };
    x.insert_space(x.begin(), 3, writer);
    assert(x.capacity() == 3);
    assert(x.unused_capacity() == 0);
    assert(x.size() == 3);
    assert(*(x.begin() + 0) == 10);
    assert(*(x.begin() + 1) == 11);
    assert(*(x.begin() + 2) == 12);
  }
  {
    eco::extent<int> x;
    auto writer = [](int* pos){ std::iota(pos, pos + 3, 10); };
    x.push_back(0);
    x.push_back(1);
    x.insert_space(x.begin() + 1, 3, writer);
    assert(x.capacity() == 5);
    assert(x.unused_capacity() == 0);
    assert(x.size() == 5);
    assert(*(x.begin() + 0) == 0);
    assert(*(x.begin() + 1) == 10);
    assert(*(x.begin() + 2) == 11);
    assert(*(x.begin() + 3) == 12);
    assert(*(x.begin() + 4) == 1);
  }

  {
    eco::extent<int> x;
    auto writer = [](int* pos){ std::iota(pos, pos + 3, 10); };
    x.insert_space(x.begin(), 3, writer);
    assert(x.capacity() == 3);
    assert(x.unused_capacity() == 0);
    assert(x.size() == 3);
    assert(*(x.begin() + 0) == 10);
    assert(*(x.begin() + 1) == 11);
    assert(*(x.begin() + 2) == 12);
  }
  {
    eco::extent<int> x;
    auto writer = [](int* pos){ std::iota(pos, pos + 3, 10); };
    x.push_back(0);
    x.push_back(1);
    x.insert_space(x.begin() + 1, 3, writer);
    assert(x.capacity() == 5);
    assert(x.unused_capacity() == 0);
    assert(x.size() == 5);
    assert(*(x.begin() + 0) == 0);
    assert(*(x.begin() + 1) == 10);
    assert(*(x.begin() + 2) == 11);
    assert(*(x.begin() + 3) == 12);
    assert(*(x.begin() + 4) == 1);
  }

  {
    eco::extent<int> x;
    x.erase_space(x.end(), 0);
    assert(x.capacity() == 0);
    assert(x.unused_capacity() == 0);
    assert(x.size() == 0);
  }
  {
    eco::extent<int> x;
    x.push_back(0);
    x.push_back(1);
    x.push_back(2);
    x.erase_space(x.begin(), 3);
    assert(x.capacity() == 3);
    assert(x.unused_capacity() == 3);
    assert(x.size() == 0);
  }
  {
    eco::extent<int> x;
    x.push_back(0);
    x.push_back(1);
    x.push_back(2);
    x.erase_space(x.begin(), 2);
    assert(x.capacity() == 3);
    assert(x.unused_capacity() == 2);
    assert(x.size() == 1);
    assert(*(x.begin() + 0) == 2);
  }
  {
    eco::extent<int> x;
    x.push_back(0);
    x.push_back(1);
    x.push_back(2);
    x.erase_space(x.begin() + 1, 2);
    assert(x.capacity() == 3);
    assert(x.unused_capacity() == 2);
    assert(x.size() == 1);
    assert(*(x.begin() + 0) == 0);
  }
  {
    eco::extent<int> x;
    x.push_back(0);
    x.push_back(1);
    x.push_back(2);
    x.erase_space(x.begin() + 1, 1);
    assert(x.capacity() == 3);
    assert(x.unused_capacity() == 1);
    assert(x.size() == 2);
    assert(*(x.begin() + 0) == 0);
    assert(*(x.begin() + 1) == 2);
  }
  {
    eco::extent<int> x;
    x.push_back(0);
    x.push_back(1);
    x.push_back(2);
    x.erase_space(x.begin() + 1, 0);
    assert(x.capacity() == 3);
    assert(x.unused_capacity() == 0);
    assert(x.size() == 3);
    assert(*(x.begin() + 0) == 0);
    assert(*(x.begin() + 1) == 1);
    assert(*(x.begin() + 2) == 2);
  }
  {
    eco::extent<int> x;
    x.push_back(0);
    x.push_back(1);
    x.push_back(2);
    x.erase_space(x.end(), 0);
    assert(x.capacity() == 3);
    assert(x.unused_capacity() == 0);
    assert(x.size() == 3);
    assert(*(x.begin() + 0) == 0);
    assert(*(x.begin() + 1) == 1);
    assert(*(x.begin() + 2) == 2);
  }
}

#endif
