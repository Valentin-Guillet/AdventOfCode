
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

struct Pos {
  int row;
  int col;
};

using Cache = std::unordered_map<std::string, long>;

using Pattern = std::unordered_map<char, Pos>;
const Pattern numeric_keypad = {
  {'7', {0, 0}}, {'8', {0, 1}}, {'9', {0, 2}}, {'4', {1, 0}}, {'5', {1, 1}}, {'6', {1, 2}},
  {'1', {2, 0}}, {'2', {2, 1}}, {'3', {2, 2}}, {'0', {3, 1}}, {'A', {3, 2}}
};
const Pattern directional_keypad = {
  {'^', {0, 1}}, {'A', {0, 2}}, {'<', {1, 0}}, {'v', {1, 1}}, {'>', {1, 2}}
};

long get_min_sequence_length(const std::string &sequence, char start_key, Cache &cache, int max_depth, int depth = 0) {
  const std::string arg_hash = sequence + std::to_string(depth);
  if (cache.contains(arg_hash))
    return cache[arg_hash];

  if (depth == max_depth + 1)
    return sequence.size();

  const Pattern &pattern = (depth == 0 ? numeric_keypad : directional_keypad);

  long total_size = 0;
  Pos pos = pattern.at(start_key);
  for (char key : sequence) {
    const Pos &target = pattern.at(key);
    std::string vertical_string = std::string(std::abs(target.row - pos.row), (target.row > pos.row ? 'v' : '^'));
    std::string horizontal_string = std::string(std::abs(target.col - pos.col), (target.col > pos.col ? '>' : '<'));

    if (vertical_string.empty()) {
      total_size += get_min_sequence_length(horizontal_string + "A", 'A', cache, max_depth, depth + 1);

    } else if (horizontal_string.empty()) {
      total_size += get_min_sequence_length(vertical_string + "A", 'A', cache, max_depth, depth + 1);

    // Going horizontal first would go over a hole
    } else if ((depth == 0 && pos.row == 3 && target.col == 0) || (depth > 0 && pos.row == 0 && target.col == 0)) {
      total_size += get_min_sequence_length(vertical_string + horizontal_string + "A", 'A', cache, max_depth, depth + 1);

    // Going vertical first would go over a hole
    } else if ((depth == 0 && pos.col == 0 && target.row == 3) || (depth > 0 && pos.col == 0 && target.row == 0)) {
      total_size += get_min_sequence_length(horizontal_string + vertical_string + "A", 'A', cache, max_depth, depth + 1);

    } else {
      long v_h = get_min_sequence_length(vertical_string + horizontal_string + "A", 'A', cache, max_depth, depth + 1);
      long h_v = get_min_sequence_length(horizontal_string + vertical_string + "A", 'A', cache, max_depth, depth + 1);
      total_size += std::min(v_h, h_v);
    }

    pos = target;
  }

  cache[arg_hash] = total_size;
  return total_size;
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

  std::vector<std::string> codes;
  if (myfile.is_open()) {
    std::string input;
    while (getline(myfile, input))
      codes.emplace_back(input);
    myfile.close();
  } else {
    std::cout << "Unable to open file" << std::endl;
    return 1;
  }

  int max_depth = (first_half ? 2 : 25);
  for (const std::string &code : codes) {
    Cache cache;
    char prev_key = 'A';
    long sequence_length = 0;
    for (char key : code) {
      sequence_length += get_min_sequence_length(std::string(1, key), prev_key, cache, max_depth);
      prev_key = key;
    }
    ans += std::stoi(code.substr(0, 3)) * sequence_length;
  }

  std::cout << "Answer : " << ans << std::endl;
  return 0;
}
