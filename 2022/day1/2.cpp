//
// Created by Gianluca on 05/12/2022.
// Copyright (c) 2022 TUHH. All rights reserved.
//

#include <fmt/core.h>

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>
#include <vector>

int main() {
  auto file = std::ifstream("../2022/day1/input");
  assert(file.is_open());

  std::vector<int> calories;

  calories.emplace_back(0);
  for (std::string line; std::getline(file, line); ) {
    if (line.empty()) calories.emplace_back(0);
    else {
      auto value = std::stoi(line);
      calories.back() += value;
    }
  }

  std::ranges::sort(calories, std::ranges::greater());
  auto first_3 = std::ranges::take_view(calories, 3);

  fmt::print("{}", std::accumulate(first_3.begin(), first_3.end(), 0));

  return 0;
}