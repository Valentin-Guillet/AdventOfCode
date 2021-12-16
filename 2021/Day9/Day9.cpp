
#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <queue>
#include <string>
#include <utility>
#include <vector>


using Grid = std::vector<std::vector<int>>;

bool is_low_point(Grid const& grid, size_t x, size_t y) {
    int value = grid[x][y];
    if ((x > 0 && grid[x-1][y] <= value) || (x < grid.size()-1 && grid[x+1][y] <= value) ||
        (y > 0 && grid[x][y-1] <= value) || (y < grid[0].size()-1 && grid[x][y+1] <= value))
        return false;

    return true;
}

int get_basin_size(Grid const& grid, size_t x, size_t y) {
    std::vector<std::vector<bool>> seen(grid.size(), std::vector<bool>(grid[0].size()));
    std::queue<std::pair<size_t, size_t>> queue_pos;
    queue_pos.push({x, y});

    int count = 0;
    while (!queue_pos.empty()) {
        auto [i, j] = queue_pos.front();
        queue_pos.pop();
        if (seen[i][j])
            continue;
        count++;
        seen[i][j] = true;

        if (i > 0 && !seen[i-1][j] && grid[i-1][j] < 9)
            queue_pos.push({i-1, j});

        if (i < grid.size()-1 && !seen[i+1][j] && grid[i+1][j] < 9)
            queue_pos.push({i+1, j});

        if (j > 0 && !seen[i][j-1] && grid[i][j-1] < 9)
            queue_pos.push({i, j-1});

        if (j < grid[0].size()-1 && !seen[i][j+1] && grid[i][j+1] < 9)
            queue_pos.push({i, j+1});
    }

    return count;
}

int main(int argc, char* argv[])
{
    bool first_half = true;
	int ans = 0;
    Grid grid;

    std::ifstream myfile("Day9Input");
    for (int i=1; i<argc; i++) {
        std::string arg(argv[i]);
        if (arg == "-t" || arg == "--test")
            myfile = std::ifstream("testInput");

        else if (arg == "-s" || arg == "--second")
            first_half = false;
    }

	if (myfile.is_open()) {
        std::string input;

        while (getline(myfile, input)) {
            std::vector<int> line;
            for (char c : input)
                line.push_back(c - '0');
            grid.push_back(line);
        }

		myfile.close();
	} else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    if (first_half) {
        for (size_t x=0; x<grid.size(); x++) {
            for (size_t y=0; y<grid[0].size(); y++)
                ans += (is_low_point(grid, x, y) ? grid[x][y] + 1 : 0);
        }

    } else {
        std::vector<std::pair<int, int>> basins;
        for (size_t x=0; x<grid.size(); x++) {
            for (size_t y=0; y<grid[0].size(); y++) {
                if (is_low_point(grid, x, y))
                    basins.push_back({x, y});
            }
        }

        std::vector<int> sizes;
        for (auto [x, y] : basins)
            sizes.push_back(get_basin_size(grid, x, y));
        std::sort(sizes.begin(), sizes.end(), std::greater<>());

        ans = sizes[0] * sizes[1] * sizes[2];
    }

    std::cout << "Answer : " << ans << std::endl;
	return 0;
}
