
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
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

  std::vector<int> list1, list2;
  if (myfile.is_open()) {
    std::string input;
    while (getline(myfile, input)) {
      std::stringstream sstream(input);
      int val1, val2;
      sstream >> val1 >> val2;
      list1.push_back(val1);
      list2.push_back(val2);
    }
    myfile.close();
  } else {
    std::cout << "Unable to open file" << std::endl;
    return 1;
  }

  if (first_half) {
    std::sort(list1.begin(), list1.end());
    std::sort(list2.begin(), list2.end());

    for (size_t i = 0; i < list1.size(); ++i)
      ans += std::abs(list1[i] - list2[i]);
  } else {
    std::unordered_map<int, int> counts;
    for (const int &elt : list2)
      counts[elt]++;

    for (const int &elt : list1)
      ans += elt * counts[elt];
  }

  std::cout << "Answer : " << ans << std::endl;
  return 0;
}
