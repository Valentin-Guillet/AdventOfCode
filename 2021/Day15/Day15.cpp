
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <unordered_set>
#include <vector>


using Point = std::pair<int, int>;
struct PointHasher {
    inline size_t operator()(Point const& p) const { return p.first + 31 * p.second; }
};
using PointSet = std::unordered_set<Point, PointHasher>;
using Grid = std::vector<std::vector<int>>;

Grid expand_grid(Grid const& grid) {
    Grid new_grid;
    for (size_t x=0; x<5*grid.size(); x++) {
        std::vector<int> new_line;
        for (size_t y=0; y<5*grid[0].size(); y++) {
            int orig_x = x % grid.size();
            int offset_x = x / grid.size();
            int orig_y = y % grid[0].size();
            int offset_y = y / grid[0].size();
            int new_value = grid[orig_x][orig_y] + offset_x + offset_y;
            new_value = ((new_value - 1) % 9) + 1;

            new_line.push_back(new_value);
        }
        new_grid.push_back(new_line);
    }
    return new_grid;
}

PointSet get_neighbors(int m, int n, PointSet const& seen, Point const& p) {
    PointSet neighbors;
    auto [x, y] = p;

    if (0 < x && !seen.contains({x-1, y}))
        neighbors.insert({x-1, y});
    if (x < m-1 && !seen.contains({x+1, y}))
        neighbors.insert({x+1, y});
    if (0 < y && !seen.contains({x, y-1}))
        neighbors.insert({x, y-1});
    if (y < n-1 && !seen.contains({x, y+1}))
        neighbors.insert({x, y+1});

    return neighbors;
}

int dijkstra(Grid const& grid) {
    int m = grid.size();
    int n = grid[0].size();

    Point curr_point = {0, 0};
    Grid dist(m, std::vector<int>(n, 100000));
    dist[0][0] = 0;
    PointSet seen = {};
    PointSet border = {curr_point};

    while (!border.empty()) {
        auto [x, y] = curr_point;
        if (x == m-1 && y == n-1)
            return dist[x][y];

        seen.insert(curr_point);
        border.erase(curr_point);

        for (Point p : get_neighbors(m, n, seen, curr_point)) {
            border.insert(p);
            if (dist[p.first][p.second] > dist[x][y] + grid[p.first][p.second])
                dist[p.first][p.second] = dist[x][y] + grid[p.first][p.second];
        }

        int min_dist = 100000;
        Point min_point;
        for (Point p : border) {
            int curr_dist = dist[p.first][p.second];
            if (curr_dist < min_dist) {
                min_dist = curr_dist;
                min_point = p;
            }
        }

        curr_point = min_point;
    }

    return -1;
}

int main(int argc, char* argv[])
{
    bool first_half = true;
	int ans = 0;
    Grid grid;

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

        while (getline(myfile, input)) {
            std::vector<int> line;
            for (char c : input)
                line.push_back(c - '0');
            grid.push_back(line);
        }

		myfile.close();
	} else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    if (!first_half)
        grid = expand_grid(grid);

    ans = dijkstra(grid);

    std::cout << "Answer : " << ans << std::endl;
	return 0;
}
