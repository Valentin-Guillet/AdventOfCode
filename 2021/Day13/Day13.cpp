
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>


struct Point {
    int x;
    int y;

    bool operator==(Point const& other) const {
        return x == other.x && y == other.y;
    }
};

struct PointHash {
    inline std::size_t operator()(Point const& p) const { return p.x + 31*p.y; }
};

using PointList = std::unordered_set<Point, PointHash>;

void fold(PointList& points, int value, bool vertical) {
    PointList new_points;
    for (Point p : points) {
        if (vertical && p.x > value)
            new_points.insert({2*value - p.x, p.y});
        else if (!vertical && p.y > value)
            new_points.insert({p.x, 2*value - p.y});
        else
            new_points.insert(p);
    }
    points = new_points;
}

void display(PointList const& points) {
    int max_x = 0, max_y = 0;
    for (Point p : points) {
        max_x = std::max(max_x, p.x);
        max_y = std::max(max_y, p.y);
    }

    for (int y=0; y<max_y+1; y++) {
        for (int x=0; x<max_x+1; x++)
            std::cout << (points.contains({x, y}) ? '#' : ' ');
        std::cout << std::endl;
    }
}

int main(int argc, char* argv[])
{
    bool first_half = true;
	int ans = 0;
    std::vector<std::string> instructions;
    PointList points;

    std::ifstream myfile("Day13Input");
    for (int i=1; i<argc; i++) {
        std::string arg(argv[i]);
        if (arg == "-t" || arg == "--test")
            myfile = std::ifstream("testInput");

        else if (arg == "-s" || arg == "--second")
            first_half = false;
    }

	if (myfile.is_open()) {
        std::string input;

        while (getline(myfile, input)) {
            if (input.empty())
                break;

            int index = input.find(',');
            int x = std::stoi(input.substr(0, index));
            int y = std::stoi(input.substr(index+1));
            points.insert(Point{x, y});
        }

        while (getline(myfile, input))
            instructions.push_back(input);


		myfile.close();
	} else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    if (first_half) {
        int value = std::stoi(instructions[0].substr(13));
        fold(points, value, instructions[0][11] == 'x');

        ans = points.size();

    } else {
        for (std::string instruction : instructions) {
            int value = std::stoi(instruction.substr(13));
            fold(points, value, instruction[11] == 'x');
        }

        display(points);
    }

    std::cout << "Answer : " << ans << std::endl;
	return 0;
}
