#include <cassert>

#include "eco_dlx.hpp"

inline void
test_dlx()
{
  {
    std::vector<int> items{'a', 'b', 'c', 'd', 'e', 'f', 'g'};
    std::vector<std::vector<int>> options{
      {2, 4},
      {0, 3, 6},
      {1, 2, 5},
      {0, 3, 5},
      {1, 6},
      {3, 4, 6}
    };
    eco::dlx x{items, options};
    auto visitor{x.visit_all()};
    x(visitor);
    assert(visitor.solutions.size() == 1);
    assert(visitor.solutions[0].size() == 3);
    assert(visitor.solutions[0][0] == 3);
    assert(visitor.solutions[0][1] == 4);
    assert(visitor.solutions[0][2] == 0);
  }

  {
    std::string covered =
      "..3" ".1." "..."
      "415" "..." ".9."
      "2.6" "5.." "3.."

      "5.." ".8." "..9"
      ".7." "9.." ".32"
      ".38" "..4" ".6."

      "..." "26." "4.3"
      "..." "3.." "..8"
      "32." "..7" "95.";

    std::vector<int> sudoku_items;
    for (int i = 0; i < 9; ++i) {
      for (int j = 0; j < 9; ++j) {
        if (covered[i * 9 + j] != '.') continue;
        sudoku_items.push_back((int('p') << 16) + (i << 8) + j);
      }
    }
    for (int i = 0; i < 9; ++i) {
      for (int k = 1; k < 10; ++k) {
        bool skip = false;
        for (int j = 0; j < 9; ++j) {
          if (covered[i * 9 + j] == k + '0') skip = true;
        }
        if (skip) continue;
        sudoku_items.push_back((int('r') << 16) + (i << 8) + k);
      }
    }
    for (int i = 0; i < 9; ++i) {
      for (int k = 1; k < 10; ++k) {
        bool skip = false;
        for (int j = 0; j < 9; ++j) {
          if (covered[j * 9 + i] == k + '0') skip = true;
        }
        if (skip) continue;
        sudoku_items.push_back((int('c') << 16) + (i << 8) + k);
      }
    }
    for (int i = 0; i < 9; ++i) {
      for (int k = 1; k < 10; ++k) {
        bool skip = false;
        auto x = (i % 3) * 3;
        auto y = (i / 3) * 3;
        for (int j = 0; j < 3; ++j) {
          if (covered[(y + j) * 9 + x + 0] == k + '0') skip = true;
          if (covered[(y + j) * 9 + x + 1] == k + '0') skip = true;
          if (covered[(y + j) * 9 + x + 2] == k + '0') skip = true;
        }
        if (skip) continue;
        sudoku_items.push_back((int('b') << 16) + (i << 8) + k);
      }
    }

    std::vector<std::vector<int>> sudoku_options;
    for (int i = 0; i < 9; ++i) {
      for (int j = 0; j < 9; ++j) {
        for (int k = 1; k < 10; ++k) {
          auto p = std::ranges::find(sudoku_items, (int('p') << 16) + (i << 8) + j);
          if (p == std::end(sudoku_items)) continue;
          auto r = std::ranges::find(sudoku_items, (int('r') << 16) + (i << 8) + k);
          if (r == std::end(sudoku_items)) continue;
          auto c = std::ranges::find(sudoku_items, (int('c') << 16) + (j << 8) + k);
          if (c == std::end(sudoku_items)) continue;
          auto x = 3 * (i / 3) + (j / 3);
          auto b = std::ranges::find(sudoku_items, (int('b') << 16) + (x << 8) + k);
          if (b == std::end(sudoku_items)) continue;
          sudoku_options.push_back(
            {
              int(p - std::begin(sudoku_items)),
              int(r - std::begin(sudoku_items)),
              int(c - std::begin(sudoku_items)),
              int(b - std::begin(sudoku_items))
            }
          );
        }
      }
    }

    eco::dlx sudoku{sudoku_items, sudoku_options};
    auto sudoku_visitor{sudoku.visit_all()};
    sudoku(sudoku_visitor);

    assert(sudoku_visitor.solutions.size() == 1);
    assert(sudoku_visitor.solutions[0].size() == 49);

    auto solution{covered};
    for (auto const& options : sudoku_visitor.solutions) {
      for (auto const& option : options) {
        auto item{sudoku_items[sudoku_options[option][0]]};
        auto i{(item & 0xff00) >> 8};
        auto j{item & 0xff};
        item = sudoku_items[sudoku_options[option][1]];
        auto k{(item & 0xff)};
        solution[i * 9 + j] = k + '0';
      }
    }
    assert(
      solution ==
        "793" "412" "685"
        "415" "638" "297"
        "286" "579" "314"

        "562" "183" "749"
        "174" "956" "832"
        "938" "724" "561"

        "859" "261" "473"
        "647" "395" "128"
        "321" "847" "956"
    );
  }
}
