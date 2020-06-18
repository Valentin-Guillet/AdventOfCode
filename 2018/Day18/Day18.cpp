
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>


using Grid = std::vector<std::string>;

int count(const Grid& grid, int x, int y, char value) {
    int ans = 0;

    for (int j=-1; j<=1; ++j) {
        for (int i=-1; i<=1; ++i) {
            if (i == 0 && j == 0)
                continue;

            if (0 <= x+i && x+i < 50 && 0 <= y+j && y+j < 50 && grid.at(y+j)[x+i] == value)
                ans++;
        }
    }

    return ans;
}

Grid next(const Grid& grid) {
    Grid new_grid;

    for (int y=0; y<50; ++y) {
        std::string line;
        for (int x=0; x<50; ++x) {
            if (grid.at(y)[x] == '.') {
                line += (count(grid, x, y, '|') > 2 ? '|' : '.');
            } else if (grid.at(y)[x] == '|') {
                line += (count(grid, x, y, '#') > 2 ? '#' : '|');
            } else if (grid.at(y)[x] == '#') {
                line += (count(grid, x, y, '#') > 0 && count(grid, x, y, '|') > 0 ? '#' : '.');
            }
        }
        new_grid.push_back(line);
    }

    return new_grid;
}

int get_total(const Grid& grid) {
    int trees = 0, lumberyards = 0;

    for (int y=0; y<50; ++y) {
        for (int x=0; x<50; ++x) {
            if (grid.at(y)[x] == '|')
                trees++;

            if (grid.at(y)[x] == '#')
                lumberyards++;
        }
    }

    return trees * lumberyards;
}

int main()
{
    bool first_half = false;
	int ans = 0;

    Grid grid;

    std::ifstream myfile ("Day18Input");
	if (myfile.is_open()) {
        std::string input;

        while (getline(myfile, input))
            grid.push_back(input);

		myfile.close();
	} else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    if (first_half) {
        for (int step=0; step<10; ++step)
            grid = next(grid);

    } else {
        std::map<Grid, int> seen;

        // We look for a repeating pattern
        int i = 0;
        do {
            seen[grid] = i++;
            grid = next(grid);

        } while (seen.find(grid) == seen.end());

        // The pattern started at the seen[grid]-th minute and finish now (i.e. i-th minute)
        int len_cycle = i - seen[grid];
        
        // We already applied i step, so we need to substract them to the target number
        long n = 1000000000 - i;

        // No need to apply every cycle, a modulo is enough
        int nb_steps = n % len_cycle;

        for (int step=0; step<nb_steps; ++step)
            grid = next(grid);
    }

    ans = get_total(grid);

    std::cout << "Answer : " << ans << std::endl;
	return 0;
}
