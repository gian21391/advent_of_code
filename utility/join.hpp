//
// Created by Gianluca on 13/12/2021.
// SPDX-License-Identifier: MIT
//

#pragma once

#include <sstream>

std::string join(const auto& v, const std::string& delim) {
  std::ostringstream s;
  for (const auto& i : v) {
    if (&i != &v[0]) {
      s << delim;
    }
    s << i;
  }
  return s.str();
}

std::string join(const auto& v, const std::string& delim, auto&& transformer) {
  std::ostringstream s;
  for (const auto& i : v) {
    if (&i != &v[0]) {
      s << delim;
    }
    s << transformer(i);
  }
  return s.str();
}