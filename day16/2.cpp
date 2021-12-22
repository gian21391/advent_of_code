//
// Created by Gianluca on 17/12/2021.
// SPDX-License-Identifier: MIT
//

#include <range/v3/all.hpp>

#include <iostream>
#include <fstream>
#include <cassert>
#include <deque>
#include <sstream>

auto hex_to_bin(const std::string& hex) {
  auto c_to_bin = [](char c) -> std::deque<bool> {
    switch (c) {
      case '0': return {0,0,0,0};
      case '1': return {0,0,0,1};
      case '2': return {0,0,1,0};
      case '3': return {0,0,1,1};
      case '4': return {0,1,0,0};
      case '5': return {0,1,0,1};
      case '6': return {0,1,1,0};
      case '7': return {0,1,1,1};
      case '8': return {1,0,0,0};
      case '9': return {1,0,0,1};
      case 'A': return {1,0,1,0};
      case 'B': return {1,0,1,1};
      case 'C': return {1,1,0,0};
      case 'D': return {1,1,0,1};
      case 'E': return {1,1,1,0};
      case 'F': return {1,1,1,1};
      default: assert(false);
    }
  };

  std::deque<bool> result;
  ranges::for_each(hex, [&](auto c){
    auto to_insert = c_to_bin(c);
    result.insert(result.end(), to_insert.begin(), to_insert.end());
  });
  return result;
}

auto range_to_string(auto&& range) {
  std::vector<char> temp;
  ranges::for_each(range, [&](const auto& elem){
    if (elem) temp.emplace_back('1');
    else temp.emplace_back('0');
  });
  return std::string(temp.begin(), temp.end());
}

struct packet_t {
  int version = 0;
  int type_id = 0;
  long content;
  std::vector<packet_t> subpackets;
};

void multibits_unpack(auto& packet, packet_t& main_structure);
std::size_t unpack(auto& packet, packet_t& structure);

std::size_t multipackets_unpack(const auto& packet, int n, packet_t& main_structure) {
  ranges::any_view<bool, ranges::category::forward | ranges::category::sized> fragment_v(packet);
  auto total_size = 0ul;
  for (int i = 0; i < n; ++i) {
    main_structure.subpackets.emplace_back();
    auto &structure = main_structure.subpackets.back();
    auto packet_size = unpack(fragment_v, structure);
    total_size += packet_size;
    fragment_v = fragment_v | ranges::views::drop(packet_size);
  }
  return total_size;
}

void multibits_unpack(auto&& packet, packet_t& main_structure) {
  ranges::any_view<bool, ranges::category::forward | ranges::category::sized> fragment_v(packet);

  while (fragment_v.begin() != fragment_v.end()) {
    main_structure.subpackets.emplace_back();
    auto &structure = main_structure.subpackets.back();
    auto packet_size = unpack(fragment_v, structure);
    fragment_v = fragment_v | ranges::views::drop(packet_size);
  }

}

std::size_t unpack(auto& packet, packet_t& structure) {
  ranges::any_view<bool, ranges::category::forward | ranges::category::sized> fragment_v = packet;

  structure.version = std::stoi(range_to_string(fragment_v | ranges::views::take(3)), nullptr, 2);
  structure.type_id = std::stoi(range_to_string(fragment_v | ranges::views::drop(3) | ranges::views::take(3)), nullptr, 2);
  fragment_v = fragment_v | ranges::views::drop(6);

  if (structure.type_id == 4) {
    std::stringstream ss;
    while (true) {
      auto data_v = fragment_v | ranges::views::take(5);
      ss << range_to_string(data_v | ranges::views::drop(1));
      fragment_v = fragment_v | ranges::views::drop(5);
      if (!*data_v.begin()) break;
    }
    structure.content = std::stol(ss.str(), nullptr, 2);
  }
  else {
    if (*fragment_v.begin()) { // 11 bits
      fragment_v = fragment_v | ranges::views::drop(1);
      int num_packets = std::stoi(range_to_string(fragment_v | ranges::views::take(11)), nullptr, 2);
      fragment_v = fragment_v | ranges::views::drop(11);
      auto num_bits = multipackets_unpack(fragment_v, num_packets, structure);
      fragment_v = fragment_v | ranges::views::drop(num_bits);
    }
    else { // 15 bits
      fragment_v = fragment_v | ranges::views::drop(1);
      int num_bits = std::stoi(range_to_string(fragment_v | ranges::views::take(15)), nullptr, 2);
      fragment_v = fragment_v | ranges::views::drop(15);
      multibits_unpack(fragment_v | ranges::views::take(num_bits), structure);
      fragment_v = fragment_v | ranges::views::drop(num_bits);
    }
  }

  auto packet_size = ranges::distance(packet.begin(), packet.end());
  auto fragment_v_size = ranges::distance(fragment_v.begin(), fragment_v.end());
  return packet_size - fragment_v_size;
}

void calculate(packet_t& packet) {
  ranges::for_each(packet.subpackets, [](packet_t& packet){ if (packet.type_id != 4) calculate(packet); });
  if (packet.type_id == 0) {
    ranges::for_each(packet.subpackets | ranges::views::transform([](const packet_t& packet){ return packet.content; }), [&](auto i){ packet.content += i; });
  }
  else if (packet.type_id == 1) {
    packet.content = 1;
    ranges::for_each(packet.subpackets | ranges::views::transform([](const packet_t& packet){ return packet.content; }), [&](auto i){ packet.content *= i; });
  }
  else if (packet.type_id == 2) {
    packet.content = ranges::min(packet.subpackets | ranges::views::transform([](const packet_t& packet){ return packet.content; }));
  }
  else if (packet.type_id == 3) {
    packet.content = ranges::max(packet.subpackets | ranges::views::transform([](const packet_t& packet){ return packet.content; }));
  }
  else if (packet.type_id == 5) {
    assert(packet.subpackets.size() == 2);
    if (packet.subpackets[0].content > packet.subpackets[1].content) packet.content = 1;
    else packet.content = 0;
  }
  else if (packet.type_id == 6) {
    assert(packet.subpackets.size() == 2);
    if (packet.subpackets[0].content < packet.subpackets[1].content) packet.content = 1;
    else packet.content = 0;
  }
  else if (packet.type_id == 7) {
    assert(packet.subpackets.size() == 2);
    if (packet.subpackets[0].content == packet.subpackets[1].content) packet.content = 1;
    else packet.content = 0;
  }
  else assert(false);
}

int main() {
  auto file = std::ifstream("../day16/input");
  assert(file.is_open());

  std::string line;
  std::getline(file, line);

  auto packet = hex_to_bin(line);

  packet_t structure;
  unpack(packet, structure);
  calculate(structure);

  std::cout << structure.content << std::endl;

  return 0;
}
