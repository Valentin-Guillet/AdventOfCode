
#include "utils.hpp"

size_t hash_position::operator()(const Position& p) const {
    size_t seed = std::hash<int>{}(p.first) + 0x9e3779b9;
    return seed ^ (std::hash<int>{}(p.second) + 0x9e3779b9 + (seed<<6) + (seed>>2));
}

bool operator<(const std::pair<int, int>& p1, const std::pair<int, int>& p2) {
    return (p1.second < p2.second) || (p1.second == p2.second && p1.first < p2.first);
}



int distance(const Position& p1, const Position& p2) {
    return std::abs(p1.first - p2.first) + std::abs(p1.second - p2.second);
}

bool is_valid(const Grid& grid, const Position& pos) {
    return (grid.find(pos) != grid.end() && grid.at(pos) == '.');
}

std::vector<Position> get_neighbours(const Grid& grid, const Position& pos) {
    std::vector<Position> neighbours;
    // North
    if (pos.second > 0 && grid.at({pos.first, pos.second-1}) == '.')
        neighbours.push_back({pos.first, pos.second-1});

    // West
    if (pos.first > 0 && grid.at({pos.first-1, pos.second}) == '.')
        neighbours.push_back({pos.first-1, pos.second});

    // East
    if (is_valid(grid, {pos.first+1, pos.second}))
        neighbours.push_back({pos.first+1, pos.second});

    // South
    if (is_valid(grid, {pos.first, pos.second+1}))
        neighbours.push_back({pos.first, pos.second+1});

    return neighbours;
}

Position get_next_pos(const Grid& grid, const Position& pos, std::vector<Position>& targets) {
    std::vector<PosParent> positions = {};
    std::unordered_set<Position, hash_position> seen;
    seen.insert(pos);

    for (Position neigh : get_neighbours(grid, pos)) {
        positions.push_back(PosParent(neigh, neigh));
        seen.insert(neigh);
    }

    while (!positions.empty()) {
        std::vector<PosParent> next_positions;

        for (PosParent next_pos : positions) {
            if (std::find(targets.begin(), targets.end(), next_pos.pos) != targets.end())
                return next_pos.parent;
        }

        for (PosParent curr_pos : positions) {
            for (Position neigh : get_neighbours(grid, curr_pos.pos)) {
                if (seen.find(neigh) == seen.end()) {
                    seen.insert(neigh);
                    next_positions.push_back(PosParent(neigh, curr_pos.parent));
                }
            }
        }

        std::sort(next_positions.begin(), next_positions.end(),
                  [](const PosParent& p1, const PosParent& p2) { return p1.parent < p2.parent; });
        positions = next_positions;
    }

    return pos;
}

SquareSorter::SquareSorter(Position pos) : pos(pos) { }

bool SquareSorter::operator()(const Position& p1, const Position& p2) {
    if (distance(pos, p1) < distance(pos, p2))
        return true;

    return (p1 < p2);
}
