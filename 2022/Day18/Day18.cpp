
#include <cmath>
#include <fstream>
#include <iostream>
#include <queue>
#include <map>
#include <string>
#include <unordered_set>
#include <vector>


struct Droplet {
    int x;
    int y;
    int z;
    int face = 6;

    Droplet(int x_, int y_, int z_) : x(x_), y(y_), z(z_) {}
    Droplet(std::string input) {
        size_t index = input.find(',');
        x = std::stoi(input.substr(0, index));
        input = input.substr(index+1);

        index = input.find(',');
        y = std::stoi(input.substr(0, index));
        z = std::stoi(input.substr(index+1));
    }

    bool check(Droplet const& other) const {
        int dist = std::abs(x - other.x) + std::abs(y - other.y) + std::abs(z - other.z);
        return dist == 1;
    }

    std::vector<Droplet> get_neighbors() const {
        std::vector<Droplet> neighbors;
        if (-1 <= x-1)
            neighbors.push_back(Droplet(x-1, y, z));
        if (x+1 <= 22)
            neighbors.push_back(Droplet(x+1, y, z));

        if (-1 <= y-1)
            neighbors.push_back(Droplet(x, y-1, z));
        if (y+1 <= 22)
            neighbors.push_back(Droplet(x, y+1, z));

        if (-1 <= z-1)
            neighbors.push_back(Droplet(x, y, z-1));
        if (z+1 <= 22)
            neighbors.push_back(Droplet(x, y, z+1));

        return neighbors;
    }

    bool operator==(Droplet const& other) const {
        return (x == other.x) && (y == other.y) && (z == other.z);
    }
};

struct DropletHasher {
    size_t operator()(Droplet const& droplet) const {
        size_t hash = droplet.x + 0x9e3779b9;
        hash ^= (hash << 6) + (hash >> 2) + droplet.y + 0x9e3779b9;
        hash ^= (hash << 6) + (hash >> 2) + droplet.z + 0x9e3779b9;
        return hash;
    }
};

std::ostream& operator<<(std::ostream& os, Droplet const& droplet) {
    os << "(" << droplet.x << ", " << droplet.y << ", " << droplet.z << ")";
    return os;
}

using Grid = std::unordered_set<Droplet, DropletHasher>;

int count_all_faces(std::vector<Droplet>& droplets) {
    std::map<int, std::vector<Droplet*>> split;
    for (Droplet& droplet : droplets) {
        int sum = droplet.x + droplet.y + droplet.z;
        split[sum].push_back(&droplet);
    }

    for (auto [key, droplets1] : split) {
        if (!split.contains(key + 1))
            continue;

        std::vector<Droplet*> droplets2 = split[key + 1];
        for (Droplet* droplet1 : droplets1) {
            for (Droplet* droplet2 : droplets2) {
                if (droplet1->check(*droplet2)) {
                    droplet1->face--;
                    droplet2->face--;
                }
            }
        }
    }

    int nb_faces = 0;
    for (Droplet droplet : droplets)
        nb_faces += droplet.face;
    return nb_faces;
}

int count_exterior_faces(std::vector<Droplet>& v_droplets) {
    Grid droplets;
    for (Droplet droplet : v_droplets)
        droplets.insert(droplet);

    std::queue<Droplet> air_cubes;
    Grid seen_cubes;

    Droplet source(1, 1, 1);
    air_cubes.push(source);
    seen_cubes.insert(source);

    int nb_faces = 0;
    while (!air_cubes.empty()) {
        Droplet air_cube = air_cubes.front();
        air_cubes.pop();

        for (Droplet neigh : air_cube.get_neighbors()) {
            if (droplets.contains(neigh)) {
                nb_faces++;
                continue;
            }

            if (!seen_cubes.contains(neigh)) {
                seen_cubes.insert(neigh);
                air_cubes.push(neigh);
            }
        }
    }

    return nb_faces;
}


int main(int argc, char* argv[])
{
    bool first_half = true;
    int ans = 0;

    std::ifstream myfile("input");
    for (int i=1; i<argc; i++) {
        std::string arg(argv[i]);
        if (arg == "-t" || arg == "--test")
            myfile = std::ifstream("test_input");

        else if (arg == "-s" || arg == "--second")
            first_half = false;
    }

    std::vector<Droplet> droplets;
	if (myfile.is_open()) {
        std::string input;

        while (getline(myfile, input))
            droplets.push_back(Droplet(input));

		myfile.close();
	} else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    if (first_half)
        ans = count_all_faces(droplets);
    else
        ans = count_exterior_faces(droplets);

    std::cout << "Answer : " << ans << std::endl;
	return 0;
}
