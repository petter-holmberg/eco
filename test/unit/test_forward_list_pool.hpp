#include <cassert>

#include "eco_algorithm.hpp"
#include "eco_forward_list_pool.hpp"

inline void
test_forward_list_pool()
{
  eco::forward_list_pool<int, int> pool;
  assert(pool.allocate_node(0, pool.limit()) == 0);
  assert(pool.value(0) == 0);

  pool.value(0) = 1;
  assert(pool.value(0) == 1);
  assert(pool.next(0) == pool.limit());

  eco::free_list(pool, 0);
  assert(pool.next(0) == pool.limit());

  auto head{pool.allocate_node(0, pool.limit())};
  assert(head == 0);
  head = pool.allocate_node(1, head);
  assert(head == 1);
  head = pool.allocate_node(2, head);
  assert(head == 2);

  {
    auto x{head};
    assert(pool.next(x) != pool.limit());
    x = pool.next(x);
    assert(pool.next(x) != pool.limit());
    x = pool.next(x);
    assert(pool.next(x) == pool.limit());
  }

  {
    auto tail{pool.allocate_node(4, pool.limit())};
    tail = pool.allocate_node(3, tail);
    tail = pool.allocate_node(2, tail);
    tail = pool.allocate_node(1, tail);
    head = pool.allocate_node(0, tail);

    auto first{pool.forward_iterator(head)};
    assert(*first == 0);
    auto next{first};
    ++next;
    assert(*next == 1);
    ++next;
    assert(*next == 2);
    ++next;
    assert(*next == 3);
    auto last{next};
    ++next;
    assert(*next == 4);

    auto linker{pool.forward_linker()};
    first = eco::reverse_append(first, last, last, linker);
    assert(*first == 2);
    next = first;
    ++next;
    assert(*next == 1);
    ++next;
    assert(*next == 0);
    ++next;
    assert(*next == 3);
    ++next;
    assert(*next == 4);
  }
}
