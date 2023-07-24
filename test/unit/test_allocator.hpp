#include <cassert>

#include "eco_allocator.hpp"

constexpr void
test_malloc_allocator()
{
  {
    eco::malloc_allocator x;
    auto memory{x.allocate(1)};
    assert(memory.first != nullptr);
    assert(memory.size == 1);
  }
  {
    eco::malloc_allocator x;
    auto memory{x.allocate(0)};
    assert(memory.first == nullptr);
    assert(memory.size == 0);
  }
  {
    eco::malloc_allocator x;
    auto memory{x.allocate(-1)};
    assert(memory.first == nullptr);
    assert(memory.size == 0);
  }

  {
    eco::malloc_allocator x;
    auto memory{x.allocate(1)};
    memory = x.reallocate(memory, 2);
    assert(memory.first != nullptr);
    assert(memory.size == 2);
  }
  {
    eco::malloc_allocator x;
    eco::memory_view memory;
    memory = x.reallocate(memory, 1);
    assert(memory.first != nullptr);
    assert(memory.size == 1);
  }
  {
    eco::malloc_allocator x;
    auto memory{x.allocate(1)};
    [[maybe_unused]] auto first{memory.first};
    [[maybe_unused]] auto size{memory.size};
    memory = x.reallocate(memory, 0);
    assert(memory.first == first);
    assert(memory.size == size);
  }
  {
    eco::malloc_allocator x;
    eco::memory_view memory;
    memory = x.reallocate(memory, -1);
    assert(memory.first == nullptr);
    assert(memory.size == 0);
  }

  {
    [[maybe_unused]] eco::malloc_allocator x;
    eco::memory_view memory;
    assert(x.deallocate(memory));
  }
  {
    eco::malloc_allocator x;
    auto memory{x.allocate(1)};
    assert(x.deallocate(memory));
  }
}

template <eco::deallocatable_allocator A>
inline constexpr void
test_arena_allocator()
{
  {
    eco::arena_allocator x{eco::malloc_allocator{}, 1};
    assert(x.can_allocate(1));
    assert(!x.can_allocate(2));
    auto memory{x.allocate(1)};
    assert(memory.first != nullptr);
    assert(memory.size == 1);
    assert(!x.can_allocate(1));
  }
  {
    eco::arena_allocator x{eco::malloc_allocator{}, 2};
    assert(x.can_allocate(2));
    auto memory{x.allocate(1)};
    assert(memory.first != nullptr);
    assert(memory.size == 1);
    memory = x.allocate(1);
    assert(memory.first != nullptr);
    assert(memory.size == 1);
  }
  {
    eco::arena_allocator<A> x;
    assert(x.can_allocate(0));
    auto memory{x.allocate(0)};
    assert(memory.first == nullptr);
    assert(memory.size == 0);
  }
  {
    eco::arena_allocator<A> x;
    auto memory{x.allocate(0)};
    assert(memory.first == nullptr);
    assert(memory.size == 0);
  }
  {
    eco::arena_allocator<A> x;
    auto memory{x.allocate(-1)};
    assert(memory.first == nullptr);
    assert(memory.size == 0);
  }

  {
    eco::arena_allocator<A> x;
    eco::memory_view memory;
    assert(x.deallocate(memory));
  }
  {
    eco::arena_allocator x{eco::malloc_allocator{}, 2};
    assert(x.can_allocate(2));
    auto memory{x.allocate(1)};
    assert(!x.can_allocate(2));
    assert(x.can_allocate(1));
    assert(x.deallocate(memory));
    assert(!x.can_allocate(2));
    assert(x.can_allocate(1));
    memory = x.allocate(1);
    assert(!x.can_allocate(1));
    x.deallocate_all();
    assert(x.can_allocate(2));
  }
}

constexpr void
test_allocator()
{
  test_malloc_allocator();
  test_arena_allocator<eco::malloc_allocator>();
}
