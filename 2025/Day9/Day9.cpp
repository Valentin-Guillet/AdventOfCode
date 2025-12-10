
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

using Tile = std::pair<int, int>;

struct Rectangle {
  int min_x;
  int max_x;
  int min_y;
  int max_y;
  long area;

  Rectangle(const Tile &tile1, const Tile &tile2) {
    if (tile1.first < tile2.first) {
      min_x = tile1.first;
      max_x = tile2.first;
    } else {
      min_x = tile2.first;
      max_x = tile1.first;
    }

    if (tile1.second < tile2.second) {
      min_y = tile1.second;
      max_y = tile2.second;
    } else {
      min_y = tile2.second;
      max_y = tile1.second;
    }

    area = (long)(max_x - min_x + 1) * (long)(max_y - min_y + 1);
  }

  bool operator<(const Rectangle &other) const { return area < other.area; }

  bool cross(const Rectangle &rect) const {
    return !(max_x <= rect.min_x || rect.max_x <= min_x ||
             max_y <= rect.min_y || rect.max_y <= min_y);
  }
};

class RectangleIterator {
public:
  RectangleIterator(std::vector<Rectangle> &all_rects)
      : rects(all_rects), index(0) {
    sort_indices.emplace_back(0, all_rects.size() - 1);
  }

  Rectangle next() {
    while (index >= sort_indices.back().first) {
      // Rectangle partial quicksort
      auto [low, high] = sort_indices.back();
      sort_indices.pop_back();

      size_t pivot = get_pivot(low, high);
      if (pivot + 1 < high)
        sort_indices.emplace_back(pivot + 1, high);
      if (low < pivot)
        sort_indices.emplace_back(low, pivot);
    }
    return rects[index++];
  }

private:
  std::vector<Rectangle> rects;
  size_t index;
  std::vector<std::pair<size_t, size_t>> sort_indices;

  size_t get_pivot(size_t low, size_t high) {
    long pivot_area = rects[low].area;
    while (true) {
      while (rects[low].area > pivot_area) {
        low++;
      }
      while (pivot_area > rects[high].area) {
        high--;
      }
      if (low >= high)
        return high;

      std::swap(rects[low], rects[high]);
      low++;
      high--;
    }
  }
};

int main(int argc, char *argv[]) {
  bool first_half = true;
  long ans = 0;

  std::ifstream myfile("input");
  for (int i = 1; i < argc; ++i) {
    std::string arg(argv[i]);
    if (arg == "-t" || arg == "--test")
      myfile = std::ifstream("test_input");

    else if (arg == "-s" || arg == "--second")
      first_half = false;
  }

  std::vector<Tile> tiles;
  if (myfile.is_open()) {
    std::string input;
    while (getline(myfile, input)) {
      size_t ind = input.find(',');
      int x = std::stoi(input.substr(0, ind));
      int y = std::stoi(input.substr(ind + 1));
      tiles.emplace_back(x, y);
    }
    myfile.close();
  } else {
    std::cout << "Unable to open file" << std::endl;
    return 1;
  }

  const size_t n = tiles.size();
  if (first_half) {
    for (size_t i = 0; i < n; ++i) {
      for (size_t j = i + 1; j < n; ++j) {
        Rectangle rect(tiles[i], tiles[j]);
        if (rect.area > ans)
          ans = rect.area;
      }
    }
  } else {
    // Get longest edge
    int longest_edge_dist = 0;
    size_t longest_edge_id = 0;
    for (size_t i = 0; i < n; ++i) {
      int edge_dist = std::abs(tiles[(i + 1) % n].first - tiles[i].first) +
                      std::abs(tiles[(i + 1) % n].second - tiles[i].second);
      if (edge_dist > longest_edge_dist) {
        longest_edge_dist = edge_dist;
        longest_edge_id = i;
      }
    }
    Rectangle longest_edge(tiles[longest_edge_id],
                           tiles[(longest_edge_id + 1) % n]);

    // Sort rectangles by area, but only if not crossed by longest edge
    std::vector<Rectangle> rectangles;
    rectangles.reserve(n * (n - 1) / 2);
    for (size_t i = 0; i < n; ++i) {
      for (size_t j = i + 1; j < n; ++j) {
        Rectangle rect(tiles[i], tiles[j]);
        if (!longest_edge.cross(rect))
          rectangles.emplace_back(std::move(rect));
      }
    }
    RectangleIterator rect_it(rectangles);

    while (!rectangles.empty()) {
      const Rectangle &rect = rect_it.next();
      bool is_valid = true;

      // Check that no edge cross the rectangle, starting with the longest one
      for (size_t ind = 0; ind < n; ++ind) {
        size_t i = (ind + longest_edge_id) % n;
        Rectangle edge(tiles[i], tiles[(i + 1) % n]);
        if (edge.cross(rect)) {
          is_valid = false;
          break;
        }
      }

      if (is_valid) {
        ans = rect.area;
        break;
      }
    }
  }

  std::cout << "Answer : " << ans << std::endl;
  return 0;
}
