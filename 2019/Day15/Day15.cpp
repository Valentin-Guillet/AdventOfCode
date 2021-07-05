
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <unordered_map>
#include <set>
#include <map>


using Pos = std::pair<int, int>;

class Maze
{
    private:
        Pos robot = {0, 0};
        int minX = 0, maxX = 0;
        int minY = 0, maxY = 0;
        std::map<Pos, char> maze;
        std::stack<int> actions;
        int last_action;
        bool backtrack = false;

        void set_char(Pos pos, int v) {
            if (v == 0) {
                maze[{pos.first, pos.second}] = '#';
            } else if (v == 1) {
                maze[{pos.first, pos.second}] = '.';
            } else if (v == 2) {
                maze[{pos.first, pos.second}] = 'O';
            }
        }

        Pos get_next_cell(Pos position, int direction) {
            if (direction == 1) {
                position.second--;
            } else if (direction == 2) {
                position.second++;
            } else if (direction == 3) {
                position.first--;
            } else if (direction == 4) {
                position.first++;
            }
            return position;
        }

        Pos get_next_cell(int direction) {
            return get_next_cell(robot, direction);
        }

        void set_cell(int direction, int value) {
            Pos next_cell = get_next_cell(direction);
            set_char(next_cell, value);
            minX = std::min(minX, next_cell.first);
            maxX = std::max(maxX, next_cell.first);
            minY = std::min(minY, next_cell.second);
            maxY = std::max(maxY, next_cell.second);
        }

        int opposite(int action) {
            switch (action) {
                case 1:
                    return 2;
                case 2:
                    return 1;
                case 3:
                    return 4;
                case 4:
                    return 3;
                default:
                    return -1;
            }
        }

        bool valid(int direction) {
            Pos next_pos = get_next_cell(direction);
            return maze.find(next_pos) == maze.end();
        }

    public:
        Maze() { maze[{0, 0}] = 'X'; }

        bool is_complete() {
            return (maze.size() > 3 && robot.first == 0 && robot.second == 0);
        }

        int get_action() {
            for (int dir=1; dir<5; ++dir) {
                if (valid(dir)) {
                    backtrack = false;
                    last_action = dir;
                    return dir;
                }
            }

            backtrack = true;
            last_action = opposite(actions.top());
            actions.pop();
            return last_action;
        }

        void get_result(int input) {
            if (!backtrack) {
                set_cell(last_action, input);
                if (input != 0)
                    actions.push(last_action);
            }

            if (input != 0)
                robot = get_next_cell(last_action);
        }

        int get_dist() {
            std::queue<std::pair<Pos, int>> positions;
            positions.push({{0, 0}, 0});
            std::set<Pos> seen;

            while (!positions.empty()) {
                auto [curr_pos, value] = positions.front();
                positions.pop();
                seen.insert(curr_pos);

                if (maze[curr_pos] == 'O')
                    return value;

                for (int dir=1; dir<5; ++dir) {
                    Pos next_cell = get_next_cell(curr_pos, dir);
                    if (seen.find(next_cell) == seen.end() && maze[next_cell] != '#')
                        positions.push({next_cell, value+1});
                }
            }
            return -1;
        }

        int get_max_dist() {
            Pos oxygen;
            for (const auto& [pos, value] : maze) {
                if (value == 'O')
                    oxygen = pos;
            }

            int diameter = 0;
            std::queue<std::pair<Pos, int>> positions;
            positions.push({oxygen, 0});
            std::set<Pos> seen;

            while (!positions.empty()) {
                auto [curr_pos, value] = positions.front();
                positions.pop();
                seen.insert(curr_pos);

                diameter = std::max(diameter, value);

                for (int dir=1; dir<5; ++dir) {
                    Pos next_cell = get_next_cell(curr_pos, dir);
                    if (seen.find(next_cell) == seen.end() && maze[next_cell] != '#')
                        positions.push({next_cell, value+1});
                }
            }

            return diameter;
        }


        void print() {
            usleep(500);
            system("clear");
            for (int j=minY-2; j<maxY+3; ++j) {
                for (int i=minX-2; i<maxX+3; ++i) {
                    if (i == robot.first && j == robot.second) {
                        std::cout << 'R';
                    } else {
                        if (maze.find({i, j}) == maze.end()) {
                            std::cout << " ";
                        } else {
                            std::cout << maze[{i, j}];
                        }
                    }
                }
                std::cout << std::endl;
            }
        }
};


class Program
{
    long pos = 0, base_ref = 0;
    long pos_bck = 0, base_ref_bck = 0;
    std::unordered_map<long, long> tab;

    public:
        Program(std::unordered_map<long, long> tab) : tab(tab) { }

        long run(long input=0, bool debug=false)
        {
            pos = pos_bck;
            base_ref = base_ref_bck;
            while (true)
            {
                int instruction = tab[pos] % 100;
                int modes = tab[pos] / 100;
                long *arg1, *arg2, *arg3;

                if (instruction != 99) {
                    if (modes % 10 == 0) {
                        arg1 = &tab[tab[pos+1]];
                    } else if (modes % 10 == 1) {
                        arg1 = &tab[pos+1];
                    } else if (modes % 10 == 2) {
                        arg1 = &tab[base_ref + tab[pos+1]];
                    }

                    if ((modes/10) % 10 == 0) {
                        arg2 = &tab[tab[pos+2]];
                    } else if ((modes/10) % 10 == 1) {
                        arg2 = &tab[pos+2];
                    } else if ((modes/10) % 10 == 2) {
                        arg2 = &tab[base_ref + tab[pos+2]];
                    }

                    if ((modes/100) % 10 == 0) {
                        arg3 = &tab[tab[pos+3]];
                    } else if ((modes/100) % 10 == 2) {
                        arg3 = &tab[base_ref + tab[pos+3]];
                    }
                }

                if (debug) {
                    std::cout << "Position : " << pos << ", tab[pos] = " << tab[pos] << ", base_ref = " << base_ref << std::endl;
                    std::cout << "Args : " << *arg1 << ", " << *arg2 << ", " << *arg3 << std::endl;
                }

                switch (instruction) {
                    case 1:
                        *arg3 = *arg1 + *arg2;
                        pos += 4;
                        break;

                    case 2:
                        *arg3 = *arg1 * *arg2;
                        pos += 4;
                        break;

                    case 3:
                        *arg1 = input;
                        pos += 2;
                        break;

                    case 4:
                        pos += 2;
                        pos_bck = pos;
                        base_ref_bck = base_ref;
                        return *arg1;
                        break;

                    case 5:
                        if (*arg1 != 0) {
                            pos = *arg2;
                        } else {
                            pos += 3;
                        }
                        break;

                    case 6:
                        if (*arg1 == 0) {
                            pos = *arg2;
                        } else {
                            pos += 3;
                        }
                        break;

                    case 7:
                        *arg3 = (long) (*arg1 < *arg2);
                        pos += 4;
                        break;

                    case 8:
                        *arg3 = (long) (*arg1 == *arg2);
                        pos += 4;
                        break;

                    case 9:
                        base_ref += *arg1;
                        pos += 2;
                        break;

                    case 99:
                        return {};

                    default:
                        std::cout << "Invalid instruction !" << std::endl;
                        return {};
                }

                if (debug)
                {
                    for (auto elt : tab)
                        std::cout << "A" << elt.first << "/V" << elt.second << ", ";
                    std::cout << std::endl;
                    std::cout << "Args : " << *arg1 << ", " << *arg2 << ", " << *arg3 << std::endl << std::endl;
                }
            }
        }
};

int main(void)
{
    bool first_half = false;
    int ans = 0;
	std::unordered_map<long, long> tab;

	std::ifstream myfile ("Day15Input");
	if (myfile.is_open()) {
        std::string input;

        int i = 0;
		while (getline(myfile, input, ','))
            tab[i++] = std::stol(input);

		myfile.close();
	} else {
        std::cout << "Unable to open file";
        return 1;
    }

    Program prog(tab);
    Maze maze;

    int status, action;
    while (!maze.is_complete()) {
        action = maze.get_action();
        status = prog.run(action);
        maze.get_result(status);
        // maze.print();
    }
    std::cout << "Maze built !" << std::endl;

    if (first_half) {
        ans = maze.get_dist();
    } else {
        ans = maze.get_max_dist();
    }

	std::cout << "Answer : " << ans << std::endl;
	return 0;
}
