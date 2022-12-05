//
// Created by Gianluca on 07/12/2021.
// SPDX-License-Identifier: MIT
//

#include <utility/split.hpp>

#include <fmt/ranges.h>

#include <fstream>
#include <vector>
#include <cassert>
#include <map>
#include <algorithm>
#include <iostream>
#include <set>

char to_number(const std::string& segment, const std::map<char, std::set<char>>& display_mapping) {
  if (segment.size() == 2) return '1';
  if (segment.size() == 3) return '7';
  if (segment.size() == 4) return '4';
  if (segment.size() == 7) return '8';
  if (segment.size() == 5) {
    std::set<char> characters(segment.begin(), segment.end());
    if (characters.contains(*display_mapping.at('e').begin())) return '2';
    if (characters.contains(*display_mapping.at('c').begin())) return '3';
    return '5';
  }
  if (segment.size() == 6) {
    std::set<char> characters(segment.begin(), segment.end());
    if (!characters.contains(*display_mapping.at('d').begin())) return '0';
    if (characters.contains(*display_mapping.at('c').begin())) return '9';
    return '6';
  }
  assert(false);
}

int main() {
  auto file = std::ifstream("../day8/input");
  assert(file.is_open());

  int count = 0;

  for (std::string line; std::getline(file, line);) {
    auto signals = split(line, '|');
    auto inputs = split(signals[0], ' ');
    auto outputs = split(signals[1], ' ');

    std::map<char, std::set<char>> display_mapping;

    // start mapping c and f using 1
    for (const auto& item : inputs) {
      if (item.size() == 2) {
        display_mapping['c'] = {item[0], item[1]};
        display_mapping['f'] = {item[0], item[1]};
      }
    }

    // identify a using 7
    for (const auto& item : inputs) {
      if (item.size() == 3) {
        for (char c : item) {
          if (!display_mapping['c'].contains(c)) display_mapping['a'] = {c};
        }
      }
    }

    // start mapping b and d using 4
    for (const auto& item : inputs) {
      if (item.size() == 4) {
        std::set<char> bd;
        for (char c : item) {
          if (!display_mapping['c'].contains(c)) bd.insert(c);
        }
        assert(bd.size() == 2);
        display_mapping['b'] = bd;
        display_mapping['d'] = bd;
      }
    }

    // identify g using 9
    for (const auto& item : inputs) {
      if (item.size() == 6) {
        std::set<char> abcdf;
        abcdf.insert(display_mapping['a'].begin(), display_mapping['a'].end());
        abcdf.insert(display_mapping['b'].begin(), display_mapping['b'].end());
        abcdf.insert(display_mapping['c'].begin(), display_mapping['c'].end());
        assert(abcdf.size() == 5);
        int n = 0;
        for (char c : item) {
          if (abcdf.contains(c)) n++;
        }
        if (n == 5) {
          for (char c : item) {
            if (!abcdf.contains(c)) display_mapping['g'].insert(c);
          }
        }
      }
    }

    // identify e using 8
    for (const auto& item : inputs) {
      if (item.size() == 7) {
        std::set<char> abcdfg;
        abcdfg.insert(display_mapping['a'].begin(), display_mapping['a'].end());
        abcdfg.insert(display_mapping['b'].begin(), display_mapping['b'].end());
        abcdfg.insert(display_mapping['c'].begin(), display_mapping['c'].end());
        abcdfg.insert(display_mapping['g'].begin(), display_mapping['g'].end());
        assert(abcdfg.size() == 6);
        for (char c : item) {
          if (!abcdfg.contains(c)) display_mapping['e'] = {c};
        }
      }
    }

    for (const auto& item : inputs) {
      if (item.size() == 5) {
        std::set<char> characters(item.begin(), item.end());
        // check if it is a 2
        assert(display_mapping['e'].size() == 1);
        if (characters.contains(*display_mapping['e'].begin())) {
          for (auto c : item) {
            if (display_mapping['b'].contains(c)) {
              display_mapping['b'].erase(c);
              assert(display_mapping['b'].size() == 1);
              display_mapping['d'].erase(*display_mapping['b'].begin());
            }
            if (display_mapping['f'].contains(c)) {
              display_mapping['f'].erase(c);
              assert(display_mapping['f'].size() == 1);
              display_mapping['c'].erase(*display_mapping['f'].begin());
            }
          }
        }
      }
    }

    std::stringstream ss;
    for (const auto& item : outputs) {
      if (!item.empty()) ss << to_number(item, display_mapping);
    }

    count += std::stoi(ss.str());
  }

  std::cout << count << std::endl;

  return 0;
}