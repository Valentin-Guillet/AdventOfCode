
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <functional>
#include <algorithm>


int count_adjacent_neighbors(std::vector<std::string> &grid, int x, int y) {
    int ans = 0;
    for (int j=-1; j<2; ++j) {
        for (int i=-1; i<2; ++i) {
            if ((i != 0 || j != 0) &&
                    0 <= x+i && x+i < (int) grid[0].size() &&
                    0 <= y+j && y+j < (int) grid.size() &&
                    grid[y+j][x+i] == '#')
                ans++;
        }
    }
    return ans;
}

int count_directions_neighbors(std::vector<std::string> &grid, int x, int y) {
    int ans = 0;
    for (int j=-1; j<2; ++j) {
        for (int i=-1; i<2; ++i) {
            if (i == 0 && j == 0)
                continue;

            int dx = i;
            int dy = j;
            while (0 <= x+dx && x+dx < (int) grid[0].size() &&
                    0 <= y+dy && y+dy < (int) grid.size() &&
                    grid[y+dy][x+dx] == '.') {
                dx += i;
                dy += j;

            }
            if (0 <= x+dx && x+dx < (int) grid[0].size() &&
                    0 <= y+dy && y+dy < (int) grid.size() &&
                    grid[y+dy][x+dx] == '#')
                ans++;
        }
    }
    return ans;
}

void next(std::vector<std::string> &grid, bool first_half) {
    std::vector<std::string> copy_grid(grid);
    for (size_t y=0; y<grid.size(); ++y) {
        for (size_t x=0; x<grid[0].size(); ++x) {

            int occ_neighbours;
            if (first_half)
                occ_neighbours = count_adjacent_neighbors(copy_grid, x, y);
            else
                occ_neighbours = count_directions_neighbors(copy_grid, x, y);

            if (grid[y][x] == 'L' && occ_neighbours == 0)
                grid[y][x] = '#';
            else if (grid[y][x] == '#' && occ_neighbours > 3+(1-first_half))
                grid[y][x] = 'L';
        }
    }
}

size_t hash_grid(std::vector<std::string> const& grid) {
    std::string s;
    for (std::string const& row : grid)
        s += row;
    return std::hash<std::string>{}(s);
}

int count_occupied(std::vector<std::string> const& grid) {
    int ans = 0;
    for (std::string const& row : grid)
        ans += std::count(row.begin(), row.end(), '#');
    return ans;
}

void debug(std::vector<std::string> const& grid) {
    for (std::string const& row : grid)
        std::cout << row << std::endl;
    std::cout << std::endl;
}

int main()
{
    bool first_half = false;
    int ans = 0;
    std::vector<std::string> grid;

    std::ifstream myfile ("input");
    if (myfile.is_open()) {
        std::string input;

        while (getline(myfile, input))
            grid.push_back(input);

        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    std::unordered_set<size_t> seen;
    size_t grid_hash = hash_grid(grid);
    do {
        seen.insert(grid_hash);
        next(grid, first_half);
        grid_hash = hash_grid(grid);
    } while(seen.find(grid_hash) == seen.end());

    ans = count_occupied(grid);

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
