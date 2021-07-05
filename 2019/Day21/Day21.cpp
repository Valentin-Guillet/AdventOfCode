
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

int run(std::unordered_map<long, long> tab, std::string script)
{
    int ans = -1;
    std::vector<long> args_long;
    for (char c : script)
        args_long.push_back((long) c);

    Program prog(tab);
    long out = prog.run(args_long);
    while (out >= 0) {
        if (out < 255) {
            std::cout << (char) out;
        } else {
            ans = out;
        }
        out = prog.run();
    }
    std::cout << std::endl;

    return ans;
}

int main(void)
{
    bool first_half = false;
    int ans = 0;
	std::unordered_map<long, long> tab;

	std::ifstream myfile ("Day21Input");
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
        std::string script;
        script.append("NOT C J\n");
        script.append("AND D J\n");
        script.append("NOT A T\n");
        script.append("OR T J\n");
        script.append("WALK\n");
        ans = run(tab, script);
    } else {
        std::string script;

        script.append("NOT C J\n");
        script.append("AND H J\n");
        script.append("OR E J\n");
        script.append("NOT B T\n");
        script.append("OR T J\n");
        script.append("AND D J\n");
        script.append("NOT A T\n");
        script.append("OR T J\n");

        script.append("RUN\n");
        ans = run(tab, script);
    }

	std::cout << "Answer : " << ans << std::endl;
	return 0;
}
