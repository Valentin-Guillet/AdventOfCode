
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Present {
  size_t size;

  Present(const std::vector<std::string> &lines) {
    size = 0;
    for (size_t i = 1; i < lines.size(); ++i) {
      for (char c : lines[i])
        size += (c == '#');
    }
  }
};

struct Region {
  size_t width;
  size_t height;
  std::vector<int> present_counts;

  Region(const std::string &input) {
    size_t index1 = input.find('x');
    size_t index2 = input.find(':');
    width = std::stoi(input.substr(0, index1));
    height = std::stoi(input.substr(index1 + 1, index2 - index1 - 1));

    std::istringstream iss(input.substr(index2 + 2));
    std::string number;
    while (getline(iss, number, ' '))
      present_counts.emplace_back(std::stoi(number));
  }

  bool is_solvable(const std::vector<Present> &presents) const {
    size_t total_cell_count = 0;
    size_t total_presents = 0;
    for (size_t i = 0; i < presents.size(); ++i) {
      total_cell_count += present_counts[i] * presents[i].size;
      total_presents += present_counts[i];
    }

    if (total_cell_count > width * height)
      return false;

    if ((height / 3) * (width / 3) >= total_presents)
      return true;

    // Never reached, input is trivial
    return false;
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

  std::vector<Present> presents;
  std::vector<Region> regions;
  if (myfile.is_open()) {
    std::string input;
    std::vector<std::string> lines;
    while (getline(myfile, input)) {
      if (input.contains('x'))
        break;

      if (!input.empty()) {
        lines.emplace_back(std::move(input));
      } else {
        presents.emplace_back(std::move(lines));
        lines.clear();
      }
    }

    do {
      regions.emplace_back(input);
    } while (getline(myfile, input));
    myfile.close();
  } else {
    std::cout << "Unable to open file" << std::endl;
    return 1;
  }

  for (const Region &region : regions)
    ans += region.is_solvable(presents);

  if (!first_half)
    std::cout << "Merry Christmas !" << std::endl;

  std::cout << "Answer : " << ans << std::endl;
  return 0;
}
