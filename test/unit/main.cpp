import std;
import eco;

#include <cassert>

template <std::regular T>
constexpr void
test_regular(T const &x)
{
  T y;
  T z;

  assert(x == x);
  assert(!(x != x));

  y = x;
  assert(y == x);

  z = std::move(y);
  assert(z == x);

  T w(x);
  assert(w == x);

  T v(std::move(w));
  assert(v == x);
}

template <std::totally_ordered T>
void test_totally_ordered(T& x, T& y)
{
    test_regular(x);
    test_regular(y);

    assert(x != y);
    assert(!(x == y));

    assert(!(x < x));
    assert(x < y);
    assert(y > x);
    assert(x <= y);
    assert(y >= x);
    assert(!(y < x));
    assert(!(x > y));
    assert(!(y <= x));
    assert(!(x >= y));
}

struct ptree_node
{
  int value;
  ptree_node* parent = nullptr;
  ptree_node* first_child = nullptr;
  ptree_node* next_sibling = nullptr;
};

struct ptree_cursor
{
  using weight_type = std::ptrdiff_t;

  ptree_node* value = nullptr;

  [[nodiscard]] inline constexpr auto
  operator==(ptree_cursor const&) const noexcept -> bool = default;

  [[nodiscard]] inline constexpr
  explicit operator bool() const { return value != nullptr; }
};

inline bool
has_left_successor(ptree_cursor i)
{
  return i.value->first_child != nullptr;
}

inline bool
has_right_successor(ptree_cursor i)
{
  return i.value->next_sibling != nullptr;
}

inline ptree_cursor
left_successor(ptree_cursor i)
{
  return {i.value->first_child};
}

inline ptree_cursor
right_successor(ptree_cursor i)
{
  return {i.value->next_sibling};
}

inline bool
has_predecessor(ptree_cursor i)
{
  return i.value->parent != nullptr;
}

inline ptree_cursor
predecessor(ptree_cursor i)
{
  return {i.value->parent};
}

inline void
set_left_successor(ptree_cursor i, ptree_cursor j)
{
  i.value->first_child = j.value;
}

inline void
set_right_successor(ptree_cursor i, ptree_cursor j)
{
  i.value->next_sibling = j.value;
}

#include "test_bit.hpp"
#include "test_memory.hpp"
#include "test_allocator.hpp"
#include "test_extent.hpp"
#include "test_array.hpp"
#include "test_array_dict.hpp"
#include "test_forward_list_pool.hpp"
#include "test_list_pool.hpp"
#include "test_fixed_array.hpp"
#include "test_codec.hpp"
#include "test_tape.hpp"
#include "test_bitvector.hpp"
#include "test_parentheses.hpp"
#include "test_binary_tree.hpp"
#include "test_ordinal_tree.hpp"
#include "test_fold.hpp"
#include "test_search.hpp"

static_assert(eco::bidirectional_bicursor<ptree_cursor>);
static_assert(eco::linked_bicursor<ptree_cursor>);

int main()
{
  test_bit();
  test_memory();
  test_allocator();
  test_extent();
  test_array();
  test_array_dict();
  test_forward_list_pool();
  test_list_pool();
  test_fixed_array<3, std::uint8_t>();
  test_fixed_array<4, std::uint8_t>();
  test_fixed_array<7, std::uint8_t>();
  test_fixed_array<4, std::uint16_t>();
  test_fixed_array<8, std::uint16_t>();
  test_fixed_array<15, std::uint16_t>();
  test_fixed_array<5, std::uint32_t>();
  test_fixed_array<16, std::uint32_t>();
  test_fixed_array<31, std::uint32_t>();
  test_fixed_array<6, std::uint64_t>();
  test_fixed_array<32, std::uint64_t>();
  test_fixed_array<63, std::uint64_t>();
  test_unary_codec();
  test_gamma_codec();
  test_tape();
  test_basic_bitvector();
  test_basic_parentheses();
  test_balanced_binary_tree();
  test_binary_louds();
  test_louds();
  test_bp_tree();
  test_dfuds();
  test_fold();
  test_search();
}
