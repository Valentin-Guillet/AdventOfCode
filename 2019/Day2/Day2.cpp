
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

int run_program(std::vector<int> tab, bool debug=false)
{
    int pos = 0;
    while (tab[pos] == 1 || tab[pos] == 2)
    {
        switch (tab[pos]) {
            case 1:
                tab[tab[pos+3]] = tab[tab[pos+1]] + tab[tab[pos+2]];
                pos += 4;
                break;

            case 2:
                tab[tab[pos+3]] = tab[tab[pos+1]] * tab[tab[pos+2]];
                pos += 4;
                break;

            case 99:
                std::cout << "99: execution finished !" << std::endl;
                break;

            default:
                std::cout << "Invalid !" << std::endl;
                return 1;
        }

        if (debug)
        {
            for (int value : tab)
                std::cout << value << ", ";
            std::cout << std::endl;
            std::cout << "Position : " << pos << ", tab[pos] = " << tab[pos] << std::endl;
        }
    }
    return tab[0];
}


int main(void)
{
    bool firstHalf = false;
	std::string input;
	int ans = 0;
	std::vector<int> tab, tab2;

	std::ifstream myfile ("Day2Input");
	if (myfile.is_open())
	{
		while (getline(myfile, input, ','))
            tab.push_back(std::stoi(input));

		myfile.close();
	} else {
        std::cout << "Unable to open file";
        return 1;
    }

    if (firstHalf)
    {
        ans = run_program(tab);
    } else {
        bool found = false;
        
        for (int noun = 0; noun < 100; ++noun) {
            for (int verb = 0; verb < 100; ++verb) {
                tab2 = tab;
                tab2[1] = noun;
                tab2[2] = verb;
                if (run_program(tab2) == 19690720) {
                    found = true;
                    ans = 100 * noun + verb;
                    break;
                }
            }
            if (found)
                break;
        }
    }

    std::cout << "Answer : " << ans << std::endl;
	return 0;
}
