
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct Machine {
  int A_x;
  int A_y;
  int B_x;
  int B_y;
  long prize_x;
  long prize_y;

  Machine(std::ifstream &file, bool far_prize) {
    std::string input;
    getline(file, input);
    A_x = std::stoi(input.substr(12, 2));
    A_y = std::stoi(input.substr(18, 2));

    getline(file, input);
    B_x = std::stoi(input.substr(12, 2));
    B_y = std::stoi(input.substr(18, 2));

    getline(file, input);
    size_t index = input.find(',');
    prize_x = std::stoi(input.substr(9, index - 8));
    index = input.find('=', index);
    prize_y = std::stoi(input.substr(index+1));

    if (far_prize) {
      prize_x += 10000000000000;
      prize_y += 10000000000000;
    }
  }

  long get_cost() const {
    // 2x2 matrix inverse
    long det = A_x * B_y - B_x * A_y;
    long coeff_A = (B_y * prize_x - B_x * prize_y);
    long coeff_B = (A_x * prize_y - A_y * prize_x);

    if (coeff_A % det != 0 || coeff_B % det != 0)
      return 0;
    return (3 * coeff_A + coeff_B) / det;
  }
};

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

  std::vector<Machine> machines;
  if (myfile.is_open()) {
    std::string input;
    do {
      machines.push_back(Machine(myfile, !first_half));
    } while (getline(myfile, input));
    myfile.close();
  } else {
    std::cout << "Unable to open file" << std::endl;
    return 1;
  }

  for (const Machine &machine : machines)
    ans += machine.get_cost();

  std::cout << "Answer : " << ans << std::endl;
  return 0;
}
