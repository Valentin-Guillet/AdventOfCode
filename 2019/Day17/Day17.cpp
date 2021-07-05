
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
Pos directions[4] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};


class View
{
    private:
        std::vector<long> ascii_view;
        std::map<Pos, char> view;
        int sizeX = 0, sizeY = 0;
        int startX, startY;
        bool finished;

        void build() {
            int x = 0, y = 0;
            for (long value : ascii_view) {
                if (value == 10) {
                    sizeX = std::max(sizeX, x);
                    x = 0;
                    y++;
                } else {
                    if (value != 35 && value != 46) {
                        startX = x;
                        startY = y;
                    }
                    view[{x++, y}] = (char) value;
                }
            }
            sizeY = y;
        }

        std::pair<char, int> get_next_direction(int x, int y, int direction) {
            Pos left = directions[(direction+3) % 4];
            Pos right = directions[(direction+1) % 4];
            if (view[{x+left.first, y+left.second}] == '#') {
                return {'L', (direction+3) % 4};
            } else if (view[{x+right.first, y+right.second}] == '#') {
                return {'R', (direction+1) % 4};
            } else {
                return {' ', -1};
            }
        }

    public:
        bool is_finished() { return finished; }

        void add(long value) {
            if (value < 0) {
                finished = true;
                build();
            } else {
                ascii_view.push_back(value);
            }
        }

        int compute_alignment() {
            int ans = 0;
            for (int j=1; j<sizeY-1; ++j) {
                for (int i=1; i<sizeX-1; ++i) {
                    if (view[{i, j}] == '#'
                        && view[{i-1, j}] == '#' && view[{i+1, j}] == '#'
                        && view[{i, j-1}] == '#' && view[{i, j+1}] == '#')
                        ans += i * j;
                }
            }
            return ans;
        }

        void display() {
            std::cout << "Size : " << sizeX << ", " << sizeY << std::endl;
            for (int j=0; j<sizeY; ++j) {
                for (int i=0; i<sizeX; ++i) {
                    std::cout << view[{i, j}];
                }
                std::cout << std::endl;
            }
        }

        void disp_straight_way() {
            std::string output;

            int x = startX, y = startY;
            int direction = 0, value;
            std::pair<char, int> left_right;
            while (true)
            {
                left_right = get_next_direction(x, y, direction);
                direction = left_right.second;
                if (direction < 0)
                    break;

                output += left_right.first;
                output += ',';
                value = -1;
                Pos next_pos = {x, y};
                do {
                    x = next_pos.first;
                    y = next_pos.second;
                    value++;
                    next_pos = {x+directions[direction].first, y+directions[direction].second};
                } while (view[next_pos] == '#');
                output.append(std::to_string(value) + ',');
            }
            std::cout << "Output : " << output << std::endl;
        }
};

class Program
{
    long pos = 0, base_ref = 0;
    long pos_bck = 0, base_ref_bck = 0;
    int index = 0, index_bck = 0;
    std::vector<long> values;
    std::unordered_map<long, long> tab;

    public:
        Program(std::unordered_map<long, long> tab) : tab(tab) { }

        long run(std::vector<long> input={}, bool debug=false)
        {
            pos = pos_bck;
            base_ref = base_ref_bck;
            index = index_bck;
            if (!input.empty())
                values = input;
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
                        *arg1 = values[index++];
                        pos += 2;
                        break;

                    case 4:
                        pos += 2;
                        pos_bck = pos;
                        base_ref_bck = base_ref;
                        index_bck = index;
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
                        return -1;

                    default:
                        std::cout << "Invalid instruction !" << std::endl;
                        return -1;
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

	std::ifstream myfile ("Day17Input");
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



    if (first_half) {
        Program prog(tab);
        View view;

        int tile;
        do {
            tile = prog.run();
            view.add(tile);
        } while (!view.is_finished());

        view.display();
        view.disp_straight_way();
        ans = view.compute_alignment();

    } else {
        tab[0] = 2;
        Program prog(tab);

        std::string mov_routine = "A,B,A,C,A,B,C,B,C,B\n";
        std::string mov_function_A = "R,10,R,10,R,6,R,4\n";
        std::string mov_function_B = "R,10,R,10,L,4\n";
        std::string mov_function_C = "R,4,L,4,L,10,L,10\n";
        std::string video_feed = "n\n";

        std::string args = mov_routine + mov_function_A + mov_function_B + mov_function_C + video_feed;
        std::vector<long> args_long;
        for (char c : args)
            args_long.push_back((long) c);

        for (int i=0; i<28*42+1; ++i)
            std::cout << (char) prog.run();

        long out = prog.run(args_long);
        while (out >= 0) {
            if (out < 255)
                std::cout << (char) out;
            ans = out;
            out = prog.run();
        }
        std::cout << std::endl;
    }

	std::cout << "Answer : " << ans << std::endl;
	return 0;
}
