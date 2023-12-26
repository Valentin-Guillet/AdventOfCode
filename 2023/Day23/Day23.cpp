
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>


struct Position {
    int row, col;

    inline bool operator==(const Position& other) const {
        return other.row == row && other.col == col;
    }
};

struct HashPosition {
    size_t operator()(const Position& pos) const {
        return (pos.row << 16) | pos.col;
    }
};


using Grid = std::vector<std::string>;

template<class T>
using PosMap = std::unordered_map<Position, T, HashPosition>;

using NodeId = int;
using Graph = std::unordered_map<NodeId, std::unordered_map<NodeId, int>>;
using SeenMask = unsigned long long int;
using Memory = std::unordered_map<unsigned long long int, int>;

PosMap<bool> get_neighs(const Grid& grid, Position pos, Position prev_pos) {
    PosMap<bool> neighbors;

    if (pos.row > 0 && grid[pos.row - 1][pos.col] != '#' && !(prev_pos.row == pos.row - 1 && prev_pos.col ==  pos.col))
        neighbors[{pos.row - 1, pos.col}] = (grid[pos.row - 1][pos.col] != 'v');
    if (pos.row < (int)grid.size() - 1 && grid[pos.row + 1][pos.col] != '#' && !(prev_pos.row == pos.row + 1 && prev_pos.col ==  pos.col))
        neighbors[{pos.row + 1, pos.col}] = (grid[pos.row + 1][pos.col] != '^');
    if (pos.col > 0 && grid[pos.row][pos.col - 1] != '#' && !(prev_pos.row == pos.row && prev_pos.col ==  pos.col - 1))
        neighbors[{pos.row, pos.col - 1}] = (grid[pos.row][pos.col - 1] != '>');
    if (pos.col < (int)grid[0].size() - 1 && grid[pos.row][pos.col + 1] != '#' && !(prev_pos.row == pos.row && prev_pos.col ==  pos.col + 1))
        neighbors[{pos.row, pos.col + 1}] = (grid[pos.row][pos.col + 1] != '<');

    return neighbors;
}

void build_node(const Grid& grid, Graph& graph, PosMap<NodeId>& node_id_map, Position pos, Position prev_pos, Position target, bool directed) {
    if (pos == target)
        return;

    Position start_pos = prev_pos;
    int path_length = 0;

    PosMap<bool> neighbors;
    while (true) {
        path_length++;
        neighbors = get_neighs(grid, pos, prev_pos);
        if (neighbors.size() != 1)
            break;

        prev_pos = pos;
        pos = neighbors.begin()->first;
    }

    if (!node_id_map.contains(start_pos))
        node_id_map[start_pos] = node_id_map.size() + 1;
    if (!node_id_map.contains(pos))
        node_id_map[pos] = node_id_map.size() + 1;

    graph[node_id_map[start_pos]][node_id_map[pos]] = path_length;
    if (!directed)
        graph[node_id_map[pos]][node_id_map[start_pos]] = path_length;

    for (auto& [neigh, follow_path] : neighbors) {
        if (follow_path)
            build_node(grid, graph, node_id_map, neigh, pos, target, directed);
    }
}

Graph build_graph(const Grid& grid, Position start, Position target, PosMap<NodeId>& node_id_map, bool directed) {
    Graph graph;

    build_node(grid, graph, node_id_map, start, start, target, directed);

    // Mark node linked to target as directed (cause it's the only one that access the exit)
    int target_id = node_id_map[target];
    for (auto& [node_id, neighbors] : graph) {
        if (neighbors.contains(target_id)) {
            int dist = neighbors[target_id];
            neighbors.clear();
            neighbors[target_id] = dist;
        }
    }
    graph[target_id].clear();

    // Start counting distance from {1, 1}
    for (auto& [neigh, dist] : graph[node_id_map[start]])
        dist--;

    return graph;
}

int count_path_length(const Graph& graph, NodeId node, NodeId target, SeenMask seen, Memory& memory, int depth=0) {
    unsigned long long seen_hash = (seen << 16) | node;
    if (memory.contains(seen_hash))
        return memory[seen_hash];

    if (node == target)
        return 0;

    int length = 0;
    seen |= (1ULL << node);
    for (auto [neigh, dist] : graph.at(node)) {
        if (seen & (1ULL << neigh))
            continue;

        int new_length = dist + count_path_length(graph, neigh, target, seen, memory, depth+1);
        if (new_length > length)
            length = new_length;
    }
    memory[seen_hash] = length;
    return length;
}

int count_path_length(const Graph& graph, NodeId start, NodeId target) {
    Memory memory;
    return count_path_length(graph, start, target, {}, memory);
}

int main(int argc, char* argv[])
{
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

    std::vector<std::string> grid;
    if (myfile.is_open()) {
        for (std::string input; getline(myfile, input); )
            grid.emplace_back(input);
        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    Position start = {0, (int)grid[0].find('.')};
    Position target{(int)grid.size() - 1, (int)grid[grid.size() - 1].find('.')};

    PosMap<NodeId> node_id_map;
    Graph graph = build_graph(grid, start, target, node_id_map, first_half);
    ans = count_path_length(graph, node_id_map[start], node_id_map[target]);

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
