//
// Created by Gianluca on 13/12/2022.
// SPDX-License-Identifier: MIT
//

#include <fmt/core.h>
#include <gsl/narrow>
#include <range/v3/all.hpp>

#include <algorithm>
#include <cassert>
#include <fstream>
#include <ranges>
#include <string>
#include <vector>

int main() {
  auto file = std::ifstream("../2022/day3/input");
  assert(file.is_open());

  auto total_score = 0;

  std::vector<std::string> elements;

  for (std::string line; std::getline(file, line); ) {
    std::ranges::sort(line);
    const auto ret = std::ranges::unique(line);
    line.erase(ret.begin(), ret.end());
    elements.emplace_back(line);
  }

  auto const chunks = elements | ranges::view::chunk(3);

  for (auto&& chunk : chunks) {
    std::vector<char> intersection1;
    std::vector<char> intersection2;
    std::ranges::set_intersection(chunk[0], chunk[1], std::back_inserter(intersection1));
    std::ranges::set_intersection(intersection1, chunk[2], std::back_inserter(intersection2));

    if (std::islower(intersection2[0])) {
      total_score += intersection2[0] - 96;
    }
    else {
      total_score += intersection2[0] - 38;
    }
  }

  fmt::print("{}", total_score);

  return 0;
}