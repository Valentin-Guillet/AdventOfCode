
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

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

  std::vector<int> rotations;
  if (myfile.is_open()) {
    std::string input;
    while (getline(myfile, input)) {
      int sign = input[0] == 'R' ? 1 : -1;
      int value = std::stoi(input.substr(1));
      rotations.push_back(sign * value);
    }
    myfile.close();
  } else {
    std::cout << "Unable to open file" << std::endl;
    return 1;
  }

  int value = 50;
  for (int rotation : rotations) {
    if (first_half) {
      value = (value + rotation + 1000) % 100;
      if (value == 0)
        ans++;
    } else {
      if (value == 0 && rotation < 0)
        ans--;
      value += rotation;
      while (value < 0) {
        value += 100;
        ans++;
      }
      if (value == 0)
        ans++;
      while (value > 99) {
        value -= 100;
        ans++;
      }
    }
  }

  std::cout << "Answer : " << ans << std::endl;
  return 0;
}
