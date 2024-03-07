#ifndef ECO_TEST_FOLD_
#define ECO_TEST_FOLD_

import std;
import eco;

#include <cassert>

void
test_fold_left()
{
  {
    int data[] = {1, 2, 3, 4};

    auto result = eco::fold_left(data, 0, std::plus<int>{});
    assert(result == 10);

    result = eco::fold_left(data, data, 0, std::plus<int>{});
    assert(result == 0);

    result = eco::fold_left(data, 0, std::minus<int>{});
    assert(result == -8);

    result = eco::fold_left(data, data, 0, std::minus<int>{});
    assert(result == 0);

    result = eco::fold_left(data, 1, std::multiplies<int>{});
    assert(result == 24);

    result = eco::fold_left(data, data, 1, std::multiplies<int>{});
    assert(result == 1);
  }
}

void
test_fold_right()
{
  {
    int data[] = {1, 2, 3, 4};

    auto result = eco::fold_right(data, 0, std::plus<int>{});
    assert(result == 10);

    result = eco::fold_right(data, data, 0, std::plus<int>{});
    assert(result == 0);

    result = eco::fold_right(data, 0, std::minus<int>{});
    assert(result == -2);

    result = eco::fold_right(data, data, 0, std::minus<int>{});
    assert(result == 0);

    result = eco::fold_right(data, 1, std::multiplies<int>{});
    assert(result == 24);

    result = eco::fold_right(data, data, 1, std::multiplies<int>{});
    assert(result == 1);
  }
}

void
test_fold_binary_left()
{
  int data0[] = {0, 1, 2, 3};
  int data1[] = {2, 3, 4, 5};
  int data2[] = {2, 3, 4, 5, 6};

  assert(eco::fold_binary_left_nonempty(data0, data1, std::plus<int>{}, std::multiplies<int>{}) == 26);

  assert(eco::fold_binary_left(data0, data1, 0, std::plus<int>{}, std::multiplies<int>{}) == 26);

  assert(eco::fold_binary_left(data0, data2, 0, std::plus<int>{}, std::multiplies<int>{}) == 26);

  assert(eco::fold_binary_left(data0, data0, data1, 0, std::plus<int>{}, std::multiplies<int>{}) == 0);
}

void
test_fold_binary_right()
{
  int data0[] = {0, 1, 2, 3};
  int data1[] = {2, 3, 4, 5};
  int data2[] = {2, 3, 4, 5, 6};

  assert(eco::fold_binary_right_nonempty(data0, data1, std::plus<int>{}, std::multiplies<int>{}) == 26);

  assert(eco::fold_binary_right(data0, data1, 0, std::plus<int>{}, std::multiplies<int>{}) == 26);

  assert(eco::fold_binary_right(data0, data2, 0, std::plus<int>{}, std::multiplies<int>{}) == 32);

  assert(eco::fold_binary_right(data0, data0, data1, data1, 0, std::plus<int>{}, std::multiplies<int>{}) == 0);
}

void
test_fold()
{
  test_fold_left();
  test_fold_right();
  test_fold_binary_left();
  test_fold_binary_right();
}

#endif
