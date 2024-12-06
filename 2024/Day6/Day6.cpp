
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct Pos {
  int row;
  int col;
  int drow;
  int dcol;
  bool operator==(const Pos &other) const = default;
};

struct pos_hash {
  size_t operator()(const Pos &pos) const {
    size_t hash = pos.row + 0x9e3779b9;
    hash = (hash << 6) + (hash >> 2) + pos.col + 0x9e3779b9;
    hash = (hash << 6) + (hash >> 2) + pos.drow + 0x9e3779b9;
    hash = (hash << 6) + (hash >> 2) + pos.dcol + 0x9e3779b9;
    return hash;
  }
};

using Grid = std::vector<std::string>;
using TargetMap = std::unordered_map<Pos, Pos, pos_hash>;
using SeenSet = std::unordered_set<Pos, pos_hash>;

inline void turn_right(Pos &pos) {
  int tmp_drow = pos.drow;
  pos.drow = pos.dcol;
  pos.dcol = -tmp_drow;
}

inline bool is_inbound(const Grid &grid, int row, int col) {
  return (0 <= row && row < (int)grid.size() && 0 <= col && col < (int)grid[0].size());
}

Pos get_target(const Grid &grid, TargetMap &target_map, const Pos &pos, bool add_intermediate) {
  if (target_map.contains(pos))
    return target_map.at(pos);

  std::vector<Pos> intermediate_pos = {pos};
  Pos target(pos);
  while (true) {
    int next_row = target.row + target.drow;
    int next_col = target.col + target.dcol;
    if (!is_inbound(grid, next_row, next_col)) {
      target.row = next_row;
      target.col = next_col;
      break;
    }

    if (grid[next_row][next_col] == '#')
      break;

    target.row = next_row;
    target.col = next_col;
    if (add_intermediate)
      intermediate_pos.push_back(target);
  }

  turn_right(target);
  for (const Pos &inter_pos : intermediate_pos)
    target_map[inter_pos] = target;
  return target;
}

Pos get_target(const Grid &grid, TargetMap &target_map, const Pos &pos, int obstacle_row, int obstacle_col) {
  Pos target = get_target(grid, target_map, pos, false);

  // Collision to the north
  if (pos.drow == -1 && obstacle_col == pos.col && target.row <= obstacle_row && obstacle_row < pos.row)
    return {obstacle_row + 1, obstacle_col, pos.dcol, -pos.drow};

  // Collision to the east
  if (pos.dcol == 1 && obstacle_row == pos.row && pos.col < obstacle_col && obstacle_col <= target.col)
    return {obstacle_row, obstacle_col - 1, pos.dcol, -pos.drow};

  // Collision to the south
  if (pos.drow == 1 && obstacle_col == pos.col && pos.row < obstacle_row && obstacle_row <= target.row)
    return {obstacle_row - 1, obstacle_col, pos.dcol, -pos.drow};

  // Collision to the west
  if (pos.dcol == -1 && obstacle_row == pos.row && target.col <= obstacle_col && obstacle_col < pos.col)
    return {obstacle_row, obstacle_col + 1, pos.dcol, -pos.drow};

  return target;
}

bool has_loop(const Grid &grid, TargetMap &target_map, Pos pos, const Pos &obstacle) {
  SeenSet seen;
  turn_right(pos);
  do {
    seen.insert(pos);
    pos = get_target(grid, target_map, pos, obstacle.row, obstacle.col);
  } while (is_inbound(grid, pos.row, pos.col) && !seen.contains(pos));

  return seen.contains(pos);
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

  Pos guard_pos;
  Grid grid;
  if (myfile.is_open()) {
    std::string input;
    int x = 0;
    while (getline(myfile, input)) {
      grid.push_back(input);
      size_t index = input.find('^');
      if (index != std::string::npos) {
        guard_pos = {x, (int)index, -1, 0};
        grid[x][index] = '.';
      }
      x++;
    }

    myfile.close();
  } else {
    std::cout << "Unable to open file" << std::endl;
    return 1;
  }

  TargetMap target_map;

  if (first_half) {
    while (is_inbound(grid, guard_pos.row, guard_pos.col))
      guard_pos = get_target(grid, target_map, guard_pos, true);

    SeenSet seen;
    for (const auto &[src, _] : target_map)
      seen.insert(Pos{src.row, src.col, 0, 0});
    ans = seen.size();

  } else {
    SeenSet seen_positions;
    while (true) {
      Pos next_pos(guard_pos.row + guard_pos.drow, guard_pos.col + guard_pos.dcol, 0, 0);
      if (!is_inbound(grid, next_pos.row, next_pos.col))
        break;

      if (grid[next_pos.row][next_pos.col] == '#') {
        turn_right(guard_pos);
        continue;
      }

      // Can only add an obstacle if it is not on the way to get to the curr pos
      if (!seen_positions.contains(next_pos))
        ans += has_loop(grid, target_map, guard_pos, next_pos);

      seen_positions.insert(next_pos);
      guard_pos.row = next_pos.row;
      guard_pos.col = next_pos.col;
    }
  }

  std::cout << "Answer : " << ans << std::endl;
  return 0;
}
