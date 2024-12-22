
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using Design = std::string;
using Cache = std::vector<long>;

struct Trie {
  std::string label;
  bool is_leaf = false;
  std::vector<Trie*> children;

  ~Trie() {
    for (Trie *child : children)
      delete child;
  }

  void insert(const std::string &word, size_t index = 0) {
    // Node corresponding to word already exist -> mark it as leaf
    size_t word_size = word.size();
    if (index == word_size) {
      is_leaf = true;
      return;
    }

    for (Trie *child : children) {
      // No common prefix
      if (child->label[0] != word[index])
        continue;

      // The common prefix is the entire child label: recursively insert word
      size_t child_label_size = child->label.size();
      if (child->label == word.substr(index, child_label_size)) {
        child->insert(word, index + child_label_size);
        return;
      }

      size_t common_prefix_size = 1;
      while (index + common_prefix_size < word_size && common_prefix_size < child_label_size &&
          word[index + common_prefix_size] == child->label[common_prefix_size])
        common_prefix_size++;

      std::string common_prefix = child->label.substr(0, common_prefix_size);
      Trie *new_child = new Trie();
      if (index + common_prefix_size == word_size) {
        new_child->label = child->label.substr(common_prefix_size);
        new_child->is_leaf = child->is_leaf;
        new_child->children = child->children;

        child->label = common_prefix;
        child->is_leaf = true;
        child->children = {new_child};

      } else if (common_prefix_size == child_label_size) {
        new_child->label = word.substr(index + common_prefix_size);
        new_child->is_leaf = child->is_leaf;

        child->label = common_prefix;
        child->is_leaf = true;
        child->children.push_back(new_child);

      } else {
        new_child->label = word.substr(index + common_prefix_size);
        new_child->is_leaf = true;

        Trie *other_child = new Trie();
        other_child->label = child->label.substr(common_prefix_size);
        other_child->is_leaf = child->is_leaf;
        other_child->children = child->children;

        child->label = common_prefix;
        child->is_leaf = false;
        child->children = {new_child, other_child};
      }
      return;
    }

    // No matching child: add node
    Trie *new_child = new Trie();
    new_child->label = word.substr(index);
    new_child->is_leaf = true;
    children.push_back(new_child);
  }

  void get_matches(const std::string &word, size_t word_index, std::vector<size_t> &match_lengths, size_t index = 0) const {
    if (is_leaf)
      match_lengths.push_back(index);

    for (Trie *child : children) {
      if (child->label[0] != word[word_index + index])
        continue;

      size_t child_label_size = child->label.size();
      for (size_t i = 0; i < child_label_size; ++i) {
        if (word[word_index + index + i] != child->label[i])
          return;
      }
      child->get_matches(word, word_index, match_lengths, index + child_label_size);
    }
  }
};

long count_possibilities(const Trie &towels, const Design &design, Cache &cache, size_t index = 0) {
  size_t design_size = design.size();
  if (index >= design_size)
    return 1;

  long &cache_value = cache[index];
  if (cache_value != -1)
    return cache_value;

  long nb_possibilities = 0;
  std::vector<size_t> match_lengths;
  match_lengths.reserve(5);
  towels.get_matches(design, index, match_lengths);
  for (size_t match_length : match_lengths)
    nb_possibilities += count_possibilities(towels, design, cache, index + match_length);

  cache_value = nb_possibilities;
  return nb_possibilities;
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

  Trie towels;
  std::vector<Design> designs;
  if (myfile.is_open()) {
    std::string input;
    getline(myfile, input);
    size_t prev_index = 0;
    size_t index;
    do {
      index = input.find(',', prev_index);
      towels.insert(input.substr(prev_index, index - prev_index));
      prev_index = index + 2;
    } while (index != std::string::npos);

    getline(myfile, input);
    while (getline(myfile, input))
      designs.emplace_back(input);

    myfile.close();
  } else {
    std::cout << "Unable to open file" << std::endl;
    return 1;
  }

  for (const Design &design : designs) {
    Cache cache(design.size(), -1);
    long nb_possibilities = count_possibilities(towels, design, cache);
    if (first_half)
      ans += (nb_possibilities > 0);
    else
      ans += nb_possibilities;
  }

  std::cout << "Answer : " << ans << std::endl;
  return 0;
}
