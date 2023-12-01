
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>


struct Point {
    int x, y;
    int id;

    Point(int x, int y, int id) : x(x), y(y), id(id) { }
};


std::vector<int> get_infinite(std::vector<Point> points) {
    std::vector<int> idx;

    for (Point point : points) {
        bool edges_blocked[4] = {false, false, false, false};

        // North side
        for (Point other : points) {
            if (other.id == point.id)
                continue;

            // North blocked
            if (other.y < point.y && std::abs(other.x - point.x) <= std::abs(other.y - point.y))
                edges_blocked[0] = true;

            // South blocked
            if (point.y < other.y && std::abs(other.x - point.x) <= std::abs(other.y - point.y))
                edges_blocked[1] = true;

            // West blocked
            if (other.x < point.x && std::abs(other.y - point.y) <= std::abs(other.x - point.x))
                edges_blocked[2] = true;

            // East blocked
            if (point.x < other.x && std::abs(other.y - point.y) <= std::abs(other.x - point.x))
                edges_blocked[3] = true;
        }

        if (!edges_blocked[0] || !edges_blocked[1] || !edges_blocked[2] || !edges_blocked[3])
            idx.push_back(point.id);
    }

    return idx;
}

int get_area(const std::vector<Point> points, size_t id) {
    int area = 0;
    for (int i=0; i<500; ++i) {
        for (int j=0; j<500; ++j) {
            bool closest = true;
            int dist = std::abs(points[id].x - i) + std::abs(points[id].y - j);
            for (size_t id_point=0; id_point<points.size(); ++id_point) {
                if (id_point == id)
                    continue;

                int point_dist = std::abs(points[id_point].x - i) + std::abs(points[id_point].y - j);
                if (point_dist <= dist) {
                    closest = false;
                    break;
                }
            }
            area += closest;
        }
    }
    return area;
}

int main(void)
{
    bool first_half = false;
    int ans = 0;
    std::vector<Point> points;

    std::ifstream myfile ("input");
    if (myfile.is_open()) {
        std::string input;

        int i = 0;
        while (getline(myfile, input)) {
            int id = input.find(',');
            int x = std::stoi(input.substr(0, id));
            int y = std::stoi(input.substr(id+2));
            points.push_back({x, y, i++});
        }

        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    if (first_half) {
        std::vector<int> idx_hull = get_infinite(points);

        for (size_t i=0; i<points.size(); ++i) {
            if (find(idx_hull.begin(), idx_hull.end(), points[i].id) != idx_hull.end())
                continue;

            int area = get_area(points, i);
            if (ans < area)
                ans = area;
        }
    } else {
        for (int i=0; i<500; ++i) {
            for (int j=0; j<500; ++j) {
                int dist = 0;
                for (Point point : points)
                    dist += std::abs(point.x - i) + std::abs(point.y - j);
                if (dist < 10000)
                    ans++;
            }
        }
    }

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
