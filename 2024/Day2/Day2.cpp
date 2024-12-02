
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using Report = std::vector<int>;

bool is_safe(const Report &report) {
  int sign = (report[1] > report[0] ? 1 : -1);
  for (size_t i = 0; i < report.size() - 1; ++i) {
    int diff = sign * (report[i + 1] - report[i]);
    if (!(1 <= diff && diff <= 3))
      return false;
  }
  return true;
}

bool is_safe_dampened(const Report &report) {
  if (is_safe(report))
    return true;

  for (size_t i = 0; i < report.size(); ++i) {
    Report report_copy(report);
    report_copy.erase(report_copy.begin() + i);
    if (is_safe(report_copy))
      return true;
  }
  return false;
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

  std::vector<Report> reports;
  if (myfile.is_open()) {
    std::string input;
    while (getline(myfile, input)) {
      std::stringstream sstr(input);
      Report report;
      int level;
      while (sstr >> level)
        report.push_back(level);
      reports.push_back(report);
    }
    myfile.close();
  } else {
    std::cout << "Unable to open file" << std::endl;
    return 1;
  }

  for (const Report &report : reports) {
    if (first_half)
      ans += is_safe(report);
    else
      ans += is_safe_dampened(report);
  }

  std::cout << "Answer : " << ans << std::endl;
  return 0;
}
