
#include <fstream>
#include <iostream>
#include <regex>
#include <string>

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

  std::string memory;
  if (myfile.is_open()) {
    std::string input;
    while (getline(myfile, input))
      memory += input;
    myfile.close();
  } else {
    std::cout << "Unable to open file" << std::endl;
    return 1;
  }

  std::string pattern = R"(mul\((\d+),(\d+)\))";
  if (!first_half)
    pattern += R"(|do\(\)|don't\(\))";
  std::regex mul_regex(pattern);
  auto match_beg = std::sregex_iterator(memory.begin(), memory.end(), mul_regex);
  auto match_end = std::sregex_iterator();

  bool enabled = true;
  for (std::sregex_iterator i = match_beg; i != match_end; ++i) {
    std::smatch match = *i;
    if (match[0] == "do()")
      enabled = true;
    else if (match[0] == "don't()")
      enabled = false;
    else if (enabled)
      ans += std::stoi(match[1]) * std::stoi(match[2]);
  }

  std::cout << "Answer : " << ans << std::endl;
  return 0;
}
