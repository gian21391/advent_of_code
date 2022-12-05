//
// Created by Gianluca on 06/12/2021.
// SPDX-License-Identifier: MIT
//

#include <utility/enumerate.hpp>
#include <utility/split.hpp>

#include <algorithm>
#include <fstream>
#include <cassert>
#include <vector>
#include <iostream>
#include <deque>

class board_t {
public:

  struct vertical_iterator {
    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = bool;
    using pointer           = bool*;  // or also value_type*
    using reference         = bool&;

    explicit vertical_iterator(board_t& b, std::size_t h) : board(b), horizontal_index(h) {}

    reference operator*() { return board.selected[vertical_index][horizontal_index]; }
    reference operator->() { return board.selected[vertical_index][horizontal_index]; }

    // Prefix increment
    vertical_iterator& operator++() { vertical_index++; return *this; }

    // Postfix increment
    vertical_iterator operator++(int) { vertical_iterator tmp = *this; ++(*this); return tmp; }

    vertical_iterator operator+(int value) { vertical_index += value; return *this; }

    friend bool operator== (const vertical_iterator& a, const vertical_iterator& b) { assert(&(a.board) == &(b.board)); return a.vertical_index == b.vertical_index; };
    friend bool operator!= (const vertical_iterator& a, const vertical_iterator& b) { assert(&(a.board) == &(b.board)); return a.vertical_index != b.vertical_index; };

  private:

    std::size_t vertical_index = 0;
    board_t& board;
    const std::size_t horizontal_index;

  };

  explicit board_t(std::vector<std::vector<int>> n) : numbers{5, std::vector<int>(5, 0)}, selected{5, std::deque<bool>(5, false)} {
    assert(n.size() == 5);
    for (int i = 0; i < n.size(); i++) {
      for (int j = 0; j < n[i].size(); j++) {
        numbers[i][j] = n[i][j];
      }
    }
  }

  vertical_iterator vertical_begin(std::size_t horizontal_index) {
    return vertical_iterator(*this, horizontal_index);
  }

  vertical_iterator vertical_end(size_t horizontal_index) {
    return vertical_iterator(*this, horizontal_index) + 5;
  }

  void select(int n) {
    for (const auto& [i, items]: enumerate(numbers)) {
      for (const auto& [j, item] : enumerate(items)) {
        if (item == n) selected[i][j] = true;
      }
    }
  }

  bool is_winning() {
    for (auto &item: selected) {
      if (std::count(item.begin(), item.end(), true) == 5) return true;
    }

    for (int i = 0; i < selected.size(); ++i) {
      if (std::count(vertical_begin(i), vertical_end(i), true) == 5) return true;
    }

    return false;
  }

  int score(int n) {
    int accumulate = 0;
    for (const auto& [i, items]: enumerate(numbers)) {
      for (const auto& [j, item] : enumerate(items)) {
        if (!selected[i][j]) accumulate += item;
      }
    }

    return accumulate * n;
  }

private:
  std::vector<std::vector<int>> numbers;
  std::vector<std::deque<bool>> selected;
};

int main() {

  auto file = std::ifstream("../day4/input");
  assert(file.is_open());

  std::vector<std::string> values_string;
  std::vector<int> values;


  std::vector<board_t> boards;

  std::vector<std::vector<int>> board_numbers;
  bool first_iteration = true;
  for(std::string line; std::getline(file, line);) {

    if (first_iteration) {
      values_string = split(line, ',');
      first_iteration = false;
      continue;
    }

    if (line.empty()) {
      if (!board_numbers.empty()) {
        boards.emplace_back(board_numbers);
        board_numbers.clear();
      }
      continue;
    }

    auto elements = split(line, ' ');
    std::erase_if(elements, [](auto item){ return item.empty(); });
    assert(elements.size() == 5);
    std::vector<int> board_line;
    board_line.reserve(elements.size());
    for (const auto& item : elements) {
      board_line.emplace_back(std::stoi(item));
    }
    board_numbers.emplace_back(board_line);
  }

  std::ranges::transform(values_string, std::back_inserter(values), [](auto item){ return std::stoi(item); });

  for (auto item : values) {
    for (auto& board : boards) {
      board.select(item);
      if (board.is_winning()) {
        std::cout << board.score(item) << std::endl;
        return 0;
      }
    }
  }

  return 0;
}