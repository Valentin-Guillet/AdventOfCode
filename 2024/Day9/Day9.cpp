
#include <array>
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
  int end_val = disk_map[end_ind]; // Nb of remaining blocks of last file

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
        end_ind -= 2; // Skip blankspace
        end_val = disk_map[end_ind];
      }
    }
  }

  return checksum;
}

std::array<int, 9> init_last_mov_inds(const std::vector<int> &disk_map) {
  std::array<int, 9> last_mov_inds{};
  std::vector<int> to_update = {0, 1, 2, 3, 4, 5, 6, 7, 8};
  int index = disk_map.size() - 1;
  while (!to_update.empty()) {
    while (disk_map[index] > to_update.back() + 1)
      index -= 2;

    while (!to_update.empty() && disk_map[index] <= to_update.back() + 1) {
      last_mov_inds[to_update.back()] = index;
      to_update.pop_back();
    }
    index -= 2;
  }
  return last_mov_inds;
}

void update_mov_ind(const std::vector<int> &disk_map,
                    const std::vector<char> &moved,
                    std::array<int, 9> &last_movable_inds,
                    int beg_ind) {
  // Count movable indices that are equal to used_index
  std::vector<int> to_update;
  for (int i = 0; i < 9; ++i) {
    if (moved[last_movable_inds[i]])
      to_update.push_back(i);
  }

  // Update all these indices
  int index = last_movable_inds[to_update[0]] - 2;
  while (!to_update.empty()) {
    // Get next index that has not been moved yet and that is big enough
    while (index > beg_ind &&
           (moved[index] || disk_map[index] > to_update.back() + 1))
      index -= 2;

    // No file left is movable, set remaining indices to 0 and exit
    if (index <= beg_ind) {
      for (int i : to_update)
        last_movable_inds[i] = 0;
      break;
    }

    // Set file index to all larger last_movable_indices
    while (!to_update.empty() && disk_map[index] <= to_update.back() + 1) {
      last_movable_inds[to_update.back()] = index;
      to_update.pop_back();
    }
    index -= 2;
  }
}

long get_filesystem_file_checksum(const std::vector<int> &disk_map) {
  int nb_blocks = 0;
  long checksum = 0;

  const int n = disk_map.size();
  // Has i-th file been moved?
  std::vector<char> moved(n, 0); // Use chars because vector<bool> access is really slow

  int beg_ind = 0;

  // The i-th cell of last_movable_inds contains the index of the last file
  // that could fit inside a blank space of size i + 1
  // /!\ The file can be smaller than i+1! If we find a blank space of size 3
  // but a file of size 1 has a larger index than a file of size 3, it
  // has priority when moving files.
  std::array<int, 9> last_movable_inds = init_last_mov_inds(disk_map);

  while (beg_ind < n - 1) {
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
      int mov_ind = last_movable_inds[blank - 1];

      if (mov_ind == 0) { // No file can fit in blank, skip
        nb_blocks += blank;
        break;
      }

      // Move file in blank
      moved[mov_ind] = true;
      for (int i = 0; i < disk_map[mov_ind]; ++i)
        checksum += (mov_ind / 2) * nb_blocks++;
      blank -= disk_map[mov_ind];

      // Update all movable indices that were pointing toward mov_ind
      update_mov_ind(disk_map, moved, last_movable_inds, beg_ind);
    }
    beg_ind++;

    for (int i = 0; i < 9; ++i) {
      if (last_movable_inds[i] <= beg_ind)
        last_movable_inds[i] = 0;
    }
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
