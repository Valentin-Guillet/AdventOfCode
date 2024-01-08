
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
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

void simplify_graph(Graph& graph, NodeId start) {
    /* Given the particular shape of the graph, we can remove some edges to accelerate the search.
     * Indeed, the graph looks like a grid, and so when reaching a node on the perimeter of this
     * grid, the path cannot go "towards the start" (or it would have to cross itself in order to
     * reach the target). In our case, the nodes on the perimeter are the nodes that have exactly
     * 3 neighbors, so we can remove the edges between perimeter nodes that are in the "wrong"
     * direction.
     */
    std::queue<NodeId> node_queue;
    node_queue.push(start);

    SeenMask seen = 0;
    std::vector<std::pair<NodeId, NodeId>> edges_to_remove;
    while (!node_queue.empty()) {
        NodeId node = node_queue.front();
        node_queue.pop();

        if (seen & (1ULL << node))
            continue;

        seen |= (1ULL << node);
        bool rm_source = (graph[node].size() == 3);
        for (const auto& [neigh, _] : graph[node]) {
            if (rm_source && !(seen & (1ULL << neigh)) && graph[neigh].size() == 3)
                edges_to_remove.push_back({neigh, node});
            node_queue.push(neigh);
        }
    }
    for (auto [node1, node2] : edges_to_remove)
        graph[node1].erase(node2);
}

int count_path_length(const Graph& graph, NodeId node, NodeId target, SeenMask seen = 0) {
    if (node == target)
        return 0;

    int max_length = 0;
    seen |= (1ULL << node);
    for (const auto& [neigh, dist] : graph.at(node)) {
        if (seen & (1ULL << neigh))
            continue;

        int length = dist + count_path_length(graph, neigh, target, seen);
        if (length > max_length)
            max_length = length;
    }
    return max_length;
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
    if (!first_half)
        simplify_graph(graph, node_id_map[start]);
    ans = count_path_length(graph, node_id_map[start], node_id_map[target]);

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
