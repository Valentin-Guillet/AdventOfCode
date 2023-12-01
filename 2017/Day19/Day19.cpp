
#include "iostream"
#include "fstream"
#include <string>
#include <vector>
#include <cctype>

enum Direction
{
    north, west, east, south
};

void go_to(Direction d, int* x, int* y)
{
    switch (d) {
        case south:
            (*x)++;
            break;
        case north:
            (*x)--;
            break;
        case east:
            (*y)++;
            break;
        case west:
            (*y)--;
            break;
    }
}

int main(void)
{
    std::string input;
    std::string ans = "";
    int ans2 = 1;
    std::vector<std::string> grid;

    std::ifstream myfile ("input");
    if (myfile.is_open())
    {
        while (getline(myfile, input))
            grid.push_back(input);
        myfile.close();
    }
    else std::cout << "Unable to open file";

    int posx = 0, posy = grid[0].find("|");
    Direction dir = south;
    bool end = false;
    go_to(dir, &posx, &posy);

    while (!end) {

        // std::cout << "(" << posx << ", " << posy << ")" << std::endl;

        if (std::isalpha(grid[posx][posy]))
            ans += grid[posx][posy];

        if (grid[posx][posy] == '+') {
            // std::cout << "bef" << dir << ", " << posx << ", " << posy << std::endl;
            for (int i = 0; i < 4; ++i) {
                int xi = posx, yi = posy;
                go_to((Direction) i, &xi, &yi);
                if (i != 3-dir && grid[xi][yi] != ' ') {
                    // std::cout << i << ", " << xi << ", " << yi << ", " << dir << std::endl;
                    dir = (Direction) i;
                    break;
                }
            }
        }

        int xi = posx, yi = posy;
        go_to(dir, &xi, &yi);

        if (grid[xi][yi] != ' ') {
            posx = xi;
            posy = yi;
        } else {
            end = true;
        }
        ans2++;
    }

    std::cout << "Answer : " << ans << std::endl;
    std::cout << "Answer : " << ans2 << std::endl;
    return 0;
}