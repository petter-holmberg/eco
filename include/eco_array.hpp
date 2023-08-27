#ifndef ECO_ARRAY_
#define ECO_ARRAY_

#include <algorithm>
#include <cassert>
#include <concepts>
#include <functional>
#include <limits>
#include <ranges>
#include <variant>

#include "eco_allocator.hpp"
#include "eco_extent.hpp"
#include "eco_memory.hpp"
#include "eco_type_traits.hpp"

namespace eco::inline cpp20 {

template
<
  typename T,
  auto ga = default_array_growth,
  auto& alloc = default_array_alloc
>
class array
{
public:
  using value_type = T;
  using ssize_type = ssize_t<memory_view>;

private:
  extent<T, ssize_type, std::monostate, default_array_copy<T>, ga, alloc> header;

  template <typename I, typename S>
  struct writer
  {
    I first;
    S last;

    [[nodiscard]] explicit constexpr
    writer(I f, S l)
      : first{f}, last{l}
    {}

    constexpr void
    operator()(T* dst)
    {
      static_assert(std::indirectly_copyable<T*, T*>);
      default_array_copy<T>.copy(first, last, dst);
    }
  };

  [[nodiscard]] constexpr auto
  is_full() const noexcept -> bool
  {
    return (header.unused_capacity() == 0);
  }

public:
  [[nodiscard]] constexpr
  array() noexcept = default;

  [[nodiscard]] explicit constexpr
  array(ssize_type capacity)
    : header{capacity}
  {}

  template <std::ranges::forward_range R>
    requires
      (!std::same_as<R, array>) &&
      std::constructible_from<T, std::ranges::range_value_t<R>>
  [[nodiscard]] explicit constexpr
  array(R&& range) noexcept(std::is_nothrow_copy_constructible_v<std::ranges::range_value_t<R>>)
    : header{std::ranges::ssize(range)}
  {
    header.insert_space(
      capacity(),
      writer{std::ranges::begin(range), std::ranges::end(range)}
    );
    [[maybe_unused]] post_0: assert(size() == std::ranges::ssize(range));
    [[maybe_unused]] post_1: assert(is_full());
  }

  template <std::ranges::forward_range R>
    requires
      (!std::same_as<R, array>) &&
      std::constructible_from<T, std::ranges::range_value_t<R>>
  constexpr auto
  operator=(R&& range) noexcept(std::is_nothrow_copy_constructible_v<std::ranges::range_value_t<R>>) -> array&
  {
    auto const n_elements{std::ranges::ssize(range)};
    if (capacity() < n_elements) {
      array temp{range};
      swap(temp);
      [[maybe_unused]] invariant_0: assert(is_full());
    } else if (header) {
      header.erase_space(header.begin(), header.size());
      header.insert_space(n_elements, writer{std::ranges::begin(range), std::ranges::end(range)});
      [[maybe_unused]] invariant_1: assert(capacity() >= size());
    }
    return *this;
  }

  [[nodiscard]] friend constexpr auto
  operator==(array const& x, array const& y) noexcept -> bool
    requires std::indirectly_comparable<T const*, T const*, std::ranges::equal_to>
  {
    return x.header == y.header;
  }

  [[nodiscard]] friend constexpr auto
  operator!=(array const& x, array const& y) noexcept -> bool
    requires std::indirectly_comparable<T const*, T const*, std::ranges::equal_to>
  {
    return x.header != y.header;
  }

  [[nodiscard]] friend constexpr auto
  operator<(array const& x, array const& y) noexcept -> bool
    requires std::indirect_strict_weak_order<std::ranges::less, T const*>
  {
    return x.header < y.header;
  }

  [[nodiscard]] friend constexpr auto
  operator>=(array const& x, array const& y) noexcept -> bool
    requires std::indirect_strict_weak_order<std::ranges::less, T const*>
  {
    return x.header >= y.header;
  }

  [[nodiscard]] friend constexpr auto
  operator>(array const& x, array const& y) noexcept -> bool
    requires std::indirect_strict_weak_order<std::ranges::less, T const*>
  {
    return x.header > y.header;
  }

  [[nodiscard]] friend constexpr auto
  operator<=(array const& x, array const& y) noexcept -> bool
    requires std::indirect_strict_weak_order<std::ranges::less, T const*>
  {
    return x.header <= y.header;
  }

  constexpr void
  swap(array& x) noexcept
  {
    header.swap(x.header);
  }

  [[nodiscard]] explicit constexpr
  operator bool() const noexcept
  {
    return static_cast<bool>(header);
  }

  [[nodiscard]] constexpr auto
  operator[](ssize_type n) noexcept -> T&
  {
    [[maybe_unused]] pre: assert(n < size());
    return *(header.begin() + n);
  }

  [[nodiscard]] constexpr auto
  operator[](ssize_type n) const noexcept -> T const&
  {
    [[maybe_unused]] pre: assert(n < size());
    return *(header.begin() + n);
  }

  [[nodiscard]] constexpr auto
  begin() noexcept -> T*
  {
    return header.begin();
  }

  [[nodiscard]] constexpr auto
  begin() const noexcept -> T const*
  {
    return header.begin();
  }

  [[nodiscard]] constexpr auto
  end() noexcept -> T*
  {
    return header.end();
  }

  [[nodiscard]] constexpr auto
  end() const noexcept -> T const*
  {
    return header.end();
  }

  [[nodiscard]] constexpr auto
  size() const noexcept -> ssize_type
  {
    return header.size();
  }

  [[nodiscard]] constexpr auto
  capacity() const noexcept -> ssize_type
  {
    return header.capacity();
  }

  [[nodiscard]] constexpr auto
  max_size() const noexcept -> ssize_type
  {
    return std::numeric_limits<ssize_type>::max() / sizeof(T);
  }

  constexpr void
  reserve(ssize_type cap)
  {
    if (capacity() < cap) {
      header.adjust_unused_capacity(cap - size());
    }
    [[maybe_unused]] post: assert(capacity() >= cap);
  }

  constexpr void
  shrink_to_fit()
  {
    if (!is_full()) {
      if (*this) {
        header.adjust_unused_capacity(0);
      } else {
        header = {};
      }
    }
    [[maybe_unused]] post: assert(is_full());
  }

  template <typename... Args>
    requires std::constructible_from<T, Args...>
  constexpr auto
  push_back(Args &&...args) -> T&
  {
    return header.push_back(std::forward<Args>(args)...);
    [[maybe_unused]] post: assert(bool{*this});
  }

  constexpr void
  pop_back() noexcept
  {
    [[maybe_unused]] pre: assert(bool{*this});
    header.pop_back();
  }

  template <std::ranges::forward_range R>
    requires std::constructible_from<T, std::ranges::range_value_t<R>>
  constexpr auto
  append(R&& range) -> T*
  {
    // pre axiom: "range not overlapped with [begin(), end())"
    auto const n_elements{std::ranges::distance(range)};
    reserve(size() + n_elements);
    return header.insert_space(n_elements, writer{std::ranges::begin(range), std::ranges::end(range)});
  }

  template <typename... Args>
    requires std::constructible_from<T, Args...>
  constexpr auto
  insert(T const* pos, Args &&...args) -> T*
  {
    // pre axiom: "pos within [begin(), end()]"
    auto df{pos - begin()};
    auto dm{size()};
    push_back(std::forward<Args>(args)...);
    return std::rotate(begin() + df, begin() + dm, end());
    // post axiom: bool{*this}
  }

  template <std::ranges::forward_range R>
    requires std::constructible_from<T, std::ranges::range_value_t<R>>
  constexpr auto
  insert(T const* pos, R&& range) -> T*
  {
    // pre axiom: "pos within [begin(), end()]"
    // pre axiom: "range not overlapped with [begin(), end())"
    auto const df{pos - begin()};
    auto const dm{size()};
    append(std::forward<R>(range));
    return std::rotate(begin() + df, begin() + dm, end());
  }

  constexpr auto
  erase(T const* pos) noexcept -> T*
  {
    [[maybe_unused]] pre: assert(bool{*this});
    // pre axiom: "pos within [begin(), end())"
    header.erase_space(const_cast<T*>(pos), 1);
    return const_cast<T*>(pos);
    // post axiom: "capacity() unchanged"
  }

  template <std::ranges::range R>
    requires std::same_as<T, std::ranges::range_value_t<R>>
  constexpr auto
  erase(R&& range) noexcept -> T*
  {
    // pre axiom: "range within [begin(), end())"
    auto const pos{std::ranges::begin(range)};
    header.erase_space(pos, std::ranges::ssize(range));
    return pos;
    // post axiom: "capacity() unchanged"
  }

  constexpr void
  clear() noexcept
  {
    header.erase_space(begin(), size());
    // post axiom: "capacity() unchanged"
  }
};

template <std::ranges::forward_range R>
array(R&& range)->array<std::ranges::range_value_t<R>>;

static_assert(std::totally_ordered<array<int>>);
static_assert(std::ranges::contiguous_range<array<int>>);

template <typename T, auto ga, auto& alloc>
constexpr void
swap(array<T, ga, alloc>& x, array<T, ga, alloc>& y) noexcept
{
  x.swap(y);
}

template <std::copy_constructible T, auto ga, auto& alloc>
constexpr void
resize(array<T, ga, alloc>& x, ssize_t<array<T, ga, alloc>> size, value_t<array<T, ga, alloc>> const& value = T{})
{
  if (x.size() < size) {
    x.reserve(size);
    size = size - x.size();
    while (size != 0) {
      x.push_back(value);
      --size;
    }
  } else if (x.size() > size) {
    size = x.size() - size;
    while (size != 0) {
      x.pop_back();
      --size;
    }
  }
}

}

namespace std {

template <typename T, auto ga, auto& alloc>
constexpr void
swap(eco::array<T, ga, alloc>& x, eco::array<T, ga, alloc>& y) noexcept
{
  using eco::swap;
  swap(x, y);
}

}

#endif
