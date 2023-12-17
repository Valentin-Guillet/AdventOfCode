
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <string>
#include <unordered_map>
#include <vector>


enum Direction {
    NORTH = 1 << 3,
    EAST = 1 << 2,
    SOUTH = 1 << 1,
    WEST = 1 << 0
};

Direction get_opposite(Direction dir) {
    switch (dir) {
        case NORTH: return SOUTH;
        case EAST: return WEST;
        case SOUTH: return NORTH;
        case WEST: return EAST;
        default:__builtin_unreachable();
    }
}

enum class Status { Valid, Invalid, Split };

using Grid = std::vector<std::string>;

struct Beam {
    Direction direction;
    int row, col;

    Status advance(const Grid& grid) {
        if (direction == NORTH)
            row--;
        else if (direction == WEST)
            col--;
        else if (direction == SOUTH)
            row++;
        else
            col++;

        if (!(0 <= row && row < (int)grid.size() && 0 <= col && col < (int)grid[0].size()))
            return Status::Invalid;

        char cell = grid[row][col];
        if (cell == '-' && (direction == NORTH || direction == SOUTH)) {
            direction = EAST;
            return Status::Split;
        }

        if (cell == '|' && (direction == WEST || direction == EAST)) {
            direction = NORTH;
            return Status::Split;
        }

        if (cell == '/' || cell == '\\') {
            if (direction == NORTH)
                direction = EAST;
            else if (direction == WEST)
                direction = SOUTH;
            else if (direction == SOUTH)
                direction = WEST;
            else if (direction == EAST)
                direction = NORTH;
            if (cell == '\\')
                direction = get_opposite(direction);
            return Status::Valid;
        }

        return Status::Valid;
    }

    Beam split() const {
        Beam new_beam(*this);
        new_beam.direction = get_opposite(direction);
        return new_beam;
    }

    size_t get_hash() const {
        return (row << 16) | col;
    }
};

std::vector<Beam> get_starting_beams(const Grid& grid) {
    std::vector<Beam> starting_beams;
    starting_beams.reserve(2 * (grid.size() + grid[0].size()));
    for (size_t col = 0; col < grid[0].size(); ++col) {
        starting_beams.emplace_back(SOUTH, -1, col);
        starting_beams.emplace_back(NORTH, grid.size(), col);
    }
    for (size_t row = 0; row < grid[0].size(); ++row) {
        starting_beams.emplace_back(EAST, row, -1);
        starting_beams.emplace_back(WEST, row, grid[0].size());
    }
    return starting_beams;
}

int count_energized_tiles(const Grid& grid, Beam starting_beam) {
    std::unordered_map<size_t, size_t> energized_tiles;
    std::list<Beam> beams = {starting_beam};
    while (!beams.empty()) {
        for (auto beam_it = beams.begin(); beam_it != beams.end(); ) {
            size_t coord_hash = beam_it->get_hash();
            if (energized_tiles[coord_hash] & beam_it->direction) {
                beam_it = beams.erase(beam_it);
                continue;
            }

            energized_tiles[coord_hash] |= beam_it->direction;
            Status status = beam_it->advance(grid);
            if (status == Status::Invalid) {
                beam_it = beams.erase(beam_it);
            } else if (status == Status::Split) {
                beams.insert(beam_it, beam_it->split());
                ++beam_it;
            } else {
                ++beam_it;
            }
        }
    }

    // Remove 1 to take into account the starting point outside of the grid
    return energized_tiles.size() - 1;
}

int main(int argc, char* argv[])
{
    bool first_half = true;
    int ans = 0;

    std::ifstream myfile("input");
    for (int i = 1; i < argc; ++i) {
        std::string arg(argv[i]);
        if (arg == "-t" || arg == "--test")
            myfile = std::ifstream("test_input");

        else if (arg == "-s" || arg == "--second")
            first_half = false;
    }

    Grid grid;
    if (myfile.is_open()) {
        std::istream_iterator<std::string> file_stream(myfile), end;
        grid.assign(file_stream, end);
        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    if (first_half) {
        ans = count_energized_tiles(grid, {EAST, 0, -1});
    } else {
        for (Beam& beam : get_starting_beams(grid)) {
            int nb_energized_tiles = count_energized_tiles(grid, beam);
            if (nb_energized_tiles > ans)
                ans = nb_energized_tiles;
        }
    }

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
