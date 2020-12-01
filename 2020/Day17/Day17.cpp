
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>


struct Point {
    int x;
    int y;
    int z;
    int w;

    bool first_half;
};

bool operator==(Point const& p1, Point const& p2) {
    return (p1.x == p2.x) && (p1.y == p2.y) && (p1.z == p2.z) && (p1.w == p2.w);
}

struct hash_position {
    size_t operator()(Point const& p) const {
        size_t h = std::hash<int>{}(p.x) + 0x9e3779b9;
        h = h ^ (std::hash<int>{}(p.y) + 0x9e3779b9 + (h<<6) + (h>>2));
        h = h ^ (std::hash<int>{}(p.z) + 0x9e3779b9 + (h<<6) + (h>>2));
        return h ^ (std::hash<int>{}(p.w) + 0x9e3779b9 + (h<<6) + (h>>2));
    }
};

using Grid = std::unordered_set<Point, hash_position>;

std::vector<Point> get_neighbors(Point p) {
    bool first_half = p.first_half;
    std::vector<Point> neighbors;
    int w_min, w_max;
    if (first_half) {
        w_min = -1;
        w_max = 2;
    } else {
        w_min = 0;
        w_max = 1;
    }
    for (int dw=w_min; dw<w_max; ++dw) {
        for (int dz=-1; dz<2; ++dz) {
            for (int dy=-1; dy<2; ++dy) {
                for (int dx=-1; dx<2; ++dx) {
                    if (dx != 0 || dy != 0 || dz != 0 || dw != 0)
                        neighbors.push_back(Point({p.x+dx, p.y+dy, p.z+dz, p.w+dw, first_half}));
                }
            }
        }
    }
    return neighbors;
}

int count_neighbors(Grid const& grid, Point p) {
    int ans = 0;
    for (Point neighbor : get_neighbors(p)){
        if (grid.find(neighbor) != grid.end())
            ans++;
    }
    return ans;
}

Grid next(Grid const& grid) {
    Grid next_grid;
    Grid seen(grid);

    for (Point point : grid) {
        int nb_neighbors = count_neighbors(grid, point);
        if (2 <= nb_neighbors && nb_neighbors <= 3)
            next_grid.insert(point);

        for (Point neighbor : get_neighbors(point)) {
            if (seen.find(neighbor) == seen.end()) {
                seen.insert(neighbor);
                if (count_neighbors(grid, neighbor) == 3)
                    next_grid.insert(neighbor);
            }
        }
    }

    return next_grid;
}

int main()
{
    bool first_half = false;
	int ans = 0;
    Grid grid;

    std::ifstream myfile ("Day17Input");
	if (myfile.is_open()) {
        std::string input;

        int y = 0;
        while (getline(myfile, input)) {
            for (int x=0; x<(int)input.size(); ++x) {
                if (input[x] == '#')
                    grid.insert(Point({x, y, 0, 0, first_half}));
            }
            ++y;
        }

		myfile.close();
	} else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    for (int step=0; step<6; ++step)
        grid = next(grid);

    ans = grid.size();

    std::cout << "Answer : " << ans << std::endl;
	return 0;
}
