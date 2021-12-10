//
// Created by Gianluca on 03/12/2021.
// SPDX-License-Identifier: MIT
//

#include <fstream>
#include <iostream>
#include <cassert>
#include <sstream>

int main() {

  auto file = std::ifstream("../day2/input");
  assert(file.is_open());

  int depth = 0;
  int position = 0;

  for(std::string line; std::getline(file, line);) {
    std::istringstream iss(line);
    std::string command;
    std::getline(iss, command, ' ');
    std::string item;
    std::getline(iss, item, ' ');
    int quantity = std::stoi(item);
    if (command == "forward") position += quantity;
    else if (command == "down") depth += quantity;
    else if (command == "up") depth -= quantity;
  }

  std::cout << depth * position << std::endl;

  return 0;
}