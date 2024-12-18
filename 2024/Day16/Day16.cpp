
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

struct Pos {
  int row;
  int col;
  int drow;
  int dcol;
  int distance = -1;
  bool operator==(const Pos &other) const {
    return (row == other.row && col == other.col &&
        drow == other.drow && dcol == other.dcol);
  }
};

struct pos_hash {
  size_t operator()(const Pos &pos) const {
    size_t hash = pos.row + 0x9e3779b9;
    hash = (hash << 6) + (hash >> 2) + pos.col + 0x9e3779b9;
    hash = (hash << 6) + (hash >> 2) + ((pos.drow + 1) << 4) + pos.dcol + 1 + 0x9e3779b9;
    return hash;
  }
};

std::ostream& operator<<(std::ostream &os, const Pos &pos) {
  os << "(" << pos.row << ", " << pos.col << ", ";
  if (pos.drow == 1) os << "SOUTH, ";
  else if (pos.drow == -1) os << "NORTH, ";
  else if (pos.dcol == 1) os << "EAST, ";
  else if (pos.dcol == -1) os << "WEST, ";
  std::cout << pos.distance << ")";
  return os;
}

using Grid = std::vector<std::string>;
using SeenSet = std::unordered_set<Pos, pos_hash>;

std::vector<Pos> get_neighbors(const Grid &grid, const Pos &pos) {
  std::vector<Pos> neighbors;

  // Forward
  int next_row = pos.row + pos.drow;
  int next_col = pos.col + pos.dcol;
  if (grid.at(next_row).at(next_col) != '#')
    neighbors.push_back(Pos{next_row, next_col, pos.drow, pos.dcol, pos.distance + 1});

  // Turn left
  neighbors.push_back(Pos{pos.row, pos.col, -pos.dcol, pos.drow, pos.distance + 1000});

  // Turn right
  neighbors.push_back(Pos{pos.row, pos.col, pos.dcol, -pos.drow, pos.distance + 1000});
  return neighbors;
}

int get_shortest_path(const Grid &grid, int start_row, int start_col) {
  SeenSet seen;
  seen.insert(Pos{start_row, start_col, 0, 1});

  std::vector<Pos> border = {Pos{start_row, start_col, 0, 1, 0}};
  while (!border.empty()) {
    std::vector<Pos>::iterator closest_pos_it;
    int min_dist = 1000000;
    for (auto it = border.begin(); it != border.end(); ++it) {
      if (it->distance < min_dist) {
        min_dist = it->distance;
        closest_pos_it = it;
      }
    }
    Pos curr_pos = *closest_pos_it;
    border.erase(closest_pos_it);

    if (grid.at(curr_pos.row).at(curr_pos.col) == 'E')
      return curr_pos.distance;

    seen.insert(curr_pos);

    for (Pos neighbor : get_neighbors(grid, curr_pos)) {
      if (seen.contains(neighbor))
        continue;

      auto neigh_it = std::find(border.begin(), border.end(), neighbor);
      if (neigh_it != border.end() && neighbor.distance < neigh_it->distance)
        neigh_it->distance = neighbor.distance;
      else if (neigh_it == border.end())
        border.push_back(neighbor);
    }
  }

  return -1;
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
  int start_row, start_col;
  if (myfile.is_open()) {
    std::string input;
    int row = 0;
    while (getline(myfile, input)) {
      grid.push_back(input);
      size_t index = input.find('S');
      if (index != std::string::npos) {
        start_row = row;
        start_col = index;
      }
      row++;
    }
    myfile.close();
  } else {
    std::cout << "Unable to open file" << std::endl;
    return 1;
  }

  ans = get_shortest_path(grid, start_row, start_col);

  std::cout << "Answer : " << ans << std::endl;
  return 0;
}
