
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

long get_low_part(const std::string &n, size_t &size) {
  long low = std::stol(n.substr(0, size));
  for (size_t i = 1; i < n.size() / size; ++i) {
    long value = std::stol(n.substr(i * size, size));
    if (value < low)
      return low;
    else if (low < value)
      return low + 1;
  }
  return low;
}

long get_high_part(const std::string &n, size_t size) {
  long high = std::stol(n.substr(0, size));
  for (size_t i = 1; i < n.size() / size; ++i) {
    long value = std::stol(n.substr(i * size, size));
    if (high < value)
      return high;
    else if (value < high)
      return high - 1;
  }
  return high;
}

long count_invalid(const std::string &low, const std::string &high,
                   bool only_half) {
  long ans = 0;
  if (low.size() != high.size()) {
    std::string curr_val = low;
    while (curr_val.size() < high.size()) {
      ans +=
          count_invalid(curr_val, std::string(curr_val.size(), '9'), only_half);
      curr_val = "1" + std::string(curr_val.size(), '0');
    }
    return ans + count_invalid(curr_val, high, only_half);
  }

  std::vector<int> factors;
  for (size_t nb_parts = 2; nb_parts <= high.size(); nb_parts++) {
    if (only_half && nb_parts > 2)
      return ans;

    if (high.size() % nb_parts != 0)
      continue;

    size_t seq_size = high.size() / nb_parts;

    // Don't count seq_size of 2 if we already saw seq_size of 4
    // Except for 1, as we must compute the 1s sums to count the duplicates
    // as it is counted in all cases
    bool already_seen = false;
    for (int factor : factors) {
      if (factor % seq_size == 0) {
        already_seen = true;
        break;
      }
    }
    if (seq_size > 1 && already_seen)
      continue;
    factors.push_back(seq_size);

    long low_part = get_low_part(low, seq_size);
    long high_part = get_high_part(high, seq_size);
    if (low_part > high_part)
      continue;

    long ten_power = 1;
    for (size_t i = 0; i < seq_size; ++i)
      ten_power *= 10;

    long high_sum = high_part * (high_part + 1) / 2;
    long low_sum = low_part * (low_part - 1) / 2;
    long total_sum = high_sum - low_sum;

    long mult = 1;
    long delta = 0;
    for (size_t i = 0; i < high.size() / seq_size; ++i) {
      delta += mult * total_sum;
      mult *= ten_power;
    }

    // We count seq_size of 1 for each factors, so we must remove the duplicates
    if (seq_size == 1) {
      ans += (2 - factors.size()) * delta;
    } else {
      ans += delta;
    }
  }
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

  std::vector<std::pair<std::string, std::string>> ranges;
  if (myfile.is_open()) {
    std::string input;
    while (getline(myfile, input, ',')) {
      if (input.ends_with('\n'))
        input = input.substr(0, input.size() - 1);
      size_t index = input.find('-');
      std::string low = input.substr(0, index);
      std::string high = input.substr(index + 1);
      ranges.emplace_back(low, high);
    }
    myfile.close();
  } else {
    std::cout << "Unable to open file" << std::endl;
    return 1;
  }

  for (auto &[low, high] : ranges)
    ans += count_invalid(low, high, first_half);

  std::cout << "Answer : " << ans << std::endl;
  return 0;
}
