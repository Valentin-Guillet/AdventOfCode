
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <utility>
#include <vector>

template <class T> using Grid = std::vector<std::vector<T>>;

int get_nb_neighbor(const Grid<char> &grid, const int &row, const int &col) {
  int nb_adjacents = 0;
  for (int drow = -1; drow <= 1; ++drow) {
    for (int dcol = -1; dcol <= 1; ++dcol) {
      if (drow == 0 && dcol == 0)
        continue;
      if (!(0 <= row + drow && row + drow < (int)grid.size() &&
            0 <= col + dcol && col + dcol < (int)grid[0].size()))
        continue;
      nb_adjacents += grid[row + drow][col + dcol] == '@';
    }
  }
  return nb_adjacents;
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

  Grid<char> grid;
  if (myfile.is_open()) {
    std::string input;
    while (getline(myfile, input))
      grid.emplace_back(std::vector<char>(input.begin(), input.end()));
    myfile.close();
  } else {
    std::cout << "Unable to open file" << std::endl;
    return 1;
  }

  if (first_half) {
    for (size_t row = 0; row < grid.size(); ++row) {
      for (size_t col = 0; col < grid[0].size(); ++col) {
        if (grid[row][col] == '@')
          ans += get_nb_neighbor(grid, row, col) < 4;
      }
    }
  } else {
    Grid<int> nb_neighbors(grid.size(), std::vector<int>(grid[0].size(), 0));
    std::queue<std::pair<int, int>> to_remove;
    for (size_t row = 0; row < grid.size(); ++row) {
      for (size_t col = 0; col < grid[0].size(); ++col) {
        if (grid[row][col] == '.')
          continue;

        int nb_neighbor = get_nb_neighbor(grid, row, col);
        nb_neighbors[row][col] = nb_neighbor;
        if (nb_neighbor < 4)
          to_remove.emplace(row, col);
      }
    }

    while (!to_remove.empty()) {
      auto [row, col] = to_remove.front();
      to_remove.pop();
      ans++;

      for (int drow = -1; drow <= 1; ++drow) {
        for (int dcol = -1; dcol <= 1; ++dcol) {
          if (drow == 0 && dcol == 0)
            continue;
          if (!(-drow <= row && row + drow < (int)grid.size() &&
                0 <= col + dcol && col + dcol < (int)grid[0].size()))
            continue;

          int &neigh_nb = nb_neighbors[row + drow][col + dcol];
          if (neigh_nb == 0)
            continue;

          neigh_nb--;
          if (neigh_nb == 3)
            to_remove.emplace(row + drow, col + dcol);
        }
      }
    }
  }

  std::cout << "Answer : " << ans << std::endl;
  return 0;
}
