//
// Created by Gianluca on 17/12/2021.
// SPDX-License-Identifier: MIT
//

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

#include <iostream>
#include <fstream>
#include <cassert>
#include <ranges>

using graph_t = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property, boost::property<boost::edge_weight_t, int>>;
using vertex_descriptor = graph_t::vertex_descriptor;

struct early_stop_visitor : boost::default_dijkstra_visitor {
  using base = boost::default_dijkstra_visitor;
  struct done{};

  explicit early_stop_visitor(vertex_descriptor vd) : destination(vd) {}

  void finish_vertex(vertex_descriptor v, const graph_t& g) {
    if (v == destination) throw done{};
    base::finish_vertex(v, g);
  }

private:
  vertex_descriptor destination;
};

int main() {
  auto file = std::ifstream("../day15/input");
  assert(file.is_open());

  std::vector<std::vector<int>> weight_matrix;

  for (std::string line; std::getline(file, line); ) {
    weight_matrix.emplace_back();
    std::ranges::for_each(line, [&](auto c){ weight_matrix.back().emplace_back(c - '0'); });
  }

  auto tile_size = weight_matrix.size();
  weight_matrix.resize(tile_size * 5);
  std::ranges::for_each(weight_matrix, [&](auto& item){ item.resize(tile_size * 5); });

  for (auto h_tile_index = 1; h_tile_index < 5; ++h_tile_index) {
    for (auto v_tile_index = 0; v_tile_index < tile_size; ++v_tile_index) {
      auto origin_view = std::ranges::take_view(weight_matrix[((h_tile_index - 1) * tile_size) + v_tile_index], tile_size);
      auto insert_view = std::ranges::take_view(weight_matrix[(h_tile_index * tile_size) + v_tile_index], tile_size);
      std::transform(origin_view.begin(), origin_view.end(), insert_view.begin(), [&](const auto &num) { return (num + 1 == 10 ? 1 : num + 1); });
    }
  }

  std::ranges::for_each(weight_matrix, [&](auto& item){
    for (auto h_tile_index = 1; h_tile_index < 5; ++h_tile_index) {
      auto origin_view = std::ranges::take_view(std::ranges::drop_view(item, (h_tile_index - 1) * tile_size), tile_size);
      auto insert_view = std::ranges::take_view(std::ranges::drop_view(item, h_tile_index * tile_size), tile_size);
      std::transform(origin_view.begin(), origin_view.end(), insert_view.begin(), [&](const auto &num) { return (num + 1 == 10 ? 1 : num + 1); });
    }
  });

  std::vector<std::pair<int, int>> edges;
  std::vector<int> weights;
  for (int i = 0; i < weight_matrix.size(); ++i) {
    for (int j = 0; j < weight_matrix[i].size(); ++j) {
      if (i > 0) {
        edges.emplace_back(std::pair(weight_matrix.size() * i + j, (weight_matrix.size() * (i-1)) + j));
        weights.emplace_back(weight_matrix[i - 1][j]);
      }
      if (i < weight_matrix.size() - 1) {
        edges.emplace_back(std::pair(weight_matrix.size() * i + j, (weight_matrix.size() * (i+1)) + j));
        weights.emplace_back(weight_matrix[i + 1][j]);
      }
      if (j > 0) {
        edges.emplace_back(std::pair(weight_matrix.size() * i + j, (weight_matrix.size() * i) + j - 1));
        weights.emplace_back(weight_matrix[i][j - 1]);
      }
      if (j < weight_matrix[i].size() - 1) {
        edges.emplace_back(std::pair(weight_matrix.size() * i + j, (weight_matrix.size() * i) + j + 1));
        weights.emplace_back(weight_matrix[i][j + 1]);
      }
    }
  }

  int start_vertex = 0;
  int end_vertex = (weight_matrix.size() * weight_matrix[0].size()) - 1;

  graph_t g(edges.begin(), edges.end(), weights.begin(), weight_matrix.size() * weight_matrix[0].size());

  std::vector<int> distances(boost::num_vertices(g));
  early_stop_visitor vis(end_vertex);

  try {
    dijkstra_shortest_paths(g, start_vertex, boost::visitor(vis).distance_map(boost::make_iterator_property_map(distances.begin(), get(boost::vertex_index, g))));
  }
  catch(early_stop_visitor::done const&) {}

  std::cout << distances[end_vertex] << std::endl;

  return 0;
}
