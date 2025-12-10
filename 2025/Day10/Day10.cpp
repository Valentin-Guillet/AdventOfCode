
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using State = int;

std::vector<int> parse_ints(const std::string &input) {
  std::istringstream input_stream(input);
  std::vector<int> results;
  std::string int_str;
  while (getline(input_stream, int_str, ','))
    results.emplace_back(std::stoi(int_str));
  return results;
}

struct Machine {
  State target;
  std::vector<State> buttons;
  std::vector<int> joltages;

  Machine(const std::string &input) {
    size_t index = 1;
    while (input[index] != ']') {
      if (input[index] == '#')
        target |= (1 << (index - 1));
      index++;
    }

    index += 2;
    while (input[index] == '(') {
      size_t end_index = input.find(')', index + 1);
      State button = 0;
      for (int wiring :
           parse_ints(input.substr(index + 1, end_index - index - 1))) {
        button |= (1 << wiring);
      }
      buttons.emplace_back(button);
      index = end_index + 2;
    }

    joltages = parse_ints(input.substr(index + 1, input.size() - index - 2));
  }

  int get_fewest_presses() const {
    for (size_t nb_press = 1; nb_press < buttons.size() + 1; ++nb_press) {
      if (is_sequence_valid(0, 0, nb_press))
        return nb_press;
    }
    return 0;
  }

  bool is_sequence_valid(State state, int next_press, int nb_press) const {
    if (nb_press == 0)
      return state == target;

    for (size_t i = next_press; i < buttons.size(); ++i) {
      if (is_sequence_valid(state ^ buttons[i], i + 1, nb_press - 1))
        return true;
    }
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

  std::vector<Machine> machines;
  if (myfile.is_open()) {
    std::string input;
    while (getline(myfile, input))
      machines.emplace_back(std::move(input));
    myfile.close();
  } else {
    std::cout << "Unable to open file" << std::endl;
    return 1;
  }

  for (const Machine &machine : machines)
    ans += machine.get_fewest_presses();

  std::cout << "Answer : " << ans << std::endl;
  return 0;
}
