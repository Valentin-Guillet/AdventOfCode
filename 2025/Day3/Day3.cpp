
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using Bank = std::string;

long get_largest_joltage(const Bank &bank, int nb_batteries) {
  std::vector<size_t> indices;
  size_t min_limit = 0;
  for (int k = 0; k < nb_batteries; ++k) {
    size_t max_index = min_limit;
    for (size_t i = min_limit + 1; i < bank.size() - (nb_batteries - 1 - k); ++i) {
      if (bank[i] > bank[max_index]) {
        max_index = i;
        if (bank[i] == '9')
          break;
      }
    }
    indices.emplace_back(max_index);
    min_limit = max_index + 1;
  }

  long ans = 0;
  for (size_t index : indices)
    ans = 10 * ans + (bank[index] - '0');
  return ans;
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

  std::vector<Bank> battery_banks;
  if (myfile.is_open()) {
    std::string input;
    while (getline(myfile, input))
      battery_banks.emplace_back(input);
    myfile.close();
  } else {
    std::cout << "Unable to open file" << std::endl;
    return 1;
  }

  int nb_batteries = (first_half ? 2 : 12);
  for (Bank &bank : battery_banks)
    ans += get_largest_joltage(bank, nb_batteries);

  std::cout << "Answer : " << ans << std::endl;
  return 0;
}
