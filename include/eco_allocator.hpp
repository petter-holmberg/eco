#ifndef ECO_ALLOCATOR_
#define ECO_ALLOCATOR_

#include <cassert>
#include <concepts>
#include <cstdlib>

#include "eco_concepts.hpp"
#include "eco_memory.hpp"
#include "eco_type_traits.hpp"

namespace eco::inline cpp20 {

template <typename T>
concept allocator =
  std::movable<T> &&
  std::totally_ordered<T> &&
  requires(T a, ssize_t<memory_view> n)
  {
    { a.allocate(n) } -> std::same_as<memory_view>;
  };

template <typename T>
concept deallocatable_allocator =
  allocator<T> &&
  requires(T& a, memory_view mem)
  {
    { a.deallocate(mem) } noexcept -> boolean_testable;
  };

template <typename T>
concept reallocatable_allocator =
  allocator<T> &&
  requires(T a, memory_view mem, ssize_t<memory_view> n)
  {
    { a.reallocate(mem, n) } -> std::same_as<memory_view>;
  };

template <typename T>
concept ownership_aware_allocator =
  allocator<T> &&
  requires(T const& a, memory_view mem)
  {
    { a.is_owner(mem) } noexcept -> boolean_testable;
  };

class malloc_allocator
{
public:
  [[nodiscard]] constexpr auto
  operator<=>(malloc_allocator const&) const noexcept = default;

  [[nodiscard]] constexpr auto
  allocate(ssize_t<memory_view> n) noexcept -> memory_view
  {
    if (n <= 0) {
      return {};
    }
    auto first{std::malloc(n)};
    if (first == nullptr) {
      n = 0;
    }
    return {first, n};
  }

  [[nodiscard]] constexpr auto
  reallocate(memory_view mem, ssize_t<memory_view> n) noexcept -> memory_view
  {
    if (n <= 0) {
      return mem;
    }
    auto first{std::realloc(mem.first, n)};
    if (first == nullptr) {
      n = 0;
    }
    return {first, n};
  }

  constexpr auto
  deallocate(memory_view mem) noexcept -> bool
  {
    std::free(mem.first);
    return true;
  }
};

static_assert(deallocatable_allocator<malloc_allocator>);
static_assert(reallocatable_allocator<malloc_allocator>);

template <allocator A = malloc_allocator>
class arena_allocator
{
  [[no_unique_address]] A alloc;
  memory_view arena;
  std::byte* pos{nullptr};

public:
  constexpr
  ~arena_allocator()
  {
    alloc.deallocate(arena);
  }

  arena_allocator(arena_allocator const&) = delete;

  arena_allocator& operator=(arena_allocator const&) = delete;

  constexpr
  arena_allocator(arena_allocator&&) noexcept = default;

  constexpr
  arena_allocator& operator=(arena_allocator&&) noexcept = default;

  [[nodiscard]] constexpr
  arena_allocator() = default;

  [[nodiscard]] constexpr
  arena_allocator(A&& alloc, ssize_t<memory_view> n)
    : alloc{std::forward<A>(alloc)}
    , arena{alloc.allocate(n)}
    , pos{arena.begin()}
  {}

  [[nodiscard]] constexpr auto
  operator==(arena_allocator const&) const noexcept -> bool = default;

  [[nodiscard]] constexpr auto
  operator<(arena_allocator const& x) const noexcept
  {
    return alloc < x.alloc ||
      (!(x.alloc < alloc) && (arena < x.arena)) ||
      (!(x.alloc < alloc) && !(x.arena < arena) && std::less<std::byte*>{}(pos, x.pos));
  }

  [[nodiscard]] constexpr auto
  operator<=(arena_allocator const& x) const noexcept
  {
    return !(*this < x);
  }

  [[nodiscard]] constexpr auto
  operator>(arena_allocator const& x) const noexcept
  {
    return (x < *this);
  }

  [[nodiscard]] constexpr auto
  operator>=(arena_allocator const& x) const noexcept
  {
    return !(x < *this);
  }

  [[nodiscard]] constexpr auto
  can_allocate(ssize_t<memory_view> n) const noexcept -> bool
  {
    return n <= arena.end() - pos;
  }

  [[nodiscard]] constexpr auto
  allocate(ssize_t<memory_view> n) -> memory_view
  {
    [[maybe_unused]] pre: assert(can_allocate(n));

    if (n <= 0) {
      return {};
    }
    memory_view allocated{pos, n};
    pos += n;
    return allocated;
  }

  constexpr auto
  deallocate(memory_view) noexcept -> bool
  requires deallocatable_allocator<A>
  {
    return true;
  }

  constexpr void
  deallocate_all() noexcept
  {
    pos = arena.begin();
  }
};

static_assert(deallocatable_allocator<arena_allocator<malloc_allocator>>);

}

#endif
