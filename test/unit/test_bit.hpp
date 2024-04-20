#ifndef ECO_TEST_BIT_
#define ECO_TEST_BIT_

import std;
import eco;

#include <cassert>

inline void
test_bit()
{
  {
    unsigned char x{0b00000000};
    unsigned char y{0b00000001};
    unsigned char z{0b00000101};
    assert(eco::bit_read(x, 0) == 0);
    assert(eco::bit_read(y, 0) == 1);
    assert(eco::bit_read(z, 0) == 1);
    assert(eco::bit_read(x, 1) == 0);
    assert(eco::bit_read(y, 1) == 0);
    assert(eco::bit_read(z, 1) == 0);
    assert(eco::bit_read(x, 2) == 0);
    assert(eco::bit_read(y, 2) == 0);
    assert(eco::bit_read(z, 2) == 1);
  }

  {
    unsigned char x{0b00000000};
    eco::bit_set(x, 0);
    assert(x == 0b00000001);
    eco::bit_set(x, 2);
    assert(x == 0b00000101);
    eco::bit_set(x, 7);
    assert(x == 0b10000101);
  }

  {
    unsigned char x{0b11111111};
    eco::bit_clear(x, 0);
    assert(x == 0b11111110);
    eco::bit_clear(x, 2);
    assert(x == 0b11111010);
    eco::bit_clear(x, 7);
    assert(x == 0b01111010);
  }

  {
    unsigned char x{0b00000000};
    eco::bit_flip(x, 0);
    assert(x == 0b00000001);
    eco::bit_flip(x, 2);
    assert(x == 0b00000101);
    eco::bit_flip(x, 7);
    assert(x == 0b10000101);
    eco::bit_flip(x, 7);
    assert(x == 0b00000101);
    eco::bit_flip(x, 2);
    assert(x == 0b00000001);
    eco::bit_flip(x, 0);
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

    assert((eco::bits_read<1>(x, 0)) == 0);
    assert((eco::bits_read<1>(y, 0)) == 1);
    assert((eco::bits_read<1>(z, 0)) == 1);
    assert((eco::bits_read<1>(x, 1)) == 0);
    assert((eco::bits_read<1>(y, 1)) == 0);
    assert((eco::bits_read<1>(z, 1)) == 0);
    assert((eco::bits_read<1>(x, 2)) == 0);
    assert((eco::bits_read<1>(y, 2)) == 0);
    assert((eco::bits_read<1>(z, 2)) == 1);
    assert((eco::bits_read<2>(x, 0)) == 0);
    assert((eco::bits_read<2>(y, 0)) == 1);
    assert((eco::bits_read<2>(z, 0)) == 1);
    assert((eco::bits_read<2>(x, 1)) == 0);
    assert((eco::bits_read<2>(y, 1)) == 0);
    assert((eco::bits_read<2>(z, 1)) == 2);
    assert((eco::bits_read<2>(x, 2)) == 0);
    assert((eco::bits_read<2>(y, 2)) == 0);
    assert((eco::bits_read<2>(z, 2)) == 1);
    assert((eco::bits_read<3>(x, 0)) == 0);
    assert((eco::bits_read<3>(y, 0)) == 1);
    assert((eco::bits_read<3>(z, 0)) == 5);
    assert((eco::bits_read<3>(x, 1)) == 0);
    assert((eco::bits_read<3>(y, 1)) == 0);
    assert((eco::bits_read<3>(z, 1)) == 2);
    assert((eco::bits_read<3>(x, 2)) == 0);
    assert((eco::bits_read<3>(y, 2)) == 0);
    assert((eco::bits_read<3>(z, 2)) == 1);

    assert((eco::bits_read(x, 1, 0)) == 0);
    assert((eco::bits_read(y, 1, 0)) == 1);
    assert((eco::bits_read(z, 1, 0)) == 1);
    assert((eco::bits_read(x, 1, 1)) == 0);
    assert((eco::bits_read(y, 1, 1)) == 0);
    assert((eco::bits_read(z, 1, 1)) == 0);
    assert((eco::bits_read(x, 1, 2)) == 0);
    assert((eco::bits_read(y, 1, 2)) == 0);
    assert((eco::bits_read(z, 1, 2)) == 1);
    assert((eco::bits_read(x, 2, 0)) == 0);
    assert((eco::bits_read(y, 2, 0)) == 1);
    assert((eco::bits_read(z, 2, 0)) == 1);
    assert((eco::bits_read(x, 2, 1)) == 0);
    assert((eco::bits_read(y, 2, 1)) == 0);
    assert((eco::bits_read(z, 2, 1)) == 2);
    assert((eco::bits_read(x, 2, 2)) == 0);
    assert((eco::bits_read(y, 2, 2)) == 0);
    assert((eco::bits_read(z, 2, 2)) == 1);
    assert((eco::bits_read(x, 3, 0)) == 0);
    assert((eco::bits_read(y, 3, 0)) == 1);
    assert((eco::bits_read(z, 3, 0)) == 5);
    assert((eco::bits_read(x, 3, 1)) == 0);
    assert((eco::bits_read(y, 3, 1)) == 0);
    assert((eco::bits_read(z, 3, 1)) == 2);
    assert((eco::bits_read(x, 3, 2)) == 0);
    assert((eco::bits_read(y, 3, 2)) == 0);
    assert((eco::bits_read(z, 3, 2)) == 1);
  }

  {
    unsigned char x{0b00000000};
    eco::bits_write<1>(x, static_cast<unsigned char>(1), 0);
    assert(x == 0b00000001);
    eco::bits_write<1>(x, static_cast<unsigned char>(1), 2);
    assert(x == 0b00000101);
    eco::bits_write<1>(x, static_cast<unsigned char>(1), 7);
    assert(x == 0b10000101);
    eco::bits_write<2>(x, static_cast<unsigned char>(2), 0);
    assert(x == 0b10000110);
    eco::bits_write<2>(x, static_cast<unsigned char>(2), 2);
    assert(x == 0b10001010);
    eco::bits_write<2>(x, static_cast<unsigned char>(1), 6);
    assert(x == 0b01001010);
    eco::bits_write<5>(x, static_cast<unsigned char>(21), 0);
    assert(x == 0b01010101);
    eco::bits_write<5>(x, static_cast<unsigned char>(10), 2);
    assert(x == 0b00101001);
    eco::bits_write<5>(x, static_cast<unsigned char>(31), 3);
    assert(x == 0b11111001);

    x = 0b00000000;
    eco::bits_write(x, static_cast<unsigned char>(1), 1, 0);
    assert(x == 0b00000001);
    eco::bits_write(x, static_cast<unsigned char>(1), 1, 2);
    assert(x == 0b00000101);
    eco::bits_write(x, static_cast<unsigned char>(1), 1, 7);
    assert(x == 0b10000101);
    eco::bits_write(x, static_cast<unsigned char>(2), 2, 0);
    assert(x == 0b10000110);
    eco::bits_write(x, static_cast<unsigned char>(2), 2, 2);
    assert(x == 0b10001010);
    eco::bits_write(x, static_cast<unsigned char>(1), 2, 6);
    assert(x == 0b01001010);
    eco::bits_write(x, static_cast<unsigned char>(21), 5, 0);
    assert(x == 0b01010101);
    eco::bits_write(x, static_cast<unsigned char>(10), 5, 2);
    assert(x == 0b00101001);
    eco::bits_write(x, static_cast<unsigned char>(31), 5, 3);
    assert(x == 0b11111001);
  }

  {
    unsigned char x{0b00000000};
    unsigned char y{0b00000001};
    unsigned char z{0b00000101};

    assert((eco::bits_read_straddled<2, 7>(x, y)) == 2);
    assert((eco::bits_read_straddled<3, 7>(x, y)) == 2);
    assert((eco::bits_read_straddled<4, 6>(x, y)) == 4);
    assert((eco::bits_read_straddled<5, 5>(x, y)) == 8);
    assert((eco::bits_read_straddled<6, 6>(x, y)) == 4);
    assert((eco::bits_read_straddled<6, 7>(x, y)) == 2);
    assert((eco::bits_read_straddled<2, 7>(x, z)) == 2);
    assert((eco::bits_read_straddled<3, 7>(x, z)) == 2);
    assert((eco::bits_read_straddled<4, 6>(x, z)) == 4);
    assert((eco::bits_read_straddled<5, 5>(x, z)) == 8);
    assert((eco::bits_read_straddled<6, 6>(x, z)) == 20);
    assert((eco::bits_read_straddled<6, 7>(x, z)) == 10);

    assert((eco::bits_read_straddled<2>(x, y, 7)) == 2);
    assert((eco::bits_read_straddled<3>(x, y, 7)) == 2);
    assert((eco::bits_read_straddled<4>(x, y, 6)) == 4);
    assert((eco::bits_read_straddled<5>(x, y, 5)) == 8);
    assert((eco::bits_read_straddled<6>(x, y, 6)) == 4);
    assert((eco::bits_read_straddled<6>(x, y, 7)) == 2);
    assert((eco::bits_read_straddled<2>(x, z, 7)) == 2);
    assert((eco::bits_read_straddled<3>(x, z, 7)) == 2);
    assert((eco::bits_read_straddled<4>(x, z, 6)) == 4);
    assert((eco::bits_read_straddled<5>(x, z, 5)) == 8);
    assert((eco::bits_read_straddled<6>(x, z, 6)) == 20);
    assert((eco::bits_read_straddled<6>(x, z, 7)) == 10);

    assert((eco::bits_read_straddled(x, y, 2, 7)) == 2);
    assert((eco::bits_read_straddled(x, y, 3, 7)) == 2);
    assert((eco::bits_read_straddled(x, y, 4, 6)) == 4);
    assert((eco::bits_read_straddled(x, y, 5, 5)) == 8);
    assert((eco::bits_read_straddled(x, y, 6, 6)) == 4);
    assert((eco::bits_read_straddled(x, y, 7, 7)) == 2);
    assert((eco::bits_read_straddled(x, z, 2, 7)) == 2);
    assert((eco::bits_read_straddled(x, z, 3, 7)) == 2);
    assert((eco::bits_read_straddled(x, z, 4, 6)) == 4);
    assert((eco::bits_read_straddled(x, z, 5, 5)) == 8);
    assert((eco::bits_read_straddled(x, z, 6, 6)) == 20);
    assert((eco::bits_read_straddled(x, z, 6, 7)) == 10);
  }

  {
    unsigned char x{0b00000000};
    unsigned char y{0b00000000};
    eco::bits_write_straddled<2>(x, y, static_cast<unsigned char>(1), 7);
    assert(x == 0b10000000);
    assert(y == 0b00000000);
    eco::bits_write_straddled<2>(x, y, static_cast<unsigned char>(3), 7);
    assert(x == 0b10000000);
    assert(y == 0b00000001);
    eco::bits_write_straddled<5>(x, y, static_cast<unsigned char>(21), 5);
    assert(x == 0b10100000);
    assert(y == 0b00000010);
    eco::bits_write_straddled<5>(x, y, static_cast<unsigned char>(21), 6);
    assert(x == 0b01100000);
    assert(y == 0b00000101);

    x = 0b00000000;
    y = 0b00000000;
    eco::bits_write_straddled(x, y, static_cast<unsigned char>(1), 2, 7);
    assert(x == 0b10000000);
    assert(y == 0b00000000);
    eco::bits_write_straddled(x, y, static_cast<unsigned char>(3), 2, 7);
    assert(x == 0b10000000);
    assert(y == 0b00000001);
    eco::bits_write_straddled(x, y, static_cast<unsigned char>(21), 5, 5);
    assert(x == 0b10100000);
    assert(y == 0b00000010);
    eco::bits_write_straddled(x, y, static_cast<unsigned char>(21), 5, 6);
    assert(x == 0b01100000);
    assert(y == 0b00000101);
  }

  {
    assert(eco::mark_ls<unsigned char>(0) == 0b00000000);
    assert(eco::mark_ls<unsigned char>(1) == 0b00000001);
    assert(eco::mark_ls<unsigned char>(2) == 0b00000011);
    assert(eco::mark_ls<unsigned char>(3) == 0b00000111);
    assert(eco::mark_ls<unsigned char>(4) == 0b00001111);
    assert(eco::mark_ls<unsigned char>(5) == 0b00011111);
    assert(eco::mark_ls<unsigned char>(6) == 0b00111111);
    assert(eco::mark_ls<unsigned char>(7) == 0b01111111);
    assert(eco::mark_ls<unsigned char>(8) == 0b11111111);
  }

  {
    unsigned char x{0b01011001};
    assert(eco::rank_1(x) == 4);
    assert(eco::rank_1(x, 0) == 0);
    assert(eco::rank_1(x, 1) == 1);
    assert(eco::rank_1(x, 2) == 1);
    assert(eco::rank_1(x, 3) == 1);
    assert(eco::rank_1(x, 4) == 2);
    assert(eco::rank_1(x, 5) == 3);
    assert(eco::rank_1(x, 6) == 3);
    assert(eco::rank_1(x, 7) == 4);
    assert(eco::rank_1(x, 8) == 4);

    assert(eco::rank_0(x) == 4);
    assert(eco::rank_0(x, 0) == 0);
    assert(eco::rank_0(x, 1) == 0);
    assert(eco::rank_0(x, 2) == 1);
    assert(eco::rank_0(x, 3) == 2);
    assert(eco::rank_0(x, 4) == 2);
    assert(eco::rank_0(x, 5) == 2);
    assert(eco::rank_0(x, 6) == 3);
    assert(eco::rank_0(x, 7) == 3);
    assert(eco::rank_0(x, 8) == 4);
  }

  {
    unsigned char x{0b01011001};

    assert(eco::mark_ls_1(x) == 0b00000001);
    x = eco::clear_ls_1(x);
    assert(x == 0b01011000);
    assert(eco::mark_ls_1(x) == 0b00001000);
    x = eco::clear_ls_1(x);
    assert(x == 0b01010000);
    assert(eco::mark_ls_1(x) == 0b00010000);
    x = eco::clear_ls_1(x);
    assert(x == 0b01000000);
    assert(eco::mark_ls_1(x) == 0b01000000);
    x = eco::clear_ls_1(x);
    assert(x == 0b00000000);
    assert(eco::mark_ls_1(x) == 0b00000000);
    x = eco::clear_ls_1(x);
    assert(x == 0b00000000);
  }

  {
    unsigned char x{0b01011001};
    assert(eco::select_1(x, 1) == 0);
    assert(eco::select_1(x, 2) == 3);
    assert(eco::select_1(x, 3) == 4);
    assert(eco::select_1(x, 4) == 6);

    assert(eco::select_0(x, 1) == 1);
    assert(eco::select_0(x, 2) == 2);
    assert(eco::select_0(x, 3) == 5);
    assert(eco::select_0(x, 4) == 7);
  }
}

#endif
