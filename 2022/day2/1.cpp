//
// Created by Gianluca on 05/12/2022.
// Copyright (c) 2022 TUHH. All rights reserved.
//

#include "../../utility/split.hpp"

#include <fmt/core.h>

#include <algorithm>
#include <cassert>
#include <fstream>

int match(int player_a, int player_b) {
  player_b = player_b - 23;

  if (player_a == player_b) return 3;
  else if (player_a == player_b + 1 || player_a + 2 == player_b) return 0;
  else return 6;
}

int main() {
  auto file = std::ifstream("../2022/day2/input");
  assert(file.is_open());

  auto total_score = 0;

  for (std::string line; std::getline(file, line); ) {
    auto moves = split(line, ' ');

    if (moves[1] == "X") total_score += 1;
    else if (moves[1] == "Y") total_score += 2;
    else if (moves[1] == "Z") total_score += 3;

    total_score += match(moves[0][0], moves[1][0]);
  }

  fmt::print("{}", total_score);

  return 0;
}