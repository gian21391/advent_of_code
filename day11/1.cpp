//
// Created by Gianluca on 10/12/2021.
// SPDX-License-Identifier: MIT
//

#include <fstream>
#include <cassert>
#include <iostream>
#include <algorithm>
#include <ranges>
#include <set>
#include <vector>

void flashing(std::vector<std::vector<int>>& map, std::set<std::pair<int, int>>& flashing_list, std::pair<int, int> f) {
  if (!flashing_list.emplace(f).second) return;
  std::vector<std::pair<int, int>> to_be_increased(8, f);
  to_be_increased[0].first--;
  to_be_increased[1].first++;
  to_be_increased[2].second--;
  to_be_increased[3].second++;
  to_be_increased[4].first++; to_be_increased[4].second++;
  to_be_increased[5].first++; to_be_increased[5].second--;
  to_be_increased[6].first--; to_be_increased[6].second++;
  to_be_increased[7].first--; to_be_increased[7].second--;
  std::erase_if(to_be_increased, [&](const auto& item){ return item.first < 0 || item.second < 0 || item.first >= map.size() || item.second >= map[item.first].size(); });
  for (const auto& item : to_be_increased) {
    if (++map[item.first][item.second] > 9) flashing(map, flashing_list, item);
  }
}

auto execute_step(std::vector<std::vector<int>>& map) {
  for (auto& items : map) {
    for (auto& item : items) {
      ++item;
    }
  }

  std::vector<std::pair<int, int>> to_be_increased;
  for (int i = 0; i < map.size(); ++i) {
    for (int j = 0; j < map[i].size(); ++j) {
      if (map[i][j] > 9) to_be_increased.emplace_back(i, j);
    }
  }

  if (to_be_increased.empty()) return 0ul;

  std::set<std::pair<int, int>> flashing_list;
  for (const auto& item : to_be_increased) {
    flashing(map, flashing_list, item);
  }

  for (auto& items : map) {
    for (auto& item : items) {
      if (item > 9) item = 0;
    }
  }

  return flashing_list.size();
}

int main() {
  auto file = std::ifstream("../day11/input");
  assert(file.is_open());

  std::vector<std::vector<int>> map;

  for (std::string line; std::getline(file, line);) {
    map.emplace_back();
    for (auto c : line) map.back().emplace_back(c - '0');
  }

  auto result = 0ul;
  for (int i = 0; i < 100; ++i) {
    result += execute_step(map);
  }

  std::cout << result << std::endl;

  return 0;
}