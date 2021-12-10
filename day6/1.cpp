//
// Created by Gianluca on 06/12/2021.
// SPDX-License-Identifier: MIT
//

#include <utility/split.hpp>

#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <deque>

struct fish_t {
  explicit fish_t(int n) : number(n) {}

  bool new_day() {
    if (number == 0) {
      number = 6;
      return true;
    }

    number--;
    return false;
  }

//private:
  int number = 0;
};

int main() {

  auto file = std::ifstream("../day6/input");
  assert(file.is_open());

  std::string line;
  std::getline(file, line);

  std::vector<std::string> initial_values_string = split(line, ',');

  std::deque<fish_t> fishes;
  for (const auto& value : initial_values_string) {
    fishes.emplace_back(std::stoi(value));
  }

  for (int i = 0; i < 80; ++i) {
    auto end = fishes.size();
    for (int j = 0; j < end; ++j) {
      if (fishes[j].new_day()) fishes.emplace_back(8);
    }
  }

  std::cout << fishes.size() << std::endl;

  return 0;
}