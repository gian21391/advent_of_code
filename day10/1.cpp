//
// Created by Gianluca on 09/12/2021.
// SPDX-License-Identifier: MIT
//

#include <fstream>
#include <cassert>
#include <iostream>
#include <algorithm>
#include <ranges>
#include <stack>
#include <numeric>

char matching_parenthesis(char p) {
  if (p == '(') return ')';
  if (p == '[') return ']';
  if (p == '{') return '}';
  if (p == '<') return '>';
  assert(false);
}

int main() {
  auto file = std::ifstream("../day10/input");
  assert(file.is_open());

  std::vector<int> total_points;
  for (std::string line; std::getline(file, line);) {
    std::stack<char> context;

    for (auto c : line) {
      if (c == '(' || c == '[' || c == '{' || c == '<') {
        context.push(c);
      }
      else {
        if (c == matching_parenthesis(context.top())) context.pop();
        else {
          int points = 0;
          if (c == ')') points += 3;
          if (c == ']') points += 57;
          if (c == '}') points += 1197;
          if (c == '>') points += 25137;
          total_points.emplace_back(points);
          break;
        }
      }
    }
  }

  std::cout << std::accumulate(total_points.begin(), total_points.end(), 0) << std::endl;

  return 0;
}