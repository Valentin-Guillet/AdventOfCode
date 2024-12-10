
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct Pos {
  int row;
  int col;

  bool operator==(const Pos &other) const {
    return (row == other.row && col == other.col);
  }
};

using Map = std::vector<std::vector<int>>;

int get_trailhead_score(const Map &map, Pos trailhead) {
  std::vector<Pos> curr_pos = {trailhead};

  for (int height = 1; height <= 9; ++height) {
    std::vector<Pos> next_pos;
    while (!curr_pos.empty()) {
      Pos pos = curr_pos.back();
      curr_pos.pop_back();

      // North
      Pos neigh = Pos{pos.row - 1, pos.col};
      if (neigh.row >= 0 && map.at(neigh.row).at(neigh.col) == height &&
          std::find(next_pos.begin(), next_pos.end(), neigh) == next_pos.end())
        next_pos.push_back(neigh);

      // East
      neigh = Pos{pos.row, pos.col + 1};
      if (neigh.col < (int)map[0].size() && map.at(neigh.row).at(neigh.col) == height &&
          std::find(next_pos.begin(), next_pos.end(), neigh) == next_pos.end())
        next_pos.push_back(neigh);

      // South
      neigh = Pos{pos.row + 1, pos.col};
      if (neigh.row < (int)map.size() && map.at(neigh.row).at(neigh.col) == height &&
          std::find(next_pos.begin(), next_pos.end(), neigh) == next_pos.end())
        next_pos.push_back(neigh);

      // West
      neigh = Pos{pos.row, pos.col - 1};
      if (neigh.col >= 0 && map.at(neigh.row).at(neigh.col) == height &&
          std::find(next_pos.begin(), next_pos.end(), neigh) == next_pos.end())
        next_pos.push_back(neigh);
    }
    curr_pos = next_pos;
  }

  return curr_pos.size();
}

void compute_scores(const Map &map, Map &scores, int height) {
  for (int row = 0; row < (int)map.size(); ++row) {
    for (int col = 0; col < (int)map[0].size(); ++col) {
      if (map.at(row).at(col) != height)
        continue;

      if (height == 9) {
        scores[row][col] = 1;
        continue;
      }

      int score = 0;
      // North
      if (row > 0 && map.at(row - 1).at(col) == height + 1)
        score += scores[row - 1][col];

      // East
      if (col < (int)map[0].size() - 1 && map.at(row).at(col + 1) == height + 1)
        score += scores[row][col + 1];

      // South
      if (row < (int)map.size() - 1 && map.at(row + 1).at(col) == height + 1)
        score += scores[row + 1][col];

      // West
      if (col > 0 && map.at(row).at(col - 1) == height + 1)
        score += scores[row][col - 1];
      scores[row][col] = score;
    }
  }
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

  Map map;
  if (myfile.is_open()) {
    std::string input;
    while (getline(myfile, input)) {
      std::vector<int> line;
      for (char c : input)
        line.push_back(c - '0');
      map.push_back(line);
    }
    myfile.close();
  } else {
    std::cout << "Unable to open file" << std::endl;
    return 1;
  }

  if (first_half) {
    for (int row = 0; row < (int)map.size(); ++row) {
      for (int col = 0; col < (int)map[0].size(); ++col) {
        if (map[row][col] == 0) {
          ans += get_trailhead_score(map, Pos{row, col});
        }
      }
    }
  } else {
    Map scores(map.size(), std::vector(map[0].size(), 0));
    for (int height = 9; height >= 0; --height)
      compute_scores(map, scores, height);

    for (int row = 0; row < (int)map.size(); ++row) {
      for (int col = 0; col < (int)map[0].size(); ++col) {
        if (map[row][col] == 0)
          ans += scores[row][col];
      }
    }
  }

  std::cout << "Answer : " << ans << std::endl;
  return 0;
}
