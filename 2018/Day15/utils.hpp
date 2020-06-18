
#ifndef HEADER_UTILS
#define HEADER_UTILS

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>


using Position = std::pair<int, int>;

bool operator<(const std::pair<int, int>& p1, const std::pair<int, int>& p2);

struct hash_position {
    size_t operator()(const Position& p) const;
};

struct PosParent {
    Position pos;
    Position parent;

    PosParent(Position pos, Position parent) : pos(pos), parent(parent) { }
};


int distance(const Position& p1, const Position& p2);


using Grid = std::unordered_map<Position, char, hash_position>;

bool is_valid(const Grid& grid, const Position& pos);

std::vector<Position> get_neighbours(const Grid& grid, const Position& pos);

Position get_next_pos(const Grid& grid, const Position& pos, std::vector<Position>& targets);


struct SquareSorter {
    Position pos;
    SquareSorter(Position pos);

    bool operator()(const Position& p1, const Position& p2);
};

#endif //HEADER_UTILS
