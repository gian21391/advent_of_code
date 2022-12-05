//
// Created by Gianluca on 16/12/2021.
// SPDX-License-Identifier: MIT
//

#include <iostream>
#include <fstream>
#include <cassert>
#include <map>
#include <set>
#include <regex>
#include <ranges>
#include <algorithm>

int main() {
  auto file = std::ifstream("../day14/input");
  assert(file.is_open());

  std::string sequence;
  std::getline(file, sequence);

  std::regex dots_regex("(\\w+) -> (\\w+)");
  std::smatch dots_match;
  std::map<std::string, char> temp_match;
  std::set<std::string> sequences;
  std::set<char> substitutes;

  for (std::string line; std::getline(file, line); ) {
    if (line.empty()) continue;
    std::regex_match(line, dots_match, dots_regex);
    temp_match.emplace(dots_match[1], dots_match[2].str()[0]);
    sequences.emplace(dots_match[1]);
    substitutes.emplace(dots_match[2].str()[0]);
  }

  std::vector<std::string> dictionary_sequences(sequences.begin(), sequences.end());
  std::vector<char> dictionary_substitutes(substitutes.begin(), substitutes.end());
  std::map<int, int> match;
  std::ranges::for_each(temp_match, [&](const auto& item){
    int a1 = std::ranges::find_if(dictionary_sequences, [&](const auto& i){ return item.first == i; }) - dictionary_sequences.begin();
    int a2 = std::ranges::find_if(dictionary_substitutes, [&](const auto& i){ return item.second == i; }) - dictionary_substitutes.begin();
    match.emplace(a1, a2);
  });

  std::map<int, long> template_status;
  auto it = std::ranges::adjacent_find(sequence, [&](auto c1, auto c2) { return temp_match.contains({c1, c2}); });;
  while (it != sequence.end()) {
    int a1 = std::ranges::find_if(dictionary_sequences, [&](const auto& i){ return std::string(it, it + 2) == i; }) - dictionary_sequences.begin();
    ++template_status[a1];
    it = std::ranges::adjacent_find(it + 1, sequence.end(), [&](auto c1, auto c2) { return temp_match.contains({c1, c2}); });
  }

  std::map<int, long> counts;
  std::ranges::for_each(sequence, [&](auto c){
    int a1 = std::ranges::find_if(dictionary_substitutes, [&](const auto& i){ return c == i; }) - dictionary_substitutes.begin();
    ++counts[a1];
  });

  std::map<int, std::pair<int, int>> useful_match;
  std::ranges::for_each(match, [&](const auto& item){
    std::string seq1({dictionary_sequences[item.first][0], dictionary_substitutes[item.second]});
    int a1 = std::ranges::find_if(dictionary_sequences, [&](const auto& i){ return seq1 == i; }) - dictionary_sequences.begin();
    std::string seq2({dictionary_substitutes[item.second], dictionary_sequences[item.first][1]});
    int a2 = std::ranges::find_if(dictionary_sequences, [&](const auto& i){ return seq2 == i; }) - dictionary_sequences.begin();
    useful_match.emplace(item.first, std::pair{a1, a2});
  });

  for (int i = 0; i < 40; ++i) {
    std::map<int, long> temp_status;
    std::ranges::for_each(template_status, [&](const auto& item){
      auto& m = useful_match.at(item.first);
      temp_status[m.first] += item.second;
      temp_status[m.second] += item.second;
      counts[match.at(item.first)] += item.second;
    });
    template_status = temp_status;
  }

  auto [min, max] = std::ranges::minmax_element(counts | std::views::values);

  std::cout << *max - *min << std::endl;

  return 0;
}
