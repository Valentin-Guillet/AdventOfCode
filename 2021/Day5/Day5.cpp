
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


struct Line {
    int x_beg, y_beg;
    int x_end, y_end;

    int min_x() const { return std::min(x_beg, x_end); }
    int max_x() const { return std::max(x_beg, x_end); }
    int min_y() const { return std::min(y_beg, y_end); }
    int max_y() const { return std::max(y_beg, y_end); }

    bool is_diag() const { return x_beg != x_end && y_beg != y_end; }
};

using Grid = std::vector<std::vector<int>>;

void update(Grid &grid, Line const& line, bool diagonal) {
    if (line.is_diag()) {
        if (!diagonal)
            return;

        int span = line.max_x() - line.min_x();
        for (int k=0; k<span+1; k++) {
            int y;
            if ((line.x_beg < line.x_end) == (line.y_beg < line.y_end))
                y = line.min_y() + k;
            else
                y = line.max_y() - k;
            grid[line.min_x()+k][y]++;
        }
        return;
    }

    for (int x=line.min_x(); x<line.max_x()+1; x++) {
        for (int y=line.min_y(); y<line.max_y()+1; y++)
            grid[x][y]++;
    }
}

void debug(Grid const& grid) {
    for (auto line : grid) {
        for (int v : line) {
            char c = ((v > 0) ? ('0'+v) : '.');
            std::cout << c;
        }
        std::cout << std::endl;
    }
}


int main(int argc, char* argv[])
{
    bool first_half = true;
    int ans = 0;
    std::vector<Line> lines;

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
            int index = input.find(" -> ");
            std::string beg = input.substr(0, index);
            std::string end = input.substr(index+4);
            index = beg.find(',');
            int x_beg = std::stoi(beg.substr(0, index));
            int y_beg = std::stoi(beg.substr(index+1));
            index = end.find(',');
            int x_end = std::stoi(end.substr(0, index));
            int y_end = std::stoi(end.substr(index+1));
            lines.push_back(Line{y_beg, x_beg, y_end, x_end});
        }

        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    int max_x = 0, max_y = 0;
    for (Line const& line : lines) {
        max_x = std::max(max_x, line.max_x()+1);
        max_y = std::max(max_y, line.max_y()+1);
    }

    Grid grid;
    for (int x=0; x<max_x; x++) {
        std::vector<int> line(max_y);
        grid.push_back(line);
    }

    for (Line const& line : lines)
        update(grid, line, !first_half);

    for (int x=0; x<max_x; x++) {
        for (int y=0; y<max_y; y++) {
            if (grid[x][y] > 1)
                ans++;
        }
    }

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
