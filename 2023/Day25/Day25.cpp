
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using NodeId = size_t;
using NodeMap = std::unordered_map<std::string, NodeId>;
using Edge = std::pair<NodeId, NodeId>;

struct DisjointSet {
    size_t n;
    std::vector<NodeId> parents;
    std::vector<NodeId> ranks;
    int nb_components;

    explicit DisjointSet(size_t n_) : n(n_), ranks(n_, 0), nb_components(n_) {
        for (NodeId i = 0; i < n; ++i)
            parents.push_back(i);
    }

    NodeId find(NodeId x) {
        if (parents[x] != x)
            parents[x] = find(parents[x]);
        return parents[x];
    }

    void merge(NodeId x, NodeId y) {
        NodeId x_root = find(x);
        NodeId y_root = find(y);
        if (x_root == y_root)
            return;

        nb_components--;
        if (ranks[x_root] < ranks[y_root]) {
            parents[x_root] = y_root;
        } else {
            parents[y_root] = x_root;
            if (ranks[x_root] == ranks[y_root])
                ranks[x_root]++;
        }
    }

    int get_score() {
        NodeId root = find(0);
        int count = 1;
        for (NodeId x = 1; x < n; ++x) {
            if (find(x) == root)
                count++;
        }
        return count * (n - count);
    }
};

int count_min_cut(size_t n, std::vector<Edge> edges) {
    while (true) {
        std::random_shuffle(edges.begin(), edges.end());

        int cut_weights = 0;
        DisjointSet dset(n);
        for (const Edge& edge : edges) {
            if (dset.nb_components != 2)
                dset.merge(edge.first, edge.second);
            else
                cut_weights += (dset.find(edge.first) != dset.find(edge.second));
        }

        if (cut_weights == 3)
            return dset.get_score();
    }
}

int main(int argc, char* argv[])
{
    bool first_half = true;
    int ans = 0;

    std::ifstream myfile("input");
    for (int i=1; i<argc; i++) {
        std::string arg(argv[i]);
        if (arg == "-t" || arg == "--test")
            myfile = std::ifstream("test_input");

        else if (arg == "-s" || arg == "--second")
            first_half = false;
    }

    NodeMap node_map;
    std::vector<Edge> edges;
    if (myfile.is_open()) {
        for (std::string input; getline(myfile, input);) {
            std::string node_name = input.substr(0, 3);
            if (!node_map.contains(node_name))
                node_map[node_name] = node_map.size();

            for (size_t i = 5; i < input.size(); i += 4) {
                std::string neigh_name = input.substr(i, 3);
                if (!node_map.contains(neigh_name))
                    node_map[neigh_name] = node_map.size();
                edges.push_back({node_map[node_name], node_map[neigh_name]});
            }
        }
        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    ans = count_min_cut(node_map.size(), edges);
    if (!first_half)
        std::cout << "Merry Christmas !" << std::endl;

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
