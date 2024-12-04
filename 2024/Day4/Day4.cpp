
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using Grid = std::vector<std::string>;

int count_xmas(const Grid &grid, int row, int col) {
  int count = 0;
  for (int dx = -1; dx <= 1; ++dx) {
    if (!(0 <= row + 3 * dx && row + 3 * dx < (int)grid.size()))
      continue;

    for (int dy = -1; dy <= 1; ++dy) {
      if (dx == 0 && dy == 0)
        continue;

      if (!(0 <= col + 3 * dy && col + 3 * dy < (int)grid[0].size()))
        continue;

      bool complete = true;
      for (int i = 0; i < 4; ++i) {
        if (grid[row + i * dx][col + i * dy] != "XMAS"[i]) {
          complete = false;
          break;
        }
      }
      if (complete)
        count++;
    }
  }
  return count;
}

bool is_x_mas(const Grid &grid, int row, int col) {
  if (grid[row][col] != 'A' ||
      !(0 <= row - 1 && row + 1 < (int)grid.size()) ||
      !(0 <= col - 1 && col + 1 < (int)grid[0].size()))
    return false;

  int sum_1 = grid[row - 1][col - 1] + grid[row + 1][col + 1];
  int sum_2 = grid[row - 1][col + 1] + grid[row + 1][col - 1];
  return (sum_1 == 'M' + 'S' && sum_1 == sum_2);
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

  for (int row = 0; row < (int)grid.size(); ++row) {
    for (int col = 0; col < (int)grid[0].size(); ++col) {
      if (first_half)
        ans += count_xmas(grid, row, col);
      else
        ans += is_x_mas(grid, row, col);
    }
  }

  std::cout << "Answer : " << ans << std::endl;
  return 0;
}
