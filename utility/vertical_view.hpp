//
// Created by Gianluca on 16/12/2021.
// SPDX-License-Identifier: MIT
//

#pragma once

#include <vector>
#include <memory>
#include <cassert>

template <typename T>
struct inner_type { using type = T; };

template<class T, class Alloc>
struct inner_type<std::vector<T, Alloc>> { using type = typename inner_type<T>::type; };

template<class T>
using inner_type_t = typename inner_type<T>::type;

template <typename T>
class Iterator
{
public:
  using difference_type = std::ptrdiff_t;
  using value_type = inner_type_t<T>;

  Iterator() = default;
  explicit Iterator(const std::shared_ptr<T>& ref, std::size_t h_index) : ref_(ref), h_index_(h_index) {}
  explicit Iterator(const std::shared_ptr<T>& ref, std::size_t h_index, std::size_t index) : ref_(ref), h_index_(h_index), index_(index) {}

  bool operator==(const Iterator& other) const {
    static_assert(std::is_same_v<decltype(other), decltype(*this)>);
    assert(ref_.lock() == other.ref_.lock());
    return index_ == other.index_;
  }

  value_type& operator*() const { return (*ref_.lock())[index_][h_index_]; }
  Iterator& operator++() { ++index_; return *this; }
  Iterator operator++(int) { Iterator temp = *this; ++*this; return temp; }
  Iterator& operator--() { --index_; return *this; }
  Iterator operator--(int) { Iterator temp = *this; --*this; return temp; }
  auto operator<=>(const Iterator& other) const { return index_ <=> other.index_; };
  Iterator& operator+=(const difference_type& n) { index_ += n; return *this; }
  Iterator& operator-=(const difference_type& n) { index_ -= n; return *this; }

  Iterator operator+(const difference_type& n) const { return Iterator(ref_.lock(), h_index_, index_ + n); }
  friend Iterator operator+(const difference_type& n, const Iterator& i) { return Iterator(i.ref_.lock(), i.h_index_, i.index_ + i.index_); }
  Iterator operator-(const difference_type& n) const { return Iterator(ref_.lock(), h_index_, index_ - n); }
  difference_type operator-(const Iterator& other) const { return index_ - other.index_; };
  value_type& operator[](const difference_type& n) const { return (*ref_.lock())[index_ + n][h_index_]; }
private:
  std::weak_ptr<T> ref_;
  std::size_t index_ = 0;
  std::size_t h_index_ = 0;
};

template <typename T>
class vertical_view {
public:
  typedef inner_type_t<T> value_type;
//  typedef value_type* pointer;
//  typedef const value_type* const_pointer;
//  typedef value_type& reference;
//  typedef const value_type& const_reference;
  typedef Iterator<T> iterator;
//  typedef Iterator<const T> const_iterator;
//  typedef std::reverse_iterator<const_iterator>	const_reverse_iterator;
//  typedef std::reverse_iterator<iterator>		reverse_iterator;
//  typedef size_t size_type;
//  typedef ptrdiff_t difference_type;

  vertical_view(T& t, std::size_t h_index) : ref_(std::shared_ptr<T>(&t, [](T*){})), h_index_(h_index) {}
  vertical_view(const T& t, std::size_t h_index) : ref_(std::make_shared<T>(t)), h_index_(h_index) {}

  iterator begin() const { return iterator(ref_, h_index_); }
  iterator end() const { return iterator(ref_, h_index_, ref_->size()); }
  value_type& operator[](const auto& n) const { return (*ref_)[n][h_index_]; }

  std::shared_ptr<T> ref_;
  std::size_t h_index_ = 0;
};

static_assert(std::ranges::random_access_range<vertical_view<std::vector<std::vector<int>>>>);