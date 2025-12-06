
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

using Range = std::pair<long, long>;

void add_range(std::vector<Range> &ranges, Range new_range) {
  for (int i = (int)ranges.size() - 1; i >= 0; i--) {
    Range &range = ranges[i];
    if (range.first > new_range.second || range.second < new_range.first)
      continue;

    new_range.first = std::min(range.first, new_range.first);
    new_range.second = std::max(range.second, new_range.second);

    if (i < (int)ranges.size() - 1)
      std::swap(range, ranges.back());
    ranges.pop_back();
  }
  ranges.push_back(new_range);
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

  std::vector<Range> ranges;
  std::vector<long> ingredients;
  if (myfile.is_open()) {
    std::string input;
    while (getline(myfile, input)) {
      if (input.empty())
        break;
      size_t ind = input.find('-');
      long low = std::stol(input.substr(0, ind));
      long high = std::stol(input.substr(ind + 1));
      add_range(ranges, Range(low, high));
    }
    while (getline(myfile, input))
      ingredients.emplace_back(std::stol(input));

    myfile.close();
  } else {
    std::cout << "Unable to open file" << std::endl;
    return 1;
  }

  if (first_half) {
    for (long ingredient : ingredients) {
      for (Range &range : ranges) {
        if (range.first <= ingredient && ingredient <= range.second) {
          ans++;
          break;
        }
      }
    }
  } else {
    for (Range &range : ranges)
      ans += range.second - range.first + 1;
  }

  std::cout << "Answer : " << ans << std::endl;
  return 0;
}
