
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


using Map = std::vector<std::string>;

struct Position {
    int x;
    int y;
    int direction;

    void step() {
        if (direction == 0)
            y++;

        else if (direction == 1)
            x++;

        else if (direction == 2)
            y--;

        else if (direction == 3)
            x--;
    }

    void step(int size_x, int size_y) {
        step();
        x = (x + size_x) % size_x;
        y = (y + size_y) % size_y;
    }
};

/* Mapping the cube coordinates by hand:

  ab
 fXXc
 gXd
fXXc
aXe
 b

a: [-1,   50:100] -> [y + 100,   0]  d=0 //  [150:200,  -1] -> [0,   x - 100] d=1
b: [-1,  100:150] -> [199, y - 100]  d=3 //  [200,    0:50] -> [0,   y + 100] d=1
c: [0:50,    150] -> [149 - x,  99]  d=2 //  [100:150, 100] -> [149 - x, 149] d=2
d: [50,  100:150] -> [y - 50,   99]  d=2 //  [50:100,  100] -> [49,   x + 50] d=3
e: [150,  50:100] -> [y + 100,  49]  d=2 //  [150:200,  50] -> [149, x - 100] d=3
f: [100:150,  -1] -> [149 - x,  50]  d=0 //  [0:50,     49] -> [149 - x,   0] d=0
g: [99,     0:50] -> [y + 50,   50]  d=0 //  [50:100,   49] -> [100,  x - 50] d=1
*/

void move_on_cube(Position& position) {
    // Face a
    if (position.x == -1 && 50 <= position.y && position.y < 100 && position.direction == 3) {
        position.x = position.y + 100;
        position.y = 0;
        position.direction = 0;
        return;
    }

    if (150 <= position.x && position.x < 200 && position.y == -1 && position.direction == 2) {
        position.y = position.x - 100;
        position.x = 0;
        position.direction = 1;
        return;
    }

    // Face b
    if (position.x == -1 && 100 <= position.y && position.y < 150 && position.direction == 3) {
        position.x = 199;
        position.y = position.y - 100;
        position.direction = 3;
        return;
    }

    if (position.x == 200 && 0 <= position.y && position.y < 50 && position.direction == 1) {
        position.x = 0;
        position.y = position.y + 100;
        position.direction = 1;
        return;
    }

    // Face c
    if (0 <= position.x && position.x < 50 && position.y == 150 && position.direction == 0) {
        position.x = 149 - position.x;
        position.y = 99;
        position.direction = 2;
        return;
    }

    if (100 <= position.x && position.x < 150 && position.y == 100 && position.direction == 0) {
        position.x = 149 - position.x;
        position.y = 149;
        position.direction = 2;
        return;
    }

    // Face d
    if (position.x == 50 && 100 <= position.y && position.y < 150 && position.direction == 1) {
        position.x = position.y - 50;
        position.y = 99;
        position.direction = 2;
        return;
    }

    if (50 <= position.x && position.x < 100 && position.y == 100 && position.direction == 0) {
        position.y = position.x + 50;
        position.x = 49;
        position.direction = 3;
        return;
    }

    // Face e
    if (position.x == 150 && 50 <= position.y && position.y < 100 && position.direction == 1) {
        position.x = position.y + 100;
        position.y = 49;
        position.direction = 2;
        return;
    }

    if (150 <= position.x && position.x < 200 && position.y == 50 && position.direction == 0) {
        position.y = position.x - 100;
        position.x = 149;
        position.direction = 3;
        return;
    }

    // Face f
    if (100 <= position.x && position.x < 150 && position.y == -1 && position.direction == 2) {
        position.x = 149 - position.x;
        position.y = 50;
        position.direction = 0;
        return;
    }

    if (0 <= position.x && position.x < 50 && position.y == 49 && position.direction == 2) {
        position.x = 149 - position.x;
        position.y = 0;
        position.direction = 0;
        return;
    }

    // Face g
    if (position.x == 99 && 0 <= position.y && position.y < 50 && position.direction == 3) {
        position.x = position.y + 50;
        position.y = 50;
        position.direction = 0;
        return;
    }

    if (50 <= position.x && position.x < 100 && position.y == 49 && position.direction == 2) {
        position.y = position.x - 50;
        position.x = 100;
        position.direction = 1;
        return;
    }
}

bool move(Map const& map, Position& position, bool is_cube) {
    Position new_position(position);
    int size_x = map.size();
    int size_y = map.at(0).size();

    if (is_cube) {
        new_position.step();
        if (new_position.x < 0 || size_x <= new_position.x ||
                new_position.y < 0 || size_y <= new_position.y ||
                map.at(new_position.x).at(new_position.y) == ' ') {

            move_on_cube(new_position);
        }

    } else {
        do
            new_position.step(size_x, size_y);
        while (map.at(new_position.x).at(new_position.y) == ' ');
    }

    if (map.at(new_position.x).at(new_position.y) == '#')
        return false;

    position = new_position;

    return true;
}

void follow_path(Map const& map, std::string path, Position& position, bool is_cube) {
    while (!path.empty()) {

        if (path[0] == 'L' || path[0] == 'R') {
            position.direction = (position.direction + (path[0] == 'L' ? 3 : 1)) % 4;
            path = path.substr(1);
            continue;
        }

        size_t index = std::min(path.find('L'), path.find('R'));
        int value = std::stoi(path.substr(0, index));

        for (int i=0; i<value; i++) {
            if (!move(map, position, is_cube))
                break;
        }

        if (index == std::string::npos)
            path.clear();
        else
            path = path.substr(index);
    }
}


int main(int argc, char* argv[])
{
    bool first_half = true;
    int ans = 0;

    std::ifstream myfile("input");
    for (int i=1; i<argc; i++) {
        std::string arg(argv[i]);
        if (arg == "-t" || arg == "--test")
            myfile = std::ifstream("test_input");

        else if (arg == "-s" || arg == "--second")
            first_half = false;
    }

    std::string path;
    std::vector<std::string> lines;
    size_t max_size = 0;
    if (myfile.is_open()) {
        std::string input;

        while(getline(myfile, input)) {
            if (input.empty())
                break;

            if (input.size() > max_size)
                max_size = input.size();
            lines.push_back(input);
        }

        getline(myfile, path);

        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    Map map;
    for (std::string line : lines) {
        while (line.size() < max_size)
            line.push_back(' ');

        map.push_back(line);
    }

    Position position{0, 0, 0};
    while (map[position.x][position.y] == ' ')
        position.y++;

    follow_path(map, path, position, !first_half);
    ans = 1000 * (position.x + 1) + 4 * (position.y + 1) + position.direction;

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
