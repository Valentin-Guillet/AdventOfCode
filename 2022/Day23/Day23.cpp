
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <utility>
#include <vector>


struct Position {
    int x;
    int y;

    bool operator==(Position const& other) const {
        return (x == other.x) && (y == other.y);
    }
};

struct hashPosition {
    size_t operator()(Position const& pos) const {
        size_t hash = pos.x + 0x9e3779b9;
        return (hash << 6) + (hash >> 2) + pos.y + 0x9e3779b9;
    }
};

using ElfSet = std::unordered_set<Position, hashPosition>;
using ElfMap = std::unordered_map<Position, int, hashPosition>;

bool check_direction(int dir, unsigned int occupied) {
    if (dir == 0)  // North
        return ((occupied & 0b11100000) == 0);

    else if (dir == 1)  // South
        return ((occupied & 0b00000111) == 0);

    else if (dir == 2)  // West
        return ((occupied & 0b10010100) == 0);

    else  // East
        return ((occupied & 0b00101001) == 0);
}

int compute_direction(int base_direction, unsigned int occupied) {
    for (int i=0; i<4; i++) {
        int dir = (base_direction + i) % 4;
        if (check_direction(dir, occupied))
            return dir;
    };

    return -1;
}

Position get_next_position(Position const& position, int dir, int dz=0) {
    int dx = 0, dy = 0;
    if (dir == 0) {  // North
        dx--;
        dy += dz;

    } else if (dir == 1) {  // South
        dx++;
        dy += dz;

    } else if (dir == 2) {  // West
        dx += dz;
        dy--;

    } else if (dir == 3) { // East
        dx += dz;
        dy++;
    }

    return Position{position.x + dx, position.y + dy};
}

void step(ElfSet& active_elves, ElfSet& inactive_elves, int base_direction) {
    ElfMap count_destinations, elf_directions;
    for (auto it=active_elves.begin(); it!=active_elves.end(); ) {
        Position const& pos = *it;
        unsigned int occupied = 0;
        for (int dx=-1; dx<=1; dx++) {
            for (int dy=-1; dy<=1; dy++) {
                if (dx == 0 && dy == 0)
                    continue;

                occupied = (occupied << 1) + (active_elves.contains({pos.x + dx, pos.y + dy}));
            }
        }

        if (occupied == 0) {
            inactive_elves.insert(pos);
            it = active_elves.erase(it);

        } else {
            int dir = compute_direction(base_direction, occupied);
            elf_directions[pos] = dir;
            count_destinations[get_next_position(pos, dir)]++;
            it++;
        }
    }

    std::vector<Position> to_insert;
    for (auto it=active_elves.begin(); it!=active_elves.end(); ) {
        Position const& pos = *it;
        int dir = elf_directions[pos];
        Position dest = get_next_position(pos, dir);

        // More than one elf want to go to the same position -> still active
        if (dir == -1 || count_destinations.at(dest) > 1) {
            it++;
            continue;
        }

        for (int dz=-1; dz<=1; dz++) {
            Position neighbor_pos = get_next_position(dest, dir, dz);
            auto neighbor = inactive_elves.find(neighbor_pos);
            if (neighbor != inactive_elves.end()) {
                to_insert.push_back(*neighbor);
                inactive_elves.erase(neighbor);
            }
        }

        to_insert.push_back(dest);
        it = active_elves.erase(it);
    }

    active_elves.insert(to_insert.begin(), to_insert.end());
}

void get_map_size(ElfSet const& elves, int& min_x, int& min_y, int& max_x, int& max_y) {
    min_x = 1000;
    min_y = 1000;
    max_x = -1000;
    max_y = -1000;
    for (auto& pos : elves) {
        if (pos.x < min_x)
            min_x = pos.x;

        if (pos.y < min_y)
            min_y = pos.y;

        if (pos.x > max_x)
            max_x = pos.x;

        if (pos.y > max_y)
            max_y = pos.y;
    }
}

int count_empty_tiles(ElfSet const& elves) {
    int min_x, min_y, max_x, max_y;
    get_map_size(elves, min_x, min_y, max_x, max_y);

    int total_tiles = (max_y - min_y + 1) * (max_x - min_x + 1);
    return total_tiles - elves.size();
}

std::ostream& operator<<(std::ostream& os, ElfSet const& elves) {
    int min_x, min_y, max_x, max_y;
    get_map_size(elves, min_x, min_y, max_x, max_y);

    for (int x=min_x-1; x<=max_x+1; x++) {
        for (int y=min_y-1; y<=max_y+1; y++) {
            if (x == 0 && y == 0)
                os << "O";
            else
                os << (elves.contains({x, y}) ? '#' : '.');
        }
        os << std::endl;
    }

    return os;
}

void debug(ElfSet const& active, ElfSet const& inactive) {
    int a_min_x, a_min_y, a_max_x, a_max_y;
    get_map_size(active, a_min_x, a_min_y, a_max_x, a_max_y);
    int i_min_x, i_min_y, i_max_x, i_max_y;
    get_map_size(inactive, i_min_x, i_min_y, i_max_x, i_max_y);

    int min_x = std::min(a_min_x, i_min_x);
    int max_x = std::max(a_max_x, i_max_x);
    int min_y = std::min(a_min_y, i_min_y);
    int max_y = std::max(a_max_y, i_max_y);

    for (int x=min_x-1; x<=max_x+1; x++) {
        for (int y=min_y-1; y<=max_y+1; y++) {
            if (x == 0 && y == 0)
                std::cout << "O";
            else if (active.contains({x, y}))
                std::cout << '#';
            else if (inactive.contains({x, y}))
                std::cout << 'X';
            else
                std::cout << '.';
        }
        std::cout << std::endl;
    }
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

    ElfSet elves;
    if (myfile.is_open()) {
        std::string input;

        int x = 0;
        while (getline(myfile, input)) {
            for (int i=0; i<(int)input.size(); i++) {
                if (input[i] == '#')
                    elves.insert({x, i});
            }
            x++;
        }

        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    ElfSet active_elves(elves);
    ElfSet inactive_elves;
    if (first_half) {
        for (int round=0; round<10; round++)
            step(active_elves, inactive_elves, round % 4);

        elves.clear();
        elves.insert(active_elves.begin(), active_elves.end());
        elves.insert(inactive_elves.begin(), inactive_elves.end());

        ans = count_empty_tiles(elves);
    } else {
        while (!active_elves.empty()) {
            step(active_elves, inactive_elves, ans % 4);
            ans++;
        }
    }

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
