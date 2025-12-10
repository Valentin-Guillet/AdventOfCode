
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct Tile {
  int x;
  int y;

  // We represent the Tile quadrant by a flag:
  // NW: 0b0001, NE: 0b0010, SW: 0b0100, SE: 0b1000
  int quadrant;

  void set_quadrant(bool is_inner_left, const Tile &prev, const Tile &next) {
    bool left = (prev.x < x || next.x < x);
    bool up = (prev.y < y || next.y < y);
    quadrant = 1 << (2 * !up + !left);
    if (is_inner_left != Tile::is_left_turn(prev, *this, next))
      quadrant ^= 0b1111;
  }

  static bool is_left_turn(const Tile &tile1, const Tile &tile2,
                           const Tile &tile3) {
    int dx1 = tile2.x - tile1.x;
    int dx2 = tile3.x - tile2.x;
    int dy1 = tile2.y - tile1.y;
    int dy2 = tile3.y - tile2.y;

    return (dx1 * dy2 - dx2 * dy1 < 0);
  }

  static bool is_valid_rect(const Tile &tile1, const Tile &tile2) {
    if (tile1.x < tile2.x &&
        (!(tile1.quadrant & 0b1010) || !(tile2.quadrant & 0b0101)))
      return false;

    if (tile2.x < tile1.x &&
        (!(tile2.quadrant & 0b1010) || !(tile1.quadrant & 0b0101)))
      return false;

    if (tile1.y < tile2.y &&
        (!(tile1.quadrant & 0b1100) || !(tile2.quadrant & 0b0011)))
      return false;

    if (tile2.y < tile1.y &&
        (!(tile2.quadrant & 0b1100) || !(tile1.quadrant & 0b0011)))
      return false;

    return true;
  }
};

struct Rectangle {
  int min_x;
  int max_x;
  int min_y;
  int max_y;
  long area;

  Rectangle(const Tile &tile1, const Tile &tile2) {
    if (tile1.x < tile2.x) {
      min_x = tile1.x;
      max_x = tile2.x;
    } else {
      min_x = tile2.x;
      max_x = tile1.x;
    }

    if (tile1.y < tile2.y) {
      min_y = tile1.y;
      max_y = tile2.y;
    } else {
      min_y = tile2.y;
      max_y = tile1.y;
    }

    area = (long)(max_x - min_x + 1) * (long)(max_y - min_y + 1);
  }

  bool operator<(const Rectangle &other) const { return area < other.area; }

  bool cross(const Rectangle &rect) const {
    return !(max_x <= rect.min_x || rect.max_x <= min_x ||
             max_y <= rect.min_y || rect.max_y <= min_y);
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

  size_t easternmost_tile_id;
  int max_tile_x = 0;
  for (size_t i = 0; i < n; ++i) {
    int x = tiles[i].x;
    if (x > max_tile_x) {
      max_tile_x = x;
      easternmost_tile_id = i;
    }
  }

  bool is_inner_left = Tile::is_left_turn(
      tiles[(easternmost_tile_id + n - 1) % n], tiles[easternmost_tile_id],
      tiles[(easternmost_tile_id + 1) % n]);

  for (size_t i = 0; i < n; ++i) {
    const Tile &prev_tile = tiles[(easternmost_tile_id + i + n - 1) % n];
    Tile &tile = tiles[(easternmost_tile_id + i) % n];
    const Tile &next_tile = tiles[(easternmost_tile_id + i + 1) % n];
    tile.set_quadrant(is_inner_left, prev_tile, next_tile);
  }

  if (first_half) {
    for (size_t i = 0; i < n; ++i) {
      for (size_t j = i + 1; j < n; ++j) {
        if (!Tile::is_valid_rect(tiles[i], tiles[j]))
          continue;

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
      int edge_dist = std::abs(tiles[(i + 1) % n].x - tiles[i].x) +
                      std::abs(tiles[(i + 1) % n].y - tiles[i].y);
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
        if (!Tile::is_valid_rect(tiles[i], tiles[j]))
          continue;

        Rectangle rect(tiles[i], tiles[j]);
        if (!longest_edge.cross(rect))
          rectangles.emplace_back(std::move(rect));
      }
    }
    std::sort(
        rectangles.begin(), rectangles.end(),
        [&](const Rectangle &r1, const Rectangle &r2) { return r2 < r1; });

    for (const Rectangle &rect : rectangles) {
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
