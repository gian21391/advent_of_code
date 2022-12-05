//
// Created by Gianluca on 05/12/2022.
// Copyright (c) 2022 TUHH. All rights reserved.
//

#include <fmt/core.h>

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
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

  fmt::print("{}", *std::ranges::max_element(calories));

  return 0;
}