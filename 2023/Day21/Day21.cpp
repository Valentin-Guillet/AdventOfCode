
#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_set>
#include <vector>


struct Position {
    int row, col;
};

template<class T>
using Grid = std::vector<std::vector<T>>;

std::vector<Position> get_neighbors(const Grid<char>& grid, Position pos) {
    std::vector<Position> neighbors;
    if (pos.row > 0 && grid[pos.row - 1][pos.col] != '#')
        neighbors.push_back({pos.row - 1, pos.col});
    if (pos.row < (int)grid.size() - 1 && grid[pos.row + 1][pos.col] != '#')
        neighbors.push_back({pos.row + 1, pos.col});
    if (pos.col > 0 && grid[pos.row][pos.col - 1] != '#')
        neighbors.push_back({pos.row, pos.col - 1});
    if (pos.col < (int)grid[0].size() - 1 && grid[pos.row][pos.col + 1] != '#')
        neighbors.push_back({pos.row, pos.col + 1});

    return neighbors;
}

Grid<int> compute_distances(const Grid<char>& grid, Position start) {
    Grid<int> distances(grid.size(), std::vector<int>(grid[0].size(), -1));
    distances[start.row][start.col] = 0;

    std::queue<Position> pos_queue;
    pos_queue.push(start);
    while (!pos_queue.empty()) {
        Position curr_pos = pos_queue.front();
        pos_queue.pop();

        int dist = distances[curr_pos.row][curr_pos.col];
        for (Position neigh : get_neighbors(grid, curr_pos)) {
            int& neigh_dist = distances[neigh.row][neigh.col];
            if (neigh_dist == -1) {
                pos_queue.push(neigh);
                neigh_dist = dist + 1;
            }
        }
    }

    return distances;
}

int count_plots(const Grid<char>& grid, Position start, int radius) {
    int count = 0;
    Grid<int> distances = compute_distances(grid, start);
    for (int row = 0; row < (int)grid.size(); ++row) {
        for (int col = 0; col < (int)grid[0].size(); ++col) {
            int dist = distances[row][col];
            count += (dist > -1 && dist % 2 == radius % 2 && dist <= radius);
        }
    }
    return count;
}

int main(int argc, char* argv[])
{
    bool first_half = true;
    long long ans = 0;

    std::ifstream myfile("input");
    for (int i = 1; i < argc; ++i) {
        std::string arg(argv[i]);
        if (arg == "-t" || arg == "--test")
            myfile = std::ifstream("test_input");

        else if (arg == "-s" || arg == "--second")
            first_half = false;
    }

    Grid<char> grid;
    Position start;
    if (myfile.is_open()) {
        int row = 0;
        for (std::string input; getline(myfile, input); ) {
            if (input.contains('S'))
                start = {row, (int)input.find('S')};
            std::vector<char> line(input.begin(), input.end());
            grid.emplace_back(line);
            row++;
        }
        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    if (first_half) {
        ans = count_plots(grid, start, 64);
    } else {
        // Input has a specific structure which allows for "simple" math computation
        // Cf. notes.txt
        const int nb_steps = 26501365;
        long long nb_tiles = nb_steps / grid.size();

        long long count_odd = count_plots(grid, start, 1001);
        long long count_even = count_plots(grid, start, 1000);

        long long count_diamond_odd = count_plots(grid, start, grid.size() / 2);
        long long count_diamond_even = count_plots(grid, start, grid.size() / 2 - 1);

        ans = nb_tiles * (nb_tiles + 1) * (count_odd + count_even);
        if (nb_tiles % 2 == 0)
            ans += (nb_tiles + 1) * count_diamond_odd - nb_tiles * count_diamond_even;
        else
            ans += (nb_tiles + 1) * count_diamond_even - nb_tiles * count_diamond_odd;
    }

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
