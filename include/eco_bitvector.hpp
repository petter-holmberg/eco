#ifndef ECO_BITVECTOR_
#define ECO_BITVECTOR_

#include <bit>
#include <concepts>

#include "eco_concepts.hpp"
#include "eco_extent.hpp"
#include "eco_memory.hpp"
#include "eco_type_traits.hpp"

namespace eco::inline cpp20 {

template <typename T>
concept bitvector =
  std::regular<T> &&
  std::constructible_from<ssize_t<T>> &&
  requires (T b, T const& cb, ssize_t<T> i) {
    { cb.size() } -> std::same_as<ssize_t<T>>;
    { cb.bitread(i) } -> boolean_testable;
    b.bitset(i);
    b.bitclear(i);
    b.init();
    { cb.rank_0(i) } -> std::same_as<ssize_t<T>>;
    { cb.rank_1(i) } -> std::same_as<ssize_t<T>>;
    { cb.select_0(i) } -> std::same_as<ssize_t<T>>;
    { cb.select_1(i) } -> std::same_as<ssize_t<T>>;
  };

template
<
  std::unsigned_integral Word = unsigned int,
  typename Size = ssize_t<memory_view>
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

  [[nodiscard]] constexpr auto
  operator<=>(basic_bitvector const&) const -> std::strong_ordering = default;

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

static_assert(std::totally_ordered<basic_bitvector<unsigned int, ssize_t<memory_view>>>);
static_assert(bitvector<basic_bitvector<unsigned int, ssize_t<memory_view>>>);

struct succ_0_impl
{
  template <bitvector B>
  [[nodiscard]] constexpr auto
  operator()(B const& b, ssize_t<B> i) const noexcept -> ssize_t<B>
  {
    return b.select_0(b.rank_0(i));
  }
};

inline constexpr succ_0_impl succ_0{};

struct succ_1_impl
{
  template <bitvector B>
  [[nodiscard]] constexpr auto
  operator()(B const& b, ssize_t<B> i) const noexcept -> ssize_t<B>
  {
    return b.select_1(b.rank_1(i));
  }
};

inline constexpr succ_1_impl succ_1{};

struct pred_0_impl
{
  template <bitvector B>
  [[nodiscard]] constexpr auto
  operator()(B const& b, ssize_t<B> i) const noexcept -> ssize_t<B>
  {
    return b.select_0(b.rank_0(i + 1) - 1);
  }
};

inline constexpr pred_0_impl pred_0{};

struct pred_1_impl
{
  template <bitvector B>
  [[nodiscard]] constexpr auto
  operator()(B const& b, ssize_t<B> i) const noexcept -> ssize_t<B>
  {
    return b.select_1(b.rank_1(i + 1) - 1);
  }
};

inline constexpr pred_1_impl pred_1{};

}

#endif
