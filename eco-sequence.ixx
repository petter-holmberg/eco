module;

#include <concepts>
#include <cstddef>
#include <iterator>
#include <memory>
#include <ranges>
#include <utility>
#include <variant>

export module eco:sequence;
import :memory;

namespace eco {

inline namespace cpp20 {

template <typename T>
struct value_type_traits
{
  using type = T::value_type;
};

export template <typename T>
using value_t = typename value_type_traits<T>::type;

export template <typename T>
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

export template <typename T>
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

export inline constexpr default_array_growth_algorithm default_array_growth{};

export inline malloc_allocator default_array_alloc{};

export template
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
    requires !std::same_as<Metadata, std::monostate>
  // [[ pre: start ]]
  {
    return header()->metadata;
  }

  [[nodiscard]] constexpr auto
  // [[ pre: start ]]
  metadata_ref() const noexcept -> Metadata const&
    requires !std::same_as<Metadata, std::monostate>
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
    return (start ? (header_byte_size() + capacity_ref() * sizeof(T)) : 0);
  }

  [[nodiscard]] constexpr auto
  metadata() noexcept -> Metadata*
    requires !std::same_as<Metadata, std::monostate>
  {
    return (start ? &metadata_ref() : nullptr);
  }

  [[nodiscard]] constexpr auto
  metadata() const noexcept -> Metadata const*
    requires !std::same_as<Metadata, std::monostate>
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
    requires std::indirectly_copyable<T*, T*>
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
    requires std::indirectly_copyable<T*, T*>
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
  // [[ post: !empty() ]]
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
  // [[ pre: !empty() ]]
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

} // namespace cpp20

} // namespace eco
