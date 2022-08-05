//
// Created by Gianluca on 13/12/2021.
// SPDX-License-Identifier: MIT
//

#include <utility/split.hpp>
#include <utility/join.hpp>
#include <utility/vertical_view.hpp>

#include <fstream>
#include <regex>
#include <iostream>
#include <cassert>
#include <numeric>
#include <ranges>
#include <algorithm>

void print(const std::vector<std::vector<int>>& v) {
  for (int i = 0; i < v.begin()->size(); ++i) {
    vertical_view<std::vector<std::vector<int>>> view(v, i);
    std::ranges::for_each(view, [](const auto& item){
      if (item == 0) std::cout << ".";
      else std::cout << "#";
    });
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

int main() {
  auto file = std::ifstream("../day13/input");
  assert(file.is_open());

  std::vector<std::pair<int, int>> dots;
  std::vector<std::string> folds;

  std::regex dots_regex("(\\d+),(\\d+)");
  std::smatch dots_match;

  std::string line;
  std::getline(file, line);
  while (std::regex_match(line, dots_match, dots_regex)) {
    dots.emplace_back(std::stoi(dots_match[1]), std::stoi(dots_match[2]));
    std::getline(file, line);
  }

  for (; std::getline(file, line);) {
    folds.emplace_back(split(line, ' ')[2]);
  }

  int max_x = 0;
  int max_y = 0;
  std::ranges::for_each(dots, [&](const auto& item) {
    if (item.first > max_x) max_x = item.first;
    if (item.second > max_y) max_y = item.second;
  });

  std::vector<std::vector<int>> sheet(max_x + 1, std::vector<int>(max_y + 1));
  std::ranges::for_each(dots, [&](const auto& item) {
    sheet[item.first][item.second]++;
  });

  std::regex folds_regex("([x,y])=(\\d+)");
  std::smatch folds_match;

  for (const auto& fold : folds) {
    std::regex_match(fold, folds_match, folds_regex);

    if (folds_match[1] == "x") {
      for (int i = 0; i < sheet.begin()->size(); ++i) {
        auto items = vertical_view<std::vector<std::vector<int>>>(sheet, i);
        auto outside_view = std::ranges::drop_view(items, std::stoi(folds_match[2]));
        auto keeping_view_reversed = std::ranges::take_view(items, std::stoi(folds_match[2]) + 1) | std::views::reverse;
        std::transform(outside_view.begin(), outside_view.end(), keeping_view_reversed.begin(), keeping_view_reversed.begin(), std::plus{});
      }
      sheet.resize(std::stoi(folds_match[2]));
    }
    if (folds_match[1] == "y") {
      for (auto &items: sheet) {
        auto outside_view = std::ranges::drop_view(items, std::stoi(folds_match[2]));
        auto keeping_view_reversed = std::ranges::take_view(items, std::stoi(folds_match[2]) + 1) | std::views::reverse;
        std::transform(outside_view.begin(), outside_view.end(), keeping_view_reversed.begin(), keeping_view_reversed.begin(), std::plus{});
        items.resize(std::stoi(folds_match[2]));
      }
    }
  }

  print(sheet);

  return 0;
}