
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


using Grid = std::vector<std::vector<int>>;
using FlashedGrid = std::vector<std::vector<bool>>;

int update(Grid& grid, FlashedGrid& flashed, int x, int y) {
    if (flashed[x][y])
        return 0;

    grid[x][y]++;
    if (grid[x][y] < 10)
        return 0;
    else
        grid[x][y] = 0;

    // Flash
    int count = 1;
    flashed[x][y] = true;
    for (int dx=-1; dx<=1; dx++) {
        for (int dy=-1; dy<=1; dy++) {
            if (dx == 0 && dy == 0)
                continue;
            if (!(0 <= x+dx && x+dx < (int)grid.size() && 0 <= y+dy && y+dy < (int)grid[0].size()))
                continue;

            count += update(grid, flashed, x+dx, y+dy);
        }
    }
    return count;
}

void debug(Grid const& grid) {
    for (size_t x=0; x<grid.size(); x++) {
        for (size_t y=0; y<grid[0].size(); y++)
            std::cout << grid[x][y];
        std::cout << std::endl;
    }
}

int main(int argc, char* argv[])
{
    bool first_half = true;
	int ans = 0;
    Grid grid;

    std::ifstream myfile("Day11Input");
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

    bool finished = false;
    int n_days = 0;
    int prev_ans = 0;
    while (!finished) {
        FlashedGrid flashed(grid.size(), std::vector<bool>(grid[0].size()));
        for (int x=0; x<(int)grid.size(); x++) {
            for (int y=0; y<(int)grid[0].size(); y++)
                ans += update(grid, flashed, x, y);
        }

        if (first_half) {
            finished = (++n_days == 100);
        } else {
            finished = ((ans - prev_ans) == ((int)grid.size() * (int)grid[0].size()));
            ans = ++n_days;
        }
        prev_ans = ans;
    }

    std::cout << "Answer : " << ans << std::endl;
	return 0;
}
