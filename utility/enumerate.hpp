//
// Created by Gianluca on 06/12/2021.
// SPDX-License-Identifier: MIT
//

#pragma once

#include <algorithm>

template <typename T,
         typename TIter = decltype(std::ranges::begin(std::declval<T>())),
         typename = decltype(std::ranges::end(std::declval<T>()))>
constexpr auto enumerate(T && iterable)
{
  struct iterator
  {
    std::size_t i;
    TIter iter;
    bool operator != (const iterator & other) const { return iter != other.iter; }
    void operator ++ () { ++i; ++iter; }
    auto operator * () const { return std::tie(i, *iter); }
  };
  struct iterable_wrapper
  {
    T iterable;
    auto begin() { return iterator{ 0, std::ranges::begin(iterable) }; }
    auto end() { return iterator{ 0, std::ranges::end(iterable) }; }
  };
  return iterable_wrapper{ std::forward<T>(iterable) };
}