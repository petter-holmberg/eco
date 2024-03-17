#ifndef ECO_TEST_FIND_
#define ECO_TEST_FIND_

import std;
import eco;

#include <cassert>

void
test_find_not()
{
  int data[] = {1, 2, 3, 4};

  auto result = eco::find_not(data, 1);
  assert(result == std::ranges::begin(data) + 1);

  result = eco::find_not(data, 1, std::negate{});
  assert(result == std::ranges::begin(data));

  result = eco::find_not(std::ranges::begin(data), std::ranges::begin(data), 2);
  assert(result == std::ranges::begin(data));
}

void
test_find_n()
{
  int data[] = {1, 2, 3, 4};

  {
    auto [result, n] = eco::find_n(data, 4, 2);
    assert(result == std::ranges::begin(data) + 1);
    assert(n == 2);
  }

  {
    auto [result, n] = eco::find_n(data, 4, -3, std::negate{});
    assert(result == std::ranges::begin(data) + 2);
    assert(n == 1);
  }
}

void
test_find_not_n()
{
  int data[] = {1, 2, 3, 4};

  {
    auto [result, n] = eco::find_not_n(data, 4, 1);
    assert(result == std::ranges::begin(data) + 1);
    assert(n == 2);
  }

  {
    auto [result, n] = eco::find_not_n(data, 4, -2, std::negate{});
    assert(result == std::ranges::begin(data));
    assert(n == 3);
  }
}

void
test_find_if_n()
{
  int data[] = {1, 2, 3, 4};

  {
    auto [result, n] = eco::find_if_n(data, 4, [](auto x){ return x > 1; });
    assert(result == std::ranges::begin(data) + 1);
    assert(n == 2);
  }

  {
    auto [result, n] = eco::find_if_n(data, 4, [](auto x){ return x < -2; }, std::negate{});
    assert(result == std::ranges::begin(data) + 2);
    assert(n == 1);
  }
}

void
test_find_if_not_n()
{
  int data[] = {1, 2, 3, 4};

  {
    auto [result, n] = eco::find_if_not_n(data, 4, [](auto x){ return x < 3; });
    assert(result == std::ranges::begin(data) + 2);
    assert(n == 1);
  }

  {
    auto [result, n] = eco::find_if_not_n(data, 4, [](auto x){ return x < -1; }, std::negate{});
    assert(result == std::ranges::begin(data));
    assert(n == 3);
  }
}

void
test_find_if_unguarded()
{
  int data[] = {1, 2, 3, 4};

  auto result = eco::find_if_unguarded(data, [](auto x){ return x > 2; });
  assert(result == std::ranges::begin(data) + 2);

  result = eco::find_if_unguarded(data, [](auto x){ return x > -2; }, std::negate{});
  assert(result == std::ranges::begin(data));
}

void
test_find_if_not_unguarded()
{
  int data[] = {1, 2, 3, 4};

  auto result = eco::find_if_not_unguarded(data, [](auto x){ return x < 2; });
  assert(result == std::ranges::begin(data) + 1);

  result = eco::find_if_not_unguarded(data, [](auto x){ return x < -2; }, std::negate{});
  assert(result == std::ranges::begin(data));
}

void
test_all_of_n()
{
  int data[] = {1, 2, 3, 4};

  assert(eco::all_of_n(data, 4, [](auto x){ return x < 5; }));

  assert(!eco::all_of_n(data, 4, [](auto x){ return x < -4; }, std::negate{}));

  assert(eco::all_of_n(data, 0, [](auto x){ return x < 1; }));
}

void
test_none_of_n()
{
  int data[] = {1, 2, 3, 4};

  assert(eco::none_of_n(data, 4, [](auto x){ return x >= 5; }));

  assert(!eco::none_of_n(data, 4, [](auto x){ return x >= -4; }, std::negate{}));

  assert(eco::none_of_n(data, 0, [](auto x){ return x >= 1; }));
}

void
test_not_all_of_n()
{
  int data[] = {1, 2, 3, 4};

  assert(eco::not_all_of_n(data, 4, [](auto x){ return x > 4; }));

  assert(!eco::not_all_of_n(data, 4, [](auto x){ return x >= -4; }, std::negate{}));

  assert(!eco::not_all_of_n(data, 0, [](auto x){ return x > 4; }));
}

void
test_any_of_n()
{
  int data[] = {1, 2, 3, 4};

  assert(eco::any_of_n(data, 4, [](auto x){ return x > 2; }));

  assert(eco::any_of_n(data, 4, [](auto x){ return x >= -2; }, std::negate{}));

  assert(!eco::any_of_n(data, 0, [](auto x){ return x > 2; }));
}

void
test_not_all_of()
{
  int data[] = {1, 2, 3, 4};

  assert(eco::not_all_of(data, [](auto x){ return x > 4; }));

  assert(!eco::not_all_of(data, [](auto x){ return x >= -4; }, std::negate{}));

  assert(!eco::not_all_of(data, data, [](auto x){ return x > 4; }));
}

void
test_count_not()
{
  int data[] = {1, 2, 3, 4};

  assert(eco::count_not(data, 2) == 3);

  assert(eco::count_not(data, -2, std::negate{}) == 3);

  assert(eco::count_not(data, data, 2) == 0);
}

void
test_count_if_not()
{
  int data[] = {1, 2, 3, 4};

  assert(eco::count_if_not(data, [](auto x){ return x < 2; }) == 3);

  assert(eco::count_if_not(data, [](auto x){ return x >= -2; }, std::negate{}) == 2);

  assert(eco::count_if_not(data, data, [](auto x){ return x < 2; }) == 0);
}

void
test_count_n()
{
  int data[] = {1, 2, 3, 4};

  assert(eco::count_n(data, 4, 2) == 1);

  assert(eco::count_n(data, 4, -2, std::negate{}) == 1);

  assert(eco::count_n(data, 0, 2) == 0);
}

void
test_count_not_n()
{
  int data[] = {1, 2, 3, 4};

  assert(eco::count_not_n(data, 4, 2) == 3);

  assert(eco::count_not_n(data, 4, -2, std::negate{}) == 3);

  assert(eco::count_not_n(data, 0, 2) == 0);
}

void
test_count_if_n()
{
  int data[] = {1, 2, 3, 4};

  assert(eco::count_if_n(data, 4, [](auto x){ return x < 3; }) == 2);

  assert(eco::count_if_n(data, 4, [](auto x){ return x < -3; }, std::negate{}) == 1);

  assert(eco::count_if_n(data, 0, [](auto x){ return x < 3; }) == 0);
}

void
test_count_if_not_n()
{
  int data[] = {1, 2, 3, 4};

  assert(eco::count_if_not_n(data, 4, [](auto x){ return x < 3; }) == 2);

  assert(eco::count_if_not_n(data, 4, [](auto x){ return x < -3; }, std::negate{}) == 3);

  assert(eco::count_if_not_n(data, 0, [](auto x){ return x < 3; }) == 0);
}

void
test_search()
{
  test_find_not();
  test_find_n();
  test_find_not_n();
  test_find_if_n();
  test_find_if_not_n();
  test_find_if_unguarded();
  test_find_if_not_unguarded();
  test_all_of_n();
  test_none_of_n();
  test_not_all_of_n();
  test_not_all_of();
  test_count_not();
  test_count_if_not();
  test_count_n();
  test_count_not_n();
  test_count_if_n();
  test_count_if_not_n();
}

#endif
