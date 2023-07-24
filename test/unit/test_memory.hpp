#include <cassert>

#include "eco_memory.hpp"

constexpr void
test_memory_view()
{
  std::byte stack[5];

  {
    eco::memory_view x;
    assert(!x);
  }
  {
    eco::memory_view x;
    assert(x.begin() == nullptr);
    assert(x.end() == nullptr);
  }

  {
    eco::memory_view x;
    eco::memory_view y;
    assert(!x.begins_in(y));
    assert(!y.begins_in(x));
    assert(!x.ends_in(y));
    assert(!y.ends_in(x));
    assert(!x.is_in(y));
    assert(!y.is_in(x));
  }
  {
    eco::memory_view x;
    eco::memory_view y{stack, 5};
    assert(!x.begins_in(y));
    assert(!y.begins_in(x));
    assert(!x.ends_in(y));
    assert(!y.ends_in(x));
    assert(!x.is_in(y));
    assert(!y.is_in(x));
  }
  {
    eco::memory_view x{stack, 5};
    eco::memory_view y;
    assert(!x.begins_in(y));
    assert(!y.begins_in(x));
    assert(!x.ends_in(y));
    assert(!y.ends_in(x));
    assert(!x.is_in(y));
    assert(!y.is_in(x));
  }
  {
    eco::memory_view x{stack, 2};
    eco::memory_view y{stack + 2, 3};
    assert(!x.begins_in(y));
    assert(!y.begins_in(x));
    assert(!x.ends_in(y));
    assert(!y.ends_in(x));
    assert(!x.is_in(y));
    assert(!y.is_in(x));
  }
  {
    eco::memory_view x{stack + 1, 2};
    eco::memory_view y{stack + 2, 3};
    assert(!x.begins_in(y));
    assert(y.begins_in(x));
    assert(x.ends_in(y));
    assert(!y.ends_in(x));
    assert(!x.is_in(y));
    assert(!y.is_in(x));
  }
  {
    eco::memory_view x{stack + 2, 2};
    eco::memory_view y{stack + 2, 3};
    assert(x.begins_in(y));
    assert(y.begins_in(x));
    assert(x.ends_in(y));
    assert(!y.ends_in(x));
    assert(x.is_in(y));
    assert(!y.is_in(x));
  }
  {
    eco::memory_view x{stack + 3, 2};
    eco::memory_view y{stack + 2, 3};
    assert(x.begins_in(y));
    assert(!y.begins_in(x));
    assert(x.ends_in(y));
    assert(y.ends_in(x));
    assert(x.is_in(y));
    assert(!y.is_in(x));
  }
  {
    eco::memory_view x{stack, 2};
    eco::memory_view y{stack + 2, 2};
    assert(!x.begins_in(y));
    assert(!y.begins_in(x));
    assert(!x.ends_in(y));
    assert(!y.ends_in(x));
    assert(!x.is_in(y));
    assert(!y.is_in(x));
  }
  {
    eco::memory_view x{stack + 1, 2};
    eco::memory_view y{stack + 2, 2};
    assert(!x.begins_in(y));
    assert(y.begins_in(x));
    assert(x.ends_in(y));
    assert(!y.ends_in(x));
    assert(!x.is_in(y));
    assert(!y.is_in(x));
  }
  {
    eco::memory_view x{stack + 2, 2};
    eco::memory_view y{stack + 2, 2};
    assert(x.begins_in(y));
    assert(y.begins_in(x));
    assert(x.ends_in(y));
    assert(y.ends_in(x));
    assert(x.is_in(y));
    assert(y.is_in(x));
  }
  {
    eco::memory_view x{stack + 3, 2};
    eco::memory_view y{stack + 2, 2};
    assert(x.begins_in(y));
    assert(!y.begins_in(x));
    assert(!x.ends_in(y));
    assert(y.ends_in(x));
    assert(!x.is_in(y));
    assert(!y.is_in(x));
  }
  {
    eco::memory_view x{stack, 3};
    eco::memory_view y{stack + 3, 2};
    assert(!x.begins_in(y));
    assert(!y.begins_in(x));
    assert(!x.ends_in(y));
    assert(!y.ends_in(x));
    assert(!x.is_in(y));
    assert(!y.is_in(x));
  }
  {
    eco::memory_view x{stack + 1, 3};
    eco::memory_view y{stack + 3, 2};
    assert(!x.begins_in(y));
    assert(y.begins_in(x));
    assert(x.ends_in(y));
    assert(!y.ends_in(x));
    assert(!x.is_in(y));
    assert(!y.is_in(x));
  }
  {
    eco::memory_view x{stack + 2, 3};
    eco::memory_view y{stack + 3, 2};
    assert(!x.begins_in(y));
    assert(y.begins_in(x));
    assert(x.ends_in(y));
    assert(y.ends_in(x));
    assert(!x.is_in(y));
    assert(y.is_in(x));
  }
}

constexpr void
test_memory()
{
  test_memory_view();
}
