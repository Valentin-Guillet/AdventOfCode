
#include <cmath>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_set>
#include <vector>


struct Point {
    int x;
    int y;
    int z;

    bool operator==(Point const& other) const {
        return (x == other.x && y == other.y && z == other.z);
    }
};

Point operator+(Point p1, Point p2) {
    return Point{p1.x + p2.x, p1.y + p2.y, p1.z + p2.z};
}

Point operator-(Point p1, Point p2) {
    return Point{p1.x - p2.x, p1.y - p2.y, p1.z - p2.z};
}

int distance(Point point) {
    return std::abs(point.x) + std::abs(point.y) + std::abs(point.z);
}

std::ostream& operator<<(std::ostream& os, Point const& point) {
    os << "(" << point.x << ", " << point.y << ", " << point.z << ")";
    return os;
}

std::vector<Point> rotate(Point point) {
    auto [x, y, z] = point;
    std::vector<Point> rotations = {
        { x,  y,  z}, { x, -y, -z}, { x, -z,  y}, { x,  z, -y},
        {-x, -y,  z}, {-x,  y, -z}, {-x,  z,  y}, {-x, -z, -y},
        { y,  z,  x}, { y, -z, -x}, { y, -x,  z}, { y,  x, -z},
        {-y, -z,  x}, {-y,  z, -x}, {-y,  x,  z}, {-y, -x, -z},
        { z,  x,  y}, { z, -x, -y}, { z, -y,  x}, { z,  y, -x},
        {-z, -x,  y}, {-z,  x, -y}, {-z,  y,  x}, {-z, -y, -x},
    };
    return rotations;
}

struct PointHash {
    size_t operator()(Point const& p) const {
        return (p.x + 31 * p.y) | p.z;
    }
};

using PointVec = std::vector<Point>;
using PointSet = std::unordered_set<Point, PointHash>;

PointSet get_intersection(PointSet d1, PointSet d2) {
    PointSet inter;
    for (Point d : d1) {
        if (d2.contains(d))
            inter.insert(d);
    }
    return inter;
}

class Scanner {
    public:
        Scanner(PointVec points) : m_points(24), m_differences(24) {
            compute_rotations(points);
            compute_differences();
        }

        int intersection_count(Scanner const& other) const {
            size_t max_size_inter = 0;
            for (size_t i=0; i<24; i++) {
                PointSet inter_diff = get_intersection(other.m_differences[0], m_differences[i]);
                if (inter_diff.size() > max_size_inter)
                    max_size_inter = inter_diff.size();
            }
            return max_size_inter;
        }

        void set_base() {
            m_position = {0, 0, 0};
            orientation_index = 0;
        }

        void shift(Scanner const& scanner) {
            PointVec other_points = scanner.get_shifted_points();
            PointSet other_diff = scanner.m_differences[scanner.orientation_index];

            size_t max_size_inter = 0;
            PointSet max_inter;
            for (size_t i=0; i<24; i++) {
                PointSet inter_diff = get_intersection(other_diff, m_differences[i]);
                if (inter_diff.size() > max_size_inter) {
                    max_size_inter = inter_diff.size();
                    max_inter = inter_diff;
                    orientation_index = i;
                }
            }

            PointVec oriented_points = m_points[orientation_index];
            PointSet common_beacons;
            for (size_t i=0; i<m_points[0].size()-1; i++) {
                for (size_t j=i+1; j<m_points[0].size(); j++) {
                    Point diff;
                    if (oriented_points[i].x >= oriented_points[j].x)
                        diff = oriented_points[i] - oriented_points[j];
                    else
                        diff = oriented_points[j] - oriented_points[i];

                    if (max_inter.contains(diff)) {
                        common_beacons.insert(oriented_points[i]);
                        common_beacons.insert(oriented_points[j]);
                    }
                }
            }

            PointVec beacons = std::vector(common_beacons.begin(), common_beacons.end());
            Point p1 = beacons[0];
            Point p2 = beacons[1];
            Point o1, o2;
            for (size_t i=0; i<other_points.size()-1; i++) {
                for (size_t j=i+1; j<other_points.size(); j++) {
                    o1 = other_points[i];
                    o2 = other_points[j];
                    if (o1 - o2 == p1 - p2 || o1 - o2 == p2 - p1)
                        goto end_loop;
                }
            }
end_loop:
            if (p1 - o1 == p2 - o2)
                m_position = o1 - p1;
            else if (p1 - o2 == p2 - o1)
                m_position = o2 - p1;
            else
                std::cout << "ERROR" << std::endl;
        }

        PointVec get_shifted_points() const {
            PointVec shifted_points;
            for (Point point : m_points[orientation_index])
                shifted_points.push_back(point + m_position);
            return shifted_points;
        }

        Point get_position() const {
            return m_position;
        }

    private:
        std::vector<PointVec> m_points;
        std::vector<PointSet> m_differences;
        Point m_position;
        size_t orientation_index;

        void compute_rotations(PointVec const& points) {
            for (Point point : points) {
                std::vector<Point> rotated_points = rotate(point);
                for (size_t i=0; i<24; i++)
                    m_points[i].push_back(rotated_points[i]);
            }
        }

        void compute_differences() {
            for (size_t n=0; n<24; n++) {
                for (size_t i=0; i<m_points[n].size()-1; i++) {
                    for (size_t j=i+1; j<m_points[n].size(); j++) {
                        if (m_points[n][i].x >= m_points[n][j].x)
                            m_differences[n].insert(m_points[n][i] - m_points[n][j]);
                        else
                            m_differences[n].insert(m_points[n][j] - m_points[n][i]);
                    }
                }
            }
        }
};

int main(int argc, char* argv[])
{
    bool first_half = true;
	int ans = 0;
    std::vector<Scanner> scanners;

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

        PointVec curr_points;
        while (getline(myfile, input)) {
            if (input.empty()) {
                scanners.push_back(Scanner(curr_points));
                curr_points.clear();
                continue;
            }
            if (input.substr(0, 3) == "---")
                continue;

            int index1 = input.find(',');
            int index2 = input.rfind(',');
            int x = std::stoi(input.substr(0, index1));
            int y = std::stoi(input.substr(index1+1, index2-index1-1));
            int z = std::stoi(input.substr(index2+1));
            curr_points.push_back(Point{x, y, z});
        }
        scanners.push_back(Scanner(curr_points));

		myfile.close();
	} else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    std::vector<std::vector<size_t>> links(scanners.size());

    for (size_t i=0; i<scanners.size()-1; i++) {
        for (size_t j=i+1; j<scanners.size(); j++) {
            int inter = scanners[i].intersection_count(scanners[j]);
            /* std::cout << "Intersection " << i << " and " << j << ": " << inter << std::endl; */
            if (inter >= 66) {
                /* std::cout << "Beacons " << i << " and " << j << " overlap !" << std::endl; */
                links[i].push_back(j);
                links[j].push_back(i);
            }
        }
    }

    scanners[0].set_base();
    PointSet final_beacons;
    for (Point point : scanners[0].get_shifted_points())
        final_beacons.insert(point);

    std::queue<size_t> queue;
    queue.push(0);

    std::unordered_set<size_t> seen;
    while (!queue.empty()) {
        size_t index = queue.front();
        queue.pop();

        if (seen.contains(index))
            continue;
        seen.insert(index);

        for (Point p : scanners[index].get_shifted_points())
            final_beacons.insert(p);

        for (size_t ind : links[index]) {
            if (!seen.contains(ind)) {
                scanners[ind].shift(scanners[index]);
                queue.push(ind);
            }
        }
    }

    if (first_half) {
        ans = final_beacons.size();
    } else {
        PointVec beacons(final_beacons.begin(), final_beacons.end());
        for (size_t i=0; i<scanners.size()-1; i++) {
            for (size_t j=i+1; j<scanners.size(); j++) {
                int dist = distance(scanners[i].get_position() - scanners[j].get_position());
                if (dist > ans)
                    ans = dist;
            }
        }
    }

    std::cout << "Answer : " << ans << std::endl;
	return 0;
}
