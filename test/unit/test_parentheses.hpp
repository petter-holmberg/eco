#ifndef ECO_TEST_BALANCED_PARENTHESES_
#define ECO_TEST_BALANCED_PARENTHESES_

import std;
import eco;

#include <cassert>

inline void
test_balanced_parentheses()
{
  eco::basic_bitvector x{40};
  x.bit_set(3);
  x.bit_set(5);
  x.bit_set(6);
  x.bit_set(12);
  x.bit_set(14);
  x.bit_set(16);
  x.bit_set(17);
  x.bit_set(18);
  x.bit_set(20);
  x.bit_set(21);
  x.bit_set(25);
  x.bit_set(28);
  x.bit_set(29);
  x.bit_set(31);
  x.bit_set(33);
  x.bit_set(34);
  x.bit_set(35);
  x.bit_set(36);
  x.bit_set(38);
  x.bit_set(39);

  eco::basic_parentheses p{std::ranges::begin(x), std::ranges::end(x)};

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
