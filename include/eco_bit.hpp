#ifndef ECO_BIT_
#define ECO_BIT_

#include <bit>
#include <cassert>
#include <climits>
#include <concepts>
#include <cstdint>

namespace eco::inline cpp20 {

/*export*/ template <typename T>
inline constexpr int bit_size_v = CHAR_BIT * sizeof(T);

template <std::unsigned_integral T>
[[nodiscard]] constexpr auto
bit_read(std::uint8_t i, T x) noexcept -> T
{
  [[maybe_unused]] pre: assert(i < bit_size_v<T>);
  return x >> i & 1;
}

template <std::unsigned_integral T>
[[nodiscard]] constexpr auto
bit_set(std::uint8_t i, T& x) noexcept
{
  [[maybe_unused]] pre: assert(i < bit_size_v<T>);
  x |= T{1} << i;
}

template <std::unsigned_integral T>
[[nodiscard]] constexpr auto
bit_reset(std::uint8_t i, T& x) noexcept
{
  [[maybe_unused]] pre: assert(i < bit_size_v<T>);
  x &= ~(T{1} << i);
}

template <std::unsigned_integral T>
[[nodiscard]] constexpr auto
bit_flip(std::uint8_t i, T& x) noexcept
{
  [[maybe_unused]] pre: assert(i < bit_size_v<T>);
  x ^= T{1} << i;
}

template <std::uint8_t n, std::uint8_t i, std::unsigned_integral T>
requires (n > 0 && i < bit_size_v<T> && i + n <= bit_size_v<T>)
[[nodiscard]] constexpr auto
bits_read(T x) noexcept -> T
{
  if constexpr (n == 1) {
    return x >> i & 1;
  } else {
    return x >> i & ((T{1} << n) - 1);
  }
}

template <std::uint8_t n, std::unsigned_integral T>
requires (n > 0 && n <= bit_size_v<T>)
[[nodiscard]] constexpr auto
bits_read(std::uint8_t i, T x) noexcept -> T
{
  [[maybe_unused]] pre:
    assert(i < bit_size_v<T>);
    assert(i + n <= bit_size_v<T>);
  if constexpr (n == 1) {
    return x >> i & 1;
  } else {
    return x >> i & ((T{1} << n) - 1);
  }
}

template <std::uint8_t n, std::unsigned_integral T>
requires (n > 0 && n <= bit_size_v<T>)
constexpr void
bits_write(std::uint8_t i, T& x, T value) noexcept
{
  [[maybe_unused]] pre:
    assert(i < bit_size_v<T>);
    assert(i + n <= bit_size_v<T>);
  x &= ~(((T{1} << n) - 1) << i);
  x |= value << i;
}

template <std::unsigned_integral T>
[[nodiscard]] constexpr auto
bits_read(std::uint8_t n, std::uint8_t i, T x) noexcept -> T
{
  [[maybe_unused]] pre:
    assert(n > 0);
    assert(i < bit_size_v<T>);
    assert(i + n <= bit_size_v<T>);
  return x >> i & ((T{1} << n) - 1);
}

template <std::uint8_t n, std::uint8_t i, std::unsigned_integral T>
requires (n > 0 && n <= bit_size_v<T> && i < bit_size_v<T> && i + n > bit_size_v<T>)
[[nodiscard]] constexpr auto
bits_read_straddled(T x, T y) noexcept -> T
{
  return (x >> i) | (y & ((T{1} << ((i + n) % bit_size_v<T>)) - 1)) << (bit_size_v<T> - i);
}

template <std::uint8_t n, std::unsigned_integral T>
requires (n > 0 && n <= bit_size_v<T>)
[[nodiscard]] constexpr auto
bits_read_straddled(std::uint8_t i, T x, T y) noexcept -> T
{
  [[maybe_unused]] pre:
    assert(i < bit_size_v<T>);
    assert(i + n > bit_size_v<T>);
  return (x >> i) | (y & ((T{1} << ((i + n) % bit_size_v<T>)) - 1)) << (bit_size_v<T> - i);
}

template <std::uint8_t n, std::unsigned_integral T>
requires (n > 1 && n <= bit_size_v<T>)
constexpr void
bits_write_straddled(std::uint8_t i, T& x, T&y, T value) noexcept
{
  [[maybe_unused]] pre:
    assert(i < bit_size_v<T>);
    assert(i + n > bit_size_v<T>);
  x = (x & ((T{1} << i) - 1)) | (value << i);
  y = (y & ~((T{1} << ((i + n) % bit_size_v<T>)) - 1)) | (value >> (bit_size_v<T> - i));
}

}

#endif
