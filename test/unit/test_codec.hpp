#ifndef ECO_TEST_CODEC_
#define ECO_TEST_CODEC_

import std;
import eco;

#include <cassert>

inline void
test_unary_codec()
{
  eco::unary_codec<std::uint32_t> tuc;
  uint32_t x[2]{0, 0};

  eco::bit_ptr<uint32_t> pos{std::begin(x), 0};

  {
    auto npos{tuc.encode(0, pos)};
    assert(npos.pos == std::begin(x));
    assert(npos.offset == 1);
    assert(x[0] == 0b10000000000000000000000000000000);
    assert(tuc.decode(pos) == 0);
    pos = npos;
  }

  {
    auto npos{tuc.encode(1, pos)};
    assert(npos.pos == std::begin(x));
    assert(npos.offset == 3);
    assert(x[0] == 0b10100000000000000000000000000000);
    assert(tuc.decode(pos) == 1);
    pos = npos;
  }

  {
    auto npos{tuc.encode(2, pos)};
    assert(npos.pos == std::begin(x));
    assert(npos.offset == 6);
    assert(x[0] == 0b10100100000000000000000000000000);
    assert(tuc.decode(pos) == 2);
    pos = npos;
  }

  {
    auto npos{tuc.encode(3, pos)};
    assert(npos.pos == std::begin(x));
    assert(npos.offset == 10);
    assert(x[0] == 0b10100100010000000000000000000000);
    assert(tuc.decode(pos) == 3);
    pos = npos;
  }

  {
    auto npos{tuc.encode(4, pos)};
    assert(npos.pos == std::begin(x));
    assert(npos.offset == 15);
    assert(x[0] == 0b10100100010000100000000000000000);
    assert(tuc.decode(pos) == 4);
    pos = npos;
  }

  {
    auto npos{tuc.encode(5, pos)};
    assert(npos.pos == std::begin(x));
    assert(npos.offset == 21);
    assert(x[0] == 0b10100100010000100000100000000000);
    assert(tuc.decode(pos) == 5);
    pos = npos;
  }

  {
    auto npos{tuc.encode(6, pos)};
    assert(npos.pos == std::begin(x));
    assert(npos.offset == 28);
    assert(x[0] == 0b10100100010000100000100000010000);
    assert(tuc.decode(pos) == 6);
    pos = npos;
  }

  {
    auto npos{tuc.encode(7, pos)};
    assert(npos.pos == std::begin(x) + 1);
    assert(npos.offset == 4);
    assert(x[0] == 0b10100100010000100000100000010000);
    assert(x[1] == 0b00010000000000000000000000000000);
    assert(tuc.decode(pos) == 7);
    pos = npos;
  }
}

inline void
test_gamma_codec()
{
  eco::gamma_codec<std::uint32_t> tgc;
  uint32_t x[2]{0, 0};

  eco::bit_ptr<uint32_t> pos{std::begin(x), 0};

  {
    auto npos{tgc.encode(1, pos)};
    assert(npos.pos == std::begin(x));
    assert(npos.offset == 1);
    assert(x[0] == 0b10000000000000000000000000000000);
    assert(tgc.decode(pos) == 1);
    pos = npos;
  }

  {
    auto npos{tgc.encode(2, pos)};
    assert(npos.pos == std::begin(x));
    assert(npos.offset == 4);
    assert(x[0] == 0b10100000000000000000000000000000);
    assert(tgc.decode(pos) == 2);
    pos = npos;
  }

  {
    auto npos{tgc.encode(3, pos)};
    assert(npos.pos == std::begin(x));
    assert(npos.offset == 7);
    assert(x[0] == 0b10100110000000000000000000000000);
    assert(tgc.decode(pos) == 3);
    pos = npos;
  }

  {
    auto npos{tgc.encode(4, pos)};
    assert(npos.pos == std::begin(x));
    assert(npos.offset == 12);
    assert(x[0] == 0b10100110010000000000000000000000);
    assert(tgc.decode(pos) == 4);
    pos = npos;
  }

  {
    auto npos{tgc.encode(5, pos)};
    assert(npos.pos == std::begin(x));
    assert(npos.offset == 17);
    assert(x[0] == 0b10100110010000101000000000000000);
    assert(tgc.decode(pos) == 5);
    pos = npos;
  }

  {
    auto npos{tgc.encode(6, pos)};
    assert(npos.pos == std::begin(x));
    assert(npos.offset == 22);
    assert(x[0] == 0b10100110010000101001100000000000);
    assert(tgc.decode(pos) == 6);
    pos = npos;
  }

  {
    auto npos{tgc.encode(7, pos)};
    assert(npos.pos == std::begin(x));
    assert(npos.offset == 27);
    assert(x[0] == 0b10100110010000101001100011100000);
    assert(tgc.decode(pos) == 7);
    pos = npos;
  }

  {
    auto npos{tgc.encode(8, pos)};
    assert(npos.pos == std::begin(x) + 1);
    assert(npos.offset == 2);
    assert(x[0] == 0b10100110010000101001100011100010);
    assert(x[1] == 0b00000000000000000000000000000000);
    assert(tgc.decode(pos) == 8);
    pos = npos;
  }

  {
    auto npos{tgc.encode(9, pos)};
    assert(npos.pos == std::begin(x) + 1);
    assert(npos.offset == 9);
    assert(x[0] == 0b10100110010000101001100011100010);
    assert(x[1] == 0b00000100100000000000000000000000);
    assert(tgc.decode(pos) == 9);
    pos = npos;
  }
}

#endif
