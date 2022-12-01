
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <string>
#include <vector>
#include <map>


using Pos = std::pair<int, int>;

class Display
{
    private:
        int sizeX, sizeY;
        int minX, minY;
        int pad_pos, ball_pos;
        std::map<Pos, char> display;
        int score = 0;

        void set_char(int x, int y, int v) {
            if (x == -1 && y == 0) {
                score = v;
                return;
            }
            if (v == 0) {
                display[{x-minX, y-minY}] = ' ';
            } else if (v == 1) {
                display[{x-minX, y-minY}] = '+';
            } else if (v == 2) {
                display[{x-minX, y-minY}] = '#';
            } else if (v == 3) {
                display[{x-minX, y-minY}] = '_';
                pad_pos = x-minX;
            } else if (v == 4) {
                display[{x-minX, y-minY}] = 'O';
                ball_pos = x-minX;
            }
        }

    public:
        Display(int sizeX, int sizeY, int minX, int minY) :
            sizeX(sizeX), sizeY(sizeY), minX(minX), minY(minY) {
            for (int j=0; j<sizeY; ++j) {
                for (int i=0; i<sizeX; ++i) {
                    display[{i, j}] = ' ';
                }
            }
        }

        int get_score() { return score; }

        void set_display(std::vector<long> outs) {
            for (unsigned long i=0; i<outs.size(); i+=3) {
                set_char((int) outs[i], (int) outs[i+1], (int) outs[i+2]);
            }
        }

        int get_action() {
            if (pad_pos < ball_pos) {
                return 1;
            } else if (pad_pos > ball_pos) {
                return -1;
            } else {
                return 0;
            }
        }

        void print() {
            usleep(5000);
            system("clear");
            for (int j=0; j<sizeY; ++j) {
                for (int i=0; i<sizeX; ++i)
                    std::cout << display[{i, j}];
                std::cout << std::endl;
            }
            std::cout << "SCORE : " << score << std::endl;
        }
};


class Program
{
    long pos = 0, base_ref = 0;
    long pos_bck = 0, base_ref_bck = 0;
    bool saved = false, finished = false;
    std::map<long, long> tab;

    public:
        Program(std::map<long, long> tab) : tab(tab) { }
        bool is_finished() { return finished; }

        std::vector<long> run(long input=0, bool debug=false)
        {
            std::vector<long> outputs;
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
                        if (!saved) {
                            pos_bck = pos;
                            base_ref_bck = base_ref;
                            saved = true;
                            return outputs;
                        } else {
                            pos = pos_bck;
                            base_ref = base_ref_bck;
                            saved = false;
                        }

                        *arg1 = input;
                        pos += 2;
                        break;

                    case 4:
                        outputs.push_back(*arg1);
                        pos += 2;
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
                        finished = true;
                        return outputs;

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

Display init_display(std::vector<long> out)
{
    for (unsigned long i=0; i<out.size(); i+=3) {
        if (out[i] == -1 && out[i+1] == 0) {
            out.erase(out.begin()+i, out.begin()+i+3);
            i -= 3;
        }
    }

    long minX = 1000, maxX = 0;
    long minY = 1000, maxY = 0;
    for (unsigned long i=0; i<out.size(); i+=3) {
        if (out[i] < minX)
            minX = out[i];
        if (maxX < out[i])
            maxX = out[i];
        if (out[i+1] < minY)
            minY = out[i+1];
        if (maxY < out[i+1])
            maxY = out[i+1];
    }

    int sizeX = maxX - minX + 1;
    int sizeY = maxY - minY + 1;

    Display display(sizeX, sizeY, minX, minY);
    display.set_display(out);
    return display;
}


int main(void)
{
    bool first_half = false;
    int ans = 0;
	std::map<long, long> tab;

	std::ifstream myfile ("input");
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

    if (!first_half)
        tab[0] = 2;

    Program prog(tab);
    std::vector<long> out;

    out = prog.run();
    Display arcade = init_display(out);

    if (first_half) {
        arcade.print();
        for (unsigned long i=0; i<out.size(); i+=3) {
            if (out[i+2] == 2)
                ans++;
        }

    } else {

        while (!prog.is_finished())
        {
            out = prog.run(arcade.get_action());
            arcade.set_display(out);
            arcade.print();
        }
        ans = arcade.get_score();
    }

	std::cout << "Answer : " << ans << std::endl;
	return 0;
}
