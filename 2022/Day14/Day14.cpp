
#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>


struct Position {
    int x;
    int y;

    bool operator==(Position const& other) const {
        return (x == other.x) && (y == other.y);
    }
};

struct hashPosition {
    size_t operator()(Position const& pos) const {
        return 31 * pos.x ^ pos.y;
    }
};

using Map = std::unordered_map<Position, char, hashPosition>;

std::ostream& operator<<(std::ostream& os, Map const& map) {
    int min_x = 1000;
    int max_x = -1000;
    int max_y = 0;
    for (auto [key, value] : map) {
        if (key.x < min_x)
            min_x = key.x;
        if (key.x > max_x)
            max_x = key.x;
        if (key.y > max_y)
            max_y = key.y;
    }

    for (int y=0; y<=max_y+1; y++) {
        for (int x=min_x-1; x<=max_x+1; x++) {
            if (x == 500 && y == 0)
                os << "+";
            else if (map.contains({x, y}))
                os << map.at({x, y});
            else
                os << '.';
        }
        os << "\n";
    }
    os << std::endl;

    return os;
}

void add_path(Map& map, std::string input) {
    std::vector<Position> positions;
    while(true) {
        size_t index_comma = input.find(',');
        size_t index_coords = input.find(' ');
        int x = std::stoi(input.substr(0, index_comma));
        int y = std::stoi(input.substr(index_comma + 1, index_coords - index_comma + 1));
        positions.push_back({x, y});

        if (index_coords == std::string::npos)
            break;
        input = input.substr(index_coords + 4);
    }

    for (size_t ind=0; ind<positions.size()-1; ind++) {
        auto [x1, y1] = positions[ind];
        auto [x2, y2] = positions[ind+1];
        int beg_x = std::min(x1, x2);
        int end_x = std::max(x1, x2);
        int beg_y = std::min(y1, y2);
        int end_y = std::max(y1, y2);

        for (int x=beg_x; x<=end_x; x++) {
            for (int y=beg_y; y<=end_y; y++)
                map[{x, y}] = '#';
        }
    }

}

bool add_sand(Map& map, Position source, int max_y) {
    Position sand(source);
    while (true) {
        // Into the void
        if (sand.y > max_y)
            return false;

        if (!map.contains({sand.x, sand.y+1})) {  // Below
            sand.y++;
        } else if (!map.contains({sand.x-1, sand.y+1})) {  // Left diagonal
            sand.x--;
            sand.y++;
        } else if (!map.contains({sand.x+1, sand.y+1})) {  // Right diagonal
            sand.x++;
            sand.y++;
        } else {  // Resting
            map[sand] = 'o';
            return true;
        }
    }
}

int fill_sand(Map& map, Position source, int max_y) {
    int count_sand = 0;
    map[source] = 'o';

    std::queue<Position> sand_positions;
    sand_positions.push(source);
    while (!sand_positions.empty()) {
        Position curr_sand = sand_positions.front();
        sand_positions.pop();
        count_sand++;

        if (curr_sand.y == max_y + 1) {
            map[{curr_sand.x-1, curr_sand.y+1}] = '#';
            map[{curr_sand.x,   curr_sand.y+1}] = '#';
            map[{curr_sand.x+1, curr_sand.y+1}] = '#';
            continue;
        }

        for (int dx=-1; dx<=1; dx++) {
            Position next_pos{curr_sand.x + dx, curr_sand.y + 1};
            if (!map.contains(next_pos)) {
                map[next_pos] = 'o';
                sand_positions.push(next_pos);
            }
        }
    }
    return count_sand;
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

    Map map;
    if (myfile.is_open()) {
        std::string input;

        while (getline(myfile, input))
            add_path(map, input);

        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    int max_y = 0;
    for (auto [position, _] : map)
        max_y = std::max(max_y, position.y);

    Position source{500, 0};
    if (first_half) {
        while (add_sand(map, source, max_y))
            ans++;
    } else {
        ans = fill_sand(map, source, max_y);
    }

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
