
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

struct Device {
  std::string name;
  std::vector<std::string> outputs;

  Device(const std::string &input) {
    name = input.substr(0, 3);

    size_t index = 5;
    while (index < input.size()) {
      outputs.emplace_back(input.substr(index, 3));
      index += 4;
    }
  }
};

using DeviceMap = std::unordered_map<std::string, Device>;
using Cache = std::unordered_map<std::string, long>;

namespace std {
template <> struct hash<Device> {
  size_t operator()(const Device &device) const {
    return std::hash<std::string>()(device.name);
  }
};
} // namespace std

int count_paths(const DeviceMap &devices, const std::string &source,
                Cache &cache) {
  if (cache.contains(source))
    return cache[source];

  int nb_paths = 0;
  for (const std::string &name : devices.at(source).outputs) {
    if (name == "out")
      nb_paths++;
    else
      nb_paths += count_paths(devices, name, cache);
  }
  cache[source] = nb_paths;
  return nb_paths;
}

long count_valid_paths(const DeviceMap &devices, const std::string &source,
                      int valid_mask, Cache &cache) {
  std::string hash = source;
  hash.push_back(valid_mask + '0');

  if (cache.contains(hash))
    return cache[hash];

  if (source == "dac")
    valid_mask |= 1;
  else if (source == "fft")
    valid_mask |= 2;

  long nb_paths = 0;
  for (const std::string &name : devices.at(source).outputs) {
    if (name == "out")
      nb_paths += (valid_mask == 3);
    else
      nb_paths += count_valid_paths(devices, name, valid_mask, cache);
  }
  cache[hash] = nb_paths;
  return nb_paths;
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

  std::unordered_map<std::string, Device> devices;
  if (myfile.is_open()) {
    std::string input;
    while (getline(myfile, input))
      devices.emplace(input.substr(0, 3), std::move(input));
    myfile.close();
  } else {
    std::cout << "Unable to open file" << std::endl;
    return 1;
  }

  Cache cache;
  if (first_half)
    ans = count_paths(devices, "you", cache);
  else
    ans = count_valid_paths(devices, "svr", 0, cache);

  std::cout << "Answer : " << ans << std::endl;
  return 0;
}
