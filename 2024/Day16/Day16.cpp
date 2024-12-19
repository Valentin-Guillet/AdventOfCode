
#include <fstream>
#include <iostream>
#include <string>
#include <queue>
#include <unordered_set>
#include <vector>

#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3

struct Pos {
  int row, col, dir;
};

struct Node {
  char value;
  bool seen = false;
  int distance = -1;
  std::vector<Pos> parents = {};
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

int advance_pos(Grid &grid, Pos &pos, Pos &last_pos) {
  // Consider start and exit as nodes
  char value = grid[pos.row][pos.col][pos.dir].value;
  if (value == 'E' || value == 'S')
    return 0;

  bool next_found = false;
  Pos only_pos;
  int distance;
  for (int ddir = 0; ddir < 4; ++ddir) {
    if (ddir == 2)
      continue;

    Pos next_pos{pos.row, pos.col, (pos.dir + ddir) % 4};
    next_pos = get_next_pos(next_pos);

    if (grid[next_pos.row][next_pos.col][next_pos.dir].value == '#')
      continue;

    if (next_found) // Found several exits: we're on a node
      return 0;
    next_found = true;

    only_pos = next_pos;
    distance = (ddir == 0 ? 1 : 1001);
  }

  if (!next_found) // Found no exit: dead end
    return -1;

  grid[pos.row][pos.col][pos.dir].parents.push_back(last_pos);
  last_pos = pos;
  pos = only_pos;
  return distance;
}

int get_shortest_path(Grid &grid, int start_row, int start_col, bool mark_parents = false) {
  grid[start_row][start_col][EAST].distance = 0;

  auto cmp = [&](Pos &left, Pos &right) {
    return grid[left.row][left.col][left.dir].distance > grid[right.row][right.col][right.dir].distance;
  };
  std::priority_queue<Pos, std::vector<Pos>, decltype(cmp)> border(cmp);
  border.push(Pos{start_row, start_col, EAST});

  int found_dist = -1;
  while (!border.empty()) {
    Pos curr_pos = border.top();
    border.pop();

    Node &curr_node = grid[curr_pos.row][curr_pos.col][curr_pos.dir];
    if (!mark_parents && curr_node.value == 'E')
      return curr_node.distance;

    if (mark_parents && found_dist != -1 && curr_node.distance > found_dist)
      return 0;

    if (curr_node.seen)
      continue;
    curr_node.seen = true;

    for (int ddir = 0; ddir < 4; ++ddir) {
      if (ddir == 2) // No U-turn
        continue;

      Pos last_pos = curr_pos;
      Pos pos = curr_pos;
      int dist_add;
      if (ddir != 0) { // Turn left or right
        dist_add = 1000;
        pos.dir = (pos.dir + ddir) % 4;

      } else { // Go forward until next node
        pos = get_next_pos(pos);
        if (grid[pos.row][pos.col][pos.dir].value == '#')
          continue;

        dist_add = 0;
        int step = 1;
        do {
          dist_add += step;
          step = advance_pos(grid, pos, last_pos);
        } while (step > 0);

        if (step == -1) // Dead end
          continue;
      }

      Node &node = grid[pos.row][pos.col][pos.dir];
      if (node.seen)
        continue;

      if (node.distance == -1 || node.distance > curr_node.distance + dist_add) {
        node.distance = curr_node.distance + dist_add;
        border.push(pos);
        if (mark_parents)
          node.parents = {last_pos};
      } else if (mark_parents && node.distance == curr_node.distance + dist_add) {
        node.parents.push_back(last_pos);
      }
    }
  }

  return -1;
}

int count_tiles_shortest_path(Grid &grid, int start_row, int start_col, int end_row, int end_col) {
  get_shortest_path(grid, start_row, start_col, true);

  std::unordered_set<size_t> seen_pos;
  std::vector<Pos> path;
  int min_dist = 1000000000;
  for (int dir = 0; dir < 4; ++dir) {
    int dir_dist = grid[end_row][end_col][dir].distance;
    if (dir_dist < min_dist) {
      path = { Pos{end_row, end_col, dir} };
      min_dist = dir_dist;
    } else if (dir_dist == min_dist) {
      path.push_back(Pos{end_row, end_col, dir});
    }
  }

  while (!path.empty()) {
    Pos pos = path.back();
    path.pop_back();
    seen_pos.insert((pos.row << 16) + pos.col);

    for (Pos parent : grid[pos.row][pos.col][pos.dir].parents)
      path.push_back(parent);
  }

  return seen_pos.size();
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
  int start_row, start_col, end_row, end_col;
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
      index = input.find('E');
      if (index != std::string::npos) {
        end_row = row;
        end_col = index;
      }
      row++;
    }
    myfile.close();
  } else {
    std::cout << "Unable to open file" << std::endl;
    return 1;
  }

  if (first_half)
    ans = get_shortest_path(grid, start_row, start_col);
  else
    ans = count_tiles_shortest_path(grid, start_row, start_col, end_row, end_col);

  std::cout << "Answer : " << ans << std::endl;
  return 0;
}
