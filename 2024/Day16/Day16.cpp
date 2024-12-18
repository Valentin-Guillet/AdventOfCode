
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3

struct Pos {
  int row, col, dir;
};

std::ostream& operator<<(std::ostream &os, const Pos &pos) {
  os << '(' << pos.row << ", " << pos.col << ", ";
  if (pos.dir == NORTH) os << 'N';
  if (pos.dir == EAST) os << 'E';
  if (pos.dir == SOUTH) os << 'S';
  if (pos.dir == WEST) os << 'W';
  os << ')';
  return os;
}

struct Node {
  char value;
  bool seen = false;
  int distance = -1;
};

using Grid = std::vector<std::vector<std::vector<Node>>>;

Pos get_shortest_dist(const Grid &grid, std::vector<Pos> &border) {
  std::vector<Pos>::iterator shortest_dist_it;
  int min_dist = 1000000;
  for (auto it = border.begin(); it != border.end(); ++it) {
    int dist = grid.at(it->row).at(it->col).at(it->dir).distance;
    if (dist < min_dist) {
      min_dist = dist;
      shortest_dist_it = it;
    }
  }
  Pos pos = *shortest_dist_it;
  border.erase(shortest_dist_it);
  return pos;
}

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
    if (node.value == '#')
      continue;

    if (next_found) // Found several exits: we're on a node
      return 0;
    next_found = true;

    only_pos = next_pos;
    distance = (ddir == 0 ? 1 : 1001);
  }

  if (!next_found) // Found no exit: dead end
    return -1;

  pos = only_pos;
  return distance;
}

int get_shortest_path(Grid &grid, int start_row, int start_col) {
  grid[start_row][start_col][EAST].distance = 0;

  std::vector<Pos> border = { Pos{start_row, start_col, EAST} };
  while (!border.empty()) {
    Pos curr_pos = get_shortest_dist(grid, border);

    Node &curr_node = grid[curr_pos.row][curr_pos.col][curr_pos.dir];
    if (curr_node.value == 'E')
      return curr_node.distance;

    for (int ddir = 0; ddir < 4; ++ddir) {
      if (ddir == 2) // No half-turn
        continue;

      Pos pos{curr_pos.row, curr_pos.col, (curr_pos.dir + ddir) % 4};
      std::cout << "Pos = " << pos << "\n";
      Node &node = grid[pos.row][pos.col][pos.dir];
      if (node.seen) {
        std::cout << "  => Node already seen !\n";
        continue;
      }


      Pos next_pos = get_next_pos(pos);
      if (grid[next_pos.row][next_pos.col][next_pos.dir].value == '#') {
        std::cout << "  => Next pos " << next_pos << " is a wall!\n";
        continue;
      }
      std::cout << "  Marking pos " << pos << " as seen\n";
      pos = next_pos;
      node.seen = true;

      int dist_add = (ddir == 0 ? 0 : 1000);
      int step = 1;
      do {
        std::cout << "    -> Advance to " << pos << " with added dist " << dist_add << " and step " << step << "\n";
        dist_add += step;
        step = advance_pos(grid, pos);
      } while (step > 0);

      if (step == -1) { // Dead end
        std::cout << "  -> Dead end!\n";
        continue;
      }

      Node &next_node = grid[pos.row][pos.col][pos.dir];
      if (next_node.seen && next_node.distance <= curr_node.distance + dist_add)
        std::cout << "YOOOOOOOOOOOOOOOOOOOOOOO\n";

      if (next_node.distance == -1) {
        next_node.distance = curr_node.distance + dist_add;
        std::cout << "  Adding " << pos << " with dist " << next_node.distance << " to the border\n";
        border.push_back(pos);
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
  int start_row, start_col;
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

  ans = get_shortest_path(grid, start_row, start_col);

  std::cout << "Answer : " << ans << std::endl;
  return 0;
}
