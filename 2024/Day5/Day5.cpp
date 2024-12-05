
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using Dependencies = std::unordered_set<int>;
using RuleMap = std::unordered_map<int, Dependencies>;
using Update = std::vector<int>;

bool is_valid(const Update &update, const RuleMap &rules) {
  for (size_t i = 1; i < update.size(); ++i) {
    const Dependencies &dependencies = rules.at(update[i]);
    for (size_t j = 0; j < i; ++j) {
      if (dependencies.contains(update[j]))
        return false;
    }
  }
  return true;
}

Update fix(const Update &update, const RuleMap &rules) {
  // The input has an interesting property: each update has a page with
  // 0 dependency, a page with 1, another with 2... until update.size()
  std::vector<int> fixed_update(update.size(), 0);
  for (size_t i = 0; i < update.size(); ++i) {
    int nb_requirements = 0;
    const Dependencies &dependencies = rules.at(update[i]);

    for (size_t j = 0; j < update.size(); ++j) {
      if (i != j && dependencies.contains(update[j]))
        nb_requirements++;
    }
    fixed_update[nb_requirements] = update[i];
  }
  return fixed_update;
}

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

    RuleMap rules;
    std::vector<Update> updates;
    if (myfile.is_open()) {
      std::string input;
      while (getline(myfile, input)) {
        if (input.empty())
          break;
        int before = std::stoi(input.substr(0, 2));
        int after = std::stoi(input.substr(3, 2));
        rules[before].insert(after);
      }

      while (getline(myfile, input)) {
        std::stringstream input_stream(input);
        std::string page;
        Update update;
        while (getline(input_stream, page, ','))
          update.push_back(std::stoi(page));
        updates.push_back(update);
      }

      myfile.close();
    } else {
      std::cout << "Unable to open file" << std::endl;
      return 1;
    }

    for (const Update &update : updates) {
      bool valid = is_valid(update, rules);
      if (first_half && valid)
        ans += update[update.size() / 2];
      else if (!first_half && !valid)
        ans += fix(update, rules)[update.size() / 2];
    }

    std::cout << "Answer : " << ans << std::endl;
    return 0;
  }
