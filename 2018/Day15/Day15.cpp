
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

#include "Units.hpp"
#include "utils.hpp"


void debug(const Grid& grid, const std::vector<Unit*> units={}) {
    for (int y=0; y<32; ++y) {
        std::vector<int> health;
        for (int x=0; x<32; ++x) {
            std::cout << grid.at({x, y});
            if (grid.at({x, y}) == 'G' || grid.at({x, y}) == 'E') {
                for (Unit* unit : units) {
                    if (unit->pos.first == x && unit->pos.second == y) {
                        health.push_back(unit->get_hp());
                        break;
                    }
                }
            }
        }
        if (!health.empty()) {
            std::cout << "    ";
            for (int hp : health)
                std::cout << hp << " / ";
        }
        std::cout << std::endl;
    }
}

std::pair<int, bool> run_battle(Grid grid, int pwr) {
    int outcome = 0;

    std::vector<Unit*> units;
    unsigned int nb_elf = 0;
    for (auto [pos, c] : grid) {
        if (c == 'E') {
            units.push_back(new Unit(grid, units, pos, pwr, true));
            ++nb_elf;
        } else if (c == 'G') {
            units.push_back(new Unit(grid, units, pos, 3, false));
        }
    }
    std::sort(units.begin(), units.end(), [](Unit* u1, Unit* u2) { return (*u1) < (*u2); });

    int rounds = 0;
    while (true) {
        for (auto unit=units.begin(); unit!=units.end(); ) {
            if (!(*unit)->has_enemies())
                goto break_out;

            // Don't increment ptr if unit killed before this one
            if (!(*unit)->step())
                ++unit;
        }
        std::sort(units.begin(), units.end(), [](Unit* u1, Unit* u2) { return (*u1) < (*u2); });
        rounds++;
    }

break_out:
    for (Unit* unit : units)
        outcome += unit->get_hp();

    outcome *= rounds;

    bool elf_survived = units.front()->is_elf() && units.size() == nb_elf;

    for (Unit* unit : units)
        delete unit;

    return {outcome, elf_survived};
}

int main(void)
{
    bool first_half = false;
    int ans = 0;
    Grid grid;

    std::ifstream myfile ("input");
    if (myfile.is_open()) {
        std::string input;

        int y = 0;
        while (getline(myfile, input)) {
            for (size_t x=0; x<input.size(); ++x)
                grid[{x, y}] = input[x];
            y++;
        }

        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    int pwr = 3;
    bool elf_survived;
    do {
        std::pair<int, bool> ans_survived = run_battle(grid, pwr++);
        ans = ans_survived.first;
        elf_survived = ans_survived.second;

    } while (!first_half && !elf_survived);

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
