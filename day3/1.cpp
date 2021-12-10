//
// Created by Gianluca on 03/12/2021.
// SPDX-License-Identifier: MIT
//

#include <utility/enumerate.hpp>

#include <fstream>
#include <cassert>
#include <vector>
#include <tuple>
#include <iostream>

struct bit_values {
  int zeros = 0;
  int ones = 0;
};

int main() {
  auto file = std::ifstream("../day3/input");
  assert(file.is_open());

  std::vector<bit_values> values;

  bool first_iteration = true;
  for(std::string line; std::getline(file, line);) {
    if (first_iteration) {
      values.resize(line.size());
      first_iteration = false;
    }
    for (auto [i, item] : enumerate(line)) {
      if (item == '0') values[i].zeros++;
      else values[i].ones++;
    }
  }

  std::string gamma_binary;
  std::string epsilon_binary;
  gamma_binary.reserve(values.size());
  epsilon_binary.reserve(values.size());

  for (const auto& item : values) {
    gamma_binary.append(1, item.ones > item.zeros ? '1' : '0');
    epsilon_binary.append(1, item.ones < item.zeros ? '1' : '0');
  }

  int gamma = std::stoi(gamma_binary, nullptr, 2);
  int epsilon = std::stoi(epsilon_binary, nullptr, 2);

  std::cout << gamma * epsilon << std::endl;

  return 0;
}