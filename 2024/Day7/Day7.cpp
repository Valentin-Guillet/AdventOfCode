
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

inline long concatenate(long x, long y) {
  // All factors are between 1 and 999
  if (y < 10)
    return 10 * x + y;
  if (y < 100)
    return 100 * x + y;
  return 1000 * x + y;
}

bool is_solvable(const Equation &eq, long value, size_t index, bool use_concat) {
  if (index == eq.factors.size())
    return value == eq.test_value;

  if (is_solvable(eq, value * eq.factors[index], index + 1, use_concat))
    return true;

  if (value >= eq.test_value)
    return false;

  return is_solvable(eq, value + eq.factors[index], index + 1, use_concat) ||
         (use_concat && is_solvable(eq, concatenate(value, eq.factors[index]),
                                    index + 1, use_concat));
}

bool is_solvable(const Equation &eq, bool use_concat) {
  return is_solvable(eq, eq.factors[0], 1, use_concat);
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
