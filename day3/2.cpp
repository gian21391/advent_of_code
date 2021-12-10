//
// Created by Gianluca on 03/12/2021.
// SPDX-License-Identifier: MIT
//

#include <utility/enumerate.hpp>

#include <fstream>
#include <cassert>
#include <vector>
#include <iostream>

struct bit_values {
  int zeros = 0;
  int ones = 0;
};

int main() {
  auto file = std::ifstream("../day3/input");
  assert(file.is_open());

  std::vector<bit_values> common_bits;
  std::vector<std::string> values;

  bool first_iteration = true;
  for(std::string line; std::getline(file, line);) {
    values.emplace_back(line);
    if (first_iteration) {
      common_bits.resize(line.size());
      first_iteration = false;
    }
    for (auto [i, item] : enumerate(line)) {
      if (item == '0') common_bits[i].zeros++;
      else common_bits[i].ones++;
    }
  }

  std::vector<std::string> oxygen_values(values);
  std::vector<bit_values> oxygen_common_bits(common_bits);

  int i = 0;
  int remove_bit = -1;
  while (oxygen_values.size() > 1) {
    if (oxygen_common_bits[i].zeros > oxygen_common_bits[i].ones) remove_bit = 0;
    else remove_bit = 1;
    std::erase_if(oxygen_values, [&](auto value){ return (value[i] - '0') == remove_bit; });
    oxygen_common_bits.clear();
    oxygen_common_bits.resize(common_bits.size());
    for (const auto& line : oxygen_values) {
      for (auto [j, item]: enumerate(line)) {
        if (item == '0') oxygen_common_bits[j].zeros++;
        else oxygen_common_bits[j].ones++;
      }
    }
    i++;
  }

  std::vector<std::string> co2_values(values);
  std::vector<bit_values> co2_common_bits(common_bits);

  i = 0;
  remove_bit = -1;
  while (co2_values.size() > 1) {
    if (co2_common_bits[i].zeros <= co2_common_bits[i].ones) remove_bit = 0;
    else remove_bit = 1;
    std::erase_if(co2_values, [&](auto value){ return (value[i] - '0') == remove_bit;});
    co2_common_bits.clear();
    co2_common_bits.resize(common_bits.size());
    for (const auto& line : co2_values) {
      for (auto [j, item]: enumerate(line)) {
        if (item == '0') co2_common_bits[j].zeros++;
        else co2_common_bits[j].ones++;
      }
    }
    i++;
  }

  int oxygen = std::stoi(oxygen_values[0], nullptr, 2);
  int co2 = std::stoi(co2_values[0], nullptr, 2);

  std::cout << oxygen * co2 << std::endl;

  return 0;
}