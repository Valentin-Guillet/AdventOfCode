
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

inline int min(int x, int y) { return (x > y ? y : x); }

long get_filesystem_block_checksum(const std::vector<int> &disk_map) {
  int nb_blocks = 0;
  long checksum = 0;

  int beg_ind = 0;
  int end_ind = disk_map.size() - 1;
  int end_val = disk_map[end_ind];

  while (beg_ind < end_ind) {
    // Add file
    for (int i = 0; i < disk_map[beg_ind]; ++i)
      checksum += (beg_ind / 2) * nb_blocks++;
    beg_ind++;

    // If we fitted all files, add remaining values and break
    if (beg_ind + 1 == end_ind) {
      for (int i = 0; i < end_val; ++i)
        checksum += (end_ind / 2) * nb_blocks++;
      break;
    }

    // Fill blank space with end files
    int blank = disk_map[beg_ind++];
    while (blank > 0) {
      int nb_to_remove = min(blank, end_val);
      for (int i = 0; i < nb_to_remove; ++i)
        checksum += (end_ind / 2) * nb_blocks++;

      blank -= nb_to_remove;
      end_val -= nb_to_remove;

      if (end_val == 0) {
        end_ind -= 2;  // Skip blankspace
        end_val = disk_map[end_ind];
      }
    }
  }

  return checksum;
}

long get_filesystem_file_checksum(const std::vector<int> &disk_map) {
  int nb_blocks = 0;
  long checksum = 0;

  const int n = disk_map.size();
  std::vector<bool> moved(n, false);

  int beg_ind = 0;
  int last_movable_ind = n - 1;

  while (beg_ind < last_movable_ind) {
    // Add file
    if (!moved[beg_ind]) {
      for (int i = 0; i < disk_map[beg_ind]; ++i)
        checksum += (beg_ind / 2) * nb_blocks++;
    } else { // Replace moved file with blank
      nb_blocks += disk_map[beg_ind];
    }
    beg_ind++;

    // Fill blank space with end files
    int blank = disk_map[beg_ind];
    while (blank > 0) {
      // Find last file that can be put in blank space
      int mov_ind = last_movable_ind;
      while (mov_ind > beg_ind && (moved[mov_ind] || disk_map[mov_ind] > blank))
        mov_ind -= 2;

      if (mov_ind <= beg_ind) { // No file can fit in blank
        nb_blocks += blank;
        break;
      }

      // Move file in blank
      moved[mov_ind] = true;
      for (int i = 0; i < disk_map[mov_ind]; ++i)
        checksum += (mov_ind / 2) * nb_blocks++;
      blank -= disk_map[mov_ind];

      // If it was the last movable file, update index
      if (mov_ind == last_movable_ind) {
        while (moved[last_movable_ind])
          last_movable_ind -= 2;
      }
    }
    beg_ind++;
  }

  while (beg_ind < n) {
    // Add file
    if (!moved[beg_ind]) {
      for (int i = 0; i < disk_map[beg_ind]; ++i)
        checksum += (beg_ind / 2) * nb_blocks++;
    } else {
      nb_blocks += disk_map[beg_ind];
    }
    beg_ind++;

    if (beg_ind == n)
      break;

    // Add blank
    nb_blocks += disk_map[beg_ind++];
  }

  return checksum;
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

  std::vector<int> disk_map;
  if (myfile.is_open()) {
    std::string input;
    getline(myfile, input);
    for (char c : input)
      disk_map.push_back(c - '0');

    myfile.close();
  } else {
    std::cout << "Unable to open file" << std::endl;
    return 1;
  }

  if (first_half)
    ans = get_filesystem_block_checksum(disk_map);
  else
    ans = get_filesystem_file_checksum(disk_map);

  std::cout << "Answer : " << ans << std::endl;
  return 0;
}
