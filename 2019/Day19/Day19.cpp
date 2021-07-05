
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <string>
#include <unordered_map>


class Program
{
    std::unordered_map<long, long> tape;

    public:
        Program(std::unordered_map<long, long> tab) : tape(tab) {
        }

        long run(long inputX, long inputY, bool debug=false)
        {
            std::unordered_map<long, long> tab = tape;
            int pos = 0, base_ref = 0;
            bool first_coord = true;
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
                        if (first_coord) {
                            *arg1 = inputX;
                        } else {
                            *arg1 = inputY;
                        }
                        first_coord = false;

                        pos += 2;
                        break;

                    case 4:
                        pos += 2;
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

std::pair<long, long> get_line_value(Program prog, long y)
{
    long init_x = y / 2;

    long x = init_x;
    while (prog.run(++x, y) == 0) { }

    init_x = x;
    while (prog.run(++x, y) == 1) { }

    return {init_x, x-1};
}

long get_down_value(Program prog, long x, long y)
{
    long init_y = y;
    while (prog.run(x, ++y) == 1) { }
    return y - init_y;
}

long find_by_dichotomy(Program prog, long target)
{
    long y1 = 0;
    long y2 = 4000;

    while (y1 != y2 && y1 != y2-1) {
        std::pair<long, long> value = get_line_value(prog, (y1+y2) / 2);
        if (value.second - value.first < target) {
            y1 = (y1 + y2) / 2;
        } else {
            y2 = (y1 + y2) / 2;
        }
    }

    return y1;
}

int main(void)
{
    bool first_half = false;
    int ans = 0;
	std::unordered_map<long, long> tab;

	std::ifstream myfile ("Day19Input");
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

    if (first_half) {
        long output;
        for (long y=0; y<50; ++y) {
            for (long x=0; x<50; ++x) {
                output = prog.run(x, y);
                std::cout << output;
                ans += output;
            }
            std::cout << std::endl;
        }
    } else {
        std::pair<long, long> values = get_line_value(prog, 1000);
        std::cout << "Line 1000 start at " << values.first << " and end at " << values.second << std::endl;

        double angle1 = 1000.0 / values.first;
        double angle2 = 1000.0 / values.second;

        std::cout << "First line : " << angle1 << " high for 1 right" << std::endl;
        std::cout << "Second line : " << angle2 << " high for 1 right" << std::endl;

        int probable_line_length = 100 + 100 / angle1;
        std::cout << "Approximative line length : " << probable_line_length << std::endl;

        long line_target = find_by_dichotomy(prog, probable_line_length) - 50;
        std::cout << "Line of value " << probable_line_length << " : " << line_target << std::endl;

        values = get_line_value(prog, line_target);
        long x = values.second - 200;
        long y = line_target;

        bool valid = false;
        while (!valid) {
            std::cout << "x = " << x << ", y = " << y << std::endl;
            long value_right = get_line_value(prog, y).second - x;
            long value_down  = get_down_value(prog, x, y) - 1;
            if (value_right < 99) {
                y++;
            } else if (value_down < 99) {
                x++;
            } else {
                valid = true;
            }
        }
        std::cout << "Final x = " << x << ", y = " << y << std::endl;

        ans = 10000 * x + y;
    }

	std::cout << "Answer : " << ans << std::endl;
	return 0;
}
