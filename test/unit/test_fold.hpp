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

    auto result = eco::fold_left(data, std::plus<int>{}, 0);
    assert(result == 10);

    result = eco::fold_left(data, data, std::plus<int>{}, 0);
    assert(result == 0);

    result = eco::fold_left(data, std::minus<int>{}, 0);
    assert(result == -8);

    result = eco::fold_left(data, data, std::minus<int>{}, 0);
    assert(result == 0);

    result = eco::fold_left(data, std::multiplies<int>{}, 1);
    assert(result == 24);

    result = eco::fold_left(data, data, std::multiplies<int>{}, 1);
    assert(result == 1);
  }
}

void
test_fold_right()
{
  {
    int data[] = {1, 2, 3, 4};

    auto result = eco::fold_right(data, std::plus<int>{}, 0);
    assert(result == 10);

    result = eco::fold_right(data, data, std::plus<int>{}, 0);
    assert(result == 0);

    result = eco::fold_right(data, std::minus<int>{}, 0);
    assert(result == -2);

    result = eco::fold_right(data, data, std::minus<int>{}, 0);
    assert(result == 0);

    result = eco::fold_right(data, std::multiplies<int>{}, 1);
    assert(result == 24);

    result = eco::fold_right(data, data, std::multiplies<int>{}, 1);
    assert(result == 1);
  }
}

void
test_fold()
{
  test_fold_left();
  test_fold_right();
}

#endif
