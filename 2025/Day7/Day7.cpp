
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using Grid = std::vector<std::string>;
using Cache = std::unordered_map<size_t, long>;

int count_beam_splitting(const Grid &grid) {
  int ans = 0;
  std::unordered_set<size_t> beams{grid[0].find('S')};
  for (size_t row = 1; row < grid.size(); ++row) {
    std::unordered_set<size_t> next_beams;
    for (size_t beam_col : beams) {
      // No splitter
      if (grid[row][beam_col] != '^') {
        next_beams.insert(beam_col);
        continue;
      }

      // Split
      ans++;
      next_beams.insert(beam_col - 1); // Input can't overflow
      next_beams.insert(beam_col + 1);
    }
    beams = next_beams;
  }
  return ans;
}

long _count_timeline_rec(const Grid &grid, size_t row, size_t col,
                         Cache &cache) {
  while (row < grid.size() && grid[row][col] == '.')
    row++;

  // Exit
  if (row >= grid.size())
    return 1;

  size_t hash = (row << 16) | col;
  if (cache.contains(hash))
    return cache[hash];

  long ans = 0;
  ans += _count_timeline_rec(grid, row, col - 1, cache);
  ans += _count_timeline_rec(grid, row, col + 1, cache);
  cache[hash] = ans;
  return ans;
}

long count_timelines(const Grid &grid) {
  Cache cache;
  return _count_timeline_rec(grid, 0, grid[0].find('S'), cache);
}

int main(int argc, char *argv[]) {
  bool first_half = true;
  long ans = 0;

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
      grid.emplace_back(std::move(input));
    myfile.close();
  } else {
    std::cout << "Unable to open file" << std::endl;
    return 1;
  }

  if (first_half)
    ans = count_beam_splitting(grid);
  else
    ans = count_timelines(grid);

  std::cout << "Answer : " << ans << std::endl;
  return 0;
}
