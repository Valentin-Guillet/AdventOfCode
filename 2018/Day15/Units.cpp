
#include "Units.hpp"


Unit::Unit(Grid& grid, std::vector<Unit*>& units, Position pos, int pwr, bool elf) :
    grid(grid), units(units), pos(pos), pwr(pwr), elf(elf) {
        hp = 200;
    }

bool Unit::is_elf() {
    return elf;
}

int Unit::get_hp() {
    return hp;
}

bool Unit::has_enemies() {
    return !get_enemies().empty();
}

bool Unit::step() {
    std::vector<Unit*> enemies = get_enemies();
    std::vector<Unit*> adjacent_enemies = get_adjacent(enemies);

    if (adjacent_enemies.empty()) {
         if (move(enemies))
             adjacent_enemies = get_adjacent(enemies);
    }

    bool killed = false;
    if (!adjacent_enemies.empty())
        killed = attack(adjacent_enemies);

    return killed;
}

bool Unit::operator<(const Unit& other) {
    return pos < other.pos;
}


bool Unit::is_adjacent(const Unit& other) {
    return distance(pos, other.pos) == 1;
}

std::vector<Unit*> Unit::get_enemies() {
    std::vector<Unit*> enemies;
    for (Unit* unit : units) {
        if (unit->elf != elf)
            enemies.push_back(unit);
    }
    return enemies;
}

std::vector<Unit*> Unit::get_adjacent(std::vector<Unit*> enemies) {
    std::vector<Unit*> adjacents;
    for (Unit* enemy : enemies) {
        if (is_adjacent(*enemy))
            adjacents.push_back(enemy);
    }
    return adjacents;
}

std::vector<Position> Unit::get_open_squares(std::vector<Unit*> targets) {
    std::vector<Position> open_squares;
    for (Unit* target : targets) {
        for (Position neigh : get_neighbours(grid, target->pos))
            open_squares.push_back(neigh);
    }
    return open_squares;
}

bool Unit::move(std::vector<Unit*> enemies) {
    std::vector<Position> open_squares = get_open_squares(enemies);

    Position next_pos = get_next_pos(grid, pos, open_squares);

    // No reachable enemy
    if (next_pos == pos)
        return false;

    int dist = distance(pos, next_pos);

    // Move on that step and update grid
    grid[pos] = '.';
    grid[next_pos] = (elf ? 'E' : 'G');
    pos = next_pos;

    // Return true if unit is now adjacent to an enemy
    return (dist == 1);
}

/* Return true if a unit has been killed and that unit was before the current unit in the vector
 * (so that we increment pointer only if this output is false) */
bool Unit::attack(std::vector<Unit*> enemies) {
    Unit* lowest_enemy = *std::min_element(
                            enemies.begin(), enemies.end(),
                            [](Unit* u1, Unit* u2) {
                                int h1 = u1->get_hp(), h2 = u2->get_hp();
                                return h1 < h2 || (h1 == h2 && *u1 < *u2);
                            }
            );

    lowest_enemy->hp -= pwr;

    bool before = false;
    if (lowest_enemy->hp <= 0) {
        before = true;
        // Enemy dead ! Remove him from units...
        for (auto it=units.begin(); it!=units.end(); ++it) {
            if (*it == this)
                before = false;

            if ((*it)->pos == lowest_enemy->pos) {
                units.erase(it);
                break;
            }
        }

        // ...and from the grid...
        grid[lowest_enemy->pos] = '.';
        
        // ...and finally, free it
        delete lowest_enemy;
    }

    return before;
}
