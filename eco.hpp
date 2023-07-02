/*module;*/

#include <bit>
#include <concepts>
#include <cstddef>
#include <cstdlib>
#include <functional>
#include <iterator>
#include <limits>
#include <memory>
#include <ranges>
#include <utility>
#include <variant>

/*export module eco;*/

namespace eco::inline cpp20 {

template <typename T>
struct ssize_type_traits
{
  using type = typename T::ssize_type;
};

/*export*/ template <typename T>
using ssize_type_t = typename ssize_type_traits<T>::type;

/*export*/ struct memory_view : public std::ranges::view_interface<memory_view>
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

/*export*/ [[nodiscard]] constexpr auto
operator==(memory_view x, memory_view y) noexcept -> bool
{
  return x.first == y.first && x.size == y.size;
}

/*export*/ [[nodiscard]] constexpr auto
operator!=(memory_view x, memory_view y) noexcept -> bool
{
  return !(x == y);
}

/*export*/ [[nodiscard]] constexpr auto
operator<(memory_view x, memory_view y) noexcept -> bool
{
  return std::less<void*>{}(x.first, y.first) || (!(std::less<void*>{}(y.first, x.first)) && (x.size < y.size));
}

/*export*/ [[nodiscard]] constexpr auto
operator>=(memory_view x, memory_view y) noexcept -> bool
{
  return !(x < y);
}

/*export*/ [[nodiscard]] constexpr auto
operator>(memory_view x, memory_view y) noexcept -> bool
{
  return y < x;
}

/*export*/ [[nodiscard]] constexpr auto
operator<=(memory_view x, memory_view y) noexcept -> bool
{
  return !(y < x);
}

static_assert(std::totally_ordered<memory_view>);

template <typename T>
concept boolean_testable =
  std::convertible_to<T, bool> &&
  requires (T&& b) {
    { !std::forward<T>(b) } -> std::convertible_to<bool>;
  };

/*export*/ template <typename T>
concept allocator =
  requires(T a, ssize_type_t<memory_view> n)
  {
    { a.allocate(n) } -> std::same_as<memory_view>;
  };

/*export*/ template <typename T>
concept deallocatable_allocator =
  allocator<T> &&
  requires(T& a, memory_view mem)
  {
    { a.deallocate(mem) } noexcept -> boolean_testable;
  };

/*export*/ template <typename T>
concept reallocatable_allocator =
  allocator<T> &&
  requires(T a, memory_view mem, ssize_type_t<memory_view> n)
  {
    { a.reallocate(mem, n) } -> std::same_as<memory_view>;
  };

/*export*/ template <typename T>
concept ownership_aware_allocator =
  allocator<T> &&
  requires(T const& a, memory_view mem)
  {
    { a.is_owner(mem) } noexcept -> boolean_testable;
  };

/*export*/ class malloc_allocator
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
  reallocate(memory_view mem, ssize_type_t<memory_view> n) noexcept -> memory_view
  {
    if (n <= 0) {
      return {};
    }
    auto first{std::realloc(mem.first, n)};
    if (first != nullptr) {
      mem.first = first;
      mem.size = n;
    }
    return mem;
  }

  auto deallocate(memory_view mem) noexcept -> bool
  {
    std::free(mem.first);
    return true;
  }
};

static_assert(deallocatable_allocator<malloc_allocator>);
static_assert(reallocatable_allocator<malloc_allocator>);

/*export*/ template <allocator A = malloc_allocator>
class arena_allocator
{
  A alloc;
  memory_view arena;
  std::byte* pos;

public:
  explicit arena_allocator(A alloc, ssize_type_t<memory_view> n)
    : arena{alloc.allocate(n)}
    , pos{arena.begin()}
  {}

  ~arena_allocator()
  {
    alloc.deallocate(arena);
  }

  arena_allocator(arena_allocator const&) = delete;

  arena_allocator& operator=(arena_allocator const&) = delete;

  [[nodiscard]] auto
  allocate(ssize_type_t<memory_view> n) -> memory_view
  // [[ pre: n <= arena.end() - pos ]]
  {
    if (n <= 0) {
      return {};
    }
    memory_view allocated{pos, n};
    pos += n;
    return allocated;
  }

  auto deallocate(memory_view) noexcept -> bool
  {
    return true;
  }
};

static_assert(deallocatable_allocator<arena_allocator<malloc_allocator>>);

template <typename T>
struct value_type_traits
{
  using type = typename T::value_type;
};

/*export*/ template <typename T>
using value_t = typename value_type_traits<T>::type;

/*export*/ template <typename T>
struct array_copier
{
  using value_type = T;

  template <typename I, typename S>
  requires
    std::forward_iterator<I> &&
    std::sentinel_for<S, I> &&
    std::constructible_from<T, std::iter_reference_t<I>>
  constexpr auto
  copy(I const f, S const l, T* dst) const -> T*
  {
    if constexpr (std::is_trivial_v<T>) {
      return std::copy(f, l, dst);
    } else {
      return std::uninitialized_copy(f, l, dst);
    }
  }

  constexpr auto
  move(T* f, T* l, T* dst) const -> T*
  {
    if constexpr (std::is_trivial_v<T>) {
      return std::copy(f, l, dst);
    } else {
      return std::uninitialized_move(f, l, dst);
    }
  }

  constexpr auto
  move_backward(T* f, T* l, T* dst) const -> T*
  {
    if constexpr (std::is_trivial_v<T>) {
      return std::copy_backward(f, l, dst);
    } else {
      return std::uninitialized_move(
        std::reverse_iterator{l},
        std::reverse_iterator{f},
        std::reverse_iterator{std::advance(dst, std::distance(f, l))}
      );
    }
  }

  constexpr void
  destroy(T* f, T* l) const
  {
    if constexpr (!std::is_trivial_v<T>) {
      std::destroy(std::reverse_iterator{l}, std::reverse_iterator{f});
    }
  }
};

/*export*/ template <typename T>
inline constexpr array_copier<T> default_array_copy{};

struct default_array_growth_algorithm
{
  using ssize_type = ssize_type_t<memory_view>;

  constexpr auto
  operator()(ssize_type capacity, ssize_type lower_bound) const noexcept -> ssize_type
  // [[ pre: capacity + lower_bound >= 0 ]]
  {
    return capacity + std::max(capacity / 2, lower_bound);
  }
};

/*export*/ inline constexpr default_array_growth_algorithm default_array_growth{};

/*export*/ inline malloc_allocator default_array_alloc{};

/*export*/ template
<
  typename T,
  typename Size = ssize_type_t<memory_view>,
  typename Metadata = std::monostate,
  auto copier = default_array_copy<T>,
  auto ga = default_array_growth,
  auto& alloc = default_array_alloc
>
class extent
{
  T* start = nullptr;

  struct header_t
  {
    header_t() = delete;

    Size size;
    Size capacity;
    [[no_unique_address]] Metadata metadata;
    alignas(T) std::byte data;
  };
  // [[ requires axiom: std::is_standard_layout_v<header_t> ]]

  [[nodiscard]] static constexpr auto
  header_byte_size() -> decltype(auto)
  {
    return offsetof(header_t, data);
  }

  [[nodiscard]] constexpr auto
  header() noexcept -> header_t*
  // [[ pre: start ]]
  {
    return reinterpret_cast<header_t*>(reinterpret_cast<std::byte*>(start) - header_byte_size());
  }

  [[nodiscard]] constexpr auto
  header() const noexcept -> header_t const*
  // [[ pre: start ]]
  {
    return reinterpret_cast<header_t*>(reinterpret_cast<std::byte*>(start) - header_byte_size());
  }

  [[nodiscard]] constexpr auto
  size_ref() noexcept -> Size&
  // [[ pre: start ]]
  {
    return header()->size;
  }

  [[nodiscard]] constexpr auto
  size_ref() const noexcept -> Size const&
  // [[ pre: start ]]
  {
    return header()->size;
  }

  [[nodiscard]] constexpr auto
  capacity_ref() noexcept -> Size&
  // [[ pre: start ]]
  {
    return header()->capacity;
  }

  [[nodiscard]] constexpr auto
  capacity_ref() const noexcept -> Size const&
  // [[ pre: start ]]
  {
    return header()->capacity;
  }

  [[nodiscard]] constexpr auto
  metadata_ref() noexcept -> Metadata&
    requires (!std::same_as<Metadata, std::monostate>)
  // [[ pre: start ]]
  {
    return header()->metadata;
  }

  [[nodiscard]] constexpr auto
  // [[ pre: start ]]
  metadata_ref() const noexcept -> Metadata const&
    requires (!std::same_as<Metadata, std::monostate>)
  {
    return header()->metadata;
  }

public:
  [[nodiscard]] explicit constexpr
  operator bool() const noexcept
  {
    return start && size_ref();
  }

  [[nodiscard]] constexpr auto
  begin() noexcept -> T*
  {
    return start;
  }

  [[nodiscard]] constexpr auto
  begin() const noexcept -> T const*
  {
    return start;
  }

  [[nodiscard]] constexpr auto
  end() noexcept -> T*
  {
    return (start ? start + size_ref() : nullptr);
  }

  [[nodiscard]] constexpr auto
  end() const noexcept -> T const*
  {
    return (start ? start + size_ref() : nullptr);
  }

  [[nodiscard]] constexpr auto
  size() const noexcept -> Size
  {
    return (start ? size_ref() : 0);
  }

  [[nodiscard]] constexpr auto
  capacity() const noexcept -> Size
  {
    return (start ? capacity_ref() : 0);
  }

  [[nodiscard]] constexpr auto
  unused_capacity()
  const noexcept -> Size
  {
    return (start ? (capacity_ref() - size_ref()) : 0);
  }

  [[nodiscard]] constexpr auto
  allocated_byte_size() const noexcept -> Size
  {
    return static_cast<Size>(start ? (header_byte_size() + capacity_ref() * sizeof(T)) : 0);
  }

  [[nodiscard]] constexpr auto
  metadata() noexcept -> Metadata*
    requires (!std::same_as<Metadata, std::monostate>)
  {
    return (start ? &metadata_ref() : nullptr);
  }

  [[nodiscard]] constexpr auto
  metadata() const noexcept -> Metadata const*
    requires (!std::same_as<Metadata, std::monostate>)
  {
    return (start ? &metadata_ref() : nullptr);
  }

private:

  [[nodiscard]] auto
  construct(Size new_capacity) const -> T*
  // [[ pre: new_capacity >= 0 ]]
  {
    auto const memory{alloc.allocate(header_byte_size() + new_capacity * sizeof(T))};
    auto new_header{reinterpret_cast<header_t*>(memory.begin())};
    new_header->size = size();
    new_header->capacity = new_capacity;
    if constexpr (!std::same_as<Metadata, std::monostate>) {
      if (start) {
        std::construct_at(&(new_header->metadata), metadata_ref());
      } else {
        std::construct_at(&(new_header->metadata));
      }
    }
    return reinterpret_cast<T*>(memory.begin() + header_byte_size());
  }

  void
  assign(T* new_start)
  {
    auto const old_byte_size{allocated_byte_size()};
    auto old_start{std::exchange(start, new_start)};
    if (old_start) {
      alloc.deallocate({reinterpret_cast<void*>(reinterpret_cast<std::byte*>(old_start) - header_byte_size()), old_byte_size});
    }
  }

  void
  move(Size n_new_elements)
  // [[ pre: n_new_elements >= 0 ]]
  {
    auto new_start{construct(std::invoke(ga, capacity(), n_new_elements))};
    if (start) {
      copier.move(begin(), end(), new_start);
    }
    assign(new_start);
  }

  void
  move(Size n_new_elements, Size offset)
  // [[ pre: n_new_elements >= 0 ]]
  // [[ pre: offset >= 0 && offset <= size() ]]
  {
    auto new_start{construct(std::invoke(ga, capacity(), n_new_elements))};
    if (start) {
      copier.move(start, start + offset, new_start);
      copier.move(start + offset, end(), new_start + offset + n_new_elements);
    }
    assign(new_start);
  }

public:
  ~extent()
  {
    assign(nullptr);
  }

  [[nodiscard]] constexpr
  extent() = default;

  [[nodiscard]] explicit constexpr
  extent(Size capacity)
  {
    start = construct(capacity);
  }

  [[nodiscard]] constexpr
  extent(extent const& x)
    ///requires std::indirectly_copyable<T*, T*>
  {
    if (x.start) {
      start = x.construct(x.size());
      copier.copy(x.begin(), x.end(), start);
      size_ref() = x.size_ref();
      if constexpr (!std::same_as<Metadata, std::monostate>) {
        metadata_ref() = x.metadata_ref();
      }
    }
  }

  constexpr void
  swap(extent& x)
  {
    std::swap(start, x.start);
  }

  constexpr auto
  operator=(extent const& x) -> extent&
    ///requires std::indirectly_copyable<T*, T*>
  {
    if (capacity() < x.size()) { // Strong exception guarantee
      // [[ assert: this != &x ]]
      auto temp{x};
      swap(temp);
      // [[ assert: capacity() >= temp.capacity() ]]
    } else if (start && &x != this) { // Basic exception guarantee
      copier.destroy(begin(), begin() + size_ref());
      size_ref() = 0;
      if constexpr (!std::same_as<Metadata, std::monostate>) {
        metadata_ref() = x.metadata_ref();
      }
      if (x.start) {
        copier.copy(x.begin(), x.begin() + x.size_ref(), begin());
      }
      size_ref() = x.size_ref();
    }
    return *this;
  }

  [[nodiscard]] constexpr
  extent(extent&& x) noexcept
    : start{std::exchange(x.start, nullptr)}
  {}

  constexpr auto
  operator=(extent&& x) noexcept -> extent&
  {
    auto* temp = x.start;
    x.start = nullptr;
    assign(nullptr);
    start = temp;
    return *this;
  }

  [[nodiscard]] friend constexpr auto
  operator==(extent const& x, extent const& y) -> bool
    requires std::indirectly_comparable<T const*, T const*, std::ranges::equal_to>
  {
    return std::equal(x.begin(), x.end(), y.begin(), y.end());
  }

  [[nodiscard]] friend constexpr auto
  operator!=(extent const& x, extent const& y) -> bool
    requires std::indirectly_comparable<T const*, T const*, std::ranges::equal_to>
  {
    return !(x == y);
  }

  [[nodiscard]] friend constexpr auto
  operator<(extent const& x, extent const& y) -> bool
    requires std::indirect_strict_weak_order<std::ranges::less, T const*>
  {
    return std::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
  }

  [[nodiscard]] friend constexpr auto
  operator>=(extent const& x, extent const& y) -> bool
    requires std::indirect_strict_weak_order<std::ranges::less, T const*>
  {
    return !(x < y);
  }

  [[nodiscard]] friend constexpr auto
  operator>(extent const& x, extent const& y) -> bool
    requires std::indirect_strict_weak_order<std::ranges::less, T const*>
  {
    return (y < x);
  }

  [[nodiscard]] friend constexpr auto
  operator<=(extent const& x, extent const& y) -> bool
    requires std::indirect_strict_weak_order<std::ranges::less, T const*>
  {
    return !(y < x);
  }

  constexpr void
  adjust_unused_capacity(Size n)
  // [[ pre: n => 0 ]]
  // [[ post: unused_capacity() == n ]]
  {
    if (unused_capacity() != n) {
      extent temp{size() + n};
      // [[ assert: temp ]]
      copier.move(begin(), end(), temp.start);
      if (start) {
        temp.size_ref() = size_ref();
        if constexpr (!std::same_as<Metadata, std::monostate>) {
          temp.metadata_ref() = metadata_ref();
        }
      }
      swap(temp);
    }
  }

  template <typename... Args>
    requires std::constructible_from<T, Args...>
  constexpr auto
  push_back(Args &&...args) -> T&
  // [[ post: bool{*this} ]]
  {
    if (unused_capacity() == 0) {
      adjust_unused_capacity(1);
    }
    // [[ assert: unused_capacity() > 0 ]]
    std::ranges::construct_at(end(), std::forward<Args>(args)...);
    ++size_ref();
    return *(end() - 1);
  }

  constexpr void
  pop_back() noexcept
  // [[ pre: bool{*this} ]]
  {
    --size_ref();
    copier.destroy(end(), end() + 1);
  }

  template <typename Writer>
  constexpr auto
  insert_space(T* insertion_point, Size n_elements, Writer writer) -> T*
  {
    if (n_elements <= 0) {
      return insertion_point;
    }
    auto const offset{insertion_point - start};
    if (unused_capacity() < n_elements) {
      move(n_elements, offset);
    } else {
      auto new_end{end() + n_elements};
      copier.move_backward(start + offset, end(), new_end);
    }
    // [[ assert: start ]]
    writer(start + offset);
    size_ref() += n_elements;
    return start + offset;
  }

  template <typename Writer>
  constexpr auto
  insert_space(Size n_elements, Writer writer) -> T*
  {
    if (n_elements <= 0) {
      return end();
    }
    if (unused_capacity() < n_elements) {
      move(n_elements);
    }
    auto insertion_point{end()};
    writer(insertion_point);
    size_ref() += n_elements;
    return insertion_point;
  }

  constexpr void
  erase_space(T* first, Size n_elements)
  // [[ pre: n_elements <= size() ]]
  {
    if (n_elements > 0) {
      auto last{first + n_elements};
      auto new_size{size() - n_elements};
      auto old_end{end()};
      size_ref() = new_size;
      if (new_size <= 0) {
        copier.destroy(first, last);
      } else {
        copier.move(last, old_end, first);
        if (end() < last) {
          copier.destroy(end(), last);
        }
      }
    }
  }
};

static_assert(std::totally_ordered<extent<int>>);
static_assert(std::ranges::contiguous_range<extent<int>>);

template <typename T>
struct iterator_type_traits
{
  using type = typename T::iterator_type;
};

/*export*/ template <typename T>
using iterator_t = typename iterator_type_traits<T>::type;

/*export*/ template <typename T>
concept forward_linker =
  std::forward_iterator<iterator_t<T>> &&
  std::invocable<T, iterator_t<T>, iterator_t<T>>;

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

/*export*/ inline constexpr reverse_append_impl reverse_append{};

/*export*/ template
<
  typename T,
  typename Size,
  auto ga = default_array_growth,
  auto& alloc = default_array_alloc
>
class forward_list_pool
{
public:
  using value_type = T;
  using ssize_type = Size;

  [[nodiscard]] constexpr auto
  limit() const noexcept -> ssize_type
  {
    return Size{-1};
  }

  constexpr
  forward_list_pool()
    : pool{1}
  {
    static_assert(std::signed_integral<ssize_type>);
    free_list_head() = limit();
  }

  explicit constexpr
  forward_list_pool(ssize_type x)
    : pool{x}
  {
    static_assert(std::signed_integral<ssize_type>);
    free_list_head() = limit();
  }

private:
  struct node_t
  {
    ssize_type next;
    T value;
  };

  extent<node_t, ssize_type, ssize_type, default_array_copy<node_t>, ga, alloc> pool;

  [[nodiscard]] constexpr auto
  free_list_head() noexcept -> ssize_type&
  {
    return *(pool.metadata());
  }

  [[nodiscard]] constexpr auto
  free_list_head() const noexcept -> ssize_type const&
  {
    return *(pool.metadata());
  }

  [[nodiscard]] constexpr auto
  node(ssize_type x) noexcept -> node_t&
  {
    return *(pool.begin() + x);
  }

  [[nodiscard]] constexpr auto
  node(ssize_type x) const noexcept -> node_t const&
  {
    return *(pool.begin() + x);
  }

  [[nodiscard]] constexpr auto
  new_node() -> ssize_type
  {
    pool.push_back(node_t{});
    return ssize_type(pool.size() - 1);
  }

  constexpr void
  set_next(ssize_type x, ssize_type y) noexcept
  {
    node(x).next = y - x;
  }

public:
  [[nodiscard]] constexpr auto
  value(ssize_type x) noexcept-> T&
  {
    return node(x).value;
  }

  [[nodiscard]] constexpr auto
  value(ssize_type x) const noexcept -> T const&
  {
    return node(x).value;
  }

  [[nodiscard]] constexpr auto
  next(ssize_type x) const noexcept -> const ssize_type
  {
    return x + node(x).next;
  }

  [[nodiscard]] constexpr auto
  allocate_node(T const& val, ssize_type tail) -> ssize_type
  {
    auto new_list = free_list_head();

    if (free_list_head() == limit()) {
      new_list = new_node();
    } else {
      free_list_head() = next(free_list_head());
    }

    set_next(new_list, tail);
    value(new_list) = val;
    return new_list;
  }

  constexpr auto
  free_node(ssize_type x) noexcept -> ssize_type
  // [[ pre: "x is a list head" ]]
  {
    auto tail = next(x);
    set_next(x, free_list_head());
    free_list_head() = x;
    return tail;
  }

  struct next_linker;

  class iterator
  {
    node_t* node;

  public:
    using value_type = forward_list_pool::value_type;
    using difference_type = forward_list_pool::ssize_type;

    constexpr iterator() noexcept = default;

    explicit constexpr
    iterator(forward_list_pool& p) noexcept
      : node{p.end()}
    {}

    constexpr
    iterator(forward_list_pool& p, ssize_type_t<forward_list_pool> node) noexcept
      : node{p.pool.begin() + node}
    {}

    [[nodiscard]] constexpr auto
    operator*() const noexcept -> value_type&
    {
      return node->value;
    }

    constexpr auto
    operator++() noexcept -> iterator&
    {
      node += node->next;
      return *this;
    }

    [[nodiscard]] constexpr auto
    operator++(int) noexcept -> iterator
    {
      auto temp{*this};
      ++(*this);
      return temp;
    }

    friend constexpr auto
    operator==(iterator const& x, iterator const& y) noexcept -> bool
    {
      return x.node == y.node;
    }

    friend constexpr auto
    operator!=(iterator const& x, iterator const& y) noexcept -> bool
    {
      return !(x == y);
    }

    friend next_linker;
  };

  struct next_linker
  {
    using iterator_type = iterator;

    void operator()(iterator i, iterator j)
    {
      i.node->next = static_cast<Size>(j.node - i.node);
    }
  };

  using iterator_type = iterator;
};

static_assert(std::forward_iterator<forward_list_pool<int, int>::iterator>);

static_assert(forward_linker<forward_list_pool<int, int>::next_linker>);

/*export*/ template <typename T, typename Size>
constexpr void
free_list(forward_list_pool<T, Size>& pool, ssize_type_t<forward_list_pool<T, Size>> x) noexcept
{
  while (x != pool.limit()) x = pool.free_node(x);
}

/*export*/ template <typename T>
concept backward_linker =
  std::bidirectional_iterator<iterator_t<T>> &&
  std::invocable<T, iterator_t<T>, iterator_t<T>>;

/*export*/ template
<
  typename T,
  typename Size,
  auto ga = default_array_growth,
  auto& alloc = default_array_alloc
>
class list_pool
{
public:
  using value_type = T;
  using ssize_type = Size;

  [[nodiscard]] constexpr auto
  limit() const noexcept -> ssize_type
  {
    return Size{-1};
  }

  constexpr
  list_pool()
    : pool{1}
  {
    static_assert(std::signed_integral<ssize_type>);
    free_list_head() = limit();
  }

  explicit constexpr
  list_pool(ssize_type x)
    : pool{x}
  {
    static_assert(std::signed_integral<ssize_type>);
    free_list_head() = limit();
  }

private:
  struct node_t
  {
    ssize_type prev;
    ssize_type next;
    T value;
  };

  extent<node_t, ssize_type, ssize_type, default_array_copy<node_t>, ga, alloc> pool;

  [[nodiscard]] constexpr auto
  free_list_head() noexcept -> ssize_type&
  {
    return *(pool.metadata());
  }

  [[nodiscard]] constexpr auto
  free_list_head() const noexcept -> ssize_type const&
  {
    return *(pool.metadata());
  }

  [[nodiscard]] constexpr auto
  node(ssize_type x) noexcept -> node_t&
  {
    return *(pool.begin() + x);
  }

  [[nodiscard]] constexpr auto
  node(ssize_type x) const noexcept -> node_t const&
  {
    return *(pool.begin() + x);
  }

  [[nodiscard]] constexpr auto
  new_node() -> ssize_type
  {
    pool.push_back(node_t{});
    return ssize_type(pool.size() - 1);
  }

  constexpr void
  set_prev(ssize_type x, ssize_type y) noexcept
  {
    node(x).prev = y - x;
  }

  constexpr void
  set_next(ssize_type x, ssize_type y) noexcept
  {
    node(x).next = y - x;
  }

public:
  [[nodiscard]] constexpr auto
  value(ssize_type x) noexcept-> T&
  {
    return node(x).value;
  }

  [[nodiscard]] constexpr auto
  value(ssize_type x) const noexcept -> T const&
  {
    return node(x).value;
  }

  [[nodiscard]] constexpr auto
  prev(ssize_type x) const noexcept -> const ssize_type
  {
    return x + node(x).prev;
  }

  [[nodiscard]] constexpr auto
  next(ssize_type x) const noexcept -> const ssize_type
  {
    return x + node(x).next;
  }

  [[nodiscard]] constexpr auto
  allocate_node(T const& val, ssize_type tail) -> ssize_type
  {
    auto new_list = free_list_head();
    if (free_list_head() == limit()) {
      new_list = new_node();
    } else {
      free_list_head() = next(free_list_head());
    }
    set_prev(new_list, limit());
    set_next(new_list, tail);
    value(new_list) = val;
    if (tail != limit()) {
      set_prev(tail, new_list);
    }
    return new_list;
  }

  constexpr auto
  free_node(ssize_type x) noexcept -> ssize_type
  {
    auto previous = prev(x);
    if (previous != limit()) {
      set_next(previous, limit());
    }
    auto tail = next(x);
    if (tail != limit()) {
      set_prev(tail, limit());
    }
    set_next(x, free_list_head());
    free_list_head() = x;
    return tail;
  }

  constexpr void
  unlink_node(ssize_type x) noexcept
  // [[ pre: prev(x) != limit() && next(x) != limit ]]
  {
    node(prev(x)).next += node(x).next;
    node(next(x)).prev += node(x).prev;
  }

  constexpr void
  relink_node(ssize_type x) noexcept
  // [[ pre: prev(x) != limit() && next(x) != limit ]]
  {
    node(prev(x)).next -= node(x).next;
    node(next(x)).prev -= node(x).prev;
  }

  struct prev_linker;
  struct next_linker;
  struct linker;

  class iterator
  {
    node_t* node;

  public:
    using value_type = list_pool::value_type;
    using difference_type = list_pool::ssize_type;

    constexpr iterator() noexcept = default;

    explicit constexpr
    iterator(list_pool& p) noexcept
      : node{p.limit()}
    {}

    constexpr
    iterator(list_pool& p, ssize_type_t<list_pool> node) noexcept
      : node{p.pool.begin() + node}
    {}

    [[nodiscard]] constexpr auto
    operator*() const noexcept -> value_type&
    {
      return node->value;
    }

    constexpr auto
    operator++() noexcept -> iterator&
    {
      node += node->next;
      return *this;
    }

    [[nodiscard]] constexpr auto
    operator++(int) noexcept -> iterator
    {
      auto temp{ *this };
      ++(*this);
      return temp;
    }

    constexpr auto
    operator--() noexcept -> iterator&
    {
      node += node->prev;
      return *this;
    }

    [[nodiscard]] constexpr auto
    operator--(int) noexcept -> iterator
    {
      auto temp{ *this };
      --(*this);
      return temp;
    }

    friend constexpr auto
    operator==(iterator const& x, iterator const& y) noexcept -> bool
    {
      return x.node == y.node;
    }

    friend constexpr auto
    operator!=(iterator const& x, iterator const& y) noexcept -> bool
    {
      return !(x == y);
    }

    friend prev_linker;
    friend next_linker;
    friend linker;
  };

  struct prev_linker
  {
    using iterator_type = iterator;

    constexpr
    void operator()(iterator i, iterator j) noexcept
    {
      i.node->prev = static_cast<Size>(j.node - i.node);
    }
  };

  struct next_linker
  {
    using iterator_type = iterator;

    constexpr void
    operator()(iterator i, iterator j) noexcept
    {
      i.node->next = static_cast<Size>(j.node - i.node);
    }
  };

  struct linker
  {
    using iterator_type = iterator;

    constexpr void
    operator()(iterator i, iterator j) noexcept
    {
      i.node->prev = static_cast<Size>(j.node - i.node);
      i.node->next = static_cast<Size>(j.node - i.node);
    }
  };

  using iterator_type = iterator;
};

static_assert(std::bidirectional_iterator<list_pool<int, int>::iterator>);

static_assert(forward_linker<list_pool<int, int>::next_linker>);

/*export*/ template <typename T, typename Size>
constexpr void
free_list(list_pool<T, Size>& pool, ssize_type_t<list_pool<T, Size>> x) noexcept
{
  while (x != pool.limit()) x = pool.free_node(x);
}

/*export*/ template
<
  typename T,
  auto ga = default_array_growth,
  auto& alloc = default_array_alloc
>
class array
{
public:
  using value_type = T;
  using ssize_type = ssize_type_t<memory_view>;

private:
  extent<T, ssize_type, std::monostate, default_array_copy<T>, ga, alloc> header;

  template <typename I, typename S>
  struct writer
  {
    I first;
    S last;

    [[nodiscard]] explicit
    writer(I f, S l)
      : first{f}, last{l}
    {}

    constexpr void
    operator()(T* dst)
      ///requires std::indirectly_copyable<T*, T*>
    {
      default_array_copy<T>.copy(first, last, dst);
    }
  };

  [[nodiscard]] constexpr auto
  is_full() const noexcept -> bool
  {
    return (header.unused_capacity() == 0);
  }

public:
  [[nodiscard]] constexpr
  array() noexcept = default;

  [[nodiscard]] explicit constexpr
  array(ssize_type capacity)
    : header{capacity}
  {}

  template <typename R>
    requires
      (!std::same_as<R, array>) &&
      std::ranges::forward_range<R> &&
      std::constructible_from<T, std::ranges::range_value_t<R>>
  [[nodiscard]] explicit constexpr
  array(R&& range) noexcept(std::is_nothrow_copy_constructible_v<std::ranges::range_value_t<R>>)
  // [[ post: size() == std::ranges::ssize(range) ]]
  // [[ post: is_full() ]]
    : header{std::distance(std::ranges::begin(range), std::ranges::end(range))}
  {
    header.insert_space(
      capacity(),
      writer{std::ranges::begin(range), std::ranges::end(range)}
    );
  }

  template <typename R>
    requires
      (!std::same_as<R, array>) &&
      std::ranges::forward_range<R> &&
      std::constructible_from<T, std::ranges::range_value_t<R>>
  constexpr auto
  operator=(R&& range) noexcept(std::is_nothrow_copy_constructible_v<std::ranges::range_value_t<R>>) -> array&
  {
    auto const n_elements{std::distance(std::ranges::begin(range), std::ranges::end(range))};
    if (capacity() < n_elements) {
      array temp{range};
      swap(temp);
      // [[ assert: is_full() ]]
    } else if (header) {
      header.erase_space(header.begin(), header.size());
      header.insert_space(n_elements, writer{std::ranges::begin(range), std::ranges::end(range)});
      // [[ assert: capacity() >= size() ]]
    }
    return *this;
  }

  [[nodiscard]] friend constexpr auto
  operator==(array const& x, array const& y) noexcept -> bool
    requires std::indirectly_comparable<T const*, T const*, std::ranges::equal_to>
  {
    return x.header == y.header;
  }

  [[nodiscard]] friend constexpr auto
  operator!=(array const& x, array const& y) noexcept -> bool
    requires std::indirectly_comparable<T const*, T const*, std::ranges::equal_to>
  {
    return x.header != y.header;
  }

  [[nodiscard]] friend constexpr auto
  operator<(array const& x, array const& y) noexcept -> bool
    requires std::indirect_strict_weak_order<std::ranges::less, T const*>
  {
    return x.header < y.header;
  }

  [[nodiscard]] friend constexpr auto
  operator>=(array const& x, array const& y) noexcept -> bool
    requires std::indirect_strict_weak_order<std::ranges::less, T const*>
  {
    return x.header >= y.header;
  }

  [[nodiscard]] friend constexpr auto
  operator>(array const& x, array const& y) noexcept -> bool
    requires std::indirect_strict_weak_order<std::ranges::less, T const*>
  {
    return x.header > y.header;
  }

  [[nodiscard]] friend constexpr auto
  operator<=(array const& x, array const& y) noexcept -> bool
    requires std::indirect_strict_weak_order<std::ranges::less, T const*>
  {
    return x.header <= y.header;
  }

  constexpr void
  swap(array& x) noexcept
  {
    header.swap(x.header);
  }

  [[nodiscard]] explicit constexpr
  operator bool() const noexcept
  {
    return static_cast<bool>(header);
  }

  [[nodiscard]] constexpr auto
  operator[](ssize_type n) noexcept -> T&
  // [[ pre: n < size() ]]
  {
    return *(header.begin() + n);
  }

  [[nodiscard]] constexpr auto
  operator[](ssize_type n) const noexcept -> T const&
  // [[ pre: n < size() ]]
  {
    return *(header.begin() + n);
  }

  [[nodiscard]] constexpr auto
  begin() noexcept -> T*
  {
    return header.begin();
  }

  [[nodiscard]] constexpr auto
  begin() const noexcept -> T const*
  {
    return header.begin();
  }

  [[nodiscard]] constexpr auto
  end() noexcept -> T*
  {
    return header.end();
  }

  [[nodiscard]] constexpr auto
  end() const noexcept -> T const*
  {
    return header.end();
  }

  [[nodiscard]] constexpr auto
  size() const noexcept -> ssize_type
  {
    return header.size();
  }

  [[nodiscard]] constexpr auto
  capacity() const noexcept -> ssize_type
  {
    return header.capacity();
  }

  [[nodiscard]] constexpr auto
  max_size() const noexcept -> ssize_type
  {
    return std::numeric_limits<ssize_type>::max() / sizeof(T);
  }

  constexpr void
  reserve(ssize_type cap)
  // [[ post: capacity() >= cap ]]
  {
    if (capacity() < cap) {
      header.adjust_unused_capacity(cap - size());
    }
  }

  constexpr void
  shrink_to_fit()
  // [[ assert: is_full() ]]
  {
    if (!is_full()) {
      if (*this) {
        header.adjust_unused_capacity(0);
      } else {
        header = {};
      }
    }
  }

  template <typename... Args>
    requires std::constructible_from<T, Args...>
  constexpr auto
  push_back(Args &&...args) -> T&
  // [[ post: bool{*this} ]]
  {
    return header.push_back(std::forward<Args>(args)...);
  }

  constexpr void
  pop_back() noexcept
  // [[ pre: bool{*this} ]]
  {
    header.pop_back();
  }

  template <typename R>
    requires
      std::ranges::forward_range<R> &&
      std::constructible_from<T, std::ranges::range_value_t<R>>
  constexpr auto
  append(R&& range) -> T*
  // [[ pre axiom: "range not overlapped with [begin(), end())" ]]
  {
    auto const n_elements{std::ranges::distance(range)};
    reserve(size() + n_elements);
    return header.insert_space(n_elements, writer{std::ranges::begin(range), std::ranges::end(range)});
  }

  template <typename... Args>
    requires std::constructible_from<T, Args...>
  constexpr auto
  insert(T const* pos, Args &&...args) -> T*
  // [[ pre axiom: "pos within [begin(), end())" ]]
  // [[ post axiom: bool{*this} ]]
  {
    auto df{pos - begin()};
    auto dm{size()};
    push_back(std::forward<Args>(args)...);
    return std::rotate(begin() + df, begin() + dm, end());
  }

  template <typename R>
    requires
      std::ranges::forward_range<R> &&
      std::constructible_from<T, std::ranges::range_value_t<R>>
  constexpr auto
  insert(T const* pos, R&& range) -> T*
  // [[ pre axiom: "pos within [begin(), end())" ]]
  // [[ pre axiom: "range not overlapped with [begin(), end())" ]]
  {
    auto const df{pos - begin()};
    auto const dm{size()};
    append(std::forward<R>(range));
    return std::rotate(begin() + df, begin() + dm, end());
  }

  constexpr auto
  erase(T const* pos) noexcept -> T*
  // [[ pre: bool{*this} ]]
  // [[ pre axiom: "pos within [begin(), end())" ]]
  // [[ post axiom: "capacity() unchanged" ]]
  {
    header.erase_space(const_cast<T*>(pos), 1);
    return const_cast<T*>(pos);
  }

  template <typename R>
    requires
      std::ranges::range<R> &&
      std::same_as<T, std::ranges::range_value_t<R>>
  constexpr auto erase(R&& range) noexcept -> T*
  // [[ pre axiom: "range within [begin(), end())" ]]
  // [[ post axiom: "capacity() unchanged" ]]
  {
    auto const pos{std::ranges::begin(range)};
    header.erase_space(pos, std::distance(std::ranges::begin(range), std::ranges::end(range)));
    return pos;
  }

  constexpr void
  clear() noexcept
  // [[ post axiom: "capacity() unchanged" ]]
  {
    header.erase_space(begin(), size());
  }
};

/*export*/ template <std::ranges::forward_range R>
array(R&& range)->array<std::ranges::range_value_t<R>>;

static_assert(std::ranges::contiguous_range<array<int>>);
static_assert(std::totally_ordered<array<int>>);

/*export*/ template <typename T, auto ga, auto& alloc>
constexpr void
swap(array<T, ga, alloc>& x, array<T, ga, alloc>& y) noexcept
{
  x.swap(y);
}

/*export*/ template <typename T>
concept bitvector =
  std::regular<T> &&
  std::constructible_from<ssize_type_t<T>> &&
  requires (T b, T const& cb, ssize_type_t<T> i) {
    { cb.size() } -> std::same_as<ssize_type_t<T>>;
    { cb.bitread(i) } -> boolean_testable;
    b.bitset(i);
    b.bitclear(i);
    b.init();
    { cb.rank_0(i) } -> std::same_as<ssize_type_t<T>>;
    { cb.rank_1(i) } -> std::same_as<ssize_type_t<T>>;
    { cb.select_0(i) } -> std::same_as<ssize_type_t<T>>;
    { cb.select_1(i) } -> std::same_as<ssize_type_t<T>>;
  };

/*export*/ template
<
  typename Word = unsigned int,
  typename Size = ssize_type_t<memory_view>
>
class basic_bitvector
{
public:
  using ssize_type = Size;

private:
  extent<Word, ssize_type, ssize_type, default_array_copy<Word>, default_array_growth, default_array_alloc> words;

  [[nodiscard]] constexpr auto
  w() const noexcept -> unsigned char
  {
    return 8 * sizeof(Word);
  }

public:
  [[nodiscard]] constexpr
  basic_bitvector() noexcept = default;

  [[nodiscard]] explicit constexpr
  basic_bitvector(ssize_type size)
    : words{static_cast<ssize_type>((size + (w() - 1)) / (w()))}
  {
    if (size == 0) return;
    *words.metadata() = size;
    size = words.capacity();
    while (size != 0) {
      words.push_back(0);
      --size;
    }
  }

  constexpr auto operator<=>(basic_bitvector const&) const noexcept = default;

  constexpr
  void init() noexcept
  {}

  [[nodiscard]] constexpr auto
  size() const noexcept -> ssize_type
  {
    if (words) return *words.metadata(); else return 0;
  }

  [[nodiscard]] constexpr auto
  bitread(ssize_type i) const noexcept -> bool
  {
    return *(words.begin() + (i / w())) >> (i % w()) & 1u;
  }

  constexpr void
  bitset(ssize_type i) noexcept
  {
    *(words.begin() + (i / w())) |= (1u << (i % w()));
  }

  constexpr void
  bitclear(ssize_type i) noexcept
  {
    *(words.begin() + (i / w())) &= ~(1u << (i % w()));
  }

  [[nodiscard]] constexpr auto
  rank_0(ssize_type i) const noexcept -> ssize_type
  {
    return i - rank_1(i);
  }

  [[nodiscard]] constexpr auto
  rank_1(ssize_type i) const noexcept -> ssize_type
  {
    auto quot = i / w();
    auto rem = i % w();
    ssize_type ret{};
    auto j = 0;
    while (j != quot) {
      ret += std::popcount(*(words.begin() + j));
      ++j;
    }
    ret += (std::popcount(*(words.begin() + j) & ((1u << rem) - 1u)));
    return ret;
  }

  [[nodiscard]] constexpr auto
  select_0(ssize_type i) const noexcept -> ssize_type
  {
    auto quot = i / w();
    ssize_type ret{};
    auto j = 0;
    auto next = 0;
    while (j != quot) {
      next = sizeof(Word) - std::popcount(*(words.begin() + j));
      if (ret + next > i) break;
      ret += next;
      ++j;
    }
    j *= w();
    while (j != size()) {
      if (!bitread(j)) ++ret;
      if (ret > i) return j;
      ++j;
    }
    return j;
  }

  [[nodiscard]] constexpr auto
  select_1(ssize_type i) const noexcept -> ssize_type
  {
    auto quot = i / w();
    ssize_type ret{};
    auto j = 0;
    auto next = 0;
    while (j != quot) {
      next = std::popcount(*(words.begin() + j));
      if (ret + next > i) break;
      ret += next;
      ++j;
    }
    j *= w();
    while (j != size()) {
      if (bitread(j)) ++ret;
      if (ret > i) return j;
      ++j;
    }
    return j;
  }
};

static_assert(bitvector<basic_bitvector<unsigned int, ssize_type_t<memory_view>>>);

struct succ_0_impl
{
  /*export*/ template <bitvector B>
  [[nodiscard]] constexpr auto
  operator()(B const& b, ssize_type_t<B> i) const noexcept -> ssize_type_t<B>
  {
    return b.select_0(b.rank_0(i));
  }
};

/*export*/ inline constexpr succ_0_impl succ_0{};

struct succ_1_impl
{
  /*export*/ template <bitvector B>
  [[nodiscard]] constexpr auto
  operator()(B const& b, ssize_type_t<B> i) const noexcept -> ssize_type_t<B>
  {
    return b.select_1(b.rank_1(i));
  }
};

/*export*/ inline constexpr succ_1_impl succ_1{};

struct pred_0_impl
{
  /*export*/ template <bitvector B>
  [[nodiscard]] constexpr auto
  operator()(B const& b, ssize_type_t<B> i) const noexcept -> ssize_type_t<B>
  {
    return b.select_0(b.rank_0(i + 1) - 1);
  }
};

/*export*/ inline constexpr pred_0_impl pred_0{};

struct pred_1_impl
{
  /*export*/ template <bitvector B>
  [[nodiscard]] constexpr auto
  operator()(B const& b, ssize_type_t<B> i) const noexcept -> ssize_type_t<B>
  {
    return b.select_1(b.rank_1(i + 1) - 1);
  }
};

/*export*/ inline constexpr pred_1_impl pred_1{};

template <typename T>
struct weight_type_traits
{
  using type = typename T::weight_type;
};

/*export*/ template <typename T>
using weight_type_t = typename weight_type_traits<T>::type;

/*export*/ template <typename T>
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

/*export*/ template <typename T>
concept bidirectional_bicursor =
  bicursor<T> &&
  requires (T cur) {
    { has_predecessor(cur) } -> boolean_testable;
    { predecessor(cur) } -> std::same_as<T>;
  };

/*export*/ template <typename T>
concept linked_bicursor =
  bicursor<T> &&
  requires (T cur) {
    set_left_successor(cur, cur);
    set_right_successor(cur, cur);
  };

template <bitvector B = basic_bitvector<unsigned int, ssize_type_t<memory_view>>>
/*export*/ class louds
{
  B bits;

public:
  using ssize_type = ssize_type_t<memory_view>;
  using index_type = ssize_type_t<memory_view>;

  constexpr louds() noexcept = default;

  /*export*/ template <linked_bicursor cur>
  constexpr
  louds(cur root, cur limit, ssize_type n)
    : bits{2 * n + 1}
  {
    auto head{root};
    auto tail{root};
    bits.bitset(0);
    ssize_type i{2};
    ssize_type j{0};
    while (head) {
      set_right_successor(tail, left_successor(head));
      while (has_right_successor(tail)) {
        bits.bitset(i);
        ++i;
        tail = right_successor(tail);
      }
      ++i;
      cur parent{head};
      head = right_successor(head);
      if (!bits.bitread(j + 1)) {
        set_right_successor(parent, limit);
        ++j;
      }
      ++j;
    }
    bits.init();
  }

  [[nodiscard]] constexpr auto
  root() const noexcept -> ssize_type
  {
    return 2;
  }

  [[nodiscard]] constexpr auto
  first_child(ssize_type v) const noexcept -> ssize_type
  // [[ pre: !isleaf(v) ]]
  {
    return child(v, 0);
  }

  [[nodiscard]] constexpr auto
  // [[ pre: !isleaf(v) ]]
  last_child(ssize_type v) const noexcept -> ssize_type
  {
    return child(v, children(v) - 1);
  }

  [[nodiscard]] constexpr auto
  next_sibling(ssize_type v) const noexcept -> ssize_type
  // [[ pre: v != root() && v != lchild(parent(v)) ]]
  {
    return succ_0(bits, v) + 1;
  }

  [[nodiscard]] constexpr auto
  prev_sibling(ssize_type v) const noexcept -> ssize_type
  // [[ pre: v != root() && v != fchild(parent(v)) ]]
  {
    return pred_0(bits, v - 2) + 1;
  }

  [[nodiscard]] constexpr auto
  parent(ssize_type v) const noexcept -> ssize_type
  // [[ pre: v != root() ]]
  {
    const auto j = bits.select_1(bits.rank_0(v - 1));
    return pred_0(bits, j) + 1;
  }

  [[nodiscard]] constexpr auto
  is_leaf(ssize_type v) const noexcept -> bool
  {
    return !bits.bitread(v);
  }

  [[nodiscard]] constexpr auto
  nodemap(ssize_type v) const noexcept -> index_type
  {
    return bits.rank_0(v - 1);
  }

  [[nodiscard]] constexpr auto
  nodeselect(index_type i) const noexcept -> ssize_type
  {
    return bits.select_0(i) + 1;
  }

  [[nodiscard]] constexpr auto
  children(ssize_type v) const noexcept -> ssize_type
  {
    return succ_0(bits, v) - v;
  }

  [[nodiscard]] constexpr auto
  child(ssize_type v, ssize_type n) const noexcept -> ssize_type
  // [[ pre: n >= 0 && n < children(v) ]]
  {
    return bits.select_0(bits.rank_1(v + n)) + 1;
  }

  [[nodiscard]] constexpr auto
  child_rank(ssize_type v) const noexcept -> ssize_type
  // [[ pre: v != root() ]]
  {
    const auto j = bits.select_1(bits.rank_0(v) - 1);
    return j - pred_0(bits, j) - 1;
  }
};

struct lca_impl
{
  template <bitvector B = basic_bitvector<unsigned int, ssize_type_t<memory_view>>>
  [[nodiscard]] constexpr auto
  operator()(louds<B> const& t, ssize_type_t<louds<B>> u, ssize_type_t<louds<B>> v) const noexcept -> ssize_type_t<louds<B>>
  {
    while (u != v) {
      if (u > v) u = t.parent(u); else v = t.parent(v);
    }
    return u;
  }
};

/*export*/ inline constexpr lca_impl lca{};

template <typename T>
struct index_type_traits
{
  using type = typename T::index_type;
};

/*export*/ template <typename T>
using index_type_t = typename index_type_traits<T>::type;

template <bitvector B = basic_bitvector<unsigned int, ssize_type_t<memory_view>>>
/*export*/ class binary_louds
{
  B bits;

public:
  using ssize_type = ssize_type_t<memory_view>;
  using index_type = ssize_type_t<memory_view>;

  constexpr binary_louds() noexcept = default;

  /*export*/ template <bidirectional_bicursor cur>
  constexpr
  binary_louds(cur root, ssize_type n)
    : bits{2 * n}
  {
    if (!root) return;

    list_pool<cur, ssize_type> queue;
    auto head = queue.allocate_node(root, queue.limit());
    auto tail = head;
    ssize_type i{0};
    while (tail != queue.limit()) {
      root = queue.value(tail);
      if (has_left_successor(root)) {
        head = queue.allocate_node(left_successor(root), head);
        bits.bitset(i);
      }
      ++i;
      if (has_right_successor(root)) {
        head = queue.allocate_node(right_successor(root), head);
        bits.bitset(i);
      }
      ++i;
      auto prev = queue.prev(tail);
      queue.free_node(tail);
      tail = prev;
    }
    bits.init();
  }

  [[nodiscard]] constexpr auto
  root() const noexcept -> ssize_type
  {
    return 0;
  }

  [[nodiscard]] constexpr auto
  parent(ssize_type v) const noexcept -> ssize_type
  // [[ pre: v != root() ]]
  {
    return bits.select_1(v - 1) / 2;
  }

  [[nodiscard]] constexpr auto
  has_left_child(ssize_type v) const noexcept -> bool
  {
    return bits.bitread(2 * v);
  }

  [[nodiscard]] constexpr auto
  has_right_child(ssize_type v) const noexcept -> bool
  {
    return bits.bitread(2 * v + 1);
  }

  [[nodiscard]] constexpr auto
  is_leaf(ssize_type v) const noexcept -> bool
  {
    return !has_left_child(v) && !has_right_child(v);
  }

  [[nodiscard]] constexpr auto
  left_child(ssize_type v) const noexcept -> index_type
  // [[ pre: !has_left_child(v) ]]
  {
    return bits.rank_1(2 * v) + 1;
  }

  [[nodiscard]] constexpr auto
  // [[ pre: !has_right_child(v) ]]
  right_child(ssize_type v) const noexcept -> index_type
  {
    return bits.rank_1(2 * (v + 1));
  }

  [[nodiscard]] constexpr auto
  child_label(ssize_type v) const noexcept -> ssize_type
  {
    if (v == 0) return ssize_type{-1};
    return (bits.select_1(v - 1) % 2);
  }
};

template <bitvector B = basic_bitvector<unsigned int, ssize_type_t<memory_view>>>
class binary_louds_cursor
{
  binary_louds<B> const* tree = nullptr;
  ssize_type_t<binary_louds<B>> node{};

public:
  using weight_type = decltype(node);

  [[nodiscard]] constexpr
  binary_louds_cursor() noexcept = default;

  [[nodiscard]] explicit constexpr
  binary_louds_cursor(binary_louds<B> const& t) noexcept
    : tree{&t}
    , node{t.root()}
  {}

  [[nodiscard]] constexpr
  binary_louds_cursor(binary_louds<B> const& t, weight_type v) noexcept
    : tree{&t}
    , node{v}
  {}

  [[nodiscard]] constexpr auto
  operator==(binary_louds_cursor const&) const noexcept -> bool = default;

  [[nodiscard]] explicit constexpr
  operator bool() const
  {
    return tree != nullptr;
  }

  [[nodiscard]] friend constexpr auto
  has_left_successor(binary_louds_cursor i) noexcept -> bool
  // [[ pre: static_cast<bool>(i) ]]
  {
    return i.tree->has_left_child(i.node);
  }

  [[nodiscard]] friend constexpr auto
  has_right_successor(binary_louds_cursor i) noexcept -> bool
  // [[ pre: static_cast<bool>(i) ]]
  {
    return i.tree->has_right_child(i.node);
  }

  [[nodiscard]] friend constexpr auto
  left_successor(binary_louds_cursor i) noexcept -> binary_louds_cursor<B>
  // [[ pre: static_cast<bool>(*this) && has_left_child() ]]
  {
    return {*i.tree, i.tree->left_child(i.node)};
  }

  [[nodiscard]] friend constexpr auto
  right_successor(binary_louds_cursor i) noexcept -> binary_louds_cursor<B>
  // [[ pre: static_cast<bool>(i) && has_right_child() ]]
  {
    return {*i.tree, i.tree->right_child(i.node)};
  }

  [[nodiscard]] friend constexpr auto
  has_predecessor(binary_louds_cursor i) noexcept -> bool
  // [[ pre: static_cast<bool>(i) ]]
  {
    return i.node != i.tree->root();
  }

  [[nodiscard]] friend constexpr auto
  predecessor(binary_louds_cursor i) noexcept -> binary_louds_cursor<B>
  // [[ pre: static_cast<bool>(i) && has_parent() ]]
  {
    return {*i.tree, i.tree->parent(i.node)};
  }

  [[nodiscard]] constexpr auto
  is_left_successor() noexcept -> bool
  {
    return tree->child_label(node) == 0;
  }

  [[nodiscard]] constexpr auto
  is_right_successor() noexcept -> bool
  {
    return tree->child_label(node) == 1;
  }
};

static_assert(eco::bidirectional_bicursor<binary_louds_cursor<basic_bitvector<unsigned int, ssize_type_t<memory_view>>>>);

/*export*/ enum class df_visit
{
  pre,
  in,
  post
};

struct is_left_successor_impl
{
  /*export*/ template <bidirectional_bicursor C>
  [[nodiscard]] constexpr auto
  operator()(C cur) const -> bool
  //[[ pre: has_parent(cur) ]]
  {
    auto pred = predecessor(cur);
    return has_left_successor(pred) && left_successor(pred) == cur;
  }

  /*export*/ template <bitvector B>
  [[nodiscard]] constexpr auto
  operator()(binary_louds_cursor<B> cur) const -> bool
  {
    return cur.is_left_successor();
  }
};

/*export*/ inline constexpr is_left_successor_impl is_left_successor{};

struct is_right_successor_impl
{
  /*export*/ template <bidirectional_bicursor C>
  [[nodiscard]] constexpr auto
  operator()(C cur) const -> bool
  //[[ pre: has_parent(c) ]]
  {
    auto pred = predecessor(cur);
    return has_right_successor(pred) && right_successor(pred) == cur;
  }

  /*export*/ template <bitvector B>
  [[nodiscard]] constexpr auto
  operator()(binary_louds_cursor<B> cur) const -> bool
  {
    return cur.is_right_successor();
  }
};

/*export*/ inline constexpr is_right_successor_impl is_right_successor{};

struct tree_traverse_step_impl
{
  /*export*/ template <bidirectional_bicursor C>
  constexpr auto
  operator()(df_visit& v, C& cur) const -> int
  {
    switch (v)
    {
    case df_visit::pre:
      if (has_left_successor(cur)) {
        cur = left_successor(cur);
        return 1;
      } else {
        v = df_visit::in;
        return 0;
      }
    case df_visit::in:
      if (has_right_successor(cur)) {
        v = df_visit::pre;
        cur = right_successor(cur);
        return 1;
      } else {
        v = df_visit::post;
        return 0;
      }
    case df_visit::post:
      if (is_left_successor(cur)) {
        v = df_visit::in;
      }
      cur = predecessor(cur);
      return -1;
    }

    return 0;
  }
};

/*export*/ inline constexpr tree_traverse_step_impl tree_traverse_step{};

struct tree_weight_impl
{
  template <bidirectional_bicursor C>
  [[nodiscard]] constexpr auto
  operator()(C cur) const noexcept -> weight_type_t<C>
  {
    if (!cur) return weight_type_t<C>(0);
    auto root{cur};
    auto v = df_visit::pre;
    weight_type_t<C> n(1);
    do
    {
      tree_traverse_step(v, cur);
      if (v == df_visit::pre) ++n;
    } while (cur != root || v != df_visit::post);
    return n;
  }
};

/*export*/ inline constexpr tree_weight_impl tree_weight{};

struct tree_height_impl
{
  template <bidirectional_bicursor C>
  [[nodiscard]] constexpr auto
  operator()(C cur) const noexcept -> weight_type_t<C>
  {
    using N = weight_type_t<C>;
    if (!cur) return N(0);
    auto root{cur};
    auto v = df_visit::pre;
    N n(1);
    N m(1);
    do
    {
      m = (m - N(1)) + N(tree_traverse_step(v, cur) + 1);
      n = std::max(n, m);
    } while (cur != root || v != df_visit::post);
    return n;
  }
};

/*export*/ inline constexpr tree_height_impl tree_height{};

} // namespace eco::cpp20

namespace std {

/*export*/ template <typename T, auto ga, auto& alloc>
constexpr void
swap(eco::array<T, ga, alloc>& x, eco::array<T, ga, alloc>& y) noexcept
{
  using eco::swap;
  swap(x, y);
}

} // namespace std
