
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>


int DEPTH = 11541;

struct Position {
    int x;
    int y;

    // Torch = 0, Gear = 1, Neither = 2
    int tool = 0;

    bool operator==(const Position& other) const {
        return other.x == x && other.y == y && other.tool == tool;
    }
};

struct hash_position {
    size_t operator()(const Position& p) const {
        size_t seed = std::hash<int>{}(p.x) + 0x9e3779b9;
        return seed ^ (std::hash<int>{}(p.y) + 0x9e3779b9 + (seed<<6) + (seed>>2));
    }
};

struct PosComp {
    std::unordered_map<Position, int, hash_position>& distances;
    PosComp(std::unordered_map<Position, int, hash_position>& distances) : distances(distances) {}

    bool operator()(Position p1, Position p2) const {
        return distances.at(p1) > distances.at(p2);
    }
};


struct Grid : std::unordered_map<Position, int, hash_position> {
    Position target;
    Grid(Position target) : target(target) { }

    int get(Position position) {
        Position pos = {position.x, position.y, 0};

        if (find(pos) == end()) {
            long value;

            if ((pos.x == 0 && pos.y == 0) || (pos.x == target.x && pos.y == target.y)) {
                value = 0;
            } else if (pos.x == 0) {
                value = pos.y * 48271;
            } else if (pos.y == 0) {
                value = pos.x * 16807;
            } else {
                value = get({pos.x-1, pos.y}) * get({pos.x, pos.y-1});
            }
            int erosion= (value + DEPTH) % 20183;

            (*this)[pos] = erosion;
        }

        return (*this)[pos];
    }

    int get_type(Position position) {
        // Rocky = 0, wet = 1, narrow = 2
        return get({position.x, position.y, 0}) % 3;
    }
};

void display(Grid& erosion) {
    int max_x = 0, max_y = 0;
    for (auto position_val : erosion) {
        Position position = position_val.first;

        if (position.x > max_x)
            max_x = position.x;

        if (position.y > max_y)
            max_y = position.y;
    }

    for (int y=0; y<=max_y; ++y) {
        for (int x=0; x<=max_x; ++x) {
            if (erosion.get_type({x, y}) == 0) {
                std::cout << '.';
            } else if (erosion.get_type({x, y}) == 1) {
                std::cout << '=';
            } else {
                std::cout << '|';
            }
        }
        std::cout << std::endl;
    }
}

int a_star(Grid& erosion, Position target) {
    std::unordered_map<Position, int, hash_position> distances, fdistances;
    distances[{0, 0, 0}] = 0;
    fdistances[{0, 0, 0}] = 0;

    std::priority_queue<Position, std::vector<Position>, PosComp> unvisited{PosComp(fdistances)};
    unvisited.push({0, 0, 0});

    while (!unvisited.empty()) {
        Position curr = unvisited.top();
        unvisited.pop();
        if (curr.x == target.x && curr.y == target.y && curr.tool == target.tool)
            return distances[curr];

        std::vector<Position> neighbours = {{curr.x-1, curr.y, curr.tool}, {curr.x, curr.y-1, curr.tool},
                                            {curr.x+1, curr.y, curr.tool}, {curr.x, curr.y+1, curr.tool}};
        for (Position neigh : neighbours) {
            if (neigh.x < 0 || neigh.y < 0 || (neigh.tool + 1) % 3 == erosion.get_type(neigh))
                continue;

            if (distances.find(neigh) == distances.end() || distances[curr]+1 < distances[neigh]) {
                distances[neigh] = distances[curr] + 1;
                fdistances[neigh] = distances[curr] + 1 + std::abs(neigh.x - target.x) + std::abs(neigh.y - target.y);
                unvisited.push(neigh);
            }
        }

        int curr_type = erosion.get_type(curr);
        int new_tool;
        if (curr_type == 0) {
            new_tool = (curr.tool == 0 ? 1 : 0);
        } else if (curr_type == 1) {
            new_tool = (curr.tool == 1 ? 2 : 1);
        } else if (curr_type == 2) {
            new_tool = (curr.tool == 0 ? 2 : 0);
        }
        Position new_pos = {curr.x, curr.y, new_tool};

        if (distances.find(new_pos) == distances.end() || distances[curr]+7 < distances[new_pos]) {
            distances[new_pos] = distances[curr] + 7;
            fdistances[new_pos] = distances[curr] + 7 + std::abs(new_pos.x - target.x) + std::abs(new_pos.y - target.y);
            unvisited.push(new_pos);
        }
    }

    return 0;
}


int main()
{
    bool first_half = false;
    int ans = 0;

    int x_target = 14;
    int y_target = 778;

    Grid erosion({x_target, y_target});

    if (first_half) {
        for (int y=0; y<=y_target; ++y) {
            for (int x=0; x<=x_target; ++x) {
                ans += erosion.get_type({x, y});
            }
        }

    } else {
        ans = a_star(erosion, {x_target, y_target});
    }

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}

