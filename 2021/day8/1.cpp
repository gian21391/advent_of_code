//
// Created by Gianluca on 07/12/2021.
// SPDX-License-Identifier: MIT
//

#include <utility/split.hpp>

#include <fstream>
#include <cassert>
#include <iostream>

int main() {
  auto file = std::ifstream("../day8/input");
  assert(file.is_open());

  int count = 0;
  for (std::string line; std::getline(file, line);) {
    auto signals = split(line, '|');
    auto outputs = split(signals[1], ' ');
    for (const auto& item : outputs) {
      if (item.size() == 2 || item.size() == 3 || item.size() == 4 || item.size() == 7) count++;
    }
  }

  std::cout << count << std::endl;

  return 0;
}