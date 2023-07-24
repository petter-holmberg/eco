#ifndef ECO_DLX_
#define ECO_DLX_

#include <concepts>
#include <type_traits>
#include <limits>

#include "eco_array.hpp"
#include "eco_type_traits.hpp"

namespace eco::inline cpp20 {

template <std::signed_integral T> class dlx;

template <typename T, typename U>
concept dlx_visitor =
  std::invocable<T, ssize_t<dlx<U>>, typename dlx<U>::solution_iterator>;

template <std::signed_integral T>
struct dlx_visit_all
{
  array<array<T>> solutions;

  constexpr void
  operator()(ssize_t<dlx<T>> l, typename dlx<T>::solution_iterator first) noexcept
  {
    auto& options = solutions.push_back(array<T>{l});
    while (l != 0) {
      options.push_back(first.choice());
      ++first;
      --l;
    }
  }
};

template <typename T, typename U>
concept dlx_item_choice_heuristic =
  std::invocable<T, ssize_t<dlx<U>>, typename dlx<U>::item_iterator> &&
  std::convertible_to<std::invoke_result_t<T, ssize_t<dlx<U>>, typename dlx<U>::item_iterator>, ssize_t<dlx<U>>>;

template <std::signed_integral T>
struct dlx_mrv_heuristic
{
  [[nodiscard]] constexpr auto
  operator()(ssize_t<dlx<T>> i, typename dlx<T>::item_iterator first) const noexcept -> ssize_t<dlx<T>>
  {
    auto min{std::numeric_limits<T>::max()};
    auto pos{first};
    ++pos;
    while (pos != first && min > 0) {
      if (*(pos) < min) {
        min = *(pos);
        i = pos.item();
      }
      ++pos;
    }
    return i;
  }
};

template <std::signed_integral T>
class dlx
{
public:
  using value_type = T;
  using ssize_type = ssize_t<memory_view>;

private:
  struct node
  {
    T top{};
    T ulink{};
    T dlink{};
  };

  array<node> data;
  array<node*> solution;
  node* items_first;

  [[nodiscard]] constexpr auto
  prev(node *x) noexcept -> node*
  {
    return data.begin() + x->ulink;
  }

  [[nodiscard]] constexpr auto
  next(node *x) noexcept -> node*
  {
    return data.begin() + x->dlink;
  }

  [[nodiscard]] constexpr auto
  top(node *x) noexcept -> node*
  {
    return data.begin() + x->top;
  }

  [[nodiscard]] constexpr auto
  item(ssize_type i) noexcept -> node*
  {
    return data.begin() + i;
  }

  [[nodiscard]] constexpr auto
  item_header(ssize_type i) noexcept -> node*
  {
    return items_first + i;
  }

  [[nodiscard]] constexpr auto
  rlink(ssize_type i) noexcept -> node*
  {
    return next(items_first + i);
  }

  constexpr void
  unlink(node* x) noexcept
  {
    prev(x)->dlink = x->dlink;
    next(x)->ulink = x->ulink;
  }

  constexpr void
  relink(node *x) noexcept
  {
    prev(x)->dlink = x - data.begin();
    next(x)->ulink = x - data.begin();
  }

  constexpr void
  hide(node* p) noexcept
  {
    auto q{p + 1};
    while (q != p) {
      auto x{q->top};
      if (x <= 0) {
        q = prev(q);
      } else {
        unlink(q);
        item(x)->top -= 1;
        ++q;
      }
    }
  }

  constexpr void
  unhide(node* p) noexcept
  {
    auto q{p - 1};
    while (q != p) {
      auto x{q->top};
      if (x <= 0)
      {
        q = next(q);
      } else {
        relink(q);
        item(x)->top += 1;
        --q;
      }
    }
  }

  constexpr
  void cover(node* i) noexcept
  {
    auto p{next(i)};
    while (p != i)
    {
      hide(p);
      p = next(p);
    }
    unlink(item_header(i - data.begin()));
  }

  constexpr
  void uncover(node* i) noexcept
  {
    relink(item_header(i - data.begin()));
    auto p{prev(i)};
    while (p != i)
    {
      unhide(p);
      p = prev(p);
    }
  }

  [[nodiscard]] constexpr auto
  cover_item(ssize_type i, ssize_type l, dlx_item_choice_heuristic<T> auto heuristic) noexcept -> ssize_type
  {
    i = choose_item(i, heuristic);
    cover(item(i));
    solution[l] = next(item(i));
    return i;
  }

  [[nodiscard]] constexpr auto
  choose_item(ssize_type i, dlx_item_choice_heuristic<T> auto heuristic) noexcept -> ssize_type
  {
    return std::invoke(heuristic, i, item_begin());
  }

  [[nodiscard]] constexpr auto
  try_option(ssize_type l) noexcept -> ssize_type
  {
    auto p{solution[l] + 1};
    while (p != solution[l]) {
      auto i{p->top};
      if (i <= 0) {
        p = prev(p);
      } else {
        cover(item(i));
        ++p;
      }
    }
    return l + 1;
  }

  [[nodiscard]] constexpr auto
  retry_option(ssize_type l) noexcept -> ssize_type
  {
    auto p{solution[l] - 1};
    while (p != solution[l]) {
      auto i{p->top};
      if (i <= 0) {
        p = next(p);
      } else {
        uncover(item(i));
        --p;
      }
    }
    auto i{solution[l]->top};
    solution[l] = next(solution[l]);
    return i;
  }

public:
  class item_iterator
  {
    friend class dlx;

    node* first;
    node* head;
    node* pos;

    [[nodiscard]] constexpr
    item_iterator(node* f, node* h, node* i) noexcept
      : first{f}, head{h}, pos{i}
    {}

  public:
    using value_type = dlx::value_type;
    using difference_type = dlx::ssize_type;

    constexpr item_iterator() noexcept = default;

    [[nodiscard]] constexpr auto
    operator*() const noexcept -> value_type&
    {
      return pos->top;
    }

    constexpr auto
    operator++() noexcept -> item_iterator&
    {
      pos = first + pos->dlink;
      return *this;
    }

    [[nodiscard]] constexpr auto
    operator++(int) noexcept -> item_iterator
    {
      auto temp{*this};
      ++(*this);
      return temp;
    }

    [[nodiscard]] constexpr auto
    item() const noexcept -> difference_type
    {
      return (pos - head);
    }

    friend constexpr auto
    operator==(item_iterator const& x, item_iterator const& y) noexcept -> bool
    {
      return x.pos == y.pos;
    }

    friend constexpr auto
    operator!=(item_iterator const& x, item_iterator const& y) noexcept -> bool
    {
      return !(x == y);
    }
  };

  class solution_iterator
  {
    friend class dlx;

    node** pos;

    [[nodiscard]] explicit constexpr
    solution_iterator(node** first) noexcept
      : pos{first}
    {}

  public:
    using value_type = dlx::value_type;
    using difference_type = dlx::ssize_type;

    constexpr solution_iterator() noexcept = default;

    [[nodiscard]] constexpr auto
    operator*() const noexcept -> value_type&
    {
      return *(*pos - 1);
    }

    constexpr auto
    operator++() noexcept -> solution_iterator&
    {
      ++pos;
      return *this;
    }

    [[nodiscard]] constexpr auto
    operator++(int) noexcept -> solution_iterator
    {
      auto temp{*this};
      ++(*this);
      return temp;
    }

    [[nodiscard]] constexpr auto
    choice() const noexcept -> difference_type
    {
      auto spacer{*pos};
      while (spacer->top > 0) --spacer;
      return -spacer->top;
    }

    friend constexpr auto
    operator==(solution_iterator const& x, solution_iterator const& y) noexcept -> bool
    {
      return x.pos == y.pos;
    }

    friend constexpr auto
    operator!=(solution_iterator const& x, solution_iterator const& y) noexcept -> bool
    {
      return !(x == y);
    }
  };

  [[nodiscard]] constexpr auto
  item_begin() noexcept -> item_iterator
  {
    return {data.begin(), items_first, items_first};
  }

  [[nodiscard]] constexpr auto
  solution_begin() noexcept -> solution_iterator
  {
    return solution_iterator{solution.begin()};
  }

  [[nodiscard]] constexpr
  dlx() noexcept = default;

  template <std::ranges::forward_range I, std::ranges::forward_range O>
    requires
      std::constructible_from<T, std::ranges::range_value_t<I>> &&
      std::ranges::forward_range<std::ranges::range_value_t<O>> &&
      std::integral<std::ranges::range_value_t<std::ranges::range_value_t<O>>>
  [[nodiscard]] constexpr
  dlx(I&& items, O&& options)
  {
    auto const n_items{std::ranges::ssize(items)};
    auto const n_options{std::ranges::ssize(options)};
    resize(solution, n_options);

    data.push_back();
    ssize_type i{1};
    while (i <= n_items) {
      data.push_back(0, i, i);
      ++i;
    }
    data.push_back();

    auto p{data.size() - 1};
    ssize_type spacer{-1};

    for (auto const& option : options) {
      ssize_type j{1};
      for (auto const& item : option) {
        auto const i{item + 1};
        data.push_back(i, 0, 0);
        ++data[i].top;
        auto prev = data[i].ulink;
        data[i].ulink = p + j;
        data[prev].dlink = p + j;
        data[p + j].top = i;
        data[p + j].ulink = prev;
        data[p + j].dlink = i;
        ++j;
      }

      data[p].dlink = p - 1 + j;
      data.push_back(spacer, data.size() - j + 1, 0);
      --spacer;
      p += j;
    }

    data.push_back(0, data.size() + n_items, data.size() + 1);

    for (auto const& item : items) {
      data.push_back(item, data.size() - 1, data.size() + 1);
    }
    data[data.size() - 1].dlink = data.size() - n_items - 1;

    items_first = data.end() - n_items - 1;
  }

  [[nodiscard]] constexpr auto
  operator<=>(dlx const&) const noexcept -> bool = default;

  [[nodiscard]] static constexpr auto
  visit_all() -> dlx_visit_all<T>
  {
    return dlx_visit_all<T>{};
  }

  template <dlx_visitor<T> V, dlx_item_choice_heuristic<T> H = dlx_mrv_heuristic<T>>
  void
  operator()(V& visitor, H heuristic = {})
  {
    ssize_type i{1};
    ssize_type l{0};

  enter_level_l:
    if (rlink(0) == items_first) {
      visit_solution(l, visitor);
      goto leave_level_l;
    }
    i = cover_item(i, l, heuristic);

  try_option_l:
    if (solution[l] == item(i)) {
      goto backtrack;
    } else {
      l = try_option(l);
      goto enter_level_l;
    }

  retry_option_l:
    i = retry_option(l);
    goto try_option_l;

  backtrack:
    uncover(item(i));

  leave_level_l:
    if (l > 0) {
      --l;
      goto retry_option_l;
    }
  }

  constexpr void
  visit_solution(ssize_type l, dlx_visitor<T> auto& visitor)
  {
    std::invoke(visitor, l, solution_begin());
  }
};

template <typename I, typename O>
dlx(I&& items, O&& options)->dlx<std::ranges::range_value_t<I>>;

static_assert(std::forward_iterator<typename dlx<int>::item_iterator>);
static_assert(std::forward_iterator<typename dlx<int>::solution_iterator>);
static_assert(dlx_visitor<dlx_visit_all<int>, int>);
static_assert(dlx_item_choice_heuristic<dlx_mrv_heuristic<int>, int>);

}

#endif
