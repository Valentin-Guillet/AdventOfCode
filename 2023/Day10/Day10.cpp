
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

enum Direction {
    NORTH = 1 << 3,
    EAST = 1 << 2,
    SOUTH = 1 << 1,
    WEST = 1 << 0
};

struct Cell {
    int dir;
    bool in_loop;
};

using Map = std::vector<std::vector<Cell>>;

std::vector<Cell> read_pipes(const std::string& input) {
    std::vector<Cell> pipes;
    for (char c : input) {
        int pipe_dir = 0;
        if (c == 'L')
            pipe_dir = NORTH | EAST;
        else if (c == '|')
            pipe_dir = NORTH | SOUTH;
        else if (c == 'J')
            pipe_dir = NORTH | WEST;
        else if (c == '-')
            pipe_dir = EAST | WEST;
        else if (c == 'F')
            pipe_dir = SOUTH | EAST;
        else if (c == '7')
            pipe_dir = SOUTH | WEST;
        pipes.push_back({pipe_dir, false});
    }
    return pipes;
}

struct Pos {
    size_t row, col;
};

void set_start_direction(Map& map, Pos start) {
    if (start.row > 0 && (map[start.row - 1][start.col].dir & SOUTH))
        map[start.row][start.col].dir |= NORTH;
    if (start.col < map[0].size() && (map[start.row][start.col + 1].dir & WEST))
        map[start.row][start.col].dir |= EAST;
    if (start.row < map.size() && (map[start.row + 1][start.col].dir & NORTH))
        map[start.row][start.col].dir |= SOUTH;
    if (start.col > 0 && (map[start.row][start.col - 1].dir & EAST))
        map[start.row][start.col].dir |= WEST;
}

std::vector<Pos> get_neighbors(const Map& map, Pos pos) {
    std::vector<Pos> neighbors;
    int dir = map[pos.row][pos.col].dir;
    if (pos.row > 0 && (dir & NORTH) && !map[pos.row - 1][pos.col].in_loop)
        neighbors.push_back({pos.row - 1, pos.col});
    if (pos.col < map[0].size() && (dir & EAST) && !map[pos.row][pos.col + 1].in_loop)
        neighbors.push_back({pos.row, pos.col + 1});
    if (pos.row < map.size() && (dir & SOUTH) && !map[pos.row + 1][pos.col].in_loop)
        neighbors.push_back({pos.row + 1, pos.col});
    if (pos.col > 0 && (dir & WEST) && !map[pos.row][pos.col - 1].in_loop)
        neighbors.push_back({pos.row, pos.col - 1});

    return neighbors;
}

int mark_map(Map& map, Pos start) {
    int max_depth = -1;
    std::vector<Pos> curr_positions = {start};
    while (!curr_positions.empty()) {
        std::vector<Pos> next_positions;
        for (Pos pos : curr_positions) {
            for (Pos neigh : get_neighbors(map, pos)) {
                map[neigh.row][neigh.col].in_loop = true;
                next_positions.push_back(neigh);
            }
        }
        curr_positions = next_positions;
        max_depth++;
    }

    return max_depth;
}

int count_inner_cells(const Map& map) {
    // For each row, we keep track of the number of times we cross a pipe that
    // goes both NORTH and SOUTH. The difficulty is that the crossing can be
    // achieved in several pipes, but a line of several pipes does NOT necessarily
    // mean that we crossed, e.g. the sequence `F--J` switches inside,
    // but `F--7` does not.
    // To keep track of this, we XOR the direction of every pipe we encounter,
    // and when the result contains both NORTH and SOUTH, this means we crossed
    int count = 0;
    for (size_t row = 0; row < map.size(); ++row) {
        bool inside = false;
        int curr_dir = 0;
        for (size_t col = 0; col < map[0].size(); ++col) {
            if (!map[row][col].in_loop) {
                if (inside)
                    count++;
                continue;
            }

            curr_dir ^= map[row][col].dir;
            if ((curr_dir & NORTH) && (curr_dir & SOUTH)) {
                inside = !inside;
                curr_dir = 0;
            }
        }
    }
    return count;
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

    Map map;
    Pos start;
    if (myfile.is_open()) {
        size_t row = 0;
        for (std::string input; getline(myfile, input); ++row) {
            size_t start_col = input.find('S');
            if (start_col != std::string::npos)
                start = {row, start_col};
            map.emplace_back(read_pipes(input));
        }

        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    set_start_direction(map, start);
    ans = mark_map(map, start);

    if (!first_half)
        ans = count_inner_cells(map);

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
