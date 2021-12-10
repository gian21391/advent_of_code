//
// Created by Gianluca on 09/12/2021.
// SPDX-License-Identifier: MIT
//

#include <utility/split.hpp>

#include <fstream>
#include <cassert>
#include <iostream>
#include <algorithm>

int main() {
  auto file = std::ifstream("../day9/input");
  assert(file.is_open());

  std::vector<std::vector<int>> map;

  for (std::string line; std::getline(file, line);) {
    map.emplace_back();
    for (const auto& c : line) {
      map.back().emplace_back(c - '0');
    }
  }

  int risk = 0;

  for (int i = 0; i < map.size(); ++i) {
    for (int j = 0; j < map[i].size(); ++j) {
      std::vector<int> min_list;
      if (i > 0) min_list.emplace_back(map[i - 1][j]);
      if (i < (map.size() - 1)) min_list.emplace_back(map[i + 1][j]);
      if (j > 0) min_list.emplace_back(map[i][j - 1]);
      if (j < (map[i].size() - 1)) min_list.emplace_back(map[i][j + 1]);
      if (map[i][j] < *std::ranges::min_element(min_list)) risk += map[i][j] + 1;
    }
  }

  std::cout << risk << std::endl;

  return 0;
}