//
// Created by Gianluca on 13/12/2022.
// SPDX-License-Identifier: MIT
//

#include <fmt/core.h>
#include <gsl/narrow>

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

  for (std::string line; std::getline(file, line); ) {
    int items_number = gsl::narrow<int>(line.size()/2);
    auto pocket_1 = std::ranges::take_view(line, items_number);
    auto pocket_2 = std::ranges::drop_view(line, items_number);
    std::ranges::sort(pocket_1);
    std::ranges::sort(pocket_2);
    std::vector<char> intersection;
    std::ranges::set_intersection(pocket_1, pocket_2, std::back_inserter(intersection));
    const auto ret = std::ranges::unique(intersection);
    intersection.erase(ret.begin(), ret.end());

    for (int item : intersection) {
      if (std::islower(item)) {
        total_score += item - 96;
      }
      else {
        total_score += item - 38;
      }
    }
  }

  fmt::print("{}", total_score);

  return 0;
}