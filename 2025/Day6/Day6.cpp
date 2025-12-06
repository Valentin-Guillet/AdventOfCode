
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

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

  std::vector<std::string> lines;
  if (myfile.is_open()) {
    std::string input;
    while (getline(myfile, input))
      lines.emplace_back(std::move(input));
    myfile.close();
  } else {
    std::cout << "Unable to open file" << std::endl;
    return 1;
  }

  const size_t line_size = lines[0].size();
  const size_t nb_lines = lines.size() - 1;
  size_t start = 0;
  size_t end = 0;
  while (end != line_size) {
    end = lines[nb_lines].find_first_of("+*", start + 1) - 1;
    if (end == std::string::npos - 1)
      end = line_size;

    bool is_mult = lines[nb_lines][start] == '*';
    std::vector<int> operands;
    if (first_half) {
      for (size_t i = 0; i < nb_lines; ++i) {
        operands.emplace_back(std::stoi(lines[i].substr(start, end - start + 1)));
      }
    } else {
      for (size_t j = start; j < end; ++j) {
        std::string operand_str = "";
        for (size_t i = 0; i < nb_lines; ++i)
          operand_str += lines[i][j];

        operands.emplace_back(std::stoi(operand_str));
      }
    }

    long problem_ans = (is_mult ? 1 : 0);
    for (int operand : operands) {
      if (is_mult)
        problem_ans *= operand;
      else
        problem_ans += operand;
    }
    ans += problem_ans;
    start = end + 1;
  }

  std::cout << "Answer : " << ans << std::endl;
  return 0;
}
