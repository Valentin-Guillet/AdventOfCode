
#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

struct Edge {
  int first;
  int second;
  long distance;

  bool operator<(const Edge &other) const { return distance < other.distance; }
};

struct Box {
  int x;
  int y;
  int z;

  Box(const std::string &input) {
    size_t ind1 = input.find(',');
    size_t ind2 = input.find(',', ind1 + 1);

    x = std::stoi(input.substr(0, ind1));
    y = std::stoi(input.substr(ind1 + 1, ind2 - ind1 - 1));
    z = std::stoi(input.substr(ind2 + 1));
  }

  static long compute_dist(const Box &box1, const Box &box2) {
    long dx = box2.x - box1.x;
    long dy = box2.y - box1.y;
    long dz = box2.z - box1.z;
    return dx * dx + dy * dy + dz * dz;
  }
};

class EdgeIterator {
public:
  EdgeIterator(std::vector<Edge> &all_edges) : edges(all_edges), index(0) {
    sort_indices.emplace_back(0, all_edges.size() - 1);
  }

  Edge next() {
    while (index >= sort_indices.back().first) {
      // Edge partial quicksort
      auto [low, high] = sort_indices.back();
      sort_indices.pop_back();

      size_t pivot = get_pivot(low, high);
      if (pivot + 1 < high)
        sort_indices.emplace_back(pivot + 1, high);
      if (low < pivot)
        sort_indices.emplace_back(low, pivot);
    }
    return edges[index++];
  }

private:
  std::vector<Edge> edges;
  size_t index;
  std::vector<std::pair<size_t, size_t>> sort_indices;

  size_t get_pivot(size_t low, size_t high) {
    long pivot_dist = edges[low].distance;
    while (true) {
      while (edges[low].distance < pivot_dist) {
        low++;
      }
      while (pivot_dist < edges[high].distance) {
        high--;
      }
      if (low >= high)
        return high;

      std::swap(edges[low], edges[high]);
      low++;
      high--;
    }
  }
};

std::vector<int> get_nth_groups(EdgeIterator &edge_it, size_t nb_boxes,
                                size_t nb_edges) {
  std::vector<int> groups;
  groups.reserve(nb_boxes);
  for (size_t i = 0; i < nb_boxes; ++i)
    groups.emplace_back(i);

  for (size_t count = 0; count < nb_edges; ++count) {
    auto [i, j, _] = edge_it.next();
    if (groups[i] == groups[j])
      continue;

    int prev_group = groups[j];
    for (size_t k = 0; k < nb_boxes; ++k) {
      if (groups[k] == prev_group) {
        groups[k] = groups[i];
      }
    }
  }

  return groups;
}

std::vector<int>
sort_frequencies(const std::unordered_map<int, int> &frequencies, size_t size) {
  std::vector<int> sorted_freq;
  sorted_freq.reserve(size);
  for (auto &[_, freq] : frequencies)
    sorted_freq.emplace_back(freq);

  std::partial_sort(sorted_freq.begin(), sorted_freq.begin() + 3,
                    sorted_freq.end(), std::greater<>());
  return sorted_freq;
}

Edge get_last_edge_id(EdgeIterator &edge_it, size_t nb_boxes) {
  std::vector<int> groups;
  groups.reserve(nb_boxes);
  for (size_t i = 0; i < nb_boxes; ++i)
    groups.emplace_back(i);

  size_t nb_edges = 0;
  while (true) {
    auto [i, j, _] = edge_it.next();
    if (groups[i] == groups[j])
      continue;

    nb_edges++;
    if (nb_edges == nb_boxes - 1)
      return {i, j, 0};

    int prev_group = groups[j];
    for (size_t k = 0; k < nb_boxes; ++k) {
      if (groups[k] == prev_group) {
        groups[k] = groups[i];
      }
    }
  }
}

int main(int argc, char *argv[]) {
  bool first_half = true;
  int ans = 1;

  std::ifstream myfile("input");
  for (int i = 1; i < argc; ++i) {
    std::string arg(argv[i]);
    if (arg == "-t" || arg == "--test")
      myfile = std::ifstream("test_input");

    else if (arg == "-s" || arg == "--second")
      first_half = false;
  }

  std::vector<Box> boxes;
  if (myfile.is_open()) {
    std::string input;
    while (getline(myfile, input))
      boxes.emplace_back(std::move(input));
    myfile.close();
  } else {
    std::cout << "Unable to open file" << std::endl;
    return 1;
  }

  const size_t n = boxes.size();
  std::vector<Edge> edges;
  edges.reserve(n * (n - 1) / 2);
  for (int i = 0; i < (int)n - 1; ++i) {
    for (int j = i + 1; j < (int)n; ++j) {
      long dist = Box::compute_dist(boxes[i], boxes[j]);
      edges.emplace_back(i, j, dist);
    }
  }
  EdgeIterator edge_it(edges);

  if (first_half) {
    std::vector<int> groups = get_nth_groups(edge_it, boxes.size(), 1000);

    std::unordered_map<int, int> frequencies;
    for (int group : groups)
      frequencies[group]++;
    std::vector<int> sorted_freq = sort_frequencies(frequencies, groups.size());

    for (int i = 0; i < 3; ++i)
      ans *= sorted_freq[i];

  } else {
    auto [i, j, _] = get_last_edge_id(edge_it, boxes.size());
    ans = boxes[i].x * boxes[j].x;
  }

  std::cout << "Answer : " << ans << std::endl;
  return 0;
}
