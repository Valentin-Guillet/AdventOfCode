
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>
#include <algorithm>


using tuplet = std::tuple<std::vector<int>, int, int, int>;

tuplet run_program(std::vector<int> tab, int pos, int nb_inputs, int phase, int input, bool debug=false)
{
    bool done = false;
    while (!done)
    {
        int instruction = tab[pos] % 100;
        int modes = tab[pos] / 100;
        int arg1, arg2;

        if (instruction != 99) {
            arg1 = (modes % 10 == 0) ? tab[tab[pos+1]] : tab[pos+1];
            arg2 = ((modes/10) % 10 == 0) ? tab[tab[pos+2]] : tab[pos+2];
        }

        if (debug)
            std::cout << "Instr : " << instruction << std::endl;

        switch (instruction) {
            case 1:
                tab[tab[pos+3]] = arg1 + arg2;
                pos += 4;
                break;

            case 2:
                tab[tab[pos+3]] = arg1 * arg2;
                pos += 4;
                break;

            case 3:
                if (nb_inputs == 0) {
                    tab[tab[pos+1]] = phase;
                } else {
                    tab[tab[pos+1]] = input;
                }
                nb_inputs++;
                pos += 2;
                break;

            case 4:
                return {tab, tab[tab[pos+1]], pos+2, nb_inputs};
                break;
            
            case 5:
                if (arg1 != 0) {
                    pos = arg2;
                } else {
                    pos += 3;
                }
                break;

            case 6:
                if (arg1 == 0) {
                    pos = arg2;
                } else {
                    pos += 3;
                }
                break;

            case 7:
                tab[tab[pos+3]] = (int) (arg1 < arg2);
                pos += 4;
                break;

            case 8:
                tab[tab[pos+3]] = (int) (arg1 == arg2);
                pos += 4;
                break;

            case 99:
                done = true;
                break;

            default:
                std::cout << "Invalid instruction !" << std::endl;
                return {};
        }

        if (debug)
        {
            for (int value : tab)
                std::cout << value << ", ";
            std::cout << std::endl;
            std::cout << "Position : " << pos << ", tab[pos] = " << tab[pos] << std::endl;
        }
    }
    return {{}, -1, -1, -1};
}


int main(void)
{
	std::string ans;
	std::vector<int> tab;

	std::ifstream myfile ("Day7Input");
	if (myfile.is_open()) {
        std::string input;

		while (getline(myfile, input, ','))
            tab.push_back(std::stoi(input));

		myfile.close();
	} else {
        std::cout << "Unable to open file";
        return 1;
    }

    std::vector<int> tabs[5] = {tab, tab, tab, tab, tab};
    int phases[5] = {5, 6, 7, 8, 9};
    int pos[5];
    int nb_inputs[5];

    tuplet out;
    int max=0, output, last_output;
    do {
        std::fill(nb_inputs, nb_inputs+5, 0);
        std::fill(pos, pos+5, 0);
        std::fill(tabs, tabs+5, tab);
        output = 0;
        last_output = 0;
        while (output != -1) {
            for (int i=0; i<5; ++i) {
                out = run_program(tabs[i], pos[i], nb_inputs[i], phases[i], output);
                tabs[i] = std::get<0>(out);
                output = std::get<1>(out);
                if (i == 4 && output != -1)
                    last_output = output;
                pos[i] = std::get<2>(out);
                nb_inputs[i] = std::get<3>(out);
            }
        }

        if (last_output > max) {
            max = last_output;
            ans = std::to_string(phases[0]) + std::to_string(phases[1]) + std::to_string(phases[2]) + \
                    std::to_string(phases[3]) + std::to_string(phases[4]);
        }

    } while (std::next_permutation(phases, phases+5));

    std::cout << "Max : " << max << std::endl; 
    std::cout << "Answer : " << ans << std::endl; 
	return 0;
}
