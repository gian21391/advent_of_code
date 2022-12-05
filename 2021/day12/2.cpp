//
// Created by Gianluca on 12/12/2021.
// SPDX-License-Identifier: MIT
//

#include <boost/graph/adjacency_list.hpp>

#include <fstream>
#include <regex>
#include <iostream>

struct vertex_properties {
  bool lower = false;
};

using vertex_t = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, vertex_properties>::vertex_descriptor;

void all_paths_helper(auto from, auto target, auto& g, auto& path, const auto& duplicate_vertex, auto&& callback) {
  path.push_back(from);

  if (from == target) {
    callback(path);
  }
  else {
    for (auto vertex: boost::make_iterator_range(boost::adjacent_vertices(from, g))) {
      if (std::ranges::find(path, vertex) == std::ranges::end(path) || !g[vertex].lower || (vertex == duplicate_vertex && std::ranges::count(path, duplicate_vertex) < 2)) {
        all_paths_helper(vertex, target, g, path, duplicate_vertex, callback);
      }
    }
  }

  path.pop_back();
}

void all_paths(auto from, auto to, auto& g, const auto& to_be_duplicated, auto&& callback) {
  std::vector<vertex_t> state;
  for (const auto& item : to_be_duplicated) {
    all_paths_helper(from, to, g, state, item, callback);
  }
}

int main() {
  auto file = std::ifstream("../day12/input");
  assert(file.is_open());

  std::set<std::string> caves_set;
  std::vector<std::pair<std::string, std::string>> pairs;

  std::regex adjacency_list_regex("(\\w+)-(\\w+)");
  std::smatch adjacency_match;

  for (std::string line; std::getline(file, line);) {
    std::regex_match(line, adjacency_match, adjacency_list_regex);
    caves_set.emplace(adjacency_match[1]);
    caves_set.emplace(adjacency_match[2]);
    pairs.emplace_back(adjacency_match[1], adjacency_match[2]);
  }

  std::vector<std::string> caves_vector(caves_set.begin(), caves_set.end());
  boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, vertex_properties> g;

  for (const auto& item : pairs) {
    auto id1 = std::distance(std::ranges::begin(caves_vector), std::ranges::find(caves_vector, item.first));
    auto id2 = std::distance(std::ranges::begin(caves_vector), std::ranges::find(caves_vector, item.second));
    boost::add_edge(id1, id2, g);
  }

  auto start_id = std::distance(std::ranges::begin(caves_vector), std::ranges::find(caves_vector, "start"));
  auto end_id = std::distance(std::ranges::begin(caves_vector), std::ranges::find(caves_vector, "end"));

  std::vector<vertex_t> to_be_duplicated;
  auto vertices = boost::vertices(g);
  for (auto it = vertices.first; it != vertices.second; ++it) {
    if (std::islower(caves_vector[*it][0]) != 0) g[*it].lower = true;
    if (std::islower(caves_vector[*it][0]) != 0 && *it != start_id && *it != end_id) to_be_duplicated.emplace_back(*it);
  }

  std::vector<std::vector<vertex_t>> paths;
  all_paths(start_id, end_id, g, to_be_duplicated, [&](const auto& path){ paths.emplace_back(path); });

  std::ranges::sort(paths);
  auto ret = std::ranges::unique(paths);
  paths.erase(ret.begin(), ret.end());
  std::cout << paths.size() << std::endl;

  return 0;
}