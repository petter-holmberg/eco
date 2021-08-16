module;

#include <concepts>
#include <cstddef>
#include <cstdlib>
#include <functional>
#include <ranges>

export module eco.memory;

namespace eco {

inline namespace cpp20 {

template <typename T>
struct ssize_type_traits
{
  using type = T::ssize_type;
};

export template <typename T>
using ssize_type_t = typename ssize_type_traits<T>::type;

export struct memory_view : public std::ranges::view_interface<memory_view>
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

  [[nodiscard]] auto
  begin() const noexcept -> std::byte*
  {
    return reinterpret_cast<std::byte*>(first);
  }

  [[nodiscard]] auto
  end() const noexcept -> std::byte*
  {
    return reinterpret_cast<std::byte*>(first) + size;
  }
};

static_assert(std::ranges::view<memory_view>);

export [[nodiscard]] constexpr auto
operator==(memory_view x, memory_view y) noexcept -> bool
{
  return x.first == y.first && x.size == y.size;
}

export [[nodiscard]] constexpr auto
operator!=(memory_view x, memory_view y) noexcept -> bool
{
  return !(x == y);
}

export [[nodiscard]] constexpr auto
operator<(memory_view x, memory_view y) noexcept -> bool
{
  return std::less<void*>{}(x.first, y.first) || (!(std::less<void*>{}(y.first, x.first)) && (x.size < y.size));
}

export [[nodiscard]] constexpr auto
operator>=(memory_view x, memory_view y) noexcept -> bool
{
  return !(x < y);
}

export [[nodiscard]] constexpr auto
operator>(memory_view x, memory_view y) noexcept -> bool
{
  return y < x;
}

export [[nodiscard]] constexpr auto
operator<=(memory_view x, memory_view y) noexcept -> bool
{
  return !(y < x);
}

static_assert(std::totally_ordered<memory_view>);

export template <typename A>
concept allocator =
  requires(A a, ssize_type_t<memory_view> n)
  {
    { a.allocate(n) } noexcept-> std::same_as<memory_view>;
  };

export template <typename A>
concept deallocatable_allocator =
  allocator<A> &&
  requires(A& a, memory_view mem)
  {
    { a.deallocate(mem) }
    noexcept->std::same_as<bool>;
  };

export template <typename A>
concept reallocatable_allocator =
  allocator<A> &&
  requires(A a, memory_view mem, ssize_type_t<memory_view> n)
  {
    { a.reallocate(mem, n) } noexcept -> std::same_as<memory_view>;
  };

export template <typename A>
concept ownership_aware_allocator =
  allocator<A> &&
  requires(A const& a, memory_view mem)
  {
    { a.is_owner(mem) } noexcept -> std::same_as<bool>;
  };

export class malloc_allocator
{
public:
  [[nodiscard]] auto
  allocate(ssize_type_t<memory_view> n) noexcept -> memory_view
  {
    if (n <= 0) {
      return {};
    }
    auto addr{std::malloc(n)};
    if (addr == nullptr) {
      n = 0;
    }
    return {addr, n};
  }

  [[nodiscard]] auto
  reallocate(memory_view m, ssize_type_t<memory_view> n) noexcept -> memory_view
  {
    if (n <= 0) {
      return {};
    }
    auto first{std::realloc(m.first, n)};
    if (first != nullptr) {
      m.first = first;
      m.size = n;
    }
    return m;
  }

  auto deallocate(memory_view mem) noexcept -> bool
  {
    std::free(mem.first);
    return true;
  }
};

static_assert(deallocatable_allocator<malloc_allocator>);
static_assert(reallocatable_allocator<malloc_allocator>);

} // namespace cpp20

} // namespace eco
