
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3

struct Node {
  char value;
  bool seen = false;
  int distance = -1;
};

struct Pos {
  int row, col, dir;
};

using Grid = std::vector<std::vector<std::vector<Node>>>;

Pos get_next_pos(const Pos &pos) {
  if (pos.dir == NORTH)
    return Pos{pos.row - 1, pos.col, pos.dir};
  if (pos.dir == EAST)
    return Pos{pos.row, pos.col + 1, pos.dir};
  if (pos.dir == SOUTH)
    return Pos{pos.row + 1, pos.col, pos.dir};
  return Pos{pos.row, pos.col - 1, pos.dir}; // WEST
}

int advance_pos(const Grid &grid, Pos &pos) {
  // Consider exit as a node
  if (grid[pos.row][pos.col][pos.dir].value == 'E')
    return 0;

  bool next_found = false;
  Pos only_pos;
  int distance;
  for (int ddir = 0; ddir < 4; ++ddir) {
    if (ddir == 2)
      continue;

    Pos next_pos{pos.row, pos.col, (pos.dir + ddir) % 4};
    next_pos = get_next_pos(next_pos);

    const Node &node = grid.at(next_pos.row).at(next_pos.col).at(next_pos.dir);
    if (node.value == '#' || node.seen)
      continue;

    if (next_found) // Found several exits: we're on a node
      return 0;
    next_found = true;

    only_pos = next_pos;
    distance = (ddir == 0 ? 1 : 1001);
  }

  if (!next_found)
    return -1;

  pos = only_pos;
  return distance;
}

int get_shortest_path(Grid &grid, int start_row, int start_col, int start_dir) {
  grid[start_row][start_col][start_dir].distance = 0;

  auto cmp = [&grid](Pos &left, Pos &right) {
    return grid[left.row][left.col][left.dir].distance > grid[right.row][right.col][right.dir].distance;
  };
  std::priority_queue<Pos, std::vector<Pos>, decltype(cmp)> border(cmp);
  border.push(Pos{start_row, start_col, start_dir});

  while (!border.empty()) {
    Pos curr_pos = border.top();
    border.pop();

    Node &curr_node = grid[curr_pos.row][curr_pos.col][curr_pos.dir];

    if (curr_node.value == 'E')
      return curr_node.distance;

    curr_node.seen = true;

    for (int ddir = 0; ddir < 4; ++ddir) {
      if (ddir == 2) // No half-turn
        continue;

      Pos pos{curr_pos.row, curr_pos.col, (curr_pos.dir + ddir) % 4};
      if (ddir != 0 && grid[pos.row][pos.col][pos.dir].seen)
        continue;

      pos = get_next_pos(pos);
      if (grid[pos.row][pos.col][pos.dir].value == '#')
        continue;

      int dist_add = (ddir == 0 ? 0 : 1000);
      int step = 1;
      do {
        dist_add += step;
        step = advance_pos(grid, pos);
      } while (step > 0);

      if (step == -1) // Dead end or node already seen
        continue;

      Node &next_node = grid[pos.row][pos.col][pos.dir];
      if (next_node.distance == -1) {
        next_node.distance = curr_node.distance + dist_add;
        border.push(pos);
      } else if (next_node.distance > curr_node.distance + dist_add) {
        next_node.distance = curr_node.distance + dist_add;
      }
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
  int start_row, start_col, start_dir;
  if (myfile.is_open()) {
    std::string input;
    int row = 0;
    while (getline(myfile, input)) {
      std::vector<std::vector<Node>> column;
      for (char c : input) {
        std::vector<Node> directions;
        for (int i = 0; i < 4; ++i)
          directions.push_back(Node{c});
        column.push_back(directions);
      }
      grid.push_back(column);
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

  start_dir = EAST;
  ans = get_shortest_path(grid, start_row, start_col, start_dir);

  std::cout << "Answer : " << ans << std::endl;
  return 0;
}
