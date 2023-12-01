
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_set>
#include <vector>


struct Position {
    int x;
    int y;

    bool operator==(Position const& other) const {
        return (x == other.x) && (y == other.y);
    }
};

struct hashPosition {
    size_t operator()(Position const& pos) const {
        return 31 * pos.x ^ pos.y;
    }
};

using PosSet = std::unordered_set<Position, hashPosition>;
using Grid = std::vector<std::vector<char>>;
using DepthMap = std::vector<std::vector<int>>;

std::vector<Position> get_neighbors(int height, int width, Position const& pos) {
    std::vector<Position> neighbors;
    if (pos.x > 0)
        neighbors.push_back({pos.x - 1, pos.y});
    if (pos.x < height - 1)
        neighbors.push_back({pos.x + 1, pos.y});
    if (pos.y > 0)
        neighbors.push_back({pos.x, pos.y - 1});
    if (pos.y < width - 1)
        neighbors.push_back({pos.x, pos.y + 1});
    return neighbors;
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

    Grid grid;
    Position start, end;
    if (myfile.is_open()) {
        std::string input;

        std::vector<char> line;
        int x = 0;
        while (getline(myfile, input)) {
            for (int y=0; y<(int)input.size(); y++) {
                if (input[y] == 'S') {
                    start = {x, y};
                    line.push_back('a');
                } else if (input[y] == 'E') {
                    end = {x, y};
                    line.push_back('z');
                } else {
                    line.push_back(input[y]);
                }
            }
            grid.push_back(line);
            line.clear();
            x++;
        }

        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    DepthMap depth(grid.size(), std::vector(grid[0].size(), 0));

    std::queue<Position> positions;
    PosSet seen_positions;

    positions.push(start);
    seen_positions.insert(start);
    if (!first_half) {
        std::vector<Position> starting_positions;
        for (int x=0; x<(int)grid.size(); x++) {
            for (int y=0; y<(int)grid[0].size(); y++) {
                if (grid[x][y] == 'a') {
                    positions.push({x, y});
                    seen_positions.insert({x, y});
                }
            }
        }
    }

    while (ans == 0 && !positions.empty()) {
        Position curr_pos = positions.front();
        positions.pop();

        int curr_depth = depth[curr_pos.x][curr_pos.y];
        char curr_height = grid[curr_pos.x][curr_pos.y];
        for (Position neighbor : get_neighbors(grid.size(), grid[0].size(), curr_pos)) {
            char neigh_height = grid[neighbor.x][neighbor.y];
            if (seen_positions.contains(neighbor) || neigh_height > curr_height + 1)
                continue;

            if (neighbor == end) {
                ans = curr_depth + 1;
                break;
            }

            depth[neighbor.x][neighbor.y] = curr_depth + 1;
            positions.push(neighbor);
            seen_positions.insert(neighbor);
        }
    }

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
