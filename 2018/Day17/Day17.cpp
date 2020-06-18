
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>


using Position = std::pair<int, int>;

struct hash_position {
    size_t operator()(const Position& p) const {
        size_t seed = std::hash<int>{}(p.first) + 0x9e3779b9;
        return seed ^ (std::hash<int>{}(p.second) + 0x9e3779b9 + (seed<<6) + (seed>>2));
    }
};

using Grid = std::unordered_map<Position, char, hash_position>;

char find(const Grid& grid, Position pos) {
    if (grid.find(pos) == grid.end()) {
        return '@';
    } else {
        return grid.at(pos);
    }
}

Position find_prev_source(const Grid& grid, Position source) {
    int x_source = -1, y_source = -1;

    // Go left
    auto [x, y] = source;
    while (find(grid, {x-1, y}) != '#') {
        --x;
        if (find(grid, {x, y-1}) == '|') {
            x_source = x;
            y_source = y;
            break;
        }
    }

    // Go right
    x = source.first;
    y = source.second;
    while (x_source < 0 && find(grid, {x+1, y}) != '#') {
        ++x;
        if (find(grid, {x, y-1}) == '|') {
            x_source = x;
            y_source = y;
        }
    }

    while (find(grid, {x_source, y_source-1}) == '|')
        --y_source;

    return {x_source, y_source};
}

std::pair<bool, std::vector<Position>> flow(Grid& grid, Position source, int max_y) {
    auto [x, y] = source;

    if (grid[{x, y}] == '~')
        return {true, {}};

    bool flooded_source = (grid[{x, y+1}] == '~' || grid[{x, y+1}] == '#');

    // Go down
    while (grid[{x, y+1}] != '~' && grid[{x, y+1}] != '#') {
        grid[{x, y+1}] = '|';
        ++y;
        if (y > max_y)
            return {true, {}};
    }

    // Go left
    bool wall_left = true;
    int x_left = x;
    while (wall_left && grid[{x_left-1, y}] != '~' && grid[{x_left-1, y}] != '#') {
        --x_left;
        if (grid[{x_left, y+1}] != '~' && grid[{x_left, y+1}] != '#')
            wall_left = false;
    }

    // Go right
    bool wall_right = true;
    int x_right = x;
    while (wall_right && grid[{x_right+1, y}] != '~' && grid[{x_right+1, y}] != '#') {
        ++x_right;
        if (grid[{x_right, y+1}] != '~' && grid[{x_right, y+1}] != '#')
            wall_right = false;
    }

    bool remove_source;
    std::vector<Position> new_sources;
    if (wall_left && wall_right) {
        for (int xi=x_left; xi<=x_right; ++xi)
            grid[{xi, y}] = '~';
        if (flooded_source) {
            remove_source = true;
            new_sources.push_back(find_prev_source(grid, source));
        } else {
            remove_source = false;
        }
    } else {
        for (int xi=x_left; xi<=x_right; ++xi)
            grid[{xi, y}] = '|';

        if (!wall_left)
            new_sources.push_back({x_left, y});

        if (!wall_right)
            new_sources.push_back({x_right, y});

        remove_source = true;
    }

    return {remove_source, new_sources};
}

int count(const Grid& grid, int min_x, int max_x, int min_y, int max_y, bool first_half) {
    int ans = 0;

    for (int y=min_y; y<=max_y; ++y) {
        for (int x=min_x-1; x<=max_x+1; ++x) {
            char c = find(grid, {x, y});
            if (c == '~' || (first_half && c == '|'))
                ++ans;
        }
    }
    return ans;
}

void debug(const Grid& grid, const std::vector<Position> sources, int min_x, int max_x, int max_y) {
    for (int y=0; y<=max_y+1; ++y) {
        for (int x=min_x-1; x<=max_x+1; ++x) {
            Position pos = {x, y};
            if (std::find(sources.begin(), sources.end(), pos) != sources.end()) {
                std::cout << '+';
            } else if (grid.find(pos) != grid.end()) {
                std::cout << grid.at(pos);
            } else {
                std::cout << '.';
            }
        }
        std::cout << std::endl;
    }
}


int main()
{
    bool first_half = false;
	int ans = 0;

    Grid grid;
    grid[{500, 0}] = '|';
    std::vector<Position> sources = {{500, 0}};
    int min_x = 10000, max_x = 0;
    int min_y = 10000, max_y = 0;

    std::ifstream myfile ("Day17Input");
	if (myfile.is_open()) {
        std::string input;

        while (getline(myfile, input)) {
            bool fix_x = (input[0] == 'x');
            int id_comma = input.find(',');
            int fix_value = std::stoi(input.substr(2, id_comma-2));
            input = input.substr(id_comma+4);

            int id_dot = input.find('.');
            int min_value = std::stoi(input.substr(0, id_dot));
            int max_value = std::stoi(input.substr(id_dot+2));

            if (fix_x) {
                for (int y=min_value; y<=max_value; ++y)
                    grid[{fix_value, y}] = '#';

                if (fix_value < min_x)
                    min_x = fix_value;
                if (max_x < fix_value)
                    max_x = fix_value;

                if (min_value < min_y)
                    min_y = min_value;
                if (max_y < max_value)
                    max_y = max_value;

            } else {
                for (int x=min_value; x<=max_value; ++x)
                    grid[{x, fix_value}] = '#';

                if (fix_value < min_y)
                    min_y = fix_value;
                if (max_y < fix_value)
                    max_y = fix_value;

                if (min_value < min_x)
                    min_x = min_value;
                if (max_x < max_value)
                    max_x = max_value;
            }

        }

		myfile.close();
	} else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    while (!sources.empty()) {
        std::vector<Position> new_sources_vector;
        for (auto it=sources.begin(); it!=sources.end(); ) {
            auto [remove_source, new_sources] = flow(grid, *it, max_y);
            for (Position new_source : new_sources)
                new_sources_vector.push_back(new_source);

            if (remove_source) {
                it = sources.erase(it);
            } else {
                ++it;
            }
        }
        for (Position new_source : new_sources_vector)
            sources.push_back(new_source);
    }

    /* debug(grid, sources, min_x, max_x, max_y); */
    ans = count(grid, min_x, max_x, min_y, max_y, first_half);

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
