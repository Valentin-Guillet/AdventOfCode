
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

enum Dir {
  NORTH = (1 << 0),
  EAST = (1 << 1),
  SOUTH = (1 << 2),
  WEST = (1 << 3),
};

void step(int &row, int &col, Dir dir) {
  if (dir == Dir::NORTH) row--;
  else if (dir == Dir::EAST) col++;
  else if (dir == Dir::SOUTH) row++;
  else col--; // Dir::WEST
}

struct Plot;
using Garden = std::vector<std::vector<Plot>>;

struct Plot {
  char type;
  int row;
  int col;
  unsigned short links = 0;
  bool seen = false;

  void link(Garden &garden) {
    if (row > 0 && garden[row - 1][col].type == type) {
      links |= Dir::NORTH;
      garden[row - 1][col].links |= Dir::SOUTH;
    }

    if (col > 0 && garden[row][col - 1].type == type) {
      links |= Dir::WEST;
      garden[row][col - 1].links |= Dir::EAST;
    }
  }

  bool should_count_side(const Garden &garden, Dir dir, bool discount) const {
    // Same type of plant on next plot: don't add a fence
    if (links & dir)
      return false;

    // If no discount, every fence counts
    if (!discount)
      return true;

    // Fences are discounted, so we count each side only once by only adding
    // the fence on a predifined extremity of the side, i.e. for fences on the
    // NORTH side, we only add the one at the extreme EAST
    Dir ortho_dir = (dir == Dir::WEST ? Dir::NORTH : Dir(dir << 1));

    // If the plot is already at the extremity of the side, count the fence
    if (!(links & ortho_dir))
      return true;

    // Else, look if the next plot has a fence on the same size
    int next_row(row), next_col(col);
    step(next_row, next_col, ortho_dir);
    return (garden[next_row][next_col].links & dir);
  }

  void setup_fences(Garden &garden, int &area, int &sides, bool discount) {
    seen = true;
    area++;

    for (int i = 0; i < 4; ++i) {
      Dir dir = Dir(1 << i);
      if (should_count_side(garden, dir, discount))
        sides++;

      int next_row(row), next_col(col);
      step(next_row, next_col, dir);
      if ((links & dir) && !garden[next_row][next_col].seen)
        garden[next_row][next_col].setup_fences(garden, area, sides, discount);
    }
  }
};

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

  Garden garden;
  if (myfile.is_open()) {
    std::string input;
    int row = 0;
    while (getline(myfile, input)) {
      std::vector<Plot> plots;
      for (int col = 0; col < (int)input.size(); ++col)
        plots.push_back(Plot{input[col], row, col});
      garden.push_back(plots);
      row++;
    }
    myfile.close();
  } else {
    std::cout << "Unable to open file" << std::endl;
    return 1;
  }

  for (size_t row = 0; row < garden.size(); ++row) {
    for (size_t col = 0; col < garden[0].size(); ++col)
      garden[row][col].link(garden);
  }

  for (size_t row = 0; row < garden.size(); ++row) {
    for (size_t col = 0; col < garden[0].size(); ++col) {
      if (!garden[row][col].seen) {
        int area = 0;
        int sides = 0;
        garden[row][col].setup_fences(garden, area, sides, !first_half);
        ans += area * sides;
      }
    }
  }


  std::cout << "Answer : " << ans << std::endl;
  return 0;
}
