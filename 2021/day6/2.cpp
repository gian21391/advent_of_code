//
// Created by Gianluca on 06/12/2021.
// SPDX-License-Identifier: MIT
//

#include <utility/split.hpp>

#include <map>
#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <deque>
#include <algorithm>

struct fish_t {
  explicit fish_t(int counter) : _counter(counter) {}

  bool new_day() {
    if (_counter == 0) {
      _counter = 6;
      return true;
    } else {
      _counter--;
      return false;
    }
  }

  auto operator<=>(const fish_t& other) const = default;

  int _counter = 0;
};

std::deque<std::pair<long, fish_t>> compact(std::deque<std::pair<long, fish_t>>& cumulative) {
  std::map<fish_t, long> frequency;

  std::deque<std::pair<long, fish_t>> result;

  std::sort(cumulative.begin(), cumulative.end(), [](const auto& a, const auto& b){ return a.second > b.second; });
  std::unique_copy(cumulative.begin(), cumulative.end(), std::back_inserter(result), [](const auto& a, const auto& b){ return a.second == b.second; });

  for (const auto& item : cumulative) {
    frequency[item.second] += item.first;
  }

  for (auto& item : result) {
    item.first = frequency[item.second];
  }

  return result;
}

int main() {

  auto file = std::ifstream("../day6/input");
  assert(file.is_open());

  std::string line;
  std::getline(file, line);

  std::vector<std::string> initial_values_string = split(line, ',');

  std::deque<std::pair<long, fish_t>> cumulative;

  for (const auto& value : initial_values_string) {
    cumulative.emplace_back(1, fish_t(std::stoi(value)));
  }

  cumulative = compact(cumulative);

  for (int i = 0; i < 256; ++i) {
    for (auto& it : cumulative) {
      if (it.second.new_day()) cumulative.emplace_back(it.first, fish_t(8));
    }
    cumulative = compact(cumulative);
  }

  long count = 0;
  for (const auto& item : cumulative) count += item.first;
  std::cout << count << std::endl;

  return 0;
}

