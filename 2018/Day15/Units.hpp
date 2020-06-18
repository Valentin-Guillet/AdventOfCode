
#ifndef HEADER_UNITS
#define HEADER_UNITS

#include <algorithm>
#include <vector>

#include "utils.hpp"


class Unit {
    public:
        Unit(Grid& grid, std::vector<Unit*>& units, Position pos, int pwr, bool elf);

        bool is_elf();
        int get_hp();
        bool has_enemies();
        bool step();

        bool operator<(const Unit& other);

    /* private: */
        Grid& grid;
        std::vector<Unit*>& units;
        Position pos;
        int pwr;
        bool elf;
        int hp;

        bool is_adjacent(const Unit& other);

        std::vector<Unit*> get_enemies();
        std::vector<Unit*> get_adjacent(std::vector<Unit*> enemies);
        std::vector<Position> get_open_squares(std::vector<Unit*> targets);

        bool move(std::vector<Unit*> enemies);
        bool attack(std::vector<Unit*> enemies);
};

#endif // HEADER_UNITS
