
#include "iostream"
#include "fstream"
#include <string>
#include <unordered_map>
#include <vector>

// void disp(std::unordered_map<std::string, int> grid) {
    // for (int i = -25; i < 26; ++i)
    // {
        // for (int j = -25; j < 26; ++j)
        // {
            // if (grid.find(std::to_string(i) + ',' + std::to_string(j)) != grid.end())
                // std::cout << "#";
            // else
                // std::cout << ".";
        // }
        // std::cout << std::endl;
    // }
    // std::cout << std::endl;
// }

int main(void)
{
    std::string input;
    int ans = 0;
    std::unordered_map<std::string, int> grid;

    std::ifstream myfile ("input");
    if (myfile.is_open())
    {
        int x = 0;
        while (getline(myfile, input)) {
            for (int y = 0; y < 25; ++y) {
                if (input[y] == '#')
                    grid[std::to_string(x-12) + ',' + std::to_string(y-12)] = 1;
            }
            ++x;
        }
        myfile.close();
    }
    else std::cout << "Unable to open file";

    int posx = 0, posy = 0;
    int direction = 0;
    // disp(grid);

    for (int i = 0; i < 10000000; ++i) {
        std::string hash = std::to_string(posx) + ',' + std::to_string(posy);
        if (grid.find(hash) == grid.end()) {
            direction = ((direction-1) % 4 + 4) % 4;
            grid[hash] = 0;
        } else {
            int value = grid[hash];
            if (value == 0) {
                ans++;
                grid[hash]++;
            } else if (value == 1) {
                grid[hash]++;
                direction = ((direction+1) % 4 + 4) % 4;
            } else {
                direction = ((direction+2) % 4 + 4) % 4;
                grid.erase(hash);
            }
        }
        switch(direction) {
            case 0:posx--; break;
            case 1:posy++; break;
            case 2:posx++; break;
            case 3:posy--; break;
        }
        // disp(grid);
        if (i % 1000000 == 0)
            std::cout << i << std::endl;
    }

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}