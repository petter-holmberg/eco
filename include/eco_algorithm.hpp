#ifndef ECO_ALGORITHM_
#define ECO_ALGORITHM_

#include <concepts>
#include <iterator>
#include <ranges>

namespace eco::inline cpp20 {

struct reverse_append_impl
{
  template <std::ranges::forward_range R, forward_linker L>
  requires std::same_as<std::ranges::iterator_t<R>, iterator_t<L>>
  constexpr auto
  operator()(R&& r, std::ranges::iterator_t<R> head, L set_successor) const -> std::ranges::iterator_t<R>
  {
    (*this)(std::ranges::begin(r), std::ranges::end(r), head, set_successor);
  }

  template <std::forward_iterator I, forward_linker L>
  requires std::same_as<I, iterator_t<L>>
  constexpr auto
  operator()(I first, std::sentinel_for<I> auto last, I head, L set_successor) const -> I
  {
    while (first != last) {
      auto next = first;
      ++next;
      set_successor(first, head);
      head = first;
      first = next;
    }
    return head;
  }
};

inline constexpr reverse_append_impl reverse_append{};

}

#endif
