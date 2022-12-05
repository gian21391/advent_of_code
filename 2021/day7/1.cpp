//
// Created by Gianluca on 07/12/2021.
// SPDX-License-Identifier: MIT
//

#include <utility/split.hpp>

#include <fstream>
#include <vector>
#include <cassert>
#include <map>
#include <algorithm>
#include <iostream>

int main() {
  auto file = std::ifstream("../day7/input");
  assert(file.is_open());

  std::string line;
  std::getline(file, line);

  std::vector<std::string> initial_values_string = split(line, ',');
  std::vector<int> values;
  values.reserve(initial_values_string.size());

  for (const auto& value : initial_values_string) {
    values.emplace_back(std::stoi(value));
  }

  std::map<int, int> costs;

  for (int i = *std::ranges::min_element(values); i <= *std::ranges::max_element(values); i++) {
    for (const auto& item : values) {
      costs[i] += std::abs(item - i);
    }
  }

  std::cout << std::ranges::min_element(costs, [](const auto& a, const auto& b){ return a.second < b.second; })->second << std::endl;

  return 0;

}