
#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

template <class T> using DiffSet = std::array<T, 19 * 19 * 19 * 19>;

inline int compute_next_secret_number(int secret_number) {
  secret_number = (secret_number ^ (secret_number << 6)) & 0xffffff;
  secret_number = (secret_number ^ (secret_number >> 5)) & 0xffffff;
  secret_number = (secret_number ^ (secret_number << 11)) & 0xffffff;
  return secret_number;
}

inline size_t get_price_hash(int price_diff, int prev_diff) {
  return (prev_diff % 6859) * 19 + (price_diff + 9);
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

  std::vector<int> secret_numbers;
  if (myfile.is_open()) {
    std::string input;
    while (getline(myfile, input))
      secret_numbers.emplace_back(std::stoi(input));
    myfile.close();
  } else {
    std::cout << "Unable to open file" << std::endl;
    return 1;
  }

  if (first_half) {
    for (int secret_number : secret_numbers) {
      for (int step = 0; step < 2000; ++step)
        secret_number = compute_next_secret_number(secret_number);
      ans += secret_number;
    }
  } else {
    DiffSet<int> hash_price_map{};
    for (int secret_number : secret_numbers) {

      int previous_price = secret_number % 10;
      size_t diff_hash = get_price_hash(previous_price, 0);
      DiffSet<bool> seen{};

      for (int step = 0; step < 2000; ++step) {
        secret_number = compute_next_secret_number(secret_number);
        int price = secret_number % 10;
        diff_hash = get_price_hash(price - previous_price, diff_hash);

        if (!seen[diff_hash]) {
          hash_price_map[diff_hash] += price;
          seen[diff_hash] = true;
        }

        previous_price = price;
      }
    }

    ans = *std::max_element(hash_price_map.begin(), hash_price_map.end());
  }

  std::cout << "Answer : " << ans << std::endl;
  return 0;
}
