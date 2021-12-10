//
// Created by Gianluca on 07/12/2021.
// SPDX-License-Identifier: MIT
//

#include <utility/split.hpp>

#include <fstream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <iostream>

int calc(int n) {
  return (n*(n+1))/2;
}

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

  int min = -1;

  for (int i = *std::ranges::min_element(values); i <= *std::ranges::max_element(values); i++) {
    int cost = 0;
    for (const auto& item : values) {
      cost += calc(std::abs(item - i));
    }
    if (min == -1 || cost < min) min = cost;
  }

  std::cout << min << std::endl;

  return 0;

}