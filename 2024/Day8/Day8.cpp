
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct Pos {
  int row;
  int col;

  bool operator==(const Pos &other) const {
    return (row == other.row && col == other.col);
  }
};

struct pos_hash {
  size_t operator()(const Pos &pos) const {
    size_t hash = pos.row + 0x9e3779b9;
    return (hash << 6) + (hash >> 2) + pos.col + 0x9e3779b9;
  }
};

using Grid = std::vector<std::string>;
using AntennaMap = std::unordered_map<char, std::vector<Pos>>;
using AntinodeSet = std::unordered_set<Pos, pos_hash>;

AntennaMap parse_antennas(const Grid &grid) {
  AntennaMap antenna_map;
  for (size_t row = 0; row < grid.size(); ++row) {
    for (size_t col = 0; col < grid[0].size(); ++col) {
      char c = grid.at(row).at(col);
      if (c != '.')
        antenna_map[c].push_back(Pos{(int)row, (int)col});
    }
  }
  return antenna_map;
}

void add_antinodes(const Grid &grid, AntinodeSet &antinodes,
                   const Pos &pos1, const Pos &pos2) {
  int x1 = 2 * pos2.row - pos1.row;
  int y1 = 2 * pos2.col - pos1.col;
  if (0 <= x1 && x1 < (int)grid.size() && 0 <= y1 && y1 < (int)grid[0].size())
    antinodes.insert(Pos{x1, y1});

  int x2 = 2 * pos1.row - pos2.row;
  int y2 = 2 * pos1.col - pos2.col;
  if (0 <= x2 && x2 < (int)grid.size() && 0 <= y2 && y2 < (int)grid[0].size())
    antinodes.insert(Pos{x2, y2});
}

void add_resonant_antinodes(const Grid &grid, AntinodeSet &antinodes,
                            const Pos &pos1, const Pos &pos2) {
  int dx = pos2.row - pos1.row;
  int dy = pos2.col - pos1.col;
  int x = pos2.row;
  int y = pos2.col;
  do {
    antinodes.insert(Pos{x, y});
    x += dx;
    y += dy;
  } while (0 <= x && x < (int)grid.size() && 0 <= y && y < (int)grid[0].size());

  x = pos1.row;
  y = pos1.col;
  do {
    antinodes.insert(Pos{x, y});
    x -= dx;
    y -= dy;
  } while (0 <= x && x < (int)grid.size() && 0 <= y && y < (int)grid[0].size());
}

int main(int argc, char *argv[]) {
  bool first_half = true;
  int ans = 0;

  std::ifstream myfile("input");
  for (int i = 1; i < argc; ++i) {
    std::string arg(argv[i]);
    if (arg == "-t" || arg == "--test")
      myfile = std::ifstream("test_input");

    else if (arg == "-s" || arg == "--second")
      first_half = false;
  }

  Grid grid;
  if (myfile.is_open()) {
    std::string input;
    while (getline(myfile, input))
      grid.push_back(input);
    myfile.close();
  } else {
    std::cout << "Unable to open file" << std::endl;
    return 1;
  }

  AntennaMap antenna_map = parse_antennas(grid);

  AntinodeSet antinodes;
  for (const auto &[_, antennas] : antenna_map) {
    for (size_t i = 1; i < antennas.size(); ++i) {
      for (size_t j = 0; j < i; ++j) {
        if (first_half)
          add_antinodes(grid, antinodes, antennas[i], antennas[j]);
        else
          add_resonant_antinodes(grid, antinodes, antennas[i], antennas[j]);
      }
    }
  }
  ans = antinodes.size();

  std::cout << "Answer : " << ans << std::endl;
  return 0;
}
