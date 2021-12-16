
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


int compute_rating(std::vector<std::string> tab, bool most=false) {
    int i = 0;
    while (tab.size() > 1) {
        int count = 0;
        for (std::string line : tab)
            count += (line[i] == '1');

        char most_bit;
        if (count == tab.size() / 2.0)
            most_bit = (most ? '1' : '0');
        else
            most_bit = ((count > tab.size() / 2.0) == most) ? '1' : '0';

        tab.erase(std::remove_if(tab.begin(), tab.end(),
                                 [i, most_bit](std::string line) { return line[i] != most_bit; }),
                  tab.end());
        i++;
    }

    int ans = 0;
    for (char bit : tab[0])
        ans = 2*ans + (bit == '1');
    return ans;
}


int main(int argc, char* argv[])
{
    bool first_half = true;
	int ans = 0;
    std::vector<std::string> tab;

    std::ifstream myfile("Day3Input");
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
            tab.push_back(input);

		myfile.close();
	} else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    int most = 0, least = 0;
    if (first_half) {
        for (size_t i=0; i<tab[0].size(); i++) {
            int count = 0;
            for (std::string line : tab)
                count += (line[i] == '1');
            most = 2*most + (count > tab.size() / 2.0);
            least = 2*least + (count < tab.size() / 2.0);
        }

    } else {
        most = compute_rating(tab, true);
        least = compute_rating(tab, false);
    }
    ans = most * least;

    std::cout << "Answer : " << ans << std::endl;
	return 0;
}
