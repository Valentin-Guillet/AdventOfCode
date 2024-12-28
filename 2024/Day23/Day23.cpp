
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using Id = int;
using IdSet = std::unordered_set<Id>;
using IdMap = std::unordered_map<std::string, Id>;
using AdjList = std::vector<std::vector<Id>>;

Id add_id(IdMap &id_map, AdjList &adjacency, std::vector<Id> &id_is_t, std::string id_str) {
  if (id_map.contains(id_str))
    return id_map[id_str];

  Id id = id_map.size();
  id_map[id_str] = id;
  id_is_t.push_back(id_str[0] == 't');
  adjacency.push_back({});

  return id;
}

int count_3_cliques(const int nb_ids, const AdjList &adjacency, const std::vector<Id> &id_is_t) {
  int count = 0;
  for (Id id1 = 0; id1 < nb_ids; ++id1) {
    if (!id_is_t[id1])
      continue;

    for (Id id2 : adjacency[id1]) {
      if (id_is_t[id2] && id2 < id1)
        continue;

      for (Id id3 : adjacency[id2]) {
        if (id3 == id1 || id3 < id2 || (id_is_t[id3] && id3 < id1))
          continue;

        const std::vector<Id> &adj3 = adjacency[id3];
        if (std::find(adj3.begin(), adj3.end(), id1) != adj3.end())
          count++;
      }
    }
  }
  return count;
}

void BronKerbosch(const AdjList &adjacency, const IdSet &R, const IdSet &P, IdSet X, IdSet &output) {
  if (P.empty() && X.empty()) {
    if (R.size() > output.size())
      output = R;
    return;
  }

  if (P.size() + R.size() <= output.size())
    return;

  IdSet P_mod(P);
  for (Id id : P) {
    IdSet new_R(R), new_P, new_X;
    new_R.insert(id);
    for (Id neigh : adjacency[id]) {
      if (P_mod.contains(neigh))
        new_P.insert(neigh);
      if (X.contains(neigh))
        new_X.insert(neigh);
    }
    BronKerbosch(adjacency, new_R, new_P, new_X, output);
    P_mod.erase(id);
    X.insert(id);
  }
}

IdSet get_max_clique(const IdMap &id_map, const AdjList &adjacency) {
  IdSet all_ids;
  for (int i = 0; i < (int)id_map.size(); ++i)
    all_ids.insert(i);
  IdSet output;
  BronKerbosch(adjacency, {}, all_ids, {}, output);
  return output;
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

  IdMap id_map;
  std::vector<int> id_is_t;
  AdjList adjacency_list;
  if (myfile.is_open()) {
    std::string input;
    while (getline(myfile, input)) {
      std::string id_str1 = input.substr(0, 2);
      std::string id_str2 = input.substr(3);

      Id id1 = add_id(id_map, adjacency_list, id_is_t, id_str1);
      Id id2 = add_id(id_map, adjacency_list, id_is_t, id_str2);

      adjacency_list[id1].push_back(id2);
      adjacency_list[id2].push_back(id1);
    }
    myfile.close();
  } else {
    std::cout << "Unable to open file" << std::endl;
    return 1;
  }

  if (first_half) {
    ans = count_3_cliques(id_map.size(), adjacency_list, id_is_t);
  } else {
    IdSet max_clique = get_max_clique(id_map, adjacency_list);
    std::vector<std::string> clique_computers;
    for (const auto &[computer, id] : id_map) {
      if (max_clique.contains(id))
        clique_computers.emplace_back(computer);
    }
    std::sort(clique_computers.begin(), clique_computers.end());
    std::cout << clique_computers[0];
    for (size_t i = 1; i < clique_computers.size(); ++i)
      std::cout << ',' << clique_computers[i];
    std::cout << "\n";
  }

  std::cout << "Answer : " << ans << std::endl;
  return 0;
}
