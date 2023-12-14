
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>


enum Direction { NORTH = 0, WEST = 1, EAST = 2, SOUTH = 3 };

using Platform = std::vector<std::string>;

size_t hash(const Platform& platform) {
    std::string all_chars;
    all_chars.reserve(platform[0].size() * platform.size());
    for (const std::string& line : platform)
        all_chars += line;
    return std::hash<std::string>()(all_chars);
}

int compute_load(const Platform& platform) {
    int ans = 0;
    for (size_t i = 0; i < platform.size(); ++i)
        ans += std::ranges::count(platform[i], 'O') * (platform.size() - i);
    return ans;
}

struct PosIterator {
    Direction direction;
    int nb_rows, nb_cols;
    int position;

    PosIterator(Direction dir, const Platform& platform) :
            direction(dir), nb_rows(platform.size()), nb_cols(platform[0].size()) {
        if (direction == SOUTH || direction == EAST)
            position = 0;
        else if (direction == NORTH)
            position = nb_rows - 1;
        else
            position = nb_cols - 1;
    }

    PosIterator operator+(int value) const {
        PosIterator other(*this);
        other.position += value * ((direction == NORTH || direction == WEST) ? -1 : 1);
        return other;
    }

    PosIterator& operator++() {
        position += ((direction == NORTH || direction == WEST) ? -1 : 1);
        return *this;
    }

    bool must_stop() const {
        return (((direction == NORTH || direction == WEST) && position == -1)
                 || (direction == SOUTH && position == nb_rows)
                 || (direction == EAST && position == nb_cols));
    }
};

char& access(Platform& platform, const PosIterator& pos, size_t index) {
    if (pos.direction == NORTH || pos.direction == SOUTH)
        return platform[pos.position][index];
    return platform[index][pos.position];
}

void roll(Platform& platform, Direction direction) {
    size_t max_cross_position = ((direction == NORTH || direction == SOUTH) ? platform[0].size() : platform.size());
    for (size_t index = 0; index < max_cross_position; ++index) {

        int empty_spaces = 0;
        Direction opp = Direction(3 - direction);
        for (PosIterator pos_it(opp, platform); !pos_it.must_stop(); ++pos_it) {
            char& c = access(platform, pos_it, index);
            if (c == '.') {
                empty_spaces++;
            } else if (c == '#') {
                empty_spaces = 0;
            } else if (empty_spaces > 0) {
                c = '.';
                access(platform, pos_it + (-empty_spaces), index) = 'O';
            }
        }
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

    Platform platform;
    if (myfile.is_open()) {
        std::istream_iterator<std::string> file_stream(myfile), end;
        platform.assign(file_stream, end);
        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    if (first_half) {
        roll(platform, NORTH);
        ans = compute_load(platform);

    } else {
        const long long nb_cycles = 1000000000;
        std::unordered_map<size_t, size_t> hashes;
        std::vector<int> loads;
        size_t load_index;
        for (size_t index = 0; ; ++index) {
            roll(platform, NORTH);
            roll(platform, WEST);
            roll(platform, SOUTH);
            roll(platform, EAST);
            loads.push_back(compute_load(platform));

            size_t curr_hash = hash(platform);
            if (hashes.contains(curr_hash)) {
                size_t loop_length = (hashes.size() - hashes[curr_hash]);
                load_index = hashes[curr_hash] + (nb_cycles - hashes[curr_hash]) % loop_length - 1;
                break;
            }
            hashes[curr_hash] = index;
        }

        ans = loads[load_index];
    }

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
