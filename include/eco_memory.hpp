#ifndef ECO_MEMORY_
#define ECO_MEMORY_

#include <bit>
#include <concepts>
#include <cstddef>
#include <ranges>

namespace eco::inline cpp20 {

struct memory_view : public std::ranges::view_interface<memory_view>
{
  using ssize_type = std::ptrdiff_t;

  void* first{};
  ssize_type size{};

  [[nodiscard]] constexpr
  memory_view() noexcept = default;

  [[nodiscard]] constexpr
  memory_view(void* f, ssize_type s) noexcept
    : first{f}
    , size{s}
  {}

  [[nodiscard]] explicit constexpr
  operator bool() const noexcept
  {
    return first != nullptr;
  }

  [[nodiscard]] constexpr auto
  begin() const noexcept -> std::byte*
  {
    return std::bit_cast<std::byte*>(first);
  }

  [[nodiscard]] constexpr auto
  end() const noexcept -> std::byte*
  {
    return std::bit_cast<std::byte*>(first) + size;
  }

  [[nodiscard]] constexpr auto
  begins_in(memory_view x) const noexcept -> bool
  {
    return !(std::less<>{}(begin(), x.begin())) && std::less<>{}(begin(), x.end());
  }

  [[nodiscard]] constexpr auto
  ends_in(memory_view x) const noexcept -> bool
  {
    return std::less<>{}(x.begin(), end()) && !(std::less<>{}(x.end(), end()));
  }

  [[nodiscard]] constexpr auto
  is_in(memory_view x) const noexcept -> bool
  {
    return begins_in(x) && ends_in(x);
  }
};

static_assert(std::ranges::view<memory_view>);

[[nodiscard]] constexpr auto
operator==(memory_view x, memory_view y) noexcept -> bool
{
  return x.first == y.first && x.size == y.size;
}

[[nodiscard]] constexpr auto
operator!=(memory_view x, memory_view y) noexcept -> bool
{
  return !(x == y);
}

[[nodiscard]] constexpr auto
operator<(memory_view x, memory_view y) noexcept -> bool
{
  return std::less<>{}(x.first, y.first) || (!(std::less<>{}(y.first, x.first)) && (x.size < y.size));
}

[[nodiscard]] constexpr auto
operator>=(memory_view x, memory_view y) noexcept -> bool
{
  return !(x < y);
}

[[nodiscard]] constexpr auto
operator>(memory_view x, memory_view y) noexcept -> bool
{
  return y < x;
}

[[nodiscard]] constexpr auto
operator<=(memory_view x, memory_view y) noexcept -> bool
{
  return !(y < x);
}

static_assert(std::totally_ordered<memory_view>);
static_assert(std::ranges::contiguous_range<memory_view>);

}

#endif
