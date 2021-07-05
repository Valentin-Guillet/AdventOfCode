
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <queue>
#include <vector>
#include <tuple>
#include <set>
#include <map>

using Pos = std::pair<int, int>;

struct Pos3D
{
    Pos pos;
    int depth;
    int length;
    bool linked_start = true;

    std::string disp() {
        std::ostringstream repr;
        repr << "(" << pos.first << ", " << pos.second;
        repr << ", " << depth << ")[" << length << "]";
        if (linked_start)
            repr << "(linked)";
        return repr.str();
    }
};

std::vector<Pos> get_neigh(Pos pos) {
    auto [i, j] = pos;
    return {{i, j-1}, {i+1, j}, {i, j+1}, {i-1, j}};

}

int find(std::vector<Pos3D>& seen, Pos3D state, bool same)
{
    for (Pos3D seen_state : seen) {
        if (seen_state.pos == state.pos && seen_state.depth == state.depth) {
            if ((same && seen_state.linked_start == state.linked_start) ||
                    (!same && seen_state.linked_start != state.linked_start))
                return seen_state.length;
        }
    }
    return -1;
}

class Maze
{
    private:
        bool first_half;
        int sizeX, sizeY;
        std::map<Pos, char> maze;
        std::map<Pos, Pos> warps;

        Pos3D start;
        Pos3D end;


        bool is_inner(Pos pos) {
            bool outer = (pos.first == 2 || pos.first == sizeX-3 || pos.second == 2 || pos.second == sizeY-3);
            return !outer;
        }

        bool update(std::map<Pos, char>& grid, int i, int j) {
            if (grid[{i, j}] != '.')
                return false;

            int ways = 0;
            std::vector<Pos> neighbours = get_neigh({i, j});
            for (Pos neigh : neighbours)
                ways += (grid[neigh] == '#' ? 1 : 0);

            if (ways < 3)
                return false;

            grid[{i, j}] = '#';
            for (Pos neigh : neighbours)
                update(grid, neigh.first, neigh.second);

            return true;
        }

        void fill() {
            bool acted = true;
            std::map<Pos, char> grid(maze);

            while (acted) {
                acted = false;
                for (int j=1; j<sizeY-1; ++j) {
                    for (int i=1; i<sizeX-1; ++i) {
                        if (update(grid, i, j))
                            acted = true;
                    }
                }
            }
            maze = grid;
        }

        std::vector<Pos> process_warp(std::map<std::string, std::vector<Pos>>& named_warps, Pos pos) {
            Pos neigh, right_neigh = {pos.first+1, pos.second};
            Pos neigh_dot;
            if ('A' <= maze[right_neigh] && maze[right_neigh] <= 'Z') {
                neigh = right_neigh;

                if (maze[{pos.first-1, pos.second}] == '.') {
                    neigh_dot = {pos.first-1, pos.second};
                } else {
                    neigh_dot = {pos.first+2, pos.second};
                }

            } else {
                neigh = {pos.first, pos.second+1};

                if (maze[{pos.first, pos.second-1}] == '.') {
                    neigh_dot = {pos.first, pos.second-1};
                } else {
                    neigh_dot = {pos.first, pos.second+2};
                }
            }
            std::string label = {maze[pos], maze[neigh]};

            if (label == "AA") {
                start = {neigh_dot, 0, 0};
            } else if (label == "ZZ") {
                end = {neigh_dot, 0, 0};
            } else {
                named_warps[label].push_back(neigh_dot);
            }

            return {pos, neigh};
        }

        std::vector<Pos3D> get_neighbours(Pos3D state) {
            std::vector<Pos3D> neighbours;
            for (Pos neigh : get_neigh(state.pos)) {
                if (maze[neigh] == '.') {
                    neighbours.push_back({neigh, state.depth, state.length+1});
                } else if ('A' <= maze[neigh] && maze[neigh] <= 'Z') {

                    if (first_half) {
                        neighbours.push_back({warps[state.pos], state.depth, state.length+1});

                    } else {
                        if (state.pos == start.pos || state.pos == end.pos)
                            continue;

                        bool inner = is_inner(state.pos);
                        if (!inner && state.depth == 0)
                            continue;

                        int depth = (inner ? 1 : -1);
                        neighbours.push_back({warps[state.pos], state.depth+depth, state.length+1});
                    }
                }
            }
            return neighbours;
        }

        int step(std::queue<Pos3D>& states, std::vector<Pos3D>& seen) {
            int length = states.front().length;
            std::cout << "Process length " << length << std::endl;
            while (states.front().length == length) {
                Pos3D state = states.front();
                states.pop();

                int end_length = find(seen, state, false);
                if (end_length > 0)
                    return state.length + end_length;

                seen.push_back(state);

                for (Pos3D neigh : get_neighbours(state)) {
                    neigh.linked_start = state.linked_start;
                    if (find(seen, neigh, true) == -1) {
                        states.push(neigh);
                    }
                }
            }
            return -1;
        }

    public:
        Maze(std::vector<std::string> tab, bool first_half) {
            first_half = first_half;
            sizeX = tab[0].size();
            sizeY = tab.size();

            for (int j=0; j<sizeY; ++j) {
                for (int i=0; i<sizeX; ++i) {
                    maze[{i, j}] = tab[j][i];
                }
            }

            std::map<std::string, std::vector<Pos>> named_warps;
            std::set<Pos> seen_letters;
            for (int j=0; j<sizeY; ++j) {
                for (int i=0; i<sizeX; ++i) {
                    if (seen_letters.find({i, j}) == seen_letters.end()
                            && 'A' <= tab[j][i] && tab[j][i] <= 'Z') {
                        for (Pos pos : process_warp(named_warps, {i, j}))
                            seen_letters.insert(pos);
                    }
                }
            }

            for (std::pair<std::string, std::vector<Pos>> warp : named_warps) {
                warps[warp.second[0]] = warp.second[1];
                warps[warp.second[1]] = warp.second[0];
            }
            fill();
        }

        int solve() {
            std::queue<Pos3D> states_start, states_end;
            states_start.push(start);
            states_end.push(end);
            states_end.front().linked_start = false;

            std::vector<Pos3D> seen;

            int length;
            while (true) {
                length = step(states_start, seen);
                if (length > 0)
                    return length;

                length = step(states_end, seen);
                if (length > 0)
                    return length;
            }
        }

        void display() {
            system("clear");
            for (int j=0; j<sizeY; ++j) {
                for (int i=0; i<sizeX; ++i)
                    std::cout << maze[{i, j}];
                std::cout << std::endl;
            }
        }
};

int main(void)
{
    bool first_half = true;
	int ans = 0;
	std::vector<std::string> tab;

	std::ifstream myfile ("Day20Input");
	if (myfile.is_open()) {
        std::string input;

		while (getline(myfile, input))
            tab.push_back(input);

		myfile.close();
	} else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    Maze maze(tab, first_half);

    ans = maze.solve();

	std::cout << "Answer : " << ans << std::endl;
	return 0;
}
