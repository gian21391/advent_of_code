//
// Created by Gianluca on 03/12/2021.
// SPDX-License-Identifier: MIT
//

#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <string>

int main() {
  auto file = std::ifstream("../day1/input");
  assert(file.is_open());

  std::vector<int> elements;

  for (std::string line; std::getline(file, line); ) {
    elements.emplace_back(std::stoi(line));
  }

  int accumulator = 0;
  int previous_window = -1;
  int current_window = -1;

  current_window = elements[0] + elements[1] + elements[2];
  for (int i = 3; i < elements.size(); ++i) {
    previous_window = current_window;
    current_window = previous_window + elements[i] - elements[i - 3];
    if (previous_window == -1) continue;
    if (current_window - previous_window > 0) accumulator++;
  }

  std::cout << accumulator << std::endl;
  return 0;
}