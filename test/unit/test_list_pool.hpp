#include <cassert>

#include "eco_algorithm.hpp"
#include "eco_list_pool.hpp"

inline void
test_list_pool()
{
  eco::list_pool<int, int> pool;
  assert(pool.allocate_node(0, pool.limit()) == 0);
  assert(pool.value(0) == 0);

  pool.value(0) = 1;
  assert(pool.value(0) == 1);
  assert(pool.prev(0) == pool.limit());
  assert(pool.next(0) == pool.limit());

  eco::free_list(pool, 0);
  assert(pool.prev(0) == pool.limit());
  assert(pool.next(0) == pool.limit());

  auto head{pool.allocate_node(0, pool.limit())};
  assert(head == 0);
  assert(pool.prev(head) == pool.limit());
  assert(pool.next(head) == pool.limit());
  head = pool.allocate_node(1, head);
  assert(head == 1);
  assert(pool.prev(head) == pool.limit());
  assert(pool.next(head) == 0);
  head = pool.allocate_node(2, head);
  assert(head == 2);

  {
    auto x{head};
    assert(pool.prev(x) == pool.limit());
    assert(pool.next(x) != pool.limit());
    x = pool.next(x);
    assert(pool.prev(x) != pool.limit());
    assert(pool.next(x) != pool.limit());
    x = pool.next(x);
    assert(pool.prev(x) != pool.limit());
    assert(pool.next(x) == pool.limit());
  }

  {
    auto n4{pool.allocate_node(4, pool.limit())};
    auto n3{pool.allocate_node(3, n4)};
    auto n2{pool.allocate_node(2, n3)};
    auto n1{pool.allocate_node(1, n2)};

    head = pool.allocate_node(0, n1);
    auto i0{pool.bidirectional_iterator(head)};
    auto i4{pool.bidirectional_iterator(n4)};

    auto linker{pool.bidirectional_linker()};
    linker(i4, i0);

    assert(pool.value(head) == 0);
    assert(pool.value(pool.next(head)) == 1);
    assert(pool.value(pool.next(pool.next(head))) == 2);
    assert(pool.value(pool.next(pool.next(pool.next(head)))) == 3);
    assert(pool.value(pool.next(pool.next(pool.next(pool.next(head))))) == 4);

    pool.unlink_node(n3);
    assert(pool.value(head) == 0);
    assert(pool.value(pool.next(head)) == 1);
    assert(pool.value(pool.next(pool.next(head))) == 2);
    assert(pool.value(pool.next(pool.next(pool.next(head)))) == 4);
    assert(pool.value(pool.next(pool.next(pool.next(pool.next(head))))) == 0);

    pool.unlink_node(n2);
    assert(pool.value(head) == 0);
    assert(pool.value(pool.next(head)) == 1);
    assert(pool.value(pool.next(pool.next(head))) == 4);
    assert(pool.value(pool.next(pool.next(pool.next(head)))) == 0);
    assert(pool.value(pool.next(pool.next(pool.next(pool.next(head))))) == 1);

    pool.unlink_node(n1);
    assert(pool.value(head) == 0);
    assert(pool.value(pool.next(head)) == 4);
    assert(pool.value(pool.next(pool.next(head))) == 0);
    assert(pool.value(pool.next(pool.next(pool.next(head)))) == 4);
    assert(pool.value(pool.next(pool.next(pool.next(pool.next(head))))) == 0);

    pool.relink_node(n1);
    assert(pool.value(head) == 0);
    assert(pool.value(pool.next(head)) == 1);
    assert(pool.value(pool.next(pool.next(head))) == 4);
    assert(pool.value(pool.next(pool.next(pool.next(head)))) == 0);
    assert(pool.value(pool.next(pool.next(pool.next(pool.next(head))))) == 1);

    pool.relink_node(n2);
    assert(pool.value(head) == 0);
    assert(pool.value(pool.next(head)) == 1);
    assert(pool.value(pool.next(pool.next(head))) == 2);
    assert(pool.value(pool.next(pool.next(pool.next(head)))) == 4);
    assert(pool.value(pool.next(pool.next(pool.next(pool.next(head))))) == 0);

    pool.relink_node(n3);
    assert(pool.value(head) == 0);
    assert(pool.value(pool.next(head)) == 1);
    assert(pool.value(pool.next(pool.next(head))) == 2);
    assert(pool.value(pool.next(pool.next(pool.next(head)))) == 3);
    assert(pool.value(pool.next(pool.next(pool.next(pool.next(head))))) == 4);
  }
}
