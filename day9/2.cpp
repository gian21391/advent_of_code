//
// Created by Gianluca on 09/12/2021.
// SPDX-License-Identifier: MIT
//

#include <fstream>
#include <cassert>
#include <iostream>
#include <set>
#include <algorithm>
#include <ranges>

void explore(int i, int j, const std::vector<std::vector<int>>& map, std::set<std::pair<int, int>>& visited) {

  visited.emplace(i, j);

  std::vector<std::pair<int, int>> exploration_list;
  if (i > 0) exploration_list.emplace_back(i - 1, j);
  if (i < (map.size() - 1)) exploration_list.emplace_back(i + 1, j);
  if (j > 0) exploration_list.emplace_back(i, j - 1);
  if (j < (map[i].size() - 1)) exploration_list.emplace_back(i, j + 1);
  std::erase_if(exploration_list, [&](const auto& item){ return visited.contains(item) || map[item.first][item.second] == 9; });

  for (const auto& item : exploration_list) explore(item.first, item.second, map, visited);
}

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

  std::vector<int> sizes;

  for (int i = 0; i < map.size(); ++i) {
    for (int j = 0; j < map[i].size(); ++j) {
      std::vector<int> min_list;
      if (i > 0) min_list.emplace_back(map[i - 1][j]);
      if (i < (map.size() - 1)) min_list.emplace_back(map[i + 1][j]);
      if (j > 0) min_list.emplace_back(map[i][j - 1]);
      if (j < (map[i].size() - 1)) min_list.emplace_back(map[i][j + 1]);
      if (map[i][j] < *std::ranges::min_element(min_list)) {
        std::set<std::pair<int, int>> v;
        explore(i, j, map, v);
        sizes.emplace_back(v.size());
      }
    }
  }

  std::ranges::sort(sizes, std::ranges::greater());
  int result = 1;
  std::ranges::for_each_n(sizes.begin(), 3, [&](const auto& item){ result *= item; });

  std::cout << result << std::endl;

  return 0;
}