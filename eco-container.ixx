module;

#include <concepts>
#include <cstddef>
#include <iterator>
#include <memory>
#include <ranges>
#include <utility>
#include <variant>

export module eco:container;
import :sequence;

namespace eco {

inline namespace cpp20 {

export template
<
  typename T,
  auto ga = default_array_growth,
  auto& alloc = default_array_alloc
>
class array
{
public:
  using value_type = T;
  using size_type = ssize_type_t<memory_view>;

private:
  extent<T, size_type, std::monostate, default_array_copy<T>, ga, alloc> header;

  template <typename I, typename S>
  struct writer
  {
    I first;
    S last;

    [[nodiscard]] explicit
    writer(I f, S l)
      : first{f}, last{l}
    {}

    void
    operator()(T* dst)
      requires std::indirectly_copyable<T*, T*>
    {
      default_array_copy<T>.copy(first, last, dst);
    }
  };

  template <typename I, typename S>
  writer(I f, S l)->writer<I, S>;

  [[nodiscard]] constexpr auto
  is_full() const noexcept -> bool
  {
    return (header.unused_capacity() == 0);
  }

public:

  [[nodiscard]] constexpr
  array() = default;

  [[nodiscard]] explicit constexpr
  array(size_type capacity) noexcept
    : header{capacity}
  {}

  template <typename R>
    requires
      (!std::convertible_to<R, array>) &&
      std::ranges::forward_range<R> &&
      std::constructible_from<T, std::ranges::range_value_t<R>>
  [[nodiscard]] explicit constexpr
  array(R&& range) noexcept(std::is_nothrow_copy_constructible_v<std::ranges::range_value_t<R>>)
  // [[ post: size() == std::ranges::ssize(range) ]]
  // [[ post: is_full() ]]
    : header{std::distance(std::ranges::begin(range), std::ranges::end(range))}
  {
    header.insert_space(
      capacity(),
      writer{std::ranges::begin(range), std::ranges::end(range)}
    );
  }

  template <typename R>
    requires
      !std::convertible_to<R, array> &&
      std::ranges::forward_range<R> &&
      std::constructible_from<T, std::ranges::range_value_t<R>>
  constexpr auto
  operator=(R&& range) noexcept(std::is_nothrow_copy_constructible_v<std::ranges::range_value_t<R>>) -> array&
  {
    auto const n_elements{std::distance(std::ranges::begin(range), std::ranges::end(range))};
    if (capacity() < n_elements) {
      array temp{range};
      swap(temp);
      // [[ assert: is_full() ]]
    } else if (header) {
      header.erase_space(header.begin(), header.size());
      header.insert_space(n_elements, writer{std::ranges::begin(range), std::ranges::end(range)});
      // [[ assert: capacity() >= size() ]]
    }
    return *this;
  }

  [[nodiscard]] friend constexpr auto
  operator==(array const& x, array const& y) -> bool
    requires std::indirectly_comparable<T const*, T const*, std::ranges::equal_to>
  {
    return x.header == y.header;
  }

  [[nodiscard]] friend constexpr auto
  operator!=(array const& x, array const& y) -> bool
    requires std::indirectly_comparable<T const*, T const*, std::ranges::equal_to>
  {
    return x.header != y.header;
  }

  [[nodiscard]] friend constexpr auto
  operator<(array const& x, array const& y) -> bool
    requires std::indirect_strict_weak_order<std::ranges::less, T const*>
  {
    return x.header < y.header;
  }

  [[nodiscard]] friend constexpr auto
  operator>=(array const& x, array const& y) -> bool
    requires std::indirect_strict_weak_order<std::ranges::less, T const*>
  {
    return x.header >= y.header;
  }

  [[nodiscard]] friend constexpr auto
  operator>(array const& x, array const& y) -> bool
    requires std::indirect_strict_weak_order<std::ranges::less, T const*>
  {
    return x.header > y.header;
  }

  [[nodiscard]] friend constexpr auto
  operator<=(array const& x, array const& y) -> bool
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
  operator[](size_type n) noexcept -> T&
  // [[ pre: n < size() ]]
  {
    return *(header.begin() + n);
  }

  [[nodiscard]] constexpr auto
  operator[](size_type n) const noexcept -> T const&
  // [[ pre: n < size() ]]
  {
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
  size() const noexcept -> size_type
  {
    return header.size();
  }

  [[nodiscard]] constexpr auto
  capacity() const noexcept -> size_type
  {
    return header.capacity();
  }

  [[nodiscard]] constexpr auto
  max_size() const noexcept -> size_type
  {
    return std::numeric_limits<size_type>::max() / sizeof(T);
  }

  constexpr void
  reserve(size_type cap)
  // [[ post: capacity() >= cap ]]
  {
    if (capacity() < cap) {
      header.adjust_unused_capacity(cap - size());
    }
  }

  constexpr void
  shrink_to_fit()
  // [[ assert: is_full() ]]
  {
    if (!is_full()) {
      if (*this) {
        header.adjust_unused_capacity(0);
      } else {
        header = {};
      }
    }
  }

  template <typename... Args>
    requires std::constructible_from<T, Args...>
  constexpr auto
  push_back(Args &&...args) -> T&
  // [[ post: !empty() ]]
  {
    return header.push_back(std::forward<Args>(args)...);
  }

  constexpr void
  pop_back() noexcept
  // [[ pre: !empty() ]]
  {
    header.pop_back();
  }

  template <typename R>
    requires
      std::ranges::forward_range<R> &&
      std::constructible_from<T, std::ranges::range_value_t<R>>
  constexpr auto
  append(R&& range) -> T*
  // [[ pre axiom: "range not overlapped with [begin(), end())" ]]
  {
    auto const n_elements{std::ranges::distance(range)};
    reserve(size() + n_elements);
    return header.insert_space(n_elements, writer{std::ranges::begin(range), std::ranges::end(range)});
  }

  template <typename... Args>
    requires std::constructible_from<T, Args...>
  constexpr auto
  insert(T const* pos, Args &&...args) -> T*
  // [[ pre axiom: "pos within [begin(), end())" ]]
  // [[ post axiom: !empty() ]]
  {
    auto df{pos - begin()};
    auto dm{size()};
    push_back(std::forward<Args>(args)...);
    return std::rotate(begin() + df, begin() + dm, end());
  }

  template <typename R>
    requires
      std::ranges::forward_range<R> &&
      std::constructible_from<T, std::ranges::range_value_t<R>>
  constexpr auto
  insert(T const* pos, R&& range) -> T*
  // [[ pre axiom: "pos within [begin(), end())" ]]
  // [[ pre axiom: "range not overlapped with [begin(), end())" ]]
  {
    auto const df{pos - begin()};
    auto const dm{size()};
    append(std::forward<R>(range));
    return std::rotate(begin() + df, begin() + dm, end());
  }

  constexpr auto
  erase(T const* pos) noexcept -> T*
  // [[ pre: !empty() ]]
  // [[ pre axiom: "pos within [begin(), end())" ]]
  // [[ post axiom: "capacity() unchanged" ]]
  {
    header.erase_space(const_cast<T*>(pos), 1);
    return const_cast<T*>(pos);
  }

  template <typename R>
    requires
      std::ranges::range<R> &&
      std::same_as<T, std::ranges::range_value_t<R>>
  constexpr auto erase(R&& range) -> T*
  // [[ pre axiom: "range within [begin(), end())" ]]
  // [[ post axiom: "capacity() unchanged" ]]
  {
    auto const pos{std::ranges::begin(range)};
    header.erase_space(pos, std::distance(std::ranges::begin(range), std::ranges::end(range)));
    return pos;
  }

  constexpr void
  clear() noexcept
  // [[ post axiom: "capacity() unchanged" ]]
  {
    header.erase_space(begin(), size());
  }
};

template <std::ranges::forward_range R>
array(R&& range)->array<std::ranges::range_value_t<R>>;

static_assert(std::ranges::contiguous_range<array<int>>);
static_assert(std::totally_ordered<array<int>>);

export template <typename T, auto ga, auto& alloc>
constexpr void
swap(array<T, ga, alloc>& x, array<T, ga, alloc>& y) noexcept
{
  x.swap(y);
}

} // namespace cpp20

} // namespace eco

namespace std {

export template <typename T, auto ga, auto& alloc>
constexpr void
swap(eco::array<T, ga, alloc>& x, eco::array<T, ga, alloc>& y) noexcept
{
  using eco::swap;
  swap(x, y);
}

} // namespace std
