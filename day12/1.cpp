#include <boost/graph/adjacency_list.hpp>

#include <fstream>
#include <regex>
#include <iostream>

struct vertex_properties {
  bool lower = false;
};

using vertex_t = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, vertex_properties>::vertex_descriptor;

void all_paths_helper(auto from, auto target, const auto& g, auto& path, auto&& callback) {
  path.push_back(from);

  if (from == target) callback(path);
  else {
    for (auto vertex: boost::make_iterator_range(boost::adjacent_vertices(from, g))) {
      if (std::ranges::find(path, vertex) == std::ranges::end(path) || !g[vertex].lower) {
        all_paths_helper(vertex, target, g, path, callback);
      }
    }
  }

  path.pop_back();
}

void all_paths(auto from, auto to, const auto& g, auto&& callback) {
  std::vector<vertex_t> state;
  all_paths_helper(from, to, g, state, callback);
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

  auto vertices = boost::vertices(g);
  for (auto it = vertices.first; it != vertices.second; ++it) {
    if (std::islower(caves_vector[*it][0]) != 0) g[*it].lower = true;
  }

  auto start_id = std::distance(std::ranges::begin(caves_vector), std::ranges::find(caves_vector, "start"));
  auto end_id = std::distance(std::ranges::begin(caves_vector), std::ranges::find(caves_vector, "end"));

  int count = 0;
  all_paths(start_id, end_id, g, [&](const auto& path) { count++; });
  std::cout << count << std::endl;

  return 0;
}