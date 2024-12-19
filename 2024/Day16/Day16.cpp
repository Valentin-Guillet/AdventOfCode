
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3

struct Pos {
  int row, col, dir;
  size_t hash() const { return (row << 20) + (col << 4) + dir; }

  void step() {
    if (dir == NORTH) row--;
    else if (dir == EAST) col++;
    else if (dir == SOUTH) row++;
    else if (dir == WEST) col--;
  }
};

struct Node {
  std::vector<Pos> parents;
  bool seen = false;
  int distance = -1;
};

using Grid = std::vector<std::string>;
using NodeMap = std::unordered_map<size_t, Node>;

int advance_pos(const Grid &grid, NodeMap &nodes, Pos &pos, Pos &last_pos, bool mark_parents) {
  // Consider exit as node
  if (grid[pos.row][pos.col] == 'E')
    return 0;

  bool next_found = false;
  Pos only_pos;
  int distance;
  for (int ddir = 0; ddir < 4; ++ddir) {
    if (ddir == 2)
      continue;

    Pos next_pos{pos.row, pos.col, (pos.dir + ddir) % 4};
    next_pos.step();

    if (grid[next_pos.row][next_pos.col] == '#')
      continue;

    if (next_found) // Found several exits: we're on a node
      return 0;
    next_found = true;

    only_pos = next_pos;
    distance = (ddir == 0 ? 1 : 1001);
  }

  if (!next_found) // Found no exit: dead end
    return -1;

  if (mark_parents)
    nodes[pos.hash()].parents.push_back(last_pos);
  last_pos = pos;
  pos = only_pos;
  return distance;
}

int get_shortest_path(const Grid &grid, NodeMap &nodes, int start_row, int start_col, bool mark_parents = false) {
  Pos init_pos{start_row, start_col, EAST};
  nodes[init_pos.hash()].distance = 0;

  auto cmp = [&nodes](Pos &left, Pos &right) {
    return nodes[left.hash()].distance > nodes[right.hash()].distance;
  };
  std::priority_queue<Pos, std::vector<Pos>, decltype(cmp)> border(cmp);
  border.push(init_pos);

  int found_dist = -1;
  while (!border.empty()) {
    Pos curr_pos = border.top();
    border.pop();

    Node &curr_node = nodes[curr_pos.hash()];
    if (!mark_parents && grid[curr_pos.row][curr_pos.col] == 'E')
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
        pos.step();
        if (grid[pos.row][pos.col] == '#')
          continue;

        Node &node = nodes[pos.hash()];
        if (node.seen)
          continue;

        dist_add = 0;
        int step = 1;
        do {
          dist_add += step;
          step = advance_pos(grid, nodes, pos, last_pos, mark_parents);
        } while (step > 0);

        if (step == -1) // Dead end
          continue;

        // Can't take a path between nodes twice
        Pos opposite_pos{last_pos.row, last_pos.col, (last_pos.dir + 2) % 4};
        nodes[opposite_pos.hash()].seen = true;
      }

      Node &node = nodes[pos.hash()];
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

int count_tiles_shortest_path(const Grid &grid, NodeMap &nodes,
                              int start_row, int start_col, int end_row, int end_col) {
  get_shortest_path(grid, nodes, start_row, start_col, true);

  std::unordered_set<size_t> seen_pos;
  std::vector<Pos> path;
  int min_dist = 1000000000;
  for (int dir = 0; dir < 4; ++dir) {
    Pos pos{end_row, end_col, dir};
    int dir_dist = nodes[pos.hash()].distance;
    if (dir_dist < min_dist) {
      path = {pos};
      min_dist = dir_dist;
    } else if (dir_dist == min_dist) {
      path.push_back(pos);
    }
  }

  while (!path.empty()) {
    Pos pos = path.back();
    path.pop_back();
    seen_pos.insert((pos.row << 16) + pos.col);

    for (Pos parent : nodes[pos.hash()].parents)
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
      grid.push_back(input);
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

  NodeMap nodes;
  if (first_half)
    ans = get_shortest_path(grid, nodes, start_row, start_col);
  else
    ans = count_tiles_shortest_path(grid, nodes,
                                    start_row, start_col,
                                    end_row, end_col);

  std::cout << "Answer : " << ans << std::endl;
  return 0;
}
