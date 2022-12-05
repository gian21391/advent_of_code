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
#include <vector>

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

  std::vector<long> total_points;
  for (std::string line; std::getline(file, line);) {
    std::stack<char> context;
    bool skip = false;
    for (auto c : line) {
      if (c == '(' || c == '[' || c == '{' || c == '<') {
        context.push(c);
      }
      else {
        if (c == matching_parenthesis(context.top())) context.pop();
        else {
          skip = true;
          break;
        }
      }
    }

    if (!skip && !context.empty()) {
      long points = 0;
      while(!context.empty()) {
        points *= 5;
        if (context.top() == '(') points += 1;
        if (context.top() == '[') points += 2;
        if (context.top() == '{') points += 3;
        if (context.top() == '<') points += 4;
        context.pop();
      }
      total_points.emplace_back(points);
    }
  }

  std::ranges::sort(total_points);
  std::cout << total_points[total_points.size()/2] << std::endl;

  return 0;
}