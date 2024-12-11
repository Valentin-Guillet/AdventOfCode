
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using Memory = std::unordered_map<long, std::vector<long>>;

long blink_count(long stone, int depth, Memory &memory) {
  std::vector<long> &stone_counts = memory[stone];
  if (depth < (int)stone_counts.size() && stone_counts[depth] != -1)
    return stone_counts[depth];

  if (stone_counts.empty())
    stone_counts.push_back(1);
  while ((int)stone_counts.size() <= depth)
    stone_counts.push_back(-1);

  if (depth == 0)
    return 1;

  if (stone == 0) {
    long nb_stones = blink_count(1, depth - 1, memory);
    for (int i = 1; i <= depth; ++i)
      stone_counts[i] = memory[1][i - 1];
    return nb_stones;
  }

  std::string stone_str = std::to_string(stone);
  size_t n = stone_str.size();
  if (n % 2 == 0) {
    long left = std::stol(stone_str.substr(0, n / 2));
    long right = std::stol(stone_str.substr(n / 2));
    long nb_stones = blink_count(left, depth - 1, memory) + blink_count(right, depth - 1, memory);
    for (int i = 1; i <= depth; ++i)
      stone_counts[i] = memory[left][i - 1] + memory[right][i - 1];
    return nb_stones;
  }

  long next = stone * 2024;
  long nb_stones = blink_count(next, depth - 1, memory);
  for (int i = 1; i <= depth; ++i)
    stone_counts[i] = memory[next][i - 1];
  return nb_stones;
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

  std::vector<long> stones;
  if (myfile.is_open()) {
    std::string input;
    getline(myfile, input);
    std::stringstream input_stream(input);
    long stone;
    while (input_stream >> stone)
      stones.push_back(stone);
    myfile.close();
  } else {
    std::cout << "Unable to open file" << std::endl;
    return 1;
  }

  Memory memory;
  int nb_blinks = (first_half ? 25 : 75);
  for (long stone : stones)
    ans += blink_count(stone, nb_blinks, memory);

  std::cout << "Answer : " << ans << std::endl;
  return 0;
}
