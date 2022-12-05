//
// Created by Gianluca on 22/12/2021.
// SPDX-License-Identifier: MIT
//

#include <iostream>
#include <fstream>
#include <cassert>
#include <regex>

auto find_values(std::pair<int, int> x, std::pair<int, int> y) {
  int start_x_vel = 0;
  int start_y_vel = 0;

  int global_max_y = 0;

  while (true) {
    ++start_x_vel;
    if (start_x_vel > x.second) break;
    start_y_vel = -1;
    bool next_x = false;

    while (true) {
      if (start_x_vel > x.second) break;
      if (next_x) break;

      ++start_y_vel;

      int x_pos = 0;
      int y_pos = 0;
      int x_vel = start_x_vel;
      int y_vel = start_y_vel;

      int max_y = 0;

      while (true) {
        x_pos += x_vel;
        y_pos += y_vel;
        if (max_y < y_pos) max_y = y_pos;

        // skip too low x values
        if (x_vel == 0 && x_pos < x.first) {
          next_x = true;
          break;
        }

        // overshoot
        if (x_pos > x.second || y_pos < y.first) {
          if (x_pos > x.second && y_pos > y.second) next_x = true;
          if (y_pos == y_vel && y_vel < y.first) next_x = true;
          break;
        }

        // target reached
        if (x_pos >= x.first && x_pos <= x.second && y_pos >= y.first && y_pos <= y.second) {
          if (global_max_y < max_y) global_max_y = max_y;
          break;
        }

        if (x_vel > 0) --x_vel;
        else if (x_vel < 0) ++x_vel;
        --y_vel;
      }
    }
  }

  return global_max_y;
}


int main() {
  auto file = std::ifstream("../day17/input");
  assert(file.is_open());

  std::string line;
  std::getline(file, line);

  std::regex target_regex(R"(target area: x=([-]?\d+)\.\.([-]?\d+), y=([-]?\d+)\.\.([-]?\d+))");
  std::smatch target_regex_result;
  std::regex_match(line, target_regex_result, target_regex);
  int x_min = std::stoi(target_regex_result[1]);
  int x_max = std::stoi(target_regex_result[2]);
  int y_min = std::stoi(target_regex_result[3]);
  int y_max = std::stoi(target_regex_result[4]);

  std::pair<int, int> x = {x_min, x_max};
  std::pair<int, int> y = {y_min, y_max};

  auto result = find_values(x, y);

  std::cout << result << std::endl;
  return 0;
}
