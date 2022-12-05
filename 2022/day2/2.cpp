//
// Created by Gianluca on 05/12/2022.
// Copyright (c) 2022 TUHH. All rights reserved.
//

#include "../../utility/split.hpp"

#include <fmt/core.h>

#include <algorithm>
#include <cassert>
#include <fstream>

int match(int player_a, std::string_view result) {
  player_a = player_a - 64;

  if (result == "Y") return player_a;
  else if (result == "X") {
    if (player_a == 1) return 3;
    else return player_a - 1;
  }
  else {
    if (player_a == 3) return 1;
    else return player_a + 1;
  }
}

int main() {
  auto file = std::ifstream("../2022/day2/input");
  assert(file.is_open());

  auto total_score = 0;

  for (std::string line; std::getline(file, line); ) {
    auto moves = split(line, ' ');

    if (moves[1] == "X") total_score += 0;
    else if (moves[1] == "Y") total_score += 3;
    else if (moves[1] == "Z") total_score += 6;

    total_score += match(moves[0][0], moves[1]);
  }

  fmt::print("{}", total_score);

  return 0;
}