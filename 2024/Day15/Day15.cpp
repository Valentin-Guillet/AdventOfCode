
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

using Warehouse = std::vector<std::string>;
using Pos = std::pair<int, int>;

std::string parse_line(const std::string &line, bool widen) {
  if (!widen)
    return line;

  std::string wide_line;
  for (char c : line) {
    if (c == '#')
      wide_line += "##";
    else if (c == 'O')
      wide_line += "[]";
    else if (c == '@')
      wide_line += "@.";
    else if (c == '.')
      wide_line += "..";
  }

  return wide_line;
}

void move_robot(Warehouse &warehouse, int &row, int &col, char dir) {
  int dx = 0;
  int dy = 0;
  if (dir == 'v') dx++;
  else if (dir == '>') dy++;
  else if (dir == '<') dy--;
  else if (dir == '^') dx--;

  int boxes = 0;
  while (warehouse[row + (boxes + 1) * dx][col + (boxes + 1) * dy] == 'O')
    boxes++;

  if (warehouse[row + (boxes + 1) * dx][col + (boxes + 1) * dy] == '#')
    return;

  warehouse[row][col] = '.';
  warehouse[row + dx][col + dy] = '@';
  if (boxes > 0)
    warehouse[row + (boxes + 1) * dx][col + (boxes + 1) * dy] = 'O';

  row += dx;
  col += dy;
}

void move_wide_robot(Warehouse &warehouse, int &row, int &col, char dir) {
  int dx = 0;
  int dy = 0;
  if (dir == 'v') dx++;
  else if (dir == '>') dy++;
  else if (dir == '<') dy--;
  else if (dir == '^') dx--;

  std::vector<Pos> to_move = {{row, col}};
  for (size_t index = 0; index < to_move.size(); ++index) {
    int item_row = to_move[index].first + dx;
    int item_col = to_move[index].second + dy;
    char obj = warehouse[item_row][item_col];

    // Wall: cancel the move
    if (obj == '#')
      return;

    // Free space, we can move the item
    if (obj == '.')
      continue;

    // Move box horizontally
    if (dy != 0) {
      to_move.push_back({item_row, item_col});
      to_move.push_back({item_row, item_col + dy});
      index++;

    } else { // Vertical
      if (std::find(to_move.begin(), to_move.end(), Pos{item_row, item_col}) == to_move.end())
        to_move.push_back({item_row, item_col});
      if (obj == '[' && std::find(to_move.begin(), to_move.end(), Pos{item_row, item_col + 1}) == to_move.end())
        to_move.push_back({item_row, item_col + 1});
      if (obj == ']' && std::find(to_move.begin(), to_move.end(), Pos{item_row, item_col - 1}) == to_move.end())
        to_move.push_back({item_row, item_col - 1});
    }
  }

  while (!to_move.empty()) {
    int item_row = to_move.back().first;
    int item_col = to_move.back().second;
    warehouse[item_row + dx][item_col + dy] = warehouse[item_row][item_col];
    warehouse[item_row][item_col] = '.';
    to_move.pop_back();
  }

  row += dx;
  col += dy;
}

int compute_gps(const Warehouse &warehouse) {
  int gps = 0;
  for (size_t row = 0; row < warehouse.size(); ++row) {
    for (size_t col = 0; col < warehouse[0].size(); ++col) {
      if (warehouse[row][col] == 'O' || warehouse[row][col] == '[')
        gps += 100 * row + col;
    }
  }
  return gps;
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

  Warehouse warehouse;
  std::string movements;
  int robot_row;
  int robot_col;
  if (myfile.is_open()) {
    std::string input;
    bool parse_warehouse = true;
    int x = 0;
    while (getline(myfile, input)) {
      if (input.empty()) {
        parse_warehouse = false;
      } else if (parse_warehouse) {
        std::string line = parse_line(input, !first_half);
        warehouse.push_back(line);
        size_t index = line.find('@');
        if (index != std::string::npos) {
          robot_row = x;
          robot_col = index;
        }
      } else {
        movements += input;
      }
      x++;
    }
    myfile.close();
  } else {
    std::cout << "Unable to open file" << std::endl;
    return 1;
  }

  if (first_half) {
    for (char dir : movements)
      move_robot(warehouse, robot_row, robot_col, dir);
  } else {
    for (char dir : movements)
      move_wide_robot(warehouse, robot_row, robot_col, dir);
  }

  ans = compute_gps(warehouse);

  std::cout << "Answer : " << ans << std::endl;
  return 0;
}
