
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>


struct Point {
    int x, y, z, t;

    Point(int x, int y, int z, int t) : x(x), y(y), z(z), t(t) { }

    static int distance(const Point& a, const Point& b) {
        return std::abs(a.x - b.x) + std::abs(a.y - b.y) + std::abs(a.z - b.z) + std::abs(a.t - b.t);
    }
};

struct Constellation {
    std::vector<Point> stars;

    std::vector<Point> grows(std::vector<Point> points) {
        std::vector<Point> remaining_points;

        for (Point p : points) {
            bool in_constellation = false;
            for (Point s : stars) {
                if (Point::distance(p, s) <= 3) {
                    in_constellation = true;
                    break;
                }
            }
            if (in_constellation) {
                stars.push_back(p);
            } else {
                remaining_points.push_back(p);
            }
        }

        return remaining_points;
    }
};


int main()
{
    std::vector<Point> stars;
	int ans = 0;

    std::ifstream myfile ("input");
	if (myfile.is_open()) {
        std::string input;

        while (getline(myfile, input)) {
            std::string token;
            std::stringstream stream(input);
            getline(stream, token, ',');
            int x = std::stoi(token);
            getline(stream, token, ',');
            int y = std::stoi(token);
            getline(stream, token, ',');
            int z = std::stoi(token);
            getline(stream, token, ',');
            int t = std::stoi(token);
            stars.push_back(Point(x, y, z, t));
        }

		myfile.close();
	} else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    std::vector<Constellation> constellations;
    while (!stars.empty()) {
        Constellation new_constellation;
        new_constellation.stars.push_back(stars.back());
        stars.pop_back();

        size_t prev_size;
        do {
            prev_size = stars.size();
            stars = new_constellation.grows(stars);
        } while (stars.size() != prev_size);

        constellations.push_back(new_constellation);
    }

    ans = constellations.size();

    std::cout << "Answer : " << ans << std::endl;
	return 0;
}
