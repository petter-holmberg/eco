#ifndef ECO_TEST_TAPE_
#define ECO_TEST_TAPE_

import std;
import eco;

#include <cassert>

inline void
test_tape()
{
  {
    std::array<std::uint16_t, 10> arr{0, 1, 0, 2, 5, 1, 3, 2, 8, 2};

    eco::tape<eco::unary_codec<std::uint64_t>> x{arr};

    assert(x.size() == 10);

    auto pos = x.begin();

    assert(*pos == 0);
    ++pos;
    assert(*pos == 1);
    ++pos;
    assert(*pos == 0);
    ++pos;
    assert(*pos == 2);
    ++pos;
    assert(*pos == 5);
    ++pos;
    assert(*pos == 1);
    ++pos;
    assert(*pos == 3);
    ++pos;
    assert(*pos == 2);
    ++pos;
    assert(*pos == 8);
    ++pos;
    assert(*pos == 2);
    ++pos;
    assert(pos == x.end());

    pos = x.append(1);
    assert(*pos == 1);
    ++pos;
    assert(pos == x.end());
  }

  {
    std::array<std::uint16_t, 10> arr{1, 2, 1, 3, 6, 2, 4, 3, 9, 3};

    eco::tape<eco::gamma_codec<std::uint64_t>> x{arr};

    assert(x.size() == 10);

    auto pos = x.begin();

    assert(*pos == 1);
    ++pos;
    assert(*pos == 2);
    ++pos;
    assert(*pos == 1);
    ++pos;
    assert(*pos == 3);
    ++pos;
    assert(*pos == 6);
    ++pos;
    assert(*pos == 2);
    ++pos;
    assert(*pos == 4);
    ++pos;
    assert(*pos == 3);
    ++pos;
    assert(*pos == 9);
    ++pos;
    assert(*pos == 3);
    ++pos;
    assert(pos == x.end());

    pos = x.append(1);
    assert(*pos == 1);
    ++pos;
    assert(pos == x.end());
  }
}

#endif
