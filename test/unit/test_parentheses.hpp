#ifndef ECO_TEST_BALANCED_PARENTHESES_
#define ECO_TEST_BALANCED_PARENTHESES_

import std;
import eco;

#include <cassert>

inline void
test_basic_parentheses()
{
  eco::basic_bitvector x{40};
  x.bit_set(0);
  x.bit_set(1);
  x.bit_set(2);
  x.bit_set(4);
  x.bit_set(7);
  x.bit_set(8);
  x.bit_set(9);
  x.bit_set(10);
  x.bit_set(11);
  x.bit_set(13);
  x.bit_set(15);
  x.bit_set(19);
  x.bit_set(22);
  x.bit_set(23);
  x.bit_set(24);
  x.bit_set(26);
  x.bit_set(27);
  x.bit_set(30);
  x.bit_set(32);
  x.bit_set(37);

  eco::basic_parentheses p{std::ranges::begin(x), std::ranges::end(x)};

  assert(p.size() == 40);

  assert(p.excess(8) == 3);
  assert(p.excess(21) == 2);
  assert(p.excess(8, 21) == 0);

  assert(eco::find_excess(p, 7, -1) == 36);
  assert(eco::find_excess_backward(p, 36, 0) + 1 == 7);

  assert(eco::find_closing(p, 7) == 36);
  assert(eco::find_opening(p, 36) == 7);
  assert(eco::find_enclosing(p, 22) == 7);

  assert(p.segment_min(8, 36) == 21);
  assert(p.segment_max(8, 36) == 11);
  assert(p.segment_min_count(7, 35) == 2);
  assert(p.segment_min_select(7, 35, 0) + 1 == 8);
  assert(p.segment_min_select(7, 35, 1) + 1 == 22);
  assert(p.segment_min_select(7, 35, 2) == 35);
}

#endif
