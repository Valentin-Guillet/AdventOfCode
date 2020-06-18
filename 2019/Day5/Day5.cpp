
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

void run_program(std::vector<int> tab, bool debug=false)
{
    int pos = 0;
    bool done;
    while (!done)
    {
        int instruction = tab[pos] % 100;
        int modes = tab[pos] / 100;
        int arg1, arg2;
        arg1 = (modes % 10 == 0) ? tab[tab[pos+1]] : tab[pos+1];
        arg2 = ((modes/10) % 10 == 0) ? tab[tab[pos+2]] : tab[pos+2];

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
                std::cin >> tab[tab[pos+1]];
                pos += 2;
                break;

            case 4:
                std::cout << tab[tab[pos+1]] << std::endl;
                pos += 2;
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
                std::cout << "Invalid !" << std::endl;
                return;
        }

        if (debug)
        {
            for (int value : tab)
                std::cout << value << ", ";
            std::cout << std::endl;
            std::cout << "Position : " << pos << ", tab[pos] = " << tab[pos] << std::endl;
        }
    }
}


int main(void)
{
	std::vector<int> tab;

	std::ifstream myfile ("Day5Input");
	if (myfile.is_open()) {
        std::string input;

		while (getline(myfile, input, ','))
            tab.push_back(std::stoi(input));

		myfile.close();
	} else {
        std::cout << "Unable to open file";
        return 1;
    }

    run_program(tab, false);

	return 0;
}
