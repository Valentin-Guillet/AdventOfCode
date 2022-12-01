
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>


struct pair_hash {
    inline std::size_t operator()(const std::pair<long, long> & v) const {
        std::hash<long> long_hasher;
        return long_hasher(v.first) ^ long_hasher(v.second);
    }
};

class Program
{
    long pos = 0, base_ref = 0;
    std::unordered_map<long, long> tab;

    public:
        Program(std::unordered_map<long, long> tab) : tab(tab) { }

        std::pair<long, long> run(long input, bool debug=false)
        {
            std::pair<long, long> outputs = {-1, -1};
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
                        if (outputs.first == -1) {
                            outputs.first = *arg1;
                        } else {
                            outputs.second = *arg1;
                            return outputs;
                        }
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
                        return {-1, -1};

                    default:
                        std::cout << "Invalid instruction !" << std::endl;
                        return {-1, -1};
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


void disp_map(std::unordered_set<std::pair<long, long>, pair_hash> surface, int posx, int posy, int direction)
{
    long minX = posx, maxX = posx;
    long minY = posy, maxY = posy;

    for (std::pair<long, long> elt : surface) {
        if (elt.first < minX)
            minX = elt.first;
        if (elt.first > maxX)
            maxX = elt.first;
        if (elt.second < minY)
            minY = elt.second;
        if (elt.second > maxY)
            maxY = elt.second;
    }

    for (int j=minY-2; j<maxY+3; ++j) {
        for (int i=minX-2; i<maxX+3; ++i) {
            char c;
            if (i == posx && j == posy) {
                if (direction == 0)
                    c = '^';
                if (direction == 1)
                    c = '>';
                if (direction == 2)
                    c = 'v';
                if (direction == 3)
                    c = '<';
            } else {
                c = (surface.find({i, j}) != surface.end() ? '#' : '.');
            }
            std::cout << c;
        }
        std::cout << std::endl;
    }
}


int main(void)
{
    int ans;
	std::unordered_map<long, long> tab;

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

    Program prog(tab);
    std::unordered_set<std::pair<long, long>, pair_hash> surface, seen;

    long in = 1;
    int posx = 0, posy = 0, direction = 0;

    surface.insert({posx, posy});
    while (true)
    {
        auto [out1, out2] = prog.run(in);

        if (out1 == -1)
            break;

        seen.insert({posx, posy});

        if (out1) {
            surface.insert({posx, posy});
        } else if (surface.find({posx, posy}) != surface.end()) {
            surface.erase({posx, posy});
        }

        direction = (direction + (out2 ? 1 : 3)) % 4;
        switch (direction) {
            case 0:
                posy--;
                break;
            case 1:
                posx++;
                break;
            case 2:
                posy++;
                break;
            case 3:
                posx--;
                break;
        }

        in = (int) (surface.find({posx, posy}) != surface.end());
    }

    disp_map(surface, posx, posy, direction);

    ans = seen.size();

	std::cout << "Answer : " << ans << std::endl;
	return 0;
}
