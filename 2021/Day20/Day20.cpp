
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>


struct Point {
    int x;
    int y;

};

bool operator==(Point const& p1, Point const& p2) {
    return (p1.x == p2.x) && (p1.y == p2.y);
}

struct PointHasher {
    size_t operator()(Point const& p) const {
        return p.x + 31 * p.y;
    }
};

using Grid = std::unordered_set<Point, PointHasher>;

void get_min_max(Grid const& grid, int& min_x, int& max_x, int& min_y, int& max_y) {
    min_x = 10000;
    min_y = 10000;
    max_x = -10000;
    max_y = -10000;
    for (Point p : grid) {
        if (p.x < min_x) min_x = p.x;
        if (p.x > max_x) max_x = p.x;
        if (p.y < min_y) min_y = p.y;
        if (p.y > max_y) max_y = p.y;
    }
}

std::vector<Point> get_neighbours(Point const p) {
    std::vector<Point> neighbours;
    for (int dx=-1; dx<=1; dx++) {
        for (int dy=-1; dy<=1; dy++)
            neighbours.push_back({p.x+dx, p.y+dy});
    }
    return neighbours;
}

bool add_pixel(int nb_steps, std::string const algorithm, Grid const& grid, Point const point, int x_min, int x_max, int y_min, int y_max) {
    size_t index = 0;
    for (Point neigh : get_neighbours(point)) {
        index <<= 1;
        if (algorithm[0] == '.' || nb_steps % 2 == 0 || (x_min <= neigh.x && neigh.x <= x_max && y_min <= neigh.y && neigh.y <= y_max))
            index += grid.contains(neigh);
        else
            index += 1;
    }
    return (algorithm[index] == '#');
}

void compute_next(int nb_steps, std::string const algorithm, Grid& grid) {
    Grid new_grid;
    int x_min, x_max, y_min, y_max;
    get_min_max(grid, x_min, x_max, y_min, y_max);

    for (int x=x_min-1; x<=x_max+1; x++) {
        for (int y=y_min-1; y<=y_max+1; y++) {
            if (add_pixel(nb_steps, algorithm, grid, {x, y}, x_min, x_max, y_min, y_max))
                new_grid.insert({x, y});
        }
    }
    grid = new_grid;
}

void debug(Grid const& grid) {
    int x_min, x_max, y_min, y_max;
    get_min_max(grid, x_min, x_max, y_min, y_max);

    for (int x=x_min; x<=x_max; x++) {
        for (int y=y_min; y<=y_max; y++)
            std::cout << (grid.contains({x, y}) ? '#' : '.');
        std::cout << std::endl;
    }
}

int main(int argc, char* argv[])
{
    bool first_half = true;
	int ans = 0;
    Grid grid;
    std::string algorithm;

    std::ifstream myfile("input");
    for (int i=1; i<argc; i++) {
        std::string arg(argv[i]);
        if (arg == "-t" || arg == "--test")
            myfile = std::ifstream("test_input");

        else if (arg == "-s" || arg == "--second")
            first_half = false;
    }

	if (myfile.is_open()) {
        std::string input;

		getline(myfile, algorithm);
		getline(myfile, input);
        int x = 0;
        while (getline(myfile, input)) {
            int y = 0;
            for (char c : input) {
                if (c == '#')
                    grid.insert({x, y});
                y++;
            }
            x++;
        }

		myfile.close();
	} else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    int nb_steps = (first_half ? 2 : 50);
    for (int i=0; i<nb_steps; i++)
        compute_next(i, algorithm, grid);
    ans = grid.size();

    std::cout << "Answer : " << ans << std::endl;
	return 0;
}
