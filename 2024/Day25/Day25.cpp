
#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using Key = std::array<int, 5>;
using Lock = std::array<int, 5>;

Key read_key(std::ifstream &myfile) {
  Key key;
  key.fill(-1);
  std::string input;
  for (int i = 0; i < 6; ++i) {
    getline(myfile, input);
    for (int j = 0; j < 5; ++j) {
      if (input[j] == '#' && key[j] == -1)
        key[j] = 5 - i;
    }
  }
  return key;
}

Lock read_lock(std::ifstream &myfile) {
  Lock lock;
  lock.fill(-1);
  std::string input;
  for (int i = 0; i < 6; ++i) {
    getline(myfile, input);
    for (int j = 0; j < 5; ++j) {
      if (input[j] == '.' && lock[j] == -1)
        lock[j] = i;
    }
  }
  return lock;
}

bool unlock(const Lock &lock, const Key &key) {
  for (int i = 0; i < 5; ++i) {
    if (lock[i] + key[i] > 5)
      return false;
  }
  return true;
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

  std::vector<Key> keys;
  std::vector<Lock> locks;
  if (myfile.is_open()) {
    std::string input;
    while (getline(myfile, input)) {
      if (input[0] == '.')
        keys.push_back(read_key(myfile));
      else
        locks.push_back(read_lock(myfile));

      getline(myfile, input);
    }
    myfile.close();
  } else {
    std::cout << "Unable to open file" << std::endl;
    return 1;
  }

  for (const Lock &lock : locks) {
    for (const Key &key : keys) {
      ans += unlock(lock, key);
    }
  }

  if (!first_half)
    std::cout << "Merry Christmas !" << std::endl;

  std::cout << "Answer : " << ans << std::endl;
  return 0;
}
