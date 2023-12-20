
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>


using Grid = std::map<int, std::map<int, bool>>;

struct Instruction {
    char dir;
    int value;
    std::string color;

    Instruction(const std::string& input, bool first_half) {
        size_t index = input.find(' ', 2);
        if (first_half) {
            dir = input[0];
            value = std::stoi(input.substr(2, index - 2));
        } else {
            switch (input[input.size() - 2]){
                case '0': dir = 'R'; break;
                case '1': dir = 'D'; break;
                case '2': dir = 'L'; break;
                case '3': dir = 'U'; break;
            }
            value = std::stoi("0x" + input.substr(index+3, 5), nullptr, 16);
        }
    }
};

void fill(Grid& grid, const std::vector<Instruction>& instructions) {
    int row = 0, col = 0;
    for (const Instruction& instr : instructions) {
        if (instr.dir == 'L' || instr.dir == 'R') {
            col += (instr.dir == 'R' ? 1 : -1) * instr.value;
            continue;
        }

        bool go_up = (instr.dir == 'U');
        grid[row][col] = go_up;
        row += (go_up ? -1 : 1) * instr.value;
        grid[row][col] = !go_up;
    }
}

long long count_edge(const std::map<int, bool>& line, const std::set<int>& cols) {
    std::map<int, int> edge;
    for (const auto& [col, go_up] : line)
        edge[col] = (go_up ? 2 : 1);
    for (int col : cols) {
        if (!edge.contains(col))
            edge[col] = 3;
    }

    long long count = 0;
    bool inside = false;
    int start_col;
    int dir_flags = 0;
    for (auto [col, dir] : edge) {
        dir_flags ^= dir;

        if (!inside) {
            start_col = col;
            inside = true;
            continue;
        }

        if (dir_flags == 0) {
            count += (col - start_col + 1);
            inside = false;
        }
    }
    return count;
}

long long count_inside(const std::set<int>& cols) {
    long long count = 0;
    bool inside = false;
    int prev_col;
    for (auto it = cols.begin(); it != cols.end(); ++it) {
        if (inside)
            count += *it - prev_col + 1;
        else
            prev_col = *it;
        inside = !inside;
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

    std::vector<Instruction> instructions;
    if (myfile.is_open()) {
        for (std::string input; getline(myfile, input); )
            instructions.emplace_back(input, first_half);
        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    Grid grid;
    fill(grid, instructions);

    std::set<int> curr_cols;
    int prev_row = 0;
    for (const auto& [row, line] : grid) {
        ans += count_edge(line, curr_cols);
        ans += (row - prev_row) * count_inside(curr_cols);

        for (const auto& [col, go_up] : line) {
            if (go_up)
                curr_cols.erase(col);
            else
                curr_cols.insert(col);
        }
        prev_row = row + 1;
    }

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
