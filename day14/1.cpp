//
// Created by Gianluca on 16/12/2021.
// SPDX-License-Identifier: MIT
//

#include <iostream>
#include <fstream>
#include <cassert>
#include <map>
#include <regex>
#include <ranges>

int main() {
  auto file = std::ifstream("../day14/input");
  assert(file.is_open());

  std::string sequence;
  std::getline(file, sequence);

  std::regex dots_regex("(\\w+) -> (\\w+)");
  std::smatch dots_match;
  std::map<std::string, std::string> match;

  for (std::string line; std::getline(file, line); ) {
    if (line.empty()) continue;
    std::regex_match(line, dots_match, dots_regex);
    match.emplace(dots_match[1], dots_match[2]);
  }

  for (int i = 0; i < 10; ++i) {
    auto it = std::ranges::adjacent_find(sequence, [&](auto c1, auto c2) { return match.contains({c1, c2}); });
    while (it != sequence.end()) {
      it = sequence.insert(it + 1, match[std::string(it, it + 2)][0]);
      it = std::ranges::adjacent_find(it + 1, sequence.end(), [&](auto c1, auto c2) { return match.contains({c1, c2}); });
    }
  }

  std::map<char, int> counts;
  std::ranges::for_each(sequence, [&](auto c){ counts[c]++; });
  auto [min, max] = std::ranges::minmax_element(counts | std::views::values);

  std::cout << *max - *min << std::endl;

  return 0;
}
