#ifndef ECO_CONCEPTS_
#define ECO_CONCEPTS_

#include <concepts>
#include <iterator>
#include <utility>

#include "eco_type_traits.hpp"

namespace eco::inline cpp20 {

template <typename T>
concept boolean_testable =
  std::convertible_to<T, bool> &&
  requires (T&& b) {
    { !std::forward<T>(b) } -> std::convertible_to<bool>;
  };

static_assert(boolean_testable<bool>);
static_assert(boolean_testable<int>);
static_assert(boolean_testable<double>);

template <typename T>
concept forward_linker =
  std::forward_iterator<iterator_t<T>> &&
  std::invocable<T, iterator_t<T>, iterator_t<T>>;

template <typename T>
concept backward_linker =
  std::bidirectional_iterator<iterator_t<T>> &&
  std::invocable<T, iterator_t<T>, iterator_t<T>>;

template <typename T>
concept bidirectional_linker =
  forward_linker<T> &&
  backward_linker<T>;

template <typename T>
concept bicursor =
  std::regular<T> &&
  std::integral<weight_type_t<T>> &&
  requires (T cur) {
    static_cast<bool>(cur);
    { has_left_successor(cur) } -> boolean_testable;
    { has_right_successor(cur) } -> boolean_testable;
    { left_successor(cur) } -> std::same_as<T>;
    { right_successor(cur) } -> std::same_as<T>;
  };

template <typename T>
concept bidirectional_bicursor =
  bicursor<T> &&
  requires (T cur) {
    { has_predecessor(cur) } -> boolean_testable;
    { predecessor(cur) } -> std::same_as<T>;
  };

template <typename T>
concept linked_bicursor =
  bicursor<T> &&
  requires (T cur) {
    set_left_successor(cur, cur);
    set_right_successor(cur, cur);
  };

}

#endif
