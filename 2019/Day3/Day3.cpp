
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>


using map = std::map<std::pair<int, int>, std::pair<int, int>>;

void print_grid(map& grid)
{
    int min_x = 10000, max_x = -100000;
    int min_y = 10000, max_y = -100000;
    for (std::pair<std::pair<int, int>, std::pair<int, int>> elt : grid) {
        if (elt.first.first < min_x)
            min_x = elt.first.first;
        if (elt.first.first > max_x)
            max_x = elt.first.first;
        if (elt.first.second < min_y)
            min_y = elt.first.second;
        if (elt.first.second > max_y)
            max_y = elt.first.second;
    }

    for (int j = max_y+1; j >= min_y-1; --j) {
        for (int i = min_x-1; i <= max_x+1; ++i) {
            if (i == 0 && j == 0) {
                std::cout << '@';
            } else if (grid[{i, j}].first & 1 && grid[{i, j}].first & 2) {
                std::cout << '+';
            } else if (grid[{i, j}].first & 1) {
                std::cout << '1';
            } else if (grid[{i, j}].first & 2) {
                std::cout << '2';
            } else {
                std::cout << '.';
            }
        }
        std::cout << std::endl;
    }
}

std::vector<std::pair<int, int>> build_wire(std::string wire, map& grid, int id)
{
    std::stringstream stream(wire);
    std::vector<std::string> actions;

    while (stream.good()) {
        std::string substr;
        getline(stream, substr, ',');
        actions.push_back(substr);
    }

    std::pair<int, int> pos = {0, 0};
    ++grid[pos].first;

    std::vector<std::pair<int, int>> intersections;
    int value = 1;

    for (std::string action : actions)
    {
        int x_dir = 0, y_dir = 0;
        switch (action[0])
        {
            case 'U':
                y_dir = 1;
                break;
            case 'D':
                y_dir = -1;
                break;
            case 'R':
                x_dir = 1;
                break;
            case 'L':
                x_dir = -1;
                break;
            default:
                std::cout << "Wire parsing error !" << std::endl;
                exit(1);
                break;
        }

        for (int j = 0; j < std::stoi(action.substr(1)); ++j) {
            pos.first += x_dir;
            pos.second += y_dir;
            if ((grid[pos].first & id) == 0) {
                grid[pos].first ^= id;
                grid[pos].second += value;
            }
            value++;
            if (grid[pos].first & 1 && grid[pos].first & 2)
                intersections.push_back(pos);
        }
    }
    return intersections;
}


int main(void)
{
    bool first_half = false;
    std::string wire1, wire2;
    int ans = 0;
    std::vector<std::pair<int, int>> intersections;
    map grid;

    std::ifstream myfile ("input");
    if (myfile.is_open()) {

        getline(myfile, wire1);
        getline(myfile, wire2);

        myfile.close();
    } else {
        std::cout << "Unable to open file";
        return 1;
    }

    build_wire(wire1, grid, 1);
    intersections = build_wire(wire2, grid, 2);

    // print_grid(grid);

    ans = 1000000000;
    for (std::pair<int, int> pos : intersections) {
        if (first_half) {
            if (abs(pos.first) + abs(pos.second) < ans)
                ans = abs(pos.first) + abs(pos.second);
        } else {
            if (grid[pos].second < ans)
                ans = grid[pos].second;
        }
    }

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
