
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>


struct Position {
    int x;
    int y;

    Position operator+(Position const& other) const {
        return {x + other.x, y + other.y};
    }

    bool operator==(Position const& other) const {
        return (x == other.x) && (y == other.y);
    }
};

struct hashPosition {
    size_t operator()(Position const& pos) const {
        return 31 * pos.x ^ pos.y;
    }
};

using Tower = std::unordered_set<Position, hashPosition>;

struct Rock {
    std::vector<Position> positions;
    int height;

    bool check_position(Tower const& tower, Position corner) {
        for (Position pos : positions) {
            Position new_pos = corner + pos;
            if (new_pos.x < 0 || new_pos.x > 6 || new_pos.y < 0 || tower.contains(new_pos))
                return false;
        }
        return true;
    }
};

void build_rocks(std::vector<Rock>& rocks) {
    Rock rock_hbar;
    rock_hbar.positions = {{0, 0}, {1, 0}, {2, 0}, {3, 0}};
    rock_hbar.height = 1;
    rocks.push_back(rock_hbar);

    Rock rock_cross;
    rock_cross.positions = {{1, 0}, {0, 1}, {1, 1}, {2, 1}, {1, 2}};
    rock_cross.height = 3;
    rocks.push_back(rock_cross);

    Rock rock_L;
    rock_L.positions = {{0, 0}, {1, 0}, {2, 0}, {2, 1}, {2, 2}};
    rock_L.height = 3;
    rocks.push_back(rock_L);

    Rock rock_vbar;
    rock_vbar.positions = {{0, 0}, {0, 1}, {0, 2}, {0, 3}};
    rock_vbar.height = 4;
    rocks.push_back(rock_vbar);

    Rock rock_square;
    rock_square.positions = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
    rock_square.height = 2;
    rocks.push_back(rock_square);
}

long drop_rock(Tower& tower, int height, Rock& rock, std::string const& movements, size_t& mov_id) {
    Position corner{2, height};

    while (true) {
        // Left/right movement
        if (movements.at(mov_id++) == '<') {
            if (rock.check_position(tower, {corner.x - 1, corner.y}))
                corner.x--;
        } else {
            if (rock.check_position(tower, {corner.x + 1, corner.y}))
                corner.x++;
        }
        if (mov_id == movements.size())
            mov_id = 0;

        // Drop one or come to rest
        if (rock.check_position(tower, {corner.x, corner.y - 1})) {
            corner.y--;
        } else {
            for (Position pos : rock.positions)
                tower.insert(corner + pos);

            return std::max(height - 3, corner.y + rock.height);
        }
    }
}

long create_rocks(std::vector<Rock> const& rocks, std::string movements, size_t nb_rocks) {
    Tower tower;
    long tower_height = 0;
    long skipped_height = 0;
    bool skipped = false;
    std::unordered_map<size_t, std::pair<int, int>> hash_map;

    size_t rock_id = 0;
    size_t mov_id = 0;
    for (size_t i=0; i<nb_rocks; i++) {
        Rock new_rock(rocks.at(rock_id++));
        tower_height = drop_rock(tower, tower_height + 3, new_rock, movements, mov_id);

        if (rock_id == rocks.size())
            rock_id = 0;

        if (skipped)
            continue;

        size_t hash = (mov_id << 3) + rock_id;
        for (int x=0; x<7; x++) {
            hash <<= 1;
            if (tower.contains({x, (int)tower_height - 1}))
                hash += 1;
        }

        if (!hash_map.contains(hash)) {
            hash_map.insert({hash, {i, tower_height}});
        } else {
            size_t nb_rocks_period = i - hash_map[hash].first;
            long height_period = tower_height - hash_map[hash].second;
            long nb_periods = ((nb_rocks - i - 1) / nb_rocks_period);
            nb_rocks -= nb_rocks_period * nb_periods;
            skipped_height = height_period * nb_periods;
            skipped = true;
        }
    }

    return skipped_height + tower_height;
}


int main(int argc, char* argv[])
{
    bool first_half = true;
	long ans = 0;

    std::ifstream myfile("input");
    for (int i=1; i<argc; i++) {
        std::string arg(argv[i]);
        if (arg == "-t" || arg == "--test")
            myfile = std::ifstream("test_input");

        else if (arg == "-s" || arg == "--second")
            first_half = false;
    }

    std::string movements;
	if (myfile.is_open()) {

		getline(myfile, movements);

		myfile.close();
	} else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    std::vector<Rock> rocks;
    build_rocks(rocks);

    size_t nb_rocks = (first_half ? 2022 : 1000000000000);
    ans = create_rocks(rocks, movements, nb_rocks);

    std::cout << "Answer : " << ans << std::endl;
	return 0;
}
