//
// Created by Gianluca on 06/12/2021.
// SPDX-License-Identifier: MIT
//

#include <fstream>
#include <cassert>
#include <vector>
#include <regex>
#include <iostream>

struct line_t {
  int x1 = 0;
  int y1 = 0;
  int x2 = 0;
  int y2 = 0;
};

int main() {

  auto file = std::ifstream("../day5/input");
  assert(file.is_open());

  std::vector<std::vector<int>> space;
  std::vector<line_t> lines;

  const auto regex = std::regex(R"((\d+),(\d+) -> (\d+),(\d+))");
  std::smatch result;

  int max_x = 0;
  int max_y = 0;

  for(std::string line; std::getline(file, line);) {
    line_t l;

    if (std::regex_match(line, result, regex)) {
      assert(result.size() == 5);
      l = line_t{.x1 = std::stoi(result[1]), .y1 = std::stoi(result[2]), .x2 = std::stoi(result[3]), .y2 = std::stoi(result[4])};
    }
    else {
      assert(false);
    }

    if (max_x < l.x1) max_x = l.x1;
    if (max_x < l.x2) max_x = l.x2;
    if (max_y < l.y1) max_y = l.y1;
    if (max_y < l.y2) max_y = l.y2;

    lines.emplace_back(l);
  }

  space.resize(max_x + 1);
  for (auto& item : space) {
    item.resize(max_y + 1);
  }

  for (const auto& item : lines) {
    if (item.x1 == item.x2) {
      for (int i = std::min(item.y1, item.y2); i <= std::max(item.y1, item.y2); i++) {
        space[i][item.x1]++;
      }
    }
    else if (item.y1 == item.y2) {
      for (int i = std::min(item.x1, item.x2); i <= std::max(item.x1, item.x2); i++) {
        space[item.y1][i]++;
      }
    }
    else {
      int j = item.x1;
      int i = item.y1;

      if (item.x1 < item.x2) {
        if (item.y1 < item.y2) {
          for (; i <= item.y2; i++, j++ ) {
            space[i][j]++;
          }
        }
        else {
          for (; i >= item.y2; i--, j++ ) {
            space[i][j]++;
          }
        }
      }
      else {
        if (item.y1 < item.y2) {
          for (; i <= item.y2; i++, j-- ) {
            space[i][j]++;
          }
        }
        else {
          for (; i >= item.y2; i--, j-- ) {
            space[i][j]++;
          }
        }
      }
    }
  }

  int accumulator = 0;
  for (const auto& items : space) {
    for (const auto& item : items) {
      if (item > 1) accumulator++;
    }
  }

  std::cout << accumulator << std::endl;

  return 0;
}
