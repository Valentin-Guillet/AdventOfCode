
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>


struct Point {
    int id;
    int x;
    int y;
};

bool operator==(const Point& p1, const Point& p2) {
    return p1.id == p2.id;
}

struct hash_point {
    size_t operator()(Point const& p) const {
        size_t seed = std::hash<int>{}(p.x) + 0x9e3779b9;
        return seed ^ (std::hash<int>{}(p.y) + 0x9e3779b9 + (seed<<6) + (seed>>2));
    }
};

using Grid = std::unordered_map<Point, std::pair<int, int>, hash_point>;

bool aligned(const Grid& points) {
    unsigned int nb_not_grouped = 0;
    for (const auto& [pos, vel] : points) {
        auto [id, x, y] = pos;

        bool got_neigh = false;
        for (const auto& [pos, vel] : points) {
            if ((pos.x == x+1 && pos.y == y) || (pos.x == x-1 && pos.y == y)
                                             || (pos.x == x && pos.y == y+1)
                                             || (pos.x == x && pos.y == y-1)) {
                got_neigh = true;
                break;
            }

        }

        if (!got_neigh)
            nb_not_grouped++;

        if (nb_not_grouped > (points.size() / 10))
            return false;
    }
    return true;
}

void step(Grid& points) {
    Grid new_points;
    for (auto& [pos, vel] : points) {
        auto [id, x, y] = pos;
        auto [vx, vy] = vel;

        new_points[{id, x+vx, y+vy}] = {vx, vy};
    }

    points = new_points;
}

void display(const Grid& points) {
    int min_x = 1000000, max_x = 0;
    int min_y = 1000000, max_y = 0;
    for (const auto& [pos, vel] : points) {
        if (pos.x < min_x)
            min_x = pos.x;
        if (pos.x > max_x)
            max_x = pos.x;

        if (pos.y < min_y)
            min_y = pos.y;
        if (pos.y > max_y)
            max_y = pos.y;
    }

    for (int j=min_y-5; j<max_y+5; ++j) {
        for (int i=min_x-10; i<max_x+10; ++i) {
            bool found = false;
            for (const auto& [pos, vel] : points) {
                if (pos.x == i && pos.y == j) {
                    found = true;
                    break;
                }
            }
            std::cout << (found ? '#' : '.');
        }
        std::cout << std::endl;
    }

}

int main(void)
{
	int ans = 0;
    Grid points;

	std::ifstream myfile ("Day10Input");
	if (myfile.is_open()) {
        std::string input;

        int id = 0;
		while (getline(myfile, input)) {
            int x = std::stoi(input.substr(10, 6));
            int y = std::stoi(input.substr(18, 6));
            int vx = std::stoi(input.substr(36, 2));
            int vy = std::stoi(input.substr(40, 2));
            points[{id++, x, y}] = {vx, vy};
        }

        myfile.close();
	} else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    while (!aligned(points)) {
        step(points);
        ans++;
    }

    display(points);

	std::cout << "Answer : " << ans << std::endl;
	return 0;
}
