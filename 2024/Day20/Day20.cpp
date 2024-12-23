
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct Pos {
  int row;
  int col;
};

using Grid = std::vector<std::string>;
using DistanceMap = std::vector<std::vector<int>>;

void step(const Grid &grid, const DistanceMap &distances, Pos &pos) {
  // North
  if (pos.row > 1 && grid[pos.row - 1][pos.col] != '#' && distances[pos.row - 1][pos.col] == -1) {
    pos.row--;
    return;
  }

  // East
  if (pos.col < (int)grid[0].size() - 1 && grid[pos.row][pos.col + 1] != '#' && distances[pos.row][pos.col + 1] == -1) {
    pos.col++;
    return;
  }

  // South
  if (pos.row < (int)grid.size() - 1 && grid[pos.row + 1][pos.col] != '#' && distances[pos.row + 1][pos.col] == -1) {
    pos.row++;
    return;
  }

  // West
  pos.col--;
}

int count_neighbors(const Grid &grid, const DistanceMap &distances, const Pos &pos, int distance, int length) {
  int count = 0;
  int min_drow = -std::min(length, pos.row - 1);
  int max_drow =  std::min(length, (int)grid.size() - pos.row - 2);
  for (int i = min_drow; i <= max_drow; ++i) {
    int min_dcol = -std::min(length - std::abs(i), pos.col - 1);
    int max_dcol =  std::min(length - std::abs(i), (int)grid[0].size() - pos.col - 2);
    for (int j = min_dcol; j <= max_dcol; ++j) {
      if (i == 0 && j == 0)
        continue;

      int dist = distances[pos.row + i][pos.col + j];
      if (dist != -1 && distance - dist - std::abs(i) - std::abs(j) >= 100)
        count++;
    }
  }
  return count;
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
  Pos start;
  if (myfile.is_open()) {
    std::string input;
    int row = 0;
    while (getline(myfile, input)) {
      grid.push_back(input);
      size_t index = input.find('S');
      if (index != std::string::npos)
        start = {row, (int)index};
      row++;
    }
    myfile.close();
  } else {
    std::cout << "Unable to open file" << std::endl;
    return 1;
  }

  int cheat_length = (first_half ? 2 : 20);
  DistanceMap distances(grid.size(), std::vector<int>(grid[0].size(), -1));

  Pos pos(start);
  int distance = 0;
  while (true) {
    distances[pos.row][pos.col] = distance;
    ans += count_neighbors(grid, distances, pos, distance, cheat_length);

    if (grid[pos.row][pos.col] == 'E')
      break;

    step(grid, distances, pos);
    distance++;
  }

  std::cout << "Answer : " << ans << std::endl;
  return 0;
}
