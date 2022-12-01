
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


enum Cucumber { NONE, EAST, SOUTH };

using Grid = std::vector<std::vector<Cucumber>>;

bool next(Grid& grid) {
    bool moved = false;
    for (size_t x=0; x<grid.size(); x++) {
        bool should_wrap = (grid[x][grid[0].size()-1] == EAST && grid[x][0] == NONE);
        for (size_t y=0; y<grid[0].size()-1; y++) {
            size_t ny = (y+1) % grid[0].size();
            if (grid[x][y] == EAST && grid[x][ny] == NONE) {
                grid[x][y] = NONE;
                grid[x][ny] = EAST;
                moved = true;
                y++;
            }
        }

        if (should_wrap) {
            grid[x][grid[0].size()-1] = NONE;
            grid[x][0] = EAST;
            moved = true;
        }
    }

    for (size_t y=0; y<grid[0].size(); y++) {
        bool should_wrap = (grid[grid.size()-1][y] == SOUTH && grid[0][y] == NONE);
        for (size_t x=0; x<grid.size()-1; x++) {
            size_t nx = (x+1) % grid.size();
            if (grid[x][y] == SOUTH && grid[nx][y] == NONE) {
                grid[x][y] = NONE;
                grid[nx][y] = SOUTH;
                moved = true;
                x++;
            }
        }

        if (should_wrap) {
            grid[grid.size()-1][y] = NONE;
            grid[0][y] = SOUTH;
            moved = true;
        }
    }

    return moved;
}

void debug(Grid const& grid) {
    for (size_t x=0; x<grid.size(); x++) {
        for (size_t y=0; y<grid[0].size(); y++) {
            if (grid[x][y] == NONE) std::cout << '.';
            else if (grid[x][y] == EAST) std::cout << '>';
            else if (grid[x][y] == SOUTH) std::cout << 'v';
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

int main(int argc, char* argv[])
{
	int ans = 0;
    Grid grid;

    std::ifstream myfile("input");
    for (int i=1; i<argc; i++) {
        std::string arg(argv[i]);
        if (arg == "-t" || arg == "--test")
            myfile = std::ifstream("test_input");
    }

	if (myfile.is_open()) {
        std::string input;

        while (getline(myfile, input)) {
            std::vector<Cucumber> line;
            for (char c : input) {
                if (c == '.')
                    line.push_back(NONE);
                else if (c == '>')
                    line.push_back(EAST);
                else if (c == 'v')
                    line.push_back(SOUTH);
            }
            grid.push_back(line);
        }

		myfile.close();
	} else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    do {
        ans++;
    } while (next(grid));

    std::cout << "Answer : " << ans << std::endl;
	return 0;
}
