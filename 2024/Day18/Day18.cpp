
#include <array>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>

#define NE 1
#define SW 2
const int SIZE = 71;

template <class T> using Grid = std::array<std::array<T, SIZE>, SIZE>;

struct Pos {
  int row;
  int col;
};

template <class T> void fill(Grid<T> &grid, T value) {
  for (size_t row = 0; row < SIZE; ++row) {
    for (size_t col = 0; col < SIZE; ++col)
      grid[row][col] = value;
  }
}

int get_shortest_path(const Grid<bool> &grid) {
  std::queue<Pos> positions;
  positions.emplace(0, 0);
  Grid<int> dist;
  fill(dist, -1);
  dist[0][0] = 0;

  while (!positions.empty()) {
    Pos pos = positions.front();
    positions.pop();

    if (pos.row == SIZE - 1 && pos.col == SIZE - 1)
      return dist[pos.row][pos.col];

    // North
    if (pos.row > 0 && grid[pos.row - 1][pos.col] && dist[pos.row - 1][pos.col] == -1) {
      dist[pos.row - 1][pos.col] = dist[pos.row][pos.col] + 1;
      positions.emplace(pos.row - 1, pos.col);
    }
    // East
    if (pos.col < SIZE - 1 && grid[pos.row][pos.col + 1] && dist[pos.row][pos.col + 1] == -1) {
      dist[pos.row][pos.col + 1] = dist[pos.row][pos.col] + 1;
      positions.emplace(pos.row, pos.col + 1);
    }
    // South
    if (pos.row < SIZE - 1 && grid[pos.row + 1][pos.col] && dist[pos.row + 1][pos.col] == -1) {
      dist[pos.row + 1][pos.col] = dist[pos.row][pos.col] + 1;
      positions.emplace(pos.row + 1, pos.col);
    }
    // West
    if (pos.col > 0 && grid[pos.row][pos.col - 1] && dist[pos.row][pos.col - 1] == -1) {
      dist[pos.row][pos.col - 1] = dist[pos.row][pos.col] + 1;
      positions.emplace(pos.row, pos.col - 1);
    }
  }
  return 0;
}

bool update_obstacles(Grid<int> &connected_walls, Pos obstacle) {
  std::queue<Pos> to_update;
  int connected_wall = 0;
  for (int drow = -1; drow <= 1; ++drow) {
    for (int dcol = -1; dcol <= 1; ++dcol) {
      if (drow == 0 && dcol == 0)
        continue;

      int nrow = obstacle.row + drow;
      int ncol = obstacle.col + dcol;
      if (nrow == -1 || ncol == SIZE) {
        connected_wall |= NE;
        continue;
      }
      if (nrow == SIZE || ncol == -1) {
        connected_wall |= SW;
        continue;
      }

      if (connected_walls[nrow][ncol] != -1) {
        connected_wall |= connected_walls[nrow][ncol];
        to_update.emplace(nrow, ncol);
      }
    }
  }

  if (connected_wall == (NE | SW))
    return true;

  connected_walls[obstacle.row][obstacle.col] = connected_wall;
  while (!to_update.empty()) {
    Pos neigh = to_update.front();
    to_update.pop();

    if ((connected_wall ^ connected_walls[neigh.row][neigh.col]) == 0)
      continue;
    if (update_obstacles(connected_walls, neigh))
      return true;
  }
  return false;
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

  std::queue<Pos> obstacles;
  if (myfile.is_open()) {
    std::string input;
    while (getline(myfile, input)) {
      size_t index = input.find(',');
      int x = std::stoi(input.substr(0, index));
      int y = std::stoi(input.substr(index + 1));
      obstacles.emplace(y, x);
    }
    myfile.close();
  } else {
    std::cout << "Unable to open file" << std::endl;
    return 1;
  }

  if (first_half) {
    Grid<bool> grid;
    fill(grid, true);

    for (size_t i = 0; i < 1024; ++i) {
      grid[obstacles.front().row][obstacles.front().col] = false;
      obstacles.pop();
    }
    ans = get_shortest_path(grid);
  } else {
    Grid<int> connected_walls;
    fill(connected_walls, -1);

    while (!obstacles.empty()) {
      Pos obstacle = obstacles.front();
      obstacles.pop();

      if (update_obstacles(connected_walls, obstacle)) {
        std::cout << obstacle.col << ',' << obstacle.row << '\n';
        break;
      }
    }
  }

  std::cout << "Answer : " << ans << std::endl;
  return 0;
}
