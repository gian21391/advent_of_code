//
// Created by Gianluca on 03/12/2021.
// SPDX-License-Identifier: MIT
//

#include <iostream>
#include <fstream>
#include <cassert>
#include <string>

int main() {
  auto file = std::ifstream("../day1/input");
  assert(file.is_open());

  int previous = -1;
  int current = -1;
  int accumulator = 0;

  for (std::string line; std::getline(file, line); ) {
    previous = current;
    current = std::stoi(line);
    if (previous == -1) continue;
    if (current - previous > 0) accumulator++;
  }

  std::cout << accumulator << std::endl;
  return 0;
}
