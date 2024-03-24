#ifndef ECO_TEST_ARRAY_DICT_
#define ECO_TEST_ARRAY_DICT_

import std;
import eco;

#include <cassert>

inline void
test_array_dict()
{
  {
    eco::array_dict<char> x;
    test_regular(x);
    assert(x.size() == 0);
    assert(x.capacity() == 0);
  }
  {
    eco::array_dict<char> x{3};
    test_regular(x);
    assert(x.size() == 0);
    assert(x.capacity() == 3);
  }
  {
    eco::array_dict<char> x;
    auto a = x.insert('a');
    auto b = x.insert('b');
    auto c = x.insert('c');
    auto d = x.insert('d');
    assert(x.size() == 4);
    assert(x.has_key(a));
    assert(x.has_key(b));
    assert(x.has_key(c));
    assert(x.has_key(d));
    assert(!x.has_key(4));
    x.erase(b);
    assert(x.size() == 3);
    assert(x.has_key(a));
    assert(!x.has_key(b));
    assert(x.has_key(c));
    assert(x.has_key(d));
    assert(!x.has_key(4));
    auto e = x.insert('e');
    assert(x.size() == 4);
    assert(e == b);
    assert(x.has_key(a));
    assert(x.has_key(b));
    assert(x.has_key(c));
    assert(x.has_key(d));
  }

  {
    eco::array_dict<char> x;
    eco::array_dict<char> y;
    assert(x == y);
    assert(!(x != y));
    assert(!(x < y));
    assert(x >= y);
    assert(!(x > y));
    assert(x <= y);
  }
  {
    eco::array_dict<char> x{1};
    eco::array_dict<char> y;
    assert(x == y);
    assert(!(x != y));
    assert(!(x < y));
    assert(x >= y);
    assert(!(x > y));
    assert(x <= y);
  }
  {
    eco::array_dict<char> x;
    eco::array_dict<char> y{2};
    assert(x == y);
    assert(!(x != y));
    assert(!(x < y));
    assert(x >= y);
    assert(!(x > y));
    assert(x <= y);
  }
  {
    eco::array_dict<char> x{1};
    eco::array_dict<char> y{2};
    assert(x == y);
    assert(!(x != y));
    assert(!(x < y));
    assert(x >= y);
    assert(!(x > y));
    assert(x <= y);
  }
  {
    eco::array_dict<char> x;
    auto a = x.insert('a');
    assert(x.has_key(a));
    eco::array_dict<char> y;
    assert(!(x == y));
    assert(x != y);
    assert(!(x < y));
    assert(x >= y);
    assert(x > y);
    assert(!(x <= y));
  }
  {
    eco::array_dict<char> x;
    eco::array_dict<char> y;
    auto a = y.insert('a');
    assert(y.has_key(a));
    assert(!(x == y));
    assert(x != y);
    assert(x < y);
    assert(!(x >= y));
    assert(!(x > y));
    assert(x <= y);
  }
  {
    eco::array_dict<char> x;
    auto ax = x.insert('a');
    eco::array_dict<char> y;
    auto ay = y.insert('a');
    assert(x[ax] == y[ay]);
    assert(x == y);
    assert(!(x != y));
    assert(!(x < y));
    assert(x >= y);
    assert(!(x > y));
    assert(x <= y);
  }
  {
    eco::array_dict<char> x;
    auto ax = x.insert('a');
    eco::array_dict<char> y;
    auto by = y.insert('b');
    assert(x[ax] != y[by]);
    assert(!(x == y));
    assert(x != y);
    assert(x < y);
    assert(!(x >= y));
    assert(!(x > y));
    assert(x <= y);
  }
  {
    eco::array_dict<char> x;
    auto ax = x.insert('a');
    auto bx = x.insert('b');
    eco::array_dict<char> y;
    auto by = y.insert('b');
    assert(x[ax] != x[bx]);
    assert(x[bx] == y[by]);
    assert(!(x == y));
    assert(x != y);
    assert(x < y);
    assert(!(x >= y));
    assert(!(x > y));
    assert(x <= y);
  }
  {
    eco::array_dict<char> x;
    auto bx = x.insert('b');
    auto ax = x.insert('a');
    eco::array_dict<char> y;
    auto by = y.insert('b');
    assert(x[ax] != x[bx]);
    assert(x[bx] == y[by]);
    assert(!(x == y));
    assert(x != y);
    assert(!(x < y));
    assert(x >= y);
    assert(x > y);
    assert(!(x <= y));
  }
  {
    eco::array_dict<char> x;
    auto bx = x.insert('b');
    eco::array_dict<char> y;
    auto ay = y.insert('a');
    auto by = y.insert('b');
    assert(x[bx] == y[by]);
    assert(y[ay] != y[by]);
    assert(!(x == y));
    assert(x != y);
    assert(!(x < y));
    assert(x >= y);
    assert(x > y);
    assert(!(x <= y));
  }
  {
    eco::array_dict<char> x;
    auto bx = x.insert('b');
    eco::array_dict<char> y;
    auto by = y.insert('b');
    auto ay = y.insert('a');
    assert(x[bx] == y[by]);
    assert(y[ay] != y[by]);
    assert(!(x == y));
    assert(x != y);
    assert(x < y);
    assert(!(x >= y));
    assert(!(x > y));
    assert(x <= y);
  }
  {
    eco::array_dict<char> x;
    auto ax = x.insert('a');
    auto bx = x.insert('b');
    eco::array_dict<char> y;
    auto ay = y.insert('a');
    auto by = y.insert('b');
    assert(x[ax] == y[ay]);
    assert(x[bx] == y[by]);
    assert(x == y);
    assert(!(x != y));
    assert(!(x < y));
    assert(x >= y);
    assert(!(x > y));
    assert(x <= y);
  }
  {
    eco::array_dict<char> x;
    auto ax = x.insert('a');
    auto bx = x.insert('b');
    eco::array_dict<char> y;
    auto by = y.insert('b');
    auto ay = y.insert('a');
    assert(x[ax] == y[ay]);
    assert(x[bx] == y[by]);
    assert(!(x == y));
    assert(x != y);
    assert(x < y);
    assert(!(x >= y));
    assert(!(x > y));
    assert(x <= y);
  }
  {
    eco::array_dict<char> x;
    auto ax = x.insert('a');
    auto bx = x.insert('b');
    x.erase(ax);
    eco::array_dict<char> y;
    auto ay = y.insert('a');
    auto by = y.insert('b');
    assert(x[bx] == y[by]);
    assert(y[ay] != y[by]);
    assert(!(x == y));
    assert(x != y);
    assert(x < y);
    assert(!(x >= y));
    assert(!(x > y));
    assert(x <= y);
  }
  {
    eco::array_dict<char> x;
    auto ax = x.insert('a');
    auto bx = x.insert('b');
    eco::array_dict<char> y;
    auto ay = y.insert('a');
    auto by = y.insert('b');
    y.erase(ay);
    assert(x[ax] != x[bx]);
    assert(x[bx] == y[by]);
    assert(!(x == y));
    assert(x != y);
    assert(!(x < y));
    assert(x >= y);
    assert(x > y);
    assert(!(x <= y));
  }

  {
    eco::array_dict<char> x;
    eco::array_dict<char> y;
    x.swap(x);
    assert(x.size() == 0);
    assert(y.size() == 0);
    eco::swap(x, y);
    assert(x.size() == 0);
    assert(y.size() == 0);
  }
  {
    eco::array_dict<char> x;
    eco::array_dict<char> y;
    x.swap(y);
    assert(x.size() == 0);
    assert(y.size() == 0);
    eco::swap(x, y);
    assert(x.size() == 0);
    assert(y.size() == 0);
  }
  {
    eco::array_dict<char> x;
    auto a = x.insert('a');
    auto b = x.insert('b');
    auto c = x.insert('c');
    eco::array_dict<char> y;
    x.swap(x);
    assert(x.size() == 3);
    assert(x[a] == 'a');
    assert(x[b] == 'b');
    assert(x[c] == 'c');
    assert(y.size() == 0);
    eco::swap(x, y);
    assert(x.size() == 0);
    assert(y.size() == 3);
    assert(y[a] == 'a');
    assert(y[b] == 'b');
    assert(y[c] == 'c');
  }
  {
    eco::array_dict<char> x;
    auto a = x.insert('a');
    auto b = x.insert('b');
    auto c = x.insert('c');
    eco::array_dict<char> y;
    x.swap(y);
    assert(x.size() == 0);
    assert(y.size() == 3);
    assert(y[a] == 'a');
    assert(y[b] == 'b');
    assert(y[c] == 'c');
    eco::swap(x, y);
    assert(x.size() == 3);
    assert(x[a] == 'a');
    assert(x[b] == 'b');
    assert(x[c] == 'c');
    assert(y.size() == 0);
  }
  {
    eco::array_dict<char> x;
    eco::array_dict y{x};
    auto a = y.insert('a');
    auto b = y.insert('b');
    auto c = y.insert('c');
    x.swap(y);
    assert(x.size() == 3);
    assert(x[a] == 'a');
    assert(x[b] == 'b');
    assert(x[c] == 'c');
    assert(y.size() == 0);
    eco::swap(x, y);
    assert(x.size() == 0);
    assert(y.size() == 3);
    assert(y[a] == 'a');
    assert(y[b] == 'b');
    assert(y[c] == 'c');
  }
  {
    eco::array_dict<char> x;
    auto a = x.insert('a');
    auto b = x.insert('b');
    auto c = x.insert('c');
    eco::array_dict<char> y;
    auto d = y.insert('d');
    auto e = y.insert('e');
    auto f = y.insert('f');
    x.swap(y);
    assert(x.size() == 3);
    assert(x[d] == 'd');
    assert(x[e] == 'e');
    assert(x[f] == 'f');
    assert(y.size() == 3);
    assert(y[a] == 'a');
    assert(y[b] == 'b');
    assert(y[c] == 'c');
    eco::swap(x, y);
    assert(x.size() == 3);
    assert(x[a] == 'a');
    assert(x[b] == 'b');
    assert(x[c] == 'c');
    assert(y.size() == 3);
    assert(y[d] == 'd');
    assert(y[e] == 'e');
    assert(y[f] == 'f');
  }

  {
    eco::array_dict<char> x;
    assert(!x);
  }
  {
    eco::array_dict<char> x{1};
    assert(!x);
  }
  {
    eco::array_dict<char> x;
    auto a = x.insert('a');
    assert(x);
    x.erase(a);
    assert(!x);
  }

  {
    eco::array_dict<char> x;
    assert(x.begin() == x.end());
  }
  {
    eco::array_dict<char> x;
    auto a = x.insert('a');
    auto b = x.insert('b');
    auto c = x.insert('c');
    assert(a != b);
    assert(a != c);
    assert(b != c);
    assert(x.begin() != x.end());
    assert(x.begin() + x.size() == x.end());
  }

  {
    eco::array_dict<char> x;
    auto a = x.insert('a');
    assert(x.size() == 1);
    assert(x.capacity() == 1);
    auto b = x.insert('b');
    assert(x.size() == 2);
    assert(x.capacity() == 2);
    auto c = x.insert('c');
    assert(x.size() == 3);
    assert(x.capacity() == 3);
    auto d = x.insert('d');
    assert(x.size() == 4);
    assert(x.capacity() == 4);
    auto e = x.insert('e');
    assert(x.size() == 5);
    assert(x.capacity() == 6);
    auto f = x.insert('f');
    assert(x.size() == 6);
    assert(x.capacity() == 6);
    auto g = x.insert('g');
    assert(x.size() == 7);
    assert(x.capacity() == 9);
    auto h = x.insert('h');
    assert(x.size() == 8);
    assert(x.capacity() == 9);
    x.erase(h);
    assert(x.size() == 7);
    assert(x.capacity() == 9);
    x.erase(g);
    assert(x.size() == 6);
    assert(x.capacity() == 9);
    x.erase(f);
    assert(x.size() == 5);
    assert(x.capacity() == 9);
    x.erase(e);
    assert(x.size() == 4);
    assert(x.capacity() == 9);
    x.erase(d);
    assert(x.size() == 3);
    assert(x.capacity() == 9);
    x.erase(c);
    assert(x.size() == 2);
    assert(x.capacity() == 9);
    x.erase(b);
    assert(x.size() == 1);
    assert(x.capacity() == 9);
    x.erase(a);
    assert(x.size() == 0);
    assert(x.capacity() == 9);
  }

  {
    eco::array_dict<char> x;
    assert(x.max_size() == std::numeric_limits<std::int32_t>::max());
    eco::array_dict<double> y;
    assert(y.max_size() == std::numeric_limits<std::int32_t>::max());
  }

  {
    eco::array_dict<char> x;
    auto a = x.insert('a');
    x.set_capacity(3);
    assert(x.size() == 1);
    assert(x[a] == 'a');
    assert(x.capacity() == 3);
    x.set_capacity(3);
    assert(x[a] == 'a');
    assert(x.capacity() == 3);
    x.set_capacity(2);
    assert(x.size() == 1);
    assert(x[a] == 'a');
    assert(x.capacity() == 3);
  }

  {
    eco::array_dict<char> x;
    auto a = x.insert('a');
    x.reset_capacity();
    assert(x.size() == 1);
    assert(x[a] == 'a');
    assert(x.capacity() == 1);
    x.set_capacity(3);
    x.reset_capacity();
    assert(x.size() == 1);
    assert(x[a] == 'a');
    assert(x.capacity() == 1);
  }

  {
    eco::array_dict<char> x{4};
    auto a = x.insert('a');
    auto b = x.insert('b');
    auto c = x.insert('c');
    auto d = x.insert('d');
    x.erase(a);
    assert(x.size() == 3);
    assert(!x.has_key(a));
    assert(x[b] == 'b');
    assert(x[c] == 'c');
    assert(x[d] == 'd');
    x.erase(c);
    assert(x.size() == 2);
    assert(!x.has_key(a));
    assert(x[b] == 'b');
    assert(!x.has_key(c));
    assert(x[d] == 'd');
    x.erase(d);
    assert(x.size() == 1);
    assert(!x.has_key(a));
    assert(x[b] == 'b');
    assert(!x.has_key(c));
    assert(!x.has_key(d));
    x.erase(b);
    assert(x.size() == 0);
    assert(!x.has_key(a));
    assert(!x.has_key(b));
    assert(!x.has_key(c));
    assert(!x.has_key(d));
    a = x.insert('a');
    b = x.insert('b');
    c = x.insert('c');
    d = x.insert('d');
    assert(x.size() == 4);
    assert(x[a] == 'a');
    assert(x[b] == 'b');
    assert(x[c] == 'c');
    assert(x[d] == 'd');
  }
  {
    eco::array_dict<char> x{4};
    auto a = x.insert('a');
    auto b = x.insert('b');
    auto c = x.insert('c');
    auto d = x.insert('d');
    x.erase(a);
    assert(x.size() == 3);
    assert(!x.has_key(a));
    assert(x[b] == 'b');
    assert(x[c] == 'c');
    assert(x[d] == 'd');
    x.erase(c);
    assert(x.size() == 2);
    assert(!x.has_key(a));
    assert(x[b] == 'b');
    assert(!x.has_key(c));
    assert(x[d] == 'd');
    x.erase(d);
    assert(x.size() == 1);
    assert(!x.has_key(a));
    assert(x[b] == 'b');
    assert(!x.has_key(c));
    assert(!x.has_key(d));
    a = x.insert('a');
    c = x.insert('c');
    d = x.insert('d');
    auto e = x.insert('e');
    assert(x.size() == 5);
    assert(x[a] == 'a');
    assert(x[b] == 'b');
    assert(x[c] == 'c');
    assert(x[d] == 'd');
    assert(x[e] == 'e');
  }
  {
    eco::array_dict<char> x{4};
    auto a = x.insert('a');
    auto b = x.insert('b');
    auto c = x.insert('c');
    auto d = x.insert('d');
    x.erase(b);
    assert(x.size() == 3);
    assert(x[a] == 'a');
    assert(!x.has_key(b));
    assert(x[c] == 'c');
    assert(x[d] == 'd');
    b = x.insert('b');
    assert(x.size() == 4);
    assert(x[a] == 'a');
    assert(x[b] == 'b');
    assert(x[c] == 'c');
    assert(x[d] == 'd');
    x.erase(b);
    assert(x.size() == 3);
    assert(x[a] == 'a');
    assert(!x.has_key(b));
    assert(x[c] == 'c');
    assert(x[d] == 'd');
  }

  {
    eco::array_dict<char> x;
    x.clear();
    assert(x.size() == 0);
    assert(x.capacity() == 0);
  }
  {
    eco::array_dict<char> x{3};
    x.clear();
    assert(x.size() == 0);
    assert(x.capacity() == 3);
  }
  {
    eco::array_dict<char> x;
    auto a = x.insert('a');
    assert(x[a] == 'a');
    x.clear();
    assert(x.size() == 0);
    assert(x.capacity() == 1);
  }
}

#endif
