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

    assert(!x.bit_read(0));
    x.bit_clear(0);
    assert(!x.bit_read(0));
    x.bit_set(0);
    assert(x.bit_read(0));
    x.bit_set(1);
    assert(x.bit_read(1));
    x.bit_set(2);
    assert(x.bit_read(2));
    x.bit_set(3);
    assert(x.bit_read(3));
    x.bit_set(4);
    assert(x.bit_read(4));
    x.bit_set(16);
    assert(x.bit_read(16));
    x.bit_set(31);
    assert(x.bit_read(31));
    x.bit_set(32);
    assert(x.bit_read(32));
    x.bit_set(54);
    assert(x.bit_read(54));
  }

  {
    eco::basic_bitvector x{55};
    assert(x.size() == 55);

    x.bit_set(0);
    x.bit_set(2);
    x.bit_set(30);
    x.bit_set(33);
    x.bit_set(34);

    assert(x.bits_read(0, 1) == 1);
    assert(x.bits_read(0, 2) == 1);
    assert(x.bits_read(0, 3) == 5);
    assert(x.bits_read(30, 1) == 1);
    assert(x.bits_read(30, 2) == 1);
    assert(x.bits_read(30, 3) == 1);
    assert(x.bits_read(30, 4) == 9);
    assert(x.bits_read(30, 5) == 25);
  }

  {
    eco::basic_bitvector x{55};
    x.bit_set(1);
    x.bit_set(3);

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
