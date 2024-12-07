
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Equation {
  long test_value;
  std::vector<long> factors;

  Equation(const std::string &input) {
    size_t index = input.find(':');
    test_value = std::stol(input.substr(0, index));

    std::stringstream sinput(input.substr(index + 1));
    long factor;
    while (sinput >> factor)
      factors.push_back(factor);
  }
};

inline long get_tenth_power(long n) {
  // All factors are between 1 and 999
  if (n < 10) return 10;
  if (n < 100) return 100;
  return 1000;
}

bool is_solvable(const Equation &eq, long value, int index, bool use_concat) {
  long factor = eq.factors.at(index);
  if (index == 0)
    return value == factor;

  // Addition
  if (value >= factor && is_solvable(eq, value - factor, index - 1, use_concat))
    return true;

  // Multiplication
  if (value % factor == 0 && is_solvable(eq, value / factor, index - 1, use_concat))
    return true;

  // Concatenation
  if (!use_concat)
    return false;

  long n = get_tenth_power(factor);
  return (value % n == factor && is_solvable(eq, value / n, index - 1, use_concat));
}

bool is_solvable(const Equation &eq, bool use_concat) {
  return is_solvable(eq, eq.test_value, eq.factors.size() - 1, use_concat);
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

  std::vector<Equation> equations;
  if (myfile.is_open()) {
    std::string input;
    while (getline(myfile, input))
      equations.push_back(Equation(input));

    myfile.close();
  } else {
    std::cout << "Unable to open file" << std::endl;
    return 1;
  }

  for (const Equation &eq : equations) {
    if (is_solvable(eq, !first_half))
      ans += eq.test_value;
  }

  std::cout << "Answer : " << ans << std::endl;
  return 0;
}
