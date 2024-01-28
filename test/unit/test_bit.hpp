#ifndef ECO_TEST_BIT_
#define ECO_TEST_BIT_

#include <cassert>

#include "eco_bit.hpp"

inline void
test_bit()
{
  {
    unsigned char x{0b00000000};
    unsigned char y{0b00000001};
    unsigned char z{0b00000101};
    assert(eco::bit_read(0, x) == 0);
    assert(eco::bit_read(0, y) == 1);
    assert(eco::bit_read(0, z) == 1);
    assert(eco::bit_read(1, x) == 0);
    assert(eco::bit_read(1, y) == 0);
    assert(eco::bit_read(1, z) == 0);
    assert(eco::bit_read(2, x) == 0);
    assert(eco::bit_read(2, y) == 0);
    assert(eco::bit_read(2, z) == 1);
  }

  {
    unsigned char x{0b00000000};
    eco::bit_set(0, x);
    assert(x == 0b00000001);
    eco::bit_set(2, x);
    assert(x == 0b00000101);
    eco::bit_set(7, x);
    assert(x == 0b10000101);
  }

  {
    unsigned char x{0b11111111};
    eco::bit_reset(0, x);
    assert(x == 0b11111110);
    eco::bit_reset(2, x);
    assert(x == 0b11111010);
    eco::bit_reset(7, x);
    assert(x == 0b01111010);
  }

  {
    unsigned char x{0b00000000};
    eco::bit_flip(0, x);
    assert(x == 0b00000001);
    eco::bit_flip(2, x);
    assert(x == 0b00000101);
    eco::bit_flip(7, x);
    assert(x == 0b10000101);
    eco::bit_flip(7, x);
    assert(x == 0b00000101);
    eco::bit_flip(2, x);
    assert(x == 0b00000001);
    eco::bit_flip(0, x);
    assert(x == 0b00000000);
  }

  {
    unsigned char x{0b00000000};
    unsigned char y{0b00000001};
    unsigned char z{0b00000101};
    assert((eco::bits_read<1, 0>(x)) == 0);
    assert((eco::bits_read<1, 0>(y)) == 1);
    assert((eco::bits_read<1, 0>(z)) == 1);
    assert((eco::bits_read<1, 1>(x)) == 0);
    assert((eco::bits_read<1, 1>(y)) == 0);
    assert((eco::bits_read<1, 1>(z)) == 0);
    assert((eco::bits_read<1, 2>(x)) == 0);
    assert((eco::bits_read<1, 2>(y)) == 0);
    assert((eco::bits_read<1, 2>(z)) == 1);
    assert((eco::bits_read<2, 0>(x)) == 0);
    assert((eco::bits_read<2, 0>(y)) == 1);
    assert((eco::bits_read<2, 0>(z)) == 1);
    assert((eco::bits_read<2, 1>(x)) == 0);
    assert((eco::bits_read<2, 1>(y)) == 0);
    assert((eco::bits_read<2, 1>(z)) == 2);
    assert((eco::bits_read<2, 2>(x)) == 0);
    assert((eco::bits_read<2, 2>(y)) == 0);
    assert((eco::bits_read<2, 2>(z)) == 1);
    assert((eco::bits_read<3, 0>(x)) == 0);
    assert((eco::bits_read<3, 0>(y)) == 1);
    assert((eco::bits_read<3, 0>(z)) == 5);
    assert((eco::bits_read<3, 1>(x)) == 0);
    assert((eco::bits_read<3, 1>(y)) == 0);
    assert((eco::bits_read<3, 1>(z)) == 2);
    assert((eco::bits_read<3, 2>(x)) == 0);
    assert((eco::bits_read<3, 2>(y)) == 0);
    assert((eco::bits_read<3, 2>(z)) == 1);

    assert((eco::bits_read<1>(0, x)) == 0);
    assert((eco::bits_read<1>(0, y)) == 1);
    assert((eco::bits_read<1>(0, z)) == 1);
    assert((eco::bits_read<1>(1, x)) == 0);
    assert((eco::bits_read<1>(1, y)) == 0);
    assert((eco::bits_read<1>(1, z)) == 0);
    assert((eco::bits_read<1>(2, x)) == 0);
    assert((eco::bits_read<1>(2, y)) == 0);
    assert((eco::bits_read<1>(2, z)) == 1);
    assert((eco::bits_read<2>(0, x)) == 0);
    assert((eco::bits_read<2>(0, y)) == 1);
    assert((eco::bits_read<2>(0, z)) == 1);
    assert((eco::bits_read<2>(1, x)) == 0);
    assert((eco::bits_read<2>(1, y)) == 0);
    assert((eco::bits_read<2>(1, z)) == 2);
    assert((eco::bits_read<2>(2, x)) == 0);
    assert((eco::bits_read<2>(2, y)) == 0);
    assert((eco::bits_read<2>(2, z)) == 1);
    assert((eco::bits_read<3>(0, x)) == 0);
    assert((eco::bits_read<3>(0, y)) == 1);
    assert((eco::bits_read<3>(0, z)) == 5);
    assert((eco::bits_read<3>(1, x)) == 0);
    assert((eco::bits_read<3>(1, y)) == 0);
    assert((eco::bits_read<3>(1, z)) == 2);
    assert((eco::bits_read<3>(2, x)) == 0);
    assert((eco::bits_read<3>(2, y)) == 0);
    assert((eco::bits_read<3>(2, z)) == 1);

    assert((eco::bits_read(1, 0, x)) == 0);
    assert((eco::bits_read(1, 0, y)) == 1);
    assert((eco::bits_read(1, 0, z)) == 1);
    assert((eco::bits_read(1, 1, x)) == 0);
    assert((eco::bits_read(1, 1, y)) == 0);
    assert((eco::bits_read(1, 1, z)) == 0);
    assert((eco::bits_read(1, 2, x)) == 0);
    assert((eco::bits_read(1, 2, y)) == 0);
    assert((eco::bits_read(1, 2, z)) == 1);
    assert((eco::bits_read(2, 0, x)) == 0);
    assert((eco::bits_read(2, 0, y)) == 1);
    assert((eco::bits_read(2, 0, z)) == 1);
    assert((eco::bits_read(2, 1, x)) == 0);
    assert((eco::bits_read(2, 1, y)) == 0);
    assert((eco::bits_read(2, 1, z)) == 2);
    assert((eco::bits_read(2, 2, x)) == 0);
    assert((eco::bits_read(2, 2, y)) == 0);
    assert((eco::bits_read(2, 2, z)) == 1);
    assert((eco::bits_read(3, 0, x)) == 0);
    assert((eco::bits_read(3, 0, y)) == 1);
    assert((eco::bits_read(3, 0, z)) == 5);
    assert((eco::bits_read(3, 1, x)) == 0);
    assert((eco::bits_read(3, 1, y)) == 0);
    assert((eco::bits_read(3, 1, z)) == 2);
    assert((eco::bits_read(3, 2, x)) == 0);
    assert((eco::bits_read(3, 2, y)) == 0);
    assert((eco::bits_read(3, 2, z)) == 1);
  }

  {
    unsigned char x{0b00000000};
    eco::bits_write<1>(0, x, static_cast<unsigned char>(1));
    assert(x == 0b00000001);
    eco::bits_write<1>(2, x, static_cast<unsigned char>(1));
    assert(x == 0b00000101);
    eco::bits_write<1>(7, x, static_cast<unsigned char>(1));
    assert(x == 0b10000101);
    eco::bits_write<2>(0, x, static_cast<unsigned char>(2));
    assert(x == 0b10000110);
    eco::bits_write<2>(2, x, static_cast<unsigned char>(2));
    assert(x == 0b10001010);
    eco::bits_write<2>(6, x, static_cast<unsigned char>(1));
    assert(x == 0b01001010);
    eco::bits_write<5>(0, x, static_cast<unsigned char>(21));
    assert(x == 0b01010101);
    eco::bits_write<5>(2, x, static_cast<unsigned char>(10));
    assert(x == 0b00101001);
    eco::bits_write<5>(3, x, static_cast<unsigned char>(31));
    assert(x == 0b11111001);
  }

  {
    unsigned char x{0b00000000};
    unsigned char y{0b00000000};
    eco::bits_write_straddled<2>(7, x, y, static_cast<unsigned char>(1));
    assert(x == 0b10000000);
    assert(y == 0b00000000);
    eco::bits_write_straddled<2>(7, x, y, static_cast<unsigned char>(3));
    assert(x == 0b10000000);
    assert(y == 0b00000001);
    eco::bits_write_straddled<5>(5, x, y, static_cast<unsigned char>(21));
    assert(x == 0b10100000);
    assert(y == 0b00000010);
    eco::bits_write_straddled<5>(6, x, y, static_cast<unsigned char>(21));
    assert(x == 0b01100000);
    assert(y == 0b00000101);
  }
}

#endif
