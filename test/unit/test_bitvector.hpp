#ifndef ECO_TEST_BITVECTOR_
#define ECO_TEST_BITVECTOR_

import std;
import eco;

#include <cassert>

inline void
test_basic_bitvector()
{
  {
    eco::basic_bitvector x;
    assert(x.size() == 0);
  }
  {
    eco::basic_bitvector x{0};
    assert(x.size() == 0);
  }
  {
    eco::basic_bitvector x{1};
    assert(x.size() == 1);
  }
  {
    eco::basic_bitvector x{5};
    assert(x.size() == 5);
  }

  {
    eco::basic_bitvector x{55};
    assert(x.size() == 55);

    assert(!x.bitread(0));
    x.bitclear(0);
    assert(!x.bitread(0));
    x.bitset(0);
    assert(x.bitread(0));
    x.bitset(1);
    assert(x.bitread(1));
    x.bitset(2);
    assert(x.bitread(2));
    x.bitset(3);
    assert(x.bitread(3));
    x.bitset(4);
    assert(x.bitread(4));
    x.bitset(16);
    assert(x.bitread(16));
    x.bitset(31);
    assert(x.bitread(31));
    x.bitset(32);
    assert(x.bitread(32));
    x.bitset(54);
    assert(x.bitread(54));
  }

  {
    eco::basic_bitvector x{55};
    x.bitset(1);
    x.bitset(3);

    assert(eco::succ_0(x, 0) == 0);
    assert(eco::succ_0(x, 1) == 2);
    assert(eco::succ_0(x, 2) == 2);
    assert(eco::succ_0(x, 3) == 4);
    assert(eco::succ_0(x, 4) == 4);
    assert(eco::succ_0(x, 5) == 5);

    assert(eco::pred_0(x, 0) == 0);
    assert(eco::pred_0(x, 1) == 0);
    assert(eco::pred_0(x, 2) == 2);
    assert(eco::pred_0(x, 3) == 2);
    assert(eco::pred_0(x, 4) == 4);
    assert(eco::pred_0(x, 5) == 5);

    assert(eco::succ_1(x, 0) == 1);
    assert(eco::succ_1(x, 1) == 1);
    assert(eco::succ_1(x, 2) == 3);
    assert(eco::succ_1(x, 3) == 3);
    assert(eco::succ_1(x, 4) == 55);
    assert(eco::succ_1(x, 5) == 55);

    assert(eco::pred_1(x, 1) == 1);
    assert(eco::pred_1(x, 2) == 1);
    assert(eco::pred_1(x, 3) == 3);
    assert(eco::pred_1(x, 4) == 3);
    assert(eco::pred_1(x, 5) == 3);

    assert(x.rank_0(0) == 0);
    assert(x.rank_0(1) == 1);
    assert(x.rank_0(2) == 1);
    assert(x.rank_0(3) == 2);
    assert(x.rank_0(4) == 2);
    assert(x.rank_0(32) == 30);
    assert(x.rank_0(33) == 31);

    assert(x.rank_1(0) == 0);
    assert(x.rank_1(1) == 0);
    assert(x.rank_1(2) == 1);
    assert(x.rank_1(3) == 1);
    assert(x.rank_1(4) == 2);
    assert(x.rank_1(32) == 2);
    assert(x.rank_1(33) == 2);

    assert(x.select_0(0) == 0);
    assert(x.select_0(1) == 2);
    assert(x.select_0(2) == 4);
    assert(x.select_0(3) == 5);

    assert(x.select_1(0) == 1);
    assert(x.select_1(1) == 3);
    assert(x.select_1(2) == 55);
    assert(x.select_1(3) == 55);
  }
}

#endif
