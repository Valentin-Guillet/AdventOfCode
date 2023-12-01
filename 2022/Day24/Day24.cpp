
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_set>
#include <vector>


struct Position {
    int x;
    int y;
    int step;

    bool operator==(Position const& other) const {
        return (x == other.x) && (y == other.y) && (step == other.step);
    }
};

struct hashPosition {
    size_t operator()(Position const& pos) const {
        size_t hash = pos.x + 0x9e3779b9;
        hash = (hash << 6) + (hash >> 2) + pos.y + 0x9e3779b9;
        hash = (hash << 6) + (hash >> 2) + pos.step + 0x9e3779b9;
        return hash;
    }
};

using Map = std::vector<std::string>;

bool is_free(Map const& map, Position const& position) {
    int size_x = map.size();
    int size_y = map[0].size();

    // Up
    int x = (position.x - position.step) % size_x;
    if (x < 0)
        x += size_x;
    if (map.at(x).at(position.y) == 'v')
        return false;

    // Down
    x = (position.x + position.step) % size_x;
    if (map.at(x).at(position.y) == '^')
        return false;

    // Left
    int y = (position.y - position.step) % size_y;
    if (y < 0)
        y += size_y;
    if (map.at(position.x).at(y) == '>')
        return false;

    // Right
    y = (position.y + position.step) % size_y;
    if (map.at(position.x).at(y) == '<')
        return false;

    return true;
}

std::vector<Position> get_neighbors(Map const& map, Position curr_pos) {
    std::vector<Position> neighbors;
    int size_x = map.size();
    int size_y = map[0].size();

    Position next_pos = curr_pos;
    next_pos.step++;

    if (curr_pos.x == -1) {
        // On first line: can only wait or go down
        neighbors.push_back(Position{-1, 0, curr_pos.step + 1});
        next_pos.x = 0;
        next_pos.y = 0;
        if (is_free(map, next_pos))
            neighbors.push_back(next_pos);
        return neighbors;

    } else if (curr_pos.x == size_x) {
        // On last line: can only wait or go up
        neighbors.push_back(Position{size_x, size_y - 1, curr_pos.step + 1});
        next_pos.x = size_x - 1;
        next_pos.y = size_y - 1;
        if (is_free(map, next_pos))
            neighbors.push_back(next_pos);
        return neighbors;
    }

    // Wait
    if (is_free(map, next_pos))
        neighbors.push_back(next_pos);

    // Up
    next_pos.x--;
    if (curr_pos.x > 0 && is_free(map, next_pos))
        neighbors.push_back(next_pos);

    // Down
    next_pos.x += 2;
    if (curr_pos.x < size_x - 1 && is_free(map, next_pos))
        neighbors.push_back(next_pos);

    // Left
    next_pos.x--;
    next_pos.y--;
    if (curr_pos.y > 0 && is_free(map, next_pos))
        neighbors.push_back(next_pos);

    // Right
    next_pos.y += 2;
    if (curr_pos.y < size_y - 1 && is_free(map, next_pos))
        neighbors.push_back(next_pos);

    return neighbors;
}


int shortest_path(Map const& map, Position source, Position target) {
    std::queue<Position> positions;
    std::unordered_set<Position, hashPosition> seen;

    positions.push(source);
    while (!positions.empty()) {
        Position curr_pos = positions.front();
        positions.pop();

        if (seen.contains(curr_pos))
            continue;
        seen.insert(curr_pos);

        for (Position neigh : get_neighbors(map, curr_pos)) {
            if (neigh.x == target.x && neigh.y == target.y)
                return neigh.step - source.step;

            positions.push(neigh);
        }
    }

    return -1;
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

    std::vector<std::string> map;
    if (myfile.is_open()) {
        std::string input;

        getline(myfile, input);
        while (getline(myfile, input))
            map.push_back(input.substr(1, input.size()-2));
        map.erase(map.end());

        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    int size_x = map.size();
    int size_y = map[0].size();

    Position source{-1, 0, ans};
    Position target{size_x - 1, size_y - 1, -1};
    int nb_steps = shortest_path(map, source, target) + 1;
    ans += nb_steps;

    if (!first_half) {
        source = Position{size_x, size_y - 1, ans};
        target = Position{0, 0, -1};
        nb_steps = shortest_path(map, source, target) + 1;
        ans += nb_steps;

        source = Position{-1, 0, ans};
        target = Position{size_x - 1, size_y - 1, -1};
        nb_steps = shortest_path(map, source, target) + 1;
        ans += nb_steps;
    }

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
