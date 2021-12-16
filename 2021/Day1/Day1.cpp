
#include <iostream>
#include <fstream>
#include <string>
#include <vector>


int main(int argc, char* argv[])
{
    bool first_half = true;
	int ans = 0;
    std::vector<int> tab;

    std::ifstream myfile("Day1Input");
    for (int i=1; i<argc; i++) {
        std::string arg(argv[i]);
        if (arg == "-t" || arg == "--test")
            myfile = std::ifstream("testInput");

        else if (arg == "-s" || arg == "--second")
            first_half = false;
    }

	if (myfile.is_open()) {
        std::string input;

		while (getline(myfile, input))
            tab.push_back(std::stoi(input));

		myfile.close();
	} else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    if (first_half) {
        for (size_t i=0; i<tab.size()-1; i++)
            ans += (tab[i+1] > tab[i]);
    } else {
        int prev_sum = tab[0] + tab[1] + tab[2];
        for (size_t i=0; i<tab.size()-3; i++) {
            int curr_sum = prev_sum - tab[i] + tab[i+3];
            ans += (curr_sum > prev_sum);
            prev_sum = curr_sum;
        }
    }

    std::cout << "Answer : " << ans << std::endl;
	return 0;
}
