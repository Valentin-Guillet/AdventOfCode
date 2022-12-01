
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>


using Position = std::pair<int, int>;

struct hash_position {
    size_t operator()(const Position& p) const {
        size_t seed = std::hash<int>{}(p.first) + 0x9e3779b9;
        return seed ^ (std::hash<int>{}(p.second) + 0x9e3779b9 + (seed<<6) + (seed>>2));
    }
};

struct compare_position {
    bool operator()(const Position& p1, const Position& p2) const {
        return (p1.second < p2.second) || (p1.second == p2.second && p1.first < p2.first);
    }
};

using Grid = std::unordered_map<Position, char, hash_position>;


class Map {
    public:
        Map(std::string input) {
            grid[{0, 0}] = 'X';

            int n = input.size();
            build(input.substr(1, n-2));
        }

        int get_max_doors() {
            return max_doors;
        }

        int get_long_path() {
            return long_path;
        }

        void display() {
            int min_x = 1000000, max_x = -1000000;
            int min_y = 1000000, max_y = -1000000;

            for (auto pos_char : grid) {
                Position pos = pos_char.first;
                if (pos.first < min_x)
                    min_x = pos.first;

                if (max_x < pos.first)
                    max_x = pos.first;

                if (pos.second < min_y)
                    min_y = pos.second;

                if (max_y < pos.second)
                    max_y = pos.second;
            }

            for (int y=min_y-1; y<=max_y+1; ++y) {
                for (int x=min_x-1; x<=max_x+1; ++x) {
                    if (x == 0 && y == 0) {
                        std::cout << 'X';
                    } else if (grid.find({x, y}) != grid.end()) {
                        std::cout << grid[{x, y}];
                    } else {
                        std::cout << '#';
                    }
                }
                std::cout << std::endl;
            }
        }

    private:
        Grid grid;
        int max_doors = 0;
        int long_path = 0;

        std::string build(std::string path, int x=0, int y=0, int nb_doors=0) {
            int bck_x = x, bck_y = y;
            int bck_nb_doors = nb_doors;

            while (!path.empty()) {
                char letter = path[0];
                path = path.substr(1);

                if (letter == ')') {
                    return path;

                } else if (letter == '|') {
                    if (path[0] == ')')
                        return path.substr(1);

                    x = bck_x;
                    y = bck_y;
                    nb_doors = bck_nb_doors;

                } else if (letter == '(') {
                    path = build(path, x, y, nb_doors);

                } else if (letter == 'N') {
                    if (grid.find({x, y-2}) == grid.end()) {
                        nb_doors++;
                        if (nb_doors >= 1000)
                            long_path++;
                    }
                    grid[{x, y-1}] = '-';
                    grid[{x, y-2}] = '.';
                    y -= 2;

                } else if (letter == 'S') {
                    if (grid.find({x, y+2}) == grid.end()) {
                        nb_doors++;
                        if (nb_doors >= 1000)
                            long_path++;
                    }
                    grid[{x, y+1}] = '-';
                    grid[{x, y+2}] = '.';
                    y += 2;

                } else if (letter == 'W') {
                    if (grid.find({x-2, y}) == grid.end()) {
                        nb_doors++;
                        if (nb_doors >= 1000)
                            long_path++;
                    }
                    grid[{x-1, y}] = '|';
                    grid[{x-2, y}] = '.';
                    x -= 2;

                } else if (letter == 'E') {
                    if (grid.find({x+2, y}) == grid.end()) {
                        nb_doors++;
                        if (nb_doors >= 1000)
                            long_path++;
                    }
                    grid[{x+1, y}] = '|';
                    grid[{x+2, y}] = '.';
                    x += 2;

                } else {
                    std::cout << "ERROR !" << std::endl;
                }

                if (nb_doors > max_doors)
                    max_doors = nb_doors;
            }

            return path;
        }

};

int main()
{
    bool first_half = false;
	int ans = 0;
    std::string input;

    std::ifstream myfile ("input");
	if (myfile.is_open()) {
		getline(myfile, input);

		myfile.close();
	} else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    Map map(input);
    /* map.display(); */

    if (first_half) {
        ans = map.get_max_doors();
    } else {
        ans = map.get_long_path();
    }

    std::cout << "Answer : " << ans << std::endl;
	return 0;
}
